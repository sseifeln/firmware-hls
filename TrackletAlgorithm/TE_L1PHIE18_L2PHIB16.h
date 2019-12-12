#ifndef TE_L1PHIE18_L2PHIB16_H
#define TE_L1PHIE18_L2PHIB16_H

#include "TrackletEngine.h"

void TE_L1PHIE18_L2PHIB16(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair);

#endif
