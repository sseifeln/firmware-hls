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
// sizes to use in this top level function 
typedef struct
{
	ap_uint<8> n1[kNRegionsLayer1]; 
	ap_uint<8> n2[kNRegions]; 
	ap_uint<8> n3[kNRegions];
} EntriesBarrelPS;
// memories 
typedef struct
{
	InputStubMemory<BARRELPS> m1[kNRegionsLayer1];
	InputStubMemory<BARRELPS> m2[kNRegions];
	InputStubMemory<BARRELPS> m3[kNRegions];
} StubsBarrelPS;
// 2S barrel memories 
// sizes 
typedef struct
{
    ap_uint<8> n1[kNRegions]; 
	ap_uint<8> n2[kNRegions]; 
	ap_uint<8> n3[kNRegions];
} EntriesBarrel2S;
// memories 
typedef struct
{
	InputStubMemory<BARREL2S> m1[kNRegionsLayer1];
	InputStubMemory<BARREL2S> m2[kNRegions];
	InputStubMemory<BARREL2S> m3[kNRegions];
} StubsBarrel2S;

// PS endcap memories 
// sizes
typedef struct
{
    ap_uint<8> n1[kNRegions]; 
	ap_uint<8> n2[kNRegions]; 
	ap_uint<8> n3[kNRegions];
	ap_uint<8> n4[kNRegions];
	ap_uint<8> n5[kNRegions];
} EntriesDiskPS;
// memories 
typedef struct
{
	InputStubMemory<DISKPS> m1[kNRegions];
	InputStubMemory<DISKPS> m2[kNRegions];
	InputStubMemory<DISKPS> m3[kNRegions];
	InputStubMemory<DISKPS> m4[kNRegions];
	InputStubMemory<DISKPS> m5[kNRegions];
} StubsDiskPS;

// 2S endcap memories 
// sizes 
typedef struct
{
    ap_uint<8> n1[kNRegions]; 
	ap_uint<8> n2[kNRegions]; 
	ap_uint<8> n3[kNRegions];
	ap_uint<8> n4[kNRegions];
	ap_uint<8> n5[kNRegions];
} EntriesDisk2S;
// memories 
typedef struct
{
	InputStubMemory<DISK2S> m1[kNRegions];
	InputStubMemory<DISK2S> m2[kNRegions];
	InputStubMemory<DISK2S> m3[kNRegions];
	InputStubMemory<DISK2S> m4[kNRegions];
	InputStubMemory<DISK2S> m5[kNRegions];
} StubsDisk2S;


// memories 
typedef struct
{
	InputStubMemory<BARRELPS> l1[kNRegionsLayer1];
	InputStubMemory<BARRELPS> l2[kNRegions];
	InputStubMemory<BARRELPS> l3[kNRegions];
	InputStubMemory<DISKPS> d1[kNRegions];
	InputStubMemory<DISKPS> d2[kNRegions];
	InputStubMemory<DISKPS> d3[kNRegions];
	InputStubMemory<DISKPS> d4[kNRegions];
	InputStubMemory<DISKPS> d5[kNRegions];
} MemoriesPS;

// memories 
typedef struct
{
	InputStubMemory<BARREL2S> l1[kNRegionsLayer1];
	InputStubMemory<BARREL2S> l2[kNRegions];
	InputStubMemory<BARREL2S> l3[kNRegions];
	InputStubMemory<DISK2S> d1[kNRegions];
	InputStubMemory<DISK2S> d2[kNRegions];
	InputStubMemory<DISK2S> d3[kNRegions];
	InputStubMemory<DISKPS> d4[kNRegions];
	InputStubMemory<DISKPS> d5[kNRegions];
} Memories2S;

// counters 
typedef struct
{
	ap_uint<8> l1[kNRegionsLayer1];
	ap_uint<8> l2[kNRegions];
	ap_uint<8> l3[kNRegions];
	ap_uint<8> d1[kNRegions];
	ap_uint<8> d2[kNRegions];
	ap_uint<8> d3[kNRegions];
	ap_uint<8> d4[kNRegions];
	ap_uint<8> d5[kNRegions];
} CountersPS;
// counters 
typedef struct
{
	ap_uint<8> l1[kNRegions];
	ap_uint<8> l2[kNRegions];
	ap_uint<8> l3[kNRegions];
	ap_uint<8> d1[kNRegions];
	ap_uint<8> d2[kNRegions];
	ap_uint<8> d3[kNRegions];
	ap_uint<8> d4[kNRegions];
	ap_uint<8> d5[kNRegions];
} Counters2S;




typedef struct
{
	ap_uint<kLINKMAPwidth> hLinkWord; 
	ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]; 
}RouterInputPort;

typedef struct
{
	ap_uint<1> region;
	ap_uint<3> lyr;
	ap_uint<3> phi; 
	hls::stream<ap_uint<kBRAMwidth>> outStrm; 
}RouterOutputPort;

typedef struct
{
	ap_uint<1> isValid; 
	ap_uint<1> isBrl;
	ap_uint<1> is2S;
	ap_uint<3> layerId;
	ap_uint<3> phiRegion;
	ap_uint<kBRAMwidth> hStub; 
}LayerRouterOutputPort;

typedef struct
{
	ap_uint<1> isValid; 
	ap_uint<3> phiRegion;
	ap_uint<kBRAMwidth> hStub; 
}PhiRouterOutputPort;

void LayerDecode(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	LayerRouterOutputPort &hOutput);

void DecodeIS(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	LayerRouterOutputPort &hOutput);

void DecodeInputStub(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	LayerRouterOutputPort &hOutput);

void LayerRouter(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	ap_uint<kBRAMwidth>& outStub) ;

template<int ISType> 
void MemoryFiller(const BXType bx
	, ap_uint<kBRAMwidth> hInputStub 
	, InputStubMemory<ISType> &hMemory )
{
	InputStub<ISType> hStub(hInputStub);
	hMemory.write_mem(bx, hStub );
}

void MemoryRouter(const BXType bx 
	, LayerRouterOutputPort hInput
	, ap_uint<8> hEntries 
	, StubsBarrelPS& hBrl
	, StubsDiskPS& hDsk);

void UpdateCounters(LayerRouterOutputPort hInput
	, EntriesBarrelPS& cBrl
	, EntriesDiskPS& cDsk);

void RouterPS(const BXType bx
	, RouterInputPort inPrt
	, StubsBarrelPS& hBrl
	, StubsDiskPS& hDsk);


template<int ISType> 
void GetPhiBin(const ap_uint<kNBits_DTC> inStub
	, ap_uint<3> pLyrId 
	, ap_uint<3> &phiBn )
{
	ap_uint<5> hPhiMSB = InputStub<ISType>::kISPhiMSB;
	ap_uint<5> hPhiLSB;
	if( pLyrId == 1 && ISType == BARRELPS ) 
		hPhiLSB = InputStub<ISType>::kISPhiMSB-(3-1);
	else
		hPhiLSB = InputStub<ISType>::kISPhiMSB-(2-1);
	phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;

}
void RouteInputStub(const BXType bx,
	const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	const ap_uint<2> pLayer, 
	CountersPS& hCntrsPS,
	MemoriesPS &hPS); 

void StubRouter(const BXType bx,
	const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	CountersPS& hCntrsPS,
	MemoriesPS &hPS) ;


void GenericRouterPS(const BXType bx
	, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded
	, StubsBarrelPS& hBrl
	, StubsDiskPS& hDsk);


void GenericRouter2S(const BXType bx, 
	RouterInputPort inPS,
	StubsBarrel2S& hBrl,
	StubsDisk2S& hDsk);


void RoutePS(const BXType bx
	, RouterInputPort inPrt
	, MemoriesPS& hPS);

void GetCntrIndex(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex);

void GetCntrIndex2S(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex);

void GetCntrIndexPS(ap_uint<1> isBrl
	, ap_uint<3> layerId 
	, ap_uint<3> phiRegion
	, ap_uint<8> &cIndex);


void DecoderInputStub(const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord,
	ap_uint<1> &hIsValid,
	ap_uint<1> &hIs2S,
	ap_uint<1> &hIsBrl,
	ap_uint<3> &layerId,
	ap_uint<3> &phiRegion,
	ap_uint<kBRAMwidth> &hStub);

template<int ISType1, int ISType2, int ISType3, int NM1, int NM2, int NM3,int LNK>
void EnLRouter3(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2] 
	, InputStubMemory<ISType3> L3[NM2])
{
	// some constants 
	const ap_uint<3> cNbitsTk=2; 
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	
	// stuff I can get from the lnk word 
	ap_uint<2> pLayer=inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
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
	// to be replaced with valid bit 
	if( 1 ) 
	{
		if( pLayer == 0 )
		{
			assert( hPhiBn < NM1 );
			InputStub<ISType1> hStub(inStub.range(kBRAMwidth-1,0));
			(&L1[hPhiBn])->write_mem(bx,hStub);
		}
		else if ( pLayer == 1 )
		{
			assert( hPhiBn < NM2 );
			InputStub<ISType2> hStub(inStub.range(kBRAMwidth-1,0));
			(&L2[hPhiBn])->write_mem(bx,hStub);
		}
		else
		{
			assert( hPhiBn < NM3 );
			InputStub<ISType3> hStub(inStub.range(kBRAMwidth-1,0));
			(&L3[hPhiBn])->write_mem(bx,hStub);
		}	
	}
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
	// stuff I can get from the lnk word 
	ap_uint<2> pLayer=inStub.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	if( pLayer == cLyr ) 
	{
		#pragma HLS array_partition variable=hMemory complete
		ap_uint<1> hIs2S = lnkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
		// now stuff I can get from the stub word 
		ap_uint<4> hLyrDecoding;
		ap_uint<8> hIndex=0;
		LOOP_LyrIndex :
		for( int iLyr=0; iLyr<4;iLyr++)
		{
			#pragma HLS unroll
		 	if( iLyr == pLayer )
			{
				hLyrDecoding = lnkWord.range(hIndex+3,hIndex);
			}
			else
				hIndex = hIndex + 4; 
		}
		ap_uint<3> hPhiBn;
		GetPhiBin<ISType>(inStub, hLyrDecoding.range(3,1), hPhiBn);
		assert( hPhiBn < NM );
		InputStub<ISType> hStub(inStub.range(kBRAMwidth-1,0));
		ap_uint<8> hEntries = nEntries[hPhiBn];
		(&hMemory[hPhiBn])->write_mem(bx,hStub,hEntries);
		*(&nEntries[hPhiBn])=hEntries+1;
	}
}

template<int ISType1, int ISType2, int NM1, int NM2>
void EnLRouter2(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2])
{
	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
}

template<int ISType1, int ISType2, int ISType3, int NM1, int NM2, int NM3>
void EnLRouter3(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, ap_uint<8> n3[NM3]
	, InputStubMemory<ISType1> L1[NM1] 
	, InputStubMemory<ISType2> L2[NM2] 
	, InputStubMemory<ISType2> L3[NM3])
{
	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,2, n2, L2);
	EnLRouter<ISType3, NM3>(bx, inStub, lnkWord,1, n3, L3);
}

template<int ISType1, int ISType2, int ISType3, int ISType4, int NM1, int NM2, int NM3, int NM4>
void EnLRouter4(const BXType bx
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
	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port=inStub
	#pragma HLS interface ap_none port=lnkWord
	EnLRouter<ISType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ISType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ISType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
	EnLRouter<ISType4, NM4>(bx, inStub, lnkWord,3, n4, L4);
}


void TopLevelIR( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hDTCMapEncoded 
	, InputStubMemory<BARREL2S> L1[4]
	, InputStubMemory<DISK2S> L2[4]);

void SimpleRouterPS(const BXType bx
	, const ap_uint<kNBits_DTC> hInputStub
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, ap_uint<8> lCnt[kNMemories]
	, StubsBarrelPS& hBrlPS);

void SimpleRouter2S(const BXType bx
	, const ap_uint<kNBits_DTC> hLnkStub
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, ap_uint<8> lCnt[kNMemories]
	, StubsBarrel2S& hBrl);


void GenericRouter(const BXType bx, 
	RouterInputPort inPS, RouterInputPort in2S
	, StubsBarrelPS& hBrlPS, StubsDiskPS& hDskPS
	, StubsBarrel2S& hBrl2S, StubsDisk2S& hDsk2S);

void InputRouterPS(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk);

void InputRouter2S(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk);

#endif


