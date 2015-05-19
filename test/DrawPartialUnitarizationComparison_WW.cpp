//////////////////////////////////////////////////
// compare or just make LHE level distributions //
//////////////////////////////////////////////////
#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TPad.h"

#include "plotter.h"
#include "ConfigParser.h"
#include "readTree.h"
#include "utils.h"

using namespace std ;

float matchingCone ;
float minLeptonCleaningPt;
float minLeptonCutPt;
float minJetCutPt;
float leptonIsoCut_mu;
float leptonIsoCut_el;
float leptonIsoCutLoose;
bool  usePuppiAsDefault;
int   lheLevelFilter;

string finalStateString ;

// no cuts are possible, all the variable to be plotted are given from an external file.

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main (int argc, char ** argv) {

  // check number of inpt parameters
  if(argc < 2){
    cerr<<"how to use it : ./bin/DrawLHEPlots <cfg file> "<<endl;
    return -1;
  }

  // Set Root style from global enviroment path                                                                                                                               
  string ROOTStyle;
  if(getenv ("ROOTStyle")!=NULL){
    ROOTStyle = getenv ("ROOTStyle");
    gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
  }

  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetErrorX(0.5);

  // parse config file parameter
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();

  TString config ; 
  config.Form("%s",argv[1]);

  if(!(gConfigParser->init(config))){
      cout << ">>> parseConfigFile::Could not open configuration file " << config << endl;
      return -1;
  }

  // import base directory where samples are located and txt file with the directory name + other info (higgs, noH and rescaled H)
  string InputBaseDirectoryH    = gConfigParser -> readStringOption("Input::InputBaseDirectoryH");
  string InputBaseDirectoryNoH  = gConfigParser -> readStringOption("Input::InputBaseDirectoryNoH");
  string InputBaseDirectoryH1   = gConfigParser -> readStringOption("Input::InputBaseDirectoryH1");
  string InputBaseDirectoryH2   = gConfigParser -> readStringOption("Input::InputBaseDirectoryH2");

  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

  // import from cfg file the cross section value for this sample                                                                                                               
  float CrossSectionH     = gConfigParser -> readFloatOption("Input::CrossSectionH");
  float CrossSectionNoH   = gConfigParser -> readFloatOption("Input::CrossSectionNoH");
  float CrossSectionH1    = gConfigParser -> readFloatOption("Input::CrossSectionH1");
  float CrossSectionH2    = gConfigParser -> readFloatOption("Input::CrossSectionH2");

  // take the cut list                                                                                                                                                          
  string InputCutList  = gConfigParser -> readStringOption("Input::InputCutList");

  // Read the cut file                                                                                                                                                          
  vector <cutContainerWW> CutList;
  if(ReadInputCutFile(InputCutList,CutList) <= 0){
    cerr<<" Empty Cut List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the variable list to be plotted
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters                                                                                                                                            
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)                                                                                                
  lumi *= 1000. ;   // transform into pb^(-1) 

  // select lepton flavour final state
  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone");
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt");
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");
  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());

  ///// Start the analysis : inport chains                                                                                                                              
  TChain* chainH = new TChain (treeName.c_str()) ;  
  chainH->Add ((InputBaseDirectoryH+"/*.root").c_str()) ;
  int totEventH = chainH->GetEntries();

  readTree* readerH  = new readTree((TTree*)(chainH));

  cout<<"H sample : Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventH<<" cross section "<<CrossSectionH<<" Nevents before selections "<<lumi*CrossSectionH<<" weight "<<lumi*CrossSectionH/float(totEventH)<<endl;

  float weightH = 1.0*lumi*CrossSectionH/float(totEventH) ;

  TChain* chainNoH = new TChain (treeName.c_str()) ;  
  chainNoH->Add ((InputBaseDirectoryNoH+"/*.root").c_str()) ;
  int totEventNoH = chainNoH->GetEntries();

  readTree* readerNoH  = new readTree((TTree*)(chainNoH));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventNoH<<" cross section "<<CrossSectionNoH<<" Nevents before selections "<<lumi*CrossSectionNoH<<" weight "<<lumi*CrossSectionNoH/float(totEventNoH)<<endl;

  float weightNoH = 1.0*lumi*CrossSectionNoH/float(totEventNoH) ;

  TChain* chainH1 = new TChain (treeName.c_str()) ;  
  chainH1->Add ((InputBaseDirectoryH1+"/*.root").c_str()) ;
  int totEventH1 = chainH1->GetEntries();

  readTree* readerH1  = new readTree((TTree*)(chainH1));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventH1<<" cross section "<<CrossSectionH1<<" Nevents before selections "<<lumi*CrossSectionH1<<" weight "<<lumi*CrossSectionH1/float(totEventH1)<<endl;

  float weightH1 = 1.0*lumi*CrossSectionH1/float(totEventH1) ;

  TChain* chainH2 = new TChain (treeName.c_str()) ;  
  chainH2->Add ((InputBaseDirectoryH2+"/*.root").c_str()) ;
  int totEventH2 = chainH2->GetEntries();

  readTree* readerH2  = new readTree((TTree*)(chainH2));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventH2<<" cross section "<<CrossSectionH2<<" Nevents before selections "<<lumi*CrossSectionH2<<" weight "<<lumi*CrossSectionH2/float(totEventH2)<<endl;

  float weightH2 = 1.0*lumi*CrossSectionH2/float(totEventH2) ;

  // declare efficiecny maps
  map<string,TH1F*> histoCutEffH ;
  map<string,TH1F*> histoCutEffNoH ;
  map<string,TH1F*> histoCutEffH1 ;
  map<string,TH1F*> histoCutEffH2 ;

  // make the plot container 
  vector<histoContainer> plotVectorH; 

  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffH["H126_pos_0_"+CutList.at(iCut).cutLayerName] = new TH1F(("H126_pos_0_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffH["H126_pos_0_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorH.push_back(histoContainer("H126_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  vector<histoContainer> plotVectorNoH;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffNoH["NoH_pos_0_"+CutList.at(iCut).cutLayerName] = new TH1F(("noH_pos_0_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffNoH["NoH_pos_0_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorNoH.push_back(histoContainer("NoH_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }
  
  vector<histoContainer> plotVectorH1;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffH1["H1_pos_0_"+CutList.at(iCut).cutLayerName] = new TH1F(("H1_pos_0_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffH1["H1_pos_0_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorH1.push_back(histoContainer("H1_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  vector<histoContainer> plotVectorH2;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffH2["H2_pos_0_"+CutList.at(iCut).cutLayerName] = new TH1F(("H2_pos_0_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffH2["H2_pos_0_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorH2.push_back(histoContainer("H2_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }
  

  // Loop on the events
  for(int iEventH = 0; iEventH < chainH->GetEntries(); iEventH++){

    readerH->fChain->GetEntry(iEventH) ;

    if (iEventH % 100000 == 0) cout << "reading event phantom H: " << iEventH << "\n" ;

    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events                                                                                                                                                         
      string name = "H126";

      if(!passCutContainerSelection(readerH,
                                    CutList.at(iCut),
                                    name,
				    0,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffH,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerH);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerH->pfmet,0.,readerH->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerH->pfmet_puppi,0.,readerH->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerH) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerH) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }


      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "H126_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorH.begin(),plotVectorH.end(),tmpPlot);
	if(itVec == plotVectorH.end()){
	  cerr<<"Problem -->plot not found for H "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightH) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightH) ;
	}

	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightH) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightH) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightH) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightH) ;
	}

        else if(variableList.at(iVar).variableName == "ptTJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(1).jet4V_).Pt(),weightH) ;
	}

        else if(variableList.at(iVar).variableName == "ptJJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weightH) ;
	}

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightH) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightH) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightH) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightH) ;
	}
      }
    }          
  }



  // Loop on the events
  for(int iEventNoH = 0; iEventNoH < chainNoH->GetEntries(); iEventNoH++){

    readerNoH->fChain->GetEntry(iEventNoH) ;

    if (iEventNoH % 100000 == 0) cout << "reading event phantom NoH: " << iEventNoH << "\n" ;

    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events                                                                                                                                                         
      string name = "NoH";

      if(!passCutContainerSelection(readerNoH,
                                    CutList.at(iCut),
                                    name,
				    0,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffNoH,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerNoH);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerNoH->pfmet,0.,readerNoH->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerNoH->pfmet_puppi,0.,readerNoH->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerNoH) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerNoH) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }


      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "NoH_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorNoH.begin(),plotVectorNoH.end(),tmpPlot);
	if(itVec == plotVectorNoH.end()){
	  cerr<<"Problem -->plot not found for NoH "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightNoH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightNoH) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightNoH) ;
	}

	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightNoH) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightNoH) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightNoH) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightNoH) ;
	}

        else if(variableList.at(iVar).variableName == "ptTJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(1).jet4V_).Pt(),weightNoH) ;
	}

        else if(variableList.at(iVar).variableName == "ptJJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weightNoH) ;
	}

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightNoH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightNoH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightNoH) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightNoH) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightNoH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightNoH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightNoH) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightNoH) ;
	}
      }
    }          
  }

  

  // Loop on the events
  for(int iEventH1 = 0; iEventH1 < chainH1->GetEntries(); iEventH1++){

    readerH1->fChain->GetEntry(iEventH1) ;

    if (iEventH1 % 100000 == 0) cout << "reading event phantom H1: " << iEventH1 << "\n" ;

    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events                                                                                                                                                         
      string name = "H1";

      if(!passCutContainerSelection(readerH1,
                                    CutList.at(iCut),
                                    name,
				    0,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffH1,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerH1);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerH1->pfmet,0.,readerH1->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerH1->pfmet_puppi,0.,readerH1->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerH1) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerH1) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }


      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "H1_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorH1.begin(),plotVectorH1.end(),tmpPlot);
	if(itVec == plotVectorH1.end()){
	  cerr<<"Problem -->plot not found for H1 "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightH1) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightH1) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightH1) ;
	}

	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightH1) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightH1) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightH1) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightH1) ;
	}

        else if(variableList.at(iVar).variableName == "ptTJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(1).jet4V_).Pt(),weightH1) ;
	}

        else if(variableList.at(iVar).variableName == "ptJJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weightH1) ;
	}

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightH1) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightH1) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightH1) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightH1) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightH1) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightH1) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightH1) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightH1) ;
	}
      }
    }          
  }



  // Loop on the events
  for(int iEventH2 = 0; iEventH2 < chainH2->GetEntries(); iEventH2++){

    readerH2->fChain->GetEntry(iEventH2) ;

    if (iEventH2 % 100000 == 0) cout << "reading event phantom H2: " << iEventH2 << "\n" ;

    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events                                                                                                                                                         
      string name = "H2";

      if(!passCutContainerSelection(readerH2,
                                    CutList.at(iCut),
                                    name,
				    0,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffH2,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerH2);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerH2->pfmet,0.,readerH2->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerH2->pfmet_puppi,0.,readerH2->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerH2) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerH2) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }


      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "H2_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorH2.begin(),plotVectorH2.end(),tmpPlot);
	if(itVec == plotVectorH2.end()){
	  cerr<<"Problem -->plot not found for H2 "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightH2) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightH2) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightH2) ;
	}

	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightH2) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightH2) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightH2) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightH2) ;
	}

        else if(variableList.at(iVar).variableName == "ptTJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(1).jet4V_).Pt(),weightH2) ;
	}

        else if(variableList.at(iVar).variableName == "ptJJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weightH2) ;
	}

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightH2) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightH2) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightH2) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightH2) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightH2) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightH2) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightH2) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightH2) ;
	}
      }
    }          
  }

  // make the canvas and basic banners                                                                                                                                         
  TCanvas *cCanvas = new TCanvas("cCanvas","",1,52,550,550);
  cCanvas->SetTicks();
  cCanvas->SetFillColor(0);
  cCanvas->SetBorderMode(0);
  cCanvas->SetBorderSize(2);
  cCanvas->SetTickx(1);
  cCanvas->SetTicky(1);
  cCanvas->SetRightMargin(0.05);
  cCanvas->SetBottomMargin(0.12);
  cCanvas->SetFrameBorderMode(0);

  TLatex * tex = new TLatex(0.9,0.93," 14 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.045);
  tex->SetLineWidth(2);
  TLatex * tex2 = new TLatex(0.14,0.92,"Delphes");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.045);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.31,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.04);
  tex3->SetLineWidth(2);

  TLegend* legend = new TLegend(0.55,0.82,0.85,0.89);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.03);
  legend->SetTextFont(42);
  legend->SetNColumns(3);

  TLegend* legend2 = new TLegend(0.55,0.82,0.8,0.89); 
  legend2->SetBorderSize(0);
  legend2->SetFillColor(0);
  legend2->SetFillStyle(0);
  legend2->SetTextSize(0.03);
  legend2->SetTextFont(42);
  legend2->SetNColumns(2);
  
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
      histoContainer tmpPlotNoH;
      tmpPlotNoH.cutName = "NoH_"+CutList.at(iCut).cutLayerName;
      tmpPlotNoH.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecNoH ;
      itVecNoH = find(plotVectorNoH.begin(),plotVectorNoH.end(),tmpPlotNoH);
      if(itVecNoH == plotVectorNoH.end()){
	cerr<<"Problem -->plot not found for NoH :  "<<variableList.at(iVar).variableName<<endl;
      }
       
      itVecNoH->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecNoH->histogram->GetYaxis()->SetRangeUser(0.001,itVecNoH->histogram->GetMaximum()*1.1);
      itVecNoH->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecNoH->histogram->SetLineColor(kBlue);
      itVecNoH->histogram->SetMarkerColor(kBlue);
      itVecNoH->histogram->SetFillColor(kBlue);
      itVecNoH->histogram->SetFillStyle(3001);
      itVecNoH->histogram->SetLineStyle(1);
      itVecNoH->histogram->SetLineWidth(2);
      itVecNoH->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecNoH->histogram,"noH","l");


      histoContainer tmpPlotH;
      tmpPlotH.cutName = "H126_"+CutList.at(iCut).cutLayerName;
      tmpPlotH.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecH ;
      itVecH = find(plotVectorH.begin(),plotVectorH.end(),tmpPlotH);
      if(itVecH == plotVectorH.end()){
	cerr<<"Problem -->plot not found for H :  "<<variableList.at(iVar).variableName<<endl;
      }

       
      itVecH->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecH->histogram->GetYaxis()->SetRangeUser(0.001,itVecH->histogram->GetMaximum()*1.1);
      itVecH->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecH->histogram->SetLineColor(kRed);
      itVecH->histogram->SetMarkerColor(kRed);
      itVecH->histogram->SetFillColor(kRed);
      itVecH->histogram->SetFillStyle(3001);

      itVecH->histogram->SetLineStyle(1);
      itVecH->histogram->SetLineWidth(2);
      itVecH->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecH->histogram,"H126","l");

      histoContainer tmpPlotH1;
      tmpPlotH1.cutName = "H1_"+CutList.at(iCut).cutLayerName;
      tmpPlotH1.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecH1 ;
      itVecH1 = find(plotVectorH1.begin(),plotVectorH1.end(),tmpPlotH1);
      if(itVecH1 == plotVectorH1.end()){
	cerr<<"Problem -->plot not found for H1 :  "<<variableList.at(iVar).variableName<<endl;
      }

       
      itVecH1->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecH1->histogram->GetYaxis()->SetRangeUser(0.001,itVecH1->histogram->GetMaximum()*1.1);
      itVecH1->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecH1->histogram->SetLineColor(kBlack);
      itVecH1->histogram->SetMarkerColor(kBlack);
      itVecH1->histogram->SetFillColor(kBlack);
      itVecH1->histogram->SetFillStyle(3001);
      itVecH1->histogram->SetLineStyle(1);
      itVecH1->histogram->SetLineWidth(2);
      itVecH1->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecH1->histogram,"H126, c=0.5","l");



      histoContainer tmpPlotH2;
      tmpPlotH2.cutName = "H2_"+CutList.at(iCut).cutLayerName;
      tmpPlotH2.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecH2 ;
      itVecH2 = find(plotVectorH2.begin(),plotVectorH2.end(),tmpPlotH2);
      if(itVecH2 == plotVectorH2.end()){
	cerr<<"Problem -->plot not found for H2 :  "<<variableList.at(iVar).variableName<<endl;
      }

       
      itVecH2->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecH2->histogram->GetYaxis()->SetRangeUser(0.001,itVecH2->histogram->GetMaximum()*1.1);
      itVecH2->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecH2->histogram->SetLineColor(kGreen+1);
      itVecH2->histogram->SetMarkerColor(kGreen+1);
      itVecH2->histogram->SetFillColor(kGreen+1);
      itVecH2->histogram->SetFillStyle(3001);
      itVecH2->histogram->SetLineStyle(1);
      itVecH2->histogram->SetLineWidth(2);
      itVecH2->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecH2->histogram,"H126, c=0.9","l");


      itVecNoH->histogram->GetYaxis()->SetRangeUser(0.000001,max(itVecNoH->histogram->GetMaximum(),max(itVecH->histogram->GetMaximum(),itVecH1->histogram->GetMaximum()))*1.35);

      itVecNoH->histogram->Draw("hist");
      itVecH2->histogram->Draw("hist same");
      itVecH1->histogram->Draw("hist same");
      itVecH->histogram->Draw("hist same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".png").c_str(),"png");
 
      // difference absolute
      TH1F* diffnoHvsH = (TH1F*) itVecNoH->histogram->Clone((string(itVecNoH->histogram->GetName())+"_diff").c_str());
      diffnoHvsH->Add(itVecH->histogram,-1);

      diffnoHvsH->SetMarkerColor(kBlue);
      diffnoHvsH->SetLineColor(kBlue);
      diffnoHvsH->SetFillColor(kBlue);
      diffnoHvsH->SetFillStyle(3001);
      diffnoHvsH->Rebin(2);

      legend2->AddEntry(diffnoHvsH,"noH-H","l");

      TH1F* diffH1vsH = (TH1F*) itVecH1->histogram->Clone((string(itVecH1->histogram->GetName())+"_diff").c_str());
      diffH1vsH->Add(itVecH->histogram,-1);
      diffH1vsH->SetLineColor(kRed);
      diffH1vsH->SetMarkerColor(kRed);
      diffH1vsH->SetFillColor(kRed);
      diffH1vsH->SetFillStyle(3001);
      diffH1vsH->Rebin(2);

      legend2->AddEntry(diffH1vsH,"H(c=0.5)-H","l");

      TH1F* diffH2vsH = (TH1F*) itVecH2->histogram->Clone((string(itVecH2->histogram->GetName())+"_diff").c_str());
      diffH2vsH->Add(itVecH->histogram,-1);
      diffH2vsH->SetLineColor(kGreen+1);
      diffH2vsH->SetMarkerColor(kGreen+1);
      diffH2vsH->SetFillColor(kGreen+1);
      diffH2vsH->SetFillStyle(3001);
      diffH2vsH->Rebin(2);

      legend2->AddEntry(diffH2vsH,"H(c=0.9)-H","l");

      diffnoHvsH->GetYaxis()->SetRangeUser(min(diffnoHvsH->GetMinimum(),min(diffH1vsH->GetMinimum(),diffH2vsH->GetMinimum())),
					   max(diffnoHvsH->GetMaximum(),max(diffH2vsH->GetMaximum(),diffH2vsH->GetMaximum()))*1.4);

      diffnoHvsH->Draw("Ehisto");
      diffH1vsH->Draw("Ehisto same");
      diffH2vsH->Draw("Ehisto same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend2->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff.png").c_str(),"png");

      diffnoHvsH->Smooth(1,"R");
      diffH1vsH->Smooth(1,"R");
      diffH2vsH->Smooth(1,"R");

      diffnoHvsH->Draw("Ehisto");
      diffH1vsH->Draw("Ehisto same");
      diffH2vsH->Draw("Ehisto same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend2->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_smooth_diff.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_smooth_diff.png").c_str(),"png");

      /*
      // normalize
      float integral = itVecNoH->histogram->Integral();
      TH1F* htempNoH = (TH1F*) itVecNoH->histogram->Clone((string(itVecNoH->histogram->GetName())+"_norm").c_str());
      htempNoH->Scale(1./(integral));
      htempNoH->GetYaxis()->SetTitle("a.u.");

      integral = itVecH->histogram->Integral();
      TH1F* htempH = (TH1F*) itVecH->histogram->Clone((string(itVecH->histogram->GetName())+"_norm").c_str());
      htempH->Scale(1./(integral));
      htempH->GetYaxis()->SetTitle("a.u.");
 
      integral = itVecH1->histogram->Integral();
      TH1F* htempH1 = (TH1F*) itVecH1->histogram->Clone((string(itVecH1->histogram->GetName())+"_norm").c_str());
      htempH1->Scale(1./(integral));
      htempH1->GetYaxis()->SetTitle("a.u.");

      integral = itVecH2->histogram->Integral();
      TH1F* htempH2 = (TH1F*) itVecH2->histogram->Clone((string(itVecH2->histogram->GetName())+"_norm").c_str());
      htempH2->Scale(1./(integral));
      htempH2->GetYaxis()->SetTitle("a.u.");

      htempNoH->GetYaxis()->SetRangeUser(0.000001,max(htempNoH->GetMaximum(),max(htempH->GetMaximum(),htempH1->GetMaximum()))*1.35);
      htempNoH->Draw("hist");
      htempH->Draw("hist same");
      htempH1->Draw("hist same");
      htempH2->Draw("hist same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend->Draw("same");


      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm.png").c_str(),"png");
 
      // difference absolute
      integral = diffnoHvsH->Integral();
      diffnoHvsH->GetYaxis()->SetTitle("a.u");
      diffnoHvsH->Scale(1./integral);
      integral = diffH1vsH->Integral();
      diffH1vsH->Scale(1./integral);
      integral = diffH2vsH->Integral();
      diffH2vsH->Scale(1./integral);

      diffnoHvsH->SetMarkerColor(kBlue);
      diffnoHvsH->SetLineColor(kBlue);
      diffnoHvsH->SetFillColor(kBlue);
      diffnoHvsH->SetFillStyle(3001);

      diffH1vsH->SetMarkerColor(kRed);
      diffH1vsH->SetLineColor(kRed);
      diffH1vsH->SetFillColor(kRed);
      diffH1vsH->SetFillStyle(3001);

      diffH2vsH->SetMarkerColor(kGreen+1);
      diffH2vsH->SetLineColor(kGreen+1);
      diffH2vsH->SetFillColor(kGreen+1);
      diffH2vsH->SetFillStyle(3001);


      diffnoHvsH->GetYaxis()->SetRangeUser(min(diffnoHvsH->GetMinimum(),diffH1vsH->GetMinimum()),max(diffnoHvsH->GetMaximum(),diffH1vsH->GetMaximum())*1.4);
      diffnoHvsH->Draw("Ehisto");
      diffH1vsH->Draw("Ehisto same");
      diffH2vsH->Draw("Ehisto same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend2->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm.png").c_str(),"png");
      */

      legend->Clear();
      legend2->Clear();

    }
  }
  
 
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffH.begin(); itMap !=  histoCutEffH.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffNoH.begin(); itMap !=  histoCutEffNoH.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffH1.begin(); itMap !=  histoCutEffH1.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffH2.begin(); itMap !=  histoCutEffH2.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();

  //Normalize histograms                                                                                                                                                        
  for(size_t ihisto = 0; ihisto < plotVectorH.size(); ihisto++){
    if(plotVectorH.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo H "<<plotVectorH.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorH.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorH.at(ihisto).histogram->Integral(0,plotVectorH.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  for(size_t ihisto = 0; ihisto < plotVectorNoH.size(); ihisto++){
    if(plotVectorNoH.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo NoH "<<plotVectorNoH.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorNoH.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorNoH.at(ihisto).histogram->Integral(0,plotVectorNoH.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  for(size_t ihisto = 0; ihisto < plotVectorH1.size(); ihisto++){
    if(plotVectorH1.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo H1 "<<plotVectorH1.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorH1.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorH1.at(ihisto).histogram->Integral(0,plotVectorH1.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  for(size_t ihisto = 0; ihisto < plotVectorH2.size(); ihisto++){
    if(plotVectorH2.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo H2 "<<plotVectorH2.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorH2.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorH2.at(ihisto).histogram->Integral(0,plotVectorH2.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  return 0 ;
}  


