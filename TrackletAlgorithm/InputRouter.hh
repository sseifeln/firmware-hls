#ifndef INPUTROUTER_HH
#define INPUTROUTER_HH

#include "Constants.hh"
#include "hls_math.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "InputStubMemory.hh"
#include <bitset> 


void is2S(ap_uint<kLINKMAPwidth> hDTCMapEncoded, ap_uint<1>& hIs2S);
void isFirst(ap_uint<kLINKMAPwidth> hDTCMapEncoded, ap_uint<1>& hIsFirst);

// function to get decoded map 
void ParseMap( const ap_uint<kNBits_DTC> hInputStub,
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	ap_uint<5>& hDecodedMap);

// function to decode tracker region 
// [barrel or endcap .. 0 --> barrel, 1 --> endcap ]
void DecodeTrackerRegion( const ap_uint<kNBits_DTC> hInputStub, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	ap_uint<1>& hTrackerRegion );

// function to decode layer id 
void DecodeLayer( const ap_uint<kNBits_DTC> hInputStub, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	ap_uint<3>& hLayerOrDiskId );

// function to decode layer id + tracker region 
void DecodeMap( const ap_uint<kNBits_DTC> hInputStub, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	ap_uint<3>& hLayerOrDiskId, ap_uint<1>& hTrackerRegion);


// templated fucntion to get coarse phi region 
template<class DataType, int NBits>
void GetCoarsePhiRegion(const ap_uint<kNBits_DTC> hInputStub, 
	ap_uint<NBits> &hPhiRegion )
{
	
	// stub from input stub 
	DataType hStub(hInputStub.range(kBRAMwidth-1,0));
	auto cPhi = hStub.getPhi();
	hPhiRegion = hStub.template getCoarsePhiRegion<NBits>(); 
	// #ifndef __SYNTHESIS__
	// 	std::cout << "\t.. Input stub is " 
	// 			  << std::bitset<kNBits_DTC>(hInputStub) 
	// 			  << " TF stub is "
	// 			  << std::bitset<kBRAMwidth>(hStub.raw())
	// 			  << " hex "
	// 			  << std::hex 
	// 			  << hStub.raw() 
	// 			  << std::dec 
	// 			  << " bend value is "
	// 			  << std::bitset<DataType::kISBendSize>(hStub.getBend())
	// 			  << " phi value is " << std::bitset<DataType::kISPhiSize>(hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiLSB)) 
	// 			  << " -- " << cPhi
	// 			  << " -- " << std::bitset<NBits>(hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiMSB-(NBits-1)) )
	// 			  << " -- " << hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiMSB-(NBits-1)) 
	// 			  // << " ---- " << cPhi/(16383/std::pow(2.,NBits))
	// 			  << " phi region is " << hPhiRegion 
	// 			  << "\n";
	// #endif
}

// templated fucntion to get coarse phi region 
template<class DataType, int NBits>
void GetCoarsePhiRegion(const ap_uint<kBRAMwidth> hInputStub, 
	ap_uint<NBits> &hPhiRegion )
{
	
	// stub from input stub 
	DataType hStub(hInputStub);
	auto cPhi = hStub.getPhi();
	hPhiRegion = hStub.template getCoarsePhiRegion<NBits>(); 
	// #ifndef __SYNTHESIS__
	// 	std::cout << "\t.. Input stub is " 
	// 			  << std::bitset<kNBits_DTC>(hInputStub) 
	// 			  << " TF stub is "
	// 			  << std::bitset<kBRAMwidth>(hStub.raw())
	// 			  << " hex "
	// 			  << std::hex 
	// 			  << hStub.raw() 
	// 			  << std::dec 
	// 			  << " bend value is "
	// 			  << std::bitset<DataType::kISBendSize>(hStub.getBend())
	// 			  << " phi value is " << std::bitset<DataType::kISPhiSize>(hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiLSB)) 
	// 			  << " -- " << cPhi
	// 			  << " -- " << std::bitset<NBits>(hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiMSB-(NBits-1)) )
	// 			  << " -- " << hStub.raw().range(DataType::kISPhiMSB,DataType::kISPhiMSB-(NBits-1)) 
	// 			  // << " ---- " << cPhi/(16383/std::pow(2.,NBits))
	// 			  << " phi region is " << hPhiRegion 
	// 			  << "\n";
	// #endif
}



// templated write memories function
template<int ISType, int NBits, int Nmemories>
void WriteMemories(const BXType bx, const ap_uint<kBRAMwidth> hInputStub, 
	InputStubMemory<ISType> hMemory[Nmemories])
{

	ap_uint<8> hEntries;
	ap_uint<NBits> cPhiRegion;
	GetCoarsePhiRegion<InputStub<ISType>,NBits>(hInputStub, cPhiRegion );
	assert(cPhiRegion >= 0 && cPhiRegion < Nmemories );
	
	//#pragma HLS dependence variable=hMemory intra WAR true
	hEntries = (&hMemory[cPhiRegion])->getEntries(bx);
	InputStub<ISType> hStub(hInputStub);
	(&hMemory[cPhiRegion])->write_mem(bx, hStub, hEntries );
}


// templated write memories function
template<int ISType, int NBits, int Nmemories>
void WriteMemories(const BXType bx, const ap_uint<kBRAMwidth> hInputStub, 
	ap_uint<8> nEntries[Nmemories], 
	InputStubMemory<ISType> hMemory[Nmemories])
{
	ap_uint<8> hEntries;
	ap_uint<NBits> cPhiRegion;
	GetCoarsePhiRegion<InputStub<ISType>,NBits>(hInputStub, cPhiRegion );
	assert(cPhiRegion >= 0 && cPhiRegion < Nmemories );
	
	#pragma HLS dependence variable=hEntries intra WAR true
	hEntries=nEntries[cPhiRegion];
	InputStub<ISType> hStub(hInputStub);
	(&hMemory[cPhiRegion])->write_mem(bx, hStub, hEntries );
	hEntries++;
	*(&nEntries[cPhiRegion])=hEntries;
}

// templated write memories function
template<int ISType>
void WriteMemories(const BXType bx, const ap_uint<kBRAMwidth> hInputStub, 
	ap_uint<8>& nEntries, 
	InputStubMemory<ISType>& hMemory)
{
	ap_uint<8> hEntries;
	// #pragma HLS dependence variable=hEntries intra WAR true
	hEntries=nEntries;
	InputStub<ISType> hStub(hInputStub);
	hMemory.write_mem(bx, hStub, hEntries );
	hEntries++;
	nEntries=hEntries;
}

// templated write memory function
template<int ISType, int NBits>
void WriteMemory(const BXType bx, const ap_uint<kNBits_DTC> hInputStub, 
	InputStubMemory<ISType>* hMemory)
{
	ap_uint<NBits> cPhiRegion;
	GetCoarsePhiRegion<InputStub<ISType>,NBits>(hInputStub, cPhiRegion );
	if( NBits == 2 ) 
		assert(cPhiRegion >= 0 && cPhiRegion < 4 );
	else
		assert(cPhiRegion >= 0 && cPhiRegion < 8 );

	InputStub<ISType> hStub(hInputStub.range(kBRAMwidth-1,0));
	hMemory->write_mem(bx, hStub, hMemory->getEntries(bx) );
}

// templated write memory function
template<int ISType, int NBits>
void WriteStubToMemory(const BXType bx, const ap_uint<kNBits_DTC> hInputStub, 
	const ap_uint<8> hIndex, IRMemory hMemories[8])
{
	ap_uint<NBits> cPhiRegion;
	GetCoarsePhiRegion<InputStub<ISType>,NBits>(hInputStub, cPhiRegion );
	if( NBits == 2 ) 
		assert(cPhiRegion >= 0 && cPhiRegion < 4 );
	else
		assert(cPhiRegion >= 0 && cPhiRegion < 8 );

	InputStub<ISType> hStub(hInputStub.range(kBRAMwidth-1,0));
	auto cEntries = (&hMemories[hIndex+cPhiRegion])->getEntries(bx);
	(&hMemories[hIndex+cPhiRegion])->write_mem(bx, hStub.raw(), cEntries );
}


// not very elegant catch-all cases router 
void RouteStub(const BXType bx, const ap_uint<kNBits_DTC> hInputStub, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	IRMemory hMemoriesPS[kTotalPSmemories], 
	IRMemory hMemories2S[kTotal2Smemories] );

#endif