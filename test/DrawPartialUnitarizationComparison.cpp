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

double matchingCone ;
double minLeptonCleaningPt;
double minLeptonCutPt;
double minJetCutPt;
double leptonIsoCut_mu;
double leptonIsoCut_el;
double leptonIsoCutLoose;
bool   usePuppiAsDefault;
int    lheLevelFilter;

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
  string InputBaseDirectoryCxH  = gConfigParser -> readStringOption("Input::InputBaseDirectoryCxH");

  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

  // import from cfg file the cross section value for this sample                                                                                                               
  float CrossSectionH     = gConfigParser -> readFloatOption("Input::CrossSectionH");
  float CrossSectionNoH   = gConfigParser -> readFloatOption("Input::CrossSectionNoH");
  float CrossSectionCxH   = gConfigParser -> readFloatOption("Input::CrossSectionCxH");

  // take the cut list                                                                                                                                                          
  string InputCutList  = gConfigParser -> readStringOption("Input::InputCutList");

  // Read the cut file                                                                                                                                                          
  vector <cutContainer> CutList;
  if(ReadInputCutFile(InputCutList,CutList) <= 0){
    cerr<<" Empty Cut List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the variable list to be plotted
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters                                                                                                                                            
  double lumi  =  gConfigParser -> readDoubleOption("Option::Lumi"); // fb^(-1)                                                                                                
  lumi *= 1000. ;   // transform into pb^(-1) 

  // select lepton flavour final state
  matchingCone        = gConfigParser -> readDoubleOption("Option::matchingCone");
  minLeptonCleaningPt = gConfigParser -> readDoubleOption("Option::minLeptonCleaningPt");
  minLeptonCutPt      = gConfigParser -> readDoubleOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readDoubleOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readDoubleOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readDoubleOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readDoubleOption("Option::leptonIsoCutLoose");

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


  TChain* chainCxH = new TChain (treeName.c_str()) ;  
  chainCxH->Add ((InputBaseDirectoryCxH+"/*.root").c_str()) ;
  int totEventCxH = chainCxH->GetEntries();

  readTree* readerCxH  = new readTree((TTree*)(chainCxH));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventCxH<<" cross section "<<CrossSectionCxH<<" Nevents before selections "<<lumi*CrossSectionCxH<<" weight "<<lumi*CrossSectionCxH/float(totEventCxH)<<endl;

  float weightCxH = 1.0*lumi*CrossSectionCxH/float(totEventCxH) ;

  // declare efficiecny maps
  map<string,TH1F*> histoCutEffH ;
  map<string,TH1F*> histoCutEffNoH ;
  map<string,TH1F*> histoCutEffCxH ;

  // make the plot container 
  vector<histoContainer> plotVectorH;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffH["H126_"+CutList.at(iCut).cutLayerName] = new TH1F(("H126_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffH["H126_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorH.push_back(histoContainer("H126_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  vector<histoContainer> plotVectorNoH;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffNoH["NoH_"+CutList.at(iCut).cutLayerName] = new TH1F(("noH_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffNoH["NoH_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorNoH.push_back(histoContainer("NoH_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }
  
  vector<histoContainer> plotVectorCxH;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEffCxH["CxH_"+CutList.at(iCut).cutLayerName] = new TH1F(("CxH_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    histoCutEffCxH["CxH_"+CutList.at(iCut).cutLayerName]->Sumw2();
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorCxH.push_back(histoContainer("CxH_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
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
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffH)) continue;


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
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
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
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),weightH) ;
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


  // Loop on the events for noH sample
  for(int iEventNoH = 0; iEventNoH < chainNoH->GetEntries(); iEventNoH++){

    readerNoH->fChain->GetEntry(iEventNoH) ;

    if (iEventNoH % 100000 == 0) cout << "reading event NoH: " << iEventNoH << "\n" ;

    
    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){
      // cut the events                                                                                                                                                         
      string name = "NoH";
      if(!passCutContainerSelection(readerNoH,
                                    CutList.at(iCut),
                                    name,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffNoH)) continue;


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
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
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
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),weightNoH) ;
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


  // Loop on the events for rescaled H sample
  for(int iEventCxH = 0; iEventCxH < chainCxH->GetEntries(); iEventCxH++){

    readerCxH->fChain->GetEntry(iEventCxH) ;

    if (iEventCxH % 100000 == 0) cout << "reading event CxH: " << iEventCxH << "\n" ;

    
    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){
      // cut the events                                                                                                                                                         
      string name = "CxH";
      if(!passCutContainerSelection(readerCxH,
                                    CutList.at(iCut),
                                    name,
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEffCxH)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerCxH);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerCxH->pfmet,0.,readerCxH->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerCxH->pfmet_puppi,0.,readerCxH->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerCxH) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerCxH) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }

      
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "CxH_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorCxH.begin(),plotVectorCxH.end(),tmpPlot);
	if(itVec == plotVectorCxH.end()){
	  cerr<<"Problem -->plot not found for CxH "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightCxH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightCxH) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightCxH) ;
	}
	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightCxH) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightCxH) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightCxH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightCxH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightCxH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightCxH) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightCxH) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightCxH) ;
	}
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),weightCxH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightCxH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightCxH) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightCxH) ;
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

  TLegend* legend = new TLegend(0.2,0.78,0.85,0.89);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.03);
  legend->SetTextFont(42);
  legend->SetNColumns(3);

  TLegend* legend2 = new TLegend(0.3,0.78,0.8,0.89);
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
      itVecH->histogram->SetLineStyle(7);
      itVecH->histogram->SetLineWidth(2);
      itVecH->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecH->histogram,"H126","l");

      histoContainer tmpPlotCxH;
      tmpPlotCxH.cutName = "CxH_"+CutList.at(iCut).cutLayerName;
      tmpPlotCxH.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecCxH ;
      itVecCxH = find(plotVectorCxH.begin(),plotVectorCxH.end(),tmpPlotCxH);
      if(itVecCxH == plotVectorCxH.end()){
	cerr<<"Problem -->plot not found for CxH :  "<<variableList.at(iVar).variableName<<endl;
      }

       
      itVecCxH->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecCxH->histogram->GetYaxis()->SetRangeUser(0.001,itVecCxH->histogram->GetMaximum()*1.1);
      itVecCxH->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecCxH->histogram->SetLineColor(kBlack);
      itVecCxH->histogram->SetLineStyle(2);
      itVecCxH->histogram->SetLineWidth(2);
      itVecCxH->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      legend->AddEntry(itVecCxH->histogram,"H126, c=0.9","l");

      itVecNoH->histogram->GetYaxis()->SetRangeUser(0.000001,max(itVecNoH->histogram->GetMaximum(),max(itVecH->histogram->GetMaximum(),itVecCxH->histogram->GetMaximum()))*1.25);

      itVecNoH->histogram->Draw("hist");
      itVecH->histogram->Draw("hist same");
      itVecCxH->histogram->Draw("hist same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".png").c_str(),"png");
 
      //      cCanvas->SetLogy();
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
      
      //      cCanvas->SetLogy(0);
      //      gPad->Update();

      // difference absolute
      TH1F* diffnoHvsH = (TH1F*) itVecNoH->histogram->Clone((string(itVecNoH->histogram->GetName())+"_diff").c_str());
      diffnoHvsH->Add(itVecH->histogram,-1);

      diffnoHvsH->Rebin(2);
      diffnoHvsH->SetMarkerColor(kBlue);
      diffnoHvsH->SetLineColor(kBlue);
      diffnoHvsH->SetFillColor(kBlue);
      diffnoHvsH->SetFillStyle(3001);

      legend2->AddEntry(diffnoHvsH,"noH-H","l");

      TH1F* diffCxHvsH = (TH1F*) itVecCxH->histogram->Clone((string(itVecCxH->histogram->GetName())+"_diff").c_str());
      diffCxHvsH->Add(itVecH->histogram,-1);
      diffCxHvsH->Rebin(2);
      diffCxHvsH->SetLineColor(kRed);
      diffCxHvsH->SetMarkerColor(kRed);
      diffCxHvsH->SetFillColor(kRed);
      diffCxHvsH->SetFillStyle(3001);


      legend2->AddEntry(diffCxHvsH,"H(c=0.9)-H","l");

      diffnoHvsH->GetYaxis()->SetRangeUser(min(diffnoHvsH->GetMinimum(),diffCxHvsH->GetMinimum()),max(diffnoHvsH->GetMaximum(),diffCxHvsH->GetMaximum())*1.2);

      diffnoHvsH->Draw("E2");
      diffCxHvsH->Draw("E same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend2->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff.png").c_str(),"png");
 
      //      cCanvas->SetLogy();
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_log.pdf").c_str(),"pdf");
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_log.png").c_str(),"png");
      
      //      cCanvas->SetLogy(0);
      //      gPad->Update();

      // normalize

      float integral = itVecNoH->histogram->Integral();
      TH1F* htempNoH = (TH1F*) itVecNoH->histogram->Clone((string(itVecNoH->histogram->GetName())+"_norm").c_str());
      htempNoH->Scale(1./(integral));
      htempNoH->GetYaxis()->SetTitle("a.u.");

      integral = itVecH->histogram->Integral();
      TH1F* htempH = (TH1F*) itVecH->histogram->Clone((string(itVecH->histogram->GetName())+"_norm").c_str());
      htempH->Scale(1./(integral));
      htempH->GetYaxis()->SetTitle("a.u.");
 
      integral = itVecCxH->histogram->Integral();
      TH1F* htempCxH = (TH1F*) itVecCxH->histogram->Clone((string(itVecCxH->histogram->GetName())+"_norm").c_str());
      htempCxH->Scale(1./(integral));
      htempCxH->GetYaxis()->SetTitle("a.u.");

      htempNoH->GetYaxis()->SetRangeUser(0.000001,max(htempNoH->GetMaximum(),max(htempH->GetMaximum(),htempCxH->GetMaximum()))*1.25);
      htempNoH->Draw("hist");
      htempH->Draw("hist same");
      htempCxH->Draw("hist same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend->Draw("same");


      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm.png").c_str(),"png");
 
      //      cCanvas->SetLogy();
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm_log.pdf").c_str(),"pdf");
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_norm_log.png").c_str(),"png");
      
      //      cCanvas->SetLogy(0);
      //      gPad->Update();


      // difference absolute
      integral = diffnoHvsH->Integral();
      diffnoHvsH->GetYaxis()->SetTitle("a.u");
      diffnoHvsH->Scale(1./integral);
      integral = diffCxHvsH->Integral();
      diffCxHvsH->Scale(1./integral);

      diffnoHvsH->SetMarkerColor(kBlue);
      diffnoHvsH->SetLineColor(kBlue);
      diffnoHvsH->SetFillColor(kBlue);
      diffnoHvsH->SetFillStyle(3001);

      diffCxHvsH->SetMarkerColor(kRed);
      diffCxHvsH->SetLineColor(kRed);
      diffCxHvsH->SetFillColor(kRed);
      diffCxHvsH->SetFillStyle(3001);


      diffnoHvsH->GetYaxis()->SetRangeUser(min(diffnoHvsH->GetMinimum(),diffCxHvsH->GetMinimum()),max(diffnoHvsH->GetMaximum(),diffCxHvsH->GetMaximum())*1.2);
      diffnoHvsH->Draw("E2");
      diffCxHvsH->Draw("E same");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend2->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm.png").c_str(),"png");
 
      //      cCanvas->SetLogy();
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm_log.pdf").c_str(),"pdf");
      //      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_diff_norm_log.png").c_str(),"png");
      
      //      cCanvas->SetLogy(0);
      //      gPad->Update();
      
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

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffCxH.begin(); itMap !=  histoCutEffCxH.end(); itMap++){
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

  for(size_t ihisto = 0; ihisto < plotVectorCxH.size(); ihisto++){
    if(plotVectorCxH.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo CxH "<<plotVectorCxH.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorCxH.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorCxH.at(ihisto).histogram->Integral(0,plotVectorCxH.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }


  return 0 ;
}  


