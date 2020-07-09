#include "InputRouterTop.h"


void InputRouterTop( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, const ap_uint<5> hNMemories  
	, const ap_uint<3> hNLayers  
	, AllStubMemory<TRACKER> *hMemories)
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS interface ap_none port=hNMemories
	#pragma HLS interface ap_none port=hNLayers
	#pragma HLS stream variable=hStubs depth=1 
	

	ap_uint<8> *nStubs = new ap_uint<8>[hNMemories];
	LOOP_ClearOutputMemories : 
	for( int cMemIndx=0; cMemIndx<hNMemories; cMemIndx++)
	{
		// clear four phi regions at a time ..
		#pragma HLS unroll 
		nStubs[cMemIndx]=0;
		(&hMemories[cMemIndx])->clear(bx);
	}

	ap_uint<1> cToFirst = hLinkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-2);
	ap_uint<1> cIsPS = hLinkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-3);
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		// decode stub 
		// check which memory 
		ap_uint<kNBits_DTC> hStub = hStubs[cStubCounter]; 
		ap_uint<3> cEncLyr = (hStub != 0 ) ? ap_uint<3>(hStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3) : ap_uint<3>(0x7) ;
		int cMemIndx=0;
		int cBaseIndx=cMemIndx;
		for( int cLyr=0 ; cLyr<hNLayers;cLyr++)
		{
			#pragma HLS unroll 
			ap_uint<4> hWrd = hLinkWord.range(4*cLyr+3,4*cLyr+1);
			ap_uint<1> hIsBrl = hWrd.range(1,0);
			ap_uint<3> hLyrId = hWrd.range(3,1);
			int cMaxPhiBin = (cIsPS && hLyrId==1) ? 8 : 4; 
			if( cEncLyr == cLyr )
			{
				ap_uint<3> hPhiBn;
				if( cIsPS && hIsBrl ) 
					GetPhiBin<BARRELPS>(hStub, hWrd, hPhiBn);
				else if( cIsPS && !hIsBrl )
					GetPhiBin<DISKPS>(hStub, hWrd, hPhiBn);
				else if( hIsBrl )
					GetPhiBin<BARREL2S>(hStub, hWrd, hPhiBn);
				else
					GetPhiBin<DISK2S>(hStub, hWrd, hPhiBn);
				
				int cIndx = cBaseIndx + int(hPhiBn);
				ap_uint<8> hEntries = nStubs[cIndx];
				AllStub<TRACKER> hMemWord(hStub.range(kBRAMwidth-1,0));
				(&hMemories[cIndx])->write_mem(bx,hMemWord,hEntries);
				nStubs[cMemIndx]=hEntries+1;
			}
			cMemIndx += cMaxPhiBin;
			cBaseIndx = cMemIndx;
		}
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
