#ifndef INPUTROUTERTOP_HH
#define INPUTROUTERTOP_HH


#include "InputRouter.hh"
#include "Constants.hh"
#include "hls_math.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "InputStubMemory.hh"

constexpr unsigned int kNPSMemories=36;
constexpr unsigned int kN2SMemories=32;
constexpr unsigned int kNMemories=48;

#define STUB_READ_FCTR 2

// structs to keep track of passing things around 
// not templated because I want them here for now .. 
// PS barrel memories 
// memories 
typedef struct
{
	InputStubMemory<BARRELPS> m1[kNRegionsLayer1];
	InputStubMemory<BARRELPS> m2[kNRegions];
	InputStubMemory<BARRELPS> m3[kNRegions];
} StubsBarrelPS;
typedef struct
{
	InputStubMemory<DISKPS> m1[kNRegions];
	InputStubMemory<DISKPS> m2[kNRegions];
	InputStubMemory<DISKPS> m3[kNRegions];
	InputStubMemory<DISKPS> m4[kNRegions];
	InputStubMemory<DISKPS> m5[kNRegions];
} StubsDiskPS;

// 2S endcap memories 
typedef struct
{
	InputStubMemory<BARREL2S> m1[kNRegionsLayer1];
	InputStubMemory<BARREL2S> m2[kNRegions];
	InputStubMemory<BARREL2S> m3[kNRegions];
} StubsBarrel2S;
typedef struct
{
	InputStubMemory<DISK2S> m1[kNRegions];
	InputStubMemory<DISK2S> m2[kNRegions];
	InputStubMemory<DISK2S> m3[kNRegions];
	InputStubMemory<DISK2S> m4[kNRegions];
	InputStubMemory<DISK2S> m5[kNRegions];
} StubsDisk2S;


typedef struct
{
	ap_uint<kLINKMAPwidth> hLinkWord; 
	ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]; 
}RouterInputPort;


template<int ISType> 
void GetPhiBin(const ap_uint<kNBits_DTC> inStub
	, ap_uint<3> pLyrId 
	, ap_uint<3> &phiBn )
{
	#pragma HLS pipeline II=1 
	ap_uint<5> hPhiMSB = InputStub<ISType>::kISPhiMSB;
	ap_uint<5> hPhiLSB;
	if( pLyrId == 1 && ISType == BARRELPS ) 
		hPhiLSB = InputStub<ISType>::kISPhiMSB-(3-1);
	else
		hPhiLSB = InputStub<ISType>::kISPhiMSB-(2-1);
	phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;

}


template<int ISType, int NM>
void EnLRouter(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, const ap_uint<2> cLyr
	, ap_uint<8> nEntries[NM]
	, InputStubMemory<ISType> hMemory[NM])
{

	#pragma HLS pipeline II=1 
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
		GetPhiBin<ISType>(inStub, lnkWord.range(4*cLyr+3,4*cLyr+1), hPhiBn);
		assert( hPhiBn < NM );
		InputStub<ISType> hStub(inStub.range(kBRAMwidth-1,0));
		ap_uint<8> hEntries = nEntries[hPhiBn];
		(&hMemory[hPhiBn])->write_mem(bx,hStub,hEntries);
		*(&nEntries[hPhiBn])=hEntries+1;
	}
}

template<int ISType1, int ISType2, int NM1, int NM2>
void EnLRouter2L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2])
{
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
}

template<int ISType1, int ISType2, int ISType3, int NM1, int NM2, int NM3>
void EnLRouter3L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, ap_uint<8> n3[NM3]
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2] 
	, InputStubMemory<ISType2> L3[NM3])
{
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ISType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
}

template<int ISType1, int ISType2, int ISType3, int ISType4, int NM1, int NM2, int NM3, int NM4>
void EnLRouter4L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, ap_uint<8> n3[NM3]
	, ap_uint<8> n4[NM4]
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2] 
	, InputStubMemory<ISType2> L3[NM3] 
	, InputStubMemory<ISType2> L4[NM4])
{
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ISType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
	EnLRouter<ISType4, NM4>(bx, inStub, lnkWord,3, n4, L4);
}

void InputRouter_PS_1Barrel3Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
	, InputStubMemory<BARRELPS> L1[8]
	, InputStubMemory<DISKPS> L2[4]
	, InputStubMemory<DISKPS> L3[4]
	, InputStubMemory<DISKPS> L4[4]);

void InputRouter_2S_1Barrel1Disk( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
	, InputStubMemory<BARREL2S> L1[4]
	, InputStubMemory<DISK2S> L2[4]);



// void InputRouterPS(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
// 	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
// 	StubsBarrelPS& hBrl, StubsDiskPS& hDsk);

// void InputRouter2S(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
// 	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
// 	StubsBarrel2S& hBrl, StubsDisk2S& hDsk);

#endif


