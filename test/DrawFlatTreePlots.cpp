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
#include "readTree.h"
#include "utils.h"

using namespace std ;

#define leptonIsoCut 0.4 
#define minLeptonCutPt 10.
#define minJetCutPt 30.
#define bTagCutVeto 99
#define matchingCone 0.3
#define minLeptonCleaningPt 15.

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -
void fillHistos (plotter & analysisPlots, readTree* reader, const string sampleName);

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

  double XS_WZ_EWK_H126   = gConfigParser -> readDoubleOption("Option::XSWZEWKH126");
  double XS_WZ_QCD        = gConfigParser -> readDoubleOption("Option::XSWZQCD");

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
           << int (baseLumi * XS_EWK_H126) << "\t"
           << int (baseLumi * XS_EWK_noH) << "\t"
           << int (baseLumi * XS_EWK_noH - baseLumi * XS_EWK_H126) << "\t"
           << int (baseLumi * XS_VV_QCD) << "\t" 
           << int (baseLumi * XS_WZ_EWK_H126) << "\t" 
           << setprecision (2) << (XS_EWK_noH - XS_EWK_H126) / (XS_VV_QCD) << "\t"
           << setprecision (2) << (baseLumi * XS_EWK_noH - baseLumi * XS_EWK_H126) / sqrt (baseLumi * XS_VV_QCD) << "\n";
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
    sample_EWK_WW2j_126->Add ((InputBaseDirectory+"/"+InputEWKH126.at(iEntry)+"/*1.root").c_str()) ;

  int totEvents_EWK_WW2j_126        = sample_EWK_WW2j_126->GetEntries();
  std::cout<<"totEvents_EWK_WW2j_126 "<<totEvents_EWK_WW2j_126<<std::endl;
  readTree* fReader_EWK_WW2j_126  = new readTree((TTree*)(sample_EWK_WW2j_126));

  analysisPlots.addSample         ("EWK_WW2j_126", XS_EWK_H126, totEvents_EWK_WW2j_126, 1, 2) ;   
  analysisPlots.addLayerToSample  ("EWK_WW2j_126", "total") ; 
  
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LL}",25,0.,3.14) ;  // add single histo to this 

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#eta_{jj}",          30, 0., 7.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#eta_{jj}^{gen}",    30, 0., 7.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#eta_{jj}^{puppi}",  30, 0., 7.) ;
  
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "M_{jj}",       50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "M_{jj}^{gen}",    50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "M_{jj}^{puppi}",  50, 0., 3000.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "M_{ll}",       50, 0., 1500.) ;
  
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{l1}",       50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{l2}",       50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "met",        50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "met^{puppi}",   50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "met^{gen}",     50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j1}^{puppi}",  50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j2}^{puppi}", 50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j1}^{gen}",  50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j2}^{gen}",  50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j1}",     50, 0., 600.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "pt_{j2}",     50, 0., 600.) ;

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "Asim_{J}",     50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "Asim_{J}^{puppi}",50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "Asim_{J}^{gen}",  50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "Asim_{L}",    50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R",         50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R^{puppi}", 50, 0., 1.) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "R^{gen}",   50, 0., 1.) ;

  /*
  /*  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LMet}",  25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LMet^{puppi}}",  25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LMet^{gen}}",    25, 0., 3.14) ;
  

  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LLMet}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LLMet^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LLMet^{gen}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{TLMet}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{TLMet^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{TLMet^{gen}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ}",    25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{gen}}",    25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJMet}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{gen}Met}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{puppi}Met}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJMet^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{puppi}Met^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{gen}Met^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{puppi}Met^{gen}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{puppi}Met^{gen}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{gen}Met}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{JJ^{gen}Met^{gen}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LJMet}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LJ^{puppi}Met}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LJMet^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{LJ^{puppi}Met^{puppi}}", 25, 0., 3.14) ;
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "#Delta#phi_{TJMet}", 25, 0., 3.14) ;
  */
  analysisPlots.copyLayerInSample ("EWK_WW2j_126","looseVBF","total");  // copy this layer in another one
  analysisPlots.copyLayerInSample ("EWK_WW2j_126","tightVBF","total");  // copy this layer in another one
  
  fillHistos (analysisPlots, fReader_EWK_WW2j_126, "EWK_WW2j_126") ; // fill the histogram
    
  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputEWKnoH.size(); iEntry++)
    sample_EWK_WW2j_noH->Add ((InputBaseDirectory+"/"+InputEWKnoH.at(iEntry)+"/*1.root").c_str()) ;

  int totEvents_EWK_WW2j_noH        = sample_EWK_WW2j_noH->GetEntries();
  std::cout<<"totEvents_EWK_WW2j_noH "<<totEvents_EWK_WW2j_noH<<std::endl;
  readTree* fReader_EWK_WW2j_noH  = new readTree((TTree*)(sample_EWK_WW2j_noH));

  analysisPlots.copySampleStructure ("EWK_WW2j_noH","EWK_WW2j_126", XS_EWK_noH, totEvents_EWK_WW2j_noH, 1,4);
  fillHistos (analysisPlots, fReader_EWK_WW2j_noH, "EWK_WW2j_noH") ; // fill the histogram

  
  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputVVQCD.size(); iEntry++)
    sample_QCD_WW2j->Add ((InputBaseDirectory+"/"+InputVVQCD.at(iEntry)+"/*1.root").c_str()) ;

  int totEvents_QCD_WW2j = sample_QCD_WW2j->GetEntries();
  std::cout<<"totEvents_QCD_WW2j "<<totEvents_QCD_WW2j<<std::endl;

  readTree* fReader_QCD_WW2j  = new readTree((TTree*)(sample_QCD_WW2j));

  analysisPlots.copySampleStructure ("QCD_WW2j","EWK_WW2j_126", XS_VV_QCD, totEvents_QCD_WW2j, 0, kGreen+1);

  fillHistos (analysisPlots, fReader_QCD_WW2j, "QCD_WW2j") ; // fill the histogram


  // WZ H126
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_WZ_EWK_H126 = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputWZEWKH126.size(); iEntry++)
    sample_WZ_EWK_H126->Add ((InputBaseDirectory+"/"+InputWZEWKH126.at(iEntry)+"/*1.root").c_str()) ;

  int totEvents_WZ_EWK_H126        = sample_WZ_EWK_H126->GetEntries();
  std::cout<<"totEvents_WZ_EWK_H126 "<<totEvents_WZ_EWK_H126<<std::endl;
  readTree* fReader_WZ_EWK_H126  = new readTree((TTree*)(sample_WZ_EWK_H126));

  analysisPlots.copySampleStructure ("WZ_EWK_H126","EWK_WW2j_126", XS_WZ_EWK_H126, totEvents_WZ_EWK_H126, 0, kAzure+1);

  fillHistos (analysisPlots, fReader_WZ_EWK_H126, "WZ_EWK_H126") ; // fill the histogram


  // WZ QCD
  // ---- ---- ---- ---- ---- ---- ----
  TChain * sample_WZ_QCD = new TChain (treeName.c_str()) ;
  for(size_t iEntry = 0; iEntry < InputWZQCD.size(); iEntry++)
    sample_WZ_QCD->Add ((InputBaseDirectory+"/"+InputWZQCD.at(iEntry)+"/*1.root").c_str()) ;

  int totEvents_WZ_QCD = sample_WZ_QCD->GetEntries();
  std::cout<<"totEvents_WZ_QCD "<<totEvents_WZ_QCD<<std::endl;
  readTree* fReader_WZ_QCD  = new readTree((TTree*)(sample_WZ_QCD));

  analysisPlots.copySampleStructure ("WZ_QCD","EWK_WW2j_126", XS_WZ_QCD, totEvents_WZ_QCD, 0, kOrange);

  fillHistos (analysisPlots, fReader_WZ_QCD, "WZ_QCD") ; // fill the histogram
      
  // plotting
  // ---- ---- ---- ---- ---- ---- ----
  analysisPlots.setPoissonErrors () ;
  analysisPlots.plotRelativeExcessFullLayer ("total", outputPlotDirectory) ;
  analysisPlots.plotRelativeExcessFullLayer ("looseVBF", outputPlotDirectory) ;
  analysisPlots.plotRelativeExcessFullLayer ("tightVBF", outputPlotDirectory) ;

}  


void fillHistos (plotter & analysisPlots, readTree* reader, const string sampleName) {
  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader->fChain->GetEntries() ;

  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){
      reader->fChain->GetEntry(iEvent) ;
      if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ; 

      if(reader->pt1 < 0          or reader->pt2 < 0)          continue ; // skip the event --> only two reco leptons are good
      if(reader->jetpt1 < 0       or reader->jetpt2 < 0)       continue ; // skip the event with less than two reco jet 
      if(reader->jetpt_puppi1 < 0 or reader->jetpt_puppi2 < 0) continue ; // skip the event with less than two reco jet 

      // read leptons, apply isolation and veto events with additional leptons                                                                                                             
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                                  
      float LEP_pt [4] ;
      float LEP_eta[4] ;
      float LEP_phi[4] ;
      float LEP_iso[4] ;
      fillRecoLeptonsArray (LEP_pt, LEP_eta, LEP_phi, LEP_iso, *reader) ;

      vector<TLorentzVector> TL_leptons ;
      dumpLeptons (TL_leptons, LEP_pt, LEP_eta, LEP_phi, LEP_iso, leptonIsoCut, minLeptonCutPt) ;
      if (TL_leptons.size () != 2) continue ;
      if(TL_leptons.at(0).Pt() < 20 or TL_leptons.at(1).Pt() < 15) continue ;

      // read jets, apply isolation and veto events with additional leptons                                                                                                                
      // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                                  
      // FIXME jets cleaning                                                                                                                                                               
      float REJ_pt  [8] ;
      float REJ_eta [8] ;
      float REJ_phi [8] ;
      float REJ_mass[8] ;
      float REJ_ID  [8] ;
      float REJ_btag[8] ;
      fillRecoJetArray (REJ_pt, REJ_eta, REJ_phi, REJ_mass, REJ_ID, REJ_btag, *reader) ;

      vector<TLorentzVector> TL_jets ;
      dumpJets (TL_jets, TL_leptons, REJ_pt, REJ_eta, REJ_phi, REJ_mass, REJ_btag, minJetCutPt, bTagCutVeto, minLeptonCleaningPt, matchingCone) ;

      if (TL_jets.size () < 2) continue ;

      // FIXME jets cleaning                                                                                                                                                               
      float REJ_pt_puppi  [8] ;
      float REJ_eta_puppi [8] ;
      float REJ_phi_puppi [8] ;
      float REJ_mass_puppi[8] ;
      float REJ_ID_puppi  [8] ;
      float REJ_btag_puppi[8] ;
      fillPuppiJetArray (REJ_pt_puppi, REJ_eta_puppi, REJ_phi_puppi, REJ_mass_puppi, REJ_ID_puppi, REJ_btag_puppi, *reader) ;

      vector<TLorentzVector> TL_jets_puppi ;
      dumpJets (TL_jets_puppi, TL_leptons, REJ_pt_puppi, REJ_eta_puppi, REJ_phi_puppi, REJ_mass_puppi, REJ_btag_puppi, minJetCutPt, bTagCutVeto, minLeptonCleaningPt, matchingCone);
      
      if (TL_jets_puppi.size () < 2) continue ;

      // FIXME jets cleaning                                                                                                                                                               
      float REJ_pt_gen  [4] ;
      float REJ_eta_gen [4] ;
      float REJ_phi_gen [4] ;
      float REJ_mass_gen[4] ;
      float REJ_ID_gen  [4] ;
      float REJ_btag_gen[4] ;
      fillGenJetArray (REJ_pt_gen, REJ_eta_gen, REJ_phi_gen, REJ_mass_gen, REJ_ID_gen, REJ_btag_gen, *reader) ;
      vector<TLorentzVector> TL_jets_gen ;

      dumpJets (TL_jets_gen, TL_leptons, REJ_pt_gen, REJ_eta_gen, REJ_phi_gen, REJ_mass_gen, REJ_btag_gen, minJetCutPt, bTagCutVeto,  minLeptonCleaningPt, matchingCone,4) ;

      if (TL_jets_gen.size () < 2) continue ;
      /*
      TLorentzVector L_dilepton = TL_leptons.at(0) + TL_leptons.at(1) ;

      TLorentzVector L_met;
      L_met.SetPtEtaPhiM (reader->pfmet,0.,reader->pfmetphi, 0.) ;
      TLorentzVector L_gen_met;
      L_gen_met.SetPtEtaPhiM (reader->metGenpt,0.,reader->metGenphi, 0.) ;
      TLorentzVector L_puppi_met;
      L_puppi_met.SetPtEtaPhiM (reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;

      TLorentzVector L_dijet       = TL_jets.at(0) + TL_jets.at(1);
      TLorentzVector L_dijet_gen   = TL_jets_gen.at(0) + TL_jets_gen.at(1);
      TLorentzVector L_dijet_puppi = TL_jets_puppi.at(0) + TL_jets_puppi.at(1) ;
   
      float asimJ = (TL_jets.at(0).Pt()-TL_jets.at(1).Pt())/(TL_jets.at(0).Pt()+TL_jets.at(1).Pt()) ;
      float asimL = (TL_leptons.at(0).Pt()-TL_leptons.at(1).Pt())/(TL_leptons.at(0).Pt ()+TL_leptons.at(1).Pt()) ;
      float Rvar  = (TL_leptons.at(0).Pt()*TL_leptons.at(1).Pt())/(TL_jets.at(0).Pt()*TL_jets.at(1).Pt()) ;

      float asimPuppiJ =  (TL_jets_puppi.at(0).Pt () - TL_jets_puppi.at(1).Pt ()) / (TL_jets_puppi.at(0).Pt () + TL_jets_puppi.at(1).Pt ()) ;
      float RvarPuppi  =  TL_leptons.at(0).Pt() * TL_leptons.at(1).Pt () / (TL_jets_puppi.at(0).Pt () * TL_jets_puppi.at(1).Pt ()) ;

      float asimGenJ  = (TL_jets_gen.at(0).Pt () - TL_jets_gen.at(1).Pt ()) / (TL_jets_gen.at(0).Pt () + TL_jets_gen.at(1).Pt ()) ;
      float RvarGen   =  TL_leptons.at(0).Pt () * TL_leptons.at(1).Pt () / (TL_jets_gen.at(0).Pt () * TL_jets_gen.at(1).Pt ()) ;
      
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LL}",   fabs (TL_leptons.at(0).DeltaPhi (TL_leptons.at(1))), 1.) ;
  
      analysisPlots.fillHisto (sampleName, "total", "M_{jj}",           L_dijet.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "M_{jj}^{gen}",     L_dijet_gen.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "M_{jj}^{puppi}",   L_dijet_puppi.M(), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#eta_{jj}",         fabs(TL_jets.at(0).Eta()-TL_jets.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "total", "#Delta#eta_{jj}^{gen}",   fabs(TL_jets_gen.at(0).Eta()-TL_jets_gen.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "total", "#Delta#eta_{jj}^{puppi}", fabs(TL_jets_puppi.at(0).Eta()-TL_jets_puppi.at(1).Eta()), 1.);

      analysisPlots.fillHisto (sampleName, "total", "M_{ll}",        L_dilepton.M(), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "Asim_{J}", asimJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "Asim_{L}", asimL, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "R",        Rvar, 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "Asim_{J}^{puppi}", asimPuppiJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "R^{puppi}",        RvarPuppi, 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "Asim_{J}^{gen}", asimGenJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "R^{gen}",        RvarGen, 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "pt_{l1}",      TL_leptons.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{l2}",      TL_leptons.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "met",          L_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "met^{gen}",    L_gen_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "met^{puppi}",  L_puppi_met.Pt(), 1) ;

      analysisPlots.fillHisto (sampleName, "total", "pt_{j1}^{puppi}", TL_jets_puppi.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{j2}^{puppi}", TL_jets_puppi.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{j1}",         TL_jets.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{j2}",         TL_jets.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{j1}^{gen}",   TL_jets_gen.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "total", "pt_{j2}^{gen}",   TL_jets_gen.at(1).Pt(), 1) ;
   
      /*
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LMet}", fabs (TL_leptons.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LMet^{puppi}}", fabs (TL_leptons.at(0).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LMet^{gen}}", fabs (TL_leptons.at(0).DeltaPhi (L_gen_met)), 1.) ;

      /*
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LLMet}", fabs (L_dilepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LLMet^{puppi}}", fabs (L_dilepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LLMet^{gen}}", fabs (L_dilepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{TLMet}", fabs (TL_leptons.at(1).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{TLMet^{puppi}}", fabs (TL_leptons.at(1).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{TLMet^{gen}}", fabs (TL_leptons.at(1).DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ}",         fabs (TL_leptons.at(0).DeltaPhi (TL_jets.at(1))), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{puppi}}", fabs (TL_jets_puppi.at(0).DeltaPhi (TL_jets_puppi.at(1)t)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{gen}}",   fabs (TL_jets_gen.at(0).DeltaPhi (TL_jets_gen.at(1))), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJMet}",          fabs (L_dijet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{gen}Met}",    fabs (L_dijet_gen.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{puppi}Met}",  fabs (L_dijet_puppi.DeltaPhi (L_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJMet^{puppi}}",        fabs (L_dijet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{gen}Met^{puppi}}",  fabs (L_dijet_gen.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{puppi}Met^{puppi}}", fabs (L_dijet_puppi.DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJMet^{gen}}",         fabs (L_dijet.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{gen}Met^{gen}}",   fabs (L_dijet_gen.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{JJ^{puppi}Met^{gen}}", fabs (L_dijet_puppi.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LJMet}",      fabs (TL_leptons.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LJMet^{puppi}}", fabs (L_lead_jet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LJ^{puppi}Met}", fabs (TL_jets_puppi.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{LJ^{puppi}Met^{puppi}}", fabs (TL_jets_puppi.at(0).DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "total", "#Delta#phi_{TJMet}",      fabs (TL_jets.at(1).DeltaPhi (L_met)), 1.) ;
      */
      /*
      if(L_dijet.M() < 350 or fabs(TL_jets.at(0).Eta()-TL_jets.at(1).Eta()) < 2.5 ) continue ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LL}",         fabs (TL_leptons.at(0).DeltaPhi (TL_leptons.at(1))), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#eta_{jj}",         fabs(TL_jets.at(0).Eta()-TL_jets.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#eta_{jj}^{gen}",   fabs(TL_jets_gen.at(0).Eta()-TL_jets_gen.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#eta_{jj}^{puppi}", fabs(TL_jets_puppi.at(0).Eta()-TL_jets_puppi.at(1).Eta()), 1.);

      analysisPlots.fillHisto (sampleName, "looseVBF", "M_{jj}",           L_dijet.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "M_{jj}^{gen}",     L_dijet_gen.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "M_{jj}^{puppi}",   L_dijet_puppi.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "M_{ll}",           L_dilepton.M(), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "Asim_{J}", asimJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "Asim_{L}", asimL, 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "R", Rvar, 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "Asim_{J}^{puppi}", asimPuppiJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "R^{puppi}", RvarPuppi, 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "Asim_{J}^{gen}", asimGenJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "R^{gen}", RvarGen, 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{l1}",      TL_leptons.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{l2}",      TL_leptons.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "met",       L_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "met^{gen}",    L_gen_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "met^{puppi}",  L_puppi_met.Pt(), 1) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j1}^{puppi}", TL_jets_puppi.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j2}^{puppi}", TL_jets_puppi.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j1}",       TL_jets.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j2}",       TL_jets.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j1}^{gen}",   TL_jets_gen.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "pt_{j2}^{gen}",   TL_jets_gen.at(1).Pt(), 1) ;
        /*
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LMet", fabs (TL_leptons.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LPuppiMet", fabs (TL_leptons.at(0).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LGenMet", fabs (TL_leptons.at(0).DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LLMet", fabs (L_dilepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LLPuppiMet", fabs (L_dilepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LLGenMet", fabs (L_dilepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{TLMet", fabs (TL_leptons.at(1).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{TLPuppiMet", fabs (TL_leptons.at(1).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{TLGenMet", fabs (TL_leptons.at(1).DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{JJ",      fabs (L_lead_jet.DeltaPhi (TL_jets.at(1))), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{PuppiJJ", fabs (TL_jets_puppi.at(0).DeltaPhi (TL_jets_puppi.at(1)t)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{GenJJ",   fabs (TL_jets_gen.at(0).DeltaPhi (TL_jets_gen.at(1))), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{JJMet",      fabs (L_dijet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{GenJJMet",      fabs (L_dijet_gen.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{PuppiJJMet",      fabs (L_dijet_puppi.DeltaPhi (L_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{JJPuppiMet",      fabs (L_dijet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{GenJJPuppiMet",   fabs (L_dijet_gen.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{PuppiJJPuppiMet", fabs (L_dijet_puppi.DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{JJGenMet",      fabs (L_dijet.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{GenJJGenMet",   fabs (L_dijet_gen.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{PuppiJJGenMet", fabs (L_dijet_puppi.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LJMet",      fabs (L_lead_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LJPuppiMet", fabs (L_lead_jet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LPuppiJMet", fabs (TL_jets_puppi.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{LPuppiJPuppiMet", fabs (TL_jets_puppi.at(0).DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "looseVBF", "#Delta#phi_{TJMet",      fabs (TL_jets.at(1).DeltaPhi (L_met)), 1.) ;
	*/
      /*      
      if(L_dijet.M() < 800 or fabs(TL_jets.at(0).Eta()-TL_jets.at(1).Eta() ) < 4.0 ) continue ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LL}",   fabs (TL_leptons.at(0).DeltaPhi (TL_leptons.at(1))), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#eta_{jj}",         fabs(TL_jets.at(0).Eta()-TL_jets.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#eta_{jj}^{gen}",   fabs(TL_jets_gen.at(0).Eta()-TL_jets_gen.at(1).Eta()), 1.);
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#eta_{jj}^{puppi}", fabs(TL_jets_puppi.at(0).Eta()-TL_jets_puppi.at(1).Eta()), 1.);
      
      analysisPlots.fillHisto (sampleName, "tightVBF", "M_{jj}",           L_dijet.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "M_{jj}^{gen}",     L_dijet_gen.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "M_{jj}^{puppi}",   L_dijet_puppi.M(), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "M_{ll}",           L_dilepton.M(), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "Asim_{J}", asimJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "Asim_{L}", asimL, 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "R",        Rvar, 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "Asim_{J}^{puppi}", asimPuppiJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "R^{puppi}",        RvarPuppi, 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "Asim_{J}^{gen}", asimGenJ, 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "R^{gen}",        RvarGen, 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{l1}",      TL_leptons.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{l2}",      TL_leptons.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "met",          L_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "met^{gen}",    L_gen_met.Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "met^{puppi}",  L_puppi_met.Pt(), 1) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j1}^{puppi}", TL_jets_puppi.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j2}^{puppi}", TL_jets_puppi.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j1}",         TL_jets.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j2}",         TL_jets.at(1).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j1}^{gen}",   TL_jets_gen.at(0).Pt(), 1) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "pt_{j2}^{gen}",   TL_jets_gen.at(1).Pt(), 1) ;
      /*
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LMet", fabs (TL_leptons.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LPuppiMet", fabs (TL_leptons.at(0).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LGenMet", fabs (TL_leptons.at(0).DeltaPhi (L_gen_met)), 1.) ;
      

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LLMet", fabs (L_dilepton.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LLPuppiMet", fabs (L_dilepton.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LLGenMet", fabs (L_dilepton.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{TLMet", fabs (TL_leptons.at(1).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{TLPuppiMet", fabs (TL_leptons.at(1).DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{TLGenMet", fabs (TL_leptons.at(1).DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{JJ",      fabs (L_lead_jet.DeltaPhi (TL_jets.at(1))), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{PuppiJJ", fabs (TL_jets_puppi.at(0).DeltaPhi (TL_jets_puppi.at(1)t)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{GenJJ",   fabs (TL_jets_gen.at(0).DeltaPhi (TL_jets_gen.at(1))), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{JJMet",      fabs (L_dijet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{GenJJMet",      fabs (L_dijet_gen.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{PuppiJJMet",      fabs (L_dijet_puppi.DeltaPhi (L_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{JJPuppiMet",      fabs (L_dijet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{GenJJPuppiMet",   fabs (L_dijet_gen.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{PuppiJJPuppiMet", fabs (L_dijet_puppi.DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{JJGenMet",      fabs (L_dijet.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{GenJJGenMet",   fabs (L_dijet_gen.DeltaPhi (L_gen_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{PuppiJJGenMet", fabs (L_dijet_puppi.DeltaPhi (L_gen_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LJMet",      fabs (L_lead_jet.DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LJPuppiMet", fabs (L_lead_jet.DeltaPhi (L_puppi_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LPuppiJMet", fabs (TL_jets_puppi.at(0).DeltaPhi (L_met)), 1.) ;
      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{LPuppiJPuppiMet", fabs (TL_jets_puppi.at(0).DeltaPhi (L_puppi_met)), 1.) ;

      analysisPlots.fillHisto (sampleName, "tightVBF", "#Delta#phi_{TJMet",  fabs (TL_jets.at(1).DeltaPhi (L_met)), 1.) ;
      */
  }
  // loop over events

  analysisPlots.setPoissonErrors() ;
  return ;
}

