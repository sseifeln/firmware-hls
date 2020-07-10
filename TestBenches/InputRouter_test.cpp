// InputRouter Test
#include "InputStubMemory.h"
#include "AllStubMemory.h"
#include "InputRouterTop.h"
#include "FileReadUtility.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <map>

// link assignment table 
// link assignment table 
static const ap_uint<kLINKMAPwidth> kLinkAssignmentTable[] =
#include "../emData/LinkAssignment.dat"
;

// LUT with phi corrections to the nominal radius. Only used by layers.
// Values are determined by the radius and the bend of the stub.
static const int kPhiCorrtable_L1[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL1.txt"
;
static const int kPhiCorrtable_L2[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL2.txt"
;
static const int kPhiCorrtable_L3[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL3.txt"
;
static const int kPhiCorrtable_L4[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL4.txt"
;
static const int kPhiCorrtable_L5[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL5.txt"
;
static const int kPhiCorrtable_L6[] =
#include "../emData/MemPrints/Tables/VMPhiCorrL6.txt"
;


using namespace std;

// map of input links  [per DTC ]
using LinkMap = std::map<int, std::pair<std::string ,std::vector<std::uint8_t>>> ; 
//map of input stubs [ per Bx ]
using InputStubs = std::map<int, std::vector<std::string>> ; 
//vector of stubs 
using Stubs = std::vector<std::bitset<kNBits_DTC>>; 
// 

// create link map
// map is 
// link id [key]
// DTC name [from dtclinklayer]
// then a list of encoded layers 
void createLinkMap(std::string pInputFile_LinkMap, int pDTCsplit, LinkMap& pLinkMap ) 
{
  std::string cBaseName  = "Link_";
  //std::cout << "Loading link map into memory .. will be used later" <<std::endl;
  std::ifstream fin_il_map;
  if (not openDataFile(fin_il_map,pInputFile_LinkMap)) 
  {
    std::cout << "Could not find file " 
      << pInputFile_LinkMap << std::endl;
  }
  // std::cout << "Reading link map from file : " 
  //   << pInputFile_LinkMap << std::endl;
  size_t cLinkCounter=0;
  // parse link map 
  for(std::string cInputLine; getline( fin_il_map, cInputLine ); )
  {
    auto cStream = std::istringstream{cInputLine};
    std::string cToken;
    while (cStream >> cToken) 
    {
      bool cIsAlNum =true;
      for( auto cChar : cToken )
        cIsAlNum = cIsAlNum && std::isalnum(cChar);
      if( !cIsAlNum ) // input link name 
      {
        if( cToken.find("2S") != std::string::npos 
          || cToken.find("PS") != std::string::npos ) 
        {
          pLinkMap[cLinkCounter].first += cBaseName + cToken;
          pLinkMap[cLinkCounter].first += (pDTCsplit==0)?"_A":"_B";
          //if( cToken[0] == 'n')
          //{
            //Link_PS10G_1_A.dat
            //pLinkMap[cLinkCounter].first = cBaseName + cToken.substr(4, cToken.length()-3); 
            //pLinkMap[cLinkCounter].first += "_B";
          //}
          //else
            //pLinkMap[cLinkCounter].first = cBaseName + cToken + "_A" ; //Link_PS10G_1_A.dat
          if( IR_DEBUG)
            std::cout << "Link name : " << pLinkMap[cLinkCounter].first << "\n";
        }
      }
      else
      {
        auto cLayerId = std::stoi( cToken);
        if(cLayerId != -1 )
          pLinkMap[cLinkCounter].second.push_back( cLayerId );
      }
    }
    cLinkCounter++;
  }
  //std::cout << "Mapped out " << +pLinkMap.size() << " links." << std::endl;
  
}

// get link id from map .. 
void getLinkId(LinkMap pInputMap, std::string pDTC, int &pLinkId)
{
  bool cFound=false;
  auto cMapIterator = pInputMap.begin();
  do
  {
    auto cDTCname = (*cMapIterator).second.first;
    auto cLayers = (*cMapIterator).second.second;
    if( cDTCname.find(pDTC) != std::string::npos  ) 
      cFound=true;
    else
      cMapIterator++;
  }while(!cFound && cMapIterator != pInputMap.end());
  assert( cFound ); 
  pLinkId = (cFound)? (*cMapIterator).first : -1;  
}

// get link information 
// memory to store LUT for mapping of DTCs to layers/disks/etc. 
// 3 bits for layer/disk id  --> 3 bits 
// 1 bit for barrel/disk --> 4 bits  
// up-to 4 layers/disks per DTC
// 16 bits per link
// then 3 bits 
// 1 bit to assign whether link is PS/2S 
// 3 bits to encode the number of layers readout by this DTC 
// 20 bits in total 
void getLinkInfo(LinkMap pInputMap, int pLinkId, 
  ap_uint<kLINKMAPwidth>& pLinkWord)
{
  // read files with stubs .. this is in the 'input' comparison [all c++ ... nothing to do with HLS for the moment]
  // figure out DTC map encoding for this link 
  pLinkWord = 0x0000;
  uint32_t cWord = 0x00000000; 
  int cIs2S = ( pInputMap[static_cast<int>(pLinkId)].first.find("2S") != std::string::npos  ) ? 1 : 0 ; 
  auto cLayerIterator = pInputMap[static_cast<int>(pLinkId)].second.begin();
  int cNLyrs=0;
  bool cFirstLayer=false;
  while( cLayerIterator <  pInputMap[static_cast<int>(pLinkId)].second.end() ) // layer id is either layer number or disk number 
  {
    auto cLayerCounter = std::distance( pInputMap[static_cast<int>(pLinkId)].second.begin(), cLayerIterator ); 
    size_t cLayerId =  *cLayerIterator;

    auto cIsBarrel = (cLayerId<10); 
    cLayerId = (cLayerId < 10 ) ? cLayerId : (cLayerId-10);
    cWord  = cWord | ( ( (cLayerId << 1) | cIsBarrel ) << 4*cLayerCounter );  
    if( cLayerId == 1 && cIsBarrel == 1) // first barrel layer is special ...
      cFirstLayer = true;
    cNLyrs++;
    cLayerIterator++;
  }
  cWord = (cNLyrs << 17) | ( cIs2S << 16) | cWord ;
  pLinkWord = ap_uint<kLINKMAPwidth>( cWord);
  if( IR_DEBUG )
  {
    std::cout  << "DTC " << pInputMap[static_cast<int>(pLinkId)].first 
      << " Link " << +pLinkId
      << " -- DTC map encoded word is " 
      << std::bitset<kLINKMAPwidth>(pLinkWord) 
      << " -- "
      << std::hex 
      << +pLinkWord 
      << std::dec 
      << " Is2S bit is set to " << +cIs2S
      << " which is " << +(pLinkWord.range(kLINKMAPwidth-1,kLINKMAPwidth-3))
      << " layers"
      << "\n";
  }
}


// get stubs from emulation file 
// get stubs from file 
bool getStubs(std::string pDTC , InputStubs& pInputStubs)
{
  std::string cBaseName  = "emData/MemPrints/InputStubs/Link_";
  std::string cInputFile = cBaseName + pDTC + ".dat";
  pInputStubs.clear();
  std::ifstream fin_il;
  bool validil = openDataFile(fin_il,cInputFile); // what does this do? 
  if (not validil) 
  {
    std::cout << "Could not find file " << cInputFile << std::endl;
    return false;
  }
  std::cout << "Read-back stubs from file : " << cInputFile << std::endl;

  
  // process the text file .. just regular c++ here 
  char cStubDelimeter = '|';
  char cStubEnd = ' ';
  int  cEventCounter=-1;
  std::string cBxLabel = "BX ";
  int cNevents = 1; 
  int cModBx = -1;
  int cBxCounter=0;
  for(std::string cInputLine; getline( fin_il, cInputLine ); )
  {
    if( cInputLine.find("Event") != std::string::npos ) 
    {
		//cStubString.first = cInputLine.substr(cInputLine.find(cBxLabel)+cBxLabel.length(), 3 ) ;
		cBxCounter = std::stoi( cInputLine.substr(cInputLine.find(cBxLabel)+cBxLabel.length(), 3 ) , nullptr,2 );
		cEventCounter++;
		cModBx++;
	}
    else
    {
      // clean up string to access stub from this event 
      cInputLine.erase( std::remove(cInputLine.begin(), cInputLine.end(), cStubDelimeter), cInputLine.end() );
      pInputStubs[cModBx].push_back( cInputLine.substr(0, cInputLine.find(cStubEnd)) );
      //cStubString.second = cInputLine.substr(0, cInputLine.find(cStubEnd));
      //cInputStubs.push_back( cStubString );
    }
  }
  return true;
}

// get stubs for bx 
void readStubs(std::string pLinkFile, int pBxSelected, ap_uint<kNBits_DTC> *pStubs) 
{
  // get stubs + fill memories with IR process 
  InputStubs cInputStubs;

  getStubs(pLinkFile , cInputStubs);
  for( int cBx = pBxSelected ; cBx < pBxSelected+1 ; cBx++)
  {
    auto& cStubs = cInputStubs[cBx];
    for( auto cStubIter = cStubs.begin(); cStubIter < cStubs.end(); cStubIter++)
    {
    	auto cStubCounter = std::distance( cStubs.begin(), cStubIter ); 
      	auto& cStub = *cStubIter;
      	pStubs[cStubCounter]=ap_uint<kNBits_DTC>( std::bitset<kNBits_DTC>( cStub.c_str() ).to_ulong()) ;
    }
  }
}

// compare memory with emulation 
std::string getMemPrint(std::string pDTC
  , int pLyrIndx , int pPhiBin , int pNonant
  , ap_uint<kLINKMAPwidth> hLinkWord )
{
  std::string cBaseName  = "emData/MemPrints/InputStubs/InputStubs_IL_";
	if( IR_DEBUG )
		std::cout << "DTC word is " << std::bitset<kLINKMAPwidth>(hLinkWord) << "\n";

	ap_uint<4> cDTCWord = (hLinkWord & (0xF << pLyrIndx*4)) >> (pLyrIndx*4); 
	if( cDTCWord != 0 )
	{
    if( IR_DEBUG )
      std::cout << "Lyr word is " << std::bitset<4>(cDTCWord) << "\n";

		ap_uint<1> cIsBrl = (cDTCWord&0x01); 
    std::string cMemoryPrint = cBaseName;
    cMemoryPrint += (cIsBrl==1) ? "L" : "D";
    uint8_t cLyrId =  (cDTCWord & 0xE) >> 1;
    cMemoryPrint += std::to_string((cLyrId)); 
    std::stringstream ss;
    ss << "PHI" ; 
    ss << static_cast<char>(int('A') + int(pPhiBin));
    ss << "_" << pDTC << "_";
    ss << std::setw(2) << std::setfill('0') << pNonant;
    cMemoryPrint += ss.str() + ".dat";
    ifstream cInputStream;
    if( openDataFile(cInputStream,cMemoryPrint) )
    {
      cInputStream.close();
      if( IR_DEBUG )
      {
        std::cout << "Lyr#" << +pLyrIndx 
          << " Lyr " << +cLyrId
          << " Mem print name " << cMemoryPrint 
          << "\n";
      }
      return cMemoryPrint; 
    }
    else
      return "";
  } 		
}	


int main()
{
  bool cTruncation = false;
  int cDTCsplit=0;
  int cNonant=4;
	std::string cDTCname_PS = "PS10G_2";
	std::string cDTCname_2S = "2S_4";
	std::string cDTCname = cDTCname_PS; 
  cDTCname += (cDTCsplit==0) ? "_A" : "_B";

	//MemPrints/InputStubs/
	std::string cInputFile_LinkMap = "emData/dtclinklayerdisk.dat";
	std::cout << "Parsing link map : " << cInputFile_LinkMap << std::endl;

	// create link map 
	LinkMap cInputMap;
	createLinkMap(cInputFile_LinkMap, cDTCsplit, cInputMap );
	std::cout << "Mapped out " << +cInputMap.size() << " links." << std::endl;

	// get link Id 
	int cDTC_LinkId;
	getLinkId(cInputMap, cDTCname , cDTC_LinkId );
	std::cout << "DTC " << cDTCname << " is link#" <<  cDTC_LinkId << std::endl;


  int cTotalErrorCount=0;
  std::vector<std::vector<int>> cErrorCount_L1(0);
  int cFirstBx=0;
  int cLastBx=1;
  for(int cSelectedBx=cFirstBx; cSelectedBx < cLastBx ; cSelectedBx ++ )
  {
    std::cout << "Bx" << +cSelectedBx << "\n";  
    // first prepare array 
  	ap_uint<kNBits_DTC> cStubs[kMaxStubsFromLink];
  	for( int cIndx=0; cIndx < kMaxStubsFromLink; cIndx++)
  		cStubs[cIndx]=ap_uint<kNBits_DTC>(0);
    
    // then fill array with 
  	// stubs from emulation 
  	// for a given Bx [Event# in the file from emulatin]
  	readStubs( cDTCname , cSelectedBx, cStubs ) ;
  	if( IR_DEBUG )
  	{
  		for(auto cStub : cStubs )
  		{
  			if( IR_DEBUG )
  			{
  				if( cStub != 0 )
  				{
  					std::cout << "Stub : " 
  						<< std::bitset<kNBits_DTC>(cStub)
  						<< " -- " << std::hex 
  						<< cStub << std::dec 
  						<< std::endl;
  				}
  			}
  		}		
  	}

  	// and get the link word that I would like 
  	// to use 
  	// can eventually replace this with a LUT 
  	ap_uint<kLINKMAPwidth> hLinkWord = 0x00;

    ap_uint<6> hLinkId(cDTC_LinkId);
    hLinkWord=kLinkAssignmentTable[hLinkId%24];
    std::cout << "Link Word is " 
      << std::bitset<kLINKMAPwidth>(hLinkWord)
      << "\n";
    // module under test here 
  	//ok..but for events > 8?
  	// check what the file read utility 
  	// does 
  	BXType hBx = cSelectedBx&0x7;

    //try and make one large array 
    //to hold all memories 
    int nMemories=20;
    InputStubMemory<TRACKER> *hMemories = new InputStubMemory<TRACKER>[nMemories];
    InputRouterTop( hLinkId 
      , kLinkAssignmentTable
      , kPhiCorrtable_L1 
      , kPhiCorrtable_L2
      , kPhiCorrtable_L3
      , kPhiCorrtable_L4
      , kPhiCorrtable_L5
      , kPhiCorrtable_L6
      , cStubs 
      , hMemories);

    ap_uint<1> hIs2S = hLinkWord.range(kLINKMAPwidth-3,kLINKMAPwidth-4);
    int cMemIndx=0;
    for(int cLyrIndx=0; cLyrIndx<2; cLyrIndx++)
    {
      ap_uint<4> hWrd = hLinkWord.range(4*cLyrIndx+3,4*cLyrIndx);
      ap_uint<1> hIsBrl = hWrd.range(1,0);
      ap_uint<3> hLyrId = hWrd.range(3,1);
      int cNPhiBns = ( (hIs2S==0) && hLyrId==1 && hIsBrl) ? 8 : 4; 
      std::vector<int> cErrors(0);
      for( int cPhiBn=0; cPhiBn<cNPhiBns; cPhiBn++)
      {
        std::string cMemPrint = getMemPrint(cDTCname ,cLyrIndx, cPhiBn, cNonant, hLinkWord);
        int cErrorCount =0;
        if( !cMemPrint.empty() )
        {
          if( IR_DEBUG )
          {
            std::cout << "Phi Bin " << cPhiBn << " of Lyr" << hLyrId << "\n";
            auto cEntries = hMemories[cMemIndx].getEntries(hBx);
            std::cout << "TopLevel found " 
              << std::dec 
              << +cEntries
              << " stubs "
              << " for Bx "
              << cSelectedBx 
              << std::dec 
              << "\n";
          }
          ifstream cInputStream;
          openDataFile(cInputStream,cMemPrint); 
          cErrorCount = compareMemWithFile<InputStubMemory<TRACKER>,2>(hMemories[cMemIndx],cInputStream,cSelectedBx,"AllStub",cTruncation);
          cInputStream.close();
        }
        cErrors.push_back( cErrorCount );
        cMemIndx++;
      }
      cTotalErrorCount += std::accumulate(cErrors.begin(), cErrors.end(), 0);
    }
  }
  std::cout << "Found " << cTotalErrorCount << " mismatches." << "\n";
	return cTotalErrorCount;
}
