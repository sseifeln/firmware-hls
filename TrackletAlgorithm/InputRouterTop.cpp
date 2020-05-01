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
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	// some constants 
	ap_uint<3> cNbitsL1=3;
	ap_uint<3> cNbitsTk=2; 

	// stuff I can get from the lnk word 
	ap_uint<1> hFirstLyr = lnkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-1);
	ap_uint<1> hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// now stuff I can get from the stub word 
	ap_uint<4> hLyrDecoding;
	ap_uint<8> hIndex=0;
	LOOP_LyrIndex :
	for( int iLyr=0; iLyr<4;iLyr++)
	{
		#pragma HLS unroll
	 	#pragma HLS pipeline II=1 
		if( iLyr == pLayer )
		{
			hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
		}
		else
			hIndex = hIndex + 4; 
	}
	ap_uint<3> hPhiBn;
	if( hLyrDecoding.range(0,0) )
	{
		if( hIs2S == 0 )
			GetPhiBin<BARRELPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<BARREL2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	else
	{
		if( hIs2S == 0 )
			GetPhiBin<DISKPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<DISK2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	hOutput.isValid=1;
	hOutput.is2S = hIs2S;
	hOutput.isBrl = hLyrDecoding.range(0,0);
	hOutput.layerId = hLyrDecoding.range(3,1);
	hOutput.phiRegion = hPhiBn;	
	hOutput.hStub = inStub.range(kBRAMwidth-1,0);
	#ifndef __SYNTHESIS__
		std::cout << "Is2S bit "
			<< hOutput.is2S 
			<< " -- barrel bit "
			<< hOutput.isBrl
			<< " -- layer "
			<< hOutput.layerId 
			<< " -- phi "
			<< hOutput.phiRegion 
			<< " -- stub word is "
			<< std::bitset<kBRAMwidth>(hOutput.hStub)
			<< "\n";
	#endif
	
}

void DecoderInputStub(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	ap_uint<1> &hIsValid,
	ap_uint<1> &hIs2S,
	ap_uint<1> &hIsBrl,
	ap_uint<3> &layerId,
	ap_uint<3> &phiRegion,
	ap_uint<kBRAMwidth> &hStub)
{
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	// some constants 
	ap_uint<3> cNbitsL1=3;
	ap_uint<3> cNbitsTk=2; 

	// stuff I can get from the lnk word 
	ap_uint<2> pLayer=inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	ap_uint<1> hFirstLyr = lnkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-1);
	hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// now stuff I can get from the stub word 
	ap_uint<4> hLyrDecoding;
	ap_uint<8> hIndex=0;
	LOOP_LyrIndex :
	for( int iLyr=0; iLyr<4;iLyr++)
	{
		#pragma HLS unroll
	 	#pragma HLS pipeline II=1 
		if( iLyr == pLayer )
		{
			hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
		}
		else
			hIndex = hIndex + 4; 
	}
	ap_uint<3> hPhiBn;
	if( hLyrDecoding.range(0,0) )
	{
		if( hIs2S == 0 )
			GetPhiBin<BARRELPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<BARREL2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	else
	{
		if( hIs2S == 0 )
			GetPhiBin<DISKPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<DISK2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	hIsValid=1;
	hIsBrl = hLyrDecoding.range(0,0);
	layerId = hLyrDecoding.range(3,1);
	phiRegion = hPhiBn;	
	hStub = inStub.range(kBRAMwidth-1,0);
}

void DecodeIS(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	LayerRouterOutputPort &hOutput)
{
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	// some constants 
	ap_uint<3> cNbitsL1=3;
	ap_uint<3> cNbitsTk=2; 

	// stuff I can get from the lnk word 
	ap_uint<2> pLayer=inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	ap_uint<1> hFirstLyr = lnkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-1);
	ap_uint<1> hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// now stuff I can get from the stub word 
	ap_uint<4> hLyrDecoding;
	ap_uint<8> hIndex=0;
	LOOP_LyrIndex :
	for( int iLyr=0; iLyr<4;iLyr++)
	{
		#pragma HLS unroll
	 	#pragma HLS pipeline II=1 
		if( iLyr == pLayer )
		{
			hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
		}
		else
			hIndex = hIndex + 4; 
	}
	ap_uint<3> hPhiBn;
	if( hLyrDecoding.range(0,0) )
	{
		if( hIs2S == 0 )
			GetPhiBin<BARRELPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<BARREL2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	else
	{
		if( hIs2S == 0 )
			GetPhiBin<DISKPS>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		else
			GetPhiBin<DISK2S>(inStub, hLyrDecoding.range(3,1), hPhiBn);
	}
	hOutput.isValid=1;
	hOutput.is2S = hIs2S;
	hOutput.isBrl = hLyrDecoding.range(0,0);
	hOutput.layerId = hLyrDecoding.range(3,1);
	hOutput.phiRegion = hPhiBn;	
	hOutput.hStub = inStub.range(kBRAMwidth-1,0);
	#ifndef __SYNTHESIS__
		std::cout << "Is2S bit "
			<< hOutput.is2S 
			<< " -- barrel bit "
			<< hOutput.isBrl
			<< " -- layer "
			<< hOutput.layerId 
			<< " -- phi "
			<< hOutput.phiRegion 
			<< " -- stub word is "
			<< std::bitset<kBRAMwidth>(hOutput.hStub)
			<< "\n";
	#endif
	
}

void RouteInputStub(const BXType bx,
	const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	CountersPS& hCntrsPS,
	MemoriesPS &hPS)
{
	InputStubMemory<BARRELPS> tmp;
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	
	// some constants 
	ap_uint<3> cNbitsL1=3;
	ap_uint<3> cNbitsTk=2; 
	ap_uint<1> isValid=1;
	// stuff I can get from the lnk word 
	ap_uint<1> hFirstLyr = lnkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-1);
	ap_uint<1> hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// now stuff I can get from the stub word 
	ap_uint<4> hLyrDecoding;
	ap_uint<8> hIndex=0;
	LOOP_LyrIndex :
	for( int iLyr=0; iLyr<4;iLyr++)
	{
		#pragma HLS unroll
	 	#pragma HLS pipeline II=1 
		if( iLyr == pLayer )
		{
			hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
		}
		else
			hIndex = hIndex + 4; 
	}
	ap_uint<1> isBrl = hLyrDecoding.range(0,0);
	ap_uint<3> layerId = hLyrDecoding.range(3,1);
	ap_uint<3> phiBn;
	ap_uint<8> hEntries;
	if( isBrl == 1 )
	{
		if( hIs2S == 0 )
		{
			GetPhiBin<BARRELPS>(inStub, layerId, phiBn);
			InputStub<BARRELPS> hStub(inStub.range(kBRAMwidth-1,0));
			assert( layerId >=1 && layerId <= 3 ); 
			if( layerId == 1 )
			{
				//(&hPS.l1[phiBn])->write_mem(bx, hStub);
				//tmp.write_mem(bx, hStub, hCntrsPS.l1[phiBn]);
				//hCntrsPS.l1[phiBn]=hCntrsPS.l1[phiBn]+1;
			}
			// else if( layerId == 2 )
			// {
			// 	(&hPS.l2[phiBn])->write_mem(bx, hStub);
			// }
			// else 
			// {
			// 	(&hPS.l3[phiBn])->write_mem(bx, hStub);
			// }
		}
		else
		{
			GetPhiBin<BARREL2S>(inStub, layerId, phiBn);
			InputStub<BARREL2S> hStub(inStub.range(kBRAMwidth-1,0));
			assert( layerId >=4 && layerId <= 6 ); 
			// if( layerId == 4 )
			// {
			// }
			// else if( layerId == 5 )
			// {
			// }
			// else 
			// {
			// }
		}
	}
	else
	{
		assert( layerId >=1 && layerId <= 5 ); 
		if( hIs2S == 0 )
		{
			GetPhiBin<DISKPS>(inStub, layerId, phiBn);
			InputStub<DISKPS> hStub(inStub.range(kBRAMwidth-1,0));
			// if( layerId == 1 )
			// {
			// 	hPS.d1[phiBn].write_mem(bx, hStub);
			// }
			// else if( layerId  == 2 )
			// {
			// 	hPS.d2[phiBn].write_mem(bx, hStub);
			// }
			// else if( layerId == 3 )
			// {
			// 	hPS.d3[phiBn].write_mem(bx, hStub);
			// }
			// else if( layerId == 4 )
			// {
			// 	hPS.d4[phiBn].write_mem(bx, hStub);
			// }
			// else
			// {
			// 	hPS.d5[phiBn].write_mem(bx, hStub);
			// }	
		}
		else
		{
			GetPhiBin<DISK2S>(inStub, layerId, phiBn);
			InputStub<DISK2S> hStub(inStub.range(kBRAMwidth-1,0));
			// if( layerId == 1 )
			// {
			// }
			// else if( layerId  == 2 )
			// {
			// }
			// else if( layerId == 3 )
			// {
			// }
			// else if( layerId == 4 )
			// {
			// }
			// else
			// {
			// }
		}	
	}
	
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

void StubRouter(const BXType bx,
	const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	CountersPS& hCntrsPS,
	MemoriesPS &hPS) 
{
	LOOP_EncLyrRouter :
	for( int cLayer=0; cLayer<=3; cLayer++)
	{
		#pragma HLS unroll
		if( cLayer == inStub.range(kNBits_DTC-1,kNBits_DTC-2) )
		{
			#pragma HLS inline
			ap_uint<2> cLyr=cLayer&0x3; 
			RouteInputStub( bx, inStub, lnkWord , cLyr,hCntrsPS, hPS );
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

void GetCntrIndex(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex) 
{
	
	ap_uint<8> indx;
	ap_uint<8> hOffsetsBrl[6]={0,8,12,16,20,24};
	ap_uint<8> hOffsetsDsk[5]={28,32,36,40,44};
	if( isBrl == 1 ) 
		indx = hOffsetsBrl[layerId-1]+phiRegion;
	else 
		indx = hOffsetsDsk[layerId-1]+phiRegion;
	#ifndef __SYNTHESIS__
		std::cout << "\tIndx  "
			<< +indx
			<< " -- layer "
			<< layerId
			<< " -- phi "
 			<< phiRegion 
			<< "\n";
	#endif
	cIndex=indx;
}

void GetCntrIndex2S(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex) 
{
	
	ap_uint<8> indx;
	ap_uint<4> hRegion=phiRegion;
	ap_uint<8> hOffsetsBrl[3]={0,4,8};
	ap_uint<8> hOffsetsDsk[5]={12,16,20,24,28};
	if( isBrl == 1 ) 
		indx = hOffsetsBrl[layerId-4]+hRegion;
	else 
		indx = hOffsetsDsk[layerId-1]+hRegion;
	// LOOP_cntr :
	// for( int phiR=0; phiR<4;phiR++)
	// {
	// 	#pragma HLS unroll
	//  	#pragma HLS pipeline II=1 
	// 	if( phiR == phiRegion )
	// 	{
	// 		if( isBrl == 1 ) 
	// 			indx = hOffsetsBrl[layerId-4]+hRegion;
	// 		else 
	// 			indx = hOffsetsDsk[layerId-1]+hRegion;
	// 	}
	// 	else
	// 		hRegion = hRegion + 1; 
	// }
	cIndex=indx;
}

void GetCntrIndexPS(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex) 
{
	
	ap_uint<8> indx;
	ap_uint<4> hRegion=0;
	ap_uint<8> hOffsetsBrl[3]={0,8,12};
	ap_uint<8> hOffsetsDsk[5]={16,20,24,28,32};
	if( isBrl == 1 ) 
		indx = hOffsetsBrl[layerId-4]+hRegion;
	else 
		indx = hOffsetsDsk[layerId-1]+hRegion;
	// LOOP_cntr :
	// for( int phiR=0; phiR<8;phiR++)
	// {
	// 	#pragma HLS unroll
	//  	#pragma HLS pipeline II=1 
	// 	if( phiR == phiRegion )
	// 	{
	// 		if( isBrl == 1 ) 
	// 			indx = hOffsetsBrl[layerId-1]+hRegion;
	// 		else 
	// 			indx = hOffsetsDsk[layerId-1]+hRegion;
	// 	}
	// 	else
	// 		hRegion = hRegion + 1; 
	// }
	cIndex=indx;
}

void SimpleRouterPS(const BXType bx
	, const ap_uint<kNBits_DTC> hLnkStub
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, ap_uint<8> lCnt[kNMemories]
	, StubsBarrelPS& hBrl) 
{
	ap_uint<8> cIndx;
	LayerRouterOutputPort oLRouter;
	DecodeIS( hLnkStub, hLinkWord, oLRouter );
	GetCntrIndex( oLRouter.isBrl, oLRouter.layerId , oLRouter.phiRegion, cIndx);
	ap_uint<8> hEntries=lCnt[cIndx];
	if( oLRouter.isBrl == 1 ) 
	{
		if( oLRouter.layerId == 1 )
			hBrl.m1[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
		else if( oLRouter.layerId == 2 )
			hBrl.m2[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
		else if( oLRouter.layerId == 3 )
			hBrl.m3[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	}
	*(&lCnt[cIndx])=hEntries+1;
}
void SimpleRouter2S(const BXType bx
	, const ap_uint<kNBits_DTC> hLnkStub
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, ap_uint<8> lCnt[kNMemories]
	, StubsBarrel2S& hBrl) 
{
	ap_uint<8> cIndx;
	LayerRouterOutputPort oLRouter;
	
	DecodeIS( hLnkStub, hLinkWord, oLRouter );
	GetCntrIndex( oLRouter.isBrl, oLRouter.layerId , oLRouter.phiRegion, cIndx);
	ap_uint<8> hEntries=lCnt[cIndx];
	if( oLRouter.isBrl == 1 ) 
	{
		if( oLRouter.layerId == 4 )
			hBrl.m1[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
		else if( oLRouter.layerId == 5 )
			hBrl.m2[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
		else if( oLRouter.layerId == 6 )
			hBrl.m3[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	}
	// else
	// {
	// 	if( oLRouter.layerId == 1 )
	// 		hDsk.m1[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	// 	else if( oLRouter.layerId == 2 )
	// 		hDsk.m2[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	// 	else if( oLRouter.layerId == 3 )
	// 		hDsk.m3[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	// 	else if( oLRouter.layerId == 4 )
	// 		hDsk.m4[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	// 	else if( oLRouter.layerId == 5 )
	// 		hDsk.m5[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub,hEntries);
	// }
	*(&lCnt[cIndx])=hEntries+1;
}
void GenericRouter(const BXType bx, 
	RouterInputPort inPS, RouterInputPort in2S
	, StubsBarrelPS& hBrlPS, StubsBarrel2S& hBrl2S)
{

	#pragma HLS clock domain=fast_clock 
	#pragma HLS stream variable=inPrt.hStubs depth=1 
	#pragma HLS array_partition variable=inPS.hStubs cyclic factor=2 dim=1 
	
	// local array to store number of
	// entries in memories 
	ap_uint<8> lCnt[48];
	#pragma HLS array_partition variable=lCnt complete 
	// clear counters
	LOOP_cntrlClear : 
	for( int cIndex=0; cIndex < 48 ; cIndex++)
	{
		#pragma HLS unroll 
		lCnt[cIndex]=0;
	}
	
	
	LOOP_OuterStubLoop :
	for (int cOtrLoop=0; cOtrLoop<kMaxStubsFromLink ; cOtrLoop++)
	{
		#pragma HLS pipeline II=1
		if(inPS.hStubs[cOtrLoop]!=0)
		{
			SimpleRouterPS(bx, inPS.hStubs[cOtrLoop], inPS.hLinkWord, lCnt, hBrlPS); 
		}
		if(in2S.hStubs[cOtrLoop]!=0)
		{
			SimpleRouter2S(bx, in2S.hStubs[cOtrLoop], in2S.hLinkWord, lCnt, hBrl2S); 
		}
	}
}

void GenericRouterPS(const BXType bx
	, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, StubsBarrelPS& hBrl
	, StubsDiskPS& hDsk)
{

	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS stream variable=hIputLink depth=1 
	//#pragma HLS array_partition variable=inPS.hStubs block factor=4 dim=1 
	
	// local array to store number of
	// entries in memories 
	//ap_uint<kBRAMwidth> lCnt[48][kMaxStubsFromLink];
	//#pragma HLS array_partition variable=lCnt complete dim=0
	//ap_uint<8> lCnt[kNPSMemories];
	//#pragma HLS array_partition variable=lCnt complete dim=1
	// clear counters
	//LOOP_cntrlClear : 
	//for( int cIndex=0; cIndex < kNPSMemories ; cIndex++)
	//{
	//	#pragma HLS unroll 
	//	lCnt[cIndex]=0;
	//}
	//
	LOOP_ClearInputStubsPS : 
	for( int cPhiBn=0; cPhiBn<kNRegionsLayer1; cPhiBn++)
	{
		#pragma HLS unroll // clear four phi regions at a time ..
		(&hBrl.m1[cPhiBn])->clear(bx);
		if(cPhiBn<kNRegions)
		{
			(&hBrl.m2[cPhiBn])->clear(bx);
			(&hBrl.m3[cPhiBn])->clear(bx);
			(&hDsk.m1[cPhiBn])->clear(bx);
			(&hDsk.m2[cPhiBn])->clear(bx);
			(&hDsk.m3[cPhiBn])->clear(bx);
			(&hDsk.m4[cPhiBn])->clear(bx);
			(&hDsk.m5[cPhiBn])->clear(bx);
		}
	}

	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		//#pragma HLS unroll factor = 2
		if( hIputLink[cStubCounter] != 0 )
		{
			ap_uint<8> cIndex;
			ap_uint<1> hIsValid;
			ap_uint<1> hIs2S;
			ap_uint<1> hIsBrl;
			ap_uint<3> layerId;
			ap_uint<3> phiRegion;
			ap_uint<kBRAMwidth> hStub;
			DecoderInputStub( hIputLink[cStubCounter], hLinkWord, hIsValid, hIs2S, hIsBrl, layerId, phiRegion,hStub );
			//GetCntrIndexPS( oLRouter.isBrl, oLRouter.layerId , oLRouter.phiRegion, cIndex); 
			//ap_uint<8> hEntries=lCnt[cIndex];
			// #ifndef __SYNTHESIS__
			// 	std::cout << "\tRouting stub  "
			// 		<< std::bitset<8>(inPS.hStubs[cStubCounter])
			// 		<< " -- memory has "
			// 		<< hEntries
			// 		<< " -- entries"
			// 		<< "\n";
			// #endif
			// fill memories 
			if( hIsBrl == 1 )
			{
				if( layerId == 1 )
					hBrl.m1[phiRegion].write_mem(bx, hStub);
				else if( layerId == 2 )
					hBrl.m2[phiRegion].write_mem(bx, hStub);
				else if( layerId == 3 )
					hBrl.m3[phiRegion].write_mem(bx, hStub);
			}	
			else
			{
				if( layerId == 1 )
					hDsk.m1[phiRegion].write_mem(bx, hStub);
				else if( layerId == 2 )
					hDsk.m2[phiRegion].write_mem(bx, hStub);
				else if( layerId == 3 )
					hDsk.m3[phiRegion].write_mem(bx, hStub);
				else if( layerId == 4 )
					hDsk.m4[phiRegion].write_mem(bx, hStub);
				else if( layerId == 5 )
					hDsk.m5[phiRegion].write_mem(bx, hStub);
			}
			//lCnt[cIndex]=hEntries+1;
		}
	}
}
void GenericRouter2S(const BXType bx, 
	RouterInputPort inPS,
	StubsBarrel2S& hBrl,
	StubsDisk2S& hDsk)
{

	#pragma HLS clock domain=fast_clock 
	#pragma HLS stream variable=inPrt.hStubs depth=1 
	//#pragma HLS array_partition variable=inPS.hStubs block factor=4 dim=1 
	ap_uint<8> lCnt[kN2SMemories];
	#pragma HLS array_partition variable=lCnt complete dim=1
	// clear counters
	LOOP_cntrlClear : 
	for( int cIndex=0; cIndex < kN2SMemories ; cIndex++)
	{
		#pragma HLS unroll 
		lCnt[cIndex]=0;
	}
	
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		//#pragma HLS unroll factor = 2
		if( inPS.hStubs[cStubCounter] != 0 )
		{
			ap_uint<8> cIndex;
			LayerRouterOutputPort oLRouter;
			DecodeIS( inPS.hStubs[cStubCounter], inPS.hLinkWord, oLRouter );
			GetCntrIndex2S( oLRouter.isBrl, oLRouter.layerId , oLRouter.phiRegion, cIndex); 
			ap_uint<8> hEntries=lCnt[cIndex];
			// fill memories 
			if( oLRouter.isBrl == 1 )
			{
				if( oLRouter.layerId == 4 )
					hBrl.m1[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 5 )
					hBrl.m2[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 6 )
					hBrl.m3[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
			}	
			else
			{
				if( oLRouter.layerId == 1 )
					hDsk.m1[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 2 )
					hDsk.m2[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 3 )
					hDsk.m3[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 4 )
					hDsk.m4[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
				else if( oLRouter.layerId == 5 )
					hDsk.m5[oLRouter.phiRegion].write_mem(bx, oLRouter.hStub, hEntries);
			}
			lCnt[cIndex]=hEntries+1;
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
	
	MemoriesPS hPSMemories;
	ap_uint<8> hEntries;
	LayerRouterOutputPort oLRouter;
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if( inPrt.hStubs[cStubCounter] != 0 ) 
		{
			// working 
			#pragma HLS inline
			DecodeIS( inPrt.hStubs[cStubCounter], inPrt.hLinkWord, oLRouter );
			assert( oLRouter.is2S == 0 ); 
			if( oLRouter.isBrl == 1 )
			{
				InputStub<BARRELPS> hStub(oLRouter.hStub);
				if( oLRouter.layerId == 1 )
				{
					hEntries = (hBrl.m1[oLRouter.phiRegion]).getEntries(bx);
					(hBrl.m1[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 2 )
				{
					hEntries = (hBrl.m2[oLRouter.phiRegion]).getEntries(bx);
					(hBrl.m2[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 3 )
				{
					hEntries = (hBrl.m3[oLRouter.phiRegion]).getEntries(bx);
					(hBrl.m3[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
			}
			else if( oLRouter.isBrl == 0 )
			{
				InputStub<DISKPS> hStub(oLRouter.hStub);
				if( oLRouter.layerId == 1 )
				{
					hEntries = (hDsk.m1[oLRouter.phiRegion]).getEntries(bx);
					(hDsk.m1[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 2 )
				{
					hEntries = (hDsk.m2[oLRouter.phiRegion]).getEntries(bx);
					(hDsk.m2[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 3 )
				{
					hEntries = (hDsk.m3[oLRouter.phiRegion]).getEntries(bx);
					(hDsk.m3[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 4 )
				{
					hEntries = (hDsk.m4[oLRouter.phiRegion]).getEntries(bx);
					(hDsk.m4[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
				else if( oLRouter.layerId == 5 )
				{
					hEntries = (hDsk.m5[oLRouter.phiRegion]).getEntries(bx);
					(hDsk.m5[oLRouter.phiRegion]).write_mem(bx, hStub, hEntries );
				}
			}
		}
	}
}
void RoutePS(const BXType bx
	, RouterInputPort inPrt
	, MemoriesPS& hPS) 
{
	#pragma HLS clock domain=fast_clock 
	#pragma HLS stream variable=inPrt.hStubs depth=1 

	CountersPS hCntrsPS;
	// one memory for each coarse phi bin 
	LOOP_ClearInputStubsL1PS : 
	for( int cPhiBn=0; cPhiBn<kNRegionsLayer1; cPhiBn++)
	{
		#pragma HLS unroll  // clear 8 phi regions at a time ..
		(&hPS.l1[cPhiBn])->clear(bx);
		hCntrsPS.l1[cPhiBn]=0;
	}
	//
	LOOP_ClearInputStubsPS : 
	for( int cPhiBn=0; cPhiBn<kNRegions; cPhiBn++)
	{
		#pragma HLS unroll // clear four phi regions at a time ..
		(&hPS.l2[cPhiBn])->clear(bx);
		(&hPS.l3[cPhiBn])->clear(bx);
		(&hPS.d1[cPhiBn])->clear(bx);
		(&hPS.d2[cPhiBn])->clear(bx);
		(&hPS.d3[cPhiBn])->clear(bx);
		(&hPS.d4[cPhiBn])->clear(bx);
		(&hPS.d5[cPhiBn])->clear(bx);
		hCntrsPS.l1[cPhiBn]=0;
		hCntrsPS.l2[cPhiBn]=0;
		hCntrsPS.l3[cPhiBn]=0;
		hCntrsPS.d1[cPhiBn]=0;
		hCntrsPS.d1[cPhiBn]=0;
		hCntrsPS.d2[cPhiBn]=0;
		hCntrsPS.d3[cPhiBn]=0;
		hCntrsPS.d4[cPhiBn]=0;
		hCntrsPS.d5[cPhiBn]=0;
	}
	
	LOOP_OuterStubLoop :
	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
	{
		#pragma HLS pipeline II=1
		if( inPrt.hStubs[cStubCounter] != 0 ) 
		{
			// working 
			#pragma HLS inline
			StubRouter(bx, inPrt.hStubs[cStubCounter], inPrt.hLinkWord, hCntrsPS, hPS);
		}
	}
}
