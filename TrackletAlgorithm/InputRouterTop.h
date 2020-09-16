#ifndef TrackletAlgorithm_InputRouterTop_h
#define TrackletAlgorithm_InputRouterTop_h


#include "Constants.h"
#include "DTCStubMemory.h"
#include "AllStubMemory.h"
#include "VMRouter.h"

// link map
constexpr int kLINKMAPwidth = 20;
#ifndef __SYNTHESIS__
	#include <bitset> 
#endif


// #define PRAGMA_SUB(x) _Pragma (#x)
// #define DO_PRAGMA(x) PRAGMA_SUB(x)
// #define STREAM_IN_DEPTH 8
// // Legal pragmas
// DO_PRAGMA(HLS stream depth=STREAM_IN_DEPTH variable=InStream)
// #pragma HLS stream depth=8 variable=OutStream


// maximum number of IR memories 
constexpr unsigned int kNMemories = 20;

#define IR_DEBUG false



// Get the corrected phi, i.e. phi at the average radius of the barrel
template<regionType ASType, unsigned int nCorrBns> 
void GetPhiCorrection(
		const typename AllStub<ASType>::ASPHI phi,
		const typename AllStub<ASType>::ASR r,
		const typename AllStub<ASType>::ASBEND bend, 
		const int phicorrtable[nCorrBns], 
		typename AllStub<ASType>::ASPHI &corrPhi ) {

	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port = phicorrtable
	//#pragma HLS array_partition variable = phicorrtable complete
	
	// local copy of table 
	if (ASType == DISKPS || ASType == DISK2S)
	{
		corrPhi = phi;
	}
	else
	{
		constexpr auto rbins = 1 << nrbitsphicorrtable; // The number of bins for r

		ap_uint<nrbitsphicorrtable> rbin = (r + (1 << (r.length() - 1)))
				>> (r.length() - nrbitsphicorrtable); // Which bin r belongs to. Note r = 0 is mid radius
		auto index = bend * rbins + rbin; // Index for where we find our correction value
		int hPhiCorr = phicorrtable[index];
		// LOOP_GetPhiCorr:
		// for (unsigned int cIndx = 0; cIndx < nCorrBns; cIndx++) 
		// {
		// 	#pragma HLS unroll
		// 	if( cIndx == index ) hPhiCorr = phicorrtable[cIndx];
		// }
		corrPhi = phi - hPhiCorr; // the corrected phi
		
		// Check for overflow
		if (corrPhi < 0)
			corrPhi = 0; // can't be less than 0
		if (corrPhi >= 1 << phi.length())
			corrPhi = (1 << phi.length()) - 1;  // can't be more than the max value
	}
}

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

	ap_uint<8> hPhiMSB = AllStub<ASType>::kASPhiMSB;
	ap_uint<8> hPhiLSB;
	if( pLyrId == 1 && ASType == BARRELPS ) 
		hPhiLSB = AllStub<ASType>::kASPhiMSB-(3-1);
	else
		hPhiLSB = AllStub<ASType>::kASPhiMSB-(2-1);

	#ifndef __SYNTHESIS__
			if( IR_DEBUG )
			{
				std::cout << "\t.. bend is " 
					<< +AllStub<ASType>::kASBendSize
					<< " bits : "
					<< std::bitset<AllStub<ASType>::kASBendSize>(inStub.range(AllStub<ASType>::kASBendMSB,AllStub<ASType>::kASBendLSB))
					<< "\n";
			}
	#endif
	AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
	typename AllStub<ASType>::ASPHI hPhiCorrected; 
	if( pLyrId == 1 || pLyrId == 4 )
	{  
		GetPhiCorrection<ASType,nCorrBns>(hStub.getPhi(), hStub.getR(), hStub.getBend(), kPhiCorrtable_L1, hPhiCorrected); 
	}
	else if( pLyrId == 2 || pLyrId == 5 )
	{  
		GetPhiCorrection<ASType,nCorrBns>(hStub.getPhi(), hStub.getR(), hStub.getBend(), kPhiCorrtable_L2, hPhiCorrected); 
	}
	else if( pLyrId == 3 || pLyrId == 6 )
	{  
		GetPhiCorrection<ASType,nCorrBns>(hStub.getPhi(), hStub.getR(), hStub.getBend(), kPhiCorrtable_L3, hPhiCorrected); 
	}
	hStub.setPhi(hPhiCorrected);
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

void InputRouterTop( const BXType hBx
	, const ap_uint<6> hLinkId 
	, const ap_uint<kLINKMAPwidth> hLinkTable[24] 
	, const int kPhiCorrtable_L1[64]
	, const int kPhiCorrtable_L2[64]
	, const int kPhiCorrtable_L3[64]
	, const int kPhiCorrtable_L4[128]
	, const int kPhiCorrtable_L5[128]
	, const int kPhiCorrtable_L6[128]
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, DTCStubMemory hMemories[20]);

#endif


