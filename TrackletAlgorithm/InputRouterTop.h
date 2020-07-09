#ifndef TrackletAlgorithm_InputRouterTop_h
#define TrackletAlgorithm_InputRouterTop_h


#include "Constants.h"
// TF constants that we need to know about 
constexpr unsigned kBRAMwidth = 36; 
// # bits encoding stub addres on link 
#include "hls_math.h"
#include "AllStubMemory.h"
#include "VMRouter.h"

// DTC constants that we need to know about 
constexpr int kNBits_DTC = 38; 
// link map
constexpr int kLINKMAPwidth = 18;
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


template<regionType ASType, int NM>
void EnLRouter(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, const ap_uint<2> cLyr
	, ap_uint<8> nEntries[NM]
	, AllStubMemory<ASType> hMemory[NM])
{

	#pragma HLS pipeline II=1 
	#pragma HLS inline 
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	#pragma HLS array_partition variable=nEntries complete
	// hMemory is partition in the memory template 
	// layer id is in the link word
	ap_uint<2> pLayer=inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	if( pLayer == cLyr ) 
	{
		// now stuff I can get from the stub word 
		ap_uint<3> hPhiBn;
		GetPhiBin<ASType>(inStub, lnkWord.range(4*cLyr+3,4*cLyr+1), hPhiBn);
		assert( hPhiBn < NM );
		#ifndef __SYNTHESIS__
			if( IR_DEBUG && cLyr == 0)
			{
				std::cout << "Stub from bx "
				 << bx 
				 << " : "
				 << std::bitset<kNBits_DTC>(inStub.range(kNBits_DTC-1,0))
				 << "\t"
				 << std::bitset<kBRAMwidth>(inStub.range(kBRAMwidth-1,0))
				 << "\t"
				 << std::hex
				 << inStub.range(kBRAMwidth-1,0)
				 << std::dec 
				 << " - layer is "
				 << std::bitset<2>(cLyr)
				 << " -- encoded layer is "
				 << std::bitset<3>(lnkWord.range(4*cLyr+3,4*cLyr+1))
				 << " -- phi bin is "
				 << hPhiBn 
				 << "\n";
			}
		#endif
		AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
		ap_uint<8> hEntries = nEntries[hPhiBn];
		(&hMemory[hPhiBn])->write_mem(bx,hStub,hEntries);
		*(&nEntries[hPhiBn])=hEntries+1;
	}
}

template<regionType ASType1, regionType ASType2, int NM1, int NM2>
void EnLRouter2L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, AllStubMemory<ASType1> L1[NM1] 
	, AllStubMemory<ASType2> L2[NM2])
{
	#pragma HLS inline 
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
}

template<regionType ASType1, regionType ASType2, regionType ASType3, int NM1, int NM2, int NM3>
void EnLRouter3L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, ap_uint<8> n3[NM3]
	, AllStubMemory<ASType1> L1[NM1] 
	, AllStubMemory<ASType2> L2[NM2] 
	, AllStubMemory<ASType2> L3[NM3])
{
	#pragma HLS inline 
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ASType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
}

template<regionType ASType1, regionType ASType2, regionType ASType3, regionType ASType4, int NM1, int NM2, int NM3, int NM4>
void EnLRouter4L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, ap_uint<8> n3[NM3]
	, ap_uint<8> n4[NM4]
	, AllStubMemory<ASType1> L1[NM1] 
	, AllStubMemory<ASType2> L2[NM2] 
	, AllStubMemory<ASType2> L3[NM3] 
	, AllStubMemory<ASType2> L4[NM4])
{
	#pragma HLS inline 
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ASType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
	EnLRouter<ASType4, NM4>(bx, inStub, lnkWord,3, n4, L4);
}

// void InputRouter_PS( const BXType bx
// 	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
// 	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
// 	, const ap_uint<5> hNMemories  
// 	, const ap_uint<3> hNLayers
// 	, const ap_uint<3> *hLayers  
// 	, AllStubMemory<BARRELPS> *hBarrelMemories
// 	, AllStubMemory<DISKPS> *hDiskMemories)

void InputRouter_PS_1Barrel3Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
	, AllStubMemory<BARRELPS> L1[8]
	, AllStubMemory<DISKPS> L2[4]
	, AllStubMemory<DISKPS> L3[4]
	, AllStubMemory<DISKPS> L4[4]);

void InputRouter_2S_1Barrel1Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
	, AllStubMemory<BARREL2S> L1[4]
	, AllStubMemory<DISK2S> L2[4]);


#endif


