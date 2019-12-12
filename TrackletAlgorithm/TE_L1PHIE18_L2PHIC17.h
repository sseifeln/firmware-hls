#ifndef TE_L1PHIE18_L2PHIC17_H
#define TE_L1PHIE18_L2PHIC17_H

#include "TrackletEngine.h"

void TE_L1PHIE18_L2PHIC17(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair);

#endif
