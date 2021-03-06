//C++ include
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <iomanip>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TClonesArray.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"

// Bacon and other includes
#include "ReadInputFile.h"
#include "TBaconObjects.h"
#include "ConfigParser.h"
#include "utilsBacon.h"
#include "utils.h"
#include "readTree.h"

using namespace std;
using namespace baconhep;

float matchingConeLepton = 0.05 ;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----                                                                                                
int main (int argc, char ** argv) {

  // check number of inpt parameters                                                                                                                                          
  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<endl;
    return -1;
  }

  float etaJetCut = 4.7;

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
  string InputBaseDirectory_FullSIM  = gConfigParser -> readStringOption("Input::InputBaseDirectoryFullSIM");
  string InputBaseDirectory_Delphes  = gConfigParser -> readStringOption("Input::InputBaseDirectoryDelphes");

  // import from cfg file the cross section value for this sample                                                                                                             
  float CrossSection   = gConfigParser -> readFloatOption("Input::CrossSection");

  // total number of events                                                                                                                                                    
  int maxEventNumber   = gConfigParser -> readFloatOption("Input::EventsNumber");

  // treeName                                                                                                                                                                  
  string treeNameFullSIM      = gConfigParser -> readStringOption("Input::TreeNameFullSIM");
  string treeNameDelphes      = gConfigParser -> readStringOption("Input::TreeNameDelphes");

  // take the variable list to be plotted                                                                                                                                      
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  string InputVariableRespList  = gConfigParser -> readStringOption("Input::InputVariableRespList");
  vector<variableContainer> variableRespList;

  if(ReadInputVariableFile(InputVariableRespList,variableRespList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters                                                                                                                                             
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)                                                                                                   
  lumi *= 1000. ;   // transform into pb^(-1)                                                                                                                                 

  string finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  float  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone");
  float  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  float  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
 
  int   nLeptons   = gConfigParser -> readIntOption("Option::nLeptons");
  int   nPU        = gConfigParser -> readIntOption("Option::nPU");
  float metCut     = gConfigParser -> readFloatOption("Option::metCut");
  float mllCut     = gConfigParser -> readFloatOption("Option::mllCut");
  float mjjCut     = gConfigParser -> readFloatOption("Option::mjjCut");
  float detajjCut  = gConfigParser -> readFloatOption("Option::detajjCut");

  // output directory                                                                                                                                                           
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());
  system(("mkdir -p output/"+outputPlotDirectory+"/norm").c_str());
  system(("mkdir -p output/"+outputPlotDirectory+"/xs").c_str());

  ///// Start the analysis for phaseI aged detector                                                                                                                          
  // make chain and branches for Aged trees

  TChain* chain_FullSIM = new TChain (treeNameFullSIM.c_str()) ;
  chain_FullSIM->Add ((InputBaseDirectory_FullSIM+"/*.root").c_str()) ;
  int totEvent_FullSIM = chain_FullSIM->GetEntries();

  TEventInfo   *fEventInfo_FullSIM = new TEventInfo(); 
  TClonesArray *fElectron_FullSIM = new TClonesArray("baconhep::TElectron");
  TClonesArray *fMuon_FullSIM     = new TClonesArray("baconhep::TMuon");
  TClonesArray *fJet_FullSIM      = new TClonesArray("baconhep::TJet");
  TClonesArray *fGenParticle_FullSIM = new TClonesArray("baconhep::TGenParticle");

  chain_FullSIM->SetBranchAddress("Info",&fEventInfo_FullSIM);
  chain_FullSIM->SetBranchAddress("Electron", &fElectron_FullSIM);
  chain_FullSIM->SetBranchAddress("Muon", &fMuon_FullSIM);
  chain_FullSIM->SetBranchAddress("Jet04", &fJet_FullSIM);
  chain_FullSIM->SetBranchAddress("GenParticle", &fGenParticle_FullSIM);

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_FullSIM<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_FullSIM)<<endl;

  float weight_FullSIM = 1.0*lumi*CrossSection/float(totEvent_FullSIM) ;

  // make chain and branches for Shashlik trees    
  TChain* chain_Delphes = new TChain (treeNameDelphes.c_str()) ;
  chain_Delphes->Add ((InputBaseDirectory_Delphes+"/*.root").c_str()) ;
  int totEvent_Delphes = chain_Delphes->GetEntries();

  readTree* readerDelphes  = new readTree((TTree*)(chain_Delphes));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_Delphes<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_Delphes)<<endl;

  float weight_Delphes = 1.0*lumi*CrossSection/float(totEvent_Delphes) ;

  TH1::SetDefaultSumw2();

  // Efficiency histograms
  map<string,TH1F*> histoCutEff_FullSIM ;
  map<string,TH1F*> histoCutEff_Delphes ;

  vector<histoContainer> plotVector_FullSIM;
  vector<histoContainer> plotVector_Delphes;
  vector<histoContainer> plotVector_GenFullSIM;
  vector<histoContainer> plotVector_GenDelphes;

  vector<histoContainer> plotResponse_FullSIM;
  vector<histoContainer> plotResponse_Delphes;

  histoCutEff_FullSIM["FullSIM"] = new TH1F("FullSIM","",9,0,9);
  histoCutEff_Delphes["Delphes"] = new TH1F("Delphes","",9,0,9);

  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    plotVector_FullSIM.push_back(histoContainer("FullSIM",variableList.at(iVar)));
    plotVector_Delphes.push_back(histoContainer("Delphes",variableList.at(iVar)));
    plotVector_GenFullSIM.push_back(histoContainer("GenFullSIM",variableList.at(iVar)));
    plotVector_GenDelphes.push_back(histoContainer("GenDelphes",variableList.at(iVar)));
   }

  for(size_t iVar = 0; iVar < variableRespList.size(); iVar++){
    plotResponse_FullSIM.push_back(histoContainer("FullSIMResponse",variableRespList.at(iVar)));
    plotResponse_Delphes.push_back(histoContainer("DelphesResponse",variableRespList.at(iVar)));
  }

  int maximumEvents_FullSIM = chain_FullSIM->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_FullSIM)
    maximumEvents_FullSIM = maxEventNumber ;

  int maximumEvents_Delphes = chain_Delphes->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_Delphes)
    maximumEvents_Delphes = maxEventNumber ;


  /// VBF jet tagging efficiency/ purity plots
  TH1F* Numerator_LeadJet_Eff_FullSIM_vsEta = new TH1F("Numerator_LeadJet_Eff_FullSIM_vsEta","Numerator_LeadJet_Eff_FullSIM_vsEta",10,0,5);
  Numerator_LeadJet_Eff_FullSIM_vsEta->Sumw2();

  TH1F* Numerator_TrailJet_Eff_FullSIM_vsEta = new TH1F("Numerator_TrailJet_Eff_FullSIM_vsEta","Numerator_TrailJet_Eff_FullSIM_vsEta",10,0,5);
  Numerator_TrailJet_Eff_FullSIM_vsEta->Sumw2();

  TH1F* Numerator_Eff_FullSIM_vsDEta = new TH1F("Numerator_Eff_FullSIM_vsDEta","Numerator_Eff_FullSIM_vsDEta",10,2,8);
  Numerator_Eff_FullSIM_vsDEta->Sumw2();

  TH1F* Numerator_Eff_FullSIM_vsMjj = new TH1F("Numerator_Eff_FullSIM_vsMjj","Numerator_Eff_FullSIM_vsMjj",15,500,2000);
  Numerator_Eff_FullSIM_vsMjj->Sumw2();

  TH1F* Denominator_LeadJet_Eff_FullSIM_vsEta = new TH1F("Denominator_LeadJet_Eff_FullSIM_vsEta","Denominator_LeadJet_Eff_FullSIM_vsEta",10,0,5);
  Denominator_LeadJet_Eff_FullSIM_vsEta->Sumw2();

  TH1F* Denominator_TrailJet_Eff_FullSIM_vsEta = new TH1F("Denominator_TrailJet_Eff_FullSIM_vsEta","Denominator_TrailJet_Eff_FullSIM_vsEta",10,0,5);
  Denominator_TrailJet_Eff_FullSIM_vsEta->Sumw2();

  TH1F* Denominator_Eff_FullSIM_vsDEta = new TH1F("Denominator_Eff_FullSIM_vsDEta","Denominator_Eff_FullSIM_vsDEta",10,2,8);
  Denominator_Eff_FullSIM_vsDEta->Sumw2();

  TH1F* Denominator_Eff_FullSIM_vsMjj = new TH1F("Denominator_Eff_FullSIM_vsMjj","Denominator_Eff_FullSIM_vsMjj",15,500,2000);
  Denominator_Eff_FullSIM_vsMjj->Sumw2();



  TH1F* Numerator_LeadJet_Eff_Delphes_vsEta = new TH1F("Numerator_LeadJet_Eff_Delphes_vsEta","Numerator_LeadJet_Eff_Delphes_vsEta",10,0,5);
  Numerator_LeadJet_Eff_Delphes_vsEta->Sumw2();

  TH1F* Numerator_TrailJet_Eff_Delphes_vsEta = new TH1F("Numerator_TrailJet_Eff_Delphes_vsEta","Numerator_TrailJet_Eff_Delphes_vsEta",10,0,5);
  Numerator_TrailJet_Eff_Delphes_vsEta->Sumw2();

  TH1F* Numerator_Eff_Delphes_vsDEta = new TH1F("Numerator_Eff_Delphes_vsDEta","Numerator_Eff_Delphes_vsDEta",10,2,8);
  Numerator_Eff_Delphes_vsDEta->Sumw2();

  TH1F* Numerator_Eff_Delphes_vsMjj = new TH1F("Numerator_Eff_Delphes_vsMjj","Numerator_Eff_Delphes_vsMjj",15,500,2000);
  Numerator_Eff_Delphes_vsMjj->Sumw2();

  TH1F* Denominator_LeadJet_Eff_Delphes_vsEta = new TH1F("Denominator_LeadJet_Eff_Delphes_vsEta","Denominator_LeadJet_Eff_Delphes_vsEta",10,0,5);
  Denominator_LeadJet_Eff_Delphes_vsEta->Sumw2();

  TH1F* Denominator_TrailJet_Eff_Delphes_vsEta = new TH1F("Denominator_TrailJet_Eff_Delphes_vsEta","Denominator_TrailJet_Eff_Delphes_vsEta",10,0,5);
  Denominator_TrailJet_Eff_Delphes_vsEta->Sumw2();

  TH1F* Denominator_Eff_Delphes_vsDEta = new TH1F("Denominator_Eff_Delphes_vsDEta","Denominator_Eff_Delphes_vsDEta",10,2,8);
  Denominator_Eff_Delphes_vsDEta->Sumw2();

  TH1F* Denominator_Eff_Delphes_vsMjj = new TH1F("Denominator_Eff_Delphes_vsMjj","Denominator_Eff_Delphes_vsMjj",15,500,2000);
  Denominator_Eff_Delphes_vsMjj->Sumw2();


  // Loop on aged events
  for(int iEvent = 0; iEvent < maximumEvents_FullSIM ; iEvent++){
    
    if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ;
    string name = "FullSIM";
    
    chain_FullSIM->GetEntry(iEvent);

    // filter away event with taus
    bool eventWithTau = false ;

    for( int iGen = 0; iGen < fGenParticle_FullSIM->GetEntriesFast() ; ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_FullSIM)[iGen]);
	if( abs(genP->pdgId) == 15 or abs(genP->pdgId) == 16) {
	  bool foundW = false, stuck = false;
	  do {
	    if(genP->parent!=-1) {
	      TGenParticle *genTemp = (TGenParticle*)((*fGenParticle_FullSIM)[genP->parent]);
	      genP = genTemp;
	    }
	    else stuck = true;
	    if( abs(genP->pdgId) == 24) {
	      foundW = true;
	    }
	  } while( !stuck && !foundW );
	  if( foundW ) {
	    eventWithTau = true;
	    break;
	  }
	}
    }

    if(eventWithTau) continue;
    
    // TElectron and TMuon are all the reco lepton to be used in the analysis --> apply some basic ID cuts
    vector<TMuon> goodTightMuons ;
    vector<TElectron> goodTightElectrons ;

    vector<TMuon> goodLooseMuons ;
    vector<TElectron> goodLooseElectrons ;

    if((fMuon_FullSIM->GetEntriesFast()+fElectron_FullSIM->GetEntriesFast()) < nLeptons) continue;
    if((fJet_FullSIM->GetEntriesFast()) < 2) continue;

    // look for tight muons and electrons in the event
    for(int iEle = 0; iEle < fElectron_FullSIM->GetEntriesFast(); iEle++){
      TElectron *ele = (TElectron*)((*fElectron_FullSIM)[iEle]);
      if(ele->pt < minLeptonCutPt) continue;
      if(fabs(ele->eta) >= 2.5) continue;
      if(passEleID(ele,fEventInfo_FullSIM->rhoIso,nPU))
	 goodTightElectrons.push_back(*ele);
    }

    for(int iMu = 0; iMu < fMuon_FullSIM->GetEntriesFast(); iMu++){
      TMuon *mu = (TMuon*)((*fMuon_FullSIM)[iMu]);
      if(mu->pt < minLeptonCutPt) continue;
      if(fabs(mu->eta) >= 2.5) continue;
      if(passMuonID(mu,fEventInfo_FullSIM->rhoIso,nPU))
	 goodTightMuons.push_back(*mu);
    }


    for(int iEle = 0; iEle < fElectron_FullSIM->GetEntriesFast(); iEle++){
      TElectron *ele = (TElectron*)((*fElectron_FullSIM)[iEle]);
      if(ele->pt < minLeptonCutPt) continue;
      if(fabs(ele->eta) >= 2.5) continue;
      if(passEleIDLoose(ele,fEventInfo_FullSIM->rhoIso,nPU))
	 goodLooseElectrons.push_back(*ele);
    }

    for(int iMu = 0; iMu < fMuon_FullSIM->GetEntriesFast(); iMu++){
      TMuon *mu = (TMuon*)((*fMuon_FullSIM)[iMu]);
      if(mu->pt < minLeptonCutPt) continue;
      if(fabs(mu->eta) >= 2.5) continue;
      if(passMuonIDLoose(mu,fEventInfo_FullSIM->rhoIso,nPU))
	 goodLooseMuons.push_back(*mu);
    }

    // sort in pt
    sort(goodTightMuons.rbegin(),goodTightMuons.rend());
    sort(goodTightElectrons.rbegin(),goodTightElectrons.rend());
    sort(goodLooseMuons.rbegin(),goodLooseMuons.rend());
    sort(goodLooseElectrons.rbegin(),goodLooseElectrons.rend());


    // apply a cut on the number of tight leptons and final state topology
    if(int(goodTightElectrons.size()+goodTightMuons.size()) != nLeptons) continue;
    
    int nLooseLepton = 0;

    for(size_t iEle = 0; iEle < goodLooseElectrons.size(); iEle++){
      bool isGoodLooseLepton = true;
      for(size_t jEle = 0; jEle < goodTightElectrons.size(); jEle++){
	if(goodLooseElectrons.at(iEle).pt == goodTightElectrons.at(jEle).pt and goodLooseElectrons.at(iEle).eta == goodTightElectrons.at(jEle).eta and goodLooseElectrons.at(iEle).phi == goodTightElectrons.at(jEle).phi) 
	  isGoodLooseLepton = false;
      }
      if(isGoodLooseLepton) nLooseLepton++;
    }

    for(size_t iMu = 0; iMu < goodLooseMuons.size(); iMu++){
      bool isGoodLooseLepton = true;
      for(size_t jMu = 0; jMu < goodTightMuons.size(); jMu++){
	if(goodLooseMuons.at(iMu).pt == goodTightMuons.at(jMu).pt and goodLooseMuons.at(iMu).eta == goodTightMuons.at(jMu).eta and goodLooseMuons.at(iMu).phi == goodTightMuons.at(jMu).phi) 
	  isGoodLooseLepton = false;
      }
      if(isGoodLooseLepton) nLooseLepton++;
    }

    if(nLooseLepton > 0 ) continue;
    
    int iBin = 1;
    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }

    
    // select final state topology    
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.size() != 2) continue;
      if(goodTightElectrons.size() != 0) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.size() != 2) continue;
      if(goodTightMuons.size() != 0) continue;
    }
    else if(TString(finalStateString).Contains("All")){
      if(goodTightMuons.size() == 2){
	if(goodTightElectrons.size() != 0) continue;
      }
      else if(goodTightElectrons.size() == 2){
	if(goodTightMuons.size() != 0) continue;
      } 
      else continue;
    }
    else continue;

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }
    
    // charge : same sign
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.at(0).q != goodTightMuons.at(1).q) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.at(0).q != goodTightElectrons.at(1).q) continue;
    }
    else if(TString(finalStateString).Contains("All")){
      if(goodTightElectrons.size() == 2){
	if(goodTightElectrons.at(0).q != goodTightElectrons.at(1).q) continue;
      }
      else if(goodTightMuons.size() == 2){
	if(goodTightMuons.at(0).q != goodTightMuons.at(1).q) continue;
      }    
      else continue;
    }
    else continue;

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }

    // met cut
    if(fEventInfo_FullSIM->pfMETC < metCut) continue;

    TLorentzVector met ; met.SetPtEtaPhiM(fEventInfo_FullSIM->pfMETC,0.,fEventInfo_FullSIM->pfMETCphi,0.);

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"met cut");
      iBin++;
    }

    // mll cut
    TLorentzVector lepton1, lepton2;
    if(TString(finalStateString).Contains("UU")){
      lepton1.SetPtEtaPhiM(goodTightMuons.at(0).pt,goodTightMuons.at(0).eta,goodTightMuons.at(0).phi,0.);
      lepton2.SetPtEtaPhiM(goodTightMuons.at(1).pt,goodTightMuons.at(1).eta,goodTightMuons.at(1).phi,0.);
    }
    else if(TString(finalStateString).Contains("EE")){
      lepton1.SetPtEtaPhiM(goodTightElectrons.at(0).pt,goodTightElectrons.at(0).eta,goodTightElectrons.at(0).phi,0.);
      lepton2.SetPtEtaPhiM(goodTightElectrons.at(1).pt,goodTightElectrons.at(1).eta,goodTightElectrons.at(1).phi,0.);
    }
    else if(TString(finalStateString).Contains("All")){
      if(goodTightMuons.size() == 2){
	lepton1.SetPtEtaPhiM(goodTightMuons.at(0).pt,goodTightMuons.at(0).eta,goodTightMuons.at(0).phi,0.);
	lepton2.SetPtEtaPhiM(goodTightMuons.at(1).pt,goodTightMuons.at(1).eta,goodTightMuons.at(1).phi,0.);
      }
      else if (goodTightElectrons.size() == 2){
	lepton1.SetPtEtaPhiM(goodTightElectrons.at(0).pt,goodTightElectrons.at(0).eta,goodTightElectrons.at(0).phi,0.);
	lepton2.SetPtEtaPhiM(goodTightElectrons.at(1).pt,goodTightElectrons.at(1).eta,goodTightElectrons.at(1).phi,0.);
      }
    }

    if((lepton1+lepton2).M() < mllCut) continue;

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"mll cut");
      iBin++;
    }


    // matching with generator level leptons : find gen muons and gene ele from W decay
    vector<int> genEleIndex ;
    vector<int> genMuIndex ;
    findGenLeptonsFromW(fGenParticle_FullSIM,genEleIndex,genMuIndex);

    vector<TGenParticle> genMatchedMuons;
    vector<TGenParticle> genMatchedElectrons;
    vector<TGenParticle> genNeutrino;
    
    findGenNeutrinoFromW(fGenParticle_FullSIM,genNeutrino);

    for(size_t iMu = 0; iMu < goodTightMuons.size(); iMu++){
      TMuon mu = goodTightMuons.at(iMu);
      TLorentzVector mu4V;
      mu4V.SetPtEtaPhiM(mu.pt,mu.eta,mu.phi,0.);
      // Match with genParticle	
      float minDR = 999;
      for( vector<int>::iterator iGen = genMuIndex.begin(); iGen != genMuIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_FullSIM)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(mu4V.DeltaR(gen4V) < matchingConeLepton){
	  if(minDR != 999)
	    cerr<<"found another genMuon matching reco muon "<<endl;
	  if(mu4V.DeltaR(gen4V) < minDR ){
	    genMatchedMuons.push_back(*genP);
	    minDR = mu4V.DeltaR(gen4V);
	  }
	}
      }
    }
   
    for(size_t iEle = 0; iEle < goodTightElectrons.size(); iEle++){
      TElectron ele = goodTightElectrons.at(iEle);
      TLorentzVector ele4V;
      ele4V.SetPtEtaPhiM(ele.pt,ele.eta,ele.phi,0.);
      // Match with genParticle	
      float minDR = 999;
      for( vector<int>::iterator iGen = genEleIndex.begin(); iGen != genEleIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_FullSIM)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(ele4V.DeltaR(gen4V) < matchingConeLepton){
	  if(minDR != 999)
	    cerr<<"found another genElectron matching reco electron "<<endl;
	  if(ele4V.DeltaR(gen4V) < minDR ){
	    genMatchedElectrons.push_back(*genP);
	    minDR = ele4V.DeltaR(gen4V);
	  }
	}
      }
    }
   
    if (int(genMatchedElectrons.size() + genMatchedMuons.size()) != nLeptons) continue;

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen lepton");
      iBin++;
    }
    
    TLorentzVector genLep1, genLep2;
    if(TString(finalStateString).Contains("UU")){
      genLep1.SetPtEtaPhiM(genMatchedMuons.at(0).pt,genMatchedMuons.at(0).eta,genMatchedMuons.at(0).phi,0.);
      genLep2.SetPtEtaPhiM(genMatchedMuons.at(1).pt,genMatchedMuons.at(1).eta,genMatchedMuons.at(1).phi,0.);
    }
    else if(TString(finalStateString).Contains("EE")){
      genLep1.SetPtEtaPhiM(genMatchedElectrons.at(0).pt,genMatchedElectrons.at(0).eta,genMatchedElectrons.at(0).phi,0.);
      genLep2.SetPtEtaPhiM(genMatchedElectrons.at(1).pt,genMatchedElectrons.at(1).eta,genMatchedElectrons.at(1).phi,0.);
    }
    else if(TString(finalStateString).Contains("All")){
      if(genMatchedElectrons.size() >= 2){
	genLep1.SetPtEtaPhiM(genMatchedElectrons.at(0).pt,genMatchedElectrons.at(0).eta,genMatchedElectrons.at(0).phi,0.);
	genLep2.SetPtEtaPhiM(genMatchedElectrons.at(1).pt,genMatchedElectrons.at(1).eta,genMatchedElectrons.at(1).phi,0.);
      }
      else if(genMatchedMuons.size() >=2){
	genLep1.SetPtEtaPhiM(genMatchedMuons.at(0).pt,genMatchedMuons.at(0).eta,genMatchedMuons.at(0).phi,0.);
	genLep2.SetPtEtaPhiM(genMatchedMuons.at(1).pt,genMatchedMuons.at(1).eta,genMatchedMuons.at(1).phi,0.);
      }
    }

    TLorentzVector genMet ;
    for(size_t iNu = 0; iNu < genNeutrino.size(); iNu++){
      TLorentzVector tmp4V;
      tmp4V.SetPtEtaPhiM(genNeutrino.at(iNu).pt,0.,genNeutrino.at(iNu).phi,0.);
      genMet += tmp4V ;
    }

    // look for jets cleaning leptons
    vector<TJet> cleanedJetsNoLep;    
    vector<TJet> cleanedJets;    
    cleanedJetsFromLeptons(cleanedJets,*fJet_FullSIM,goodTightMuons,goodTightElectrons,minJetCutPt,matchingCone,etaJetCut); 
    //cleanedJesFromNeutrino(cleanedJets,cleanedJetsNoLep,genNeutrino,minJetCutPt,matchingCone,etaJetCut);
    sort(cleanedJets.rbegin(),cleanedJets.rend());
    if(cleanedJets.size() < 2) continue;
    
    TLorentzVector jet1, jet2, dijet ;
    jet1.SetPtEtaPhiM(cleanedJets.at(0).pt,cleanedJets.at(0).eta,cleanedJets.at(0).phi,cleanedJets.at(0).mass);
    jet2.SetPtEtaPhiM(cleanedJets.at(1).pt,cleanedJets.at(1).eta,cleanedJets.at(1).phi,cleanedJets.at(1).mass);

    dijet = jet1+jet2;

    // matching with gen jets by the flag

    TLorentzVector genjet1, genjet2;
    genjet1.SetPtEtaPhiM(cleanedJets.at(0).genpt,cleanedJets.at(0).geneta,cleanedJets.at(0).genphi,cleanedJets.at(0).genm);
    genjet2.SetPtEtaPhiM(cleanedJets.at(1).genpt,cleanedJets.at(1).geneta,cleanedJets.at(1).genphi,cleanedJets.at(1).genm);


    // cuts
    if(dijet.M() < mjjCut) continue;
    
    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"mjj cut");
      iBin++;
    }

    if(fabs(jet1.Eta()-jet2.Eta()) < detajjCut) continue;
    
    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"detajj cut");
      iBin++;
    }

    // efficiency and purity plots :
    Denominator_LeadJet_Eff_FullSIM_vsEta->Fill(fabs(jet1.Eta()));
    Denominator_TrailJet_Eff_FullSIM_vsEta->Fill(fabs(jet2.Eta()));

    if(genjet1.Pt() > 15 and jet1.DeltaR(genjet1) < 0.25)
      Numerator_LeadJet_Eff_FullSIM_vsEta->Fill(fabs(jet1.Eta()));
    if(genjet2.Pt() > 15 and jet2.DeltaR(genjet2) < 0.25)
      Numerator_TrailJet_Eff_FullSIM_vsEta->Fill(fabs(jet2.Eta()));

    Denominator_Eff_FullSIM_vsDEta->Fill(fabs(jet1.Eta()-jet2.Eta()));
    Denominator_Eff_FullSIM_vsMjj->Fill((jet1+jet2).M());

    if(genjet1.Pt() > 15 and genjet2.Pt() > 15 and jet1.DeltaR(genjet1) < 0.25 and jet2.DeltaR(genjet2) < 0.25){
      Numerator_Eff_FullSIM_vsDEta->Fill(fabs(jet1.Eta()-jet2.Eta()));
      Numerator_Eff_FullSIM_vsMjj->Fill((jet1+jet2).M());
    }

    if(genjet1.Pt() < 15. or genjet2.Pt() < 15) continue;
    if(jet1.DeltaR(genjet1) > 0.25 or jet2.DeltaR(genjet2) > 0.25) continue; 

    if(histoCutEff_FullSIM.size()!=0){
      histoCutEff_FullSIM[name]->SetBinContent(iBin,histoCutEff_FullSIM[name]->GetBinContent(iBin)+1);
      histoCutEff_FullSIM[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen jets after cleaning");
     iBin++;
    }

    // fill histos
    fillHistos(plotVector_FullSIM,variableList,name,weight_FullSIM,lepton1,lepton2,jet1,jet2,met);
    fillHistos(plotVector_GenFullSIM,variableList,"Gen"+name,weight_FullSIM,genLep1,genLep2,genjet1,genjet2,genMet);

    // fill response
    fillResponse(plotResponse_FullSIM,variableRespList,name+"Response",weight_FullSIM,lepton1,genLep1,lepton2,genLep2,jet1,genjet1,jet2,genjet2,met,genMet);
    
  }
    
  // Loop on Delphes events
  for(int iEvent = 0; iEvent < maximumEvents_Delphes ; iEvent++){
    
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;
    string name = "Delphes";
    
    chain_Delphes->GetEntry(iEvent);

    // filter way taus from the event
    if(fabs(readerDelphes->leptonLHEpid1) == 15 or fabs(readerDelphes->leptonLHEpid2) == 15 or fabs(readerDelphes->neutrinoLHEpid1) == 16 or fabs(readerDelphes->neutrinoLHEpid2) == 16) continue;

    // dump all the lepton in the event                                                                                                                                       
    vector<leptonContainer> LeptonsAll;
    fillRecoLeptonsArray (LeptonsAll, *readerDelphes);

    // dump tight leptons                                                                                                                                                      
    vector<leptonContainer> leptonsIsoTight ;
    if( nPU == 50)
      leptonsIsoTight = dumpLeptons (LeptonsAll, 0.38, 0.38, minLeptonCutPt);
    else
      leptonsIsoTight = dumpLeptons (LeptonsAll, 0.6, 0.6, minLeptonCutPt);

    // identify loose leptons                                                                                                                                                  
    vector<leptonContainer> leptonsIsoLoose ;
    if( nPU == 50)
      leptonsIsoLoose = dumpLeptons (LeptonsAll, 0.40, minLeptonCutPt);
    else
      leptonsIsoLoose = dumpLeptons (LeptonsAll, 0.75, minLeptonCutPt);

    // take reco jets                                                                                                                                                        
    vector<jetContainer> RecoJetsAll ;
    fillRecoJetArray (RecoJetsAll, *readerDelphes) ;


    if(LeptonsAll.size()  < 2)  continue ; // skip the event --> only two reco leptons are good                                                                           
    if(RecoJetsAll.size() < 2)  continue ; // skip the event with less than two reco jet                                                                                
    
    if (int(leptonsIsoTight.size()) != nLeptons ) continue;
    if (fabs(leptonsIsoTight.at(0).lepton4V_.Eta()) >= 2.5) continue;

    int extraLepton = 0; // count the extra lepton number                                                                                                                          
    for(size_t iLepton = 0; iLepton < leptonsIsoLoose.size() ; iLepton++){
      if(leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(0).lepton4V_ or leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(1).lepton4V_) continue; 
      if(leptonsIsoLoose.at(iLepton).lepton4V_.Eta() >= 2.5) continue ;
      extraLepton++;
    }

    if(extraLepton > 0) continue;

    bool badTrailingLepton = false;
    for( size_t iLep = 1 ; iLep < leptonsIsoTight.size(); iLep++){
      if( leptonsIsoTight.at(iLep).lepton4V_.Pt() < minLeptonCutPt or fabs(leptonsIsoTight.at(iLep).lepton4V_.Eta()) >= 2.5){
        badTrailingLepton = true;
      }
    }
    if(badTrailingLepton) continue;

    int iBin = 1;
    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }
    
    // flavor
    if(TString(finalStateString).Contains("UU")){
      if(fabs(leptonsIsoTight.at(0).flavour_) != 13) continue;
      if(fabs(leptonsIsoTight.at(1).flavour_) != 13) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(fabs(leptonsIsoTight.at(0).flavour_) != 11) continue;
      if(fabs(leptonsIsoTight.at(1).flavour_) != 11) continue;
    }
    else if(TString(finalStateString).Contains("All")){
      if(fabs(leptonsIsoTight.at(0).flavour_) !=  fabs(leptonsIsoTight.at(1).flavour_)) continue; 
    }
    else continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }
    
    // Charge cut for same sign final state : zero means no cut, one means same sign, -1 opposite sign                                                                      
    int sign = 1;
    for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns                                                                             
      sign *= leptonsIsoTight.at(leptSize).charge_;
    if(sign != 1) continue;
      
    if(TString(finalStateString).Contains("pp") and (leptonsIsoTight.at(0).charge_ < 0 or leptonsIsoTight.at(1).charge_ < 0))
      continue;

    if(TString(finalStateString).Contains("mm") and (leptonsIsoTight.at(0).charge_ > 0 or leptonsIsoTight.at(1).charge_ > 0))
      continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }

    // met cut                                                                                                                                                                 
    if(readerDelphes->pfmet < metCut) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"met selection");
      iBin++;
    }

    TLorentzVector met ; met.SetPtEtaPhiM(readerDelphes->pfmet,0.,readerDelphes->pfmetphi,0.);
    TLorentzVector genMet ; genMet.SetPtEtaPhiM(readerDelphes->metGenpt,0.,readerDelphes->metGenphi,0.);


    ///////// Lorentz Vector                                                                                                                                           
    TLorentzVector L_dilepton;
    L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;

    // apply Mll cut                                                                                                                                                        
    if(L_dilepton.M() < mllCut) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"Mll");
      iBin++;
    }

    // mathing with gen leptons
    vector<leptonContainer> genLeptons;
    fillGenLeptonsArray (genLeptons, *readerDelphes);
    sort(genLeptons.rbegin(),genLeptons.rend());

    if(int(genLeptons.size()) != nLeptons) continue;

    vector<int> matchedWith ;

    for(size_t iLep = 0; iLep < leptonsIsoTight.size(); iLep++){
      float minDR = 999;
      for(size_t iGenLep = 0; iGenLep < genLeptons.size(); iGenLep++){
	if(leptonsIsoTight.at(iLep).lepton4V_.DeltaR(genLeptons.at(iGenLep).lepton4V_) < matchingConeLepton){
	  if(minDR!= 999){
	    cerr<<" found another gen lepton matched to the reco one --> delphes"<<minDR<<" "<<leptonsIsoTight.at(iLep).lepton4V_.DeltaR(genLeptons.at(iGenLep).lepton4V_)<<" pt "<<genLeptons.at(iGenLep).lepton4V_.Pt()<<" iLep "<<iLep<<endl;
	  }
	  if(leptonsIsoTight.at(iLep).lepton4V_.DeltaR(genLeptons.at(iGenLep).lepton4V_) < minDR){
	    matchedWith.push_back(iGenLep);
	    minDR = leptonsIsoTight.at(iLep).lepton4V_.DeltaR(genLeptons.at(iGenLep).lepton4V_);
	  }
	}		  
      }
    }

    if(matchedWith.size() != leptonsIsoTight.size()) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen lepton");
      iBin++;
    }

    TLorentzVector genLepton1 = genLeptons.at(matchedWith.at(0)).lepton4V_ ;
    TLorentzVector genLepton2 = genLeptons.at(matchedWith.at(1)).lepton4V_ ;

     
    vector<jetContainer> RecoJets;
    RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., -99, minLeptonCutPt, matchingCone,etaJetCut);
    sort(RecoJets.rbegin(),RecoJets.rend());
    

    if(RecoJets.size() < 2 )  continue;

    vector<jetContainer> GenJetsAll ;
    fillGenJetArray (GenJetsAll, *readerDelphes) ;

    // take gen jets                                                                                                                                                           
    vector<jetContainer> GenJets;
    vector<leptonContainer> leptTmp;
    GenJets  = dumpJets (GenJetsAll, leptTmp,0., 999, -999, 0., matchingCone, 5.0);
    sort(GenJets.rbegin(),GenJets.rend());

    TLorentzVector genJet1 ; genJet1.SetPtEtaPhiM(0.,0.,0.,0.);
    TLorentzVector genJet2 ; genJet2.SetPtEtaPhiM(0.,0.,0.,0.);

    float minDR = 999 ;

    for(size_t iGen = 0; iGen < GenJets.size(); iGen++){
      if(RecoJets.at(0).jet4V_.DeltaR(GenJets.at(iGen).jet4V_) < matchingCone) {
	//	if(minDR != 999)
	  //cerr<<" more than one gen jet matched to reco leading jet in delphes "<<minDR<<" "<<RecoJets.at(0).jet4V_.DeltaR(GenJets.at(iGen).jet4V_)<<" pt "<<GenJets.at(iGen).jet4V_.Pt()<<" postion "<<iGen<<endl;
	if(RecoJets.at(0).jet4V_.DeltaR(GenJets.at(iGen).jet4V_) < minDR){
	  genJet1 = GenJets.at(iGen).jet4V_;
	  minDR = RecoJets.at(0).jet4V_.DeltaR(GenJets.at(iGen).jet4V_);
	}
      }
    }

    minDR = 999 ;

    for(size_t iGen = 0; iGen < GenJets.size(); iGen++){
      if(RecoJets.at(1).jet4V_.DeltaR(GenJets.at(iGen).jet4V_) < matchingCone and GenJetsAll.at(iGen).jet4V_ != genJet1) {
	//	if(minDR != 999)
	  //cerr<<" more than one gen jet matched to reco trailing jet in delphes "<<minDR<<" "<<RecoJets.at(1).jet4V_.DeltaR(GenJets.at(iGen).jet4V_)<<" pt "<<GenJets.at(iGen).jet4V_.Pt()<<" postion "<<iGen<<endl;
	if(RecoJets.at(1).jet4V_.DeltaR(GenJets.at(iGen).jet4V_) < minDR){
	  genJet2 = GenJets.at(iGen).jet4V_;
	  minDR = RecoJets.at(1).jet4V_.DeltaR(GenJets.at(iGen).jet4V_);
	}
      }
    }

    // cuts
    if((RecoJets.at(0).jet4V_+RecoJets.at(1).jet4V_).M() < mjjCut) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"mjj cut");
      iBin++;
    }

    if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < detajjCut) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"detajj cut");
      iBin++;
    }

    // efficiency and purity plots :
    Denominator_LeadJet_Eff_Delphes_vsEta->Fill(fabs(RecoJets.at(0).jet4V_.Eta()));
    Denominator_TrailJet_Eff_Delphes_vsEta->Fill(fabs(RecoJets.at(1).jet4V_.Eta()));

    if(genJet1.Pt() > 0)
      Numerator_LeadJet_Eff_Delphes_vsEta->Fill(fabs(RecoJets.at(0).jet4V_.Eta()));
    if(genJet2.Pt() > 0)
      Numerator_TrailJet_Eff_Delphes_vsEta->Fill(fabs(RecoJets.at(1).jet4V_.Eta()));

    Denominator_Eff_Delphes_vsDEta->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()));
    Denominator_Eff_Delphes_vsMjj->Fill((RecoJets.at(0).jet4V_+RecoJets.at(1).jet4V_).M());

    if(genJet1.Pt() > 0 and genJet2.Pt() > 0){
      Numerator_Eff_Delphes_vsDEta->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()));
      Numerator_Eff_Delphes_vsMjj->Fill((RecoJets.at(0).jet4V_+RecoJets.at(1).jet4V_).M());
    }

    if(genJet1.Pt() <= 0 or genJet2.Pt() <= 0) continue;

    if(histoCutEff_Delphes.size()!=0){
      histoCutEff_Delphes[name]->SetBinContent(iBin,histoCutEff_Delphes[name]->GetBinContent(iBin)+1);
      histoCutEff_Delphes[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen jets after cleaning");
      iBin++;
    }


    // fill histos                                                                                                                                                             
    fillHistos(plotVector_Delphes,variableList,name,weight_Delphes,
	       leptonsIsoTight.at(0).lepton4V_,leptonsIsoTight.at(1).lepton4V_,
	       RecoJets.at(0).jet4V_,RecoJets.at(1).jet4V_,
	       met);

    fillHistos(plotVector_GenDelphes,variableList,"Gen"+name,weight_Delphes,
	       genLepton1,genLepton2,
	       genJet1,genJet2,
	       genMet);

    // fill response                                                                                                                                                         
    fillResponse(plotResponse_Delphes,variableRespList,name+"Response",weight_Delphes,
		 leptonsIsoTight.at(0).lepton4V_,genLepton1,leptonsIsoTight.at(1).lepton4V_,genLepton2,
		 RecoJets.at(0).jet4V_,genJet1,RecoJets.at(1).jet4V_,genJet2,met,genMet);
    
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

  TLatex * tex2 = new TLatex(0.14,0.92,"CMS");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.045);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.28,0.92,"Simulation Preliminary");
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

  // store efficiency histograms
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  vector<TH1F*> efficiencyRatio;

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_FullSIM.begin(); itMap !=  histoCutEff_FullSIM.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_FullSIM));
    efficiencyRatio.push_back((TH1F*) itMap->second->Clone((itMap->first+"_ratio").c_str()));
    itMap->second->GetYaxis()->SetRangeUser(0.,itMap->second->GetMaximum()*1.3);
    itMap->second->Draw("hist text");    
    itMap->second->SetLineColor(kBlue);
    itMap->second->SetLineWidth(2);
    itMap->second->GetYaxis()->SetTitle("Efficiency");
    itMap->second->GetYaxis()->SetTitleOffset(1.15);
    legend->AddEntry(itMap->second,"Full SIM","l");
    itMap->second->Write();
    
  }

  int iHisto = 0 ;
  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_Delphes.begin(); itMap !=  histoCutEff_Delphes.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_Delphes));
    legend->AddEntry(itMap->second,"Delphes","l");
    itMap->second->Draw("hist same");
    itMap->second->SetLineColor(kRed);
    itMap->second->SetLineWidth(2);
    itMap->second->SetLineStyle(7);
    itMap->second->Write();
    efficiencyRatio.at(iHisto)->Divide(itMap->second);
    efficiencyRatio.at(iHisto)->GetYaxis()->SetRangeUser(efficiencyRatio.at(iHisto)->GetMinimum()*0.8,efficiencyRatio.at(iHisto)->GetMaximum()*1.2);
    efficiencyRatio.at(iHisto)->Write();
    iHisto++;
  }

  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
  legend->Draw("same");

  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency.png").c_str(),"png");  
  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency.pdf").c_str(),"pdf");  

  cCanvas->Update();
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
  efficiencyRatio.at(0)->GetYaxis()->SetTitle("efficiency ratio");
  efficiencyRatio.at(0)->SetLineWidth(2);
  efficiencyRatio.at(0)->Draw("hist");
  cCanvas->Update();
  
  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiencyRatio.png").c_str(),"png");
  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiencyRatio.pdf").c_str(),"pdf");


  legend->Clear();

  outputEfficiency->Close();


  // make the plot on the same canvas for each variable (legend entry is the cut layer name)                                                                                   
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var                                                                                                     
    histoContainer tmpPlot_FullSIM;
    tmpPlot_FullSIM.cutName = "FullSIM";
    tmpPlot_FullSIM.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_FullSIM ;
    itVec_FullSIM = find(plotVector_FullSIM.begin(),plotVector_FullSIM.end(),tmpPlot_FullSIM);
    if(itVec_FullSIM == plotVector_FullSIM.end()){
      cerr<<"Problem -->plot not found for Full SIM  "<<variableList.at(iVar).variableName<<endl;
    }

    itVec_FullSIM->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_FullSIM->histogram->GetXaxis()->SetTitleOffset(1.25);
    itVec_FullSIM->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,itVec_FullSIM->histogram->GetMaximum()*1.1);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_FullSIM->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_FullSIM->histogram->SetLineColor(kBlue);

    itVec_FullSIM->histogram->SetLineWidth(2);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleOffset(1.1);

    itVec_FullSIM->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    histoContainer tmpPlot_Delphes;
    tmpPlot_Delphes.cutName = "Delphes";
    tmpPlot_Delphes.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_Delphes ;
    itVec_Delphes = find(plotVector_Delphes.begin(),plotVector_Delphes.end(),tmpPlot_Delphes);
    if(itVec_Delphes == plotVector_Delphes.end()){
      cerr<<"Problem -->plot not found for Delphes  "<<variableList.at(iVar).variableName<<endl;
    }

    histoContainer tmpPlot_GenFullSIM;
    tmpPlot_GenFullSIM.cutName = "GenFullSIM";
    tmpPlot_GenFullSIM.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_GenFullSIM ;
    itVec_GenFullSIM = find(plotVector_GenFullSIM.begin(),plotVector_GenFullSIM.end(),tmpPlot_GenFullSIM);
    if(itVec_GenFullSIM == plotVector_GenFullSIM.end()){
      cerr<<"Problem -->plot not found for Delphes  "<<variableList.at(iVar).variableName<<endl;
    }


    histoContainer tmpPlot_GenDelphes;
    tmpPlot_GenDelphes.cutName = "GenDelphes";
    tmpPlot_GenDelphes.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_GenDelphes ;
    itVec_GenDelphes = find(plotVector_GenDelphes.begin(),plotVector_GenDelphes.end(),tmpPlot_GenDelphes);
    if(itVec_GenDelphes == plotVector_GenDelphes.end()){
      cerr<<"Problem -->plot not found for Delphes  "<<variableList.at(iVar).variableName<<endl;
    }


    itVec_Delphes->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_Delphes->histogram->GetXaxis()->SetTitleOffset(1.16);
    itVec_Delphes->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_Delphes->histogram->GetYaxis()->SetRangeUser(0.001,itVec_Delphes->histogram->GetMaximum()*1.1);
    itVec_Delphes->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_Delphes->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_Delphes->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_Delphes->histogram->SetLineColor(kRed);
    itVec_Delphes->histogram->SetLineWidth(2);
    itVec_Delphes->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    itVec_GenDelphes->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_GenDelphes->histogram->GetXaxis()->SetTitleOffset(1.16);
    itVec_GenDelphes->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_GenDelphes->histogram->GetYaxis()->SetRangeUser(0.001,itVec_GenDelphes->histogram->GetMaximum()*1.1);
    itVec_GenDelphes->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_GenDelphes->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_GenDelphes->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_GenDelphes->histogram->SetLineColor(kGreen+1);
    itVec_GenDelphes->histogram->SetLineWidth(2);
    itVec_GenDelphes->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    itVec_GenFullSIM->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_GenFullSIM->histogram->GetXaxis()->SetTitleOffset(1.16);
    itVec_GenFullSIM->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_GenFullSIM->histogram->GetYaxis()->SetRangeUser(0.001,itVec_GenFullSIM->histogram->GetMaximum()*1.1);
    itVec_GenFullSIM->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_GenFullSIM->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_GenFullSIM->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_GenFullSIM->histogram->SetLineColor(kOrange+1);
    itVec_GenFullSIM->histogram->SetLineWidth(2);
    itVec_GenFullSIM->histogram->GetYaxis()->SetTitle("#sigma x lumi");


    legend->AddEntry(itVec_FullSIM->histogram,"Reco CMSSW","l");
    legend->AddEntry(itVec_Delphes->histogram,"Reco Delphes","l");



    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_FullSIM->histogram->GetMaximum(),itVec_Delphes->histogram->GetMaximum())*1.3);

    /*
    itVec_FullSIM->histogram->Draw("hist");
    itVec_Delphes->histogram->Draw("hist same");
    
    itVec_GenDelphes->histogram->SetLineColor(kGreen+1);
    itVec_GenDelphes->histogram->SetLineWidth(2);
    itVec_GenFullSIM->histogram->SetLineColor(kYellow+1);
    itVec_GenFullSIM->histogram->SetLineWidth(2);

    legend->AddEntry(itVec_Delphes->histogram,"Delphes Gen","l");
    legend->AddEntry(itVec_Delphes->histogram,"Full SIM Gen","l");


    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    //    cCanvas->SetLogy(1);

    //    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.1,max(itVec_FullSIM->histogram->GetMaximum(),itVec_Delphes->histogram->GetMaximum())*100);

    //    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
    //    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
    //    cCanvas->SetLogy(0);
    */    

    itVec_FullSIM->histogram->Scale(1./itVec_FullSIM->histogram->Integral());
    itVec_Delphes->histogram->Scale(1./itVec_Delphes->histogram->Integral());
    itVec_GenFullSIM->histogram->Scale(1./itVec_GenFullSIM->histogram->Integral());
    itVec_GenDelphes->histogram->Scale(1./itVec_GenDelphes->histogram->Integral());

    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_FullSIM->histogram->GetMaximum(),itVec_Delphes->histogram->GetMaximum())*1.3);
    itVec_FullSIM->histogram->GetYaxis()->SetTitle("a.u.");

    itVec_FullSIM->histogram->Draw("hist");
    itVec_Delphes->histogram->Draw("hist same");
    itVec_GenFullSIM->histogram->Draw("hist same");
    itVec_GenDelphes->histogram->Draw("hist same");
    

    legend->AddEntry(itVec_GenFullSIM->histogram,"Gen CMSSW","l");
    legend->AddEntry(itVec_GenDelphes->histogram,"Gen Delphes","l");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm.png").c_str(),"png");

    legend->Clear();

  }  



  // make the plot on the same canvas for each variable (legend entry is the cut layer name)                                                                                   
  for(size_t iVar = 0; iVar < variableRespList.size(); iVar++){ // loop on var                                                                                                
    histoContainer tmpPlot_FullSIM;
    tmpPlot_FullSIM.cutName = "FullSIMResponse";
    tmpPlot_FullSIM.varName = variableRespList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_FullSIM ;
    itVec_FullSIM = find(plotResponse_FullSIM.begin(),plotResponse_FullSIM.end(),tmpPlot_FullSIM);
    if(itVec_FullSIM == plotResponse_FullSIM.end()){
      cerr<<"Problem -->plot not found for Full SIM  "<<variableRespList.at(iVar).variableName<<endl;
    }

    itVec_FullSIM->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_FullSIM->histogram->GetXaxis()->SetTitleOffset(1.25);
    itVec_FullSIM->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,itVec_FullSIM->histogram->GetMaximum()*1.3);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_FullSIM->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_FullSIM->histogram->SetLineColor(kBlue);

    itVec_FullSIM->histogram->SetLineWidth(2);
    itVec_FullSIM->histogram->GetYaxis()->SetTitleOffset(1.1);

    itVec_FullSIM->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    legend->AddEntry(itVec_FullSIM->histogram,"CMSSW","l");

    histoContainer tmpPlot_Delphes;
    tmpPlot_Delphes.cutName = "DelphesResponse";
    tmpPlot_Delphes.varName = variableRespList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_Delphes ;
    itVec_Delphes = find(plotResponse_Delphes.begin(),plotResponse_Delphes.end(),tmpPlot_Delphes);
    if(itVec_Delphes == plotResponse_Delphes.end()){
      cerr<<"Problem -->plot not found for Delphes  "<<variableRespList.at(iVar).variableName<<endl;
    }


    itVec_Delphes->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_Delphes->histogram->GetXaxis()->SetTitleOffset(1.16);
    itVec_Delphes->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_Delphes->histogram->GetYaxis()->SetRangeUser(0.001,itVec_Delphes->histogram->GetMaximum()*1.1);
    itVec_Delphes->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_Delphes->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_Delphes->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_Delphes->histogram->SetLineColor(kRed);

    itVec_Delphes->histogram->SetLineWidth(2);
    itVec_Delphes->histogram->GetYaxis()->SetTitle("#sigma x lumi");
    legend->AddEntry(itVec_Delphes->histogram,"Delphes"," l");

    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_FullSIM->histogram->GetMaximum(),itVec_Delphes->histogram->GetMaximum())*1.3);
    /*
    itVec_FullSIM->histogram->Draw("hist");
    itVec_Delphes->histogram->Draw("hist same");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableRespList.at(iVar).variableName+"_resp.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableRespList.at(iVar).variableName+"_resp.png").c_str(),"png");
    */

    itVec_FullSIM->histogram->Scale(1./itVec_FullSIM->histogram->Integral());
    itVec_Delphes->histogram->Scale(1./itVec_Delphes->histogram->Integral());

    itVec_FullSIM->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_FullSIM->histogram->GetMaximum(),itVec_Delphes->histogram->GetMaximum())*1.3);

    itVec_FullSIM->histogram->GetYaxis()->SetTitle("a.u.");

    itVec_FullSIM->histogram->Draw("hist");
    itVec_Delphes->histogram->Draw("hist same");
    

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableRespList.at(iVar).variableName+"_resp_norm.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableRespList.at(iVar).variableName+"_resp_norm.png").c_str(),"png");

    legend->Clear();

  }  

 // plot VBF efficiency and efficiency
  TGraphAsymmErrors* Eff_Lead_FullSIM_vsEta = new TGraphAsymmErrors();
  Eff_Lead_FullSIM_vsEta->SetName("Eff_Lead_vsEta_FullSIM");
  Eff_Lead_FullSIM_vsEta->BayesDivide(Numerator_LeadJet_Eff_FullSIM_vsEta,Denominator_LeadJet_Eff_FullSIM_vsEta);

  TGraphAsymmErrors* Eff_Trail_FullSIM_vsEta = new TGraphAsymmErrors();
  Eff_Trail_FullSIM_vsEta->SetName("Eff_Trail_vsEta_FullSIM");
  Eff_Trail_FullSIM_vsEta->BayesDivide(Numerator_TrailJet_Eff_FullSIM_vsEta,Denominator_TrailJet_Eff_FullSIM_vsEta);

  TGraphAsymmErrors* Eff_FullSIM_vsDEta = new TGraphAsymmErrors();
  Eff_FullSIM_vsDEta->SetName("Eff_vsDEta_FullSIM");
  Eff_FullSIM_vsDEta->BayesDivide(Numerator_Eff_FullSIM_vsDEta,Denominator_Eff_FullSIM_vsDEta);

  TGraphAsymmErrors* Eff_FullSIM_vsMjj = new TGraphAsymmErrors();
  Eff_FullSIM_vsMjj->SetName("Eff_vsMjj_FullSIM");
  Eff_FullSIM_vsMjj->BayesDivide(Numerator_Eff_FullSIM_vsMjj ,Denominator_Eff_FullSIM_vsMjj);

  TGraphAsymmErrors* Eff_Lead_Delphes_vsEta = new TGraphAsymmErrors();
  Eff_Lead_Delphes_vsEta->SetName("Eff_Lead_vsEta_Delphes");
  Eff_Lead_Delphes_vsEta->BayesDivide(Numerator_LeadJet_Eff_Delphes_vsEta ,Denominator_LeadJet_Eff_Delphes_vsEta);

  TGraphAsymmErrors* Eff_Trail_Delphes_vsEta = new TGraphAsymmErrors();
  Eff_Trail_Delphes_vsEta->SetName("Eff_Trail_vsEta_Delphes");
  Eff_Trail_Delphes_vsEta->BayesDivide(Numerator_TrailJet_Eff_Delphes_vsEta ,Denominator_TrailJet_Eff_Delphes_vsEta);

  TGraphAsymmErrors* Eff_Delphes_vsDEta = new TGraphAsymmErrors();
  Eff_Delphes_vsDEta->SetName("Eff_vsDEta_Delphes");
  Eff_Delphes_vsDEta->BayesDivide(Numerator_Eff_Delphes_vsDEta ,Denominator_Eff_Delphes_vsDEta);

  TGraphAsymmErrors* Eff_Delphes_vsMjj = new TGraphAsymmErrors();
  Eff_Delphes_vsMjj->SetName("Eff_vsMjj_Delphes");
  Eff_Delphes_vsMjj->BayesDivide(Numerator_Eff_Delphes_vsMjj ,Denominator_Eff_Delphes_vsMjj);

  // Draw
  Eff_Lead_FullSIM_vsEta->SetMarkerColor(kBlue);
  Eff_Lead_FullSIM_vsEta->SetLineColor(kBlue);
  Eff_Lead_FullSIM_vsEta->SetMarkerStyle(20);
  Eff_Lead_FullSIM_vsEta->SetMarkerSize(1.1);
  Eff_Lead_FullSIM_vsEta->SetLineWidth(2);
  Eff_Lead_FullSIM_vsEta->GetXaxis()->SetTitle("|#eta_{j1}|");
  Eff_Lead_FullSIM_vsEta->GetYaxis()->SetTitle("efficiency");
  Eff_Lead_FullSIM_vsEta->GetYaxis()->SetRangeUser(0.5,1.2);
  Eff_Lead_FullSIM_vsEta->Draw("ap");

  Eff_Lead_Delphes_vsEta->SetMarkerColor(kRed);
  Eff_Lead_Delphes_vsEta->SetLineColor(kRed);
  Eff_Lead_Delphes_vsEta->SetMarkerStyle(20);
  Eff_Lead_Delphes_vsEta->SetMarkerSize(1.1);
  Eff_Lead_Delphes_vsEta->SetLineWidth(2);
  Eff_Lead_Delphes_vsEta->Draw("psame");

  TLegend* leg = new TLegend(0.25,0.3,0.45,0.55);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);

  leg->AddEntry(Eff_Lead_FullSIM_vsEta,"CMSSW full sim","p");
  leg->AddEntry(Eff_Lead_Delphes_vsEta,"Delphes sim","p");
  leg->Draw("same");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
    
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_LeadvsEta.pdf").c_str(),"pdf");
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_LeadvsEta.png").c_str(),"png");


  Eff_Trail_FullSIM_vsEta->SetMarkerColor(kBlue);
  Eff_Trail_FullSIM_vsEta->SetLineColor(kBlue);
  Eff_Trail_FullSIM_vsEta->SetMarkerStyle(20);
  Eff_Trail_FullSIM_vsEta->SetMarkerSize(1.1);
  Eff_Trail_FullSIM_vsEta->SetLineWidth(2);
  Eff_Trail_FullSIM_vsEta->GetXaxis()->SetTitle("|#eta_{j2}|");
  Eff_Trail_FullSIM_vsEta->GetYaxis()->SetTitle("efficiency");
  Eff_Trail_FullSIM_vsEta->GetYaxis()->SetRangeUser(0.5,1.2);
  Eff_Trail_FullSIM_vsEta->Draw("ap");

  Eff_Trail_Delphes_vsEta->SetMarkerColor(kRed);
  Eff_Trail_Delphes_vsEta->SetLineColor(kRed);
  Eff_Trail_Delphes_vsEta->SetMarkerStyle(20);
  Eff_Trail_Delphes_vsEta->SetMarkerSize(1.1);
  Eff_Trail_Delphes_vsEta->SetLineWidth(2);
  Eff_Trail_Delphes_vsEta->Draw("psame");

  leg->Clear();
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(Eff_Trail_FullSIM_vsEta,"CMSSW full sim","p");
  leg->AddEntry(Eff_Trail_Delphes_vsEta,"Delphes sim","p");
  leg->Draw("same");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
    
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_TrailvsEta.pdf").c_str(),"pdf");
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_TrailvsEta.png").c_str(),"png");


  Eff_FullSIM_vsDEta->SetMarkerColor(kBlue);
  Eff_FullSIM_vsDEta->SetLineColor(kBlue);
  Eff_FullSIM_vsDEta->SetMarkerStyle(20);
  Eff_FullSIM_vsDEta->SetMarkerSize(1.1);
  Eff_FullSIM_vsDEta->SetLineWidth(2);
  Eff_FullSIM_vsDEta->GetXaxis()->SetTitle("|#Delta#eta_{jj}|");
  Eff_FullSIM_vsDEta->GetYaxis()->SetTitle("efficiency");
  Eff_FullSIM_vsDEta->GetYaxis()->SetRangeUser(0.5,1.2);
  Eff_FullSIM_vsDEta->Draw("ap");

  Eff_Delphes_vsDEta->SetMarkerColor(kRed);
  Eff_Delphes_vsDEta->SetLineColor(kRed);
  Eff_Delphes_vsDEta->SetMarkerStyle(20);
  Eff_Delphes_vsDEta->SetMarkerSize(1.1);
  Eff_Delphes_vsDEta->SetLineWidth(2);
  Eff_Delphes_vsDEta->Draw("psame");

  leg->Clear();
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(Eff_FullSIM_vsDEta,"CMSSW full sim","p");
  leg->AddEntry(Eff_Delphes_vsDEta,"Delphes sim","p");
  leg->Draw("same");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
    
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_vsDEta.pdf").c_str(),"pdf");
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_vsDEta.png").c_str(),"png");


  Eff_FullSIM_vsMjj->SetMarkerColor(kBlue);
  Eff_FullSIM_vsMjj->SetLineColor(kBlue);
  Eff_FullSIM_vsMjj->SetMarkerStyle(20);
  Eff_FullSIM_vsMjj->SetMarkerSize(1.1);
  Eff_FullSIM_vsMjj->SetLineWidth(2);
  Eff_FullSIM_vsMjj->GetXaxis()->SetTitle("M_{jj}");
  Eff_FullSIM_vsMjj->GetYaxis()->SetTitle("efficiency");
  Eff_FullSIM_vsMjj->GetYaxis()->SetRangeUser(0.,1.2);
  Eff_FullSIM_vsMjj->Draw("ap");

  Eff_Delphes_vsMjj->SetMarkerColor(kRed);
  Eff_Delphes_vsMjj->SetLineColor(kRed);
  Eff_Delphes_vsMjj->SetMarkerStyle(20);
  Eff_Delphes_vsMjj->SetMarkerSize(1.1);
  Eff_Delphes_vsMjj->SetLineWidth(2);
  Eff_Delphes_vsMjj->Draw("psame");

  leg->Clear();
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(Eff_FullSIM_vsMjj,"CMSSW full sim","p");
  leg->AddEntry(Eff_Delphes_vsMjj,"Delphes sim","p");
  leg->Draw("same");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
    
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_vsMjj.pdf").c_str(),"pdf");
  cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/Eff_vsMjj.png").c_str(),"png");


  
 return 0 ;

}

