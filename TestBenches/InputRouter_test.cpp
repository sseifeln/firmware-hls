// InputRouter Test
// Sarah.Storey@cern.ch for questions/comments 
#include "hls_stream.h"
#include "InputStubMemory.hh"
#include "FileReadUtility.hh"

// #include "InputRouter.cc"
#include "InputRouterTop.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <bitset> 

using namespace std;

// map of input links  [per DTC ]
using LinkMap = std::map<int, std::pair<std::string ,std::vector<std::uint8_t>>> ; 
//map of input stubs [ per Bx ]
using InputStubs = std::map<int, std::vector<std::string>> ; 
//vector of stubs 
using Stubs = std::vector<std::bitset<kNBits_DTC>>; 

void parseLinkMap(std::string pInputFile_LinkMap, LinkMap& pLinkMap ) 
{
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
          if( cToken[0] == 'n')
          {
            //Link_PS10G_1_A.dat
            pLinkMap[cLinkCounter].first = cToken.substr(4, cToken.length()-3); 
            pLinkMap[cLinkCounter].first += "_B";
          }
          else
            pLinkMap[cLinkCounter].first = cToken + "_A" ; //Link_PS10G_1_A.dat
          //std::cout << "Link name : " << pLinkMap[cLinkCounter].first << "\n";
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
  ap_uint<kLINKMAPwidth>& pLinkWord, std::string& pLinkName )
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
  pLinkName = pInputMap[static_cast<int>(pLinkId)].first;//"IL/Link_PS10G_1_A.dat";
}

void getLinkId(LinkMap pInputMap, std::string pDTC, int &pLinkId)
{
  bool cFound=false;
  auto cMapIterator = pInputMap.begin();
  do
  {
    auto cDTCname = (*cMapIterator).second.first;
    auto cLayers = (*cMapIterator).second.second;
    if( cDTCname == pDTC ) 
      cFound=true;
    else
      cMapIterator++;
  }while(!cFound && cMapIterator != pInputMap.end());
  assert( cFound ); 
  pLinkId = (cFound)? (*cMapIterator).first : -1;  
}
// get stubs from file 
bool getStubs(std::string pInputFile , InputStubs& pInputStubs)
{
  std::ifstream fin_il;
  bool validil = openDataFile(fin_il,pInputFile); // what does this do? 
  if (not validil) 
  {
    std::cout << "Could not find file " << pInputFile << std::endl;
    return false;
  }
  std::cout << "Read-back stubs from file : " << pInputFile << std::endl;

  
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
      // std::cout << "Stub from Bx "
      //   << cBxCounter
      //   << " event "
      //   << cEventCounter 
      //   << " -- Bx modified "
      //   << cModBx
      //   << " -- "
      //   << cInputLine.substr(0, cInputLine.find(cStubEnd))
      //   << "\n";
      pInputStubs[cModBx].push_back( cInputLine.substr(0, cInputLine.find(cStubEnd)) );
      //cStubString.second = cInputLine.substr(0, cInputLine.find(cStubEnd));
      //cInputStubs.push_back( cStubString );
    }
  }
  return true;
}

// get stubs from file 
// based on DTC name 
bool getStubs(std::string pInputFile, std::string pDTCname, InputStubs& pInputStubs)
{
  // link map 
  LinkMap cInputMap;
  parseLinkMap(pInputFile, cInputMap );
  std::cout << "Mapped out " << +cInputMap.size() << " links." << std::endl;
  
  // figure out DTC map encoding for this DTC 
  int cLinkId=0;
  getLinkId(cInputMap, pDTCname, cLinkId);
 
  
  ap_uint<kLINKMAPwidth> cLinkWord = 0x0000;
  getLinkInfo(cInputMap, cLinkId, cLinkWord, pDTCname);
  std::string cLinkFile = "IL/IL_" + pDTCname + "/Link_" + pDTCname + ".dat" ;
  
  getStubs(cLinkFile , pInputStubs);
}


// get stubs for bx 
void getStubs(std::string pLinkFile, int pBxSelected, Stubs &pStubs) 
{
  // get stubs + fill memories with IR process 
  InputStubs cInputStubs;
  getStubs(pLinkFile , cInputStubs);
  pStubs.clear();
  for( int cBx = pBxSelected ; cBx < pBxSelected+1 ; cBx++)
  {
    BXType hBx = cBx&0x7;
    auto& cStubs = cInputStubs[cBx];
    for( auto cStubIter = cStubs.begin(); cStubIter < cStubs.end(); cStubIter++)
    {
      auto cStubCounter = std::distance( cStubs.begin(), cStubIter ); 
      auto& cStub = *cStubIter;
      pStubs.push_back(std::bitset<kNBits_DTC>( cStub.c_str() ) ); 
    }
  }
}

// get stubs from file 
// based on DTC name 
// and bx 
bool getStubs(std::string pInputFile, std::string pDTCname, int pBxSelected, Stubs &pStubs, ap_uint<kLINKMAPwidth>& pLinkWord )
{
  // figure out DTC map encoding for this DTC
  LinkMap cInputMap;
  parseLinkMap(pInputFile, cInputMap );
  int cLinkId=0;
  getLinkId(cInputMap, pDTCname, cLinkId);
  pLinkWord = 0x0000;
  getLinkInfo(cInputMap, cLinkId, pLinkWord, pDTCname);

  InputStubs cInputStubs;
  std::string cLinkFile = "IL/IL_" + pDTCname + "/Link_" + pDTCname + ".dat" ;
  getStubs(cLinkFile , cInputStubs);

  pStubs.clear();
  for( int cBx = pBxSelected ; cBx < pBxSelected+1 ; cBx++)
  {
    BXType hBx = cBx&0x7;
    auto& cStubs = cInputStubs[cBx];
    for( auto cStubIter = cStubs.begin(); cStubIter < cStubs.end(); cStubIter++)
    {
      auto cStubCounter = std::distance( cStubs.begin(), cStubIter ); 
      auto& cStub = *cStubIter;
      pStubs.push_back(std::bitset<kNBits_DTC>( cStub.c_str() ) ); 
    }
  }
}

// fill input stream with stubs 
void fillInputStream(Stubs pStubs, hls::stream<ap_uint<kNBits_DTC>>& hLink)
{
  for( auto cStubIter = pStubs.begin(); cStubIter < pStubs.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubs.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      hLink.write_nb(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }
}

// fill input stream with stubs 
void fillInputStreams(Stubs pStubsPS, Stubs pStubs2S,
 hls::stream<ap_uint<kNBits_DTC>> hLinks[2])
{
  for( auto cStubIter = pStubsPS.begin(); cStubIter < pStubsPS.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubsPS.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      (&hLinks[0])->write_nb(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

  for( auto cStubIter = pStubs2S.begin(); cStubIter < pStubs2S.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubs2S.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      (&hLinks[1])->write_nb(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

}

void prepareStream(ap_uint<kNBits_DTC> hLink[kMaxStubsFromLink])
{
  for (int cStubCounter=0; cStubCounter<kMaxStubsFromLink; cStubCounter++)
  {
    hLink[cStubCounter]=0x00;
  }
}
// fill input stream with stubs 
void fillInputStreams(Stubs pStubsPS, Stubs pStubs2S
 , ap_uint<kNBits_DTC> hLinkPS[kMaxStubsFromLink]
 , ap_uint<kNBits_DTC> hLink2S[kMaxStubsFromLink])
{
  prepareStream(hLinkPS);
  prepareStream(hLink2S);

  for( auto cStubIter = pStubsPS.begin(); cStubIter < pStubsPS.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubsPS.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      hLinkPS[cStubCounter]=(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

  for( auto cStubIter = pStubs2S.begin(); cStubIter < pStubs2S.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubs2S.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      hLink2S[cStubCounter]=(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

}

// fill input stream with stubs 
void fillInputStreams(Stubs pStubsPS, Stubs pStubs2S,
 hls::stream<ap_uint<kNBits_DTC>> &hLinkPS, hls::stream<ap_uint<kNBits_DTC>> &hLink2S )
{
  for( auto cStubIter = pStubsPS.begin(); cStubIter < pStubsPS.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubsPS.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      hLinkPS.write_nb(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

  for( auto cStubIter = pStubs2S.begin(); cStubIter < pStubs2S.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubs2S.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      hLink2S.write_nb(ap_uint<kNBits_DTC>(cStub.to_ulong()));
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

}


// fill input stream with stubs 
void fillInputStreams(Stubs pStubsPS, Stubs pStubs2S,
ap_uint<kNBits_DTC> hLinks[2][kMaxStubsFromLink])
{
  for( auto cStubIter = pStubsPS.begin(); cStubIter < pStubsPS.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubsPS.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      *(&hLinks[0][cStubCounter]) = ap_uint<kNBits_DTC>(cStub.to_ulong());
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

  for( auto cStubIter = pStubs2S.begin(); cStubIter < pStubs2S.end(); cStubIter++)
  {
    auto cStubCounter = std::distance( pStubs2S.begin(), cStubIter ); 
    auto& cStub = *cStubIter;
    if( cStubCounter < kMaxStubsFromLink )
    {
      *(&hLinks[1][cStubCounter]) = ap_uint<kNBits_DTC>(cStub.to_ulong());
    }
    else
    {
      if( cStubCounter == kMaxStubsFromLink) 
        std::cout << "Warning - truncation expected!" 
          << "Stubs from simulation [currently @ stub #" << +cStubCounter 
          << "] exceed maximum allowed on this link.."
          << " not passing to input stream.\n";
    }
  }

}

// compare memory contents with stubs 
// from emulation mem prints 
int compareMemories(std::string pInputFile, 
  std::string pPSDTC, std::string p2SDTC, 
  int pBxSelected, 
  StubsBarrelPS hBarrelPS, StubsDiskPS hDiskPS,
  StubsBarrel2S hBarrel2S, StubsDisk2S hDisk2S)
{

  // figure out DTC map encoding for this DTC
  LinkMap cInputMap;
  parseLinkMap(pInputFile, cInputMap );
  int cLinkId2S=0;
  int cLinkIdPS=0;
  getLinkId(cInputMap, p2SDTC, cLinkId2S);
  getLinkId(cInputMap, pPSDTC, cLinkIdPS);
  
  int cNmismatchedMemories=0;
  std::vector<std::string> cRegionLabels{ "A", "B" , "C", "D", "E", "F","G", "H"};
  std::vector<int> cLinkIds{cLinkIdPS, cLinkId2S};
  for( auto cLinkId : cLinkIds )
  {
    auto cLayerIterator = cInputMap[static_cast<int>(cLinkId)].second.begin();
    bool cFirstLayer=false;
    bool cTruncated=true;
    
    ap_uint<kLINKMAPwidth> cLinkWord=0x00;
    ap_uint<1> cIs2S;
    std::string cDTCname;
    getLinkInfo(cInputMap, cLinkId, cLinkWord, cDTCname);
    is2S(cLinkWord, cIs2S);
    std::cout << "Comparing memories for "
      << cDTCname 
      << " --link "
      << cLinkId 
      << "\n";
    while( cLayerIterator <  cInputMap[static_cast<int>(cLinkId)].second.end() ) 
    {
      size_t cLayerId =  *cLayerIterator;
      auto cIsBarrel = (cLayerId<10); 
      cLayerId = (cLayerId < 10 ) ? cLayerId : (cLayerId-10);
      if( cLayerId == 1 && cIsBarrel == 1) // first barrel layer is special ...
        cFirstLayer = true;
      
      std::string cEmFile = "IL/IL_" + cDTCname + "/InputStubs_IL_";
      if( cIsBarrel )
        cEmFile += "L" + std::to_string(cLayerId);
      else
        cEmFile += "D" + std::to_string(cLayerId);
      
      int cNRegions = (cLayerId==1 && cIsBarrel) ? 8 : 4 ;
      for( int cPhiRegion=0; cPhiRegion < cNRegions ; cPhiRegion++)
      {
        int err_count =0; 
        std::string cRegion;
        if( cLayerId == 1 && cIsBarrel ) 
          cRegion = cRegionLabels[cPhiRegion];
        else
          cRegion = cRegionLabels[cPhiRegion];
          
        std::string cFile = cEmFile + "PHI"+ cRegion + "_" + cDTCname + "_04.dat";
        
        ifstream cInputStream;
        int cCounter=0;
        int cIndex=0;
        if( openDataFile(cInputStream,cFile) )
        {
          std::cout << cFile << "\n";
          if( cIs2S == 0 )
          {
            if( !cIsBarrel  ) 
            {
              if( cLayerId == 1)
                err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m1[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 2 )
                err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m2[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 3 )
                err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m3[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 4 )
                err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m4[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else 
                err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m5[cPhiRegion],cInputStream,pBxSelected,"InputStub");
            }
            else 
            {
              if( cLayerId == 1)
                err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m1[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 2 )
                err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m2[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 3 )
                err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m3[cPhiRegion],cInputStream,pBxSelected,"InputStub");
            }
          }
          else
          {
            if( !cIsBarrel  ) 
            {
              if( cLayerId == 1)
                err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m1[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 2 )
                err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m2[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 3 )
                err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m3[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 4 )
                err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m4[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else 
                err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m5[cPhiRegion],cInputStream,pBxSelected,"InputStub");
            }
            else 
            {
              if( cLayerId == 4)
                err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m1[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 5 )
                err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m2[cPhiRegion],cInputStream,pBxSelected,"InputStub");
              else if( cLayerId == 6 )
                err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m3[cPhiRegion],cInputStream,pBxSelected,"InputStub");
            }
          }
          cNmismatchedMemories += (err_count > 0 );
        }
      }
      cLayerIterator++;
    }
  }
  return cNmismatchedMemories;
}
int main()
{
  #ifndef __SYNTHESIS__
    std::cout << "SYNTHESIS" << std::endl;
  #endif

  int cBxSelected = 0; 
  std::string cInputFile_LinkMap = "IL/dtclinklayerdisk.dat";
  // get stubs 
  Stubs cStubs_DTC_PS; 
  Stubs cStubs_DTC_2S; 
  RouterInputPort cInputs_PS;
  RouterInputPort cInputs_2S;

  //ap_uint<kLINKMAPwidth> cLinkWords[2];
  //hls::stream<ap_uint<kNBits_DTC>> hLinks[2];
  // 
  getStubs(cInputFile_LinkMap, "PS10G_2_B", cBxSelected, cStubs_DTC_PS, cInputs_PS.hLinkWord );
  getStubs(cInputFile_LinkMap, "2S_4_B", cBxSelected, cStubs_DTC_2S, cInputs_2S.hLinkWord );
  fillInputStreams(cStubs_DTC_PS, cStubs_DTC_2S, cInputs_PS.hStubs, cInputs_2S.hStubs );
  
  // memories 
  InputStubMemory<BARRELPS> L1_PS_PhiRA; 
  // PS memories 
  StubsBarrelPS hBarrelPS;
  StubsDiskPS hDiskPS;
  // 2S memories 
  StubsBarrel2S hBarrel2S;
  StubsDisk2S hDisk2S;
  // compare memories 
  BXType hBx = cBxSelected&0x7;
  RouterOutputPort outPS; 
  RouterOutputPort out2S; 
  ap_uint<8> nRoutedPS=0; 
  ap_uint<8> nRouted2S=0;
  //GenericRouter(cInputs_PS,cInputs_2S, nRoutedPS, nRouted2S);
  RouterPS(hBx , cInputs_PS , hBarrelPS , hDiskPS); 

  //InputRouterPS(hBx, cInputs_PS.hStubs, cInputs_PS.hLinkWord, hBarrelPS, hDiskPS);
  InputRouter2S(hBx, cInputs_2S.hStubs, cInputs_2S.hLinkWord, hBarrel2S, hDisk2S);
  int nMismatches = compareMemories(cInputFile_LinkMap,"PS10G_2_B","2S_4_B", cBxSelected,hBarrelPS,  hDiskPS, hBarrel2S,  hDisk2S);
  return nMismatches; 
}