// InputRouter Test
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
// then 2 bits 
// 1 bit to assign whether link is PS/2S 
// 1 bit if it is connected to the very first layer of the tracker or not 
// 18 bits in total per link .. 
// so can fit all links into a BRAM 12 deep ( 36 bits wide )
// for the moment I store them one link per word 
// (so a 36 bit wide bram , 24 deep can be used) 
void getLinkInfo(LinkMap pInputMap, int pLinkId, 
  ap_uint<kLINKMAPwidth>& pLinkWord)
{
  // read files with stubs .. this is in the 'input' comparison [all c++ ... nothing to do with HLS for the moment]
  // figure out DTC map encoding for this link 
  pLinkWord = 0x0000;
  uint32_t cWord = 0x00000000; 
  bool cIs2S = ( pInputMap[static_cast<int>(pLinkId)].first.find("2S") != std::string::npos  ); 
  auto cLayerIterator = pInputMap[static_cast<int>(pLinkId)].second.begin();
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
    
    cLayerIterator++;
  }
  cWord = cWord | (cFirstLayer << 17) | (cIs2S << 16);
  pLinkWord = ap_uint<kLINKMAPwidth>( cWord);
  std::cout  << "DTC " << pInputMap[static_cast<int>(pLinkId)].first << " Link " << +pLinkId << " -- DTC map encoded word is " << std::bitset<kLINKMAPwidth>(pLinkWord) << "\n";
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
  for(int cSelectedBx=10; cSelectedBx < 11 ; cSelectedBx ++ )
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
  	getLinkInfo(cInputMap, cDTC_LinkId, hLinkWord);
  	// module under test here 
  	//ok..but for events > 8?
  	// check what the file read utility 
  	// does 
  	BXType hBx = cSelectedBx&0x7;

    // now preapre memories that the IR 
    // top level will fill 
    AllStubMemory<BARRELPS> hL1[8];
    AllStubMemory<DISKPS> hL2[4];
    AllStubMemory<DISKPS> hL3[4];
    AllStubMemory<DISKPS> hL4[4];
    
  	// actual IR module under test 
  	InputRouter_PS_1Barrel3Disk( hBx, 
  		cStubs, hLinkWord, 
  		hL1, hL2, hL3, hL4);
    int cLyrIndx=0;
    int cPhiBin=1;
    std::vector<int> cErrors(0);
    for(int cPhiBin=0; cPhiBin < 8 ; cPhiBin++)
    {
      std::string cMemPrint = getMemPrint(cDTCname ,cLyrIndx, cPhiBin, cNonant, hLinkWord);
      if( !cMemPrint.empty() )
      {
        if( IR_DEBUG )
        {
          auto cEntries = hL1[cPhiBin].getEntries(hBx);
          std::cout << "TopLevel found " 
            << +cEntries
            << " stub in L"
            << +cLyrIndx
            << " phi bin "
            << +cPhiBin
            << " for Bx "
            << hBx 
            << "\n";
        }
        std::cout << "Phi Bin " << cPhiBin << "\n";
        ifstream cInputStream;
        openDataFile(cInputStream,cMemPrint); 
        int cErrorCount = compareMemWithFile<AllStubMemory<BARRELPS>,2>(hL1[cPhiBin],cInputStream,cSelectedBx,"AllStub",cTruncation);
        cErrors.push_back( cErrorCount );
      }
      else
        cErrors.push_back(0);
    }
    cTotalErrorCount += std::accumulate(cErrors.begin(), cErrors.end(), 0);
    cErrorCount_L1.push_back(cErrors);
  }
  // now look at the mismatches
  for(int cSelectedBx=10; cSelectedBx < 11 ; cSelectedBx++ )
  {
    for(int cPhiBin=0; cPhiBin < 8 ; cPhiBin++)
    {
      if( cErrorCount_L1[cSelectedBx][cPhiBin] != 0 ) 
        std::cout << "Found " << +cErrorCount_L1[cSelectedBx][cPhiBin]
          << std::dec 
          << " mismatches in Bx#" << cSelectedBx 
          << " [ phi bin " << cPhiBin
          << " ]"
          << "\n";
    }
  }
  std::cout << "Found " << cTotalErrorCount << " mismatches." << "\n";
	return cTotalErrorCount;
}
