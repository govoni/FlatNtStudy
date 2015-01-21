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

int    lheLevelFilter;
double minLeptonCutPt;


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

  // take the variable list to be plotted
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // select lepton flavour final state
  lheLevelFilter      = gConfigParser -> readDoubleOption("Option::lheLevelFilter");
  minLeptonCutPt      = gConfigParser -> readDoubleOption("Option::minLeptonCutPt");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());


  TChain* chainMG = new TChain (treeName.c_str()) ;  
  chainMG->Add ((InputBaseDirectoryMG+"/*.root").c_str()) ;
  int totEventMG = chainMG->GetEntries();

  readTree* readerMG  = new readTree((TTree*)(chainMG));

  cout<<"MG: entries before "<<totEventMG<<endl;

  TChain* chainPH = new TChain (treeName.c_str()) ;  
  chainPH->Add ((InputBaseDirectoryPH+"/*.root").c_str()) ;
  int totEventPH = chainPH->GetEntries();

  readTree* readerPH  = new readTree((TTree*)(chainPH));

  cout<<"PH: entries before "<<totEventPH<<endl;

  // make the plot container 
  vector<histoContainer> plotVectorMG;
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorMG.push_back(histoContainer("MG",variableList.at(iVar)));
  }

  vector<histoContainer> plotVectorPH;
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVectorPH.push_back(histoContainer("PH",variableList.at(iVar)));
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
    if(lheLevelFilter == 0){
      if(fabs(readerMG->leptonLHEpid1) != 13 or fabs(readerMG->leptonLHEpid2) != 13)
	continue;
    }
    else if(lheLevelFilter == 1){      
      if(fabs(readerMG->leptonLHEpid1) != 11 or fabs(readerMG->leptonLHEpid2) != 11) continue;
    }
    else if(lheLevelFilter == 2){
      if(fabs(readerMG->leptonLHEpid1) == fabs(readerMG->leptonLHEpid2)) continue ;
    }

    passingLHEFilterMG++;

    // if an event pass the cut, fill the associated map
    TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2, L_vboson1, L_vboson2, L_neutrino1, L_neutrino2, L_met, L_dijet, L_dilepton, L_LLmet ;

    L_lepton1.SetPtEtaPhiM(readerMG->leptonLHEpt1,readerMG->leptonLHEeta1,readerMG->leptonLHEphi1,readerMG->leptonLHEm1);
    L_lepton2.SetPtEtaPhiM(readerMG->leptonLHEpt2,readerMG->leptonLHEeta2,readerMG->leptonLHEphi2,readerMG->leptonLHEm2);

    L_parton1.SetPtEtaPhiM(readerMG->jetLHEPartonpt1,readerMG->jetLHEPartoneta1,readerMG->jetLHEPartonphi1,0.);
    L_parton2.SetPtEtaPhiM(readerMG->jetLHEPartonpt2,readerMG->jetLHEPartoneta2,readerMG->jetLHEPartonphi2,0.);

    L_vboson1.SetPtEtaPhiM(readerMG->vbosonLHEpt1,readerMG->vbosonLHEeta1,readerMG->vbosonLHEphi1,readerMG->vbosonLHEm1);
    L_vboson2.SetPtEtaPhiM(readerMG->vbosonLHEpt2,readerMG->vbosonLHEeta2,readerMG->vbosonLHEphi2,readerMG->vbosonLHEm2);

    L_neutrino1.SetPtEtaPhiM(readerMG->neutrinoLHEpt1,readerMG->neutrinoLHEeta1,readerMG->neutrinoLHEphi1,0.);
    L_neutrino2.SetPtEtaPhiM(readerMG->neutrinoLHEpt2,readerMG->neutrinoLHEeta2,readerMG->neutrinoLHEphi2,0.);

    L_dilepton = L_lepton1 + L_lepton2;    
    L_met      = L_neutrino1 + L_neutrino2;
    L_LLmet    = L_dilepton + L_met;  

    if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;

    passingLHEFilterMG_Pt++;

    if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;

    passingLHEFilterMG_Deta++;
    
 
    float asimJ = 0, asimL = 0, Rvar = 0;

    asimL = (L_lepton1.Pt()-L_lepton2.Pt())/(L_lepton1.Pt()+L_lepton2.Pt()) ;

    L_dijet  = L_parton1 + L_parton2;
    asimJ    = (L_parton1.Pt()-L_parton2.Pt())/(L_parton1.Pt()+L_parton2.Pt()) ;
    Rvar     = (L_lepton1.Pt()*L_lepton2.Pt())/(L_parton1.Pt()*L_parton2.Pt()) ;

      
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      histoContainer tmpPlot;
      tmpPlot.cutName = "MG";
      tmpPlot.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVec ;
      itVec = find(plotVectorMG.begin(),plotVectorMG.end(),tmpPlot);
      if(itVec == plotVectorMG.end()){
	cerr<<"Problem -->plot not found for MG "<<"  "<<variableList.at(iVar).variableName<<endl;
	continue ;
      }

     if(variableList.at(iVar).variableName == "ptW1"){
	itVec->histogram->Fill(L_vboson1.Pt(),1) ; 
      }
      else if(variableList.at(iVar).variableName == "ptW2" ){
	itVec->histogram->Fill(L_vboson2.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaW1" ){
	itVec->histogram->Fill(L_vboson1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaW2" ){
	itVec->histogram->Fill(L_vboson2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "detaWW" ){
	itVec->histogram->Fill(fabs(L_vboson1.Eta()-L_vboson2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptWW" ){
	itVec->histogram->Fill((L_vboson1+L_vboson2).Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mWW" ){
	itVec->histogram->Fill((L_vboson1+L_vboson2).M(),1) ;
      }
      
     
      if(variableList.at(iVar).variableName == "ptj1"){
	itVec->histogram->Fill(L_parton1.Pt(),1) ; 
      }
      else if(variableList.at(iVar).variableName == "ptj2" ){
	itVec->histogram->Fill(L_parton2.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj1" ){
	itVec->histogram->Fill(L_parton1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj2" ){
	itVec->histogram->Fill(L_parton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "detajj" ){
	itVec->histogram->Fill(fabs(L_parton1.Eta()-L_parton2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptjj" ){
	itVec->histogram->Fill(L_dijet.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mjj" ){
	itVec->histogram->Fill(L_dijet.M(),1) ;
      }
      else if(variableList.at(iVar).variableName == "dRjj" ){
	itVec->histogram->Fill(L_parton1.DeltaR(L_parton2),1) ;
      }
      else if(variableList.at(iVar).variableName == "Asim_j" ){
          itVec->histogram->Fill(asimJ,1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
	itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_parton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj1etaj2" ){
	itVec->histogram->Fill(L_parton1.Eta()*L_parton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptl1" ){
	itVec->histogram->Fill(L_lepton1.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptl2" ){
	itVec->histogram->Fill(L_lepton2.Pt(),1) ;
      }
 
      else if(variableList.at(iVar).variableName == "etal1" ){
	itVec->histogram->Fill(L_lepton1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etal2" ){
	itVec->histogram->Fill(L_lepton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mll" ){
	itVec->histogram->Fill(L_dilepton.M(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptll" ){
	itVec->histogram->Fill(L_dilepton.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etall" ){
	itVec->histogram->Fill(L_dilepton.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "phill" ){
	itVec->histogram->Fill(L_dilepton.Phi(),1) ;
      }
      else if(variableList.at(iVar).variableName == "dRll" ){
	itVec->histogram->Fill(L_lepton1.DeltaR(L_lepton2),1) ;
      }
      else if(variableList.at(iVar).variableName == "etal1etal2" ){
	itVec->histogram->Fill(L_lepton1.Eta()*L_lepton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_lepton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaEta_LL" ){
	itVec->histogram->Fill(fabs(L_lepton1.Eta()-L_lepton2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "Asim_l" ){
	itVec->histogram->Fill(asimL,1) ;
      }
      else if(variableList.at(iVar).variableName == "met" ){
	itVec->histogram->Fill(L_met.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "R" ){
	itVec->histogram->Fill(Rvar,1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
	itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),1) ;
      }
      ///
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton1)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_dijet)),1) ;
      }
      ///

      else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" ){
	itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" ){
	itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" ){
	itVec->histogram->Fill(fabs(L_parton2.DeltaPhi(L_met)),1) ;
      }
      ///

      else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" ){
	itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),1) ;
      }
      else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" ){
	itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),1) ;
      }

      else if(variableList.at(iVar).variableName == "mlljj" ){
	itVec->histogram->Fill((L_dilepton+L_dijet).M(),1) ;
      }
      
      else if(variableList.at(iVar).variableName == "mlljjmet" ){
	itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),1) ;
      }

      else if(variableList.at(iVar).variableName == "mTH" ){
	itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),1) ;
      } 
    }            
  }

  // Loop on the events
  for(int iEventPH = 0; iEventPH < chainPH->GetEntries(); iEventPH++){

    readerPH->fChain->GetEntry(iEventPH) ;

    if (iEventPH % 100000 == 0) cout << "reading event PH: " << iEventPH << "\n" ;

    // filter LHE level leptons

    if(lheLevelFilter == 0){
      if(fabs(readerPH->leptonLHEpid1) != 13 or fabs(readerPH->leptonLHEpid2) != 13)
	continue;
    }
    else if(lheLevelFilter == 1){      
      if(fabs(readerPH->leptonLHEpid1) != 11 or fabs(readerPH->leptonLHEpid2) != 11) continue;
    }
    else if(lheLevelFilter == 2){
      if(fabs(readerPH->leptonLHEpid1) == fabs(readerPH->leptonLHEpid2)) continue ;
    }
    else { 
      cerr<<"Bad event "<<endl;
      continue;
    }

    passingLHEFilterPH++;


    // if an event pass the cut, fill the associated map
    TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2, L_vboson1, L_vboson2, L_neutrino1, L_neutrino2, L_met, L_dijet, L_dilepton, L_LLmet ;

    L_lepton1.SetPtEtaPhiM(readerPH->leptonLHEpt1,readerPH->leptonLHEeta1,readerPH->leptonLHEphi1,readerPH->leptonLHEm1);
    L_lepton2.SetPtEtaPhiM(readerPH->leptonLHEpt2,readerPH->leptonLHEeta2,readerPH->leptonLHEphi2,readerPH->leptonLHEm2);

    L_parton1.SetPtEtaPhiM(readerPH->jetLHEPartonpt1,readerPH->jetLHEPartoneta1,readerPH->jetLHEPartonphi1,0.);
    L_parton2.SetPtEtaPhiM(readerPH->jetLHEPartonpt2,readerPH->jetLHEPartoneta2,readerPH->jetLHEPartonphi2,0.);

    L_vboson1.SetPtEtaPhiM(readerPH->vbosonLHEpt1,readerPH->vbosonLHEeta1,readerPH->vbosonLHEphi1,readerPH->vbosonLHEm1);
    L_vboson2.SetPtEtaPhiM(readerPH->vbosonLHEpt2,readerPH->vbosonLHEeta2,readerPH->vbosonLHEphi2,readerPH->vbosonLHEm2);

    L_neutrino1.SetPtEtaPhiM(readerPH->neutrinoLHEpt1,readerPH->neutrinoLHEeta1,readerPH->neutrinoLHEphi1,0.);
    L_neutrino2.SetPtEtaPhiM(readerPH->neutrinoLHEpt2,readerPH->neutrinoLHEeta2,readerPH->neutrinoLHEphi2,0.);

    L_dilepton = L_lepton1 + L_lepton2;    
    L_met      = L_neutrino1 + L_neutrino2;
    L_LLmet    = L_dilepton + L_met;  

    if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;

    passingLHEFilterPH_Pt++;
    
    if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;

    passingLHEFilterPH_Deta++;
    
 
    float asimJ = 0, asimL = 0, Rvar = 0;

    asimL = (L_lepton1.Pt()-L_lepton2.Pt())/(L_lepton1.Pt()+L_lepton2.Pt()) ;

    L_dijet  = L_parton1 + L_parton2;
    asimJ    = (L_parton1.Pt()-L_parton2.Pt())/(L_parton1.Pt()+L_parton2.Pt()) ;
    Rvar     = (L_lepton1.Pt()*L_lepton2.Pt())/(L_parton1.Pt()*L_parton2.Pt()) ;

      
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      histoContainer tmpPlot;
      tmpPlot.cutName = "PH";
      tmpPlot.varName = variableList.at(iVar).variableName;
      vector<histoContainer>::iterator itVec ;
      itVec = find(plotVectorPH.begin(),plotVectorPH.end(),tmpPlot);
      if(itVec == plotVectorPH.end()){
	cerr<<"Problem -->plot not found for PH "<<"  "<<variableList.at(iVar).variableName<<endl;
	continue ;
      }
 
      
      if(variableList.at(iVar).variableName == "ptW1" and readerPH->vbosonLHEpt1 > 0){
	itVec->histogram->Fill(L_vboson1.Pt(),1) ; 
      }
      else if(variableList.at(iVar).variableName == "ptW2" and readerPH->vbosonLHEpt2 > 0){
	itVec->histogram->Fill(L_vboson2.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaW1" and readerPH->vbosonLHEpt1 > 0){
	itVec->histogram->Fill(L_vboson1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaW2" and readerPH->vbosonLHEpt2> 0){
	itVec->histogram->Fill(L_vboson2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "detaWW" and readerPH->vbosonLHEpt1 > 0 and readerPH->vbosonLHEpt2 > 0 ){
	itVec->histogram->Fill(fabs(L_vboson1.Eta()-L_vboson2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptWW"   and readerPH->vbosonLHEpt1 > 0 and readerPH->vbosonLHEpt2 > 0){
	itVec->histogram->Fill((L_vboson1+L_vboson2).Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mWW"    and readerPH->vbosonLHEpt1 > 0 and readerPH->vbosonLHEpt2 > 0){
	itVec->histogram->Fill((L_vboson1+L_vboson2).M(),1) ;
      }
      

      if(variableList.at(iVar).variableName == "ptj1"){
	itVec->histogram->Fill(L_parton1.Pt(),1) ; 
      }
      else if(variableList.at(iVar).variableName == "ptj2" ){
	itVec->histogram->Fill(L_parton2.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj1" ){
	itVec->histogram->Fill(L_parton1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj2" ){
	itVec->histogram->Fill(L_parton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "detajj" ){
	itVec->histogram->Fill(fabs(L_parton1.Eta()-L_parton2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptjj" ){
	itVec->histogram->Fill(L_dijet.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mjj" ){
	itVec->histogram->Fill(L_dijet.M(),1) ;
      }
      else if(variableList.at(iVar).variableName == "dRjj" ){
	itVec->histogram->Fill(L_parton1.DeltaR(L_parton2),1) ;
      }
      else if(variableList.at(iVar).variableName == "Asim_j" ){
          itVec->histogram->Fill(asimJ,1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
	itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_parton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "etaj1etaj2" ){
	itVec->histogram->Fill(L_parton1.Eta()*L_parton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptl1" ){
	itVec->histogram->Fill(L_lepton1.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptl2" ){
	itVec->histogram->Fill(L_lepton2.Pt(),1) ;
      }
 
      else if(variableList.at(iVar).variableName == "etal1" ){
	itVec->histogram->Fill(L_lepton1.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etal2" ){
	itVec->histogram->Fill(L_lepton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "mll" ){
	itVec->histogram->Fill(L_dilepton.M(),1) ;
      }
      else if(variableList.at(iVar).variableName == "ptll" ){
	itVec->histogram->Fill(L_dilepton.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "etall" ){
	itVec->histogram->Fill(L_dilepton.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "phill" ){
	itVec->histogram->Fill(L_dilepton.Phi(),1) ;
      }
      else if(variableList.at(iVar).variableName == "dRll" ){
	itVec->histogram->Fill(L_lepton1.DeltaR(L_lepton2),1) ;
      }
      else if(variableList.at(iVar).variableName == "etal1etal2" ){
	itVec->histogram->Fill(L_lepton1.Eta()*L_lepton2.Eta(),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_lepton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaEta_LL" ){
	itVec->histogram->Fill(fabs(L_lepton1.Eta()-L_lepton2.Eta()),1) ;
      }
      else if(variableList.at(iVar).variableName == "Asim_l" ){
	itVec->histogram->Fill(asimL,1) ;
      }
      else if(variableList.at(iVar).variableName == "met" ){
	itVec->histogram->Fill(L_met.Pt(),1) ;
      }
      else if(variableList.at(iVar).variableName == "R" ){
	itVec->histogram->Fill(Rvar,1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
	itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),1) ;
      }
      ///
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton1)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton2)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" ){
	itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_dijet)),1) ;
      }
      ///

      else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" ){
	itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" ){
	itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_met)),1) ;
      }
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" ){
	itVec->histogram->Fill(fabs(L_parton2.DeltaPhi(L_met)),1) ;
      }
      ///

      else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" ){
	itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),1) ;
      }
      else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" ){
	itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),1) ;
      }

      else if(variableList.at(iVar).variableName == "mlljj" ){
	itVec->histogram->Fill((L_dilepton+L_dijet).M(),1) ;
      }
      
      else if(variableList.at(iVar).variableName == "mlljjmet" ){
	itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),1) ;
      }

      else if(variableList.at(iVar).variableName == "mTH" ){
	itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),1) ;
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
 

  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
    histoContainer tmpPlotMG;
    tmpPlotMG.cutName = "MG";
    tmpPlotMG.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVecMG ;
    itVecMG = find(plotVectorMG.begin(),plotVectorMG.end(),tmpPlotMG);
    if(itVecMG == plotVectorMG.end()){
      cerr<<"Problem -->plot not found for MG :  "<<variableList.at(iVar).variableName<<endl;
    }
       
    itVecMG->histogram->GetXaxis()->SetTitleSize(0.04);
    itVecMG->histogram->Scale(1./itVecMG->histogram->Integral(0,itVecMG->histogram->GetNbinsX()+1));
    itVecMG->histogram->GetYaxis()->SetRangeUser(0.001,itVecMG->histogram->GetMaximum()*1.1);
    itVecMG->histogram->GetYaxis()->SetTitleSize(0.05);

    itVecMG->histogram->SetLineColor(kBlue);
    itVecMG->histogram->SetLineStyle(1);
    itVecMG->histogram->SetLineWidth(2);
    itVecMG->histogram->GetYaxis()->SetTitle("a.u.");

    itVecMG->histogram->Draw("hist");
    legend->AddEntry(itVecMG->histogram,"Madgraph","l");


    histoContainer tmpPlotPH;
    tmpPlotPH.cutName = "PH";
    tmpPlotPH.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVecPH ;
    itVecPH = find(plotVectorPH.begin(),plotVectorPH.end(),tmpPlotPH);
    if(itVecPH == plotVectorPH.end()){
      cerr<<"Problem -->plot not found for PH :  "<<variableList.at(iVar).variableName<<endl;
    }

       
    itVecPH->histogram->GetXaxis()->SetTitleSize(0.04);
    itVecPH->histogram->Scale(1./itVecPH->histogram->Integral(0,itVecPH->histogram->GetNbinsX()+1));
    itVecPH->histogram->GetYaxis()->SetRangeUser(0.001,itVecPH->histogram->GetMaximum()*1.1);
    itVecPH->histogram->GetYaxis()->SetTitleSize(0.05);

    itVecPH->histogram->SetLineColor(kRed);
    itVecPH->histogram->SetLineStyle(7);
    itVecPH->histogram->SetLineWidth(2);
    itVecPH->histogram->GetYaxis()->SetTitle("a.u.");

    itVecPH->histogram->Draw("hist same");
    legend->AddEntry(itVecPH->histogram,"Phantom","l");

    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");
    legend->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".root").c_str(),"root");

    cCanvas->SetLogy();
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.root").c_str(),"root");

    cCanvas->SetLogy(0);
    gPad->Update();
    
    legend->Clear();

  }
  
  cout<<"LHE filter efficiency MG : "<<passingLHEFilterMG<<" totEvent "<<totEventMG<<" efficiency "<<float(passingLHEFilterMG)/float(totEventMG)*100<<" % "<<endl;
  cout<<"LHE filter efficiency PH : "<<passingLHEFilterPH<<" totEvent "<<totEventPH<<" efficiency "<<float(passingLHEFilterPH)/float(totEventPH)*100<<" % "<<endl;

  cout<<"PT filter efficiency MG : "<<passingLHEFilterMG_Pt<<" LHE event "<<passingLHEFilterMG<<" efficiency "<<float(passingLHEFilterMG_Pt)/float(passingLHEFilterMG)*100<<" % "<<endl;
  cout<<"PT filter efficiency PH : "<<passingLHEFilterPH_Pt<<" LHE event "<<passingLHEFilterPH<<" efficiency "<<float(passingLHEFilterPH_Pt)/float(passingLHEFilterPH)*100<<" % "<<endl;

  cout<<"Deta filter efficiency MG : "<<passingLHEFilterMG_Deta<<" LHE event "<<passingLHEFilterMG_Pt<<" efficiency "<<float(passingLHEFilterMG_Deta)/float(passingLHEFilterMG_Pt)*100<<" % "<<endl;
  cout<<"Deta filter efficiency PH : "<<passingLHEFilterPH_Deta<<" LHE event "<<passingLHEFilterPH_Pt<<" efficiency "<<float(passingLHEFilterPH_Deta)/float(passingLHEFilterPH_Pt)*100<<" % "<<endl;
  
  return 0 ;
}  

