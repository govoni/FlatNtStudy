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

#include "utils.h"

using namespace std ;


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

      // read leptons, apply isolation and veto events with additional leptons
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      float LEP_pt[4] ;
      float LEP_eta[4] ;
      float LEP_phi[4] ;
      float LEP_iso[4] ;
      fillRecoLeptonsArray (LEP_pt, LEP_eta, LEP_phi, LEP_iso, reader) ;
      vector<TLorentzVector> TL_leptons ;
      dumpLeptons (TL_leptons, LEP_pt, LEP_eta, LEP_phi, LEP_iso, 
                   0.4, // isolation
                   10.  // min pT
        ) ;

      if (TL_leptons.size () != 2) continue ;

      // read jets, apply isolation and veto events with additional leptons
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      // FIXME jets cleaning
      float REJ_pt[8] ;
      float REJ_eta[8] ;
      float REJ_phi[8] ;
      float REJ_mass[8] ;
      float REJ_ID[8] ;
      float REJ_btag[8] ;
      fillRecoJetArray (REJ_pt, REJ_eta, REJ_phi, REJ_mass, REJ_ID, REJ_btag, reader) ;
      vector<TLorentzVector> TL_jets ;
      dumpJets (TL_jets, TL_leptons, 
                REJ_pt, REJ_eta, REJ_phi, REJ_mass, REJ_btag, 
                30.,   // min pT
                99.,   // btag
                15.,   // min pt of cleaning from leptons
                0.3    // matching cone
        ) ;
      
      if (TL_jets.size () < 2) continue ;

      float TJ_etaMin = TL_jets.at (0).Eta () ;
      float TJ_etaMax = TL_jets.at (1).Eta () ;
      if (TJ_etaMin > TJ_etaMax) swap (TJ_etaMin, TJ_etaMax) ;
      float dEta = TJ_etaMax - TJ_etaMin ;
      float avEta = 0.5 * (TJ_etaMax + TJ_etaMin) ;

      // mild VBF cuts
      if (dEta < 2.5) continue ;

      analysisPlots.fillHisto (sampleName, "total", "leadLepZep", (TL_leptons.at (0).Eta () - avEta) / dEta, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "traiLepZep", (TL_leptons.at (1).Eta () - avEta) / dEta, 1.) ;

      float TKJ_pt[8] ;
      float TKJ_eta[8] ;
      float TKJ_phi[8] ;
      float TKJ_mass[8] ;
      fillTrackJetArray (TKJ_pt, TKJ_eta, TKJ_phi, TKJ_mass, reader) ;
      vector<TLorentzVector> TL_trackJets ;
      dumpTrackJets (TL_trackJets, TL_leptons, 
                     TKJ_pt, TKJ_eta, TKJ_phi, TKJ_mass, 
                     2.,    // min pT
                     15.,   // min pt of leptons in the cleaning from leptons
                     0.3    // matching cone for cleaning from leptons
        ) ;
      
      int TKJ_num              = 0 ;
      int TKJ_num_ThreeGeV     = 0 ;
      int TKJ_num_FourGeV      = 0 ;
      float TKJ_SumHT          = 0. ;
      float TKJ_SumHT_ThreeGeV = 0. ;
      float TKJ_SumHT_FourGeV  = 0. ;

      int TKJ_num_IN              = 0 ;
      int TKJ_num_ThreeGeV_IN     = 0 ;
      int TKJ_num_FourGeV_IN      = 0 ;
      float TKJ_SumHT_IN          = 0. ;
      float TKJ_SumHT_ThreeGeV_IN = 0. ;
      float TKJ_SumHT_FourGeV_IN  = 0. ;

      OUTt TKJ_num_OUT              = 0 ;
      OUTt TKJ_num_ThreeGeV_OUT     = 0 ;
      OUTt TKJ_num_FourGeV_OUT      = 0 ;
      float TKJ_SumHT_OUT          = 0. ;
      float TKJ_SumHT_ThreeGeV_OUT = 0. ;
      float TKJ_SumHT_FourGeV_OUT  = 0. ;

      float deltaEtaThreshold = 0.7 ;
       // loop over track jets
      for (unsigned int iJet = 0 ; iJet < TL_trackJets.size () ; ++iJet)      
        {
          analysisPlots.fillHisto (sampleName, "total", "tkJetPt",  TL_trackJets.at (iJet).Pt (), 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetEta", TL_trackJets.at (iJet).Eta (), 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetZep", (TL_trackJets.at (iJet).Eta () - avEta) / dEta, 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetPhi", TL_trackJets.at (iJet).Phi (), 1.) ;

          ++TKJ_num ;
          TKJ_SumHT += TL_trackJets.at (iJet).Pt () ;
          if (TL_trackJets.at (iJet).Pt () > 3.) 
            {
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_ThreeGeV", TL_trackJets.at (iJet).Eta (), 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetZep_ThreeGeV", (TL_trackJets.at (iJet).Eta () - avEta) / dEta, 1.) ;
              ++TKJ_num_ThreeGeV ;
              TKJ_SumHT_ThreeGeV += TL_trackJets.at (iJet).Pt () ;
            }
          if (TL_trackJets.at (iJet).Pt () > 4.) 
            {
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_FourGeV", TL_trackJets.at (iJet).Eta (), 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetZep_FourGeV", (TL_trackJets.at (iJet).Eta () - avEta) / dEta, 1.) ;
              ++TKJ_num_FourGeV ;
              TKJ_SumHT_FourGeV += TL_trackJets.at (iJet).Pt () ;
            }
          // only the central region  
          if (TL_trackJets.at (iJet).Eta () > TJ_etaMin + deltaEtaThreshold &&
              TL_trackJets.at (iJet).Eta () < TJ_etaMax - deltaEtaThreshold) 
            {
			  analysisPlots.fillHisto (sampleName, "total", "tkJetPt_IN", TL_trackJets.at (iJet).Pt (), 1.) ;
			  ++TKJ_num_IN ;
			  TKJ_SumHT_IN += TL_trackJets.at (iJet).Pt () ;
			  if (TL_trackJets.at (iJet).Pt () > 3.) 
				{
				  ++TKJ_num_ThreeGeV_IN ;
				  TKJ_SumHT_ThreeGeV_IN += TL_trackJets.at (iJet).Pt () ;
				}
			  if (TL_trackJets.at (iJet).Pt () > 4.) 
				{
				  ++TKJ_num_FourGeV_IN ;
				  TKJ_SumHT_FourGeV_IN += TL_trackJets.at (iJet).Pt () ;
				}
			}
		  else if (TL_trackJets.at (iJet).Eta () < TJ_etaMin - deltaEtaThreshold ||
                   TL_trackJets.at (iJet).Eta () > TJ_etaMax + deltaEtaThreshold) 
            {
			  analysisPlots.fillHisto (sampleName, "total", "tkJetPt_OUT", TL_trackJets.at (iJet).Pt (), 1.) ;
			  ++TKJ_num_OUT ;
			  TKJ_SumHT_OUT += TL_trackJets.at (iJet).Pt () ;
			  if (TL_trackJets.at (iJet).Pt () > 3.) 
				{
				  ++TKJ_num_ThreeGeV_OUT ;
				  TKJ_SumHT_ThreeGeV_OUT += TL_trackJets.at (iJet).Pt () ;
				}
			  if (TL_trackJets.at (iJet).Pt () > 4.) 
				{
				  ++TKJ_num_FourGeV_OUT ;
				  TKJ_SumHT_FourGeV_OUT += TL_trackJets.at (iJet).Pt () ;
				}
			}
       } // loop over track jets
      
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum",               TKJ_num, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV",       TKJ_num_FourGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV",      TKJ_num_ThreeGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_IN",            TKJ_num_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV_IN",    TKJ_num_FourGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV_IN",   TKJ_num_ThreeGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_OUT",            TKJ_num_OUT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV_OUT",    TKJ_num_FourGeV_OUT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV_OUT",   TKJ_num_ThreeGeV_OUT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT",             TKJ_SumHT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV",     TKJ_SumHT_FourGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV",    TKJ_SumHT_ThreeGeV, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_IN",          TKJ_SumHT_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV_IN",  TKJ_SumHT_FourGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_IN", TKJ_SumHT_ThreeGeV_IN, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_OUT",          TKJ_SumHT_OUT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV_OUT",  TKJ_SumHT_FourGeV_OUT, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_OUT", TKJ_SumHT_ThreeGeV_OUT, 1.) ;
      
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

  plotter analysisPlots (lumi, "plots_" + etaLimit) ;

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
  
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "leadLepZep",             75, -2., 2.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "traiLepZep",             75, -2., 2.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt",                200, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPhi",               50, -3.14, 3.14) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta",               50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep",               75, -2., 2.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt_IN",             200, 0., 200.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_FourGeV",       50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep_FourGeV",       75, -2., 2.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_ThreeGeV",      50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep_ThreeGeV",      75, -2., 2.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum",               10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV",       10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV",      10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_IN",            10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_IN",    10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_IN",   10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_OUT",            10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_OUT",    10, 0., 10.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_OUT",   10, 0., 10.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT",             200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV",     200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV",    200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_IN",          200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_IN",  200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_OUT", 200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_OUT",          200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_OUT",  200, 0., 1000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_OUT", 200, 0., 1000.) ; 

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
