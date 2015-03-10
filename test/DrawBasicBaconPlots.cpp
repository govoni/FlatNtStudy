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


// Bacon and other includes
#include "ReadInputFile.h"
#include "TBaconObjects.h"
#include "ConfigParser.h"
#include "utilsBacon.h"

using namespace std;
using namespace baconhep;
 
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

  TChain* chain_PhaseIAged = new TChain (treeName.c_str()) ;
  chain_PhaseIAged->Add ((InputBaseDirectory_PhaseIAged+"/*.root").c_str()) ;
  int totEvent_PhaseIAged = chain_PhaseIAged->GetEntries();

  TEventInfo   *fEventInfo_PhaseIAged = new TEventInfo(); 
  TClonesArray *fElectron_PhaseIAged = new TClonesArray("baconhep::TElectron");
  TClonesArray *fMuon_PhaseIAged     = new TClonesArray("baconhep::TMuon");
  TClonesArray *fJet_PhaseIAged      = new TClonesArray("baconhep::TJet");
  TClonesArray *fGenParticle_PhaseIAged = new TClonesArray("baconhep::TGenParticle");

  chain_PhaseIAged->SetBranchAddress("Info",&fEventInfo_PhaseIAged);
  chain_PhaseIAged->SetBranchAddress("Electron", &fElectron_PhaseIAged);
  chain_PhaseIAged->SetBranchAddress("Muon", &fMuon_PhaseIAged);
  chain_PhaseIAged->SetBranchAddress("Jet04", &fJet_PhaseIAged);
  chain_PhaseIAged->SetBranchAddress("GenParticle", &fGenParticle_PhaseIAged);

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_PhaseIAged<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_PhaseIAged)<<endl;

  float weight_PhaseIAged = 1.0*lumi*CrossSection/float(totEvent_PhaseIAged) ;

  // make chain and branches for Shashlik trees
    
  TChain* chain_PhaseIIShashlik = new TChain (treeName.c_str()) ;
  chain_PhaseIIShashlik->Add ((InputBaseDirectory_PhaseIIShashlik+"/*.root").c_str()) ;
  int totEvent_PhaseIIShashlik = chain_PhaseIIShashlik->GetEntries();

  TClonesArray *fElectron_PhaseIIShashlik  = new TClonesArray("baconhep::TElectron");
  TClonesArray *fMuon_PhaseIIShashlik      = new TClonesArray("baconhep::TMuon");
  TClonesArray *fJet_PhaseIIShashlik       = new TClonesArray("baconhep::TJet");
  TClonesArray *fGenParticle_PhaseIIShashlik  = new TClonesArray("baconhep::TGenParticle");
  TEventInfo   *fEventInfo_PhaseIIShashlik = new TEventInfo(); 

  chain_PhaseIIShashlik ->SetBranchAddress("Electron", &fElectron_PhaseIIShashlik);
  chain_PhaseIIShashlik ->SetBranchAddress("Muon", &fMuon_PhaseIIShashlik);
  chain_PhaseIIShashlik ->SetBranchAddress("Jet04", &fJet_PhaseIIShashlik);
  chain_PhaseIIShashlik ->SetBranchAddress("GenParticle", &fGenParticle_PhaseIIShashlik);
  chain_PhaseIIShashlik->SetBranchAddress("Info",&fEventInfo_PhaseIIShashlik);

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent_PhaseIIShashlik<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent_PhaseIIShashlik)<<endl;

  float weight_PhaseIIShashlik = 1.0*lumi*CrossSection/float(totEvent_PhaseIIShashlik) ;

  TH1::SetDefaultSumw2();

  // Efficiency histograms
  map<string,TH1F*> histoCutEff_PhaseIAged ;
  map<string,TH1F*> histoCutEff_PhaseIIShashlik ;
  map<string,TH1F*> histoCutEff_Gen ;

  vector<histoContainer> plotVector_PhaseIAged;
  vector<histoContainer> plotVector_PhaseIIShashlik;
  vector<histoContainer> plotVector_Gen;

  vector<histoContainer> plotResponse_PhaseIAged;
  vector<histoContainer> plotResponse_PhaseIIShashlik;

  histoCutEff_PhaseIAged["PhaseIAged"]           = new TH1F("PhaseIAged","",11,0,11);
  histoCutEff_PhaseIIShashlik["PhaseIIShashlik"] = new TH1F("PhaseIIShashlik","",11,0,11);
  histoCutEff_Gen["Gen"]                         = new TH1F("Gen","",11,0,11);

  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    plotVector_PhaseIAged.push_back(histoContainer("PhaseIAged",variableList.at(iVar)));
    plotVector_Gen.push_back(histoContainer("Gen",variableList.at(iVar)));
    plotVector_PhaseIIShashlik.push_back(histoContainer("PhaseIIShashlik",variableList.at(iVar)));
  }

  for(size_t iVar = 0; iVar < variableRespList.size(); iVar++){
    plotResponse_PhaseIAged.push_back(histoContainer("PhaseIAgedResponse",variableRespList.at(iVar)));
    plotResponse_PhaseIIShashlik.push_back(histoContainer("PhaseIIShashlikResponse",variableRespList.at(iVar)));
  }

  int maximumEvents_PhaseIAged = chain_PhaseIAged->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_PhaseIAged)
    maximumEvents_PhaseIAged = maxEventNumber ;

  int maximumEvents_PhaseIIShashlik = chain_PhaseIIShashlik->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents_PhaseIIShashlik)
    maximumEvents_PhaseIIShashlik = maxEventNumber ;


  // Loop on aged events
  for(int iEvent = 0; iEvent < maximumEvents_PhaseIAged ; iEvent++){
    
    if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ;
    string name = "PhaseIAged";
    
    chain_PhaseIAged->GetEntry(iEvent);

    // TElectron and TMuon are all the reco lepton to be used in the analysis --> apply some basic ID cuts
    vector<TMuon> goodTightMuons ;
    vector<TElectron> goodTightElectrons ;

    int iBin = 1;
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }

    if((fMuon_PhaseIAged->GetEntriesFast()+fElectron_PhaseIAged->GetEntriesFast()) < nLeptons) continue;
    if((fJet_PhaseIAged->GetEntriesFast()) < 2) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }


    // look for tight muons and electrons in the event

    for(int iEle = 0; iEle < fElectron_PhaseIAged->GetEntriesFast(); iEle++){
      TElectron *ele = (TElectron*)((*fElectron_PhaseIAged)[iEle]);
      if(ele->pt < minLeptonCutPt) continue;
      if(fabs(ele->eta) > 2.5) continue;
      if(passEleID(ele,fEventInfo_PhaseIAged->rhoIso))
	 goodTightElectrons.push_back(*ele);
    }

    for(int iMu = 0; iMu < fMuon_PhaseIAged->GetEntriesFast(); iMu++){
      TMuon *mu = (TMuon*)((*fMuon_PhaseIAged)[iMu]);
      if(mu->pt < minLeptonCutPt) continue;
      if(fabs(mu->eta) > 2.5) continue;
      if(passMuonID(mu,fEventInfo_PhaseIAged->rhoIso))
	 goodTightMuons.push_back(*mu);
    }

    // sort in pt

    sort(goodTightMuons.rbegin(),goodTightMuons.rend());
    sort(goodTightElectrons.rbegin(),goodTightElectrons.rend());

    // apply a cut on the number of tight leptons and final state topology
    if(int(goodTightElectrons.size()+goodTightMuons.size()) < nLeptons) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }

    // select final state topology    
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.size() !=2 or goodTightElectrons.size() != 0) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.size() !=2 or goodTightMuons.size() != 0) continue;
    }
    else continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }

    // charge : same sign
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.at(0).q != goodTightMuons.at(1).q) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.at(0).q != goodTightElectrons.at(1).q) continue;
    }
    else continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }

    // met cut
    if(fEventInfo_PhaseIAged->pfMETC < metCut) continue;

    TLorentzVector met ; met.SetPtEtaPhiM(fEventInfo_PhaseIAged->pfMETC,0.,fEventInfo_PhaseIAged->pfMETCphi,0.);

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"met cut");
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

    if((lepton1+lepton2).M() < mllCut) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"mll cut");
      iBin++;
    }


    // matching with generator level leptons : find gen muons and gene ele from W decay
    vector<int> genEleIndex ;
    vector<int> genMuIndex ;
    findGenLeptonsFromW(fGenParticle_PhaseIAged,genEleIndex,genMuIndex);

    vector<TGenParticle> genMatchedMuons;
    vector<TGenParticle> genMatchedElectrons;
    vector<TGenParticle> genNeutrino;
    
    findGenNeutrinoFromW(fGenParticle_PhaseIAged,genNeutrino);

    for(size_t iMu = 0; iMu < goodTightMuons.size(); iMu++){
      TMuon mu = goodTightMuons.at(iMu);
      TLorentzVector mu4V;
      mu4V.SetPtEtaPhiM(mu.pt,mu.eta,mu.phi,0.);
      // Match with genParticle	
      for( vector<int>::iterator iGen = genMuIndex.begin(); iGen != genMuIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_PhaseIAged)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(mu4V.DeltaR(gen4V) < matchingCone){
	  genMatchedMuons.push_back(*genP);
	  break;
	}
      }
    }
   
    for(size_t iEle = 0; iEle < goodTightElectrons.size(); iEle++){
      TElectron ele = goodTightElectrons.at(iEle);
      TLorentzVector ele4V;
      ele4V.SetPtEtaPhiM(ele.pt,ele.eta,ele.phi,0.);
      // Match with genParticle	
      for( vector<int>::iterator iGen = genEleIndex.begin(); iGen != genEleIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_PhaseIAged)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(ele4V.DeltaR(gen4V) < matchingCone){
	  genMatchedElectrons.push_back(*genP);
	  break;
	}
      }
    }
   
    if (int(genMatchedElectrons.size() + genMatchedMuons.size()) != nLeptons) continue;

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen lepton");
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


    TLorentzVector genMet ;
    for(size_t iNu = 0; iNu < genNeutrino.size(); iNu++){
      TLorentzVector tmp4V;
      tmp4V.SetPtEtaPhiM(genNeutrino.at(iNu).pt,0.,genNeutrino.at(iNu).phi,0.);
      genMet += tmp4V ;
    }

    // look for jets cleaning leptons
    vector<TJet> cleanedJets;    
    cleanedJetsFromLeptons(cleanedJets,*fJet_PhaseIAged,goodTightMuons,goodTightElectrons,minJetCutPt,matchingCone);
    sort(cleanedJets.rbegin(),cleanedJets.rend());
    if(cleanedJets.size() < 2) continue;
    
    TLorentzVector jet1, jet2, dijet ;
    jet1.SetPtEtaPhiM(cleanedJets.at(0).pt,cleanedJets.at(0).eta,cleanedJets.at(0).phi,cleanedJets.at(0).mass);
    jet2.SetPtEtaPhiM(cleanedJets.at(1).pt,cleanedJets.at(1).eta,cleanedJets.at(1).phi,cleanedJets.at(1).mass);

    dijet = jet1+jet2;

    // matching with gen jets by the flag

    if(cleanedJets.at(0).genpt <= 0 and cleanedJets.at(1).genpt <= 0) continue;

    TLorentzVector genjet1, genjet2;
    genjet1.SetPtEtaPhiM(cleanedJets.at(0).genpt,cleanedJets.at(0).geneta,cleanedJets.at(0).genphi,cleanedJets.at(0).genm);
    genjet2.SetPtEtaPhiM(cleanedJets.at(1).genpt,cleanedJets.at(1).geneta,cleanedJets.at(1).genphi,cleanedJets.at(1).genm);

    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen jets after cleaning");
      iBin++;
    }

    if(dijet.M() < mjjCut) continue;
    
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"mjj cut");
      iBin++;
    }

    if(fabs(jet1.Eta()-jet2.Eta()) < detajjCut) continue;
    
    if(histoCutEff_PhaseIAged.size()!=0){
      histoCutEff_PhaseIAged[name]->SetBinContent(iBin,histoCutEff_PhaseIAged[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIAged[name]->GetXaxis()->SetBinLabel(iBin,"detajj cut");
      iBin++;
    }
    
    // fill histos
    fillHistos(plotVector_PhaseIAged,variableList,name,weight_PhaseIAged,lepton1,lepton2,jet1,jet2,met);
    // fill response
    fillResponse(plotResponse_PhaseIAged,variableRespList,name+"Response",weight_PhaseIAged,lepton1,genLep1,lepton2,genLep2,jet1,genjet1,jet2,genjet2,met,genMet);

  }

  // Loop on shashlik events
  for(int iEvent = 0; iEvent < maximumEvents_PhaseIIShashlik ; iEvent++){
    
    if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ;
    string name = "PhaseIIShashlik";
    
    chain_PhaseIIShashlik->GetEntry(iEvent);

    // TElectron and TMuon are all the reco lepton to be used in the analysis --> apply some basic ID cuts
    vector<TMuon> goodTightMuons ;
    vector<TElectron> goodTightElectrons ;

    int iBin = 1;
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }

    if((fMuon_PhaseIIShashlik->GetEntriesFast()+fElectron_PhaseIIShashlik->GetEntriesFast()) < nLeptons) continue;
    if((fJet_PhaseIIShashlik->GetEntriesFast()) < 2) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }

    for(int iEle = 0; iEle < fElectron_PhaseIIShashlik->GetEntriesFast(); iEle++){
      TElectron *ele = (TElectron*)((*fElectron_PhaseIIShashlik)[iEle]);
      if(ele->pt < minLeptonCutPt) continue;
      if(fabs(ele->eta) > 2.5) continue;
      if(passEleID(ele,fEventInfo_PhaseIIShashlik->rhoIso))
	 goodTightElectrons.push_back(*ele);
    }

    for(int iMu = 0; iMu < fMuon_PhaseIIShashlik->GetEntriesFast(); iMu++){
      TMuon *mu = (TMuon*)((*fMuon_PhaseIIShashlik)[iMu]);
      if(mu->pt < minLeptonCutPt) continue;
      if(fabs(mu->eta) > 2.5) continue;
      if(passMuonID(mu,fEventInfo_PhaseIIShashlik->rhoIso))
	 goodTightMuons.push_back(*mu);
    }

    sort(goodTightMuons.rbegin(),goodTightMuons.rend());
    sort(goodTightElectrons.rbegin(),goodTightElectrons.rend());

    // apply a cut on the number of tight leptons and final state topology
    if(int(goodTightElectrons.size()+goodTightMuons.size()) < nLeptons) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }

    // select final state topology    
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.size() !=2 or goodTightElectrons.size() != 0) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.size() !=2 or goodTightMuons.size() != 0) continue;
    }
    else continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
      iBin++;
    }

    // charge : same sign
    if(TString(finalStateString).Contains("UU")){
      if(goodTightMuons.at(0).q != goodTightMuons.at(1).q) continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(goodTightElectrons.at(0).q != goodTightElectrons.at(1).q) continue;
    }
    else continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"same sign");
      iBin++;
    }

    // met cut
    if(fEventInfo_PhaseIIShashlik->pfMETC < metCut) continue;

    TLorentzVector met ; met.SetPtEtaPhiM(fEventInfo_PhaseIIShashlik->pfMETC,0.,fEventInfo_PhaseIIShashlik->pfMETCphi,0.);

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"met cut");
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

    if((lepton1+lepton2).M() < mllCut) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"mll cut");
      iBin++;
    }


    // matching with generator level leptons : find gen muons and gene ele from W decay
    vector<int> genEleIndex ;
    vector<int> genMuIndex ;
    findGenLeptonsFromW(fGenParticle_PhaseIIShashlik,genEleIndex,genMuIndex);

    vector<TGenParticle> genMatchedMuons;
    vector<TGenParticle> genMatchedElectrons;
    vector<TGenParticle> genNeutrino;
    
    findGenNeutrinoFromW(fGenParticle_PhaseIIShashlik,genNeutrino);


    for(size_t iMu = 0; iMu < goodTightMuons.size(); iMu++){
      TMuon mu = goodTightMuons.at(iMu);
      TLorentzVector mu4V;
      mu4V.SetPtEtaPhiM(mu.pt,mu.eta,mu.phi,0.);
      // Match with genParticle	
      for( vector<int>::iterator iGen = genMuIndex.begin(); iGen != genMuIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_PhaseIIShashlik)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(mu4V.DeltaR(gen4V) < matchingCone){
	  genMatchedMuons.push_back(*genP);
	  break;
	}
      }
    }
   
    for(size_t iEle = 0; iEle < goodTightElectrons.size(); iEle++){
      TElectron ele = goodTightElectrons.at(iEle);
      TLorentzVector ele4V;
      ele4V.SetPtEtaPhiM(ele.pt,ele.eta,ele.phi,0.);
      // Match with genParticle	
      for( vector<int>::iterator iGen = genEleIndex.begin(); iGen != genEleIndex.end(); ++iGen ) {
	TGenParticle *genP = (TGenParticle*)((*fGenParticle_PhaseIIShashlik)[*iGen]); 
	TLorentzVector gen4V;
	gen4V.SetPtEtaPhiM(genP->pt,genP->eta,genP->phi,0.);
	if(ele4V.DeltaR(gen4V) < matchingCone){
	  genMatchedElectrons.push_back(*genP);
	  break;
	}
      }
    }
   
    if (int(genMatchedMuons.size()+genMatchedElectrons.size()) != nLeptons) continue;

    TLorentzVector genLep1, genLep2;
    if(TString(finalStateString).Contains("UU")){
      genLep1.SetPtEtaPhiM(genMatchedMuons.at(0).pt,genMatchedMuons.at(0).eta,genMatchedMuons.at(0).phi,0.);
      genLep2.SetPtEtaPhiM(genMatchedMuons.at(1).pt,genMatchedMuons.at(1).eta,genMatchedMuons.at(1).phi,0.);
    }
    else if(TString(finalStateString).Contains("EE")){
      genLep1.SetPtEtaPhiM(genMatchedElectrons.at(0).pt,genMatchedElectrons.at(0).eta,genMatchedElectrons.at(0).phi,0.);
      genLep2.SetPtEtaPhiM(genMatchedElectrons.at(1).pt,genMatchedElectrons.at(1).eta,genMatchedElectrons.at(1).phi,0.);
    }

    

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen lepton");
      iBin++;
    }

    // look for jets cleaning leptons
    vector<TJet> cleanedJets;    
    cleanedJetsFromLeptons(cleanedJets,*fJet_PhaseIIShashlik,goodTightMuons,goodTightElectrons,minJetCutPt,matchingCone);
    sort(cleanedJets.rbegin(),cleanedJets.rend());
    if(cleanedJets.size() < 2) continue;
    
    TLorentzVector jet1, jet2, dijet ;
    jet1.SetPtEtaPhiM(cleanedJets.at(0).pt,cleanedJets.at(0).eta,cleanedJets.at(0).phi,cleanedJets.at(0).mass);
    jet2.SetPtEtaPhiM(cleanedJets.at(1).pt,cleanedJets.at(1).eta,cleanedJets.at(1).phi,cleanedJets.at(1).mass);


    dijet = jet1+jet2;

    // matching with gen jets by the flag

    if(cleanedJets.at(0).genpt <= 0 and cleanedJets.at(1).genpt <= 0) continue;

    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"mathcing gen jets after cleaning");
      iBin++;
    }

    if(dijet.M() < mjjCut) continue;
    
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"mjj cut");
      iBin++;
    }

    if(fabs(jet1.Eta()-jet2.Eta()) < detajjCut) continue;
    
    if(histoCutEff_PhaseIIShashlik.size()!=0){
      histoCutEff_PhaseIIShashlik[name]->SetBinContent(iBin,histoCutEff_PhaseIIShashlik[name]->GetBinContent(iBin)+1);
      histoCutEff_PhaseIIShashlik[name]->GetXaxis()->SetBinLabel(iBin,"detajj cut");
      iBin++;
    }
    
    // fill histos
    fillHistos(plotVector_PhaseIIShashlik,variableList,name,weight_PhaseIIShashlik,lepton1,lepton2,jet1,jet2,met);

    TLorentzVector genMet ;
    for(size_t iNu = 0; iNu < genNeutrino.size(); iNu++){
      TLorentzVector tmp4V;
      tmp4V.SetPtEtaPhiM(genNeutrino.at(iNu).pt,0.,genNeutrino.at(iNu).phi,0.);
      genMet += tmp4V ;
    }

    TLorentzVector genjet1, genjet2;
    genjet1.SetPtEtaPhiM(cleanedJets.at(0).genpt,cleanedJets.at(0).geneta,cleanedJets.at(0).genphi,cleanedJets.at(0).genm);
    genjet2.SetPtEtaPhiM(cleanedJets.at(1).genpt,cleanedJets.at(1).geneta,cleanedJets.at(1).genphi,cleanedJets.at(1).genm);


    fillHistos(plotVector_Gen,variableList,"Gen",1,genLep1,genLep2,genjet1,genjet2,genMet);

    fillResponse(plotResponse_PhaseIIShashlik,variableRespList,name+"Response",weight_PhaseIIShashlik,lepton1,genLep1,lepton2,genLep2,jet1,genjet1,jet2,genjet2,met,genMet);


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

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_PhaseIAged.begin(); itMap !=  histoCutEff_PhaseIAged.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_PhaseIAged));
    itMap->second->Draw("hist");    
    itMap->second->SetLineColor(kBlue);
    itMap->second->SetLineWidth(2);
    itMap->second->GetYaxis()->SetTitle("Efficiency");
    itMap->second->GetYaxis()->SetTitleOffset(1.15);
    legend->AddEntry(itMap->second,"Aged efficiency","l");
    itMap->second->Write();
  }

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff_PhaseIIShashlik.begin(); itMap !=  histoCutEff_PhaseIIShashlik.end(); itMap++){
    itMap->second->Scale(1./(maximumEvents_PhaseIIShashlik));
    legend->AddEntry(itMap->second,"Shashlik efficiency","l");
    itMap->second->Draw("hist same");
    itMap->second->SetLineColor(kRed);
    itMap->second->SetLineWidth(2);
    itMap->second->SetLineStyle(7);
    itMap->second->Write();
  }

  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
  legend->Draw("same");

  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency.png").c_str(),"png");  
  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency.pdf").c_str(),"pdf");  

  cCanvas->SetLogy(1);

  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency_log.png").c_str(),"png");  
  cCanvas->SaveAs(("output/"+outputPlotDirectory+"/efficiency_log.pdf").c_str(),"pdf");  

  cCanvas->SetLogy(0);

  legend->Clear();

  outputEfficiency->Close();


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
      cerr<<"Problem -->plot not found for PhaseII_Shashlik  "<<variableList.at(iVar).variableName<<endl;
    }

    histoContainer tmpPlot_Gen;
    tmpPlot_Gen.cutName = "Gen";
    tmpPlot_Gen.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_Gen ;
    itVec_Gen = find(plotVector_Gen.begin(),plotVector_Gen.end(),tmpPlot_Gen);
    if(itVec_Gen == plotVector_Gen.end()){
      cerr<<"Problem -->plot not found for Gen  "<<variableList.at(iVar).variableName<<endl;
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

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.3);

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");
    legend->AddEntry(itVec_PhaseIIShashlik->histogram,"Phase II Shashlik","l");

    itVec_Gen->histogram->SetLineColor(kGreen+1);
    itVec_Gen->histogram->SetLineWidth(2);

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.1,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*100);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    itVec_PhaseIAged->histogram->Scale(1./itVec_PhaseIAged->histogram->Integral());
    itVec_PhaseIIShashlik->histogram->Scale(1./itVec_PhaseIIShashlik->histogram->Integral());
    itVec_Gen->histogram->Scale(1./itVec_Gen->histogram->Integral());

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.3);

    itVec_PhaseIAged->histogram->GetYaxis()->SetTitle("a.u.");

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");
    itVec_Gen->histogram->Draw("hist same");
    

    legend->AddEntry(itVec_Gen->histogram,"gen level","l");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm.png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.01,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*100);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_norm_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    legend->Clear();

  }  




  // make the plot on the same canvas for each variable (legend entry is the cut layer name)                                                                                   
  for(size_t iVar = 0; iVar < variableRespList.size(); iVar++){ // loop on var                                                                                                
    histoContainer tmpPlot_PhaseIAged;
    tmpPlot_PhaseIAged.cutName = "PhaseIAgedResponse";
    tmpPlot_PhaseIAged.varName = variableRespList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_PhaseIAged ;
    itVec_PhaseIAged = find(plotResponse_PhaseIAged.begin(),plotResponse_PhaseIAged.end(),tmpPlot_PhaseIAged);
    if(itVec_PhaseIAged == plotResponse_PhaseIAged.end()){
      cerr<<"Problem -->plot not found for PhaseI_Aged  "<<variableRespList.at(iVar).variableName<<endl;
    }

    itVec_PhaseIAged->histogram->GetXaxis()->SetTitleSize(0.04);
    itVec_PhaseIAged->histogram->GetXaxis()->SetTitleOffset(1.25);
    itVec_PhaseIAged->histogram->GetXaxis()->SetLabelSize(0.04);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,itVec_PhaseIAged->histogram->GetMaximum()*1.3);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleSize(0.05);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleOffset(1.20);
    itVec_PhaseIAged->histogram->GetYaxis()->SetLabelSize(0.04);

    itVec_PhaseIAged->histogram->SetLineColor(kBlue);

    itVec_PhaseIAged->histogram->SetLineWidth(2);
    itVec_PhaseIAged->histogram->GetYaxis()->SetTitleOffset(1.1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetTitle("#sigma x lumi");

    legend->AddEntry(itVec_PhaseIAged->histogram,"Phase I Aged","l");


    histoContainer tmpPlot_PhaseIIShashlik;
    tmpPlot_PhaseIIShashlik.cutName = "PhaseIIShashlikResponse";
    tmpPlot_PhaseIIShashlik.varName = variableRespList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec_PhaseIIShashlik ;
    itVec_PhaseIIShashlik = find(plotResponse_PhaseIIShashlik.begin(),plotResponse_PhaseIIShashlik.end(),tmpPlot_PhaseIIShashlik);
    if(itVec_PhaseIIShashlik == plotResponse_PhaseIIShashlik.end()){
      cerr<<"Problem -->plot not found for PhaseII_Shashlik  "<<variableRespList.at(iVar).variableName<<endl;
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

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.3);

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");
    legend->AddEntry(itVec_PhaseIIShashlik->histogram,"Phase II Shashlik"," l");

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_resp.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_resp.png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.1,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*100);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_resp_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+"_resp_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    itVec_PhaseIAged->histogram->Scale(1./itVec_PhaseIAged->histogram->Integral());
    itVec_PhaseIIShashlik->histogram->Scale(1./itVec_PhaseIIShashlik->histogram->Integral());

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.001,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*1.3);

    itVec_PhaseIAged->histogram->GetYaxis()->SetTitle("a.u.");

    itVec_PhaseIAged->histogram->Draw("hist");
    itVec_PhaseIIShashlik->histogram->Draw("hist same");
    

    legend->Draw("same");
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_resp_norm.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_resp_norm.png").c_str(),"png");
    cCanvas->SetLogy(1);

    itVec_PhaseIAged->histogram->GetYaxis()->SetRangeUser(0.01,max(itVec_PhaseIAged->histogram->GetMaximum(),itVec_PhaseIIShashlik->histogram->GetMaximum())*100);

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_resp_norm_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+"_resp_norm_log.png").c_str(),"png");
    cCanvas->SetLogy(0);

    legend->Clear();

  }  
  
 return 0 ;

}

