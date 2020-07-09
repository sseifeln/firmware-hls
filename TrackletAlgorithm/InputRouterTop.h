#ifndef TrackletAlgorithm_InputRouterTop_h
#define TrackletAlgorithm_InputRouterTop_h


#include "Constants.h"
// # bits encoding stub addres on link 
#include "hls_math.h"
#include "InputStubMemory.h"
#include "AllStubMemory.h"
#include "VMRouter.h"

// link map
constexpr int kLINKMAPwidth = 20;
// maxumum number of IR memories  
constexpr unsigned int kMAXIRMemories = 8 + 4 * 3; 
constexpr unsigned int kMAXNStubsPerPhi = 30; 
constexpr unsigned int kMAXIRStubs = 8 * kMAXNStubsPerPhi + 4 * 3 * kMAXNStubsPerPhi ; 
#ifndef __SYNTHESIS__
	#include <bitset> 
#endif
constexpr unsigned int kNPSMemories=36;
constexpr unsigned int kN2SMemories=32;
constexpr unsigned int kNMemories=48;

constexpr unsigned int kNRegionsLayer1 = 8;
constexpr unsigned int kNRegions = 4;  
constexpr unsigned int kNRbinsPhiCorr = 3 ;

constexpr unsigned int kNBns = 4; 
constexpr unsigned int kMAXNStubsPerBn = kMaxStubsFromLink; 
constexpr int kMaxSizeArray = kMAXNStubsPerBn*kNBns; 


// #define PRAGMA_SUB(x) _Pragma (#x)
// #define DO_PRAGMA(x) PRAGMA_SUB(x)
// #define STREAM_IN_DEPTH 8
// // Legal pragmas
// DO_PRAGMA(HLS stream depth=STREAM_IN_DEPTH variable=InStream)
// #pragma HLS stream depth=8 variable=OutStream


#define PHI_CORRECTION true
#define IR_DEBUG true
// LUT with phi corrections to the nominal radius. Only used by layers.
// Values are determined by the radius and the bend of the stub.
static const int phicorrtable_L1[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL1.txt"
;
static const int phicorrtable_L2[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL2.txt"
;
static const int phicorrtable_L3[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL3.txt"
;
static const int phicorrtable_L4[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL4.txt"
;
static const int phicorrtable_L5[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL5.txt"
;
static const int phicorrtable_L6[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL6.txt"
;

template<regionType ASType> 
void GetPhiBin(const ap_uint<kNBits_DTC> inStub
	, ap_uint<3> pLyrId 
	, ap_uint<3> &phiBn )
{
	#pragma HLS pipeline II=1 
	#pragma HLS inline 
	ap_uint<5> hPhiMSB = AllStub<ASType>::kASPhiMSB;
	ap_uint<5> hPhiLSB;
	if( pLyrId == 1 && ASType == BARRELPS ) 
		hPhiLSB = AllStub<ASType>::kASPhiMSB-(3-1);
	else
		hPhiLSB = AllStub<ASType>::kASPhiMSB-(2-1);

	// check if phi of input stub needs to be corrected 
	if( PHI_CORRECTION ) 
	{
		AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
		if( (ASType == BARRELPS || ASType == BARREL2S) )
		{
			// Corrected phi, i.e. phi at nominal radius (what about disks?)
			// for now I'm going to use the one from the VMRouter 
			// because .. they should be the same 
			auto hPhi = getPhiCorr<ASType>(hStub.getPhi(), hStub.getR(), hStub.getBend(), phicorrtable_L1); 
			hStub.setPhi(hPhi);
		}
		phiBn = hStub.raw().range(hPhiMSB,hPhiLSB) & 0x7;
	}
	else
		phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;
}

//templated function that 
//can be called by the top level function 
// template<unsigned int nLayers, unsigned int nMemories>
// void InputRouterGeneric( const BXType bx
// 	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
// 	, const ap_uint<kLINKMAPwidth> hLinkWord 
// 	, InputStubMemory<TRACKER> hMemories[nMemories])
// {
// 	#pragma HLS inline 

// 	ap_uint<3> hNLayers = hLinkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-3);
// 	ap_uint<1> hIs2S = hLinkWord.range(kLINKMAPwidth-3,kLINKMAPwidth-4);
	
// 	// prepare variables needed 
// 	// to be able to move through 
// 	// the array of memories 
// 	unsigned int cNMemories=0; 
// 	// hard code things for now 
// 	constexpr unsigned int kNLayers=nLayers;
// 	ap_uint<4> hNPhiBns[kNLayers]; //at most 4 layers
// 	ap_uint<1> hBrlBits[kNLayers]; //at most 4 layers

// 	#pragma HLS array_partition variable=hNPhiBns complete
// 	#pragma HLS array_partition variable=hBrlBits complete
// 	LOOP_GetNPhiBns : 
// 	for( unsigned int cIndx=0; cIndx < kNLayers; cIndx++)
// 	{
// 		#pragma HLS unroll 
// 		if( cIndx < hNLayers )
// 		{
// 			ap_uint<4> hWrd = hLinkWord.range(4*cIndx+3,4*cIndx);
// 			ap_uint<1> hIsBrl = hWrd.range(1,0);
// 			ap_uint<3> hLyrId = hWrd.range(3,1);
// 			hNPhiBns[cIndx] = ( (hIs2S==0) && hLyrId==1 && hIsBrl) ? 8 : 4; 
// 			hBrlBits[cIndx] = hIsBrl;
// 			//cNPhiBns[cIndx] = (!cIs2S && hLyrId==1 && hIsBrl) ? 8 : 4; 
// 			//cBrlBits[cIndx] = hIsBrl;
// 			#ifndef __SYNTHESIS__
// 				if( IR_DEBUG )
// 				{
// 					std::cout << "Lyr#" << cIndx
// 						<< " encoded word " << std::bitset<4>(hWrd)
// 						<< " - " <<  hNPhiBns[cIndx]
// 						<< " phi bins"
// 						<< "\n"; 
// 				}
// 			#endif
// 			cNMemories += (unsigned int)(hNPhiBns[cIndx]);
// 		}
// 	}
// 	// clear memories and stub counter
// 	constexpr unsigned int kNMemories=nMemories;
// 	ap_uint<8> hNStubs[kNMemories];
// 	#pragma HLS array_partition variable=hNStubs complete
// 	LOOP_ClearOutputMemories : 
// 	for( unsigned int cMemIndx=0; cMemIndx<kNMemories; cMemIndx++)
// 	{
// 		// clear four phi regions at a time ..
// 		#pragma HLS unroll 
// 		hNStubs[cMemIndx]=0;
// 		(&hMemories[cMemIndx])->clear(0);
// 		//local_memories[cMemIndx].clear(0);
// 	}
	
// 	ap_uint<kBRAMwidth> hEmpty=ap_uint<kBRAMwidth>(0); 
// 	LOOP_OuterStubLoop :
// 	for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter++)
// 	{
// 		#pragma HLS pipeline II=1
// 		// decode stub 
// 		// check which memory 
// 		ap_uint<kNBits_DTC> hStub = hStubs[cStubCounter]; 
// 		if( hStub == 0 )
// 			continue;

// 		ap_uint<3> hEncLyr = ap_uint<3>(hStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3) ;
// 		ap_uint<kBRAMwidth> hStbWrd = hStub.range(kBRAMwidth-1,0);
// 		//get 36 bit word 
// 		InputStub<TRACKER> hMemWord(hStbWrd);
// 		//decode link wrd for this layer 
// 		ap_uint<4> hWrd = hLinkWord.range(4*hEncLyr+3,4*hEncLyr);
// 		ap_uint<1> hIsBrl = hWrd.range(1,0);
// 		ap_uint<3> hLyrId = hWrd.range(3,1);
// 		//get phi bin 
// 		ap_uint<3> hPhiBn;
// 		if( (hIs2S==0) && hIsBrl ) 
// 			GetPhiBin<BARRELPS>(hStub, hLyrId, hPhiBn);
// 		else if( (hIs2S==0) && !hIsBrl )
// 			GetPhiBin<DISKPS>(hStub, hLyrId, hPhiBn);
// 		else if( hIsBrl )
// 			GetPhiBin<BARREL2S>(hStub, hLyrId, hPhiBn);
// 		else
// 			GetPhiBin<DISK2S>(hStub, hLyrId, hPhiBn);
		
// 		//update index 
// 		unsigned int cIndx=0;
// 		//unsigned int cMemIndx=0;
// 		//unsigned int cBaseIndx=cMemIndx;
// 		LOOP_UpdateIndxLoop :
// 		for( int cLyr=0 ; cLyr<kNLayers;cLyr++)
// 		{
// 			#pragma HLS unroll 
// 			//update index 
// 			cIndx += (cLyr >= hEncLyr ) ? 0 : (unsigned int)(hNPhiBns[cLyr]);
// 		}
// 		//write to memory 
// 		unsigned int cMemIndx = cIndx+hPhiBn;
// 		ap_uint<8> hEntries = hNStubs[cMemIndx];
// 		#ifndef __SYNTHESIS__
// 			if( IR_DEBUG )
// 			{
// 				std::cout << "\t.. Stub : " << std::hex 
// 					<< hStbWrd
// 					<< std::dec 
// 					<< "[ LyrId " << hLyrId 
// 					<< " ] IsBrl bit " << +hIsBrl
// 					<< " PhiBn#" << +hPhiBn 
// 					<< " Mem#" << cMemIndx 
// 					<< " Current number of entries " << +hEntries 
// 					<< "\n"; 
// 			}
// 		#endif
// 		(&hMemories[cMemIndx])->write_mem(0,hMemWord,hEntries);
// 		//(&local_memories[cIndx])->write_mem(bx,hMemWord,hEntries);
// 		hNStubs[cMemIndx]=hEntries+1;
// 	}
// }

void InputRouterTop( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<TRACKER> hMemories[20]);


#endif


