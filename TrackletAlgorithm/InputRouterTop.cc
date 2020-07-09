#include "InputRouterTop.h"


// void InputRouter_PS( const BXType bx
// 	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
// 	, const ap_uint<kLINKMAPwidth> hLinkWord 
// 	, const ap_uint<5> hNMemories  
// 	, const ap_uint<3> hNLayers
// 	, const ap_uint<3> *hLayers  
// 	, AllStubMemory<BARRELPS> *hBarrelMemories
// 	, AllStubMemory<DISKPS> *hDiskMemories)
// {
// 	#pragma HLS clock domain=slow_clock 
// 	#pragma HLS interface ap_none port=hLinkWord
// 	#pragma HLS interface ap_none port=hNMemories
// 	#pragma HLS interface ap_none port=hNLayers
// 	#pragma HLS interface ap_none port=hLayers
// 	#pragma HLS stream variable=hStubs depth=1 
	
// 	LOOP_OuterStubLoop :
// 	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
// 	{
// 		#pragma HLS pipeline II=1
// 		// decode stub 
// 		// check which memory 
// 		ap_uint<3> pLayer= (hStubs[cStubCounter] != 0 ) ? ap_uint<3>(inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3) : 0x7 ;
// 		for( int cMemIndx=0 ; cMemIndx<hNMemories;cMemIndx++)
// 		{
// 			#pragma HLS unroll 
// 			ap_uint<3> hEncodedLyr=	hLayers[cMemIndx];
// 		}
// 		// if( hStubs[cStubCounter] != 0 )
// 		// {
			
// 		// }
// 	}
// }
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
