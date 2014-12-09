//////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"

#include "readTree.h"
#include "plotter.h"
#include "ConfigParser.h"
#include "treeReader.h"

using namespace std ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
void fillHistos (plotter & analysisPlots, treeReader* reader, const string sampleName);

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main (int argc, char ** argv) {

  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<std::endl;
    return -1;
  }
 
  // parse config file parameter
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();

  TString config ; 
  config.Form("%s",argv[1]);

  if(!(gConfigParser->init(config))){
      std::cout << ">>> parseConfigFile::Could not open configuration file " << config << std::endl;
      return -1;
  }


  std::string InputDirectoryEWKH126 = gConfigParser -> readStringOption("Input::InputDirectoryEWKH126");
  // std::string InputDirectory_noH     = gConfigParser -> readStringOption("Input::InputDirectory_EWKnoH");
  // std::string InputDirectory_VVQCD   = gConfigParser -> readStringOption("Input::InputDirectory_QCD");
  // std::string InputDirectory_WZ      = gConfigParser -> readStringOption("Input::InputDirectory_WZ");

  double XS_SignalEWK_H126 = gConfigParser -> readDoubleOption("Option::XSSignalEWKH126");
  double XS_SignalEWK_noH  = gConfigParser -> readDoubleOption("Option::XSSignalEWKnoH");
  double XS_VV_QCD         = gConfigParser -> readDoubleOption("Option::XSVVQCD");
  double XS_WZ             = gConfigParser -> readDoubleOption("Option::XSWZ");
  

  // print a table with the expected number of events
  // ---- ---- ---- ---- ---- ---- ----

  float baseLumi = 20. ; // fb^(-1)
  if(argc >=3) {  
    baseLumi = atof(argv[2]);
  }

  baseLumi *= 1000. ;    // transform into pb^(-1)
  cout << "lumi" << "\t" 
       << "EWK126" << "\t"
       << "EWKnoH" << "\t"
       << "dif" << "\t"
       << "QCD" << "\t"
       << "WZ"  << "\t"
       << "dif/QCD" << "\t"
       << "dif/rad(QCD)" << "\n" ;
  cout << "(/fb)" << "\t" 
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << " -- " << "\t" 
       << " -- " << "\n" ;

  for (int i = 0 ; i < 8 ; ++i)
    {
      cout << (baseLumi / 1000.) << "\t" 
           << int (baseLumi * XS_SignalEWK_H126) << "\t"
           << int (baseLumi * XS_SignalEWK_noH) << "\t"
           << int (baseLumi * XS_SignalEWK_noH - baseLumi * XS_SignalEWK_H126) << "\t"
           << int (baseLumi * XS_VV_QCD) << "\t" 
           << int (baseLumi * XS_WZ) << "\t" 
           << setprecision (2) << (XS_SignalEWK_noH - XS_SignalEWK_H126) / (XS_VV_QCD) << "\t"
           << setprecision (2) << (baseLumi * XS_SignalEWK_noH - baseLumi * XS_SignalEWK_H126) / sqrt (baseLumi * XS_VV_QCD) << "\n";
      baseLumi *= 2 ;
    }
  
  
  // make distributions with several luminosities
  // ---- ---- ---- ---- ---- ---- ----
 
  float lumi =  gConfigParser -> readDoubleOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)
  plotter analysisPlots (lumi) ;

  
  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ((InputDirectoryEWKH126+"/*root").c_str()) ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries();

  treeReader* fReader_EWK_WW2j_126  = new treeReader((TTree*)(sample_EWK_WW2j_126), false);

  analysisPlots.addSample        ("EWK_WW2j_126", XS_SignalEWK_H126, totEvents_EWK_WW2j_126, 1, 50) ;   
  analysisPlots.addLayerToSample ("EWK_WW2j_126", "total") ; // cut applyed  -> no cut
  analysisPlots.addPlotToLayer   ("EWK_WW2j_126", "total", "dPhiLL",25,0.,3.14) ;  // plot
  
  fillHistos (analysisPlots, fReader_EWK_WW2j_126, "EWK_WW2j_126") ; // fill the histogram
  analysisPlots.plotSingleSample("EWK_WW2j_126","total","dPhiLL","#Delta#phi_{LL}","entries",0,"output");
  
  /*
  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_noH->Add ("eos/cms/store/group/dpg_ecal/alca_ecalcalib/ecalMIBI/rgerosa/TP_ANALYSIS/REDUCED_TREE/JOBS_DUMPER/PHANTOM_SS_EWK_DF_noH_2p5/outDumper_0.root") ;
  int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;

  readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;

  analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", XS_EWK_WW2j_noH, totEvents_EWK_WW2j_noH, 1, 38) ;

  fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH") ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ("eos/cms/store/group/dpg_ecal/alca_ecalcalib/ecalMIBI/rgerosa/TP_ANALYSIS/REDUCED_TREE/JOBS_DUMPER/PHANTOM_SS_QCD_DF_126_2p5/outDumper_0.root") ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;

  analysisPlots.copySampleStructure ("QCD_WW2j_126", "EWK_WW2j_126", XS_QCD_WW2j_126, totEvents_QCD_WW2j_126, 0, 8) ;

  fillHistos (analysisPlots, reader_QCD_WW2j_126, "QCD_WW2j_126") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  int intlumi = lumi / 1000. ;
  for (int i = 0 ; i < 8 ; ++i)
    {
      analysisPlots.scaleAllHistos (2.) ;
      analysisPlots.setPoissonErrors () ;
      string baseFolder = "lumi_" ;
      baseFolder += to_string (intlumi) ;
      //      analysisPlots.plotRelativeExcessFullLayer ("total", baseFolder) ;
      intlumi *= 2. ;
    }

  return 0 ;
  */
}  


void fillHistos (plotter & analysisPlots, treeReader* reader, const string sampleName) {
  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader->GetEntries() ;

  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents/25 ; ++iEvent){
      reader->GetEntry(iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      if(reader->getFloat("pt1")[0] < 0 or reader->getFloat("pt2")[0] < 0) continue ;

      TLorentzVector L_leadi_lepton ;
      L_leadi_lepton.SetPtEtaPhiM (reader->getFloat("pt1")[0], reader->getFloat("eta1")[0], reader->getFloat("phi1")[0], 0.) ;     

      TLorentzVector L_trail_lepton ;
      L_trail_lepton.SetPtEtaPhiM (reader->getFloat("pt2")[0], reader->getFloat("eta2")[0], reader->getFloat("phi2")[0], 0.) ;

      TLorentzVector L_dilepton = L_leadi_lepton + L_trail_lepton ;

      TLorentzVector L_met ;
      L_met.SetPtEtaPhiM (reader->getFloat("pfmet")[0],0.,reader->getFloat("pfmetphi")[0], 0.) ;

      TLorentzVector L_leadi_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader->getFloat("jetpt1")[0], reader->getFloat("jeteta1")[0], reader->getFloat("jetphi1")[0], 0.) ;

      TLorentzVector L_trail_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader->getFloat("jetpt2")[0], reader->getFloat("jeteta2")[0], reader->getFloat("jetphi2")[0], 0.) ;

      TLorentzVector L_dijet = L_leadi_jet + L_trail_jet ;

      float asimJ = (L_leadi_jet.Pt () - L_trail_jet.Pt ()) / (L_leadi_jet.Pt () + L_trail_jet.Pt ()) ;
      float asimL = (L_leadi_lepton.Pt () - L_trail_lepton.Pt ()) / (L_leadi_lepton.Pt () + L_trail_lepton.Pt ()) ;
      float Rvar = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_jet.Pt () * L_trail_jet.Pt ()) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLL", fabs (L_leadi_lepton.DeltaPhi (L_trail_lepton)), 1.) ;
      // pT Ws per distinguere segnale da fondo QCD

    } // loop over events

  analysisPlots.setPoissonErrors() ;
  return ;
}

