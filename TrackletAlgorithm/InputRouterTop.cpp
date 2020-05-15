#include "InputRouterTop.h"


void InputRouter_2S_1Barrel1Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<BARREL2S> L1[4]
	, InputStubMemory<DISK2S> L2[4])
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
		//#pragma HLS unroll factor = 2
		if( hStubs[cStubCounter] != 0 )
		{
			EnLRouter2L<BARREL2S,DISK2S,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2, L1, L2 );
		}
	}
}


void InputRouter_PS_1Barrel3Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<BARRELPS> L1[8]
	, InputStubMemory<DISKPS> L2[4]
	, InputStubMemory<DISKPS> L3[4]
	, InputStubMemory<DISKPS> L4[4])
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
		if( cPhiBn < 4 )
		{
			n2[cPhiBn]=0;
			n3[cPhiBn]=0;
			n4[cPhiBn]=0;
		}
	}

	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		//#pragma HLS unroll factor = 2
		if( hStubs[cStubCounter] != 0 )
		{
			EnLRouter4L<BARRELPS,DISKPS,DISKPS,DISKPS,8,4,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2,n3,n4, L1, L2,L3,L4 );
		}
	}
}
