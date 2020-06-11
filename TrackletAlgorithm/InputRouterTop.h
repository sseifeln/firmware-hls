#ifndef INPUTROUTERTOP_HH
#define INPUTROUTERTOP_HH



#include "Constants.h"
// TF constants that we need to know about 
constexpr unsigned kBRAMwidth = 36; 
// # bits encoding stub addres on link 
#include "hls_math.h"
#include "AllStubMemory.h"

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


#define STUB_READ_FCTR 2
#define PHI_CORRECTION false
// LUT with phi corrections to the nominal radius. Only used by layers.
// Values are determined by the radius and the bend of the stub.
static const int phicorrtable_L1[] =
#include "../emData/VMR/tables/VMPhiCorrL1.txt"
;
static const int phicorrtable_L2[] =
#include "../emData/VMR/tables/VMPhiCorrL2.txt"
;
static const int phicorrtable_L3[] =
#include "../emData/VMR/tables/VMPhiCorrL3.txt"
;
static const int phicorrtable_L4[] =
#include "../emData/VMR/tables/VMPhiCorrL4.txt"
;
static const int phicorrtable_L5[] =
#include "../emData/VMR/tables/VMPhiCorrL5.txt"
;
static const int phicorrtable_L6[] =
#include "../emData/VMR/tables/VMPhiCorrL6.txt"
;

// structs to keep track of passing things around 
// not templated because I want them here for now .. 
// PS barrel memories 
// memories 
typedef struct
{
	AllStubMemory<BARRELPS> m1[kNRegionsLayer1];
	AllStubMemory<BARRELPS> m2[kNRegions];
	AllStubMemory<BARRELPS> m3[kNRegions];
} StubsBarrelPS;
typedef struct
{
	AllStubMemory<DISKPS> m1[kNRegions];
	AllStubMemory<DISKPS> m2[kNRegions];
	AllStubMemory<DISKPS> m3[kNRegions];
	AllStubMemory<DISKPS> m4[kNRegions];
	AllStubMemory<DISKPS> m5[kNRegions];
} StubsDiskPS;

// 2S endcap memories 
typedef struct
{
	AllStubMemory<BARREL2S> m1[kNRegionsLayer1];
	AllStubMemory<BARREL2S> m2[kNRegions];
	AllStubMemory<BARREL2S> m3[kNRegions];
} StubsBarrel2S;
typedef struct
{
	AllStubMemory<DISK2S> m1[kNRegions];
	AllStubMemory<DISK2S> m2[kNRegions];
	AllStubMemory<DISK2S> m3[kNRegions];
	AllStubMemory<DISK2S> m4[kNRegions];
	AllStubMemory<DISK2S> m5[kNRegions];
} StubsDisk2S;


typedef struct
{
	ap_uint<kLINKMAPwidth> hLinkWord; 
	ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]; 
}RouterInputPort;


template<int ASType> 
void GetPhiBin(const ap_uint<kNBits_DTC> inStub
	, ap_uint<3> pLyrId 
	, ap_uint<3> &phiBn )
{
	#pragma HLS pipeline II=1 
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
		if( ASType == BARRELPS || ASType == BARREL2S )
		{
			auto hPhi = hStub.getPhi();
			constexpr auto nRBns = 1 << kNRbinsPhiCorr;
			auto stubRcoord = inStub.range(AllStub<ASType>::kASRMSB,AllStub<ASType>::kASRLSB);
			auto stubBend = inStub.range(AllStub<ASType>::kASBendMSB,AllStub<ASType>::kASBendLSB); 
			auto hMSB = AllStub<ASType>::kASRMSB;
			auto hLSB = AllStub<ASType>::kASRMSB-(kNRbinsPhiCorr-1);
			ap_uint<kNRbinsPhiCorr> rBn = (hStub.getR() + (1 << (hStub.getR().length() - 1))) >> (hStub.getR().length() - kNRbinsPhiCorr);
			auto hIndex = stubBend * nRBns + rBn;
			if( pLyrId == 1 )
			{
				hPhi = hPhi - phicorrtable_L1[hIndex];
			}
			else if( pLyrId == 2 )
			{
				hPhi = hPhi - phicorrtable_L2[hIndex];
			}
			else if( pLyrId == 3 )
			{
				hPhi = hPhi - phicorrtable_L3[hIndex];
			}
			else if( pLyrId == 4 )
			{
				hPhi = hPhi - phicorrtable_L4[hIndex];
			}
			else if( pLyrId == 5 )
			{
				hPhi = hPhi - phicorrtable_L5[hIndex];
			}
			else if( pLyrId == 6 )
			{
				hPhi = hPhi - phicorrtable_L6[hIndex];
			}
			hStub.setPhi(hPhi);
		}
		phiBn = hStub.raw().range(hPhiMSB,hPhiLSB) & 0x7;
	}
	else
		phiBn = inStub.range(hPhiMSB,hPhiLSB) & 0x7;
}


template<int ASType, int NM>
void EnLRouter(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, const ap_uint<2> cLyr
	, ap_uint<8> nEntries[NM]
	, AllStubMemory<ASType> hMemory[NM])
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
		GetPhiBin<ASType>(inStub, lnkWord.range(4*cLyr+3,4*cLyr+1), hPhiBn);
		assert( hPhiBn < NM );
		// #ifndef __SYNTHESIS__
		// 	if( pLayer == 1 )
		// 	{
		// 		std::cout << "Stub from bx "
		// 		 << bx 
		// 		 << " : "
		// 		 << std::bitset<kNBits_DTC>(inStub.range(kNBits_DTC-1,0))
		// 		 << "\t"
		// 		 << std::bitset<kBRAMwidth>(inStub.range(kBRAMwidth-1,0))
		// 		 << "\t"
		// 		 << std::hex
		// 		 << inStub.range(kBRAMwidth-1,0)
		// 		 << std::dec 
		// 		 << " - layer is "
		// 		 << std::bitset<2>(cLyr)
		// 		 << " -- encoded layer is "
		// 		 << std::bitset<3>(lnkWord.range(4*cLyr+3,4*cLyr+1))
		// 		 << " -- phi bin is "
		// 		 << hPhiBn 
		// 		 << "\n";
		// 	}
		// #endif
		AllStub<ASType> hStub(inStub.range(kBRAMwidth-1,0));
		ap_uint<8> hEntries = nEntries[hPhiBn];
		(&hMemory[hPhiBn])->write_mem(bx,hStub,hEntries);
		*(&nEntries[hPhiBn])=hEntries+1;
	}
}

template<int ASType1, int ASType2, int NM1, int NM2>
void EnLRouter2L(const BXType bx
	, ap_uint<kNBits_DTC> inStub
	, const ap_uint<kLINKMAPwidth> lnkWord
	, ap_uint<8> n1[NM1] 
	, ap_uint<8> n2[NM2]
	, AllStubMemory<ASType1> L1[NM1] 
	, AllStubMemory<ASType2> L2[NM2])
{
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
}

template<int ASType1, int ASType2, int ASType3, int NM1, int NM2, int NM3>
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
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ASType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
}

template<int ASType1, int ASType2, int ASType3, int ASType4, int NM1, int NM2, int NM3, int NM4>
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
	EnLRouter<ASType1, NM1>(bx, inStub, lnkWord,0, n1, L1);
	EnLRouter<ASType2, NM2>(bx, inStub, lnkWord,1, n2, L2);
	EnLRouter<ASType3, NM3>(bx, inStub, lnkWord,2, n3, L3);
	EnLRouter<ASType4, NM4>(bx, inStub, lnkWord,3, n4, L4);
}

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

template<int Width>
void Fill(ap_uint<Width> data_in
	, ap_uint<8> &data_addr
	, ap_uint<Width> data_out[kMaxSizeArray] )
{
	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port=data_in
	
	*(&data_out[data_addr]) = data_in;
	data_addr++;
}	

template<int WordWidthIn, int WordWidthOut, int Depth, int read_increment >
void FilterInput(  ap_uint<WordWidthIn> data_in[Depth]
	, ap_uint<2> data_filter 
	, ap_uint<WordWidthOut> data_out[Depth]) 
{
	#pragma HLS array_partition variable=data_out cyclic factor=2
	int cLpIndex=0;
	int cLyrStubCntr=0;
	int cProcessed=0;
	
	static ap_uint<WordWidthOut> hTmp[read_increment]={0};
	#pragma HLS array_partition variable=hTmp complete
	ap_uint<8> nEntries=0; 
	ap_uint<8> nFound=0;
	ap_uint<1> hFill=0;
	ap_uint<WordWidthIn> cWord =0x00;
	LOOP_ReadLoop:
	for (int cIncrement=0; cIncrement< kMaxStubsFromLink ; cIncrement+=read_increment)
	{
		// check if this is the last increment 
		// if the next elemet is a 0 .. then just fill what we have 
		if( data_in[cIncrement] == 0 && nEntries < read_increment )
		{
			LOOP_FillEnd0:
			for( int cSmallLp=0; cSmallLp < 4 ; cSmallLp++)
			{
				#pragma HLS unroll 
				if( cSmallLp >= nEntries ) 
					continue;
				int cLclIndex = nFound*4 + cSmallLp; 
				#ifndef __SYNTHESIS__
					std::cout << "\t\t... index of output array is " << cLclIndex 
						<< " -- fill stub with " << std::bitset<WordWidthOut>(hTmp[cSmallLp]) 
						<< "\n";
				#endif	
				data_out[cLclIndex]=0x00;
			}
			nEntries = read_increment; 
			continue;
		}
		// if stub is 0x00 .. continue 
		else if( data_in[cIncrement] == 0 || cIncrement >= Depth )
			continue;

		LOOP_DecodeLoop: 
		for( int cIndex=cIncrement ; cIndex < cIncrement+read_increment ; cIndex++)
		{
			// one read per clock cycle 
			// store value in local variable 
			// now use local variable for everything else 
			// just to be sure 
			cWord = data_in[cIndex] ;
			if( cWord == 0 ) 
				continue;
			
			ap_uint<2> cLyrId= cWord.range(WordWidthIn-1,WordWidthIn-2)&0x3;
			if( cLyrId == data_filter )
			{
				#ifndef __SYNTHESIS__
					std::cout << "Stub : " << cIndex
						<< " -- encoded layer id is " << cLyrId 
					    << " -- stub word is " << std::bitset<WordWidthOut>(cWord )
						<< "\n";
				#endif	
				hFill = (nEntries == (read_increment-1)) ? 1 : 0 ; 
				if( nEntries < read_increment )
				{
					hTmp[nEntries]=cWord.range(WordWidthOut-1,0);
					nEntries++;
				}

				if( hFill == 1 )
				{
					nEntries=0;
					LOOP_Fill:
					for( int cSmallLp=0; cSmallLp < 4 ; cSmallLp++)
					{
						#pragma HLS unroll 
						int cLclIndex = nFound*read_increment + cSmallLp; 
						#ifndef __SYNTHESIS__
							std::cout << "\t\t... index of output array is " << cLclIndex 
								<< " -- fill stub with " << std::bitset<WordWidthOut>(hTmp[cSmallLp]) 
								<< "\n";
						#endif	
						data_out[cLclIndex]=0x00;
					}
					nFound++;
				}
			}
			cProcessed++;
		}

		// if the last element ... then just fill what we have 
		if( (cIncrement+read_increment) >= Depth && nEntries < read_increment )
		{
			LOOP_FillLast:
			for( int cSmallLp=0; cSmallLp < 4 ; cSmallLp++)
			{
				#pragma HLS unroll 
				if( cSmallLp >= nEntries ) 
					continue;

				int cLclIndex = nFound*4 + cSmallLp; 
				#ifndef __SYNTHESIS__
					std::cout << "\t\t... index of output array is " << cLclIndex 
						<< " -- fill stub with " << std::bitset<WordWidthOut>(hTmp[cSmallLp]) 
						<< "\n";
				#endif	
				data_out[cLclIndex]=0x00;
			}
			nEntries = read_increment; 
			continue;
		}
		cLpIndex++;
	}
}


// read 
template< int WordWidthIn, int WordWidthOut, int Nbits > 
void Read( ap_uint<WordWidthIn> *data_in
	, ap_uint<Nbits> data_address
	, ap_uint<WordWidthOut> &data_out ) 
{
	#pragma HLS pipeline II=1 
	#pragma HLS interface ap_none port=data_address
	if( data_address < ( 1 << Nbits ) ) 
	{
		data_out = data_in[data_address];
		// #ifndef __SYNTHESIS__
		// 	std::cout << "\t.. Found data word  "
		// 		<< std::bitset<WordWidthOut>(data_out)
		// 		<< " at data address "
		// 		<< data_address 
		// 		<< "\n";
		// #endif
	}
	else
		data_out = 0x00;
}
// compare 
template< int WordWidthIn, int MSB, int nBits > 
void Compare( ap_uint<WordWidthIn> data_in
	, ap_uint<nBits> data_filter  
	, ap_uint<WordWidthIn> & data_out ) 
{
	#pragma HLS pipeline II=1 
	ap_uint<nBits> cFilter = data_out.range(MSB, MSB-(nBits-1));
	if( cFilter == data_filter && data_in != 0x00)	
	{
		data_out = data_in;
		// #ifndef __SYNTHESIS__
		// 	std::cout << "\t.. Found data word  "
		// 		<< std::bitset<WordWidthIn>(data_out)
		// 		<< "\n";
		// #endif
	}
	else
		data_out = 0x00;
}

// process single
template< int WordWidthIn, int Nbits , int MSB, int nBitsFilterWord ,  int WordWidthOut >
void Process( ap_uint<WordWidthIn> *data_in , 
	ap_uint <Nbits> read_address, 
	const ap_uint<nBitsFilterWord> data_filter ,
	ap_uint<WordWidthOut> &data_out ) 
{
	#pragma HLS pipeline II=1 
	#pragma HLS inline
	ap_uint<WordWidthIn> cReadBack=0;
	ap_uint<WordWidthIn> cFiltered=0;
	Read< WordWidthIn, WordWidthIn, Nbits > ( data_in , read_address , cReadBack );
	Compare<WordWidthIn, MSB, nBitsFilterWord>( cReadBack,  data_filter, cFiltered ); 
	if( cFiltered != 0x00 )
	{
		data_out = cFiltered.range(WordWidthOut-1,0);
	}
	else
		data_out = 0x00;
}

template< int WordWidthIn, int Nbits , int MSB, int nBitsFilterWord ,  int WordWidthOut , int NbitsW>
void ProcessArray( ap_uint<WordWidthIn> *data_in , 
	ap_uint <Nbits> read_address, 
	const ap_uint<nBitsFilterWord> data_filter ,
	ap_uint <NbitsW>  &write_address , 
	ap_uint<WordWidthOut> *data_out ) 
{
	#pragma HLS pipeline II=1 
	ap_uint<Nbits> cEntries=write_address;
	ap_uint<WordWidthIn> cReadBack=0;
	ap_uint<WordWidthIn> cFiltered=0;
	Read< WordWidthIn, WordWidthIn, Nbits > ( data_in , read_address , cReadBack );
	Compare<WordWidthIn, MSB, nBitsFilterWord>( cReadBack,  data_filter, cFiltered ); 
	if( cFiltered != 0x00 )
	{
		if( write_address < ( 1 << NbitsW ) ) 
		{
			*(&data_out[cEntries])=cFiltered.range(WordWidthOut-1,0);
			#ifndef __SYNTHESIS__
				std::cout << "\t.. Writing data word  "
					<< std::bitset<WordWidthOut>(cFiltered.range(WordWidthOut-1,0))
					<< " to address "
					<< cEntries 
					<< "\n";
			#endif
			write_address = cEntries+1 ;	
		}
		else
		{
			write_address = cEntries;
		}

	}
	else
		data_out = 0x00;
}

// write 
template< int WordWidthIn, int WordWidthOut, int Nbits > 
void Write( ap_uint<WordWidthIn> data_in
	, ap_uint<Nbits> &write_address 
	, ap_uint<Nbits> &write_last
	, ap_uint<WordWidthOut> *data_out ) 
{
	#pragma HLS pipeline II=1 
	ap_uint<WordWidthIn> data_to_write = data_in;
	if( write_address < ( 1 << Nbits ) && data_to_write != 0x00 ) 
	{
		ap_uint<Nbits> cEntries=write_address;
		write_last = cEntries;
		*(&data_out[cEntries])=data_to_write.range(WordWidthOut-1,0);
		#ifndef __SYNTHESIS__
			std::cout << "\t.. Writing data word  "
				<< std::bitset<WordWidthOut>(data_to_write)
				<< " to address "
				<< cEntries 
				<< " last written is "
				<< write_last 
				<< "\n";
		#endif
		write_address = cEntries+1 ;	
	}
	else
		write_last = 0;
}


// TO-DO
// the output array can be used to fill 
// as many memories for the VMR as needed 
template<int NLayers>
void GenericRouter( ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, ap_uint<kBRAMwidth> hOutput[kMaxSizeArray] )
{
	#pragma HLS clock domain=slow_clock 
	// eventually replace this with a LUT in the IR 
	#pragma HLS interface ap_none port=hLinkWord
	// input BRAM - stubs from link x 
	#pragma HLS RESOURCE variable=hStubs core=RAM_2P
	// paritition to be able to access 
	// multiple stubs per clock cycle 
	// cyclic allows you to acess factor*2 
	// consecutive elements at a time 
	#pragma HLS array_partition variable=hStubs cyclic factor=2 // 32  
	constexpr int cStubIncrement=4; // factor*2 
	
	// L1 fifo
	//#pragma HLS INTERFACE ap_fifo port=hOutput
	#pragma HLS RESOURCE variable=hOutput core=RAM_T2P_BRAM
	#pragma HLS array_partition variable=hOutput cyclic factor=32 // 4*factor  
	//#pragma HLS array_partition variable=hOutput cyclic factor=4 // 2*NLayers  
	
	//static ap_uint<kBRAMwidth> Interm_RAM [kMaxStubsFromLink] = { 0 };
	//#pragma HLS RESOURCE variable=Interm_RAM latency=1 core=RAM_S2P_BRAM
	
	// output BRAM - all stubs from link x arranged by layer 
	//factor x number of layers  
	//#pragma HLS array_partition variable=hOutput cyclic factor=16
	// LOOP_LyrLoop:
	// for( int cLyr = 0 ; cLyr  < 1 ; cLyr ++ )
	// {
	// 	// want to make sure that I can process the next stub in one clock cycle 
	// 	// even if this hasn't finished 
	int cLpIndex=0;
	int cLyrStubCntr=0;
	int cProcessed=0;
	//ap_uint<8> cWriteAddresses=0;
	ap_uint<8> cWriteAddresses[NLayers] = {0};
	//static ap_uint<kBRAMwidth> hTmp[8]={0};
	LOOP_ReadLoop:
	for (int cIncrement=0; cIncrement< kMaxStubsFromLink ; cIncrement+=cStubIncrement)
	{
		#pragma HLS pipeline II=1 
		#ifndef __SYNTHESIS__
			std::cout << "Loop increment  "
				<< cLpIndex
				<< "\n";
		#endif
		// check if this is the last increment 
		// if the next elemet is a 0 .. then just fill what we have 
		LOOP_DecodeLoop: 
		for( int cIndex=0 ; cIndex < cStubIncrement ; cIndex++)
		{
			// one read per loop iteration
			ap_uint<8> cReadAddress = cIncrement + cIndex;
			ap_uint<kNBits_DTC> cReadBack=hStubs[cReadAddress];
			ap_uint<2> cLyrId= cReadBack.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
			LOOP_LyrLoop: 
			for( int cLyr = 0 ; cLyr  <  1 ; cLyr ++ )
			{
				int cStartIndex = cLyr*kMaxStubsFromLink;
				if( cLyrId == cLyr && cReadBack != 0 )
				{
					int nEntries = cWriteAddresses[cLyr];
					if( nEntries < 3 )
					{
						#ifndef __SYNTHESIS__
							std::cout << "\t.. Writing data word  "
								<< std::bitset<kBRAMwidth>(cReadBack)
								<< " to address "
								<< nEntries 
								<< " -- read address is "
								<< cReadAddress
								<< "\n";
						#endif
						*(&hOutput[nEntries])=cReadBack; 
						cWriteAddresses[cLyr] = nEntries + 1 ;
					}
				}
			}
			//*(&hOutput[cReadAddress])=cReadBack; 
			cProcessed++;
		}
		cLpIndex++;
	}
		// //fill local array 
		// #pragma HLS pipeline II=1 
		// FilterInput<kNBits_DTC, kBRAMwidth, kMaxStubsFromLink, cStubIncrement>(  hStubs, cLyr, cLclArray);
		// LOOP_FillLoopOuter:
		// for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter+=cStubIncrement)
		// {
		// 	if( cLclArray[cStubCounter] == 0 )
		// 		continue;

		// 	LOOP_FillLoopInner: 
		// 	for( int cEl=0; cEl < cStubIncrement ; cEl++)
		// 	{
		// 		int cIndex = cStubCounter+cEl;
		// 		// one read per clock cycle 
		// 		// store value in local variable 
		// 		// now use local variable for everything else 
		// 		// just to be sure 
		// 		ap_uint<kNBits_DTC> cDTCword = cLclArray[cIndex] ;
		// 		if( cDTCword == 0 ) 
		// 			continue;
				
		// 		*(&hOutput[cIndex])= cDTCword;
		// 	}
		// }
	//}

	//int nStubs=0;
	//int cLpIndex=0;
	// ap_uint<8> nStubsL[NLayers][cStubIncrement];
	// // into NLayer arrays in first dimension 
	// #pragma HLS array_partition variable=nStubsL complete
	// ap_uint<8> nStbsAcc[NLayers];
	// #pragma HLS array_partition variable=nStbsAcc complete
	// LOOP_SortLoop:
	// for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink ; cStubCounter+=cStubIncrement)
	// {
	// 	#pragma HLS pipeline II=1
	// 	if( hStubs[cStubCounter] == 0 ) 
	// 		continue;
			
	// 	// LOOP_UpdateAccCntr:
	// 	for( int cLyr=0; cLyr< NLayers ; cLyr++)
	// 	{
	// 		if( cStubCounter > 0 )
	// 		{
	// 			nStbsAcc[cLyr] +=  nStubsL[cLyr][0];
	// 		}
	// 		else
	// 			nStbsAcc[cLyr] = 0;
	// 	}
		
	// 	LOOP_DecodeLoop: 
	// 	for( int cEl=0; cEl < cStubIncrement ; cEl++)
	// 	{
	// 		int cIndex = cStubCounter+cEl;
	// 		// one read per clock cycle 
	// 		// store value in local variable 
	// 		// now use local variable for everything else 
	// 		// just to be sure 
	// 		ap_uint<kNBits_DTC> cDTCword = hStubs[cIndex];
	// 		if( cDTCword == 0 ) 
	// 			continue;
			
	// 		// ap_uint<kBRAMwidth> cStub = cDTCword.range(kBRAMwidth-1,0);
	// 		// ap_uint<2> cLyrId= cDTCword.range(kNBits_DTC-1,kNBits_DTC-2)&0x3;
	// 		// ap_uint<1> cIs2S  =  hLinkWord.range(kLINKMAPwidth-2,kLINKMAPwidth-2);
	// 		// ap_uint<kLINKMAPwidth> cWrd = hLinkWord.range(4*cLyrId+3,4*cLyrId);
	// 		// ap_uint<1> cIsBrl =  cWrd.range(0,0) ;
	// 		// ap_uint<3> cTkLyr = cWrd.range(3,1) ;
	// 		// int cMaxPhiBins= ( cTkLyr == 1 && cIsBrl == 1 ) ? 8 : 4 ; 
			
	// 		// LOOP_UpdateLclCntr:
	// 		// for( int cLyr=0; cLyr< NLayers ; cLyr++)
	// 		// {
	// 		// 	int cStartIndx = cLyrId*kMaxStubsFromLink;
	// 		// 	ap_uint<8> cIncrement=( cLyr == cLyrId ) ? 1 : 0;
	// 		// 	//if( cStubCounter == 0 )
	// 		// 	//	nStbsAcc[cLyr] = 0;
	// 		// 	// if element == 0 then stuff neeeds to be reset 
	// 		// 	if( cEl == 0 )
	// 		// 	{
	// 		// 		// only reset accumulated counter for the first element
	// 		// 		// if( cStubCounter == 0 )
	// 		// 		// 	nStbsAcc[cLyr]=0;
	// 		// 		nStubsL[cLyr][cEl]=0;
	// 		// 		nStubsL[cLyr][cEl+1] = 0 + cIncrement;
	// 		// 	}
	// 		// 	else if ( cEl < cStubIncrement-1 )
	// 		// 	{
	// 		// 		nStubsL[cLyr][cEl+1] = nStubsL[cLyr][cEl] + cIncrement;
	// 		// 	}
	// 		// 	else
	// 		// 	{
	// 		// 		nStubsL[cLyr][0] = nStubsL[cLyr][cEl] + cIncrement;
	// 		// 	}
	// 		// 	if( cLyrId == cLyr && cLpIndex == 0  )
	// 		// 	{
	// 		// 		int cIndxArray = nStubsL[cLyr][cEl];
	// 		// 		#ifndef __SYNTHESIS__
	// 		// 			std::cout << "Stub : " << cIndex
	// 		// 				<< " L" << cLpIndex 
	// 		// 			    << " number of stubs processed is " << nStubs
	// 		// 			    << " - encoded layer id is " << cLyrId 
	// 		// 			    << " - tk layer is " << cTkLyr 
	// 		// 			    << " - acc stub counter for this layer : " << nStbsAcc[cLyrId]
	// 		// 			    << " -- index of input array is " << cIndxArray 
	// 		// 				<< "\n";
	// 		// 		#endif	
	// 		// 		*(&hOutput[0])=cDTCword;		
	// 		// 	}
	// 		// }
	// 		// int nStubsLcl = nStbsAcc[cLyrId] + nStubsL[cLyrId][cEl];
	// 		// int cStartIndx = cLyrId*kMaxStubsFromLink + nStubsLcl;
	// 		// //int cIndx = 4*(cLyrId + 4*(nStubsLcl/4)) + (nStubsLcl%4);
	// 		nStubs++;
	// 	}
	// 	//*(&hOutput[cLpIndex])=0;
	// 	cLpIndex++;
	// }
}


template <unsigned int WordWidthIn, unsigned int NbitsAddress, unsigned int WordWidthOut, unsigned int NLayers>
void Generic_LayerRouter( ap_uint<WordWidthIn> data_in[(1<<NbitsAddress)]
	, ap_uint<WordWidthOut> data_out[NLayers][(1<<NbitsAddress)] )
{
	
	// input BRAM - stubs from link x 
	#pragma HLS RESOURCE variable=data_in core=RAM_2P
	// paritition to be able to access 
	// multiple stubs per clock cycle 
	// cyclic allows you to acess factor*2 
	// consecutive elements at a time 
	#pragma HLS array_partition variable=data_in cyclic factor=2 // 32 
	constexpr unsigned int cStubIncrement=4; // factor*2 
	constexpr unsigned int cNloops=(1<<NbitsAddress)/cStubIncrement;

	unsigned int cLpIndex=0;
	//#pragma HLS array_partition variable=data_out complete
	bool cDone=false;
	LOOP_ReadLoop:
	for (unsigned i=0; i < (1 << NbitsAddress) ; i+=cStubIncrement)
	{
		#pragma HLS pipeline II=1 
		bool cDone = (data_in[i] == 0 ) ? true : false;
		if( !cDone )
		{
			LOOP_FilterLoop:
			for( unsigned int j = 0;  j < cStubIncrement ; j++ )
			{
				#pragma HLS unroll
				ap_uint<NbitsAddress> addr_r = i + j; 
				ap_uint<WordWidthIn> data_new = data_in[addr_r];
				// decode layer 
		 		ap_uint<2> cLyrId = data_new.range(WordWidthIn-1,WordWidthIn-2)&0x3;
		 		// push this element into the temporary arrays 
		 		// used to store information from this iteration 
		 		ShiftData : 
		 		{

	 				LOOP_ShiftData:
	 				for( unsigned int k = (1<<NbitsAddress)-1; k > 0; k-- )
		 			{
	 					#pragma HLS unroll 
	 					*(&data_out[cLyrId][k])= data_out[cLyrId][k-1];
	 				}
	 				// first element --> new data 
	 				*(&data_out[cLyrId][0])= addr_r;
					//*(&data_out[cLyr][0])=(cLyrId==cLyr) ? data_new.range(WordWidthOut-1,0) : cNoMatch;
		 		}
			}
		}
		cLpIndex++;
	}
}

template<unsigned int NbitsInput, unsigned int NbitsWordIn, unsigned int NbitsWordOut >
void GenericSorter_EOT( ap_uint<NbitsWordIn> data_in[(1 << NbitsInput)]
	, ap_uint<NbitsWordOut> data_out[(1 << NbitsInput)]  )
{
	#pragma HLS clock domain=slow_clock 
	// input BRAM - stubs from link x 
	// paritition to be able to access 
	// multiple stubs per clock cycle 
	// cyclic allows you to acess factor*2 
	// consecutive elements at a time 
	#pragma HLS array_partition variable=data_in complete// 32  
	
	// array to hold even entries
	constexpr unsigned int N = (1 << NbitsInput); 
	ap_uint<NbitsWordIn> data_evenOdd[N];
	#pragma HLS array_partition variable=data_evenOdd complete // 32  

	bool cDone =false; 
	int cSortingLoopIndex=0; 
	LOOP_Sorting :  
	while( !cDone )
	{
		#pragma HLS pipeline II=1 
		#ifndef __SYNTHESIS__
			std::cout << "Sorting .. on iteration "
				<< cSortingLoopIndex  
				<< "\n";
		#endif
		cDone= true;
		// even-odd comparator lines
		// un-roll loop to perform all comparisons in parallel 
		LOOP_EvenOddComparitor : 
		for (unsigned j = 0; j < (N / 2); j++) 
		{
			#pragma HLS unroll
			ap_uint<2> c1 = data_in[2 * j].range(NbitsWordIn-1,NbitsWordIn-2)&0x3;
			ap_uint<2> c2  = data_in[2 * j+1].range(NbitsWordIn-1,NbitsWordIn-2)&0x3; 
			// switch odd and even 
			if ( c1 > c2 ) 
			{
				cDone = false; 
				data_evenOdd[2*j] =  data_in[2 * j+1]; 
				data_evenOdd[2*j+1] =  data_in[2 * j];
			}
			else
			{
				data_evenOdd[2*j] =  data_in[2 * j];
				data_evenOdd[2*j+1] =  data_in[2 * j+1]; 
			}
		}

		// odd lines
		// un-roll loop to perform all comparisons in parallel 
		LOOP_OddEvenComparitor : 
		for (unsigned j = 0; j < (N / 2 - 1 ); j++) 
		{
			#pragma HLS unroll
			ap_uint<2> c1 = data_evenOdd[2 * j + 1 ].range(NbitsWordIn-1,NbitsWordIn-2)&0x3;
			ap_uint<2> c2  = data_evenOdd[2 * j + 2].range(NbitsWordIn-1,NbitsWordIn-2)&0x3; 
			// switch odd and even 
			if ( c1 > c2 ) 
			{
				cDone = false; 
				data_in[2*j+1] =  data_evenOdd[2 * j + 2]; 
				data_in[2*j+2] =  data_evenOdd[2 * j + 1];
			}
			else
			{
				data_in[2*j+1] =  data_evenOdd[2 * j+1];
				data_in[2*j+2] =  data_evenOdd[2 * j+2]; 
			}
		}
		// carry over first and last 
		data_in[0] = data_evenOdd[0];
		data_in[N-1]= data_evenOdd[N-1];
		cSortingLoopIndex++;
	};
	LOOP_Fill :  
	for (unsigned j =0; j < N  ; j++)
	{
		#pragma HLS pipeline II=1
		if( data_in[j] != 0 ) 
		{ 
			ap_uint<2> cLyr  = data_in[j].range(NbitsWordIn-1,NbitsWordIn-2)&0x3;
			#ifndef __SYNTHESIS__
				std::cout << "Loop index  "
					<< +j
					<< " layer id "
					<< cLyr 
					<< " stub word is "
					<< std::bitset<NbitsWordIn>(data_in[j])
					<< "\n";
			#endif
			*(&data_out[j])=data_in[j].range(NbitsWordOut-1,0);
		}
		else
			continue;
	}
}

//generic IR implementation 
// void InputRouter_Generic( const BXType bx
// 	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
// 	, const ap_uint<kLINKMAPwidth> hLinkWord 
// 	, ap_uint<kNBits_DTC> hAllStubs[kMAXIRStubs] );

//generic IR implementation 
void InputRouter_Generic( const BXType bx
	, ap_uint<kNBits_DTC> hStubs[kMaxStubsFromLink]
	, const ap_uint<kLINKMAPwidth> hLinkWord 
	, ap_uint<kBRAMwidth> hOutput[kMaxSizeArray] );

#endif


