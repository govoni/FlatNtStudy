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

#include "kinematicUtils.h"
#include "plotter.h"
#include "ConfigParser.h"
#include "readTree.h"
#include "utils.h"

using namespace std ;

float matchingCone ;
float minLeptonCutPt;
float minJetCutPt;
string  finalStateString;

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
  string InputBaseDirectory  = gConfigParser -> readStringOption("Input::InputBaseDirectory");

  // import from cfg file the cross section value for this sample
  float CrossSection   = gConfigParser -> readFloatOption("Input::CrossSection");

  // total number of events
  int maxEventNumber   = gConfigParser -> readFloatOption("Input::EventsNumber");

  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

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

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");

  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone"); 
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());
  system(("mkdir -p output/"+outputPlotDirectory+"/xs").c_str());
  system(("mkdir -p output/"+outputPlotDirectory+"/norm").c_str());


  ///// Start the analysis  
  map<string,TH1F*> histoCutEff ;

  TChain* chain = new TChain (treeName.c_str()) ;  
  chain->Add ((InputBaseDirectory+"/*.root").c_str()) ;
  int totEvent = chain->GetEntries();

  readTree* reader  = new readTree((TTree*)(chain));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent)<<endl;

  float weight = 1.0*lumi*CrossSection/float(totEvent) ;

  // make the plot container 
  vector<histoContainer> plotVector;

  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEff["WW_EWK_pos_"+to_string(iCut)+"_"+CutList.at(iCut).cutLayerName] = new TH1F(("WW_EWK_pos_"+to_string(iCut)+"_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVector.push_back(histoContainer(CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  int passingLHEFilter = 0 ;
  
  int maximumEvents = chain->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents) 
    maximumEvents = maxEventNumber ;
  
  // Loop on the events
  for(int iEvent = 0; iEvent < maximumEvents ; iEvent++){

    reader->fChain->GetEntry(iEvent) ;

    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;

    // filter LHE level leptons
    if(TString(finalStateString).Contains("UU")){
      if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
        continue;
    }
    else if(TString(finalStateString).Contains("EE")){
      if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) continue;
    }
    else if(TString(finalStateString).Contains("EU")){
      if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) !=13) continue ;
    }
    else if(TString(finalStateString).Contains("UE")){
      if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) !=11) continue ;
    }
    else{
      cerr<<"problem with lhe level filter definition --> skip event"<<endl;
      continue;
    }

    passingLHEFilter++;

    // if an event pass the cut, fill the associated map                                                              
    leptonContainer lepton1,lepton2,parton1,parton2,neutrino1,neutrino2,vboson1,vboson2;

    lepton1.lepton4V_.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
    lepton1.charge_  = reader->leptonLHEch1;
    lepton1.flavour_ = reader->leptonLHEpid1;  

    lepton2.lepton4V_.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
    lepton2.charge_  = reader->leptonLHEch2;
    lepton2.flavour_ = reader->leptonLHEpid2;  

    parton1.lepton4V_.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
    parton2.lepton4V_.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

    neutrino1.lepton4V_.SetPtEtaPhiM(reader->neutrinoLHEpt1,reader->neutrinoLHEeta1,reader->neutrinoLHEphi1,0.);
    neutrino1.charge_  = 0.;
    neutrino1.flavour_ = reader->neutrinoLHEpid1;  

    neutrino2.lepton4V_.SetPtEtaPhiM(reader->neutrinoLHEpt2,reader->neutrinoLHEeta2,reader->neutrinoLHEphi2,0.);
    neutrino2.charge_  = 0.;
    neutrino2.flavour_ = reader->neutrinoLHEpid2;  

    vboson1.lepton4V_.SetPtEtaPhiM(reader->vbosonLHEpt1,reader->vbosonLHEeta1,reader->vbosonLHEphi1,reader->vbosonLHEm1);
    vboson1.charge_  = reader->vbosonLHEch1;
    vboson1.flavour_ = reader->vbosonLHEpid1;  
    
    vboson2.lepton4V_.SetPtEtaPhiM(reader->vbosonLHEpt2,reader->vbosonLHEeta2,reader->vbosonLHEphi2,reader->vbosonLHEm2);
    vboson2.charge_  = reader->vbosonLHEch2;
    vboson2.flavour_ = reader->vbosonLHEpid2;  

    float minDR_1 = 999;
    float minDR_2 = 999;

    vector<leptonContainer> lepton, neutrino, jets;
    lepton.push_back(lepton1);
    lepton.push_back(lepton2);
    neutrino.push_back(neutrino1);
    neutrino.push_back(neutrino2);
    jets.push_back(parton1);
    jets.push_back(parton2);

    leptonContainer leptFromV1, leptFromV2, neuFromV1, neuFromV2;

    for(size_t iLep= 0; iLep < lepton.size(); iLep++){
      for(size_t iNeu = 0; iNeu < neutrino.size(); iNeu++){
	if((lepton.at(iLep).lepton4V_+neutrino.at(iNeu).lepton4V_).DeltaR(vboson1.lepton4V_) < minDR_1 ){
	  minDR_1    = (lepton.at(iLep).lepton4V_+neutrino.at(iNeu).lepton4V_).DeltaR(vboson1.lepton4V_);
	  leptFromV1 = lepton.at(iLep);
	  neuFromV1  = neutrino.at(iNeu);
	}

	if((lepton.at(iLep).lepton4V_+neutrino.at(iNeu).lepton4V_).DeltaR(vboson2.lepton4V_) < minDR_2){
	  minDR_2    = (lepton.at(iLep).lepton4V_+neutrino.at(iNeu).lepton4V_).DeltaR(vboson2.lepton4V_);
	  leptFromV2 = lepton.at(iLep);
	  neuFromV2  = neutrino.at(iNeu);
	}	
      }
    }

    if(leptFromV1.lepton4V_ == leptFromV2.lepton4V_ or neuFromV1.lepton4V_ == neuFromV2.lepton4V_){
      cerr<<" bad matching with gen W "<<endl;
      continue;
    }
		       

    double costheta1 = 0;
    double costheta2 = 0;
    double Phi  = 0;
    double costhetastar  = 0;
    double Phi1  = 0;

    double costheta1_vbf = 0;
    double costheta2_vbf = 0;
    double Phi_vbf = 0;
    double costhetastar_vbf = 0;
    double Phi1_vbf = 0;


    TLorentzVector VV = vboson1.lepton4V_ + vboson2.lepton4V_;

    if(leptFromV1.charge_ > 0  and leptFromV2.charge_ > 0){

      computeAnglesResonance(VV,vboson1.lepton4V_,neuFromV1.lepton4V_,leptFromV1.lepton4V_,vboson2.lepton4V_,neuFromV2.lepton4V_,leptFromV2.lepton4V_,
			     costheta1,costheta2,Phi,costhetastar,Phi1);

      computeAnglesVBF(VV,vboson1.lepton4V_,neuFromV1.lepton4V_,leptFromV1.lepton4V_,vboson2.lepton4V_,neuFromV2.lepton4V_,leptFromV2.lepton4V_,parton1.lepton4V_,
		       parton2.lepton4V_,costheta1_vbf,costheta2_vbf,Phi_vbf,costhetastar_vbf,Phi1_vbf);
    }
    else if(leptFromV1.charge_ < 0  and leptFromV2.charge_ < 0){

      computeAnglesResonance(VV,vboson1.lepton4V_,leptFromV1.lepton4V_,neuFromV1.lepton4V_,vboson2.lepton4V_,leptFromV2.lepton4V_,neuFromV2.lepton4V_,
			     costheta1,costheta2,Phi,costhetastar,Phi1);

      computeAnglesVBF(VV,vboson1.lepton4V_,leptFromV1.lepton4V_,neuFromV1.lepton4V_,vboson2.lepton4V_,leptFromV2.lepton4V_,neuFromV2.lepton4V_,parton1.lepton4V_,
		       parton2.lepton4V_,costheta1_vbf,costheta2_vbf,Phi_vbf,costhetastar_vbf,Phi1_vbf);

    }

    else if(leptFromV1.charge_ < 0  and leptFromV2.charge_ > 0){
      computeAnglesResonance(VV,vboson1.lepton4V_,leptFromV1.lepton4V_,neuFromV1.lepton4V_,vboson2.lepton4V_,neuFromV2.lepton4V_,leptFromV2.lepton4V_,
			     costheta1,costheta2,Phi,costhetastar,Phi1);

      computeAnglesVBF(VV,vboson1.lepton4V_,leptFromV1.lepton4V_,neuFromV1.lepton4V_,vboson2.lepton4V_,neuFromV2.lepton4V_,leptFromV2.lepton4V_,parton1.lepton4V_,
		       parton2.lepton4V_,costheta1_vbf,costheta2_vbf,Phi_vbf,costhetastar_vbf,Phi1_vbf);
    }
    else if(leptFromV1.charge_ > 0  and leptFromV2.charge_ < 0){

      computeAnglesResonance(VV,vboson1.lepton4V_,neuFromV1.lepton4V_,leptFromV1.lepton4V_,vboson2.lepton4V_,leptFromV2.lepton4V_,neuFromV2.lepton4V_,
			     costheta1,costheta2,Phi,costhetastar,Phi1);

      computeAnglesVBF(VV,vboson1.lepton4V_,neuFromV1.lepton4V_,leptFromV1.lepton4V_,vboson2.lepton4V_,leptFromV2.lepton4V_,neuFromV2.lepton4V_,parton1.lepton4V_,
		       parton2.lepton4V_,costheta1_vbf,costheta2_vbf,Phi_vbf,costhetastar_vbf,Phi1_vbf);

    }
    else{ cerr<<" wrong charge composition "<<endl;
      continue;
    }
    
    float mTR = 0;
    float mR  = 0;

    TLorentzVector L_met ,L_dijet, L_dilepton, L_LLmet; 

    L_met      = neutrino1.lepton4V_ + neutrino2.lepton4V_;
    L_dijet    = parton1.lepton4V_   + parton2.lepton4V_;
    L_dilepton = lepton1.lepton4V_   + lepton2.lepton4V_;
    L_LLmet    = L_dilepton + L_met ;

    // sort in pt before calculating this values
    sort(lepton.rbegin(),lepton.rend());
    sort(jets.rbegin(),jets.rend());

    computeRazor(lepton1.lepton4V_,lepton2.lepton4V_,L_met,mTR,mR);

    if(lepton1.lepton4V_.Pt() < minLeptonCutPt or lepton2.lepton4V_.Pt() < minLeptonCutPt) continue;

    // Loop  on the cut list --> one cut for each polarization
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events
      string name = "WW_EWK_pos_"+to_string(iCut);

      int iBin = 1;
      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
	iBin++;
      }
      if(lepton.size() < 2)  continue ; // skip the event --> only two reco leptons are good                                                                              
      if(jets.size()   < 2)  continue ; // skip the event with less than two reco jet                                                                                       


      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
	iBin++;
      }

      // identify tight leptons and require exactly nLep                                                                                                                       
      if (int(lepton.size()) != CutList.at(iCut).nLep ) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
	iBin++;
      }

      if (lepton.at(0).lepton4V_.Pt() < CutList.at(iCut).ptL.first) continue;
      if (fabs(lepton.at(0).lepton4V_.Eta()) > CutList.at(iCut).etaMaxL) continue ;

      bool badTrailingLepton = false;
      for( size_t iLep = 1 ; iLep < lepton.size(); iLep++){
	if( lepton.at(iLep).lepton4V_.Pt() < CutList.at(iCut).ptL.second or fabs(lepton.at(iLep).lepton4V_.Eta()) > CutList.at(iCut).etaMaxL){
	  badTrailingLepton = true;
	}
      }
      if(badTrailingLepton) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
	iBin++;
      }

      // apply Zmass veto: if nLep == 2 is a veto (WW analysis) if nLep == 3 is a tag (WZ analysis)                                                                         
      bool isZMassVeto = false;
      for(size_t iLept = 0; iLept < lepton.size() ; iLept++){
	if(lepton.at(iLept).lepton4V_.Pt() < minLeptonCutPt) continue;
	for(size_t jLept = iLept+1; jLept < lepton.size() ; jLept++){
	  if(lepton.at(jLept).lepton4V_.Pt() < minLeptonCutPt) continue;
	  if(fabs(lepton.at(jLept).flavour_) != fabs(lepton.at(iLept).flavour_)) continue;
	  if((lepton.at(iLept).lepton4V_ + lepton.at(jLept).lepton4V_).M() > CutList.at(iCut).MllZVeto.first and
	     (lepton.at(iLept).lepton4V_ + lepton.at(jLept).lepton4V_).M() < CutList.at(iCut).MllZVeto.second and
	     lepton.at(iLept).charge_*lepton.at(jLept).charge_ == -1)
	    isZMassVeto = true;
	}
      }

      if(isZMassVeto and CutList.at(iCut).nLep == 2) continue;
      if(not isZMassVeto and CutList.at(iCut).nLep == 3) continue;


      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z veto");
	iBin++;
      }

      // charge cut for same sign final state : zero means no cut, one means same sign, -1 opposite sign                                                                    
      if(CutList.at(iCut).chargeSign != 0){
	int sign = 1;
	for(size_t leptSize = 0; leptSize < lepton.size(); leptSize++) // loop on tight leptns                                                                        
	  sign *= lepton.at(leptSize).charge_;
	if(sign != CutList.at(iCut).chargeSign) continue;
      }

      if(TString(finalStateString).Contains("pp") and (lepton.at(0).charge_ < 0 or lepton.at(1).charge_ < 0))
	continue;

      if(TString(finalStateString).Contains("mm") and (lepton.at(0).charge_ > 0 or lepton.at(1).charge_ > 0))
	continue;


      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"same sign");
	iBin++;
      }

      // flavour selection                                                                                                                                                   
      if(CutList.at(iCut).flavour != 0){
	int flavour = 0;
	int sameflavour = 0;
	for(size_t leptSize = 0; leptSize < lepton.size(); leptSize++) // loop on tight leptns                                                                              
	  flavour += fabs(lepton.at(leptSize).flavour_) ;
	if(flavour/lepton.size() == 11 or flavour/lepton.size() == 13 ) sameflavour = 1;
	else sameflavour = -1;
	if(sameflavour != CutList.at(iCut).flavour) continue;
      }

      if(TString(finalStateString).Contains("UU")){
	if(fabs(lepton.at(0).flavour_) != 13) continue;
	if(fabs(lepton.at(1).flavour_) != 13) continue;
      }
      else if(TString(finalStateString).Contains("EE")){
	if(fabs(lepton.at(0).flavour_) != 11) continue;
	if(fabs(lepton.at(1).flavour_) != 11) continue;
      }

      else if(TString(finalStateString).Contains("EU")){
	if(fabs(lepton.at(0).flavour_) != 11) continue;
	if(fabs(lepton.at(1).flavour_) != 13) continue;
      }
      else if(TString(finalStateString).Contains("UE")){
	if(fabs(lepton.at(0).flavour_) != 13) continue;
	if(fabs(lepton.at(1).flavour_) != 11) continue;
      }

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
	iBin++;
      }

      // met cut                                                                                                                                                                     
      if(reader->pfmet_puppi < CutList.at(iCut).MET) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"met selection");
	iBin++;
      }


      if(jets.at(0).lepton4V_.Pt() < minJetCutPt) continue;
      if(jets.at(1).lepton4V_.Pt() < minJetCutPt) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
	iBin++;
      }

      // apply Mll cut                                                                                                                                                      
      if(L_dilepton.M() < CutList.at(iCut).Mll.first or L_dilepton.M() > CutList.at(iCut).Mll.second) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
	iBin++;
      }

      // apply VBF cuts                                                                                                                                                     
      if(L_dijet.M() < CutList.at(iCut).Mjj) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
	iBin++;
      }

      if(fabs(jets.at(0).lepton4V_.Eta()-jets.at(1).lepton4V_.Eta()) < CutList.at(iCut).DetaJJ) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
	iBin++;
      }

      if(fabs(lepton.at(0).lepton4V_.Eta()-lepton.at(1).lepton4V_.Eta()) > CutList.at(iCut).DetaLL) continue;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaLL");
	iBin++;
      }



      // apply further cuts for fakes                                                                                                                                                
      float aveEta    = 0.5*(jets.at(0).lepton4V_.Eta()+jets.at(1).lepton4V_.Eta());
      float leadZep   = fabs((lepton.at(0).lepton4V_.Eta()-aveEta)/(fabs(jets.at(0).lepton4V_.Eta()-jets.at(1).lepton4V_.Eta())));
      float trailZep  = fabs((lepton.at(1).lepton4V_.Eta()-aveEta)/(fabs(jets.at(0).lepton4V_.Eta()-jets.at(1).lepton4V_.Eta())));


      if(L_dijet.DeltaR(L_dilepton) > CutList.at(iCut).dRlJ) continue;
      if(jets.at(0).lepton4V_.DeltaR(lepton.at(1).lepton4V_) > CutList.at(iCut).dRlJ) continue;

      if(leadZep  > CutList.at(iCut).lZep) continue ;
      if(trailZep > CutList.at(iCut).lZep) continue ;

      if(histoCutEff.size()!=0){
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"fake bkg reduction");
	iBin++;
      }

      // polarized cut                                                                                                                                                        
      if(CutList.at(iCut).polarization != 99 or TString(name).Contains("Madgraph")){

	int polarizationFlag = 0;

	if(CutList.at(iCut).polarization != 99){

	  
	  if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
	    polarizationFlag = 0 ; //LL                                                                                                                                       
	  else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
	    polarizationFlag = 1 ; //TL                                                                                                                                        
	  else
	    polarizationFlag = 2 ; //TT                                                                                                                                       

	  if(polarizationFlag != CutList.at(iCut).polarization) continue;
	  
	  if(histoCutEff.size()!=0){
	    histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	    histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	    iBin++;
	  }
	}
	else if(TString(name).Contains("Madgraph")){
	  if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
	    polarizationFlag = 0 ; //LL                                                                                                                                         
	  else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
	    polarizationFlag = 1 ; //TL                                                                                                                                        
	  else
	    polarizationFlag = 2 ; //TT                                                                                                                                       
	  
	  if(TString(name).Contains("_LL")  and polarizationFlag != 0) continue;
	  if(TString(name).Contains("_TL")  and polarizationFlag != 1) continue;
	  if(TString(name).Contains("_TT")  and polarizationFlag != 2) continue;
	  
	  if(histoCutEff.size()!=0){
	    histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	    histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	    iBin++;
	  }
	}
      }
      else{
	if(histoCutEff.size()!=0){
	  histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);
	  histoCutEff[name+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	  iBin++;
	}
      }
        
    
      float asimL = (lepton1.lepton4V_.Pt()-lepton2.lepton4V_.Pt())/(lepton1.lepton4V_.Pt()+lepton2.lepton4V_.Pt()) ;
      
      
      float asimJ    = (parton1.lepton4V_.Pt()-parton2.lepton4V_.Pt())/(parton1.lepton4V_.Pt()+parton2.lepton4V_.Pt()) ;
      float Rvar     = (lepton1.lepton4V_.Pt()*lepton2.lepton4V_.Pt())/(parton1.lepton4V_.Pt()*parton2.lepton4V_.Pt()) ;
      
      // loop on variables
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	histoContainer tmpPlot;
	tmpPlot.cutName = CutList.at(iCut).cutLayerName;
	tmpPlot.varName = variableList.at(iVar).variableName;
	vector<histoContainer>::iterator itVec ;
	itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
	if(itVec == plotVector.end()){
	  cerr<<"Problem -->plot not found for "<<CutList.at(iCut).cutLayerName<<"  "<<variableList.at(iVar).variableName<<endl;
	continue ;
	}
	
	// histoCutEffor boson info
	if(variableList.at(iVar).variableName == "ptV1")
	  itVec->histogram->Fill(vboson1.lepton4V_.Pt(),1.*weight) ; 

	else if(variableList.at(iVar).variableName == "ptV2")
	  itVec->histogram->Fill(vboson2.lepton4V_.Pt(),weight) ;

	else if(variableList.at(iVar).variableName == "etaV1")
	  itVec->histogram->Fill(vboson1.lepton4V_.Eta(),weight) ;
	
	else if(variableList.at(iVar).variableName == "etaV2")
          itVec->histogram->Fill(vboson2.lepton4V_.Eta(),weight) ;
	
	else if(variableList.at(iVar).variableName == "ptVV")
	  itVec->histogram->Fill(L_dijet.Pt(),weight) ;
	
	else if(variableList.at(iVar).variableName == "mVV")
	  itVec->histogram->Fill(L_dijet.M(),weight) ;
	
      
	// decay information
	if(variableList.at(iVar).variableName == "costheta1")
	  itVec->histogram->Fill(fabs(costheta1),1.*weight) ; 
	
	else if(variableList.at(iVar).variableName == "costheta2")
	  itVec->histogram->Fill(fabs(costheta2),weight) ;
	
	if(variableList.at(iVar).variableName == "costheta1_vbf")
	  itVec->histogram->Fill(fabs(costheta1_vbf),1.*weight) ; 
      
	else if(variableList.at(iVar).variableName == "costheta2_vbf")
	  itVec->histogram->Fill(fabs(costheta2_vbf),weight) ;
            
	if(variableList.at(iVar).variableName == "Phi")
	  itVec->histogram->Fill(fabs(Phi),1.*weight) ; 
	
	else if(variableList.at(iVar).variableName == "Phi1")
	  itVec->histogram->Fill(fabs(Phi1),weight) ;
      
	if(variableList.at(iVar).variableName == "Phi_vbf")
	  itVec->histogram->Fill(fabs(Phi_vbf),1.*weight) ; 
      
	else if(variableList.at(iVar).variableName == "Phi1_vbf")
	  itVec->histogram->Fill(fabs(Phi1_vbf),weight) ;
            
	else if(variableList.at(iVar).variableName == "costhetastar")
	  itVec->histogram->Fill(fabs(costhetastar),weight) ;
	
	else if(variableList.at(iVar).variableName == "costhetastar_vbf")
          itVec->histogram->Fill(fabs(costhetastar_vbf),weight) ;
            
	else if(variableList.at(iVar).variableName == "mTR")
	  itVec->histogram->Fill(mTR,weight) ;
	      
	else if(variableList.at(iVar).variableName == "mR")
	  itVec->histogram->Fill(mR,weight) ;
	
      
	// jet info
      
	if(variableList.at(iVar).variableName == "ptj1")
	  itVec->histogram->Fill(parton1.lepton4V_.Pt(),1.*weight) ; 
	
	else if(variableList.at(iVar).variableName == "ptj2")
	  itVec->histogram->Fill(parton2.lepton4V_.Pt(),weight) ;
	
	else if(variableList.at(iVar).variableName == "etaj1")
	  itVec->histogram->Fill(parton1.lepton4V_.Eta(),weight) ;
      
	else if(variableList.at(iVar).variableName == "etaj2")
	  itVec->histogram->Fill(parton2.lepton4V_.Eta(),weight) ;
	
	else if(variableList.at(iVar).variableName == "detajj")
	  itVec->histogram->Fill(fabs(parton1.lepton4V_.Eta()-parton2.lepton4V_.Eta()),weight) ;
      
	else if(variableList.at(iVar).variableName == "ptjj")
	  itVec->histogram->Fill(L_dijet.Pt(),weight) ;
	
	else if(variableList.at(iVar).variableName == "mjj")
	  itVec->histogram->Fill(L_dijet.M(),weight) ;
            
	else if(variableList.at(iVar).variableName == "Asim_j")
	  itVec->histogram->Fill(asimJ,weight) ;
	
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ")
	  itVec->histogram->Fill(fabs(parton1.lepton4V_.DeltaPhi(parton2.lepton4V_)),weight) ;
	
	else if(variableList.at(iVar).variableName == "ptl1")
	  itVec->histogram->Fill(lepton1.lepton4V_.Pt(),weight) ;
	
	else if(variableList.at(iVar).variableName == "ptl2")
	  itVec->histogram->Fill(lepton2.lepton4V_.Pt(),weight) ;
           
	else if(variableList.at(iVar).variableName == "etal1")
	  itVec->histogram->Fill(lepton1.lepton4V_.Eta(),weight) ;
      
	else if(variableList.at(iVar).variableName == "etal2")
	  itVec->histogram->Fill(lepton2.lepton4V_.Eta(),weight) ;
      
	else if(variableList.at(iVar).variableName == "mll")
	  itVec->histogram->Fill(L_dilepton.M(),weight) ;
	
	else if(variableList.at(iVar).variableName == "ptll")
	  itVec->histogram->Fill(L_dilepton.Pt(),weight) ;
      
	else if(variableList.at(iVar).variableName == "DeltaPhi_LL")
	  itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(lepton2.lepton4V_)),weight) ;
	
	else if(variableList.at(iVar).variableName == "Asim_l")
	  itVec->histogram->Fill(asimL,weight) ;
	      
	else if(variableList.at(iVar).variableName == "met")
	  itVec->histogram->Fill(L_met.Pt(),weight) ;
	      
	else if(variableList.at(iVar).variableName == "R")
	  itVec->histogram->Fill(Rvar,weight) ;
	      
	else if(variableList.at(iVar).variableName == "DeltaPhi_LMet")
	  itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(L_met)),weight) ;

	else if(variableList.at(iVar).variableName == "ptLMet")
	itVec->histogram->Fill((lepton1.lepton4V_ + L_met).Pt(),weight) ;

	else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet")
	  itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(L_met)),weight) ;

	else if(variableList.at(iVar).variableName == "ptTLMet")
	  itVec->histogram->Fill((lepton2.lepton4V_ + L_met).Pt(),weight) ;

	else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet")
	  itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weight) ;

	else if(variableList.at(iVar).variableName == "ptLLMet")
	itVec->histogram->Fill((L_dilepton + L_met).Pt(),weight) ;

	///
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJL")
	itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(parton1.lepton4V_)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptLJL")
	itVec->histogram->Fill((lepton1.lepton4V_+parton1.lepton4V_).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJL")
	itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(parton2.lepton4V_)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptTJL")
	itVec->histogram->Fill((lepton1.lepton4V_+parton2.lepton4V_).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJL")
	itVec->histogram->Fill(fabs(lepton1.lepton4V_.DeltaPhi(L_dijet)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptJJL")
	itVec->histogram->Fill((lepton1.lepton4V_+L_dijet).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL")
	itVec->histogram->Fill(fabs(lepton2.lepton4V_.DeltaPhi(parton1.lepton4V_)),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL")
	itVec->histogram->Fill(fabs(lepton2.lepton4V_.DeltaPhi(parton2.lepton4V_)),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL")
	itVec->histogram->Fill(fabs(lepton2.lepton4V_.DeltaPhi(L_dijet)),weight) ;
          
      else if(variableList.at(iVar).variableName == "ptLJTL")
	itVec->histogram->Fill((lepton2.lepton4V_+parton1.lepton4V_).Pt(),weight) ;
            
      else if(variableList.at(iVar).variableName == "ptTJTL")
	itVec->histogram->Fill((lepton2.lepton4V_+parton2.lepton4V_).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "ptJJTL")
	itVec->histogram->Fill((lepton2.lepton4V_+L_dijet).Pt(),weight) ;
      
            
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL")
	itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(parton1.lepton4V_)),weight) ;
      

      else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL")
	itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(parton2.lepton4V_)),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL")
	itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "ptLJLL")
	itVec->histogram->Fill((L_dilepton+parton1.lepton4V_).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "ptTJLL")
	itVec->histogram->Fill((L_dilepton+parton2.lepton4V_).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "ptJJLL")
	itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weight) ;
      

      ///
      else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet")
	itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptJJMet")
	itVec->histogram->Fill((L_dijet+L_met).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet")
	itVec->histogram->Fill(fabs(parton1.lepton4V_.DeltaPhi(L_met)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptLJMet")
	itVec->histogram->Fill((parton1.lepton4V_+L_met).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet")
	itVec->histogram->Fill(fabs(parton2.lepton4V_.DeltaPhi(L_met)),weight) ;
      
      else if(variableList.at(iVar).variableName == "ptTJMet")
	itVec->histogram->Fill((parton2.lepton4V_+L_met).Pt(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "mlljj")
	itVec->histogram->Fill((L_dilepton+L_dijet).M(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "mlljjmet")
	itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weight) ;
      
      
      else if(variableList.at(iVar).variableName == "mTH")
	itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weight) ;

      }
    }
  }      
  
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
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

  cCanvas->cd();

  TPad* upperPad = new TPad("upperPad", "upperPad", .005, .180, .995, .980);
  TPad* lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .18);
  lowerPad->SetGridx();
  lowerPad->SetGridy();
  upperPad->SetLeftMargin(0.12);
  upperPad->SetRightMargin(0.1);
  lowerPad->SetLeftMargin(0.12);
  lowerPad->SetRightMargin(0.1);
  lowerPad->SetTopMargin(0.002);

  lowerPad->Draw();
  upperPad->Draw();

  TCanvas *cCanvasNorm = new TCanvas("cCanvasNorm","",1,52,550,550);
  cCanvasNorm->SetTicks();
  cCanvasNorm->SetFillColor(0);
  cCanvasNorm->SetBorderMode(0);
  cCanvasNorm->SetBorderSize(2);
  cCanvasNorm->SetTickx(1);
  cCanvasNorm->SetTicky(1);
  cCanvasNorm->SetRightMargin(0.05);
  cCanvasNorm->SetBottomMargin(0.12);
  cCanvasNorm->SetFrameBorderMode(0);

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
  TLatex * tex3 = new TLatex(0.295,0.92,"Simulation Preliminary");
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
  legend->SetNColumns (3) ;
 
  // make the plot on the same canvas for each variable (legend entry is the cut layer name)  
  vector<TH1F*>  numerator ;
  vector<TH1F*>  denominator ;
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
    numerator.clear();
    denominator.clear();

    for(size_t iCut = 0; iCut < CutList.size(); iCut++){ // loop on cuts
        histoContainer tmpPlot;
        tmpPlot.cutName = CutList.at(iCut).cutLayerName;
        tmpPlot.varName = variableList.at(iVar).variableName;
        vector<histoContainer>::iterator itVec ;
        itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
        if(itVec == plotVector.end()){
          cerr<<"Problem -->plot not found for "<<CutList.at(iCut).cutLayerName<<"  "<<variableList.at(iVar).variableName<<endl;
        }

       
        itVec->histogram->GetXaxis()->SetTitleSize(0.04);
        itVec->histogram->GetXaxis()->SetTitleOffset(1.16);
        itVec->histogram->GetXaxis()->SetLabelSize(0.04);

        itVec->histogram->GetYaxis()->SetRangeUser(0.001,itVec->histogram->GetMaximum()*1.25);
        itVec->histogram->GetYaxis()->SetTitleSize(0.05);
        itVec->histogram->GetYaxis()->SetTitleOffset(1.20);
        itVec->histogram->GetYaxis()->SetLabelSize(0.04);

        itVec->histogram->SetLineColor(iCut+1);

        if(iCut %2 == 0)
          itVec->histogram->SetLineStyle(1);
        else
          itVec->histogram->SetLineStyle(2);

        itVec->histogram->SetLineWidth(2);
        itVec->histogram->GetYaxis()->SetTitle("#sigma x lumi");

        upperPad->cd();

        if(iCut == 0) 
          itVec->histogram->Draw("hist");
        else
          itVec->histogram->Draw("hist same");

        legend->AddEntry(itVec->histogram,CutList.at(iCut).cutLayerName.c_str(),"l");

        if(itVec->findCutByLabel("LL")) numerator.push_back(itVec->histogram);
        denominator.push_back(itVec->histogram);

	cCanvasNorm->cd();

	TH1F* htempNorm = (TH1F*) itVec->histogram->Clone((string(itVec->histogram->GetName())+"_norm").c_str());
	htempNorm->Scale(1./itVec->histogram->Integral());

	htempNorm->GetYaxis()->SetRangeUser(0.,htempNorm->GetMaximum()*1.5);

        if(iCut == 0)
          htempNorm->Draw("hist");
        else
          htempNorm->Draw("hist same");
    }
    
    // make ratio plot
    lowerPad->cd();
    
    TH1F* numTotal = 0;
    TH1F* denTotal = 0;
    TH1F* ratio    = 0;
    TH1F* ratioW   = 0;

    for(size_t itNum = 0; itNum < numerator.size(); itNum ++){
      if(itNum == 0 and ratio == 0) 
        numTotal = (TH1F*) numerator.at(itNum)->Clone(("Num_"+string(numerator.at(itNum)->GetName())).c_str());
      else if(ratio !=0) 
        numTotal->Add(numerator.at(itNum));
    }

    for(size_t itDen = 0; itDen < denominator.size(); itDen ++){
      if(itDen == 0 and denTotal == 0 ) {
        denTotal = (TH1F*) denominator.at(itDen)->Clone(("Den_"+string(denominator.at(itDen)->GetName())).c_str());
      }
      else if(denTotal !=0){  
        denTotal->Add(denominator.at(itDen));
      }
    }
    
    ratio = new TH1F(("Ratio_"+string(denominator.at(0)->GetName())).c_str(),"",numTotal->GetNbinsX(),numTotal->GetBinLowEdge(1),numTotal->GetBinLowEdge(numTotal->GetNbinsX()+1));
    ratio->GetYaxis()->SetTitle("S/(#sqrt{S+B})");
    ratio->SetMarkerSize(1.1);

    ratioW = new TH1F(("ratioW_"+string(denominator.at(0)->GetName())).c_str(),"",numTotal->GetNbinsX(),numTotal->GetBinLowEdge(1),numTotal->GetBinLowEdge(numTotal->GetNbinsX()+1));
    ratioW->GetYaxis()->SetTitle("weighted S/(#sqrt{S+B})");
    ratioW->SetMarkerSize(1.1);

    TString name = "norm_" ;
    name += denTotal->GetName () ;
    TH1F * norm_denTotal = (TH1F *) denTotal->Clone (name) ;
    norm_denTotal->Scale (1. / norm_denTotal->GetMaximum ()) ; 
    // weight the S/sqrt (B) by the shape of the total, 
    // so that only bins with a lot of stats become visibly significant
    for(int iBin = 0; iBin < ratio->GetNbinsX()+1; iBin++){
      if(denTotal->GetBinContent(iBin) !=0){ 
        ratioW->SetBinContent(iBin,
			      norm_denTotal->GetBinContent (iBin) * numTotal->GetBinContent(iBin) / 
			      sqrt(denTotal->GetBinContent(iBin)));
        ratio->SetBinContent(iBin,
			     numTotal->GetBinContent(iBin) / sqrt(denTotal->GetBinContent(iBin)));
      }
      else 
	ratio->SetBinContent(iBin,0.);
    }
 
    ratio->GetXaxis()->SetTitle("");
    ratio->SetLineColor(kBlue);
    ratio->SetLineStyle(2);
    ratio->SetLineWidth(2);
    ratio->GetXaxis()->SetLabelOffset(999);
    ratio->GetXaxis()->SetLabelSize(0);
    ratio->GetYaxis()->SetLabelSize(0.15);
    ratio->GetYaxis()->SetTitleSize(0.15);
    ratio->GetYaxis()->SetTitleOffset(0.30);
    ratio->GetYaxis()->SetNdivisions(504);

    ratioW->GetXaxis()->SetTitle("");
    ratioW->SetLineColor(kBlack);
    ratioW->SetLineWidth(2);
    ratioW->GetXaxis()->SetLabelOffset(999);
    ratioW->GetXaxis()->SetLabelSize(0);
    ratioW->GetYaxis()->SetLabelSize(0.15);
    ratioW->GetYaxis()->SetTitleSize(0.15);
    ratioW->GetYaxis()->SetTitleOffset(0.30);
    ratioW->GetYaxis()->SetNdivisions(504);

    ratio->GetYaxis()->SetRange(min(ratio->GetMinimum(),ratioW->GetMinimum())*0.9,max(ratio->GetMaximum(),ratioW->GetMaximum())*1.1);    


    TH1F * frame = lowerPad->DrawFrame (ratio->GetXaxis ()->GetXmin (), 0., 
                                        ratio->GetXaxis ()->GetXmax (), 2.) ;
    frame->GetXaxis()->SetTitle (ratio->GetXaxis ()->GetTitle ()) ;
    frame->GetYaxis()->SetTitle (ratio->GetYaxis ()->GetTitle ()) ;
    frame->GetXaxis()->SetLabelOffset(999);
    frame->GetXaxis()->SetLabelSize(0);
    frame->GetYaxis()->SetLabelSize(0.15);
    frame->GetYaxis()->SetTitleSize(0.15);
    frame->GetYaxis()->SetTitleOffset(0.30);
    frame->GetYaxis()->SetNdivisions(504);

    ratio->Draw("P");    
    ratioW->Draw("Lsame");    
    
    upperPad->cd();
    
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");
    legend->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/xs/"+variableList.at(iVar).variableName+".root").c_str(),"root");

    cCanvasNorm->cd();

    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");
    legend->Draw("same");

    cCanvasNorm->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvasNorm->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvasNorm->SaveAs(string("output/"+outputPlotDirectory+"/norm/"+variableList.at(iVar).variableName+".root").c_str(),"root");

    
    legend->Clear();
    
  } // loop on var

  cout<<"LHE filter efficiency : "<<passingLHEFilter<<" totEvent "<<totEvent<<" efficiency "<<float(passingLHEFilter)/float(totEvent)*100<<" % "<<endl;
  
  //Normalize histograms
  for(size_t ihisto = 0; ihisto < plotVector.size(); ihisto++){
    if(plotVector.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo "<<plotVector.at(ihisto).histogram->GetName()<<" unweighted "<<plotVector.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVector.at(ihisto).histogram->Integral(0,plotVector.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }	          

  return 0 ;
}  

