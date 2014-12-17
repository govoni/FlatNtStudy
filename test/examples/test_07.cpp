/*
fill a couple of plots when running on two samples
-------------------

c++ -o test_07 `root-config --glibs --cflags` -lm test_07.cpp readTree.cc plotter.cc 
./test_07

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
  float lumi = 20. ;
  plotter analysisPlots (lumi) ;
   
  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root") ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries () ;

  readTree reader_EWK_WW2j_126 (sample_EWK_WW2j_126) ;

  analysisPlots.addSample ("EWK_WW2j_126", 4.13649215685881443E-003, totEvents_EWK_WW2j_126, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j_126", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLL",    25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLMet",  25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLLMet", 25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTLMet", 25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJ",    25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJMet", 25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLJMet", 25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTJMet", 25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTL",    25, 0., 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mjj",       50, 0., 10000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mll",       50, 0., 1500.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimJ",     50, 0., 1.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimL",     50, 0., 1.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R",         50, 0., 5.) ; 

  fillHistos (analysisPlots, reader_EWK_WW2j_126, "EWK_WW2j_126") ;

  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_noH->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_noH.root") ;
  int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;

  readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;

  analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", 4.49200073018412010E-003, totEvents_EWK_WW2j_noH, 0, 38) ;

  fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  analysisPlots.plotFullLayer ("total") ;
  analysisPlots.compareStoBFullLayer ("total") ;

//  analysisPlots.compareStoB ("total", "dPhiLL",    "#Delta#phi (l,l)"           , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiLMet",  "#Delta#phi (ll,MET)"        , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiLLMet", "#Delta#phi (leading l,MET)" , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiTLMet", "#Delta#phi (trailing l,MET)", "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiJJ",    "#Delta#phi (j,j)"           , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiJJMet", "#Delta#phi (jj,MET)"        , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiLJMet", "#Delta#phi (leading j,MET)" , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiTJMet", "#Delta#phi (trailing j,MET)", "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "dPhiTL",    "#Delta#phi (ll,jj)"         , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "mjj",       "m_{jj} (GeV)"               , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "asimJ",     "rel p_T diff jets"          , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "asimL",     "rel p_T diff leptons"       , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "R",         "R variable"                 , "#sigma #times lumi") ;
//  analysisPlots.compareStoB ("total", "R",         "R variable"                 , "#sigma #times lumi", false, 1, 1) ;

  return 0 ;
}  
