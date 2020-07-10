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
#define IR_DEBUG false

template<regionType ASType, unsigned int nCorrBns> 
void GetPhiBinBrl(const ap_uint<kNBits_DTC> inStub
	, const int kPhiCorrtable_L1[nCorrBns]
	, const int kPhiCorrtable_L2[nCorrBns]
	, const int kPhiCorrtable_L3[nCorrBns]
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

	AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
	// Corrected phi, i.e. phi at nominal radius (what about disks?)
	// for now I'm going to use the one from the VMRouter 
	// because .. they should be the same 
	const int* pCorrTable=kPhiCorrtable_L1;
	if( pLyrId == 2 || pLyrId == 5 ) 
		pCorrTable =kPhiCorrtable_L2; 
	else if( pLyrId == 3 || pLyrId == 6 )
		pCorrTable =kPhiCorrtable_L3; 
	// corrected phi 
	auto hPhi = getPhiCorr<ASType>(hStub.getPhi(), hStub.getR(), hStub.getBend(), pCorrTable); 
	hStub.setPhi(hPhi);
	phiBn = hStub.raw().range(hPhiMSB,hPhiLSB) & 0x7;
}

template<regionType ASType> 
void GetPhiBinDsk(const ap_uint<kNBits_DTC> inStub
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

	phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;
}


// template<regionType ASType> 
// void GetPhiBin(const ap_uint<kNBits_DTC> inStub
// 	, ap_uint<3> pLyrId 
// 	, ap_uint<3> &phiBn )
// {
// 	#pragma HLS pipeline II=1 
// 	#pragma HLS inline 
// 	ap_uint<5> hPhiMSB = AllStub<ASType>::kASPhiMSB;
// 	ap_uint<5> hPhiLSB;
// 	if( pLyrId == 1 && ASType == BARRELPS ) 
// 		hPhiLSB = AllStub<ASType>::kASPhiMSB-(3-1);
// 	else
// 		hPhiLSB = AllStub<ASType>::kASPhiMSB-(2-1);

// 	// check if phi of input stub needs to be corrected 
// 	if( PHI_CORRECTION ) 
// 	{
// 		AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
// 		if( (ASType == BARRELPS || ASType == BARREL2S) )
// 		{
// 			// Corrected phi, i.e. phi at nominal radius (what about disks?)
// 			// for now I'm going to use the one from the VMRouter 
// 			// because .. they should be the same 
// 			auto hPhi = getPhiCorr<ASType>(hStub.getPhi(), hStub.getR(), hStub.getBend(), phicorrtable_L1); 
// 			hStub.setPhi(hPhi);
// 		}
// 		phiBn = hStub.raw().range(hPhiMSB,hPhiLSB) & 0x7;
// 	}
// 	else
// 		phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;
// }


void InputRouterTop( const ap_uint<6> hLinkId 
	, const ap_uint<kLINKMAPwidth> hLinkTable[24] 
	, const int kPhiCorrtable_L1[64]
	, const int kPhiCorrtable_L2[64]
	, const int kPhiCorrtable_L3[64]
	, const int kPhiCorrtable_L4[128]
	, const int kPhiCorrtable_L5[128]
	, const int kPhiCorrtable_L6[128]
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, InputStubMemory<TRACKER> hMemories[20]);


#endif


