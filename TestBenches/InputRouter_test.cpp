// InputRouter Test
#include "InputRouterTop.h"
#include "FileReadUtility.h"
#include "InputStubs.h"

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
  
  bool cTruncated = false;
  int cDTCsplit=1;
  int cNonant=4;
  std::string cInputFile_LinkMap = "emData/dtclinklayerdisk.dat";
 
  //
  int cFirstBx = 0 ;
  int cLastBx = 1;
  // 
  int pLinkId = 6; 
  int cTotalErrorCount = simInputRouter( hMemories
    , cTruncated 
    , pLinkId 
    , cInputFile_LinkMap
    , cFirstBx
    , cLastBx
    , cDTCsplit
    , cNonant); 
  return cTotalErrorCount;
}
