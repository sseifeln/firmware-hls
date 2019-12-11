#include "TrackletCalculator.hh"

// Wrapper function for TC_L1L2E, since Vivado HLS apparently does not like
// template parameters in the top function.
void TrackletCalculator_L1L2E(
    const BXType bx,
    const AllStubMemory<BARRELPS> innerStubs[2],
    const AllStubMemory<BARRELPS> outerStubs[1],
    const StubPairMemory stubPairs[13],
    TrackletParameterMemory * trackletParameters,
    TrackletProjectionMemory<BARRELPS> * projout_L3PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHIA,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHIA,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHIA,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHIC,
    TrackletProjectionMemory<DISK> * projout_D1PHIA,
    TrackletProjectionMemory<DISK> * projout_D1PHIB,
    TrackletProjectionMemory<DISK> * projout_D1PHIC,
    TrackletProjectionMemory<DISK> * projout_D2PHIA,
    TrackletProjectionMemory<DISK> * projout_D2PHIB,
    TrackletProjectionMemory<DISK> * projout_D2PHIC,
    TrackletProjectionMemory<DISK> * projout_D3PHIA,
    TrackletProjectionMemory<DISK> * projout_D3PHIB,
    TrackletProjectionMemory<DISK> * projout_D3PHIC,
    TrackletProjectionMemory<DISK> * projout_D4PHIA,
    TrackletProjectionMemory<DISK> * projout_D4PHIB,
    TrackletProjectionMemory<DISK> * projout_D4PHIC
) {
  TC_L1L2E: TrackletCalculator_L1L2<TC::E, 2, 1, 13, 0x1FE0, 0x0000, 0x77777772, kMaxProc>(
    bx,
    innerStubs,
    outerStubs,
    stubPairs,
    trackletParameters,
    NULL,
    projout_L3PHIB,
    NULL,
    NULL,
    projout_L4PHIA,
    projout_L4PHIB,
    projout_L4PHIC,
    NULL,
    projout_L5PHIA,
    projout_L5PHIB,
    projout_L5PHIC,
    NULL,
    projout_L6PHIA,
    projout_L6PHIB,
    projout_L6PHIC,
    NULL,
    projout_D1PHIA,
    projout_D1PHIB,
    projout_D1PHIC,
    NULL,
    projout_D2PHIA,
    projout_D2PHIB,
    projout_D2PHIC,
    NULL,
    projout_D3PHIA,
    projout_D3PHIB,
    projout_D3PHIC,
    NULL,
    projout_D4PHIA,
    projout_D4PHIB,
    projout_D4PHIC,
    NULL
  );
}

// Wrapper function for TC_L1L2G, since Vivado HLS apparently does not like
// template parameters in the top function.
void TrackletCalculator_L1L2G(
    const BXType bx,
    const AllStubMemory<BARRELPS> innerStubs[1],
    const AllStubMemory<BARRELPS> outerStubs[2],
    const StubPairMemory stubPairs[13],
    TrackletParameterMemory * trackletParameters,
    TrackletProjectionMemory<BARRELPS> * projout_L3PHIB,
    TrackletProjectionMemory<BARRELPS> * projout_L3PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L4PHID,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L5PHID,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHIB,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHIC,
    TrackletProjectionMemory<BARREL2S> * projout_L6PHID,
    TrackletProjectionMemory<DISK> * projout_D1PHIB,
    TrackletProjectionMemory<DISK> * projout_D1PHIC,
    TrackletProjectionMemory<DISK> * projout_D1PHID,
    TrackletProjectionMemory<DISK> * projout_D2PHIB,
    TrackletProjectionMemory<DISK> * projout_D2PHIC,
    TrackletProjectionMemory<DISK> * projout_D2PHID,
    TrackletProjectionMemory<DISK> * projout_D3PHIB,
    TrackletProjectionMemory<DISK> * projout_D3PHIC,
    TrackletProjectionMemory<DISK> * projout_D3PHID,
    TrackletProjectionMemory<DISK> * projout_D4PHIB,
    TrackletProjectionMemory<DISK> * projout_D4PHIC,
    TrackletProjectionMemory<DISK> * projout_D4PHID
) {
#pragma HLS inline recursive
#pragma HLS resource variable=innerStubs.dataarray_ latency=2
#pragma HLS resource variable=outerStubs.dataarray_ latency=2
#pragma HLS resource variable=stubPairs.dataarray_ latency=2

  TC_L1L2G: TrackletCalculator_L1L2<TC::G, 1, 2, 13, 0x0000, 0x1FEE, 0xEEEEEEE6, kMaxProc>(
    bx,
    innerStubs,
    outerStubs,
    stubPairs,
    trackletParameters,
    NULL,
    projout_L3PHIB,
    projout_L3PHIC,
    NULL,
    NULL,
    projout_L4PHIB,
    projout_L4PHIC,
    projout_L4PHID,
    NULL,
    projout_L5PHIB,
    projout_L5PHIC,
    projout_L5PHID,
    NULL,
    projout_L6PHIB,
    projout_L6PHIC,
    projout_L6PHID,
    NULL,
    projout_D1PHIB,
    projout_D1PHIC,
    projout_D1PHID,
    NULL,
    projout_D2PHIB,
    projout_D2PHIC,
    projout_D2PHID,
    NULL,
    projout_D3PHIB,
    projout_D3PHIC,
    projout_D3PHID,
    NULL,
    projout_D4PHIB,
    projout_D4PHIC,
    projout_D4PHID
  );
}
