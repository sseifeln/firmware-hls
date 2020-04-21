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

int main()
{
  #ifndef __SYNTHESIS__
    std::cout << "SYNTHESIS" << std::endl;
  #endif

  int cBxSelected = 0; 
  std::string cDTCname = "2S_4_A";
  
  // name is  : 
  // DTCtype[PS10G_PS5G_2S]
  // _[DTC_number]
  // _[which tracking nonant: each DTC reads out 2 tracking nonants]
  std::string cInputFile_LinkMap = "IL/dtclinklayerdisk.dat";
  std::map<int, std::pair<std::string ,std::vector<std::uint8_t>>> cInputMap;
  std::cout << "Loading link map into memory .. will be used later" <<std::endl;
  std::ifstream fin_il_map;
  if (not openDataFile(fin_il_map,cInputFile_LinkMap)) 
  {
    std::cout << "Could not find file " 
      << cInputFile_LinkMap << std::endl;
    return 0;
  }
  std::cout << "Reading link map from file : " 
    << cInputFile_LinkMap << std::endl;
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
            cInputMap[cLinkCounter].first = cToken.substr(4, cToken.length()-3); 
            cInputMap[cLinkCounter].first += "_B";
          }
          else
            cInputMap[cLinkCounter].first = cToken + "_A" ; //Link_PS10G_1_A.dat
          std::cout << "Link name : " << cInputMap[cLinkCounter].first << "\n";
        }
      }
      else
      {
        auto cLayerId = std::stoi( cToken);
        if(cLayerId != -1 )
          cInputMap[cLinkCounter].second.push_back( cLayerId );
      }
    }
    cLinkCounter++;
  }
  std::cout << "Mapped out " << +cInputMap.size() << " links." << std::endl;
  
  // figure out DTC map encoding for this DTC 
  int cLinkId=0;
  getLinkId(cInputMap, cDTCname, cLinkId);
  std::cout << "Found " 
    << cDTCname
    << " link id is "
    << cLinkId
    << "\n";

  ap_uint<kLINKMAPwidth> cLinkWord = 0x0000;
  getLinkInfo(cInputMap, cLinkId, cLinkWord, cDTCname);
  std::string cLinkFile = "IL/IL_" + cDTCname + "/Link_" + cDTCname + ".dat" ;
  std::cout << "DTC name from CMSSW " 
    << cDTCname << " input file is " 
    << cLinkFile << "\n";
  
  // memories for stubs 
  // PS memories 
  StubsBarrelPS hBarrelPS;
  StubsDiskPS hDiskPS;
  // 2S memories 
  StubsBarrel2S hBarrel2S;
  StubsDisk2S hDisk2S;

  // get stubs 
  Stubs cStubs;
  getStubs(cLinkFile, cBxSelected, cStubs);
  // declare input stream to be used in hls test bench 
  hls::stream<ap_uint<kNBits_DTC>> hLink;
  // fill input stream 
  fillInputStream(cStubs, hLink);
 
  // fill memories using IR process 
  BXType hBx = cBxSelected&0x7;
  ap_uint<1> cIs2S;
  is2S(cLinkWord, cIs2S);
  if( cIs2S == 0 )
  {
    InputRouterPS(hBx, hLink, cLinkWord, hBarrelPS, hDiskPS);
  }
  else
  {
    InputRouter2S(hBx, hLink, cLinkWord, hBarrel2S, hDisk2S);
  }

  std::vector<std::string> cRegionLabels{ "A", "B" , "C", "D", "E", "F","G", "H"};
  if( cIs2S == 0 )
  {
    for( size_t cRegion=0; cRegion < 8 ; cRegion++)
    {
      std::cout << "Barrel memory [PS] layer 1 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hBarrelPS.m1[cRegion].getEntries(cBxSelected) 
        << " entries\n";

      for( size_t cIndex = 0 ; cIndex < hBarrelPS.m1[cRegion].getEntries(cBxSelected); cIndex++ )
      {
        auto cStub = hBarrelPS.m1[cRegion].read_mem( cBxSelected, cIndex ).raw();
        std::cout << "Stub#" << +cIndex
          << " -- " << std::hex 
          << cStub 
          << std::dec
          << "\n";
      }

    }
    for( size_t cRegion=0; cRegion < 4 ; cRegion++)
    {
      std::cout << "Endcap memory [PS] layer 2 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hDiskPS.m2[cRegion].getEntries(cBxSelected) 
        << " entries\n";

      for( size_t cIndex = 0 ; cIndex < hDiskPS.m2[cRegion].getEntries(cBxSelected); cIndex++ )
      {
        auto cStub = hDiskPS.m2[cRegion].read_mem( cBxSelected, cIndex ).raw();
        std::cout << "Stub#" << +cIndex
          << " -- " << std::hex 
          << cStub 
          << std::dec
          << "\n";
      }
    }
    for( size_t cRegion=0; cRegion < 4 ; cRegion++)
    {
      std::cout << "Endcap memory [PS] layer 4 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hDiskPS.m4[cRegion].getEntries(cBxSelected) 
        << " entries\n";

    }
  }
  else
  {
    // barrel 2S memories 
    for( size_t cRegion=0; cRegion < 4 ; cRegion++)
    {
      std::cout << "Barrel memory [2S] layer 4 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hBarrel2S.m1[cRegion].getEntries(cBxSelected) 
        << " entries\n";

      std::cout << "Barrel memory [2S] layer 5 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hBarrel2S.m2[cRegion].getEntries(cBxSelected) 
        << " entries\n";

      std::cout << "Barrel memory [2S] layer 6 " 
        << " region " << +cRegion 
        << " [" <<  cRegionLabels[cRegion]
        << "] -- "
        << hBarrel2S.m3[cRegion].getEntries(cBxSelected) 
        << " entries\n";

    }
  }
  auto cLayerIterator = cInputMap[static_cast<int>(cLinkId)].second.begin();
  bool cFirstLayer=false;
  bool cTruncated=true;
  int cNmismatchedMemories=0;
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
        if( cIs2S == 0  )
        {
          if( !cIsBarrel  ) 
          {
            if( cLayerId == 1)
              err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m1[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 2 )
              err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m2[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 3 )
              err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m3[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 4 )
              err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m4[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else 
              err_count = compareMemWithFile<InputStubMemory<DISKPS> >(hDiskPS.m5[cPhiRegion],cInputStream,cBxSelected,"InputStub");
          }
          else 
          {
            if( cLayerId == 1)
              err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m1[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 2 )
              err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m2[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 3 )
              err_count = compareMemWithFile<InputStubMemory<BARRELPS> >(hBarrelPS.m3[cPhiRegion],cInputStream,cBxSelected,"InputStub");
          }
        }
        else
        {
          if( !cIsBarrel  ) 
          {
            if( cLayerId == 1)
              err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m1[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 2 )
              err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m2[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 3 )
              err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m3[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 4 )
              err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m4[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else 
              err_count = compareMemWithFile<InputStubMemory<DISK2S> >(hDisk2S.m5[cPhiRegion],cInputStream,cBxSelected,"InputStub");
          }
          else 
          {
            if( cLayerId == 4)
              err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m1[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 5 )
              err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m2[cPhiRegion],cInputStream,cBxSelected,"InputStub");
            else if( cLayerId == 6 )
              err_count = compareMemWithFile<InputStubMemory<BARREL2S> >(hBarrel2S.m3[cPhiRegion],cInputStream,cBxSelected,"InputStub");
          }
        }
        cNmismatchedMemories += (err_count > 0 );
      }
    }
    cLayerIterator++;
  }
  return cNmismatchedMemories;
}