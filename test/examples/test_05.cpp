/*
fill a couple of plots when running on a single sample
-------------------

c++ -o test_05 `root-config --glibs --cflags` -lm test_05.cpp readTree.cc plotter.cc 
./test_05 ~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root

*/

#include <iostream>
#include <map>

#include "TChain.h"

#include "readTree.h"
#include "plotter.h"

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

  float lumi = 20. ;
  plotter analysisPlots (lumi) ;
  analysisPlots.addSample ("EWK_WW2j", 4.13649215685881443E-003, totEvents, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "mjj", 200, 0., 2000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "ptj1", 100, 0., 1000.) ; 
  analysisPlots.copyLayerInSample ("EWK_WW2j", "mjj", "total") ;

  // read a sample and fill in the plots

  // loop over events
  for (int iEvent = 0 ; iEvent < totEvents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      analysisPlots.fillHisto ("EWK_WW2j", "total", "mjj", reader.mjj, 1.) ;
      analysisPlots.fillHisto ("EWK_WW2j", "total", "ptj1", reader.jetpt_puppi1, 1.) ;

      if (reader.mjj < 350) continue ;
      analysisPlots.fillHisto ("EWK_WW2j", "mjj", "mjj", reader.mjj, 1.) ;
      analysisPlots.fillHisto ("EWK_WW2j", "mjj", "ptj1", reader.jetpt_puppi1, 1.) ;

    } // loop over events

  analysisPlots.printStructure () ;
  analysisPlots.setPoissonErrors () ;
  analysisPlots.plotSingleSample ("EWK_WW2j", "mjj", "ptj1", "leading jet pT (GeV)", "#sigma #times lumi") ;


  return 0 ;
}  
