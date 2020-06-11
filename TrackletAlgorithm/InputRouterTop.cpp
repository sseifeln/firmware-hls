#include "InputRouterTop.h"


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
		//(&L1[cPhiBn])->clear(bx);
		if( cPhiBn < 4 )
		{
			n2[cPhiBn]=0;
			n3[cPhiBn]=0;
			n4[cPhiBn]=0;
			// (&L2[cPhiBn])->clear(bx);
			// (&L3[cPhiBn])->clear(bx);
			// (&L4[cPhiBn])->clear(bx);
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



void InputRouter_Generic( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, ap_uint<kBRAMwidth> hOutput[kMaxSizeArray] )
{

	#pragma HLS clock domain=slow_clock 
	constexpr unsigned int nBits_inputAddress = 8; 
	constexpr unsigned int nLayers = 4; 

	ap_uint<(nBits_inputAddress+1)> cNoMatch = (1<<nBits_inputAddress); 
	static ap_uint<(nBits_inputAddress+1)> data_out[nLayers][(1<<nBits_inputAddress)];
	PrepData : 
	{
		PrepData_LoopLyr : 
		for( unsigned int j = 0 ; j < nLayers; j++)
		{
			#pragma HLS unroll 
			PrepData_LoopAddr : 
			for( unsigned int k = (1<<nBits_inputAddress)-1; k > 0; k-- )
			{
				#pragma HLS unroll 
				data_out[j][k]= cNoMatch;
			}
			data_out[j][0] = cNoMatch;
		}
	}

	LayerRouter : 
	{
		Generic_LayerRouter<kNBits_DTC, nBits_inputAddress, nBits_inputAddress+1, nLayers >( hStubs , data_out );
		#ifndef __SYNTHESIS__
			std::cout << "After call .. " 
				<< "\n";
			for( unsigned int j = 0; j < (1 << nBits_inputAddress); j++ )
	 		{
	 			if( data_out[2][j] != cNoMatch )
	 			{
		 			std::cout << "\t...i" << +j
		 				<< " : address  " 
		 				<< +data_out[2][j]
		 				<< "\n";
		 		}
			}
		#endif
	}
	*(&hOutput[0])= 0;  
	// working 
	// // eventually replace this with a LUT in the IR 
	// #pragma HLS interface ap_none port=hLinkWord
	// // input BRAM - stubs from link x 
	// #pragma HLS RESOURCE variable=hStubs core=RAM_2P
	// // paritition to be able to access 
	// // multiple stubs per clock cycle 
	// // cyclic allows you to acess factor*2 
	// // consecutive elements at a time 
	// #pragma HLS array_partition variable=hStubs cyclic factor=2 // 32 
	// constexpr unsigned int cNlayers=4; 
	// constexpr unsigned int cStubIncrement=4; // factor*2 
	// constexpr unsigned int cNloops=(1<<nBits_inputAddress)/cStubIncrement;

	// // output array 
	// // able to write twice per clock cycle 
	// #pragma HLS interface bram port=hOutput
 // 	#pragma HLS RESOURCE variable=hOutput core=RAM_T2P_BRAM
	// #pragma HLS array_partition variable=hOutput cyclic factor=8 // number of layers   
	// //#pragma HLS DEPENDENCE variable=hOutput inter false
	// //#pragma HLS DEPENDENCE variable=hOutput intra false
	

	// unsigned int cLpIndex=0;
	// //static ap_uint<(1<<nBits_inputAddress)> cLastWrittenAddress[cNlayers][cNloops][cStubIncrement]={0};
	// //#pragma HLS array_partition variable=cLastWrittenAddress complete
	// //#pragma HLS array_partition variable=cLastWrittenAddress complete dim=1 

	// // constexpr unsigned int cNbits=2; // each iteration reads 8 stubs 
	// // constexpr unsigned int cArraySize=(1 << cNbits);
	// // ap_uint<kBRAMwidth> hTmp[cNlayers][cArraySize]={0};
	// // #pragma HLS array_partition variable=hTmp complete
	// constexpr unsigned int cNbits=2; // input address is 256 
	// constexpr unsigned int cArraySize=(1 << cNbits);
	// static ap_uint<kBRAMwidth> hTmpL1[cArraySize]={0};
	// #pragma HLS array_partition variable=hTmpL1 complete
	// static ap_uint<kBRAMwidth> hTmpL2[cArraySize]={0};
	// #pragma HLS array_partition variable=hTmpL2 complete
	// static ap_uint<kBRAMwidth> hTmpL3[cArraySize]={0};
	// #pragma HLS array_partition variable=hTmpL3 complete
	// static ap_uint<kBRAMwidth> hTmpL4[cArraySize]={0};
	// #pragma HLS array_partition variable=hTmpL4 complete
	// ap_uint<kBRAMwidth> cNoMatch=0x00;
	// bool cDone=false;
	// LOOP_ReadLoop:
	// for (unsigned i=0; i < (1 << nBits_inputAddress) ; i+=cStubIncrement)
	// {
	// 	#pragma HLS pipeline II=1 
	// 	if(  hStubs[i] == 0 )
	// 		continue;

	// 	LOOP_FilterLoop:
	// 	for( unsigned int j = 0;  j < cStubIncrement ; j++ )
	// 	{
	// 		#pragma HLS unroll
	// 		ap_uint<nBits_inputAddress> addr_r = i + j; 
	// 		ap_uint<kNBits_DTC> data_in = hStubs[addr_r];
	// 		if(  data_in == 0 )
	// 			continue;
			
	// 		// decode layer 
	//  		ap_uint<2> cLyrId = data_in.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	//  		// push this element into the temporary arrays 
	//  		// used to store information from this iteration 
	//  		ShiftData : 
	//  		{
	//  			// LOOP_LyrLoop: 
	//  			// for( unsigned int cLyr=0; cLyr < cNlayers; cLyr++)
	//  			// {
	//  			// 	#pragma HLS unroll 
	//  			// 	LOOP_ShiftData:
	// 	 		// 	for( unsigned int k = cArraySize-1; k > 0; k-- )
	// 	 		// 	{
	//  			// 		hTmp[cLyr][k]= hTmp[cLyr][k-1];
	//  			// 	}
	//  			// 	// first element --> new data 
	//  			// 	hTmp[cLyr][0]=(cLyrId==cLyr) ? data_in.range(kBRAMwidth-1,0) : cNoMatch;
	//  			// }
	//  			// shift all other elements over by one 
	// 	 		LOOP_ShiftData:
	// 	 		for( unsigned int k = cArraySize-1; k > 0; k-- )
	// 	 		{
	// 	 			hTmpL1[k]= hTmpL1[k-1];
	// 				hTmpL2[k]= hTmpL2[k-1];
	// 				hTmpL3[k]= hTmpL3[k-1];
	// 				hTmpL4[k]= hTmpL4[k-1];
	// 	 		}
	// 	 		hTmpL1[0]= (cLyrId == 0 ) ? data_in.range(kBRAMwidth-1,0) : cNoMatch;
	// 	 		hTmpL2[0]= (cLyrId == 1 ) ? data_in.range(kBRAMwidth-1,0) : cNoMatch;
	// 	 		hTmpL3[0]= (cLyrId == 2 ) ? data_in.range(kBRAMwidth-1,0) : cNoMatch;
	// 	 		hTmpL4[0]= (cLyrId == 3 ) ? data_in.range(kBRAMwidth-1,0) : cNoMatch;
	// 	 	}
	//  		//*(&hOutput[addr_r])= ( data_in != 0 ) ? 1 : 0; 
	// 	}

	// 	#ifndef __SYNTHESIS__
	// 		std::cout << "Loop" 
	// 			<< cLpIndex 
	// 			<< "\n";
	// 		// only need to look at the first [cStubIncrement]
	// 		// that's how many I can push back in one clock 
	// 		// for( unsigned int j = 0; j < cStubIncrement; j++ )
	//  	// 	{
	//  	// 		std::cout << "\t...i" << +j
	//  	// 			<< " : " 
	//  	// 			<< std::bitset<kBRAMwidth>(hTmpL1[j])
	//  	// 			<< "\n";
	// 		// }
	// 	#endif
	// 	LOOP_FillLoop:
	// 	{
	// 		*(&hOutput[0])= hTmpL1[0];
	// 		*(&hOutput[1])= hTmpL1[1];
	// 		*(&hOutput[8])= hTmpL1[2];
	// 		*(&hOutput[9])= hTmpL1[3];
	// 	}
	// 	cLpIndex++;
	// }

	// working 


	//unsigned int cReadIndexTmp = cArraySize-1;
	//LOOP_FillLoop : 
	// for( unsigned i=0; i < (1 << nBits_inputAddress) ; i+=(cNlayers*2))
	// {
	// 	#pragma HLS pipeline II=1 
	// 	// first 
	// 	*(&hOutput[i+0])= hTmpL1[cReadIndexTmp];
	// 	*(&hOutput[i+1])= hTmpL2[cReadIndexTmp];
	// 	*(&hOutput[i+2])= hTmpL3[cReadIndexTmp];
	// 	*(&hOutput[i+3])= hTmpL4[cReadIndexTmp]; 
	// 	// second 
	// 	*(&hOutput[i+4])= hTmpL1[cReadIndexTmp-1];
	// 	*(&hOutput[i+5])= hTmpL2[cReadIndexTmp-1];
	// 	*(&hOutput[i+6])= hTmpL3[cReadIndexTmp-1];
	// 	*(&hOutput[i+7])= hTmpL4[cReadIndexTmp-1]; 
	// 	cReadIndexTmp = cReadIndexTmp - 2; 
	// }
	// local array of 32 bit words.. 
	//ap_uint<kBRAMwidth> hLocal_perLyr[kMaxSizeArray];
	//GenericRouter<4>( hStubs, hLinkWord, hOutput);
}
	//bool cLastThisIter = ( j == (cStubIncrement-1) );
	// get indices of array used to keep track of entries 
	//ap_uint<(1<<nBits_inputAddress)> cIndxLpNxt = (cLastThisIter)  ? cLpIndex+1 : cLpIndex;
	//ap_uint<(1<<nBits_inputAddress)> cIndxElNxt = (cLastThisIter) ?  0 : j+1;

// working 
// TO-DO : template this function 
// one input and one output array 
// the output array can be used to fill 
// as many memories for the VMR as needed 
// void InputRouter_Generic( const BXType bx
// 	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
// 	, const ap_uint<kLINKMAPwidth> hLinkWord 
// 	, ap_uint<kBRAMwidth> hOutput[kMaxSizeArray] )
// {
// 	#pragma HLS clock domain=fast_clock 
// 	// eventually replace this with a LUT in the IR 
// 	#pragma HLS interface ap_none port=hLinkWord
// 	// input BRAM - stubs from link x 
// 	#pragma HLS RESOURCE variable=hStubs core=RAM_T2P_BRAM
// 	// paritition to be able to access 
// 	// multiple stubs per clock cycle 
// 	// cyclic allows you to acess factor*2 
// 	// consecutive elements at a time 
// 	#pragma HLS array_partition variable=hStubs cyclic factor=4 // 32  
// 	constexpr int cStubIncrement=8; // factor*2 
	
// 	// output BRAM - all stubs from link x arranged by layer 
// 	#pragma HLS RESOURCE variable=hOutput core=RAM_T2P_BRAM
// 	//factor x number of layers  
// 	#pragma HLS array_partition variable=hOutput cyclic factor=16 // factor*4
	
// 	int nStubs=0;
// 	int cLpIndex=0; 
// 	LOOP_SortLoop:
// 	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter+=cStubIncrement)
// 	{
// 		#pragma HLS pipeline II=1
// 		LOOP_DecodeLoop: 
// 		for( int cEl=0; cEl < cStubIncrement ; cEl++)
// 		{
// 			int cIndex = cStubCounter+cEl;
// 			// one read per clock cycle 
// 			// store value in local variable 
// 			// now use local variable for everything else 
// 			// just to be sure 
// 			ap_uint<kNBits_DTC> cDTCword = hStubs[cIndex];
// 			if( cDTCword == 0 ) 
// 				continue;
			
// 			ap_uint<kBRAMwidth> cStub = cDTCword.range(kBRAMwidth-1,0);
// 			ap_uint<2> cLyrId= cDTCword.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
// 			ap_uint<1> cIs2S  =  hLinkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
// 			ap_uint<kLINKMAPwidth> cWrd = hLinkWord.range(4*cLyrId+3,4*cLyrId);
// 			ap_uint<1> cIsBrl =  cWrd.range(0,0) ;
// 			ap_uint<3> cTkLyr = cWrd.range(3,1) ;
// 			int cMaxPhiBins= ( cTkLyr == 1 && cIsBrl == 1 ) ? 8 : 4 ; 
// 			ap_uint<3> cPhiBn;
// 			if( cIs2S == 1 ) 
// 			{
// 				if( cIsBrl )
// 				{
// 					GetPhiBin<BARREL2S>(cDTCword, cTkLyr, cPhiBn);
// 				}
// 				else
// 				{
// 					GetPhiBin<DISK2S>(cDTCword, cTkLyr, cPhiBn);
// 				}
// 			}
// 			else
// 			{
// 				if( cIsBrl )
// 				{
// 					GetPhiBin<BARRELPS>(hStubs[cIndex], cTkLyr, cPhiBn);
// 				}
// 				else
// 				{
// 					GetPhiBin<DISKPS>(hStubs[cIndex], cTkLyr, cPhiBn);
// 				}	
// 			}
// 			assert( cPhiBn < cMaxPhiBins );
// 			#ifndef __SYNTHESIS__
// 				std::cout << "Stub : " << cIndex
// 				    << " number of stubs processed is " << nStubs
// 				    << " - tk layer is " << cTkLyr 
// 				    << " - phi bin is " << cPhiBn 
// 				    << " -- stubs will be stored in index " << (1 << 2 )*cEl + cLyrId 
// 				    << " -- local stub is " << std::bitset<kBRAMwidth>(cDTCword.range(kBRAMwidth-1,0))
// 					<< "\n";
// 			#endif
			
// 			LOOP_LyrLoop: 
// 			for( int cLyr=0; cLyr< 4 ; cLyr++)
// 			{
// 				int cSecondIndex=cLpIndex*(cStubIncrement*4) + 4*cEl + cLyr;
// 				assert( cSecondIndex < kMaxSizeArray );
// 				#ifndef __SYNTHESIS__
// 					std::cout << "\t\t.. LI " << cLpIndex 
// 						<< " index " << cSecondIndex
// 						<< " layer " << cLyr 
// 						<< "\n";
// 				#endif
// 				ap_uint<kBRAMwidth> cOutput=0;
// 				if( cLyr == cLyrId )
// 					cOutput=cDTCword.range(kBRAMwidth-1,0);
// 				*(&hOutput[cSecondIndex])=cOutput;
// 			}
// 			nStubs++;
// 		}
// 		cLpIndex++;
// 	}
// }