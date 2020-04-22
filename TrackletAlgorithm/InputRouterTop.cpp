#include "InputRouterTop.h"
#include "InputRouter.cc"

// //route stubs for 2S memories only 
void InputRouter2S(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk)
{
	// local variables to keep track of how many entries 
	// are in each memory 
	EntriesBarrel2S cBrl;
	EntriesDisk2S cDsk;
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS interface ap_fifo port=hIputLink

	// needed this to allow pipeline of 1 
	#pragma HLS ARRAY_PARTITION variable=cBrl.n1 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cBrl.n2 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cBrl.n3 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n1 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n2 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n3 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n4 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n5 cyclic factor=2 dim=1

	// clear memories for this bunch crossing 
	// one memory for each coarse phi bin 
	LOOP_ClearInputStubs2S : 
	for( int cPhiBn=0; cPhiBn<kNRegions; cPhiBn++)
	{
		#pragma HLS pipeline II=1
		#pragma HLS unroll factor=2 // clear four phi regions at a time ..
			// (&hBrl.m1[cPhiBn])->clear(bx);
			// (&hBrl.m2[cPhiBn])->clear(bx);
			// (&hBrl.m3[cPhiBn])->clear(bx);
			// (&hDsk.m1[cPhiBn])->clear(bx);
			// (&hDsk.m2[cPhiBn])->clear(bx);
			// (&hDsk.m3[cPhiBn])->clear(bx);
			// (&hDsk.m4[cPhiBn])->clear(bx);
			// (&hDsk.m5[cPhiBn])->clear(bx);
			
			cBrl.n1[cPhiBn]=0;
			cBrl.n2[cPhiBn]=0;
			cBrl.n3[cPhiBn]=0;
			cDsk.n1[cPhiBn]=0;
			cDsk.n2[cPhiBn]=0;
			cDsk.n3[cPhiBn]=0;
			cDsk.n4[cPhiBn]=0;
			cDsk.n5[cPhiBn]=0;
	}

	ap_uint<1> cIs2S; is2S(hDTCMapEncoded, cIs2S);
	ap_uint<3> cLyrOrDskId;
	ap_uint<1> cIsBrl;
	ap_uint<kNBits_DTC> hWord;
	LOOP_InputStubs2S : 
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if (hIputLink.read_nb(hWord))
		{
			DecodeMap( hWord, hDTCMapEncoded , cLyrOrDskId, cIsBrl);
			assert( cIs2S == 1 ); // this should only be for 2S modules 
			
			ap_uint<2> cPhiBn;
			if( cIsBrl == 1 ) // stub is from a barrel module 
			{
				// three layers [4--6]
				assert( cLyrOrDskId >=4 && cLyrOrDskId <= 6 ); 
				InputStub<BARREL2S> hStub(hWord.range(kBRAMwidth-1,0));
				GetCoarsePhiRegion<InputStub<BARREL2S>,2>(hWord, cPhiBn);
					
				if( cLyrOrDskId == 4 ) 
					WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n1[cPhiBn], hBrl.m1[cPhiBn]);
				else if( cLyrOrDskId == 5 ) 
					WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n2[cPhiBn], hBrl.m2[cPhiBn]);
				else 
					WriteMemories<BARREL2S>(bx,hStub.raw(),cBrl.n3[cPhiBn], hBrl.m3[cPhiBn]);
			}
			else
			{
				// five disks [1--5] in the endcap 
				assert( cLyrOrDskId >=1 && cLyrOrDskId <= 5 ); 
				InputStub<DISK2S> hStub(hWord.range(kBRAMwidth-1,0));
				GetCoarsePhiRegion<InputStub<DISK2S>,2>(hWord, cPhiBn);
				
				if( cLyrOrDskId == 1 ) 
					WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n1[cPhiBn], hDsk.m1[cPhiBn]);
				else if( cLyrOrDskId == 2 ) 
					WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n2[cPhiBn], hDsk.m2[cPhiBn]);
				else if( cLyrOrDskId == 3 ) 
					WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n3[cPhiBn], hDsk.m3[cPhiBn]);
				else if( cLyrOrDskId == 4 ) 
					WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n4[cPhiBn], hDsk.m4[cPhiBn]);
				else 
					WriteMemories<DISK2S>(bx,hStub.raw(),cDsk.n5[cPhiBn], hDsk.m5[cPhiBn]);
			}
		}
	}
	
}

//route stubs for PS memories only 
void InputRouterPS(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk)
{
	// local variables to keep track of how many entries 
	// are in each memory 
	EntriesBarrelPS cBrl;
	EntriesDiskPS cDsk;
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS interface ap_fifo port=hIputLink

	// needed this to allow pipeline of 1 
	#pragma HLS ARRAY_PARTITION variable=cBrl.n1 cyclic factor=4 dim=1
	#pragma HLS ARRAY_PARTITION variable=cBrl.n2 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cBrl.n3 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n1 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n2 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n3 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n4 cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=cDsk.n5 cyclic factor=2 dim=1

	// clear memories for this bunch crossing 
	// one memory for each coarse phi bin 
	LOOP_ClearInputStubsPS : 
	for( int cPhiBn=0; cPhiBn<kNRegionsLayer1; cPhiBn++)
	{
		#pragma HLS pipeline II=1
		#pragma HLS unroll factor=2 // clear four phi regions at a time ..
		cBrl.n1[cPhiBn]=0;
		if( cPhiBn < kNRegions)
		{
			cBrl.n2[cPhiBn]=0;
			cBrl.n3[cPhiBn]=0;
			cDsk.n1[cPhiBn]=0;
			cDsk.n2[cPhiBn]=0;
			cDsk.n3[cPhiBn]=0;
			cDsk.n4[cPhiBn]=0;
			cDsk.n5[cPhiBn]=0;
		}
	}

	ap_uint<1> cIs2S; is2S(hDTCMapEncoded, cIs2S);
	ap_uint<3> cLyrOrDskId;
	ap_uint<1> cIsBrl;
	ap_uint<kNBits_DTC> hWord;
	LOOP_InputStubsPS : 
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if (hIputLink.read_nb(hWord))
		{
			DecodeMap( hWord, hDTCMapEncoded , cLyrOrDskId, cIsBrl);
			assert( cIs2S == 0 ); // this should only be for PS modules 
			
			if( cIsBrl == 1 ) // stub is from a barrel module 
			{
				assert( cLyrOrDskId >=1 && cLyrOrDskId <= 3 ); 
				InputStub<BARRELPS> hStub(hWord.range(kBRAMwidth-1,0));
				if( cLyrOrDskId == 1 )
				{
					ap_uint<3> cPhiBn;
					GetCoarsePhiRegion<InputStub<BARRELPS>,3>(hWord, cPhiBn);
					WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n1[cPhiBn], hBrl.m1[cPhiBn]);
				}
				else
				{
					ap_uint<2> cPhiBn;
					GetCoarsePhiRegion<InputStub<BARRELPS>,2>(hWord, cPhiBn);
					if( cLyrOrDskId == 2 )
						WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n2[cPhiBn], hBrl.m2[cPhiBn]);
					else
						WriteMemories<BARRELPS>(bx,hStub.raw(),cBrl.n3[cPhiBn], hBrl.m3[cPhiBn]);
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
					WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n1[cPhiBn], hDsk.m1[cPhiBn]);
				else if( cLyrOrDskId == 2 ) 
					WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n2[cPhiBn], hDsk.m2[cPhiBn]);
				else if( cLyrOrDskId == 3 ) 
					WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n3[cPhiBn], hDsk.m3[cPhiBn]);
				else if( cLyrOrDskId == 4 ) 
					WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n4[cPhiBn], hDsk.m4[cPhiBn]);
				else 
					WriteMemories<DISKPS>(bx,hStub.raw(),cDsk.n5[cPhiBn], hDsk.m5[cPhiBn]);
			}
		}
	}
	
}
