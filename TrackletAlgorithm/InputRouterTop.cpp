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


// output stream 
// an 8 bit word 3 + 3 + 2 
void RegionRouter(hls::stream<ap_uint<kNBits_DTC>> & inData
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, hls::stream<ap_uint<8>> &hRoutingInfo   
	, hls::stream<ap_uint<kBRAMwidth>> outData[8])
{
	ap_uint<1> hValid=1; 
	ap_uint<8> hRWord; 
	ap_uint<3> hPhi;
	ap_uint<3> hLyr;
	ap_uint<1> hIs2S;
	ap_uint<1> hIsBrl;
	is2S(hLinkWord, hIs2S);
	ap_uint<kNBits_DTC> hWord;
	LOOP_ReadInputStubs :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++) 
	//while(hValid)
	{
		#pragma HLS pipeline II=1
		if(inData.read_nb(hWord)) 
		{
			DecodeMap( hWord ,  hLinkWord, hLyr, hIsBrl);
			if( hIsBrl  == 1) 
			{
				if( hIs2S == 1 )
				{
					ap_uint<2> cPhiBn;
					GetCoarsePhiRegion<InputStub<BARREL2S>,2>(hWord, cPhiBn);
					hPhi = cPhiBn & 0x7 ; 
				}
				else
				{
					if( hLyr == 1 )
					{
						GetCoarsePhiRegion<InputStub<BARRELPS>,3>(hWord, hPhi);
					}
				}
				(&outData[hPhi])->write_nb(hWord.range(kBRAMwidth-1,0));
			}
			else
			{
				ap_uint<2> cPhiBn;
				if( hIs2S == 1 )
				{
					GetCoarsePhiRegion<InputStub<DISK2S>,2>(hWord, cPhiBn);
				}
				else
				{
					GetCoarsePhiRegion<InputStub<DISKPS>,2>(hWord, cPhiBn);
				}
				hPhi = cPhiBn & 0x7 ; 
				(&outData[hPhi])->write_nb(hWord.range(kBRAMwidth-1,0));
			}
			hRWord.range(7,7)=hIs2S;
			hRWord.range(6,6)=hIsBrl;
			hRWord.range(5,3)=hLyr;
			hRWord.range(2,0)=hPhi;
			hRoutingInfo.write_nb(hRWord); 
		}
		else
			hValid=0;
	}
}

void MemoryRouter(const BXType bx, hls::stream<ap_uint<kBRAMwidth>> inData[8]
	, hls::stream<ap_uint<8>> &hRoutingInfo 
	, StubsBarrelPS& hBrl, StubsDiskPS& hDsk )
{
	ap_uint<1> hValid=1; 
	ap_uint<3> hPhi;
	ap_uint<3> hLyr;
	ap_uint<1> hIs2S;
	ap_uint<1> hIsBrl;
	ap_uint<kBRAMwidth> hWord;
	ap_uint<8> hRoutingWord; 
	LOOP_ReadRoutedStubs :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++) 
	{
		#pragma HLS pipeline II=1
		if( hRoutingInfo.read_nb(hRoutingWord) )
		{
			hIs2S = hRoutingWord.range(7,7); 
			hIsBrl = hRoutingWord.range(6,6); 
			hLyr = hRoutingWord.range(5,3); 
			hPhi = hRoutingWord.range(2,0); 
			if( inData[hPhi].read_nb(hWord) )
			{
				#ifndef __SYNTHESIS__
					std::cout << "\t\tRouting word is "
						<< std::bitset<8>(hRoutingWord)
						<< " -- is2S bit "
						<< hIs2S 
						<< " -- barrel bit "
						<< hIsBrl
						<< " -- layer "
						<< hLyr 
						<< " -- stub word is "
						<< std::bitset<kBRAMwidth>(hWord)
						<< "\n";
				#endif
				if( hIsBrl == 1 )
				{
					InputStub<BARRELPS> hStub(hWord);
					if( hLyr == 1 )
						hBrl.m1[hPhi].write_mem(bx, hStub );
					else if( hLyr == 2 )
						hBrl.m2[hPhi].write_mem(bx, hStub );
					else if( hLyr == 3 )
						hBrl.m3[hPhi].write_mem(bx, hStub );
				}
				else
				{
					InputStub<DISKPS> hStub(hWord);
					if( hLyr == 1 )
						hDsk.m1[hPhi].write_mem(bx, hStub );
					else if( hLyr == 2 )
						hDsk.m2[hPhi].write_mem(bx, hStub );
					else if( hLyr == 3 )
						hDsk.m3[hPhi].write_mem(bx, hStub );
					else if( hLyr == 4 )
						hDsk.m4[hPhi].write_mem(bx, hStub );
					else if( hLyr == 5 )
						hDsk.m5[hPhi].write_mem(bx, hStub );
				}
			}
		}
		else
		{
			hValid = 0; 
		}
	}
}

void LayerDecode(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	LayerRouterOutputPort &hOutput)
{
	hOutput.isValid=1;
	hOutput.hStub = inStub.range(kBRAMwidth-1,0);
	ap_uint<4> hLyrDecoding = lnkWord.range(pLayer*4+3,pLayer*4);
	hOutput.isBrl = hLyrDecoding.range(0,0);
	hOutput.layerId = hLyrDecoding.range(3,1);
}

void DecodeInputStub(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	LayerRouterOutputPort &hOutput)
{
	// some constants 
	ap_uint<3> cNbitsL1=3;
	ap_uint<3> cNbitsTk=2; 

	// stuff I can get from the lnk word 
	ap_uint<1> hFirstLyr = lnkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-1);
	ap_uint<1> hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// now stuff I can get from the stub word 
	//ap_uint<4> hLyrDecoding = lnkWord.range(pLayer*4+3,pLayer*4);
	ap_uint<8> hIndex=0;
	ap_uint<1> hIsBrl;
	ap_uint<3> hLyr; 
	LOOP_LyrIndex :
	for( int iLyr=0; iLyr<4;iLyr++)
	{
		#pragma HLS unroll
		if( iLyr == pLayer )
		{
			hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
			hIsBrl = lnkWord.range(hIndex,hIndex);
			hLyr = lnkWord.range(hIndex+3,hIndex+1);
		}
		else
			hIndex = hIndex + 4; 
	}
	ap_uint<5> hPhiMSB;
	ap_uint<5> hPhiLSB; 
	if( hIsBrl == 1 )
	{
		if( hIs2S == 0 )
		{
			hPhiMSB = InputStub<BARRELPS>::kISPhiMSB;
			if( hLyr == 1 ) 
				hPhiLSB = InputStub<BARRELPS>::kISPhiMSB-(cNbitsL1-1);
			else
				hPhiLSB = InputStub<BARRELPS>::kISPhiMSB-(cNbitsTk-1);
		}
		else
		{
			hPhiMSB = InputStub<BARRELPS>::kISPhiMSB;
			hPhiLSB = InputStub<BARRELPS>::kISPhiMSB-(cNbitsTk-1);
		}
	}
	else
	{
		if( hIs2S == 0 )
		{
			hPhiMSB = InputStub<DISKPS>::kISPhiMSB;
			hPhiLSB = InputStub<DISKPS>::kISPhiMSB-(cNbitsTk-1);
		}
		else
		{
			hPhiMSB = InputStub<DISK2S>::kISPhiMSB;
			hPhiLSB = InputStub<DISK2S>::kISPhiMSB-(cNbitsTk-1);
		}	
	}
	// if( hIs2S == 0 && hLyrDecoding.range(3,1) == 1 && hLyrDecoding.range(0,0) == 1 )
	// {
	// 	hPhiMSB = InputStub<BARRELPS>::kISPhiMSB;
	// 	hPhiLSB = InputStub<BARRELPS>::kISPhiMSB-(cNbitsL1-1);
	// }
	// // BRL PS 
	// else if( hIs2S == 0 && hLyrDecoding.range(0,0) == 1 )
	// {
	// 	hPhiMSB = InputStub<BARRELPS>::kISPhiMSB;
	// 	hPhiLSB = InputStub<BARRELPS>::kISPhiMSB-(cNbitsTk-1);
	// }
	// // DSK PS 
	// else if( hIs2S == 0 && hLyrDecoding.range(0,0) == 0 ) 
	// {
	// 	hPhiMSB = InputStub<DISKPS>::kISPhiMSB;
	// 	hPhiLSB = InputStub<DISKPS>::kISPhiMSB-(cNbitsTk-1);
	// }
	// // BRL 2S 
	// else if( hIs2S == 1 && hLyrDecoding.range(0,0) == 1 )
	// {
	// 	hPhiMSB = InputStub<BARREL2S>::kISPhiMSB;
	// 	hPhiLSB = InputStub<BARREL2S>::kISPhiMSB-(cNbitsTk-1);
	// }
	// // DSK 2S 
	// else if( hIs2S == 1 && hLyrDecoding.range(0,0) == 0 )
	// {
	// 	hPhiMSB = InputStub<DISK2S>::kISPhiMSB;
	// 	hPhiLSB = InputStub<DISK2S>::kISPhiMSB-(cNbitsTk-1);
	// }
	hOutput.isValid=1;
	hOutput.is2S = hIs2S;
	hOutput.isBrl = hIsBrl;
	hOutput.layerId = hLyr;
	hOutput.phiRegion = inStub.range(hPhiMSB,hPhiLSB) & 0x7;	
	hOutput.hStub = inStub.range(kBRAMwidth-1,0);
}

void LayerRouter(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	LayerRouterOutputPort &hOutput) 
{
	LOOP_EncLyrRouter :
	for( int cLayer=0; cLayer<=3; cLayer++)
	{
		#pragma HLS unroll
		if( cLayer == inStub.range(kNBits_DTC-1,kNBits_DTC-2) )
		{
			#pragma HLS inline
			ap_uint<2> cLyr=cLayer&0x3; 
			DecodeInputStub( inStub, lnkWord , cLyr, hOutput );
		}
	}
}



void UpdateCounters(LayerRouterOutputPort hInput
	, EntriesBarrelPS& cBrl
	, EntriesDiskPS& cDsk)
{
	#pragma HLS ARRAY_PARTITION variable=cBrl.n1 complete
	#pragma HLS ARRAY_PARTITION variable=cBrl.n2 complete
	#pragma HLS ARRAY_PARTITION variable=cBrl.n3 complete
	#pragma HLS ARRAY_PARTITION variable=cDsk.n1 complete
	#pragma HLS ARRAY_PARTITION variable=cDsk.n2 complete
	#pragma HLS ARRAY_PARTITION variable=cDsk.n3 complete
	#pragma HLS ARRAY_PARTITION variable=cDsk.n4 complete
	#pragma HLS ARRAY_PARTITION variable=cDsk.n5 complete
	
	LOOP_CounterUpdate :
	for( int cLayer=1; cLayer<=5; cLayer++)
	{
		#pragma HLS unroll
		if( cLayer == hInput.layerId && hInput.isValid == 1 && hInput.is2S == 0  && hInput.isBrl == 1 )
		{
			if( cLayer == 1 )
			{
				cBrl.n1[hInput.phiRegion]++;
			}
			else if( cLayer == 2 )
			{
				cBrl.n2[hInput.phiRegion]++;
			}
			else if( cLayer == 3 )
			{
				cBrl.n3[hInput.phiRegion]++;
			}
		}
		else if( cLayer == hInput.layerId && hInput.isValid == 1 && hInput.is2S == 0  && hInput.isBrl == 0 )
		{
			if( cLayer == 1 )
			{
				cDsk.n1[hInput.phiRegion]++;
			}
			else if( cLayer == 2 )
			{
				cDsk.n2[hInput.phiRegion]++;
			}
			else if( cLayer == 3 )
			{
				cDsk.n3[hInput.phiRegion]++;
			}
			else if( cLayer == 4 )
			{
				cDsk.n4[hInput.phiRegion]++;
			}
			else if( cLayer == 5 )
			{
				cDsk.n5[hInput.phiRegion]++;
			}
		}
	}
}

void RouterPS(const BXType bx
	, RouterInputPort inPrt
	, StubsBarrelPS& hBrl
	, StubsDiskPS& hDsk) 
{
	#pragma HLS clock domain=fast_clock 
	#pragma HLS stream variable=inPrt.hStubs depth=1 

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
	
	ap_uint<8> hEntries;
	LayerRouterOutputPort oLRouter;
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if( inPrt.hStubs[cStubCounter] != 0 ) 
		{
			// working 
			LayerRouter(inPrt.hStubs[cStubCounter], inPrt.hLinkWord, oLRouter);
			assert( oLRouter.is2S == 0 ); 
			if( oLRouter.isValid == 1 )
			{
				if( oLRouter.isBrl == 1 )
				{
					InputStub<BARRELPS> hStub(oLRouter.hStub);
					if( oLRouter.layerId == 1 )
						hBrl.m1[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 2 )
						hBrl.m2[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 3 )
						hBrl.m3[oLRouter.phiRegion].write_mem(bx, hStub);
				}
				else
				{
					InputStub<DISKPS> hStub(oLRouter.hStub);
					if( oLRouter.layerId == 1 )
						hDsk.m1[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 2 )
						hDsk.m2[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 3 )
						hDsk.m3[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 4 )
						hDsk.m4[oLRouter.phiRegion].write_mem(bx, hStub);
					else if( oLRouter.layerId == 5 )
						hDsk.m5[oLRouter.phiRegion].write_mem(bx, hStub);
				}
			}
			// if( oLRouter.isValid == 1 && oLRouter.is2S == 0  && oLRouter.isBrl == 1 )
			// {
			// 	InputStub<BARRELPS> hStub(oLRouter.hStub);
			// 	if( oLRouter.layerId == 1 )
			// 	{
			// 		hEntries = (hBrl.m1[oLRouter.phiRegion]).getEntries(bx);
			// 		(hBrl.m1[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 2 )
			// 	{
			// 		hEntries = (hBrl.m2[oLRouter.phiRegion]).getEntries(bx);
			// 		(hBrl.m2[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 3 )
			// 	{
			// 		hEntries = (hBrl.m3[oLRouter.phiRegion]).getEntries(bx);
			// 		(hBrl.m3[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// }
			// else if( oLRouter.isValid == 1 && oLRouter.is2S == 0  && oLRouter.isBrl == 0 )
			// {
			// 	InputStub<DISKPS> hStub(oLRouter.hStub);
			// 	if( oLRouter.layerId == 1 )
			// 	{
			// 		hEntries = (hDsk.m1[oLRouter.phiRegion]).getEntries(bx);
			// 		(hDsk.m1[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 2 )
			// 	{
			// 		hEntries = (hDsk.m2[oLRouter.phiRegion]).getEntries(bx);
			// 		(hDsk.m2[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 3 )
			// 	{
			// 		hEntries = (hDsk.m3[oLRouter.phiRegion]).getEntries(bx);
			// 		(hDsk.m3[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 4 )
			// 	{
			// 		hEntries = (hDsk.m4[oLRouter.phiRegion]).getEntries(bx);
			// 		(hDsk.m4[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// 	else if( oLRouter.layerId == 5 )
			// 	{
			// 		hEntries = (hDsk.m5[oLRouter.phiRegion]).getEntries(bx);
			// 		(hDsk.m5[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
			// 	}
			// }
		}
	}
}

void GenericRouter(RouterInputPort inPS,
	RouterInputPort in2S, 
	ap_uint<8>& nRoutedPS,
	ap_uint<8>& nRouted2S)
{
	#pragma HLS clock domain=fast_clock 
	#pragma HLS stream variable=inPS.hStubs depth=1 
	#pragma HLS stream variable=in2S.hStubs depth=1 

	nRoutedPS=0;
	nRouted2S=0;

	ap_uint<3> nPhiBins=4;
	LayerRouterOutputPort oLRouterPS;
	LayerRouterOutputPort oLRouter2S;

	PhiRouterOutputPort oPhiRouterPS;
	PhiRouterOutputPort oPhiRouter2S;
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<10 ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		LayerRouter(inPS.hStubs[cStubCounter], inPS.hLinkWord, oLRouterPS);
		LayerRouter(in2S.hStubs[cStubCounter], in2S.hLinkWord, oLRouter2S);
		if( oLRouterPS.isValid == 1 )
		{
			nRoutedPS++;
		}
		if( oLRouter2S.isValid == 1 )
		{
			nRouted2S++;
		}

		// LOOP_EncLyrRouter :
		// for( ap_uint<3> cLayer=1; cLayer<=3; cLayer++)
		// {
		// 	#pragma HLS unroll
		// 	LayerRouter(inPS.hStubs[cStubCounter], inPS.hLinkWord, cLayer, oLRouterPS);
		// 	if( oLRouterPS.isValid == 1 )
		// 	{
		// 		if( oLRouterPS.layerId == 1 && oLRouterPS.isBrl == 1 ) 
		// 		{
		// 			LOOP_PhiRouterPSL1 :
		// 			for( ap_uint<3> cPhiBn=0; cPhiBn < 8; cPhiBn++ )
		// 			{
		// 				#pragma HLS unroll
		// 				PhiRouterPS(oLRouterPS, cPhiBn, oPhiRouterPS);
		// 				if( oPhiRouterPS.isValid == 1 ) 
		// 					nRoutedPS++;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			LOOP_PhiRouterPS :
		// 			for( ap_uint<3> cPhiBn=0; cPhiBn < 4; cPhiBn++ )
		// 			{
		// 				#pragma HLS unroll
		// 				PhiRouterPS(oLRouterPS, cPhiBn, oPhiRouterPS);
		// 				if( oPhiRouterPS.isValid == 1 ) 
		// 					nRoutedPS++;
		// 			}
		// 		}
		// 	}
		// 	LayerRouter(in2S.hStubs[cStubCounter], in2S.hLinkWord, cLayer, oLRouter2S);
		// 	if( oLRouter2S.isValid == 1 )
		// 	{
		// 		nRouted2S++;
		// 	}
		// }
	}
}