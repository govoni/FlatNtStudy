/*
example of selections
-------------------

apply selections when looping over the entries, test the counter.h class

c++ -o test_02 `root-config --glibs --cflags` -lm test_02.cpp readTree.cc 
./test_02 ~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root 1000

*/

#include <iostream>
#include <map>

#include "readTree.h"
#include "TChain.h"

#include "counter.h"

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
       
  if (argc == 3 && atoi (argv[2]) > 0)
    {
      totEvents = atoi (argv[2]) ;
      cout << "looping on " << totEvents << " events only\n" ;
    }
  
  counter countEvents ;
  countEvents.addStage ("tot") ; 
  countEvents.addStage ("njets") ; 
  countEvents.addStage ("nleps") ;
  countEvents.addStage ("nbjets") ;
       
  for (int iEvent = 0 ; iEvent < totEvents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 
      countEvents.increase ("tot") ;

      if (reader.njet < 2) continue ;
      countEvents.increase ("njets") ;

      if (reader.nextra > 0) continue ; //PG FIXME is this the number of extra leptons?
      countEvents.increase ("nleps") ;

      if (reader.nbjet > 0) continue ;
      countEvents.increase ("nbjets") ;

    }

  countEvents.printSummary () ;
  
  delete sample ;
  return 0 ;
}
