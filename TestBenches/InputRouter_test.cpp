// InputRouter Test
#include "InputRouterTop.h"
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



int main()
{
 
  DTCStubMemory hMemories[kMaxIRMemories];

  // clear memories
  for (unsigned int cMemIndx = 0; cMemIndx < kNMemories; cMemIndx++) 
  {
    for( unsigned int cBxId= 0; cBxId < 8 ; cBxId++)
    {
      hMemories[cMemIndx].clear(cBxId);
    }
  }


  bool cTruncation = true;
  int cDTCsplit=0;
  int cNonant=4;
  std::string cInputFile_LinkMap = "emData/dtclinklayerdisk.dat";
 
  //
  int cFirstBx = 0 ;
  int cLastBx = 8 ;
  // 
  int pLinkId = 18; 
  int cTotalErrorCount = simInputRouter( hMemories
    , pLinkId 
    , cInputFile_LinkMap
    , cFirstBx
    , cLastBx
    , cDTCsplit
    , cNonant
    , cTruncation ); 
  return cTotalErrorCount;
}
