#ifndef INPUTROUTERTOP_HH
#define INPUTROUTERTOP_HH




#include "InputRouter.hh"
#include "Constants.hh"
#include "hls_math.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "InputStubMemory.hh"

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
	else if( hInput.phiRegion == 4 )
	{
		hEntries = hPhi4.getEntries(bx);
		hPhi4.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 5 )
	{
		hEntries = hPhi5.getEntries(bx);
		hPhi5.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 6 )
	{
		hEntries = hPhi6.getEntries(bx);
		hPhi6.write_mem(bx, hStub, hEntries );
	}
	else if( hInput.phiRegion == 7 )
	{
		hEntries = hPhi7.getEntries(bx);
		hPhi7.write_mem(bx, hStub, hEntries );
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


void GenericRouter(RouterInputPort inPS,
	RouterInputPort in2S, 
	ap_uint<8>& nRoutedPS,
	ap_uint<8>& nRouted2S);

void InputRouterPS(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk);

void InputRouter2S(const BXType bx, ap_uint<kNBits_DTC> hIputLink[kMaxStubsFromLink], 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBrl, StubsDisk2S& hDsk);

#endif


