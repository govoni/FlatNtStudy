/*
check several variables to spot some sensitive to the H / noH difference
-------------------

c++ -o test_08 `root-config --glibs --cflags` -lm test_08.cpp readTree.cc plotter.cc 
./test_08

*/

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"

#include "readTree.h"
#include "plotter.h"

using namespace std ;


double 
deltaPhi (double phi1, double phi2)
{
  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillHistos (plotter & analysisPlots, readTree & reader, const string sampleName)
{
  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader.fChain->GetEntries () ;
//  maxevents = 10 ;
  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      TLorentzVector L_leadi_lepton ;
      L_leadi_lepton.SetPtEtaPhiM (reader.pt1, reader.eta1, reader.phi1, 0.) ;

      TLorentzVector L_trail_lepton ;
      L_trail_lepton.SetPtEtaPhiM (reader.pt2, reader.eta2, reader.phi2, 0.) ;

      TLorentzVector L_dilepton = L_leadi_lepton + L_trail_lepton ;

      TLorentzVector L_met ;
      L_met.SetPtEtaPhiM (reader.pfmet, 0., reader.pfmetphi, 0.) ;

      TLorentzVector L_leadi_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader.jetpt1, reader.jeteta1, reader.jetphi1, 0.) ;

      TLorentzVector L_trail_jet ;
      L_trail_jet.SetPtEtaPhiM (reader.jetpt2, reader.jeteta2, reader.jetphi2, 0.) ;

      TLorentzVector L_dijet = L_leadi_jet + L_trail_jet ;

      float asimJ = (L_leadi_jet.Pt () - L_trail_jet.Pt ()) / (L_leadi_jet.Pt () + L_trail_jet.Pt ()) ;
      float asimL = (L_leadi_lepton.Pt () - L_trail_lepton.Pt ()) / (L_leadi_lepton.Pt () + L_trail_lepton.Pt ()) ;
      float Rvar = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_jet.Pt () * L_trail_jet.Pt ()) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLL", fabs (L_leadi_lepton.DeltaPhi (L_trail_lepton)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLMet", fabs (L_dilepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLLMet", fabs (L_leadi_lepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiTLMet", fabs (L_trail_lepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiJJ", fabs (L_leadi_jet.DeltaPhi (L_trail_jet)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiJJMet", fabs (L_dijet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLJMet", fabs (L_leadi_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiTJMet", fabs (L_trail_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiTL", fabs (L_dijet.DeltaPhi (L_dilepton)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "mjj", L_dijet.M (), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "mll", L_dilepton.M (), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "asimJ", asimJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "asimL", asimL, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "R", Rvar, 1.) ;

      // pT Ws per distinguere segnale da fondo QCD

    } // loop over events
  analysisPlots.setPoissonErrors () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  float lumi = 3000 ; // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)
  plotter analysisPlots (lumi) ;
   
  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root") ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries () ;

  readTree reader_EWK_WW2j_126 (sample_EWK_WW2j_126) ;

  analysisPlots.addSample ("EWK_WW2j_126", 4.13649215685881443E-003/*pb*/, totEvents_EWK_WW2j_126, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j_126", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLL",    10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLMet",  10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLLMet", 10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTLMet", 10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJ",    10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJMet", 10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLJMet", 10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTJMet", 10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTL",    10, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mjj",       20, 0., 10000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mll",       20, 0., 1500.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimJ",     20, 0., 1.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimL",     20, 0., 1.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R",         20, 0., 15.) ; 

  fillHistos (analysisPlots, reader_EWK_WW2j_126, "EWK_WW2j_126") ;

  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_noH->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_noH.root") ;
  int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;

  readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;

  analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", 4.49200073018412010E-003/*pb*/, totEvents_EWK_WW2j_noH, 1, 38) ;

  fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH") ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_QCD_DF_126.root") ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;

  analysisPlots.copySampleStructure ("QCD_WW2j_126", "EWK_WW2j_126", 1.06691296353271774E-003/*pb*/, totEvents_QCD_WW2j_126, 0, 8) ;

  fillHistos (analysisPlots, reader_QCD_WW2j_126, "QCD_WW2j_126") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  analysisPlots.plotRelativeExcessFullLayer ("total") ;

  return 0 ;
}  
