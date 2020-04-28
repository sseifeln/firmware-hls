#include "InputRouterTop.h"
#include "InputRouter.cc"

// //route stubs for 2S memories only 
void InputRouter2S(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk)
{
	#pragma HLS clock domain=slow_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS interface ap_fifo port=hIputLink

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
		if (hIputLink.read_nb(hWord))
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
void InputRouterPS(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk)
{
	#pragma HLS clock domain=fast_clock 
	#pragma HLS interface ap_none port=hDTCMapEncoded
	#pragma HLS interface ap_fifo port=hIputLink

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
		if (hIputLink.read_nb(hWord))
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


void InputRouterTop(const BXType bx, 
	hls::stream<ap_uint<kNBits_DTC>> & hIputLink, 
	const ap_uint<kLINKMAPwidth> hLinkWord,
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk)
{

	#pragma HLS interface ap_none port=hLinkWord
	#pragma HLS stream variable=hIputLink
	#pragma HLS clock domain=fast_clock 

	
	//hls::stream<ap_uint<kBRAMwidth>> hStrms[8];
	//hls::stream<ap_uint<8>> hRoutingInfo;
	//#pragma HLS stream variable=hStrms depth=1 
	//#pragma HLS stream variable=hRoutingInfo depth=1 
	//RegionRouter(hIputLink, hLinkWord, hRoutingInfo, hStrms);
	//MemoryRouter(bx, hStrms, hRoutingInfo, hBrl, hDsk); 
	
	// LOOP_InputStubsTop : 
	// for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++) 
	// {	
	// 	#pragma HLS pipeline II=1
	// 	if( hRoutingInfo.read_nb(hRoutingWord) )
	// 	{
	// 		ap_uint<1> hIs2S = hRoutingWord.range(7,7); 
	// 		ap_uint<1> hIsBrl = hRoutingWord.range(6,6); 
	// 		ap_uint<3> hLyr = hRoutingWord.range(5,3); 
	// 		ap_uint<3> hPhi = hRoutingWord.range(2,0); 
	// 		if( hStrms[hPhi].read_nb(hWord) )
	// 		{
	// 			#ifndef __SYNTHESIS__
	// 				std::cout << "\t\tRouting word is "
	// 					<< std::bitset<8>(hRoutingWord)
	// 					<< " -- is2S bit "
	// 					<< hIs2S 
	// 					<< " -- barrel bit "
	// 					<< hIsBrl
	// 					<< " -- layer "
	// 					<< hLyr 
	// 					<< " -- stub word is "
	// 					<< std::bitset<kBRAMwidth>(hWord)
	// 					<< "\n";
	// 			#endif
	// 			if( hIsBrl == 1 )
	// 			{
	// 				InputStub<BARRELPS> hStub(hWord);
	// 				if( hLyr == 1 )
	// 					hBrl.m1[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 2 )
	// 					hBrl.m2[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 3 )
	// 					hBrl.m3[hPhi].write_mem(bx, hStub );
	// 			}
	// 			else
	// 			{
	// 				InputStub<DISKPS> hStub(hWord);
	// 				if( hLyr == 1 )
	// 					hDsk.m1[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 2 )
	// 					hDsk.m2[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 3 )
	// 					hDsk.m3[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 4 )
	// 					hDsk.m4[hPhi].write_mem(bx, hStub );
	// 				else if( hLyr == 5 )
	// 					hDsk.m5[hPhi].write_mem(bx, hStub );
	// 			}
	// 		}
	// 	}
	// }
	// for( int cPhi=0; cPhi < 8; cPhi++)
	// {
	// 	#pragma HLS pipeline II=1
	// 	//#pragma HLS unroll 
	// 	while(hStrms[cPhi].read_nb(hWord)) 
	// 	{
	// 		#ifndef __SYNTHESIS__
	// 			std::cout << "Input word is "
	// 				<< std::bitset<kBRAMwidth>(hWord)
	// 				<< "\n";
	// 		#endif
	// 	}
	// }
	//#pragma HLS dependence variable=hRoutingWord inter RAW true
	// for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	// {
	// 	#pragma HLS pipeline II=1
	// 	RegionRouter(hIputLink, hLinkWord, hRoutingWord, hStrms);
	// 	//ap_uint<1> hIs2S = hRoutingWord & 0x80; 
	// 	//ap_uint<1> hIsBrl = hRoutingWord & 0x40; 
	// 	//ap_uint<3> hLyr = hRoutingWord & 0x38; 
	// 	for( int cPhi=0; cPhi < 8; cPhi++)
	// 	{
	// 		#pragma HLS unroll 
	// 		if(hStrms[cPhi].read_nb(hWord))
	// 		{
	// 			#ifndef __SYNTHESIS__
	// 				std::cout << "Input word is "
	// 					<< std::bitset<kBRAMwidth>(hWord)
	// 					<< "\n";
	// 			#endif
	// 			// if(hStrms[hPhi].read_nb(hWord)) 
	// 			// {
	// 			// 	if( hIsBrl == 1 )
	// 			// 	{
	// 			// 		InputStub<BARRELPS> hStub(hWord);
	// 			// 		if( hLyr == 1 )
	// 			// 			hBrl.m1[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 2 )
	// 			// 			hBrl.m2[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 3 )
	// 			// 			hBrl.m3[hPhi].write_mem(bx, hStub );
	// 			// 	}
	// 			// 	else
	// 			// 	{
	// 			// 		InputStub<DISKPS> hStub(hWord);
	// 			// 		if( hLyr == 1 )
	// 			// 			hDsk.m1[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 2 )
	// 			// 			hDsk.m2[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 3 )
	// 			// 			hDsk.m3[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 4 )
	// 			// 			hDsk.m4[hPhi].write_mem(bx, hStub );
	// 			// 		else if( hLyr == 5 )
	// 			// 			hDsk.m5[hPhi].write_mem(bx, hStub );
	// 			// 	}
	// 			// 	#ifndef __SYNTHESIS__
	// 			// 		std::cout << "Is2S " << hIs2S 
	// 			// 			<< " -- IsBrl "
	// 			// 			<< hIsBrl 
	// 			// 			<< " -- Lyr "
	// 			// 			<< hLyr
	// 			// 			<< " -- Phi "
	// 			// 			<< hPhi
	// 			// 			<< " -- "
	// 			// 			<< std::bitset<kBRAMwidth>(hWord)
	// 			// 			<< "\n";
	// 			// 	#endif
	// 			// }
	// 		}
	// 	}
	// }
	//ap_uint<1> hValid;
	//hls::stream<ap_uint<kBRAMwidth>> hBrlStrms[3];
	//hls::stream<ap_uint<kBRAMwidth>> hEcapStrms[5];
	//LyrRouter(hIputLink,  hLinkWord, hBrlStrms, hEcapStrms);
	//RegionRouter<3>(hBrlStrms);
	//RegionRouter<5>(hEcapStrms);

	// #pragma HLS stream variable=hBrlStrm depth=1 
	// #pragma HLS stream variable=hEcapStrm depth=1 
	// for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
	// {
	// 	#pragma HLS pipeline II=1
	// 	TkRegionRouter(hIputLink, hLinkWord  , hValid, hBrlStrm , hEcapStrm ); 
	// 	if( hValid == 1 )
	// 	{

	// 	}
	// 	else
	// 		break;
	// 	// ap_uint<1> hValid;
	// 	// ap_uint<1> hType;
	// 	// ap_uint<1> hRegion;
	// 	// ap_uint<3> hLyr;
	// 	// ap_uint<3> hPhi; 
	// 	// ap_uint<kBRAMwidth> hWord; 
	// 	// GenericParser(hIputLink, hLinkWord, hValid, hType , hRegion, hLyr, hPhi , hWord); 
	// 	// if( hValid )
	// 	// {
	// 	// 	#ifndef __SYNTHESIS__
	// 	// 		std::cout << "Type " << hType 
	// 	// 			<< " -- Region "
	// 	// 			<< hRegion 
	// 	// 			<< " -- Lyr "
	// 	// 			<< hLyr
	// 	// 			<< " -- Phi "
	// 	// 			<< hPhi
	// 	// 			<< " -- "
	// 	// 			<< std::bitset<kBRAMwidth>(hWord)
	// 	// 			<< "\n";
	// 	// 	#endif
	// 	// }
	// }
}