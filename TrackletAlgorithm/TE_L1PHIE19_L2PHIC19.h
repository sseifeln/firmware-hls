#ifndef TE_L1PHIE19_L2PHIC19_H
#define TE_L1PHIE19_L2PHIC19_H

#include "TrackletEngine.h"

void TE_L1PHIE19_L2PHIC19(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair);

#endif
