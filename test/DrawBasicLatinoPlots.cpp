/////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TPad.h"

#include "plotter.h"
#include "ConfigParser.h"
#include "readTreeLatino.h"
#include "utilsLatino.h"
#include "utils.h"

using namespace std ;

float  matchingCone ;
float  minLeptonCleaningPt;
float  minLeptonCutPt;
float  minJetCutPt;
string finalStateString;

int   nLeptons   = 2;
bool  sameCharge = true;
float metCut     = 0;
float mllCut     = 0;
float mjjCut     = 400;
float detajjCut  = 2.5;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main (int argc, char ** argv) {

  // check number of inpt parameters
  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<endl;
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
  string InputBaseDirectory_PhaseIAged       = gConfigParser -> readStringOption("Input::InputBaseDirectoryPhaseIAged");
  string InputBaseDirectory_PhaseIIShashlik  = gConfigParser -> readStringOption("Input::InputBaseDirectoryPhaseIIShashlik");

  // import from cfg file the cross section value for this sample
  float CrossSection   = gConfigParser -> readFloatOption("Input::CrossSection");

  // total number of events
  int maxEventNumber   = gConfigParser -> readFloatOption("Input::EventsNumber");

  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

  // take the variable list to be plotted
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());
  
  ///// Start the analysis for phaseI aged detector  
  map<string,TH1F*> histoCutEff_PhaseIAged ;
  map<string,TH1F*> histoCutEff_PhaseIIShashlik ;
  map<string,TH1F*> histoCutEff_Gen ;

  TChain* chain_PhaseIAged = new TChain (treeName.c_str()) ;  
  chain_PhaseIAged->Add ((InputBaseDirectory_PhaseIAged+"/*.root").c_str()) ;
  int totEvent_PhaseIAged = chain_PhaseIAged->GetEntries();

  TChain* chain_PhaseIIShashlik = new TChain (treeName.c_str()) ;  
  chain_PhaseIIShashlik->Add ((InputBaseDirectory_PhaseIIShashlik+"/*.root").c_str()) ;
  int totEvent_PhaseIIShashlik = chain_PhaseIIShashlik->GetEntries();

  readTreeLatino* reader_PhaseIAged  = new readTreeLatino((TTree*)(chain_PhaseIAged));
  readTreeLatino* reader_PhaseIIShashlik  = new readTreeLatino((TTree*)(chain_PhaseIIShashlik));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_PhaseIAged<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_PhaseIAged)<<endl;
  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_PhaseIIShashlik<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_PhaseIIShashlik)<<endl;

  float weight_PhaseIAged = 1.0*lumi*CrossSection/float(totEvent_PhaseIAged) ;
  float weight_PhaseIIShashlik = 1.0*lumi*CrossSection/float(totEvent_PhaseIIShashlik) ;

  // make the plot container 
  vector<histoContainer> plotVector_PhaseIAged;
  vector<histoContainer> plotVector_PhaseIIShashlik;
  vector<histoContainer> plotVector_Gen;

  histoCutEff_PhaseIAged["PhaseIAged"]           = new TH1F("PhaseIAged","",15,0,15);
  histoCutEff_PhaseIIShashlik["PhaseIIShashlik"] = new TH1F("PhaseIIShashlik","",15,0,15);
  histoCutEff_Gen["Gen"]                         = new TH1F("Gen","",15,0,15);

  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    plotVector_PhaseIAged.push_back(histoContainer("PhaseIAged",variableList.at(iVar)));
    plotVector_Gen.push_back(histoContainer("Gen",variableList.at(iVar)));
    plotVector_PhaseIIShashlik.push_back(histoContainer("PhaseIIShashlik",variableList.at(iVar)));
  }

  
  int maximumEvents_PhaseIAged = chain_PhaseIAged->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_PhaseIAged) 
    maximumEvents_PhaseIAged = maxEventNumber ;

  int maximumEvents_PhaseIIShashlik = chain_PhaseIIShashlik->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_PhaseIIShashlik) 
    maximumEvents_PhaseIIShashlik = maxEventNumber ;
  
  // Loop on the events
  for(int iEvent = 0; iEvent < maximumEvents_PhaseIAged ; iEvent++){

    reader_PhaseIAged->fChain->GetEntry(iEvent) ;

    if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ;
    
    // cut the events
    string name = "PhaseIAged";
    
    // dump all the lepton in the event                                                                                                                                  
    vector<leptonContainerLatino> LeptonsAll;
    fillRecoLeptonsArray (LeptonsAll, *reader_PhaseIAged);

    vector<leptonContainerLatino> LeptonsGen;
    fillGenLeptonsArray (LeptonsGen, *reader_PhaseIAged);

    // dump tight leptons                                                                                                                                                 
    vector<leptonContainerLatino> leptonsCleaned ;
    leptonsCleaned = dumpLeptons (LeptonsAll, LeptonsGen, minLeptonCutPt, matchingCone);

    // take reco jets                                                                                                                                                         
    vector<jetContainerLatino> RecoJetsAll ;
    fillRecoJetArray (RecoJetsAll, *reader_PhaseIAged) ;

    // take jets                                                                                                                                                         
    vector<jetContainerLatino> RecoJets;
    RecoJets  = dumpJets (RecoJetsAll, leptonsCleaned, minJetCutPt, 99., -99, minLeptonCutPt, matchingCone);

    vector<genJetContainerLatino> GenJetsAll ;
    fillGenJetArray (GenJetsAll, *reader_PhaseIAged) ;

    vector<genJetContainerLatino> GenJets;
    GenJets  = dumpJets (GenJetsAll, LeptonsGen, 0., 99., -99, 0., matchingCone);

    // apply cuts defined in the cut list      
    int iBin = 1;
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }

    if(LeptonsAll.size()  < 2) continue;
    if(RecoJetsAll.size() < 2) continue;
    if(GenJetsAll.size()  < 2) continue;

    
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }
    
    // identify tight leptons and require exactly nLep                                                                                                                 
    if (int(leptonsCleaned.size()) != nLeptons ) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }

    if (leptonsCleaned.at(0).lepton4V_.Pt() < minLeptonCutPt) continue;
    if (fabs(leptonsCleaned.at(0).lepton4V_.Eta()) > 2.5) continue ;

    bool badTrailingLepton = false;
    for( size_t iLep = 1 ; iLep < leptonsCleaned.size(); iLep++){
      if( leptonsCleaned.at(iLep).lepton4V_.Pt() < minLeptonCutPt or fabs(leptonsCleaned.at(iLep).lepton4V_.Eta()) > 2.5){
	badTrailingLepton = true;
      }
    }
    if(badTrailingLepton) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
      iBin++;
    }

    if(sameCharge) {
      int sign = 1;
      for(size_t leptSize = 0; leptSize < leptonsCleaned.size(); leptSize++) // loop on tight leptns                                                                         
	sign *= leptonsCleaned.at(leptSize).charge_;
      if(sign != 1) continue;
    }

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }


    if(TString(finalStateString).Contains("UU")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 13) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 13) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 11) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 11) continue;
    }
    else if(TString(finalStateString).Contains("EU")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 11) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 13) continue;
    }
    else if(TString(finalStateString).Contains("UE")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 13) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 11) continue;
    }

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }

    TLorentzVector L_dilepton, L_met, L_metGen;
    TLorentzVector L_dijet, L_LLmet;

    L_dilepton = leptonsCleaned.at(0).lepton4V_ + leptonsCleaned.at(1).lepton4V_ ;
    
    L_met.SetPtEtaPhiM(reader_PhaseIAged->pfmet,0.,reader_PhaseIAged->pfmetphi, 0.) ;
    L_metGen.SetPtEtaPhiM(reader_PhaseIAged->metGenpt,0.,reader_PhaseIAged->metGenphi, 0.) ;

    L_LLmet = L_dilepton + L_met ;

    if(RecoJets.size() >= 2){
      L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    }

    // met cut                                                                                                                                                              
    if(L_met.Pt() < metCut) continue;

    // apply Mll cut                                                                                                                                                        
    if(L_dilepton.M() > 14000 or L_dilepton.M() < mllCut) continue;
 
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"Mll");
      iBin++;
    }

    if(RecoJets.size() < 2) continue;

    // apply VBF cuts                                                                                                                                                       
    if(L_dijet.M() < mjjCut) continue;


    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"Mjj");
      iBin++;
    }

    if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < detajjCut) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
      iBin++;
    }
  
    // loop on variables
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
        histoContainer tmpPlot;
        tmpPlot.cutName = "PhaseIAged";
        tmpPlot.varName = variableList.at(iVar).variableName;
        vector<histoContainer>::iterator itVec ;
        itVec = find(plotVector_PhaseIAged.begin(),plotVector_PhaseIAged.end(),tmpPlot);
        if(itVec == plotVector_PhaseIAged.end()){
          cerr<<"Problem -->plot not found for PhaseIAged "<<variableList.at(iVar).variableName<<endl;
          continue ;
        }


	if(variableList.at(iVar).variableName == "ptj1"){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),1.*weight_PhaseIAged) ; 
        }
	else if(variableList.at(iVar).variableName == "ptj2"){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "etaj1"){
          itVec->histogram->Fill(fabs (RecoJets.at(0).jet4V_.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "etaj2" ){
          itVec->histogram->Fill(fabs (RecoJets.at(1).jet4V_.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "detajj" ){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptl1" ){
          itVec->histogram->Fill(leptonsCleaned.at(0).lepton4V_.Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptl2" ){
          itVec->histogram->Fill(leptonsCleaned.at(1).lepton4V_.Pt(),weight_PhaseIAged) ;
        }
 
	else if(variableList.at(iVar).variableName == "etal1" ){
          itVec->histogram->Fill(fabs (leptonsCleaned.at(0).lepton4V_.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "etal2" ){
          itVec->histogram->Fill(fabs (leptonsCleaned.at(1).lepton4V_.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "mll" ){
          itVec->histogram->Fill(L_dilepton.M(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" ){
          itVec->histogram->Fill(L_dilepton.Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "etall" ){
          itVec->histogram->Fill(fabs (L_dilepton.Eta()),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(0).lepton4V_.DeltaPhi(leptonsCleaned.at(1).lepton4V_)),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "met" ){
          itVec->histogram->Fill(L_met.Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(0).lepton4V_.DeltaPhi(L_met)),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptLMet" ){
          itVec->histogram->Fill((leptonsCleaned.at(0).lepton4V_ + L_met).Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(1).lepton4V_.DeltaPhi(L_met)),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptTLMet" ){
          itVec->histogram->Fill((leptonsCleaned.at(1).lepton4V_ + L_met).Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "ptLLMet" ){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weight_PhaseIAged) ;
        }
	else if(variableList.at(iVar).variableName == "mTH" ){
          itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weight_PhaseIAged) ;
        } 
    } // loop on variables   
  } // Loop  on the cut list



  // Loop on the events
  for(int iEvent = 0; iEvent < maximumEvents_PhaseIIShashlik ; iEvent++){

    reader_PhaseIIShashlik->fChain->GetEntry(iEvent) ;

    if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ;
    
    // cut the events
    string name = "PhaseIIShashlik";
    
    // dump all the lepton in the event                                                                                                                                  
    vector<leptonContainerLatino> LeptonsAll;
    fillRecoLeptonsArray (LeptonsAll, *reader_PhaseIIShashlik);

    vector<leptonContainerLatino> LeptonsGen;
    fillGenLeptonsArray (LeptonsGen, *reader_PhaseIIShashlik);

    // dump tight leptons                                                                                                                                                 
    vector<leptonContainerLatino> leptonsCleaned ;
    leptonsCleaned = dumpLeptons (LeptonsAll, LeptonsGen, minLeptonCutPt, matchingCone);

    // take reco jets                                                                                                                                                         
    vector<jetContainerLatino> RecoJetsAll ;
    fillRecoJetArray (RecoJetsAll, *reader_PhaseIIShashlik) ;

    // take jets                                                                                                                                                         
    vector<jetContainerLatino> RecoJets;
    RecoJets  = dumpJets (RecoJetsAll, leptonsCleaned, minJetCutPt, 99., -99, minLeptonCutPt, matchingCone);

    vector<genJetContainerLatino> GenJetsAll ;
    fillGenJetArray (GenJetsAll, *reader_PhaseIIShashlik) ;

    vector<genJetContainerLatino> GenJets;
    GenJets  = dumpJets (GenJetsAll, LeptonsGen, 0., 99., -99, 0., matchingCone);

    // apply cuts defined in the cut list      
    int iBin = 1;
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }

    if(LeptonsAll.size()  < 2) continue;
    if(RecoJetsAll.size() < 2) continue;
    if(GenJetsAll.size()  < 2) continue;

    
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }
    
    // identify tight leptons and require exactly nLep                                                                                                                 
    if (int(leptonsCleaned.size()) != nLeptons ) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }

    if (leptonsCleaned.at(0).lepton4V_.Pt() < minLeptonCutPt) continue;
    if (fabs(leptonsCleaned.at(0).lepton4V_.Eta()) > 2.5) continue ;

    bool badTrailingLepton = false;
    for( size_t iLep = 1 ; iLep < leptonsCleaned.size(); iLep++){
      if( leptonsCleaned.at(iLep).lepton4V_.Pt() < minLeptonCutPt or fabs(leptonsCleaned.at(iLep).lepton4V_.Eta()) > 2.5){
	badTrailingLepton = true;
      }
    }
    if(badTrailingLepton) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
      iBin++;
    }

    if(sameCharge) {
      int sign = 1;
      for(size_t leptSize = 0; leptSize < leptonsCleaned.size(); leptSize++) // loop on tight leptns                                                                         
	sign *= leptonsCleaned.at(leptSize).charge_;
      if(sign != 1) continue;
    }

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }


    if(TString(finalStateString).Contains("UU")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 13) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 13) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 11) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 11) continue;
    }
    else if(TString(finalStateString).Contains("EU")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 11) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 13) continue;
    }
    else if(TString(finalStateString).Contains("UE")){
      if(fabs(leptonsCleaned.at(0).flavour_) != 13) continue;
      if(fabs(leptonsCleaned.at(1).flavour_) != 11) continue;
    }

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }

    TLorentzVector L_dilepton, L_met, L_metGen;
    TLorentzVector L_dijet, L_LLmet;

    L_dilepton = leptonsCleaned.at(0).lepton4V_ + leptonsCleaned.at(1).lepton4V_ ;
    L_met.SetPtEtaPhiM(reader_PhaseIIShashlik->pfmet,0.,reader_PhaseIIShashlik->pfmetphi, 0.) ;
    L_metGen.SetPtEtaPhiM(reader_PhaseIIShashlik->metGenpt,0.,reader_PhaseIIShashlik->metGenphi, 0.) ;

    L_LLmet = L_dilepton + L_met ;

    if(RecoJets.size() >= 2){
      L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    }

    // met cut                                                                                                                                                              
    if(L_met.Pt() < metCut) continue;

    // apply Mll cut                                                                                                                                                        
    if(L_dilepton.M() > 14000 or L_dilepton.M() < mllCut) continue;
 
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"Mll");
      iBin++;
    }

    if(RecoJets.size() < 2) continue;

    // apply VBF cuts                                                                                                                                                       
    if(L_dijet.M() < mjjCut) continue;


    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"Mjj");
      iBin++;
    }

    if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < detajjCut) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
      iBin++;
    }
  
    // loop on variables
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
        histoContainer tmpPlot;
        tmpPlot.cutName = "PhaseIIShashlik";
        tmpPlot.varName = variableList.at(iVar).variableName;
        vector<histoContainer>::iterator itVec ;
        itVec = find(plotVector_PhaseIIShashlik.begin(),plotVector_PhaseIIShashlik.end(),tmpPlot);
        if(itVec == plotVector_PhaseIIShashlik.end()){
          cerr<<"Problem -->plot not found for PhaseIIShashlik "<<variableList.at(iVar).variableName<<endl;
          continue ;
        }


	if(variableList.at(iVar).variableName == "ptj1"){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),1.*weight_PhaseIIShashlik) ; 
        }
	else if(variableList.at(iVar).variableName == "ptj2"){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "etaj1"){
          itVec->histogram->Fill(fabs (RecoJets.at(0).jet4V_.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "etaj2" ){
          itVec->histogram->Fill(fabs (RecoJets.at(1).jet4V_.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "detajj" ){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptl1" ){
          itVec->histogram->Fill(leptonsCleaned.at(0).lepton4V_.Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptl2" ){
          itVec->histogram->Fill(leptonsCleaned.at(1).lepton4V_.Pt(),weight_PhaseIIShashlik) ;
        }
 
	else if(variableList.at(iVar).variableName == "etal1" ){
          itVec->histogram->Fill(fabs (leptonsCleaned.at(0).lepton4V_.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "etal2" ){
          itVec->histogram->Fill(fabs (leptonsCleaned.at(1).lepton4V_.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "mll" ){
          itVec->histogram->Fill(L_dilepton.M(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" ){
          itVec->histogram->Fill(L_dilepton.Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "etall" ){
          itVec->histogram->Fill(fabs (L_dilepton.Eta()),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(0).lepton4V_.DeltaPhi(leptonsCleaned.at(1).lepton4V_)),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "met" ){
          itVec->histogram->Fill(L_met.Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(0).lepton4V_.DeltaPhi(L_met)),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptLMet" ){
          itVec->histogram->Fill((leptonsCleaned.at(0).lepton4V_ + L_met).Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
          itVec->histogram->Fill(fabs(leptonsCleaned.at(1).lepton4V_.DeltaPhi(L_met)),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptTLMet" ){
          itVec->histogram->Fill((leptonsCleaned.at(1).lepton4V_ + L_met).Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "ptLLMet" ){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weight_PhaseIIShashlik) ;
        }
	else if(variableList.at(iVar).variableName == "mTH" ){
          itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weight_PhaseIIShashlik) ;
        } 
    } // loop on variables   
  } // Loop  on the cut list



  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_PhaseIAged.begin(); itMap !=  histoCutEff_PhaseIAged.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_PhaseIAged));
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_PhaseIIShashlik.begin(); itMap !=  histoCutEff_PhaseIIShashlik.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_PhaseIIShashlik));
    itMap->second->Write();
  }

  outputEfficiency->Close();

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
  TLatex * tex3 = new TLatex(0.31,0.92,"Simulation Preliminary");
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

 
  // make the plot on the same canvas for each variable (legend entry is the cut layer name)  
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
    histoContainer tmpPlot_PhaseIAged;
    tmpPlot_PhaseIAged.cutName = "PhaseIAged";
    tmpPlot_PhaseIAged.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_PhaseIAged ;
    itVec_PhaseIAged = find(plotVector_PhaseIAged.begin(),plotVector_PhaseIAged.end(),tmpPlot_PhaseIAged);
    if(itVec_PhaseIAged == plotVector_PhaseIAged.end()){
      cerr<<"Problem -->plot not found for PhaseI_Aged  "<<variableList.at(iVar).variableName<<endl;
    }

       
    itVec_PhaseIAged->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_PhaseIAged->histogram->GetXaxis()->SetTitleOffset(1.25);
    itVec_PhaseIAged->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,itVec_PhaseIAged->histogram->GetMaximum()*1.1);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_PhaseIAged->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_PhaseIAged->histogram->SetLineColor(kBlue);

    itVec_PhaseIAged->histogram->SetLineWidth(2);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleOffset(1.1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    legend->AddEntry(itVec_PhaseIAged->histogram,"Phase I Aged","l");


    histoContainer tmpPlot_PhaseIIShashlik;
    tmpPlot_PhaseIIShashlik.cutName = "PhaseIIShashlik";
    tmpPlot_PhaseIIShashlik.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_PhaseIIShashlik ;
    itVec_PhaseIIShashlik = find(plotVector_PhaseIIShashlik.begin(),plotVector_PhaseIIShashlik.end(),tmpPlot_PhaseIIShashlik);
    if(itVec_PhaseIIShashlik == plotVector_PhaseIIShashlik.end()){
      cerr<<"Problem -->plot not found for PhaseI_Aged  "<<variableList.at(iVar).variableName<<endl;
    }

       
    itVec_PhaseIIShashlik->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_PhaseIIShashlik->histogram->GetXaxis()->SetTitleOffset(1.16);
    itVec_PhaseIIShashlik->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_PhaseIIShashlik->histogram->GetYaxis()->SetRangeUser(0.001,itVec_PhaseIIShashlik->histogram->GetMaximum()*1.1);
    itVec_PhaseIIShashlik->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_PhaseIIShashlik->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_PhaseIIShashlik->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_PhaseIIShashlik->histogram->SetLineColor(kRed);

    itVec_PhaseIIShashlik->histogram->SetLineWidth(2);
    itVec_PhaseIIShashlik->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.1);

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");

    legend->AddEntry(itVec_PhaseIIShashlik->histogram,"Phase II Shashlik","l");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.1,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*100);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    itVec_PhaseIAged->histogram->Scale(1./itVec_PhaseIAged->histogram->Integral());
    itVec_PhaseIIShashlik->histogram->Scale(1./itVec_PhaseIIShashlik->histogram->Integral());

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.1);

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_norm.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_norm.png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.1);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_norm_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_norm_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    legend->Clear();

  }

  return 0 ;
}  

