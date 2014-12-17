/*
fill a couple of plots when running on two samples
-------------------

c++ -o test_06 `root-config --glibs --cflags` -lm test_06.cpp readTree.cc plotter.cc 
./test_06

*/

#include <iostream>
#include <map>

#include "TChain.h"

#include "readTree.h"
#include "plotter.h"

using namespace std ;



void fillHistos (plotter & analysisPlots, readTree & reader, const string sampleName)
{
  cout << "reading sample " << sampleName << "\n" ;
  // loop over events
  for (int iEvent = 0 ; iEvent < reader.fChain->GetEntries () ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      analysisPlots.fillHisto (sampleName, "total", "mjj", reader.mjj, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "ptj1", reader.jetpt_puppi1, 1.) ;

      if (reader.mjj < 350) continue ;
      analysisPlots.fillHisto (sampleName, "mjj", "mjj", reader.mjj, 1.) ;
      analysisPlots.fillHisto (sampleName, "mjj", "ptj1", reader.jetpt_puppi1, 1.) ;

    } // loop over events
  analysisPlots.setPoissonErrors () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  float lumi = 20. ;
  plotter analysisPlots (lumi) ;
   
  // ewk sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j = new TChain ("easyDelphes") ;
  sample_EWK_WW2j->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root") ;
  int totEvents_EWK_WW2j = sample_EWK_WW2j->GetEntries () ;

  readTree reader_EWK_WW2j (sample_EWK_WW2j) ;

  analysisPlots.addSample ("EWK_WW2j", 4.13649215685881443E-003, totEvents_EWK_WW2j, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "mjj", 50, 0., 10000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "ptj1", 100, 0., 1000.) ; 
  analysisPlots.copyLayerInSample ("EWK_WW2j", "mjj", "total") ;

  fillHistos (analysisPlots, reader_EWK_WW2j, "EWK_WW2j") ;

  // qcd sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j = new TChain ("easyDelphes") ;
  sample_QCD_WW2j->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_QCD_DF_126.root") ;
  int totEvents_QCD_WW2j = sample_QCD_WW2j->GetEntries () ;

  readTree reader_QCD_WW2j (sample_QCD_WW2j) ;

  analysisPlots.copySampleStructure ("QCD_WW2j", "EWK_WW2j", 1.06691296353271774E-003, totEvents_QCD_WW2j, 0, 8) ;

  fillHistos (analysisPlots, reader_QCD_WW2j, "QCD_WW2j") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  analysisPlots.plotSingleLayer ("total", "mjj", "mjj (GeV)", "#sigma #times lumi") ;
  analysisPlots.plotSingleLayer ("total", "mjj", "mjj (GeV)", "#sigma #times lumi", 1) ;
  analysisPlots.compareStoB ("total", "mjj", "mjj (GeV)", "#sigma #times lumi") ;

  return 0 ;
}  
