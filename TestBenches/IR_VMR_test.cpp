// InputRouter Test
#include "InputRouterTop.h"
#include "VMRouterTop.h"
#include "FileReadUtility.h"
#include "LinkUtility.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>

using namespace std;

const int kMaxVMRInputs = 6; 

int IR_VM_main()
{
	// L1
	int cLyrSlct=1; 
	int cPhiSlct=4; // phi bin "E"
	int cDTCsplit=0;
  	int cNonant=4;
  	int cEvtId=0;
	int cBxId=cEvtId; 

	int cFirstLyrNbins = ( cLyrSlct > 10) ? 4 : 8; 
  	int cLyrIndxCorr = ( cLyrSlct > 10) ? 10 : 0; 
  	int cTkLyr 		 = cLyrSlct - cLyrIndxCorr;
  	int cFrstMemIndx = cFirstLyrNbins*(cTkLyr>1) + (cTkLyr>1)*(cTkLyr-2)*4; 
  	std::cout << "Tk layer "  << cTkLyr
  			<< " First memory index is  " 
  			<< cFrstMemIndx
  			<< "\n";

  	std::cout << "Test bench for Bx# " 
  		<< +cBxId << "\n";

	std::string cInputFile_LinkMap = "emData/dtclinklayerdisk.dat";
	std::cout << "Parsing link map : " << cInputFile_LinkMap << std::endl;
	TkMap cMap = getLyrLinkMap( cInputFile_LinkMap ); 
	LinkMap cLinkMap;
	createLinkMap(cInputFile_LinkMap, cDTCsplit, cLinkMap );

	// array of memories that can be filled by the IR 
	int cInptCntr=0;
	//DTCStubMemory hIRmemories[kMaxVMRInputs][kMaxIRMemories];
	DTCStubMemory hMemories0[kMaxIRMemories];
	DTCStubMemory hMemories1[kMaxIRMemories];
	DTCStubMemory hMemories2[kMaxIRMemories];
	DTCStubMemory hMemories3[kMaxIRMemories];
	DTCStubMemory hMemories4[kMaxIRMemories];
	DTCStubMemory hMemories5[kMaxIRMemories];
	for( auto cItm : cMap )
  	{
  		if( cItm.first != cLyrSlct )
  			continue;

  		auto cLnkIds = cItm.second; 
  		std::cout << +cLnkIds.size()
  			<< "\t... link(s) connected to tk layer " 
  			<< cItm.first 
  			<< "\n";

  		for( auto cLnkId : cLnkIds )
  		{
  			std::cout << "\t\t...Preparing IR memory from Link#" 
  				<< +cLnkId
  				<< " - DTC "
  				<< cLinkMap[cLnkId].first 
  				<< "\n";
			ap_uint<kNBits_DTC> cStubs[kMaxStubsFromLink];
			fillInputStubs(cStubs, cLnkId,  cEvtId 
				, cDTCsplit
				, cInputFile_LinkMap );

			// fill IR memories  
			if( cInptCntr == 0 )
				procInputRouter(hMemories0 , cLnkId  , cEvtId , cStubs ); 
			else if( cInptCntr == 1 )
				procInputRouter(hMemories1 , cLnkId  , cEvtId , cStubs ); 
			else if( cInptCntr == 2 )
				procInputRouter(hMemories2 , cLnkId  , cEvtId , cStubs ); 
			else if( cInptCntr == 3 )
				procInputRouter(hMemories3 , cLnkId  , cEvtId , cStubs ); 
			else if( cInptCntr == 4 )
				procInputRouter(hMemories4 , cLnkId  , cEvtId , cStubs );
			else if( cInptCntr == 5 )
				procInputRouter(hMemories5 , cLnkId  , cEvtId , cStubs ); 
			cInptCntr++;
  		}// loop over links that readout these layers 
  	}// loop over layers 
  	static InputStubMemory<BARRELPS> cIRmemories2Lnks[2];
	static InputStubMemory<BARRELPS> cIRmemories4Lnks[4];
	static InputStubMemory<BARRELPS> cIRmemories6Lnks[6];
	InputStubMemory<BARRELPS>* cIRmemories; 
	if( cInptCntr == 2 ) cIRmemories = cIRmemories2Lnks;
	else if( cInptCntr == 4 ) cIRmemories = cIRmemories4Lnks;
	else if( cInptCntr == 6 ) cIRmemories = cIRmemories6Lnks;

	// for now I want to transform DTCStubMemory to InputStubMemory 
	int cMemIndx = cFrstMemIndx + cPhiSlct;
	BXType hBx = cEvtId&0x7;
  	for( int cLnkIndx= 0 ; cLnkIndx < cInptCntr ; cLnkIndx++)
	{
		DTCStubMemory* hStubMem; 
		if( cLnkIndx == 0 ) hStubMem = hMemories0;
		else if( cLnkIndx == 1 ) hStubMem = hMemories1;
		else if( cLnkIndx == 2 ) hStubMem = hMemories2;
		else if( cLnkIndx == 3 ) hStubMem = hMemories3;
		else if( cLnkIndx == 4 ) hStubMem = hMemories4;
		else if( cLnkIndx == 5 ) hStubMem = hMemories5;

		// loop over entries 
		std::cout << "\t Memory from link#" << cLnkIndx 
				<< " contains " << hStubMem[cMemIndx].getEntries(hBx) 
				<< " for this bx...\n";
		// // clear memory
		cIRmemories[cLnkIndx].clear(cBxId&0x7);
		for( int cIndx=0; cIndx < hStubMem[cMemIndx].getEntries(hBx) ; cIndx++)
		{
			auto hStubRaw = hStubMem[cMemIndx].read_mem( hBx, cIndx);
			std::cout << "\t\t.. entry number " << cIndx 
				<< " stub word is " << std::bitset<hStubRaw.getWidth()>(hStubRaw.raw())
				<< "\n";

			auto cNentries=cIRmemories[cLnkIndx].getEntries( hBx );
			ap_uint<kBRAMwidth> hRaw = ap_uint<hStubRaw.getWidth()>(hStubRaw.raw()).range(kBRAMwidth-1,0); 
			InputStub<BARRELPS> hInputStub(hRaw);
			cIRmemories[cLnkIndx].write_mem( hBx, hInputStub, cNentries); 
		}//loop over memory entries
	}
	
  	// Unit Under Test
  	// output memories
  	static AllStubMemory<BARRELPS> allStub[6];
	// ME memories
	static VMStubMEMemory<BARRELPS, 3> meMemories[4];
	// TE Inner memories, including copies
	static VMStubTEInnerMemory<BARRELPS> teiMemories[4][5];
	// TE Inner Overlap memories, including copies
	static VMStubTEInnerMemory<BARRELOL> olMemories[2][3];
	VMRouterTop(hBx, cIRmemories,
			allStub, meMemories, teiMemories, olMemories);


	// // compare all stub
	// // AllStub
 //  ifstream fout_allstub_n1;
 //  bool valid_allstub_n1 = openDataFile(fout_allstub_n1, "VMR_L1PHIE/AllStubs_AS_L1PHIEn1_04.dat");
 //  if (not valid_allstub_n1) return -1;

 //  ifstream fout_allstub_n2;
 //  bool valid_allstub_n2 = openDataFile(fout_allstub_n2, "VMR_L1PHIE/AllStubs_AS_L1PHIEn2_04.dat");
 //  if (not valid_allstub_n2) return -1;

 //  ifstream fout_allstub_n3;
 //  bool valid_allstub_n3 = openDataFile(fout_allstub_n3, "VMR_L1PHIE/AllStubs_AS_L1PHIEn3_04.dat");
 //  if (not valid_allstub_n3) return -1;

 //  ifstream fout_allstub_n4;
 //  bool valid_allstub_n4 = openDataFile(fout_allstub_n4, "VMR_L1PHIE/AllStubs_AS_L1PHIEn4_04.dat");
 //  if (not valid_allstub_n4) return -1;

 //  ifstream fout_allstub_n5;
 //  bool valid_allstub_n5 = openDataFile(fout_allstub_n5, "VMR_L1PHIE/AllStubs_AS_L1PHIEn5_04.dat");
 //  if (not valid_allstub_n5) return -1;

 //  ifstream fout_allstub_n6;
 //  bool valid_allstub_n6 = openDataFile(fout_allstub_n6, "VMR_L1PHIE/AllStubs_AS_L1PHIEn6_04.dat");
 //  if (not valid_allstub_n6) return -1;

 //  // compare the computed outputs with the expected ones
 //  // add 1 per stub that is incorrect
 //  // AllStub
 //  bool truncation = false;

 //  int err=0;
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[0], fout_allstub_n1, cEvtId, "AllStub", truncation);
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[1], fout_allstub_n2, cEvtId, "AllStub", truncation);
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[2], fout_allstub_n3, cEvtId, "AllStub", truncation);
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[3], fout_allstub_n4, cEvtId, "AllStub", truncation);
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[4], fout_allstub_n5, cEvtId, "AllStub", truncation);
 //  err += compareMemWithFile<AllStubMemory<BARRELPS>>(allStub[5], fout_allstub_n6, cEvtId, "AllStub", truncation);
  

  	
	return 0; 
}

int main()
{
	IR_VM_main();
	return 0;
}