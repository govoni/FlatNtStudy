/*
put in place MVA to train a rectangular separator
-------------------

c++ -o test_10 `root-config --glibs --cflags` \
     -lTMVA -L/Users/govoni/private/job/learn/root/TMVA/TMVA/lib \
     -lm test_10.cpp readTree.cc plotter.cc 
./test_10

useful links:

https://aholzner.wordpress.com/2011/08/27/a-tmva-example-in-pyroot/

*/

#include <iostream>
#include <map>

#include "TChain.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TNtuple.h"
#include "TMVA/Factory.h"

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


void fillNtuple (TNtuple * ntuple, readTree & reader, float weight = 1.)
{
  int maxevents = reader.fChain->GetEntries () ;
//  maxevents = 10 ;
  // loop over events
   float mjj = -99. ;
   float dEtajj = -99. ;
   float mll = -99. ;
   float dPhill = -99. ;
   float asL = -99. ;
   float asJ = -99. ;
   float dPhijjMET = -99. ;
   float dPhiLeadJMET = -99. ;
   float R = -99. ;

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

      mjj = L_dijet.M () ;
      dEtajj = fabs (reader.jeteta1 - reader.jeteta2) ;
      mll = L_dilepton.M () ;
      dPhill = fabs (L_leadi_lepton.DeltaPhi (L_trail_lepton)) ;
      asL = (L_leadi_lepton.Pt () - L_trail_lepton.Pt ()) / (L_leadi_lepton.Pt () + L_trail_lepton.Pt ()) ;
      asJ = (L_leadi_jet.Pt () - L_trail_jet.Pt ()) / (L_leadi_jet.Pt () + L_trail_jet.Pt ()) ;
      dPhijjMET = fabs (L_dijet.DeltaPhi (L_met)) ;
      dPhiLeadJMET = fabs (L_leadi_jet.DeltaPhi (L_met)) ;
      R = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_jet.Pt () * L_trail_jet.Pt ()) ;

      ntuple->Fill (mjj,dEtajj,mll,dPhill,asL,asJ,dPhijjMET,dPhiLeadJMET,R,weight) ;

    } // loop over events
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  
  float XS_EWK_WW2j_126 = 4.13649215685881443E-003/*pb*/ ;
  float XS_EWK_WW2j_noH = 4.49200073018412010E-003/*pb*/ ;
  float XS_QCD_WW2j_126 = 1.06691296353271774E-003/*pb*/ ;
  
  string varSet = "mjj:dEtajj:mll:dPhill:asL:asJ:dPhijjMET:dPhiLeadJMET:R:weight" ;
  
  float lumi = 3000 ; // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root") ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries () ;

  readTree reader_EWK_WW2j_126 (sample_EWK_WW2j_126) ;

  TNtuple * nt_EWK_WW2j_126 = new TNtuple ("NT", "nt_EWK_WW2j_126", varSet.c_str ()) ;
  fillNtuple (nt_EWK_WW2j_126, reader_EWK_WW2j_126, XS_EWK_WW2j_126 * lumi / totEvents_EWK_WW2j_126) ;

  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_noH->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_noH.root") ;
  int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;

  readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;

  TNtuple * nt_EWK_WW2j_noH = new TNtuple ("NT", "nt_EWK_WW2j_noH", varSet.c_str ()) ;
  fillNtuple (nt_EWK_WW2j_noH, reader_EWK_WW2j_noH, XS_EWK_WW2j_noH * lumi / totEvents_EWK_WW2j_noH) ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ("~/data/TP/ntuples/2014-09-22/TP_SS_QCD_DF_126.root") ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;

  TNtuple * nt_QCD_WW2j_126 = new TNtuple ("NT", "nt_QCD_WW2j_126", varSet.c_str ()) ;
  fillNtuple (nt_QCD_WW2j_126, reader_QCD_WW2j_126, XS_QCD_WW2j_126 * lumi / totEvents_QCD_WW2j_126) ;

  // book the TMVA factory
  // ---- ---- ---- ---- ---- ---- ----
  
  TFile * outputfile = TFile::Open ("outputTMVA.root", "RECREATE") ;
  TMVA::Factory * TMVAfactory = new TMVA::Factory ("TMVAfactory", outputfile) ;

  TNtuple * nt_sig = nt_EWK_WW2j_126 ;
  TNtuple * nt_bkg = nt_QCD_WW2j_126 ;

  TMVAfactory->AddSignalTree (nt_sig, 1.) ;
  TMVAfactory->AddBackgroundTree (nt_bkg, 1.) ;  

  TMVAfactory->AddVariable ("mjj"         , 'F') ;
  TMVAfactory->AddVariable ("dEtajj"      , 'F') ;
  TMVAfactory->AddVariable ("mll"         , 'F') ;
  TMVAfactory->AddVariable ("dPhill"      , 'F') ;
//  TMVAfactory->AddVariable ("asL"         , 'F') ;
//  TMVAfactory->AddVariable ("asJ"         , 'F') ;
  TMVAfactory->AddVariable ("dPhijjMET"   , 'F') ;
//  TMVAfactory->AddVariable ("dPhiLeadJMET", 'F') ;
  TMVAfactory->AddVariable ("R"           , 'F') ;

  TMVAfactory->SetWeightExpression ("weight") ;

  int sigNumTrain = nt_sig->GetEntries () * 0.5 ;
  int bkgNumTrain = nt_bkg->GetEntries () * 0.5 ;
  int sigNumTest  = nt_sig->GetEntries () - sigNumTrain ;
  int bkgNumTest  = nt_bkg->GetEntries () - bkgNumTrain ;

  char trainOptions[120] ;
  sprintf (trainOptions, "nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random",
           sigNumTrain, bkgNumTrain,
           sigNumTest, bkgNumTest) ;
  cout << "\nTRAINING CONFIGURATION : " << trainOptions << endl ;

  TMVAfactory->PrepareTrainingAndTestTree ("", trainOptions) ;

  // normalisation, decorrelation, principal component analysis, Gaussianisation,
  // Norm, Deco, PCA, Uniform, Gauss, 
  // N, D, P, U , G.
//  TMVAfactory->BookMethod (TMVA::Types::kCuts, "cuts", "H:V:VarTransform=Norm_Signal,Uniform_Signal,Gauss_Signal:FitMethod=SA") ;

//  TMVAfactory->BookMethod (TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10") ;
  TMVAfactory->BookMethod (TMVA::Types::kFisher, "Fisher", "H:!V") ;
  TMVAfactory->BookMethod (TMVA::Types::kBDT, "BDT", "H:!V") ;

  TMVAfactory->TrainAllMethods () ;
  TMVAfactory->TestAllMethods () ;
  TMVAfactory->EvaluateAllMethods () ;

  return 0 ;
}  
