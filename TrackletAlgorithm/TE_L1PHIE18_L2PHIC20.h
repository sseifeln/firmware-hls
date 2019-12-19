#ifndef TE_L1PHIE18_L2PHIC20_H
#define TE_L1PHIE18_L2PHIC20_H

#include "TrackletEngine.h"

void TE_8(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair);

#endif
