#include "TrackletEngineTop.h"

void TrackletEngineTop(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair)
{
  // !!! currently handles TE_L1PHIE18_L2PHIC17 only !!!
  TE_L1PHIE18_L2PHIC17: TrackletEngine<TE::L1L2, TE::E18, TE::C17, BARRELPS, BARRELPS, 32, 256, 256>
			(bx, instubinnerdata, instubouterdata, outstubpair);
}
