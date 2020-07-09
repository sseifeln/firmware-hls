#include "InputRouterTop.h"


void InputRouterGeneric( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<TRACKER> hMemories[20])
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS stream variable=hStubs depth=1 
		

	ap_uint<3> hNLayers = hLinkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-3);
	ap_uint<1> hIs2S = hLinkWord.range(kLINKMAPwidth-3,kLINKMAPwidth-4);
	
	//local variable 
	//InputStubMemory<TRACKER>* local_memories = hMemories;

	// prepare variable needed 
	// to be able to move through 
	// the array of memories 
	unsigned int cNMemories=0; 
	// hard code things for now 
	constexpr unsigned int kNLayers=4;
	ap_uint<4> hNPhiBns[kNLayers]; //at most 4 layers
	ap_uint<1> hBrlBits[kNLayers]; //at most 4 layers

	#pragma HLS array_partition variable=hNPhiBns complete
	#pragma HLS array_partition variable=hBrlBits complete
	LOOP_GetNPhiBns : 
	for( unsigned int cIndx=0; cIndx < kNLayers; cIndx++)
	{
		#pragma HLS unroll 
		if( cIndx < hNLayers )
		{
			ap_uint<4> hWrd = hLinkWord.range(4*cIndx+3,4*cIndx);
			ap_uint<1> hIsBrl = hWrd.range(1,0);
			ap_uint<3> hLyrId = hWrd.range(3,1);
			hNPhiBns[cIndx] = ( (hIs2S==0) && hLyrId==1 && hIsBrl) ? 8 : 4; 
			hBrlBits[cIndx] = hIsBrl;
			//cNPhiBns[cIndx] = (!cIs2S && hLyrId==1 && hIsBrl) ? 8 : 4; 
			//cBrlBits[cIndx] = hIsBrl;
			#ifndef __SYNTHESIS__
				if( IR_DEBUG )
				{
					std::cout << "Lyr#" << cIndx
						<< " encoded word " << std::bitset<4>(hWrd)
						<< " - " <<  hNPhiBns[cIndx]
						<< " phi bins"
						<< "\n"; 
				}
			#endif
			cNMemories += (unsigned int)(hNPhiBns[cIndx]);
		}
	}
	// clear memories and stub counter
	constexpr unsigned int kNMemories=20;
	ap_uint<8> hNStubs[kNMemories];
	#pragma HLS array_partition variable=hNStubs complete
	LOOP_ClearOutputMemories : 
	for( unsigned int cMemIndx=0; cMemIndx<kNMemories; cMemIndx++)
	{
		// clear four phi regions at a time ..
		#pragma HLS unroll 
		hNStubs[cMemIndx]=0;
		(&hMemories[cMemIndx])->clear(0);
		//local_memories[cMemIndx].clear(0);
	}
	
	ap_uint<kBRAMwidth> hEmpty=ap_uint<kBRAMwidth>(0); 
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		// decode stub 
		// check which memory 
		ap_uint<kNBits_DTC> hStub = hStubs[cStubCounter]; 
		if( hStub == 0 )
			continue;

		ap_uint<3> hEncLyr = ap_uint<3>(hStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3) ;
		ap_uint<kBRAMwidth> hStbWrd = hStub.range(kBRAMwidth-1,0);
		//get 36 bit word 
		InputStub<TRACKER> hMemWord(hStbWrd);
		//decode link wrd for this layer 
		ap_uint<4> hWrd = hLinkWord.range(4*hEncLyr+3,4*hEncLyr);
		ap_uint<1> hIsBrl = hWrd.range(1,0);
		ap_uint<3> hLyrId = hWrd.range(3,1);
		//get phi bin 
		ap_uint<3> hPhiBn;
		if( (hIs2S==0) && hIsBrl ) 
			GetPhiBin<BARRELPS>(hStub, hLyrId, hPhiBn);
		else if( (hIs2S==0) && !hIsBrl )
			GetPhiBin<DISKPS>(hStub, hLyrId, hPhiBn);
		else if( hIsBrl )
			GetPhiBin<BARREL2S>(hStub, hLyrId, hPhiBn);
		else
			GetPhiBin<DISK2S>(hStub, hLyrId, hPhiBn);
		
		//update index 
		unsigned int cIndx=0;
		//unsigned int cMemIndx=0;
		//unsigned int cBaseIndx=cMemIndx;
		LOOP_UpdateIndxLoop :
		for( int cLyr=0 ; cLyr<kNLayers;cLyr++)
		{
			#pragma HLS unroll 
			//update index 
			cIndx += (cLyr >= hEncLyr ) ? 0 : (unsigned int)(hNPhiBns[cLyr]);
		}
		//write to memory 
		unsigned int cMemIndx = cIndx+hPhiBn;
		ap_uint<8> hEntries = hNStubs[cMemIndx];
		#ifndef __SYNTHESIS__
			if( IR_DEBUG )
			{
				std::cout << "\t.. Stub : " << std::hex 
					<< hStbWrd
					<< std::dec 
					<< "[ LyrId " << hLyrId 
					<< " ] IsBrl bit " << +hIsBrl
					<< " PhiBn#" << +hPhiBn 
					<< " Mem#" << cMemIndx 
					<< " Current number of entries " << +hEntries 
					<< "\n"; 
			}
		#endif
		(&hMemories[cMemIndx])->write_mem(0,hMemWord,hEntries);
		//(&local_memories[cIndx])->write_mem(bx,hMemWord,hEntries);
		hNStubs[cMemIndx]=hEntries+1;
	}
}
void InputRouter_2S_1Barrel1Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, AllStubMemory<BARREL2S> L1[4]
	, AllStubMemory<DISK2S> L2[4])
{
	
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS stream variable=hStubs depth=1 
	ap_uint<8> n1[4];
	ap_uint<8> n2[4];
	#pragma HLS array_partition variable=n1 complete
	#pragma HLS array_partition variable=n2 complete
	
	LOOP_ClearInputStubs : 
	for( int cPhiBn=0; cPhiBn<4; cPhiBn++)
	{
		// clear four phi regions at a time ..
		#pragma HLS unroll 
		n1[cPhiBn]=0;
		n2[cPhiBn]=0;
	}

	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if( hStubs[cStubCounter] != 0 )
		{
			EnLRouter2L<BARREL2S,DISK2S,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2, L1, L2 );
		}
	}
}


void InputRouter_PS_1Barrel3Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, AllStubMemory<BARRELPS> L1[8]
	, AllStubMemory<DISKPS> L2[4]
	, AllStubMemory<DISKPS> L3[4]
	, AllStubMemory<DISKPS> L4[4])
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS stream variable=hStubs depth=1 
	ap_uint<8> n1[8];
	ap_uint<8> n2[4];
	ap_uint<8> n3[4];
	ap_uint<8> n4[4];
	#pragma HLS array_partition variable=n1 complete
	#pragma HLS array_partition variable=n2 complete
	#pragma HLS array_partition variable=n3 complete
	#pragma HLS array_partition variable=n4 complete
	LOOP_ClearInputStubs : 
	for( int cPhiBn=0; cPhiBn<8; cPhiBn++)
	{
		// clear four phi regions at a time ..
		#pragma HLS unroll 
		n1[cPhiBn]=0;
		(&L1[cPhiBn])->clear(bx);
		if( cPhiBn < 4 )
		{
			n2[cPhiBn]=0;
			n3[cPhiBn]=0;
			n4[cPhiBn]=0;
			(&L2[cPhiBn])->clear(bx);
			(&L3[cPhiBn])->clear(bx);
			(&L4[cPhiBn])->clear(bx);
		}
	}

	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if( hStubs[cStubCounter] != 0 )
		{
			EnLRouter4L<BARRELPS,DISKPS,DISKPS,DISKPS,8,4,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2,n3,n4, L1, L2,L3,L4 );
		}
	}
}
