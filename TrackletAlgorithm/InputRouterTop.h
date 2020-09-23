#ifndef TrackletAlgorithm_InputRouterTop_h
#define TrackletAlgorithm_InputRouterTop_h

#include "InputRouter.h"

void InputRouterTop( const BXType hBx
	, const ap_uint<6> hLinkId 
	, const ap_uint<kLINKMAPwidth> *hLinkTable 
	, const int *kPhiCorrtable_L1
	, const int *kPhiCorrtable_L2
	, const int *kPhiCorrtable_L3
	, const int *kPhiCorrtable_L4
	, const int *kPhiCorrtable_L5
	, const int *kPhiCorrtable_L6
	, ap_uint<kNBits_DTC> *hStubs
	, DTCStubMemory *hMemories);

#endif


