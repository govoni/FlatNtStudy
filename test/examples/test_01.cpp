/*
test the readTree
-------------------
readClass is done in root as follows:
root ~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root 
easyDelphes->MakeClass ("readTree")
plus some fixes, i.e. removing duplicate definition of branches

c++ -o test_01 `root-config --glibs --cflags` -lm test_01.cpp readTree.cc 
*/

#include <iostream>
#include "readTree.h"
#include "TChain.h"

using namespace std ;

int main (int argc, char ** argv)
{
  if (argc < 2) exit (1) ;
  TChain * sample = new TChain ("easyDelphes") ;
  sample->Add (argv[1]) ;
  readTree reader (sample) ;
  
  int totEvents = sample->GetEntries () ;
  cout << "found " << totEvents 
       << " events in the sample " << argv[1]
       << "\n" ;
  totEvents = 10 ;
  for (int iEvent = 0 ; iEvent < totEvents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      cout << "event " << iEvent 
           << "\t| found " << reader.njet << " jets\n" ;
    }

  delete sample ;
  return 0 ;
}
