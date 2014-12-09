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

  // check number of inpt parameters
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

  // import some informations
  std::string InputBaseDirectory           = gConfigParser -> readStringOption("Input::InputBaseDirectory");

  std::vector<std::string> InputEWKH126    = gConfigParser -> readStringListOption("Input::InputEWKH126");
  std::vector<std::string> InputEWKnoH     = gConfigParser -> readStringListOption("Input::InputEWKnoH");
  std::vector<std::string> InputVVQCD      = gConfigParser -> readStringListOption("Input::InputVVQCD");
  std::vector<std::string> InputWZEWKH126  = gConfigParser -> readStringListOption("Input::InputWZEWKH126");
  std::vector<std::string> InputWZQCD      = gConfigParser -> readStringListOption("Input::InputWZQCD");

  std::string treeName                     = gConfigParser -> readStringOption("Input::TreeName");

  double XS_EWK_H126 = gConfigParser -> readDoubleOption("Option::XSEWKH126");
  double XS_EWK_noH  = gConfigParser -> readDoubleOption("Option::XSEWKnoH");
  double XS_VV_QCD   = gConfigParser -> readDoubleOption("Option::XSVVQCD");

  double XS_WZ_EWK_H126   = gConfigParser -> readDoubleOption("Option::XSVVQCD");
  double XS_WZ_QCD        = gConfigParser -> readDoubleOption("Option::XSVVQCD");

  std::string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p "+outputPlotDirectory).c_str());
  system(("rm -r "+outputPlotDirectory+"/*").c_str());


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
           << int (baseLumi * XSBackground.at(0)) << "\t" 
           << setprecision (2) << (XS_SignalEWK_noH - XS_SignalEWK_H126) / (XS_VV_QCD) << "\t"
           << setprecision (2) << (baseLumi * XS_SignalEWK_noH - baseLumi * XS_SignalEWK_H126) / sqrt (baseLumi * XS_VV_QCD) << "\n";
      baseLumi *= 2 ;
    }
  
  
  // make distributions with several luminosities
  // ---- ---- ---- ---- ---- ---- ----
 
  double lumi =  gConfigParser -> readDoubleOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)
  plotter analysisPlots (lumi) ;

  
  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_EWK_WW2j_126 = new TChain (treeName.c_str()) ;  
  for(size_t iEntry = 0; iEntry < InputEWKH126.size(); iEntry++)
    sample_EWK_WW2j_126->Add ((InputBaseDirectory+"/"+InputEWKH126.at(iEntry)+"/*root").c_str()) ;

  int totEvents_EWK_WW2j_126        = sample_EWK_WW2j_126->GetEntries();
  treeReader* fReader_EWK_WW2j_126  = new treeReader((TTree*)(sample_EWK_WW2j_126), false);

  analysisPlots.addSample         ("EWK_WW2j_126", XS_SignalEWK_H126, totEvents_EWK_WW2j_126, 1, 2) ;   
  analysisPlots.addLayerToSample  ("EWK_WW2j_126", "total") ; 

  analysisPlots.addPlotToLayer    ("EWK_WW2j_126", "total", "dPhiLL",25,0.,3.14) ;  // add single histo to this 

  analysisPlots.copyLayerInSample ("EWK_WW2j_126","looseVBF","total");  // copy this layer in another one
  analysisPlots.copyLayerInSample ("EWK_WW2j_126","tightVBF","total");  // copy this layer in another one

  /*  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLMet",  25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLPuppiMet",  25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLGenMet",    25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLLMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLLPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLLGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTLMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTLPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTLGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJ",    25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiGenJJ",    25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiPuppiJJ", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiGenJJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiPuppiJJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiPuppiJJPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiGenJJPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJPuppiGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiPuppiJJGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiJJGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiGenJJGenMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLPuppiJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLJPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLPuppiJPuppiMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiTJMet", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mjj",       50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mjjgen",    50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mjjpuppi",  50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "mll",       50, 0., 1500.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimJ",     50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimPuppiJ",50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimGenJ",50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "asimL",     50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R",         50, 0., 5.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "RPuppi",         50, 0., 5.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "RGen",         50, 0., 5.) ;
  */

  fillHistos (analysisPlots, fReader_EWK_WW2j_126, "EWK_WW2j_126") ; // fill the histogram
  
  
  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputEWKnoH.size(); iEntry++)
    sample_EWK_WW2j_noH->Add ((InputBaseDirectory+"/"+InputEWKnoH.at(iEntry)+"/*root").c_str()) ;

  int totEvents_EWK_WW2j_noH        = sample_EWK_WW2j_noH->GetEntries();
  treeReader* fReader_EWK_WW2j_noH  = new treeReader((TTree*)(sample_EWK_WW2j_noH), false);

  analysisPlots.copySampleStructure ("EWK_WW2j_noH","EWK_WW2j_126", XS_SignalEWK_noH, totEvents_EWK_WW2j_noH, 1,4)
  fillHistos (analysisPlots, fReader_EWK_WW2j_noH, "EWK_WW2j_noH") ; // fill the histogram

  
  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_QCD_WW2j = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputVVQCD.size(); iEntry++)
    sample_QCD_WW2j->Add ((InputBaseDirectory+"/"+InputVVQCD.at(iEntry)+"/*root").c_str()) ;

  int totEvents_QCD_WW2j = sample_QCD_WW2j->GetEntries();
  treeReader* fReader_QCD_WW2j  = new treeReader((TTree*)(sample_QCD_WW2j), false);

  analysisPlots.copySampleStructure ("QCD_WW2j","EWK_WW2j_126", XS_VV_QCD, totEvents_QCD_WW2j, 0, kGreen+1)

  fillHistos (analysisPlots, fReader_QCD_WW2j, "QCD_WW2j") ; // fill the histogram


  // WZ H126
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_WZ_EWK_H126 = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputWZEWKH126.size(); iEntry++)
    sample_WZ_EWK_H126->Add ((InputBaseDirectory+"/"+InputWZEWKH126.at(iEntry)+"/*root").c_str()) ;

  int totEvents_WZ_EWK_H126 = sample_WZ_EWK_H126->GetEntries();
  treeReader* fReader_WZ_EWK_H126  = new treeReader((TTree*)(sample_WZ_EWK_H126), false);

  analysisPlots.copySampleStructure ("WZ_EWK_H126","WZ_EWK_H126", XS_WZ_EWK_H126, totEvents_WZ_EWK_H126, 0, kAzure+1)

  fillHistos (analysisPlots, fReader_WZ_EWK_H126, "WZ_EWK_H126") ; // fill the histogram


  // WZ QCD
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_WZ_QCD = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputWZQCD.size(); iEntry++)
    sample_WZ_QCD->Add ((InputBaseDirectory+"/"+InputWZQCD.at(iEntry)+"/*root").c_str()) ;

  int totEvents_WZ_QCD = sample_WZ_QCD->GetEntries();
  treeReader* fReader_WZ_QCD  = new treeReader((TTree*)(sample_WZ_QCD), false);

  analysisPlots.copySampleStructure ("WZ_QCD","WZ_QCD", XS_WZ_QCD, totEvents_WZ_QCD, 0, kYellow+1)

  fillHistos (analysisPlots, fReader_WZ_QCD, "WZ_QCD") ; // fill the histogram
      
  // plotting
  // ---- ---- ---- ---- ---- ---- ----
  analysisPlots.setPoissonErrors () ;
  analysisPlots.plotFullLayer("total");  
  analysisPlots.compareStoBFullLayer("total");
  analysisPlots.plotRelativeExcessFullLayer ("total", outputPlotDirectory) ;

}  


void fillHistos (plotter & analysisPlots, treeReader* reader, const string sampleName) {
  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader->GetEntries() ;

  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents/100 ; ++iEvent){
      reader->GetEntry(iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      if(reader->getFloat("pt1")[0] < 0 or reader->getFloat("pt2")[0] < 0) continue ; // skip the event --> only two reco leptons are good
      if(reader->getFloat("jetpt1")[0] < 0 or reader->getFloat("jetpt2")[0] < 0) continue ; // skip the event with less than two reco jet 
      if(reader->getFloat("jetpt_puppi1")[0] < 0 or reader->getFloat("jetpt_puppi2")[0] < 0) continue ; // skip the event with less than two reco jet 

      TLorentzVector L_leadi_lepton ;
      L_leadi_lepton.SetPtEtaPhiM (reader->getFloat("pt1")[0], reader->getFloat("eta1")[0], reader->getFloat("phi1")[0], 0.) ;     

      TLorentzVector L_trail_lepton ;
      L_trail_lepton.SetPtEtaPhiM (reader->getFloat("pt2")[0], reader->getFloat("eta2")[0], reader->getFloat("phi2")[0], 0.) ;

      TLorentzVector L_dilepton = L_leadi_lepton + L_trail_lepton ;

      TLorentzVector L_met ;
      L_met.SetPtEtaPhiM (reader->getFloat("pfmet")[0],0.,reader->getFloat("pfmetphi")[0], 0.) ;

      TLorentzVector L_gen_met ;
      L_gen_met.SetPtEtaPhiM (reader->getFloat("metGenpt")[0],0.,reader->getFloat("metGenphi")[0], 0.) ;

      TLorentzVector L_puppi_met ;
      L_puppi_met.SetPtEtaPhiM (reader->getFloat("pfmet_puppi")[0],0.,reader->getFloat("pfmetphi_puppi")[0], 0.) ;

      TLorentzVector L_leadi_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader->getFloat("jetpt1")[0], reader->getFloat("jeteta1")[0], reader->getFloat("jetphi1")[0], 0.) ;

      TLorentzVector L_trail_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader->getFloat("jetpt2")[0], reader->getFloat("jeteta2")[0], reader->getFloat("jetphi2")[0], 0.) ;

      TLorentzVector L_dijet = L_leadi_jet + L_trail_jet ;

      TLorentzVector L_leadi_gen_jet ;
      L_leadi_gen_jet.SetPtEtaPhiM (reader->getFloat("jetGenpt1")[0], reader->getFloat("jetGeneta1")[0], reader->getFloat("jetGenphi1")[0], 0.) ;

      TLorentzVector L_trail_gen_jet ;
      L_leadi_gen_jet.SetPtEtaPhiM (reader->getFloat("jetGenpt2")[0], reader->getFloat("jetGeneta2")[0], reader->getFloat("jetGenphi2")[0], 0.) ;

      TLorentzVector L_dijet_gen = L_leadi_gen_jet + L_trail_gen_jet ;

      TLorentzVector L_leadi_puppi_jet ;
      L_leadi_puppi_jet.SetPtEtaPhiM (reader->getFloat("jetpt_puppi1")[0], reader->getFloat("jeteta_puppi1")[0], reader->getFloat("jetphi_puppi1")[0], 0.) ;

      TLorentzVector L_trail_puppi_jet ;
      L_leadi_puppi_jet.SetPtEtaPhiM (reader->getFloat("jetpt_puppi2")[0], reader->getFloat("jeteta_puppi2")[0], reader->getFloat("jetphi_puppi2")[0], 0.) ;

      TLorentzVector L_dijet_puppi = L_leadi_puppi_jet + L_trail_puppi_jet ;
   

      float asimJ = (L_leadi_jet.Pt () - L_trail_jet.Pt ()) / (L_leadi_jet.Pt () + L_trail_jet.Pt ()) ;
      float asimL = (L_leadi_lepton.Pt () - L_trail_lepton.Pt ()) / (L_leadi_lepton.Pt () + L_trail_lepton.Pt ()) ;
      float Rvar = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_jet.Pt () * L_trail_jet.Pt ()) ;

      float asimPuppiJ = (L_leadi_puppi_jet.Pt () - L_trail_puppi_jet.Pt ()) / (L_leadi_puppi_jet.Pt () + L_trail_puppi_jet.Pt ()) ;
      float RvarPuppi  = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_puppi_jet.Pt () * L_trail_puppi_jet.Pt ()) ;

      float asimGenJ  = (L_leadi_gen_jet.Pt () - L_trail_gen_jet.Pt ()) / (L_leadi_gen_jet.Pt () + L_trail_gen_jet.Pt ()) ;
      float RvarGen   = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_gen_jet.Pt () * L_trail_gen_jet.Pt ()) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLL",   fabs (L_leadi_lepton.DeltaPhi (L_trail_lepton)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLMet", fabs (L_leadi_lepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLPuppiMet", fabs (L_leadi_lepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLGenMet", fabs (L_leadi_lepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLLMet", fabs (L_dilepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLLPuppiMet", fabs (L_dilepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLLGenMet", fabs (L_dilepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiTLMet", fabs (L_trail_lepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiTLPuppiMet", fabs (L_trail_lepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiTLGenMet", fabs (L_trail_lepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiJJ",      fabs (L_leadi_jet.DeltaPhi (L_trail_jet)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiPuppiJJ", fabs (L_leadi_puppi_jet.DeltaPhi (L_trail_puppi_jet)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiGenJJ",   fabs (L_leadi_gen_jet.DeltaPhi (L_trail_gen_jet)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiJJMet",      fabs (L_dijet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiGenJJMet",      fabs (L_dijet_gen.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiPuppiJJMet",      fabs (L_dijet_puppi.DeltaPhi (L_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiJJPuppiMet",      fabs (L_dijet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiGenJJPuppiMet",   fabs (L_dijet_gen.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiPuppiJJPuppiMet", fabs (L_dijet_puppi.DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiJJGenMet",      fabs (L_dijet.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiGenJJGenMet",   fabs (L_dijet_gen.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiPuppiJJGenMet", fabs (L_dijet_puppi.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLJMet",      fabs (L_leadi_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLJPuppiMet", fabs (L_leadi_jet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLPuppiJMet", fabs (L_leadi_puppi_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "dPhiLPuppiJPuppiMet", fabs (L_leadi_puppi_jet.DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiTJMet",      fabs (L_trail_jet.DeltaPhi (L_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "mjj",        L_dijet.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "mjjgen",     L_dijet_gen.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "mjjpuppi",   L_dijet_puppi.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "mll",        L_dilepton.M(), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "asimJ", asimJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "asimL", asimL, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "R", Rvar, 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "asimPuppiJ", asimPuppiJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "RPuppi", RvarPuppi, 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "asimGenJ", asimGenJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "RGen", RvarGen, 1.) ;

      // pT Ws per distinguere segnale da fondo QCD

    } // loop over events

  analysisPlots.setPoissonErrors() ;
  return ;
}

