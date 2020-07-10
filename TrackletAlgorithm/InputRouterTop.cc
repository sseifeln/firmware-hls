#include "InputRouterTop.h"


void InputRouterTop( const ap_uint<6> hLinkId 
	, const ap_uint<kLINKMAPwidth> hLinkTable[24] 
	, const int kPhiCorrtable_L1[64]
	, const int kPhiCorrtable_L2[64]
	, const int kPhiCorrtable_L3[64]
	, const int kPhiCorrtable_L4[128]
	, const int kPhiCorrtable_L5[128]
	, const int kPhiCorrtable_L6[128]
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, InputStubMemory<TRACKER> hMemories[20])
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkId
	#pragma HLS interface ap_none port=hLinkTable
	#pragma HLS stream variable=hStubs depth=1 
		
	ap_uint<kLINKMAPwidth> hLinkWord=hLinkTable[hLinkId%24];
	ap_uint<3> hNLayers = hLinkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-3);
	ap_uint<1> hIs2S = hLinkWord.range(kLINKMAPwidth-3,kLINKMAPwidth-4);
	
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
		if( hIsBrl==1 )
		{
			if( hIs2S == 0 ) 
				GetPhiBinBrl<BARRELPS,64>(hStub, kPhiCorrtable_L1, kPhiCorrtable_L2, kPhiCorrtable_L3, hLyrId, hPhiBn);
			else
				GetPhiBinBrl<BARREL2S,128>(hStub, kPhiCorrtable_L1, kPhiCorrtable_L2, kPhiCorrtable_L3, hLyrId, hPhiBn);
		}
		else 
		{
			if( hIs2S == 0 ) 
				GetPhiBinDsk<DISKPS>(hStub, hLyrId, hPhiBn);
			else
				GetPhiBinDsk<DISK2S>(hStub, hLyrId, hPhiBn);
		}
		// if( (hIs2S==0) && hIsBrl ) 
		// {
		// 	GetPhiBinBrl<BARRELPS,64>(hStub, kPhiCorrtable_L1, kPhiCorrtable_L2, kPhiCorrtable_L3, hLyrId, hPhiBn);
		// }
		// else if( (hIs2S==0) && !hIsBrl )
		// {
		// 	GetPhiBinDsk<DISKPS>(hStub, hLyrId, hPhiBn);
		// }
		// else if( hIsBrl )
		// {
		// 	GetPhiBinBrl<BARREL2S,128>(hStub, kPhiCorrtable_L4, kPhiCorrtable_L5, kPhiCorrtable_L6, hLyrId, hPhiBn);
		// }
		// else
		// {
		// 	GetPhiBinDsk<DISK2S>(hStub, hLyrId, hPhiBn);
		// }
		
		//update index 
		unsigned int cIndx=0;
		LOOP_UpdateIndxLoop :
		for( int cLyr=0 ; cLyr<kNLayers;cLyr++)
		{
			#pragma HLS unroll 
			//update index 
			cIndx += (cLyr < hEncLyr ) ? (unsigned int)(hNPhiBns[cLyr]) : 0;
		}
		//write to memory 
		unsigned int cMemIndx = cIndx+hPhiBn;
		assert(cMemIndx<cNMemories);
		//get entries 
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
		hNStubs[cMemIndx]=hEntries+1;
	}
}