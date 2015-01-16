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


float fillHistos (plotter & analysisPlots, readTree & reader, const string sampleName,
                 float scaleSample = 1)
{
  int maxevents = reader.fChain->GetEntries () * scaleSample ;
  cout << "reading sample " << sampleName 
       << " with " << maxevents << " entries"
       << " (scaling down sample by " << scaleSample << ")\n" ;

  int TKJ_num              = 0 ;
  int TKJ_num_ThreeGeV     = 0 ;
  int TKJ_num_FourGeV      = 0 ;
  float TKJ_SumHT          = 0. ;
  float TKJ_SumHT_ThreeGeV = 0. ;
  float TKJ_SumHT_FourGeV  = 0. ;

  int TKJ_num_IN              = 0 ;
  int TKJ_num_ThreeGeV_IN     = 0 ;
  int TKJ_num_FourGeV_IN      = 0 ;
  float TKJ_SumHT_eta2p5      = 0. ;
  float TKJ_SumHT_IN          = 0. ;
  float TKJ_SumHT_ThreeGeV_IN = 0. ;
  float TKJ_SumHT_FourGeV_IN  = 0. ;

  int TKJ_num_OUT              = 0 ;
  int TKJ_num_ThreeGeV_OUT     = 0 ;
  int TKJ_num_FourGeV_OUT      = 0 ;
  float TKJ_SumHT_OUT          = 0. ;
  float TKJ_SumHT_ThreeGeV_OUT = 0. ;
  float TKJ_SumHT_FourGeV_OUT  = 0. ;

  float deltaEtaThreshold = 0.5 ;

  int selectedEvents = 0 ;
  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 50000 == 0) cout << "reading event " << iEvent << "\n" ; 

      // read leptons, apply isolation and veto events with additional leptons
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      vector<leptonContainer> leptonVectAll ;
      fillRecoLeptonsArray (leptonVectAll, reader) ;
      vector<leptonContainer> leptonVectTight ;
      leptonVectTight = dumpLeptons (leptonVectAll, 0.55, // isolation
                                                    10.  // min pT
        ) ;

      if (leptonVectTight.size () != 2) continue ;

      // read jets, apply isolation and veto events with additional leptons
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      // jets cleaning
      vector<jetContainer> jetVectAll ;
      fillRecoJetArray (jetVectAll,reader) ;
      vector<jetContainer> recoJets ;
      recoJets = dumpJets (jetVectAll, leptonVectTight, 
                           30.,   // min pT
                           99.,   // btag
                           -999,  // PUID
                           15.,   // min pt of leptons in the cleaning from leptons
                           0.3    // matching cone
        ) ;

      if (recoJets.size () < 2) continue ;

      float TJ_etaMin = recoJets.at (0).jet4V_.Eta () ;
      float TJ_etaMax = recoJets.at (1).jet4V_.Eta () ;
      float TJ_phiMin = recoJets.at (0).jet4V_.Phi () ;
      float TJ_phiMax = recoJets.at (1).jet4V_.Phi () ;
      if (TJ_etaMin > TJ_etaMax) 
        {
          swap (TJ_etaMin, TJ_etaMax) ;
          swap (TJ_phiMin, TJ_phiMax) ;
        }  
      float dEta = TJ_etaMax - TJ_etaMin ;
      float avEta = 0.5 * (TJ_etaMax + TJ_etaMin) ;

      // mild VBF cuts
      if (dEta < 2.5) continue ;
      
      ++selectedEvents ;

      analysisPlots.fillHisto (sampleName, "total", "leadLepZep", (leptonVectTight.at (0).lepton4V_.Eta () - avEta) / dEta, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "traiLepZep", (leptonVectTight.at (1).lepton4V_.Eta () - avEta) / dEta, 1.) ;
      
      vector<jetContainer> trackJetsAll;
      fillTrackJetArray (trackJetsAll,reader) ;
      vector<jetContainer> trackJets ;
      trackJets = dumpTrackJets (trackJetsAll, leptonVectTight, 
                     2.,    // min pT
                     15.,   // min pt of leptons in the cleaning from leptons
                     0.3    // matching cone for cleaning from leptons
        ) ;
      
      TKJ_num            = 0 ;
      TKJ_num_ThreeGeV   = 0 ;
      TKJ_num_FourGeV    = 0 ;
      TKJ_SumHT          = 0. ;
      TKJ_SumHT_ThreeGeV = 0. ;
      TKJ_SumHT_FourGeV  = 0. ;

      TKJ_num_IN            = 0 ;
      TKJ_num_ThreeGeV_IN   = 0 ;
      TKJ_num_FourGeV_IN    = 0 ;
      TKJ_SumHT_eta2p5      = 0. ;
      TKJ_SumHT_IN          = 0. ;
      TKJ_SumHT_ThreeGeV_IN = 0. ;
      TKJ_SumHT_FourGeV_IN  = 0. ;

      TKJ_num_OUT            = 0 ;
      TKJ_num_ThreeGeV_OUT   = 0 ;
      TKJ_num_FourGeV_OUT    = 0 ;
      TKJ_SumHT_OUT          = 0. ;
      TKJ_SumHT_ThreeGeV_OUT = 0. ;
      TKJ_SumHT_FourGeV_OUT  = 0. ;

       // loop over track jets
      for (unsigned int iJet = 0 ; iJet < trackJets.size () ; ++iJet)      
        {
          float iJetPhi = trackJets.at (iJet).jet4V_.Phi () ;
          float iJetEta = trackJets.at (iJet).jet4V_.Eta () ;
          float iJetPt  = trackJets.at (iJet).jet4V_.Pt () ;
          float dR2_Min = deltaPhi (TJ_phiMin, iJetPhi) ;
          dR2_Min *= dR2_Min ;
          dR2_Min += (iJetEta - TJ_etaMin) * (iJetEta - TJ_etaMin) ;
          float dR2_Max = deltaPhi (TJ_phiMax, iJetPhi) ;
          dR2_Max *= dR2_Max ;
          dR2_Max += (iJetEta - TJ_etaMax) * (iJetEta - TJ_etaMax) ;
          
          // veto the tag jets
          if (sqrt(dR2_Max) < deltaEtaThreshold || sqrt(dR2_Min) < deltaEtaThreshold) continue ;

          float iJetModPhi = iJetPhi ;
          float iJetZep = (trackJets.at (iJet).jet4V_.Eta () - avEta) / dEta ;
          if (iJetZep < -0.5)     iJetModPhi -= TJ_phiMin ;
          else if (iJetZep > 0.5) iJetModPhi -= TJ_phiMax ;
          analysisPlots.fill2DHisto (sampleName, "total", "strangeTopo", 
                                     iJetZep, iJetModPhi, 1) ;

          analysisPlots.fillHisto (sampleName, "total", "tkJetPt",  trackJets.at (iJet).jet4V_.Pt (), 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetEta", trackJets.at (iJet).jet4V_.Eta (), 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetZep", iJetZep, 1.) ;
          analysisPlots.fillHisto (sampleName, "total", "tkJetPhi", trackJets.at (iJet).jet4V_.Phi (), 1.) ;

          ++TKJ_num ;
          TKJ_SumHT += iJetPt ;
          if (fabs (iJetEta) < 2.5) TKJ_SumHT_eta2p5 += iJetPt ;
          if (iJetPt > 3.) 
            {
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_ThreeGeV", iJetEta, 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetZep_ThreeGeV", iJetZep, 1.) ;
              ++TKJ_num_ThreeGeV ;
              TKJ_SumHT_ThreeGeV += iJetPt ;
            }
          if (iJetPt > 4.) 
            {
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_FourGeV", iJetEta, 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetZep_FourGeV", iJetZep, 1.) ;
              ++TKJ_num_FourGeV ;
              TKJ_SumHT_FourGeV += iJetPt ;
            }
          // only the central region
// if I do not veto the tag jets, then I add the margin deltaEtaThreshold        
//          if (iJetEta > TJ_etaMin + deltaEtaThreshold &&
//              iJetEta < TJ_etaMax - deltaEtaThreshold) 
          if (iJetEta > TJ_etaMin &&
              iJetEta < TJ_etaMax) 
            {
			  analysisPlots.fillHisto (sampleName, "total", "tkJetPt_IN", iJetPt, 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_IN", iJetEta, 1.) ;
			  ++TKJ_num_IN ;
			  TKJ_SumHT_IN += iJetPt ;
			  if (iJetPt > 3.) 
				{
				  ++TKJ_num_ThreeGeV_IN ;
				  TKJ_SumHT_ThreeGeV_IN += iJetPt ;
				}
			  if (iJetPt > 4.) 
				{
				  ++TKJ_num_FourGeV_IN ;
				  TKJ_SumHT_FourGeV_IN += iJetPt ;
				}
			} // only the central region
// if I do not veto the tag jets, then I add the margin deltaEtaThreshold        
//		  else if (iJetEta < TJ_etaMin - deltaEtaThreshold ||
//                   iJetEta > TJ_etaMax + deltaEtaThreshold) 
		  else if (iJetEta < TJ_etaMin ||
                   iJetEta > TJ_etaMax) 
            {
			  analysisPlots.fillHisto (sampleName, "total", "tkJetPt_OUT", iJetPt, 1.) ;
              analysisPlots.fillHisto (sampleName, "total", "tkJetEta_OUT", iJetEta, 1.) ;
			  ++TKJ_num_OUT ;
			  TKJ_SumHT_OUT += iJetPt ;
			  if (iJetPt > 3.) 
				{
				  ++TKJ_num_ThreeGeV_OUT ;
				  TKJ_SumHT_ThreeGeV_OUT += iJetPt ;
				}
			  if (iJetPt > 4.) 
				{
				  ++TKJ_num_FourGeV_OUT ;
				  TKJ_SumHT_FourGeV_OUT += iJetPt ;
				}
			} // only the forward regions
       } // loop over track jets
      
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum",                TKJ_num,                1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV",        TKJ_num_FourGeV,        1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV",       TKJ_num_ThreeGeV,       1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_IN",             TKJ_num_IN,             1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV_IN",     TKJ_num_FourGeV_IN,     1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV_IN",    TKJ_num_ThreeGeV_IN,    1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_OUT",            TKJ_num_OUT,            1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_FourGeV_OUT",    TKJ_num_FourGeV_OUT,    1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetNum_ThreeGeV_OUT",   TKJ_num_ThreeGeV_OUT,   1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_eta2p5",       TKJ_SumHT_eta2p5,       1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT",              TKJ_SumHT,              1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV",      TKJ_SumHT_FourGeV,      1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV",     TKJ_SumHT_ThreeGeV,     1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_IN",           TKJ_SumHT_IN,           1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV_IN",   TKJ_SumHT_FourGeV_IN,   1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_IN",  TKJ_SumHT_ThreeGeV_IN,  1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_OUT",          TKJ_SumHT_OUT,          1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_FourGeV_OUT",  TKJ_SumHT_FourGeV_OUT,  1.) ;
      analysisPlots.fillHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_OUT", TKJ_SumHT_ThreeGeV_OUT, 1.) ;

      analysisPlots.fill2DHisto (sampleName, "total", "tkJetNum_OUTvsIN",          TKJ_num_IN,          TKJ_num_OUT,          1.) ;
      analysisPlots.fill2DHisto (sampleName, "total", "tkJetNum_ThreeGeV_OUTvsIN", TKJ_num_ThreeGeV_IN, TKJ_num_ThreeGeV_OUT, 1.) ;
      analysisPlots.fill2DHisto (sampleName, "total", "tkJetNum_FourGeV_OUTvsIN",  TKJ_num_FourGeV_IN,  TKJ_num_FourGeV_OUT,  1.) ;

      analysisPlots.fill2DHisto (sampleName, "total", "tkJetSumHT_OUTvsIN",          TKJ_SumHT_IN,          TKJ_SumHT_OUT,          1.) ;
      analysisPlots.fill2DHisto (sampleName, "total", "tkJetSumHT_ThreeGeV_OUTvsIN", TKJ_SumHT_ThreeGeV_IN, TKJ_SumHT_ThreeGeV_OUT, 1.) ;
      analysisPlots.fill2DHisto (sampleName, "total", "tkJetSumHT_FourGeV_OUTvsIN",  TKJ_SumHT_FourGeV_IN,  TKJ_SumHT_FourGeV_OUT,  1.) ;

    } // loop over events
  analysisPlots.setPoissonErrors () ;
  return (1. * selectedEvents) / maxevents ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv) {

  float lumi = 300. ; // fb^(-1)
  lumi      *= 1000. ;   // transform into pb^(-1)

  // on cmsmi4
  string baseFolder = "/data2/rgerosa/NTUPLES_TP/" ;
  string etaLimit = argv[1] ;

  float scaleSamples = 1.;
  if (argc > 2)
      scaleSamples = atof (argv[2]) ;
  if (scaleSamples > 1) scaleSamples = 1. ;
  
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
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_IN",            50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_OUT",           50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep",               75, -2., 2.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt_IN",             200, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetPt_OUT",             200, 0., 200.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_FourGeV",       50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep_FourGeV",       75, -2., 2.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetEta_ThreeGeV",      50, -5., 5.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetZep_ThreeGeV",      75, -2., 2.) ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum",               20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV",       20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV",      20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_IN",            20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_IN",    20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_IN",   20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_OUT",           20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_OUT",   20, 0., 20.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_OUT",  20, 0., 20.) ; 

  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_OUTvsIN",  
                                  10, 0., 10.,
                                  10, 0., 10.,
                                  "Njets IN", "NJets OUT") ; 
  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_ThreeGeV_OUTvsIN",  
                                  10, 0., 10.,
                                  10, 0., 10.,
                                  "Njets IN (pT > 3 GeV)", "NJets OUT") ; 
  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetNum_FourGeV_OUTvsIN",  
                                  10, 0., 10.,
                                  10, 0., 10.,
                                  "Njets IN (pT > 4 GeV)", "NJets OUT") ; 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT",              20, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_eta2p5",       20, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV",      20, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV",     20, 0., 200.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_IN",           20, 0., 100.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_IN",   20, 0., 100.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_IN",  20, 0., 100.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_OUT",          20, 0., 100.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_OUT",  20, 0., 100.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_OUT", 20, 0., 100.) ; 

  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_OUTvsIN",  
                                  20, 0., 200.,
                                  20, 0., 200.,
                                  "SumHT IN", "SumHT OUT") ; 
  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_ThreeGeV_OUTvsIN",  
                                  20, 0., 200.,
                                  20, 0., 200.,
                                  "SumHT IN (pT > 3 GeV)", "SumHT OUT") ; 
  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "tkJetSumHT_FourGeV_OUTvsIN",  
                                  20, 0., 200.,
                                  20, 0., 200.,
                                  "SumHT IN (pT > 4 GeV)", "SumHT OUT") ; 

  analysisPlots.add2DPlotToLayer ("EWK_WW2j_126", "total", "strangeTopo",  
                                  20, -2., 2.,
                                  24, -3.14, 3.14,
                                  "zeppenfeld", "rotated phi") ; 

  float eff_EWK_WW2j_126 = 
      fillHistos (analysisPlots, reader_EWK_WW2j_126, "EWK_WW2j_126", scaleSamples) ;
 
  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

//   TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
//   sample_EWK_WW2j_noH->Add ((baseFolder + "PHANTOM_SS_EWK_DF_noH_" + etaLimit + "/*.root").c_str ()) ;
//   int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;
// 
//   readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;
//   analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", XS_EWK_WW2j_noH, totEvents_EWK_WW2j_noH, 0, 38) ;
//   fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH", scaleSamples) ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ((baseFolder + "PHANTOM_SS_QCD_DF_126_" + etaLimit + "/*.root").c_str ()) ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;
  analysisPlots.copySampleStructure ("QCD_WW2j_126", "EWK_WW2j_126", XS_QCD_WW2j_126, totEvents_QCD_WW2j_126, 0, 8) ;
  float eff_QCD_WW2j_126 = 
      fillHistos (analysisPlots, reader_QCD_WW2j_126, "QCD_WW2j_126", scaleSamples) ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  analysisPlots.plotFullLayer ("total") ;
  analysisPlots.compareStoBFullLayer ("total") ;
  analysisPlots.saveAllHistos ("outfile.root") ;
  analysisPlots.normaliseAllHistos () ;
  analysisPlots.compareStoBFullLayer ("total", "_norm") ;
  analysisPlots.compareStoBFullLayer2D ("total", "_norm") ;
  
  cout << "eff_EWK_WW2j_126 : " << eff_EWK_WW2j_126 << endl ; 
  cout << "eff_QCD_WW2j_126 : " << eff_QCD_WW2j_126 << endl ; 
  return 0 ;
}  
