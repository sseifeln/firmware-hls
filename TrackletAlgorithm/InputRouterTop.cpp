#include "InputRouterTop.h"
#include "InputRouter.cc"

// //route stubs for 2S memories only 
void InputRouter2S(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk)
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS stream variable=hIputLink depth=1 
	// #pragma HLS interface ap_fifo port=hIputLink

	// clear memories for this bunch crossing 
	// one memory for each coarse phi bin 
	LOOP_ClearInputStubs2S : 
	for( int cPhiBn=0; cPhiBn<kNRegions; cPhiBn++)
	{
		#pragma HLS unroll  // clear all phi regions at a time ..
		(&hBrl.m1[cPhiBn])->clear(bx);
		(&hBrl.m2[cPhiBn])->clear(bx);
		(&hBrl.m3[cPhiBn])->clear(bx);
		(&hDsk.m1[cPhiBn])->clear(bx);
		(&hDsk.m2[cPhiBn])->clear(bx);
		(&hDsk.m3[cPhiBn])->clear(bx);
		(&hDsk.m4[cPhiBn])->clear(bx);
		(&hDsk.m5[cPhiBn])->clear(bx);
	}

	ap_uint<1> cIs2S; is2S(hDTCMapEncoded, cIs2S);
	assert( cIs2S == 1 ); // this should only be for 2S modules 
			
	ap_uint<3> cLyrOrDskId;
	ap_uint<1> cIsBrl;
	ap_uint<kNBits_DTC> hWord;
	LOOP_InputStubs2S : 
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		hWord = hIputLink[cStubCounter];
		if (hWord != 0x00 )
		//if (hIputLink.read_nb(hWord))
		{
			DecodeMap( hWord, hDTCMapEncoded , cLyrOrDskId, cIsBrl);
			
			ap_uint<2> cPhiBn;
			if( cIsBrl == 1 ) // stub is from a barrel module 
			{
				// three layers [4--6]
				assert( cLyrOrDskId >=4 && cLyrOrDskId <= 6 ); 
				InputStub<BARREL2S> hStub(hWord.range(kBRAMwidth-1,0));
				GetCoarsePhiRegion<InputStub<BARREL2S>,2>(hWord, cPhiBn);
					
				if( cLyrOrDskId == 4 ) 
					hBrl.m1[cPhiBn].write_mem(bx, hStub );
				//	WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n1[cPhiBn], hBrl.m1[cPhiBn]);
				else if( cLyrOrDskId == 5 ) 
					hBrl.m2[cPhiBn].write_mem(bx, hStub );
				//	WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n2[cPhiBn], hBrl.m2[cPhiBn]);
				else 
					hBrl.m3[cPhiBn].write_mem(bx, hStub );
				//	WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n3[cPhiBn], hBrl.m3[cPhiBn]);
			}
			else
			{
				// five disks [1--5] in the endcap 
				assert( cLyrOrDskId >=1 && cLyrOrDskId <= 5 ); 
				InputStub<DISK2S> hStub(hWord.range(kBRAMwidth-1,0));
				GetCoarsePhiRegion<InputStub<DISK2S>,2>(hWord, cPhiBn);
				
				if( cLyrOrDskId == 1 ) 
					hDsk.m1[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n1[cPhiBn], hDsk.m1[cPhiBn]);
				else if( cLyrOrDskId == 2 ) 
					hDsk.m2[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n2[cPhiBn], hDsk.m2[cPhiBn]);
				else if( cLyrOrDskId == 3 ) 
					hDsk.m3[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n3[cPhiBn], hDsk.m3[cPhiBn]);
				else if( cLyrOrDskId == 4 ) 
					hDsk.m4[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n4[cPhiBn], hDsk.m4[cPhiBn]);
				else 
					hDsk.m5[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n5[cPhiBn], hDsk.m5[cPhiBn]);
			}
		}
	}
	
}

//route stubs for PS memories only 
void InputRouterPS(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk)
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS stream variable=hIputLink depth=1 
	// #pragma HLS interface ap_fifo port=hIputLink

	// one memory for each coarse phi bin 
	LOOP_ClearInputStubsL1PS : 
	for( int cPhiBn=0; cPhiBn<kNRegionsLayer1; cPhiBn++)
	{
		#pragma HLS unroll  // clear 8 phi regions at a time ..
		(&hBrl.m1[cPhiBn])->clear(bx);
	}
	//
	LOOP_ClearInputStubsPS : 
	for( int cPhiBn=0; cPhiBn<kNRegions; cPhiBn++)
	{
		#pragma HLS unroll // clear four phi regions at a time ..
		(&hBrl.m2[cPhiBn])->clear(bx);
		(&hBrl.m3[cPhiBn])->clear(bx);
		(&hDsk.m1[cPhiBn])->clear(bx);
		(&hDsk.m2[cPhiBn])->clear(bx);
		(&hDsk.m3[cPhiBn])->clear(bx);
		(&hDsk.m4[cPhiBn])->clear(bx);
		(&hDsk.m5[cPhiBn])->clear(bx);
	}
	

	ap_uint<1> cIs2S; is2S(hDTCMapEncoded, cIs2S);
	assert( cIs2S == 0 ); // this should only be for PS modules 
			
	ap_uint<3> cLyrOrDskId;
	ap_uint<1> cIsBrl;
	//ap_uint<8> hEntries;
	//#pragma HLS dependence variable=hEntries intra WAR true
	ap_uint<kNBits_DTC> hWord;
	LOOP_InputStubsPS : 
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		hWord = hIputLink[cStubCounter];
		if (hWord != 0x00 )
		//if (hIputLink.read_nb(hWord))
		{
			DecodeMap( hWord, hDTCMapEncoded , cLyrOrDskId, cIsBrl);
			
			if( cIsBrl == 1 ) // stub is from a barrel module 
			{
				assert( cLyrOrDskId >=1 && cLyrOrDskId <= 3 ); 
				InputStub<BARRELPS> hStub(hWord.range(kBRAMwidth-1,0));
				if( cLyrOrDskId == 1 )
				{
					ap_uint<3> cPhiBn;
					GetCoarsePhiRegion<InputStub<BARRELPS>,3>(hWord, cPhiBn);
					hBrl.m1[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n1[cPhiBn], hBrl.m1[cPhiBn]);
				}
				else
				{
					ap_uint<2> cPhiBn;
					GetCoarsePhiRegion<InputStub<BARRELPS>,2>(hWord, cPhiBn);
					if( cLyrOrDskId == 2 )
					{
						hBrl.m2[cPhiBn].write_mem(bx, hStub );
						//WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n2[cPhiBn], hBrl.m2[cPhiBn]);
					}
					else
					{
						hBrl.m3[cPhiBn].write_mem(bx, hStub );
						//WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n3[cPhiBn], hBrl.m3[cPhiBn]);
					}
				}
			}
			else
			{
				ap_uint<2> cPhiBn;
				// five disks [1--5] in the endcap 
				assert( cLyrOrDskId >=1 && cLyrOrDskId <= 5 ); 
				InputStub<DISKPS> hStub(hWord.range(kBRAMwidth-1,0));
				GetCoarsePhiRegion<InputStub<DISKPS>,2>(hWord, cPhiBn);
				
				if( cLyrOrDskId == 1 ) 
				{

					hDsk.m1[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n1[cPhiBn], hDsk.m1[cPhiBn]);
				}
				else if( cLyrOrDskId == 2 ) 
				{
					hDsk.m2[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n2[cPhiBn], hDsk.m2[cPhiBn]);
				}
				else if( cLyrOrDskId == 3 ) 
				{
					hDsk.m3[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n3[cPhiBn], hDsk.m3[cPhiBn]);
				}
				else if( cLyrOrDskId == 4 ) 
				{
					hDsk.m4[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n4[cPhiBn], hDsk.m4[cPhiBn]);
				}
				else 
				{
					hDsk.m5[cPhiBn].write_mem(bx, hStub );
					//WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n5[cPhiBn], hDsk.m5[cPhiBn]);
				}
			}
		}
	}
	
}


void TopLevelIR( const BXType bx
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
	LOOP_ClearInputStubs : 
	for( int cPhiBn=0; cPhiBn<4; cPhiBn++)
	{
		// clear four phi regions at a time ..
		#pragma HLS unroll 
		n1[cPhiBn]=0;
		n2[cPhiBn]=0;
		//(&L1[cPhiBn])->clear(bx);
		//(&L2[cPhiBn])->clear(bx);
	}

	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		//#pragma HLS unroll factor = 2
		if( hStubs[cStubCounter] != 0 )
		{
			EnLRouter2<BARREL2S,DISK2S,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2, L1, L2 );
		}
	}
}


void PSIRTest( const BXType bx
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
			EnLRouter4<BARRELPS,DISKPS,DISKPS,DISKPS,8,4,4,4>(bx , hStubs[cStubCounter], hLinkWord, n1,n2,n3,n4, L1, L2,L3,L4 );
		}
	}
}
