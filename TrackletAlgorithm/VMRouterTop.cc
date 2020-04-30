#include "VMRouterTop.h"

// VMRouter Top Function for layer 1, AllStub region E

void VMRouterTop(BXType bx, const InputStubMemory<BARRELPS> *i0,
		const InputStubMemory<BARRELPS> *i1,
		const InputStubMemory<BARRELPS> *i2,
		const InputStubMemory<BARRELPS> *i3,
		const InputStubMemory<BARRELPS> *i4,
		const InputStubMemory<BARRELPS> *i5,
//		const InputStubMemory<BARRELPS> *i6,
//		const InputStubMemory<BARRELPS> *i7,
		AllStubMemory<BARRELPS> *allStub,
		// ME memories
		VMStubMEMemory<BARRELPS> *m0,
		VMStubMEMemory<BARRELPS> *m1,
		VMStubMEMemory<BARRELPS> *m2,
		VMStubMEMemory<BARRELPS> *m3,
		VMStubMEMemory<BARRELPS> *m4,
		VMStubMEMemory<BARRELPS> *m5,
		VMStubMEMemory<BARRELPS> *m6,
		VMStubMEMemory<BARRELPS> *m7,
		// TE Inner memories
		VMStubTEInnerMemory<BARRELPS> *mtei0,
		VMStubTEInnerMemory<BARRELPS> *mtei1,
		VMStubTEInnerMemory<BARRELPS> *mtei2,
		VMStubTEInnerMemory<BARRELPS> *mtei3,
		VMStubTEInnerMemory<BARRELPS> *mtei4,
		VMStubTEInnerMemory<BARRELPS> *mtei5,
		VMStubTEInnerMemory<BARRELPS> *mtei6,
		VMStubTEInnerMemory<BARRELPS> *mtei7,
		VMStubTEInnerMemory<BARRELOL> *mteol1,
		VMStubTEInnerMemory<BARRELOL> *mteol2) {

// Variables for that are specified with regards to the test bench, should be set somewhere else

// const uint32_t MEMask(0x000F0000UL); // Mask of which memories that are being used.
// const uint32_t TEIMask(0x000F0000UL); // Mask of which TE Inner memories that are used
// const uint16_t OLMask(0x300); // Mask of which TE Outer memories that are used
// const uint32_t TEOMask(0x0UL); // Mask of which TE Inner memories that are used
// Note that the masks are "reversed"
	const int layer(1); // Which barrel layer number the data is coming from, 0 if not barrel
	const int disk(0); // Which disk number the data is coming from, 0 if not disk
	static const ap_uint<6> imask(0xF); // Mask of which inputs that are being used
	static const ap_uint<32> memask(0x000F0000UL); // Mask of which memories that are being used.
	static const ap_uint<32> teimask(0x000F0000UL); // Mask of which TE Inner memories that are used
	static const ap_uint<16> olmask(0x300); // Mask of which TE Outer memories that are used
	static const ap_uint<32> teomask(0x0); // Mask of which TE Inner memories that are used

///////////////////////////
// Open Lookup tables

// lookup table - 2^nbinsfinbinetable entries actually filled
// Table is filled with numbers between 0 and 7 (and -1): the finer region each z/r bin is divided into.
	static const int finebintable[kMaxFineBinTable] =
#include "../emData/VMR/VMR_L1PHIE/VMR_L1PHIE_finebin.tab"
	;

	// Only used by layers. LUT with phi corrections for different r and bend
	static const int phicorrtable[] =
#include "../emData/VMR/VMPhiCorrL1.txt"
	;

	static const int binlookuptable[2048] = // 11 bits used for LUT
#include "../emData/VMR/VMR_L1PHIE/VMTableInnerL1L2.tab" // Only for Layer 1
					;


// Bendcut tables
static const int bendtablesize = 8; // The size of each vmbendcut table. Either 8 or 16.
static const int bendtable[][bendtablesize] = {
			#include "../emData/VMR/VMSTE_L1PHIE17n1_vmbendcut.tab" // Seems to be the same for all E regions
			,
			#include "../emData/VMR/VMSTE_L1PHIE18n1_vmbendcut.tab"
			,
			#include "../emData/VMR/VMSTE_L1PHIE19n1_vmbendcut.tab"
			,
			#include "../emData/VMR/VMSTE_L1PHIE20n1_vmbendcut.tab"
};

// Overlap LUT
	static const int overlaptable[1024] = // 10 bits used for LUT
#include "../emData/VMR/VMR_L1PHIE/VMTableInnerL1D1.tab" // Only for Layer 1
					;
// SHOULD I USE SOMETHING ELSE THAN INT FOR MY TABLES???

					VMRouter<BARRELPS, BARRELPS, layer, disk, bendtablesize>
					(bx, finebintable, phicorrtable, binlookuptable, bendtable, overlaptable,
					imask, i0, i1, i2, i3, nullptr, nullptr, //i5,i6,i7,
					allStub,
// ME memories
					memask, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 0-7
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 8-15
					m0, m1, m2, m3, nullptr, nullptr, nullptr, nullptr, // 16-23
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 24-31
// TEInner memories
					teimask, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 0-7
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 8-15
					mtei0, mtei1, mtei2, mtei3, nullptr, nullptr, nullptr, nullptr, // 16-23
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 24-31
// TEInner Overlap memories
					olmask, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 0-7
					mteol1, mteol2, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 8-15
// TEOuter memories
					teomask, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 0-7
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 8-15
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 16-23
					nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr // 24-31
					);
	return;
}
