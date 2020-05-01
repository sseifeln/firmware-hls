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
void Router8R(const BXType bx
	, LayerRouterOutputPort hInput
	, InputStubMemory<ISType> &hPhi0
	, InputStubMemory<ISType> &hPhi1
	, InputStubMemory<ISType> &hPhi2
	, InputStubMemory<ISType> &hPhi3
	, InputStubMemory<ISType> &hPhi4
	, InputStubMemory<ISType> &hPhi5
	, InputStubMemory<ISType> &hPhi6
	, InputStubMemory<ISType> &hPhi7)
{
	ap_uint<8> hEntries;
	#pragma HLS dependence variable=hEntries intra WAR true
	InputStub<ISType> hStub(hInput.hStub);
	if( hInput.phiRegion == 0 )
	{
		//hEntries = hPhi0.getEntries(bx);
		hPhi0.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 1 )
	{
		//hEntries = hPhi1.getEntries(bx);
		hPhi1.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 2 )
	{
		//hEntries = hPhi2.getEntries(bx);
		hPhi2.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 3 )
	{
		//hEntries = hPhi3.getEntries(bx);
		hPhi3.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 4 )
	{
		//hEntries = hPhi4.getEntries(bx);
		hPhi4.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 5 )
	{
		//hEntries = hPhi5.getEntries(bx);
		hPhi5.write_mem(bx, hStub );
	}
	else if( hInput.phiRegion == 6 )
	{
		hEntries = hPhi6.getEntries(bx);
		//hPhi6.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 7 )
	{
		//hEntries = hPhi7.getEntries(bx);
		hPhi7.write_mem(bx, hStub );
	}
}

template<int ISType>
void Router4R(const BXType bx
	, LayerRouterOutputPort hInput
	, InputStubMemory<ISType> &hPhi0
	, InputStubMemory<ISType> &hPhi1
	, InputStubMemory<ISType> &hPhi2
	, InputStubMemory<ISType> &hPhi3)
{
	ap_uint<8> hEntries;
	#pragma HLS dependence variable=hEntries intra WAR true
	InputStub<ISType> hStub(hInput.hStub);
	if( hInput.phiRegion == 0 )
	{
		hEntries = hPhi0.getEntries(bx);
		hPhi0.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 1 )
	{
		hEntries = hPhi1.getEntries(bx);
		hPhi1.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 2 )
	{
		hEntries = hPhi2.getEntries(bx);
		hPhi2.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 3 )
	{
		hEntries = hPhi3.getEntries(bx);
		hPhi3.write_mem(bx, hStub, hEntries );
	}
}

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


void GenericRouterPS(const BXType bx, 
	RouterInputPort inPS,
	StubsBarrelPS& hBrl,
	StubsDiskPS& hDsk);


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

 
void L1Router( const BXType bx, const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	InputStubMemory<BARRELPS> &h0,
	InputStubMemory<BARRELPS> &h1,
	InputStubMemory<BARRELPS> &h2,
	InputStubMemory<BARRELPS> &h3,
	InputStubMemory<BARRELPS> &h4,
	InputStubMemory<BARRELPS> &h5,
	InputStubMemory<BARRELPS> &h6,
	InputStubMemory<BARRELPS> &h7);

void BrlRouterPS( const BXType bx, const ap_uint<kNBits_DTC> inStub, 
	const ap_uint<kLINKMAPwidth> lnkWord, 
	const ap_uint<3> pLayer,
	InputStubMemory<BARRELPS> &h0,
	InputStubMemory<BARRELPS> &h1,
	InputStubMemory<BARRELPS> &h2,
	InputStubMemory<BARRELPS> &h3);

void GenericRouter(const BXType bx, 
	RouterInputPort inPS,
	RouterInputPort in2S,
	InputStubMemory<BARRELPS> &L1PhiA,
	InputStubMemory<BARREL2S> &L4PhiA);

void InputRouterPS(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk);

void InputRouter2S(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk);

#endif


