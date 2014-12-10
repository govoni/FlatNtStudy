/*
cjv plots
-------------------
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


bool closeToLeptons (float eta, float phi, readTree & reader, float R = 0.3)
{
  if ((eta - reader.eta1) * (eta - reader.eta1) +
      deltaPhi (phi, reader.phi1) * deltaPhi (phi, reader.phi1) < R * R) return true ;
  if ((eta - reader.eta2) * (eta - reader.eta2) +
      deltaPhi (phi, reader.phi2) * deltaPhi (phi, reader.phi2) < R * R) return true ;
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillTrackJetArray (float * pt, float * eta, float * phi, readTree & reader)
  {
    int i = 0 ;
    pt[++i] = reader.jetTrackpt1 ;
    eta[i]  = reader.jetTracketa1 ;
    phi[i]  = reader.jetTrackphi1 ;
    pt[++i] = reader.jetTrackpt2 ;
    eta[i]  = reader.jetTracketa2 ;
    phi[i]  = reader.jetTrackphi2 ;
    pt[++i] = reader.jetTrackpt3 ;
    eta[i]  = reader.jetTracketa3 ;
    phi[i]  = reader.jetTrackphi3 ;
    pt[++i] = reader.jetTrackpt4 ;
    eta[i]  = reader.jetTracketa4 ;
    phi[i]  = reader.jetTrackphi4 ;
    pt[++i] = reader.jetTrackpt5 ;
    eta[i]  = reader.jetTracketa5 ;
    phi[i]  = reader.jetTrackphi5 ;
    pt[++i] = reader.jetTrackpt6 ;
    eta[i]  = reader.jetTracketa6 ;
    phi[i]  = reader.jetTrackphi6 ;
    pt[++i] = reader.jetTrackpt7 ;
    eta[i]  = reader.jetTracketa7 ;
    phi[i]  = reader.jetTrackphi7 ;
    pt[++i] = reader.jetTrackpt8 ;
    eta[i]  = reader.jetTracketa8 ;
    phi[i]  = reader.jetTrackphi8 ;
    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillHistos (plotter & analysisPlots, readTree & reader, const string sampleName)
{
  int maxevents = reader.fChain->GetEntries () ;
  cout << "reading sample " << sampleName 
       << " with " << maxevents << " entries\n" ;
  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      // sanity checks and mild requirements on pt objects
      if (reader.pt1 < 10    ||
          reader.pt2 < 10    ||
          reader.jetpt1 < 30 ||
          reader.jetpt2 < 30) continue ;
          
      // mild VBF cuts
      if (reader.detajj < 2.5) continue ;

      TLorentzVector L_leadi_lepton ;
      L_leadi_lepton.SetPtEtaPhiM (reader.pt1, reader.eta1, reader.phi1, 0.) ;

      TLorentzVector L_trail_lepton ;
      L_trail_lepton.SetPtEtaPhiM (reader.pt2, reader.eta2, reader.phi2, 0.) ;

      TLorentzVector L_leadi_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader.jetpt1, reader.jeteta1, reader.jetphi1, 0.) ;

      TLorentzVector L_trail_jet ;
      L_trail_jet.SetPtEtaPhiM (reader.jetpt2, reader.jeteta2, reader.jetphi2, 0.) ;

      float TJ_etaMin = reader.jeteta1 ;
      float TJ_etaMax = reader.jeteta2 ;
      if (TJ_etaMin > TJ_etaMax) swap (TJ_etaMin, TJ_etaMax) ;
      
      int TKJ_num = 0 ;
      int TKJ_num_FourGeV = 0 ;
      int TKJ_num_ThreeGeV = 0 ;
      float TKJ_SumHT = 0 ;
      float TKJ_SumHT_FourGeV = 0 ;
      float TKJ_SumHT_ThreeGeV = 0 ;

      int TKJ_num_IN = 0 ;
      int TKJ_num_FourGeV_IN = 0 ;
      int TKJ_num_ThreeGeV_IN = 0 ;
      float TKJ_SumHT_IN = 0 ;
      float TKJ_SumHT_FourGeV_IN = 0 ;
      float TKJ_SumHT_ThreeGeV_IN = 0 ;

      float TKJ_pt[8] ;
      float TKJ_eta[8] ;
      float TKJ_phi[8] ;
      fillTrackJetArray (TKJ_pt, TKJ_eta, TKJ_phi, reader) ;

      // loop over track jets
      for (int iJet = 0 ; iJet < 8 ; ++iJet)      
        {
          if (TKJ_pt[iJet] < 2.) continue ; // this is the cut applied by Raffele
          if (closeToLeptons (TKJ_eta[iJet], TKJ_phi[iJet], reader, 0.3/*R*/)) continue ;

          analysisPlots.fillHisto (sampleName, "total", "tkJetPt", TKJ_pt[iJet], 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetEta", TKJ_eta[iJet], 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetPhi", TKJ_phi[iJet], 1.) ;

          ++TKJ_num ;
          TKJ_SumHT += TKJ_pt[iJet] ;
          if (TKJ_pt[iJet] > 4.) 
            {
              ++TKJ_num_FourGeV ;
              TKJ_SumHT_FourGeV += TKJ_pt[iJet] ;
            }
          if (TKJ_pt[iJet] > 3.) 
            {
              ++TKJ_num_ThreeGeV ;
              TKJ_SumHT_ThreeGeV += TKJ_pt[iJet] ;
            }
          // only the central region  
          if (TKJ_eta[iJet] < TJ_etaMin + 0.5 ||
              TKJ_eta[iJet] > TJ_etaMax - 0.5) continue ;  
          analysisPlots.fillHisto (sampleName, "total", "tkJetPt_IN", TKJ_pt[iJet], 1.) ;
          ++TKJ_num_IN ;
          TKJ_SumHT_IN += TKJ_pt[iJet] ;
          if (TKJ_pt[iJet] > 4.) 
            {
              ++TKJ_num_FourGeV_IN ;
              TKJ_SumHT_FourGeV_IN += TKJ_pt[iJet] ;
            }
          if (TKJ_pt[iJet] > 3.) 
            {
              ++TKJ_num_ThreeGeV_IN ;
              TKJ_SumHT_ThreeGeV_IN += TKJ_pt[iJet] ;
            }
        } // loop over track jets
      
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum",               TKJ_num, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV",       TKJ_num_FourGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV",      TKJ_num_ThreeGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_IN",            TKJ_num_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV_IN",    TKJ_num_FourGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV_IN",   TKJ_num_ThreeGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT",             TKJ_SumHT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV",     TKJ_SumHT_FourGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV",    TKJ_SumHT_ThreeGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_IN",          TKJ_SumHT_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV_IN",  TKJ_SumHT_FourGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_IN", TKJ_SumHT_ThreeGeV_IN, 1.) ;
      
    } // loop over events
  analysisPlots.setPoissonErrors () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  float lumi = 300. ; // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  // on cmsmi4
  string baseFolder = "/data2/govoni/TP/ntuples/2012-12-09/" ;   
  string etaLimit = argv[1] ;

  plotter analysisPlots (lumi, "plot_" + etaLimit) ;

  float XS_EWK_WW2j_126 = 4.13649215685881443E-003/*pb*/ ;
//  float XS_EWK_WW2j_noH = 4.49200073018412010E-003/*pb*/ ;
  float XS_QCD_WW2j_126 = 1.06691296353271774E-003/*pb*/ ;

  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ((baseFolder + "PHANTOM_SS_EWK_DF_126_" + etaLimit + "/*.root").c_str ()) ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries () ;
  cout << "reading TChain from " << (baseFolder + "PHANTOM_SS_EWK_DF_126_" + etaLimit + "/*.root") << endl ;

  readTree reader_EWK_WW2j_126 (sample_EWK_WW2j_126) ;

  analysisPlots.addSample ("EWK_WW2j_126", XS_EWK_WW2j_126, totEvents_EWK_WW2j_126, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j_126", "total") ; 
  
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt",                200, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta",               50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPhi",               50, -3.14, 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt_IN",             200, 0., 200.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum",               10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV",       10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV",      10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_IN",            10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_IN",    10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_IN",   10, 0., 10.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT",             200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV",     200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV",    200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_IN",          200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_IN",  200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_IN", 200, 0., 1000.) ; 

  fillHistos (analysisPlots, reader_EWK_WW2j_126, "EWK_WW2j_126") ;

  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

//   TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
//   sample_EWK_WW2j_noH->Add ((baseFolder + "PHANTOM_SS_EWK_DF_noH_" + etaLimit + "/*.root").c_str ()) ;
//   int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;
// 
//   readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;
//   analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", XS_EWK_WW2j_noH, totEvents_EWK_WW2j_noH, 0, 38) ;
//   fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH") ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ((baseFolder + "PHANTOM_SS_QCD_DF_126_" + etaLimit + "/*.root").c_str ()) ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;
  analysisPlots.copySampleStructure ("QCD_WW2j_126", "EWK_WW2j_126", XS_QCD_WW2j_126, totEvents_QCD_WW2j_126, 0, 8) ;
  fillHistos (analysisPlots, reader_QCD_WW2j_126, "QCD_WW2j_126") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  analysisPlots.plotFullLayer ("total") ;
  analysisPlots.compareStoBFullLayer ("total") ;
  analysisPlots.normaliseAllHistos () ;
  analysisPlots.compareStoBFullLayer ("total", "_norm") ;
  
  


  return 0 ;
}  
