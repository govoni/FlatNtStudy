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

bool   usePuppiAsDefault;

string  finalStateString;

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

  // import base directory where samples are located and txt file with the directory name + other info
  string InputBaseDirectoryMG  = gConfigParser -> readStringOption("Input::InputBaseDirectoryMG");
  string InputBaseDirectoryPH  = gConfigParser -> readStringOption("Input::InputBaseDirectoryPH");

  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

  // import from cfg file the cross section value for this sample                                                                                                               
  float CrossSectionMG   = gConfigParser -> readFloatOption("Input::CrossSectionMG");
  // import from cfg file the cross section value for this sample                                                                                                               
  float CrossSectionPH   = gConfigParser -> readFloatOption("Input::CrossSectionPH");

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
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)                                                                                                
  lumi *= 1000. ;   // transform into pb^(-1) 

  // select lepton flavour final state
  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone");
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt");
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());

  ///// Start the analysis                                                                                                                                                    
  map<string,TH1F*> histoCutEffMG ;
  map<string,TH1F*> histoCutEffPH ;


  TChain* chainMG = new TChain (treeName.c_str()) ;  
  chainMG->Add ((InputBaseDirectoryMG+"/*.root").c_str()) ;
  int totEventMG = chainMG->GetEntries();

  readTree* readerMG  = new readTree((TTree*)(chainMG));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventMG<<" cross section "<<CrossSectionMG<<" Nevents before selections "<<lumi*CrossSectionMG<<" weight "<<lumi*CrossSectionMG/float(totEventMG)<<endl;

  float weightMG = 1.0*lumi*CrossSectionMG/float(totEventMG) ;


  TChain* chainPH = new TChain (treeName.c_str()) ;  
  chainPH->Add ((InputBaseDirectoryPH+"/*.root").c_str()) ;
  int totEventPH = chainPH->GetEntries();

  readTree* readerPH  = new readTree((TTree*)(chainPH));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEventPH<<" cross section "<<CrossSectionPH<<" Nevents before selections "<<lumi*CrossSectionPH<<" weight "<<lumi*CrossSectionPH/float(totEventPH)<<endl;

  float weightPH = 1.0*lumi*CrossSectionPH/float(totEventPH) ;

  // make the plot container 
  vector<histoContainer> plotVectorMG;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorMG.push_back(histoContainer("MG_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  vector<histoContainer> plotVectorPH;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorPH.push_back(histoContainer("PH_"+CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }
  

  int passingLHEFilterMG = 0 ;
  int passingLHEFilterPH = 0 ;
  int passingLHEFilterMG_Pt = 0 ;
  int passingLHEFilterPH_Pt = 0 ;
  int passingLHEFilterMG_Deta = 0 ;
  int passingLHEFilterPH_Deta = 0 ;

  // Loop on the events
  for(int iEventMG = 0; iEventMG < chainMG->GetEntries(); iEventMG++){

    readerMG->fChain->GetEntry(iEventMG) ;

    if (iEventMG % 100000 == 0) cout << "reading event MG: " << iEventMG << "\n" ;

    // filter LHE level leptons
    if(finalStateString == "UU"){
      if(fabs(readerMG->leptonLHEpid1) != 13 or fabs(readerMG->leptonLHEpid2) != 13)
	continue;
    }
    else if(finalStateString == "EE"){      
      if(fabs(readerMG->leptonLHEpid1) != 11 or fabs(readerMG->leptonLHEpid2) != 11) continue;
    }
    else if(finalStateString == "EU"){
      if(fabs(readerMG->leptonLHEpid1) != 11 or fabs(readerMG->leptonLHEpid2) != 13) continue ;
    }
    else if(finalStateString == "UE"){
      if(fabs(readerMG->leptonLHEpid1) != 13 or fabs(readerMG->leptonLHEpid2) != 11) continue ;
    }
    else {
      cerr<<"problem with LHE level final state definition --> skip event"<<endl;
      continue;
    }

    passingLHEFilterMG++;

    // if an event pass the cut, fill the associated map
    TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2;

    L_lepton1.SetPtEtaPhiM(readerMG->leptonLHEpt1,readerMG->leptonLHEeta1,readerMG->leptonLHEphi1,readerMG->leptonLHEm1);
    L_lepton2.SetPtEtaPhiM(readerMG->leptonLHEpt2,readerMG->leptonLHEeta2,readerMG->leptonLHEphi2,readerMG->leptonLHEm2);

    L_parton1.SetPtEtaPhiM(readerMG->jetLHEPartonpt1,readerMG->jetLHEPartoneta1,readerMG->jetLHEPartonphi1,0.);
    L_parton2.SetPtEtaPhiM(readerMG->jetLHEPartonpt2,readerMG->jetLHEPartoneta2,readerMG->jetLHEPartonphi2,0.);

    if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;

    passingLHEFilterMG_Pt++;

    if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;

    passingLHEFilterMG_Deta++;
    
    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events                                                                                                                                                         
      string name = "MG";

      if(!passCutContainerSelection(readerMG,
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
                                    histoCutEffMG,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerMG);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerMG->pfmet,0.,readerMG->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerMG->pfmet_puppi,0.,readerMG->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerMG) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerMG) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }

      
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "MG_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorMG.begin(),plotVectorMG.end(),tmpPlot);
	if(itVec == plotVectorMG.end()){
	  cerr<<"Problem -->plot not found for MG "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etaj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "dRjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.DeltaR(RecoJets.at(1).jet4V_),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightMG) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etaj1etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "etal1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightMG) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etall" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "phill" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Phi(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "dRll" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "etal1etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Eta()*leptonsIsoTight.at(1).lepton4V_.Eta(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightMG) ;
	}
	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightMG) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "dR_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "dR_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightMG) ;
        }
	else if(variableList.at(iVar).variableName == "dR_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(L_dijet),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "dR_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),weightMG) ;
        }
	else if(variableList.at(iVar).variableName == "dR_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "dR_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(L_dijet),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightMG) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "dR_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(0).jet4V_),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "dR_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(1).jet4V_),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "dR_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(L_dijet),weightMG) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightMG) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightMG) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightMG) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightMG) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightMG) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightMG) ;
	}
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),weightMG) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightMG) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightMG) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightMG) ;
	}
      }
    }          
  }


  // Loop on the events
  for(int iEventPH = 0; iEventPH < chainPH->GetEntries(); iEventPH++){

    readerPH->fChain->GetEntry(iEventPH) ;

    if (iEventPH % 100000 == 0) cout << "reading event PH: " << iEventPH << "\n" ;

    // filter LHE level leptons
    if(finalStateString == "UU"){
      if(fabs(readerPH->leptonLHEpid1) != 13 or fabs(readerPH->leptonLHEpid2) != 13)
	continue;
    }
    else if(finalStateString == "EE"){      
      if(fabs(readerPH->leptonLHEpid1) != 11 or fabs(readerPH->leptonLHEpid2) != 11) continue;
    }
    else if(finalStateString == "EU"){
      if(fabs(readerPH->leptonLHEpid1) != 11 or fabs(readerPH->leptonLHEpid2) != 13) continue ;
    }
    else if(finalStateString == "UE"){
      if(fabs(readerPH->leptonLHEpid1) != 13 or fabs(readerPH->leptonLHEpid2) != 11) continue ;
    }
    else {
      cerr<<"problem with LHE level final state definition --> skip event"<<endl;
      continue;
    }



    passingLHEFilterPH++;

    // if an event pass the cut, fill the associated map
    TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2;

    L_lepton1.SetPtEtaPhiM(readerPH->leptonLHEpt1,readerPH->leptonLHEeta1,readerPH->leptonLHEphi1,readerPH->leptonLHEm1);
    L_lepton2.SetPtEtaPhiM(readerPH->leptonLHEpt2,readerPH->leptonLHEeta2,readerPH->leptonLHEphi2,readerPH->leptonLHEm2);

    L_parton1.SetPtEtaPhiM(readerPH->jetLHEPartonpt1,readerPH->jetLHEPartoneta1,readerPH->jetLHEPartonphi1,0.);
    L_parton2.SetPtEtaPhiM(readerPH->jetLHEPartonpt2,readerPH->jetLHEPartoneta2,readerPH->jetLHEPartonphi2,0.);

    if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;

    passingLHEFilterPH_Pt++;

    if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;

    passingLHEFilterPH_Deta++;
    
    // Loop  on the cut list --> one cut for each polarization                                                                                                                  
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){
      // cut the events                                                                                                                                                         
      string name = "PH";
      if(!passCutContainerSelection(readerPH,
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
                                    histoCutEffPH,
				    finalStateString)) continue;


      // if an event pass the cut, fill the associated map                                                                                                                      
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *readerPH);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (readerPH->pfmet,0.,readerPH->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (readerPH->pfmet_puppi,0.,readerPH->pfmetphi_puppi, 0.) ;

      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
        fillRecoJetArray (RecoJetsAll, *readerPH) ;
      else
        fillPuppiJetArray (RecoJetsAll, *readerPH) ;

      // take jets                                                                                                                                                              
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999., CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);
      
      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;
      
      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }

      
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = "PH_"+CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVectorPH.begin(),plotVectorPH.end(),tmpPlot);
	if(itVec == plotVectorPH.end()){
	  cerr<<"Problem -->plot not found for PH "<<"  "<<variableList.at(iVar).variableName<<endl;
	  continue ;
	}

        if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etaj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "dRjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.DeltaR(RecoJets.at(1).jet4V_),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,weightPH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etaj1etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "etal1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),weightPH) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etall" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "phill" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Phi(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "dRll" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "etal1etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Eta()*leptonsIsoTight.at(1).lepton4V_.Eta(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,weightPH) ;
	}
	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weightPH) ;
        }
        ///                                                                                                                                                                      
        else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "dR_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "dR_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),weightPH) ;
        }
	else if(variableList.at(iVar).variableName == "dR_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(L_dijet),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_)),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet)),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "dR_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),weightPH) ;
        }
	else if(variableList.at(iVar).variableName == "dR_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "dR_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(L_dijet),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_)),weightPH) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_)),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "dR_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(0).jet4V_),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "dR_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(1).jet4V_),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "dR_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(L_dijet),weightPH) ;
        }


        else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),weightPH) ;
	}
        else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),weightPH) ;
        }

        else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),weightPH) ;
        }
        else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),weightPH) ;
        }

        ///                                                                                                                                                                
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
	itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weightPH) ;
	}
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weightPH) ;
	}
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
	  itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),weightPH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet).M(),weightPH) ;
	}

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
	  itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weightPH) ;
	}

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
	  itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weightPH) ;
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

  TLatex * tex = new TLatex(0.88,0.92," 14 TeV");
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
  TLatex * tex3 = new TLatex(0.286,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.04);
  tex3->SetLineWidth(2);

  TLegend* legend = new TLegend(0.55,0.75,0.85,0.89);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
 
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
      histoContainer tmpPlotMG;
      tmpPlotMG.cutName = "MG_"+CutList.at(iCut).cutLayerName;
      tmpPlotMG.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecMG ;
      itVecMG = find(plotVectorMG.begin(),plotVectorMG.end(),tmpPlotMG);
      if(itVecMG == plotVectorMG.end()){
	cerr<<"Problem -->plot not found for MG :  "<<variableList.at(iVar).variableName<<endl;
      }
       
      itVecMG->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecMG->histogram->GetYaxis()->SetRangeUser(0.001,itVecMG->histogram->GetMaximum()*1.1);
      itVecMG->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecMG->histogram->SetLineColor(kBlue);
      itVecMG->histogram->SetLineStyle(1);
      itVecMG->histogram->SetLineWidth(2);
      itVecMG->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      itVecMG->histogram->Draw("hist");
      legend->AddEntry(itVecMG->histogram,"Madgraph","l");


      histoContainer tmpPlotPH;
      tmpPlotPH.cutName = "PH_"+CutList.at(iCut).cutLayerName;
      tmpPlotPH.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVecPH ;
      itVecPH = find(plotVectorPH.begin(),plotVectorPH.end(),tmpPlotPH);
      if(itVecPH == plotVectorPH.end()){
	cerr<<"Problem -->plot not found for PH :  "<<variableList.at(iVar).variableName<<endl;
      }

       
      itVecPH->histogram->GetXaxis()->SetTitleSize(0.04);
      itVecPH->histogram->GetYaxis()->SetRangeUser(0.001,itVecPH->histogram->GetMaximum()*1.1);
      itVecPH->histogram->GetYaxis()->SetTitleSize(0.05);

      itVecPH->histogram->SetLineColor(kRed);
      itVecPH->histogram->SetLineStyle(7);
      itVecPH->histogram->SetLineWidth(2);
      itVecPH->histogram->GetYaxis()->SetTitle("#sigma x lumi");

      itVecPH->histogram->Draw("hist same");
      legend->AddEntry(itVecPH->histogram,"Phantom","l");

      tex->Draw("same");
      tex2->Draw("same");
      tex3->Draw("same");
      legend->Draw("same");

      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".png").c_str(),"png");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+".root").c_str(),"root");

      cCanvas->SetLogy();
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
      cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList.at(iVar).variableName+"_log.root").c_str(),"root");
      
      cCanvas->SetLogy(0);
      gPad->Update();
      
      legend->Clear();
    }
  }
  
  cout<<"LHE filter efficiency MG : "<<passingLHEFilterMG<<" totEvent "<<totEventMG<<" efficiency "<<float(passingLHEFilterMG)/float(totEventMG)*100<<" % "<<endl;
  cout<<"LHE filter efficiency PH : "<<passingLHEFilterPH<<" totEvent "<<totEventPH<<" efficiency "<<float(passingLHEFilterPH)/float(totEventPH)*100<<" % "<<endl;

  cout<<"PT filter efficiency MG : "<<passingLHEFilterMG_Pt<<" LHE event "<<passingLHEFilterMG<<" efficiency "<<float(passingLHEFilterMG_Pt)/float(passingLHEFilterMG)*100<<" % "<<endl;
  cout<<"PT filter efficiency PH : "<<passingLHEFilterPH_Pt<<" LHE event "<<passingLHEFilterPH<<" efficiency "<<float(passingLHEFilterPH_Pt)/float(passingLHEFilterPH)*100<<" % "<<endl;

  cout<<"Deta filter efficiency MG : "<<passingLHEFilterMG_Deta<<" LHE event "<<passingLHEFilterMG_Pt<<" efficiency "<<float(passingLHEFilterMG_Deta)/float(passingLHEFilterMG_Pt)*100<<" % "<<endl;
  cout<<"Deta filter efficiency PH : "<<passingLHEFilterPH_Deta<<" LHE event "<<passingLHEFilterPH_Pt<<" efficiency "<<float(passingLHEFilterPH_Deta)/float(passingLHEFilterPH_Pt)*100<<" % "<<endl;

  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffMG.begin(); itMap !=  histoCutEffMG.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEffPH.begin(); itMap !=  histoCutEffPH.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();

  //Normalize histograms                                                                                                                                                        
  for(size_t ihisto = 0; ihisto < plotVectorMG.size(); ihisto++){
    if(plotVectorMG.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo MG "<<plotVectorMG.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorMG.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorMG.at(ihisto).histogram->Integral(0,plotVectorMG.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  for(size_t ihisto = 0; ihisto < plotVectorPH.size(); ihisto++){
    if(plotVectorPH.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo PH "<<plotVectorPH.at(ihisto).histogram->GetName()<<" unweighted "<<plotVectorPH.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVectorPH.at(ihisto).histogram->Integral(0,plotVectorPH.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }

  
  return 0 ;
}  


//  LocalWords:  iEventPH
