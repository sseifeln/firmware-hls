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


void InputRouterPS(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrelPS& hBarrelMemories, StubsDiskPS& hDiskMemories);

void InputRouter2S(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
	StubsBarrel2S& hBarrelMemories, StubsDisk2S& hDiskMemories);


void RegionRouter(hls::stream<ap_uint<kNBits_DTC>> & inData
	, const ap_uint<kLINKMAPwidth> hLinkWord
	, hls::stream<ap_uint<8>> &hRoutingInfo  
	, hls::stream<ap_uint<kBRAMwidth>> outData[8]);

void MemoryRouter(const BXType bx, hls::stream<ap_uint<kBRAMwidth>> inData[8]
	, hls::stream<ap_uint<8>> &hRoutingInfo 
	, StubsBarrelPS& hBrl, StubsDiskPS& hDsk ); 


// route stub word from DTC to correct memory 
template<int ISType, int NmemoriesL1, int NmemoriesL2, int NmemoriesL3> 
void BarrelStubMemoryRouter(const BXType bx
	, const ap_uint<kNBits_DTC> inData 
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<ISType> hL1[NmemoriesL1]
	, InputStubMemory<ISType> hL2[NmemoriesL2]
	, InputStubMemory<ISType> hL3[NmemoriesL3]) 
{
	ap_uint<1> hIs2S;
	is2S(hLinkWord, hIs2S);
	ap_uint<3> hLyr;
	ap_uint<1> hIsBrl;
	DecodeMap( inData ,  hLinkWord, hLyr, hIsBrl);
	InputStub<ISType> hStub(inData.range(kBRAMwidth-1,0));
	if( hIsBrl == 1 )
	{
		ap_uint<3> hPhi; 
		if( hIs2S == 1 )
		{
			ap_uint<2> cPhiBn;
			GetCoarsePhiRegion<InputStub<BARREL2S>,2>(inData, cPhiBn);
			hPhi = cPhiBn & 0x7 ; 
		}
		else
		{
			if( hLyr == 1 )
				GetCoarsePhiRegion<InputStub<BARRELPS>,3>(inData, hPhi);
			else
			{
				ap_uint<2> cPhiBn;
				GetCoarsePhiRegion<InputStub<BARRELPS>,2>(inData, cPhiBn);
				hPhi = cPhiBn & 0x7 ; 
			}
		}
		if( hLyr == 1 || hLyr == 3 )
			(&hL1[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 2 || hLyr == 4 ) 
			(&hL2[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 3 || hLyr == 6 ) 
			(&hL3[hPhi])->write_mem(bx, hStub);
	}
}

// route stub word from DTC to correct memory 
template<int ISType, int NmemoriesL1, int NmemoriesL2, int NmemoriesL3, int NmemoriesL4, int NmemoriesL5> 
void EndcapStubMemoryRouter(const BXType bx
	, const ap_uint<kNBits_DTC> inData 
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<ISType> hL1[NmemoriesL1]
	, InputStubMemory<ISType> hL2[NmemoriesL2]
	, InputStubMemory<ISType> hL3[NmemoriesL3]
	, InputStubMemory<ISType> hL4[NmemoriesL4]
	, InputStubMemory<ISType> hL5[NmemoriesL5]) 
{
	ap_uint<3> hPhi; 
	ap_uint<2> cPhiBn;
	ap_uint<3> hLyr;
	ap_uint<1> hIsBrl;
	DecodeMap( inData ,  hLinkWord, hLyr, hIsBrl);
	InputStub<ISType> hStub(inData.range(kBRAMwidth-1,0));

	if( hIsBrl == 0 )
	{
		GetCoarsePhiRegion<InputStub<ISType>,2>(inData, cPhiBn);
		hPhi = cPhiBn & 0x7 ; 
		if( hLyr == 1 )
			(&hL1[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 2 ) 
			(&hL2[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 3 ) 
			(&hL3[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 4 ) 
			(&hL4[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 5 ) 
			(&hL5[hPhi])->write_mem(bx, hStub);
	}
}

// route stub word from DTC to correct memory 
template<int ISTypeBrl, int ISTypeDsk, int NregionsL1, int NregionsTk> 
void StubMemoryRouter(const BXType bx
	, const ap_uint<kNBits_DTC> inData 
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, InputStubMemory<ISTypeBrl> hL1[NregionsL1]
	, InputStubMemory<ISTypeBrl> hL2[NregionsTk]
	, InputStubMemory<ISTypeBrl> hL3[NregionsTk]
	, InputStubMemory<ISTypeDsk> hD1[NregionsTk]
	, InputStubMemory<ISTypeDsk> hD2[NregionsTk]
	, InputStubMemory<ISTypeDsk> hD3[NregionsTk]
	, InputStubMemory<ISTypeDsk> hD4[NregionsTk]
	, InputStubMemory<ISTypeDsk> hD5[NregionsTk]) 
{
	ap_uint<1> hIs2S;
	is2S(hLinkWord, hIs2S);
	ap_uint<3> hPhi; 
	ap_uint<2> cPhiBn;
	ap_uint<3> hLyr;
	ap_uint<1> hIsBrl;
	DecodeMap( inData ,  hLinkWord, hLyr, hIsBrl);
	if( hIsBrl == 0 )
	{
		InputStub<ISTypeDsk> hStub(inData.range(kBRAMwidth-1,0));
		GetCoarsePhiRegion<InputStub<ISTypeDsk>,2>(inData, cPhiBn);
		hPhi = cPhiBn & 0x7 ; 
		if( hLyr == 1 )
			(&hD1[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 2 ) 
			(&hD2[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 3 ) 
			(&hD3[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 4 ) 
			(&hD4[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 5 ) 
			(&hD5[hPhi])->write_mem(bx, hStub);
	}
	else if( hIsBrl == 1 )
	{
		InputStub<ISTypeBrl> hStub(inData.range(kBRAMwidth-1,0));
		ap_uint<3> hPhi; 
		if( hIs2S == 1 )
		{
			ap_uint<2> cPhiBn;
			GetCoarsePhiRegion<InputStub<ISTypeBrl>,2>(inData, cPhiBn);
			hPhi = cPhiBn & 0x7 ; 
		}
		else
		{
			if( hLyr == 1 )
				GetCoarsePhiRegion<InputStub<ISTypeBrl>,3>(inData, hPhi);
			else
			{
				ap_uint<2> cPhiBn;
				GetCoarsePhiRegion<InputStub<ISTypeBrl>,2>(inData, cPhiBn);
				hPhi = cPhiBn & 0x7 ; 
			}
		}
		if( hLyr == 1 || hLyr == 3 )
			(&hL1[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 2 || hLyr == 4 ) 
			(&hL2[hPhi])->write_mem(bx, hStub);
		else if( hLyr == 3 || hLyr == 6 ) 
			(&hL3[hPhi])->write_mem(bx, hStub);
	}
}


void InputRouterTop(const BXType bx, 
	hls::stream<ap_uint<kNBits_DTC>> & hIputLink, 
	const ap_uint<kLINKMAPwidth> hLinkWord,
	StubsBarrelPS& hBrl, StubsDiskPS& hDsk);

// void InputRouterTop(const BXType bx, 
// 	hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
// 	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
// 	StubsBarrelPS& hBrlPS, StubsDiskPS& hDskPS, 
// 	StubsBarrel2S& hBrl2S, StubsDisk2S& hDsk2S);

// void InputRouterGeneric(const BXType bx, const int nStubs, 
// 	hls::stream<ap_uint<kNBits_DTC>> &hIputLink,
// 	const ap_uint<kLINKMAPwidth> hDTCMapEncoded, 
// 	IRMemory hMemoriesPS[kTotalPSmemories], 
// 	IRMemory hMemories2S[kTotal2Smemories]);	

// void InputRouterTest(const BXType bx, hls::stream<ap_uint<kNBits_DTC>> &hIputLink, 
// 	const ap_uint<kLINKMAPwidth> hDTCMapEncoded,
// 	StubsBarrelPS& hBrl, StubsDiskPS& hDsk,
// 	StubsBarrel2S& hBrl2S, StubsDisk2S& hDsk2S);

#endif


