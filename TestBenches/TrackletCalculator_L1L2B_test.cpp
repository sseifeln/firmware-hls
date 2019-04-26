// Test bench for TrackletCalculator
#include "TrackletCalculator.hh"

#include <algorithm>
#include <iterator>
#include <cstring>

#include "FileReadUtility.hh"
#include "Constants.hh"

const int nevents = 100;  //number of events to run

using namespace std;

int main(int argc, char *argv[])
{
  // toggle truncation
  bool truncate = argc < 2 || strcmp(argv[1], "0");
  cout << "Truncation: " << (truncate ? "ON" : "OFF") << endl;

  // error counts
  int err = 0;

  ///////////////////////////
  // input memories
  static AllStubMemory<BARRELPS> innerStubs[2];
  static AllStubMemory<BARRELPS> outerStubs[1];
  static StubPairMemory stubPairs[13];

  // output memories
  static TrackletParameterMemory tpar;
  static TrackletProjectionMemory<BARRELPS> tproj_L3PHIA;
  static TrackletProjectionMemory<BARRELPS> tproj_L3PHIB;
  static TrackletProjectionMemory<BARREL2S> tproj_L4PHIA;
  static TrackletProjectionMemory<BARREL2S> tproj_L4PHIB;
  static TrackletProjectionMemory<BARREL2S> tproj_L5PHIA;
  static TrackletProjectionMemory<BARREL2S> tproj_L5PHIB;
  static TrackletProjectionMemory<BARREL2S> tproj_L6PHIA;
  static TrackletProjectionMemory<BARREL2S> tproj_L6PHIB;
  static TrackletProjectionMemory<DISK> tproj_D1PHIA;
  static TrackletProjectionMemory<DISK> tproj_D1PHIB;
  static TrackletProjectionMemory<DISK> tproj_D2PHIA;
  static TrackletProjectionMemory<DISK> tproj_D2PHIB;
  static TrackletProjectionMemory<DISK> tproj_D3PHIA;
  static TrackletProjectionMemory<DISK> tproj_D3PHIB;
  static TrackletProjectionMemory<DISK> tproj_D4PHIA;
  static TrackletProjectionMemory<DISK> tproj_D4PHIB;


  ///////////////////////////
  // open input files
  cout << "Open files..." << endl;

  const string dir = (truncate ? "TC_L1L2B_truncated108" : "TC_L1L2B");

  ifstream fin_innerStubs0;
  if (not openDataFile(fin_innerStubs0, dir + "/AllStubs_AS_L1PHIAn2_04.dat")) return -1;

  ifstream fin_innerStubs1;
  if (not openDataFile(fin_innerStubs1, dir + "/AllStubs_AS_L1PHIBn1_04.dat")) return -1;

  ifstream fin_outerStubs0;
  if (not openDataFile(fin_outerStubs0, dir + "/AllStubs_AS_L2PHIAn2_04.dat")) return -1;

  ifstream fin_stubPairs0;
  if (not openDataFile(fin_stubPairs0, dir + "/StubPairs_SP_L1PHIA4_L2PHIA3_04.dat")) return -1;

  ifstream fin_stubPairs1;
  if (not openDataFile(fin_stubPairs1, dir + "/StubPairs_SP_L1PHIA4_L2PHIA4_04.dat")) return -1;

  ifstream fin_stubPairs2;
  if (not openDataFile(fin_stubPairs2, dir + "/StubPairs_SP_L1PHIA4_L2PHIA5_04.dat")) return -1;

  ifstream fin_stubPairs3;
  if (not openDataFile(fin_stubPairs3, dir + "/StubPairs_SP_L1PHIA4_L2PHIA6_04.dat")) return -1;

  ifstream fin_stubPairs4;
  if (not openDataFile(fin_stubPairs4, dir + "/StubPairs_SP_L1PHIB5_L2PHIA3_04.dat")) return -1;

  ifstream fin_stubPairs5;
  if (not openDataFile(fin_stubPairs5, dir + "/StubPairs_SP_L1PHIB5_L2PHIA4_04.dat")) return -1;

  ifstream fin_stubPairs6;
  if (not openDataFile(fin_stubPairs6, dir + "/StubPairs_SP_L1PHIB5_L2PHIA5_04.dat")) return -1;

  ifstream fin_stubPairs7;
  if (not openDataFile(fin_stubPairs7, dir + "/StubPairs_SP_L1PHIB5_L2PHIA6_04.dat")) return -1;

  ifstream fin_stubPairs8;
  if (not openDataFile(fin_stubPairs8, dir + "/StubPairs_SP_L1PHIB5_L2PHIA7_04.dat")) return -1;

  ifstream fin_stubPairs9;
  if (not openDataFile(fin_stubPairs9, dir + "/StubPairs_SP_L1PHIB6_L2PHIA4_04.dat")) return -1;

  ifstream fin_stubPairs10;
  if (not openDataFile(fin_stubPairs10, dir + "/StubPairs_SP_L1PHIB6_L2PHIA5_04.dat")) return -1;

  ifstream fin_stubPairs11;
  if (not openDataFile(fin_stubPairs11, dir + "/StubPairs_SP_L1PHIB6_L2PHIA6_04.dat")) return -1;

  ifstream fin_stubPairs12;
  if (not openDataFile(fin_stubPairs12, dir + "/StubPairs_SP_L1PHIB6_L2PHIA7_04.dat")) return -1;


  ///////////////////////////
  // open output files
  ifstream fout_tpar;
  if (not openDataFile(fout_tpar, dir + "/TrackletParameters_TPAR_L1L2B_04.dat")) return -1;

  ifstream fout_tproj0;
  if (not openDataFile(fout_tproj0, dir + "/TrackletProjections_TPROJ_L1L2B_L3PHIA_04.dat")) return -1;

  ifstream fout_tproj1;
  if (not openDataFile(fout_tproj1, dir + "/TrackletProjections_TPROJ_L1L2B_L3PHIB_04.dat")) return -1;

  ifstream fout_tproj2;
  if (not openDataFile(fout_tproj2, dir + "/TrackletProjections_TPROJ_L1L2B_L4PHIA_04.dat")) return -1;

  ifstream fout_tproj3;
  if (not openDataFile(fout_tproj3, dir + "/TrackletProjections_TPROJ_L1L2B_L4PHIB_04.dat")) return -1;

  ifstream fout_tproj4;
  if (not openDataFile(fout_tproj4, dir + "/TrackletProjections_TPROJ_L1L2B_L5PHIA_04.dat")) return -1;

  ifstream fout_tproj5;
  if (not openDataFile(fout_tproj5, dir + "/TrackletProjections_TPROJ_L1L2B_L5PHIB_04.dat")) return -1;

  ifstream fout_tproj6;
  if (not openDataFile(fout_tproj6, dir + "/TrackletProjections_TPROJ_L1L2B_L6PHIA_04.dat")) return -1;

  ifstream fout_tproj7;
  if (not openDataFile(fout_tproj7, dir + "/TrackletProjections_TPROJ_L1L2B_L6PHIB_04.dat")) return -1;

  ifstream fout_tproj8;
  if (not openDataFile(fout_tproj8, dir + "/TrackletProjections_TPROJ_L1L2B_D1PHIA_04.dat")) return -1;

  ifstream fout_tproj9;
  if (not openDataFile(fout_tproj9, dir + "/TrackletProjections_TPROJ_L1L2B_D1PHIB_04.dat")) return -1;

  ifstream fout_tproj10;
  if (not openDataFile(fout_tproj10, dir + "/TrackletProjections_TPROJ_L1L2B_D2PHIA_04.dat")) return -1;

  ifstream fout_tproj11;
  if (not openDataFile(fout_tproj11, dir + "/TrackletProjections_TPROJ_L1L2B_D2PHIB_04.dat")) return -1;

  ifstream fout_tproj12;
  if (not openDataFile(fout_tproj12, dir + "/TrackletProjections_TPROJ_L1L2B_D3PHIA_04.dat")) return -1;

  ifstream fout_tproj13;
  if (not openDataFile(fout_tproj13, dir + "/TrackletProjections_TPROJ_L1L2B_D3PHIB_04.dat")) return -1;

  ifstream fout_tproj14;
  if (not openDataFile(fout_tproj14, dir + "/TrackletProjections_TPROJ_L1L2B_D4PHIA_04.dat")) return -1;

  ifstream fout_tproj15;
  if (not openDataFile(fout_tproj15, dir + "/TrackletProjections_TPROJ_L1L2B_D4PHIB_04.dat")) return -1;

  ///////////////////////////
  // loop over events
  cout << "Start event loop ..." << endl;
  for (unsigned int ievt = 0; ievt < nevents; ++ievt) {
    cout << "Event: " << dec << ievt << endl;

    // read event and write to memories
    writeMemFromFile<AllStubMemory<BARRELPS> >(innerStubs[0], fin_innerStubs0, ievt);
    writeMemFromFile<AllStubMemory<BARRELPS> >(innerStubs[1], fin_innerStubs1, ievt);
    writeMemFromFile<AllStubMemory<BARRELPS> >(outerStubs[0], fin_outerStubs0, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[0], fin_stubPairs0, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[1], fin_stubPairs1, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[2], fin_stubPairs2, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[3], fin_stubPairs3, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[4], fin_stubPairs4, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[5], fin_stubPairs5, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[6], fin_stubPairs6, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[7], fin_stubPairs7, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[8], fin_stubPairs8, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[9], fin_stubPairs9, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[10], fin_stubPairs10, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[11], fin_stubPairs11, ievt);
    writeMemFromFile<StubPairMemory>(stubPairs[12], fin_stubPairs12, ievt);

    // bx
    BXType bx = ievt;

    const uint8_t NASMemInner = 2;
    const uint8_t NASMemOuter = 1;
    const uint8_t NSPMem = 13;

    // This is the maximum number of stub pairs, if every stub pair memory is
    // filled to capacity.
    const uint16_t N = NSPMem * (1 << kNBits_MemAddr);

    void (*TC) (
        const BXType,
        const AllStubMemory<BARRELPS> [],
        const AllStubMemory<BARRELPS> [],
        const StubPairMemory [],
        TrackletParameterMemory * const,
        TrackletProjectionMemory<BARRELPS> * const,
        TrackletProjectionMemory<BARRELPS> * const,
        TrackletProjectionMemory<BARRELPS> * const,
        TrackletProjectionMemory<BARRELPS> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<BARREL2S> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const,
        TrackletProjectionMemory<DISK> * const
    );

    // The last parameter is how many stub pairs to process.
    if (truncate)
      TC = &TrackletCalculator_L1L2B;
    else
      TC = &TrackletCalculator_L1L2<TC::B, true, NASMemInner, NASMemOuter, NSPMem, 0x1FF0, 0x0000, 0x33333333, N>;

    // Unit Under Test
    TC(bx, innerStubs, outerStubs, stubPairs,
       &tpar,
       &tproj_L3PHIA,
       &tproj_L3PHIB,
       NULL,
       NULL,
       &tproj_L4PHIA,
       &tproj_L4PHIB,
       NULL,
       NULL,
       &tproj_L5PHIA,
       &tproj_L5PHIB,
       NULL,
       NULL,
       &tproj_L6PHIA,
       &tproj_L6PHIB,
       NULL,
       NULL,
       &tproj_D1PHIA,
       &tproj_D1PHIB,
       NULL,
       NULL,
       &tproj_D2PHIA,
       &tproj_D2PHIB,
       NULL,
       NULL,
       &tproj_D3PHIA,
       &tproj_D3PHIB,
       NULL,
       NULL,
       &tproj_D4PHIA,
       &tproj_D4PHIB,
       NULL,
       NULL
    );


    // compare the computed outputs with the expected ones
    err += compareMemWithFile<TrackletParameterMemory>(tpar, fout_tpar, ievt,
                                                   "\nTrackletParameter", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARRELPS> >(tproj_L3PHIA, fout_tproj0, ievt,
                                                   "\nTrackletProjection (L3PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARRELPS> >(tproj_L3PHIB, fout_tproj1, ievt,
                                                   "\nTrackletProjection (L3PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L4PHIA, fout_tproj2, ievt,
                                                   "\nTrackletProjection (L4PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L4PHIB, fout_tproj3, ievt,
                                                   "\nTrackletProjection (L4PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L5PHIA, fout_tproj4, ievt,
                                                   "\nTrackletProjection (L5PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L5PHIB, fout_tproj5, ievt,
                                                   "\nTrackletProjection (L5PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L6PHIA, fout_tproj6, ievt,
                                                   "\nTrackletProjection (L6PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<BARREL2S> >(tproj_L6PHIB, fout_tproj7, ievt,
                                                   "\nTrackletProjection (L6PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D1PHIA, fout_tproj8, ievt,
                                                   "\nTrackletProjection (D1PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D1PHIB, fout_tproj9, ievt,
                                                   "\nTrackletProjection (D1PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D2PHIA, fout_tproj10, ievt,
                                                   "\nTrackletProjection (D2PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D2PHIB, fout_tproj11, ievt,
                                                   "\nTrackletProjection (D2PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D3PHIA, fout_tproj12, ievt,
                                                   "\nTrackletProjection (D3PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D3PHIB, fout_tproj13, ievt,
                                                   "\nTrackletProjection (D3PHIB)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D4PHIA, fout_tproj14, ievt,
                                                   "\nTrackletProjection (D4PHIA)", truncate);
    err += compareMemWithFile<TrackletProjectionMemory<DISK> >(tproj_D4PHIB, fout_tproj15, ievt,
                                                   "\nTrackletProjection (D4PHIB)", truncate);
    cout << endl;

  } // end of event loop

  return err;

}
