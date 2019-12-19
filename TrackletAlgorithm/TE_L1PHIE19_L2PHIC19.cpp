#include "TE_L1PHIE19_L2PHIC19.h"

void TE_11(const BXType bx,
                       const VMStubTEInnerMemory<BARRELPS>& instubinnerdata,
                       const VMStubTEOuterMemory<BARRELPS>& instubouterdata,
                       StubPairMemory& outstubpair)
{
#pragma HLS resource variable=instubinnerdata.dataarray_ latency=2
#pragma HLS resource variable=instubouterdata.dataarray_ latency=2
  TrackletEngine<TE::L1L2, TE::E19, TE::C19, BARRELPS, BARRELPS, 32, 256, 256>
			(bx, instubinnerdata, instubouterdata, outstubpair);
}
