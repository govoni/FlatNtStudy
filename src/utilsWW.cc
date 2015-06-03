#include "utils.h"
#include <memory>

void fillHistoWW( plotter & analysisPlots,
		  const string & sampleName,
		  const int    & samplePosition,
		  const string & cutLayerName,
		  vector<variableContainer> & VariableList,
		  vector<leptonContainer> & leptonsIsoTight,
		  vector<leptonContainer> & softMuons,
		  vector<jetContainer> & RecoJets,
		  vector<jetContainer> & GenJets,
		  trackJetEvent & trackEvent,
		  TLorentzVector & L_met,
		  const string & systematicName,
		  const float & eventFakeWeight
                ){

  // some basic lorentz vectors                                                                                                                                                
  TLorentzVector L_dilepton, L_LLmet;
  TLorentzVector L_dijet, L_dijet_gen;

  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
  L_LLmet = L_dilepton + L_met ;

  float asimJ = 0, asimL = 0, Rvar = 0, aveEta = 0;
  float asimGenJ = 0, RvarGen = 0;

  asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;

  if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
    aveEta   = 0.5*(RecoJets.at(0).jet4V_.Eta()+RecoJets.at(1).jet4V_.Eta());
  }

  if(GenJets.size() >= 2){
    L_dijet_gen   = GenJets.at(0).jet4V_ + GenJets.at(1).jet4V_;
    asimGenJ      = (GenJets.at(0).jet4V_.Pt()-GenJets.at(1).jet4V_.Pt())/(GenJets.at(0).jet4V_.Pt()+GenJets.at(1).jet4V_.Pt()) ;
    RvarGen       = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(GenJets.at(0).jet4V_.Pt()*GenJets.at(1).jet4V_.Pt()) ;
  }

  TString Name ;
  string  NameSample ;
  if(TString(sampleName).Contains("Madgraph_")){
    Name = sampleName;
    Name.ReplaceAll("Madgraph_","");
    NameSample = string(Name) ;
  }
  else
    NameSample = sampleName ;

  // cunt b-jet                                                                                                                                                                 
  // make mass between l1 and l2 and jet closer to W                                                                                                                            
  // make mass between l1 and l2 and pair of jet closer to T                                                                                                                   
 
  int nBjetsL = 0, nBjetsM = 0, nBjetsT = 0;

  TLorentzVector L_l1j, L_l2j, L_l1jj, L_l2jj;

  float dMassW_l1 = 9999, dMassTop_l1 = 9999;
  float dMassW_l2 = 9999, dMassTop_l2 = 9999;

  for(size_t iJet = 0; iJet < RecoJets.size(); iJet++){
    if(fabs(RecoJets.at(iJet).jet4V_.Eta()) >2.5) continue;
    if(RecoJets.at(iJet).btag_ >= 1)
      nBjetsL ++;
    if(RecoJets.at(iJet).btag_ >= 2)
      nBjetsM++;
    if(RecoJets.at(iJet).btag_ >= 3)
      nBjetsT++;

    if(fabs((RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(0).lepton4V_).M()-pdgWMass) < dMassW_l1){
      dMassW_l1 = fabs((RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(0).lepton4V_).M()-pdgWMass);
      L_l1j = RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(0).lepton4V_;
    }

    if(fabs((RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(1).lepton4V_).M()-pdgWMass) < dMassW_l2){
      dMassW_l2 = fabs((RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(1).lepton4V_).M()-pdgWMass);
      L_l2j = RecoJets.at(iJet).jet4V_+leptonsIsoTight.at(1).lepton4V_;
    }

    for(size_t kJet = iJet+1; kJet < RecoJets.size(); kJet++){

      if(fabs((RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(0).lepton4V_).M()-pdgTopMass) < dMassTop_l1){
        dMassTop_l1 = fabs((RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(0).lepton4V_).M()-pdgTopMass);
        L_l1jj = RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(0).lepton4V_;
      }

      if(fabs((RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(1).lepton4V_).M()-pdgTopMass) < dMassTop_l2){
        dMassTop_l2 = fabs((RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(1).lepton4V_).M()-pdgTopMass);
        L_l2jj = RecoJets.at(iJet).jet4V_+RecoJets.at(kJet).jet4V_+leptonsIsoTight.at(1).lepton4V_;
      }
    }
  }

  TLorentzVector L_lj, L_ljj;
  if(fabs(dMassW_l1) < dMassW_l2)
    L_lj = L_l1j ;
  else
    L_lj = L_l2j ;
  if(fabs(dMassTop_l1) < dMassTop_l2)
    L_ljj = L_l1jj ;
  else
    L_ljj = L_l2jj ;

  // razor variables                                                                                                                                                           
  float mTR = 0;
  float mR  = 0;

  computeRazor(leptonsIsoTight.at(0).lepton4V_,leptonsIsoTight.at(1).lepton4V_,L_met,mTR,mR);

  for(size_t iVar = 0; iVar < VariableList.size(); iVar++){

    if(VariableList.at(iVar).variableName == "mR" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  mR, eventFakeWeight,systematicName) ;
    }
    if(VariableList.at(iVar).variableName == "mTR" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  mTR, eventFakeWeight,systematicName) ;
    }

    // track jets                                                                                                                                                               
    if(VariableList.at(iVar).variableName == "numTkjets" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.numTrack_, eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "numTkjets_In"  and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.numTrackIn_, eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "numTkjets_Out" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.numTrackOut_, eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "HTtkjets"      and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.HTTrack_,    eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "HTtkjets_In"   and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.HTTrackIn_,  eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "HTtkjets_Out"  and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  trackEvent.HTTrackOut_, eventFakeWeight,systematicName) ;
    }

    // njets                                                                                                                                                                    
    else if(VariableList.at(iVar).variableName == "nbjetsL"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  nBjetsL,          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "nbjetsM"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  nBjetsM,          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "nbjetsT"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  nBjetsT,          eventFakeWeight,systematicName) ;
    }

    else if(VariableList.at(iVar).variableName == "nSoftMu"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  softMuons.size(),   eventFakeWeight,systematicName) ;
    }

    else if(VariableList.at(iVar).variableName == "ml1jW"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_l1j.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "ml2jW"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_l2j.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "mljW"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_lj.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "mljW"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_l2j.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "ml1jjT"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_l1jj.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "ml2jjT"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_l2jj.M(),          eventFakeWeight,systematicName) ;
    }
    else if(VariableList.at(iVar).variableName == "mljjT"){
      analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName, VariableList.at(iVar).variableName,  L_ljj.M(),          eventFakeWeight,systematicName) ;
    }


    // jet based variables                                                                                                                                                     
    if(VariableList.at(iVar).variableName == "ptj1" and RecoJets.size() >= 1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptj1_gen" and GenJets.size() >= 1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptj2" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptj2_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "etaj1" and RecoJets.size() >= 1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Eta(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "etaj1_gen" and GenJets.size() >= 1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Eta(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "etaj2" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "etaj2_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
    }

    // btagging section                                                                                                                                                        
    else if(VariableList.at(iVar).variableName == "btagj1" and RecoJets.size() >= 1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).btag_,eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "btagj2" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).btag_,eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "detajj" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "detajj_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.Eta()-GenJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "dRjj" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(RecoJets.at(1).jet4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dRjj_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.DeltaR(GenJets.at(1).jet4V_),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "mjj" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "mjj_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.M(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptjj" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptjj_gen" and GenJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,asimJ,eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "Asim_j_gen" and GenJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,asimGenJ,eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ"       and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen"   and GenJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.DeltaPhi(GenJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "etaj1etaj2"       and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "etaj1etaj2_gen"       and GenJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Eta()*GenJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
    }

    // lepton based quantities                                                                                                                                                 
    else if(VariableList.at(iVar).variableName == "ptl1"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
     }
    else if(VariableList.at(iVar).variableName == "ptl2"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
     }

    else if(VariableList.at(iVar).variableName == "etal1"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Eta(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "etal2"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.Eta(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptll"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dRll"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "mll" or VariableList.at(iVar).variableName == "mll_v2" or VariableList.at(iVar).variableName == "mll_v3" or VariableList.at(iVar).variableName == "mll_v4"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dilepton.M(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "etal1etal2"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Eta()*leptonsIsoTight.at(1).lepton4V_.Eta(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_LL"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaEta_LL"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "Asim_l"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,asimL,eventFakeWeight,systematicName);
    }
    // met                                                                                                                                                                      
    else if(VariableList.at(iVar).variableName == "met"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_met.Pt(),eventFakeWeight,systematicName);
    }
    /// mixed variables using all event objects                                                                                                                                
    else if(VariableList.at(iVar).variableName == "leadLepZep" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs((leptonsIsoTight.at(0).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "trailLepZep" and RecoJets.size() >= 2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs((leptonsIsoTight.at(1).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "R"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,Rvar,eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "R_gen"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RvarGen,eventFakeWeight,systematicName);
    }

    // lepton and met                                                                                                                                                           
    else if(VariableList.at(iVar).variableName == "DeltaPhi_LMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptLMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(leptonsIsoTight.at(0).lepton4V_+L_met).Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_TLMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptTLMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(leptonsIsoTight.at(1).lepton4V_+L_met).Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_LLMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptLLMet"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }


    // lepton and jets                                                                                                                                                          
    else if(VariableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "dR_LJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_LJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_LJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(L_dilepton),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptLJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptLJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptLJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+L_dilepton).Pt(),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "dR_TJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_TJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_TJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(L_dilepton),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "ptTJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptTJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptTJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+L_dilepton).Pt(),eventFakeWeight,systematicName);
    }


    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "dR_JJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_JJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_JJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_dilepton),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptJJL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dijet+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptJJTL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dijet+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "ptJJLL" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_dilepton).Pt(),eventFakeWeight,systematicName);
    }

    // jet and met                                                                                                                                                              
    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptJJMet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met" and GenJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet_gen.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size()>=1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptLJMet" and RecoJets.size()>=1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+L_met).Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_gen_Met" and GenJets.size()>=1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "ptTJMet" and RecoJets.size()>=1){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+L_met).Pt(),eventFakeWeight,systematicName);
    }

    // Lepton JJ_LLMET                                                                                                                                                          
    else if(VariableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_LLmet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_LLmet)),eventFakeWeight,systematicName);
    }
    else if(VariableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_LLmet),eventFakeWeight,systematicName);
    }

    // some invariant mass                                                                                                                                                      
    else if(VariableList.at(iVar).variableName == "mlljj" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_dijet).M(),eventFakeWeight,systematicName);
    }

    else if(VariableList.at(iVar).variableName == "mlljjmet" and RecoJets.size()>=2){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_dijet+L_met).M(),eventFakeWeight,systematicName);
    }

    // tranvserse mass                                                                                                                                                          
    else if(VariableList.at(iVar).variableName == "mTH"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
  }
}


// 2D histograms                                                                                                                                                                
void fillHisto2DWW ( plotter & analysisPlots,
		     const string & sampleName,
		     const int    & samplePosition,
		     const string & cutLayerName,
		     vector<variableContainer2D> & VariableList2D,
		     vector<leptonContainer> & leptonsIsoTight,
		     vector<leptonContainer> & softMuons,
		     vector<jetContainer> & RecoJets,
		     vector<jetContainer> & GenJets,
		     trackJetEvent & trackEvent,
		     TLorentzVector & L_met,
		     const string & systematicName,
		     const float & eventFakeWeight){

  // some basic lorentz vectors                                                                                                                                                 
  TLorentzVector L_dilepton, L_LLmet;
  TLorentzVector L_dijet, L_dijet_gen;


  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;

  L_LLmet = L_dilepton + L_met ;

  float asimJ = 0, asimL = 0, Rvar = 0;

  asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;

  if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
  }


  TString Name ;
  string NameSample ;
  if(TString(sampleName).Contains("Madgraph_")){
    Name = sampleName;
    Name.ReplaceAll("Madgraph_","");
    NameSample = string(Name) ;
  }

  else
    NameSample = sampleName ;

  // razor variables                                                                                                                                                            
  float mTR = 0;
  float mR  = 0;

  computeRazor(leptonsIsoTight.at(0).lepton4V_,leptonsIsoTight.at(1).lepton4V_,L_met,mTR,mR);

  for(size_t iVar = 0; iVar < VariableList2D.size(); iVar++){

    // tranvserse mass                                                                                                                                                          
    if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "Asim_l" ){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,asimL,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "R"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,Rvar,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 asimJ,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimJ,mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "R"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,Rvar,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 asimL,(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "Asim_l" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 asimL,mTR,eventFakeWeight,systematicName);
    }

    ////                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,L_met.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "R" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 Rvar,mTR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),mTR,eventFakeWeight,systematicName);
    }
    ////                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(L_dijet.DeltaPhi(L_dilepton)),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(L_dijet.DeltaPhi(L_dilepton)),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJLL" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dijet.DeltaPhi(L_dilepton)),mTR,eventFakeWeight,systematicName);
    }

    ////                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LL" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),mTR,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_met.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }


    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),L_met.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.M(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.M(),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mTH" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_LLMet"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),mR,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "met" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_met.Pt(),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "met"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),L_met.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "detajj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "mjj"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "mTH"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "mll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(L_dilepton.DeltaPhi(L_met)),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 fabs(L_dilepton.DeltaPhi(L_met)),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_LLMet" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 fabs(L_dilepton.DeltaPhi(L_met)),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptJJLL"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),(L_dilepton+L_dijet).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dilepton.M(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dilepton.M(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dilepton.M(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.M(),mTR,eventFakeWeight,systematicName);
    }
    ///                                                                                                                                                                      
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptJJ"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 (L_dilepton+L_dijet).Pt(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton+L_dijet).Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton+L_dijet).Pt(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton+L_dijet).Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 (L_dilepton+L_dijet).Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 (L_dilepton+L_dijet).Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton+L_dijet).Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptJJLL" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton+L_dijet).Pt(),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                         
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "ptll"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.Pt(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
				 L_dijet.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dijet.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dijet).Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptJJ" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dijet).Pt(),mTR,eventFakeWeight,systematicName);
    }
    ///                                                                                                                                                                          
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptl1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.Pt(),leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 L_dilepton.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton).Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 (L_dilepton).Pt(),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptl2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(0).lepton4V_.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(0).lepton4V_.Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(0).lepton4V_.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(0).lepton4V_.Pt(),mR,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(0).lepton4V_.Pt(),mTR,eventFakeWeight,systematicName);
    }

    ///                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptj1"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(1).lepton4V_.Pt(),RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(1).lepton4V_.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(1).lepton4V_.Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 leptonsIsoTight.at(1).lepton4V_.Pt(),mTR,eventFakeWeight,systematicName);
    }
    ///                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "ptj1" and VariableList2D.at(iVar).variableNameY == "ptj2"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 RecoJets.at(0).jet4V_.Pt(),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj1" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 RecoJets.at(0).jet4V_.Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptj1" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 RecoJets.at(0).jet4V_.Pt(),mTR,eventFakeWeight,systematicName);
    }
    ///                                                                                                                                                                        
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 RecoJets.at(1).jet4V_.Pt(),mR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 RecoJets.at(1).jet4V_.Pt(),mTR,eventFakeWeight,systematicName);
    }

    else if(VariableList2D.at(iVar).variableNameX == "mR" and VariableList2D.at(iVar).variableNameY == "mTR"){
      analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                 mR,mTR,eventFakeWeight,systematicName);
    }

  }
}


//// method in  order to apply cuts                                                                                                                                             
bool passCutContainerSelection (cutContainerWW & Cut,
				const string & sampleName,
				const int    & samplePosition,
				readTree* reader,
				vector<leptonContainer> & LeptonsAll,
				vector<leptonContainer> & leptonsIsoTight,
				vector<leptonContainer> & softMuons,
				vector<jetContainer> & RecoJets,
				trackJetEvent & trackEvent,
				const TLorentzVector & L_met,
				const float & minPtLeptonCut,
				const float & leptonIsoLooseCut,
				map<string,TH1F*> & vect,
				const string & finalStateString,
				const float & eventWeight){

  // identify loose leptons                                                                                                                                                     
  vector<leptonContainer> leptonsIsoLoose ;
  leptonsIsoLoose = dumpLeptons (LeptonsAll, leptonIsoLooseCut,minPtLeptonCut);

  // skip event with less than two leptons by default                                                                                                                           
  TString nameTemp;
  string Name;
  if(TString(sampleName).Contains("Madgraph_")){
    nameTemp = sampleName + "_pos_"+to_string(samplePosition) ;
    nameTemp.ReplaceAll("Madgraph_","");
    Name = string(nameTemp);
  }
  else
    Name = sampleName+ "_pos_"+to_string(samplePosition) ;


  int iBin = 1;

  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }

  }
  else iBin++;

  if(LeptonsAll.size() < 2 )  return false ; // skip the event --> only two reco leptons are good                                                                               
  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }
  }
  else iBin++;

  // identify tight leptons and require exactly nLep                                                                                                                            
  if (int(leptonsIsoTight.size()) != Cut.nLep ) return false;

  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }
  }
  else iBin++;


  if (leptonsIsoTight.at(0).lepton4V_.Pt() < Cut.ptL.first) return false;
  if (fabs(leptonsIsoTight.at(0).lepton4V_.Eta()) > Cut.etaMaxL) return false ;

  bool badTrailingLepton = false;
  for( size_t iLep = 1 ; iLep < leptonsIsoTight.size(); iLep++){
    if( leptonsIsoTight.at(iLep).lepton4V_.Pt() < Cut.ptL.second or fabs(leptonsIsoTight.at(iLep).lepton4V_.Eta()) > Cut.etaMaxL){
      badTrailingLepton = true;
    }
  }
  if(badTrailingLepton) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
    iBin++;
  }

  int extraLepton = 0; // count the extra lepton number                                                                                                                        
  for(size_t iLepton = 0; iLepton < leptonsIsoLoose.size() ; iLepton++){
    if(leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(0).lepton4V_ or leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(1).lepton4V_) continue; // skip tight ones                                                                                                                                                                       
  extraLepton++;
  }

  if(extraLepton > Cut.nextra ) return false; // extra lepton cut                                                                                                               

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"extra lepton");
    iBin++;
  }

  // apply Zmass veto: if nLep == 2 is a veto (WW analysis) if nLep == 3 is a tag (WZ analysis)                                                                                 
  bool isZMassVeto = false;
  for(size_t iLept = 0; iLept < LeptonsAll.size() ; iLept++){
    if(LeptonsAll.at(iLept).lepton4V_.Pt() < minPtLeptonCut) continue;
    for(size_t jLept = iLept+1; jLept < LeptonsAll.size() ; jLept++){
      if(LeptonsAll.at(jLept).lepton4V_.Pt() < minPtLeptonCut) continue;
      if(fabs(LeptonsAll.at(jLept).flavour_) != fabs(LeptonsAll.at(iLept).flavour_)) continue;
      if((LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() > Cut.MllZVeto.first and
         (LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() < Cut.MllZVeto.second and
         LeptonsAll.at(iLept).charge_*LeptonsAll.at(jLept).charge_ == -1)
        isZMassVeto = true;
    }
  }

  if(isZMassVeto and Cut.nLep == 2) return false;
  if(not isZMassVeto and Cut.nLep == 3) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z veto");
    iBin++;
  }

  // charge cut for same sign final state : zero means no cut, one means same sign, -1 opposite sign                                                                            
  if(Cut.chargeSign != 0){
    int sign = 1;
    for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns                                                                             
      sign *= leptonsIsoTight.at(leptSize).charge_;
    if(sign != Cut.chargeSign) return false;
  }

  if(TString(finalStateString).Contains("pp") and (leptonsIsoTight.at(0).charge_ < 0 or leptonsIsoTight.at(1).charge_ < 0))
    return false;

  if(TString(finalStateString).Contains("mm") and (leptonsIsoTight.at(0).charge_ > 0 or leptonsIsoTight.at(1).charge_ > 0))
    return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"same sign");
    iBin++;
  }

  // flavour selection                                                                                                                                                          
  if(Cut.flavour != 0){
    int flavour = 0;
    int sameflavour = 0;
    for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns                                                                             
      flavour += fabs(leptonsIsoTight.at(leptSize).flavour_) ;
    if(flavour/leptonsIsoTight.size() == 11 or flavour/leptonsIsoTight.size() == 13 ) sameflavour = 1;
    else sameflavour = -1;
    if(sameflavour != Cut.flavour) return false;
  }
  if(TString(finalStateString).Contains("UU")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
  }
  else if(TString(finalStateString).Contains("EE")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
  }
  else if(TString(finalStateString).Contains("EU")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
  }
  else if(TString(finalStateString).Contains("UE")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
  }

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
    iBin++;
  }

  // met cut                                                                                                                                                                    
  if(L_met.Pt() < Cut.MET) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"met selection");
    iBin++;
  }
  // jets cut                                                                                                                                                                   
  if(RecoJets.size() < 2 ) return false;

  if(RecoJets.at(0).jet4V_.Pt() < Cut.ptJet.first) return false;
  if(RecoJets.at(1).jet4V_.Pt() < Cut.ptJet.second) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
    iBin++;
  }

  // bveto                                                                                                                                                                      
  int nBjets = 0 ;
  for(size_t iJet = 0; iJet < RecoJets.size(); iJet++){
    if(fabs(RecoJets.at(iJet).jet4V_.Eta()) > 2.4) continue;
    if(RecoJets.at(iJet).btag_ >= Cut.bTagCut)
      nBjets ++ ;
  }

  if(nBjets > Cut.nBVeto) return false ;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"bveto");
    iBin++;
  }

  //soft muons                                                                                                                                                                  
  if(int(softMuons.size()) > Cut.nSoftMu) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"softMu");
    iBin++;
  }

  ///////// Lorentz Vector                                                                                                                                                      
  TLorentzVector L_dilepton;
  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;

  // apply Mll cut                                                                                                                                                              
  if(L_dilepton.M() < Cut.Mll.first or L_dilepton.M() > Cut.Mll.second) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
    iBin++;
  }

  TLorentzVector L_dijet;
  if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
  }

  // apply VBF cuts                                                                                                                                                             
  if(L_dijet.M() < Cut.Mjj) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
    iBin++;
  }

  if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < Cut.DetaJJ) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
    iBin++;
  }

  if(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()) > Cut.DetaLL) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaLL");
    iBin++;
  }

  // apply further cuts for fakes                                                                                                                                              
  float aveEta    = 0.5*(RecoJets.at(0).jet4V_.Eta()+RecoJets.at(1).jet4V_.Eta());
  float leadZep   = fabs((leptonsIsoTight.at(0).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta())));
  float trailZep  = fabs((leptonsIsoTight.at(1).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta())));


  if(L_dijet.DeltaR(L_dilepton) > Cut.dRlJ) return false;
  if(RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_) > Cut.dRlJ) return false;

  if((L_dilepton+L_dijet+L_met).Pt() > Cut.ptJJLLMet) return false;

  if(trackEvent.HTTrack_ > Cut.HTTrackjet) return false;
  if(leadZep  > Cut.lZep) return false ;
  if(trailZep > Cut.lZep) return false ;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"fake bkg reduction");
    iBin++;
  }

  // polarized cut                                                                                                                                                              
  if(Cut.polarization != 99 or TString(sampleName).Contains("Madgraph")){

    int polarizationFlag = 0;

    if(Cut.polarization != 99){
      if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
        polarizationFlag = 0 ; //LL                                                                                                                                             
      else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
        polarizationFlag = 1 ; //TL                                                                                                                                             
      else
        polarizationFlag = 2 ; //TT                                                                                                                                            

      if(polarizationFlag != Cut.polarization) return false;

      if(vect.size()!=0){
        vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
        vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
        iBin++;
      }
    }
    else if(TString(sampleName).Contains("Madgraph")){
      if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
        polarizationFlag = 0 ; //LL                                                                                                                                            
      else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
        polarizationFlag = 1 ; //TL                                                                                                                                            
      else
        polarizationFlag = 2 ; //TT                                                                                                                                            

      if(TString(sampleName).Contains("_LL")  and polarizationFlag != 0) return false;
      if(TString(sampleName).Contains("_TT")  and polarizationFlag != 2) return false;
      if((TString(sampleName).Contains("_TL") or TString(sampleName).Contains("_LT"))  and polarizationFlag != 1) return false;

      if(vect.size()!=0){
        vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
        vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
        iBin++;
      }
    }
  }
  else{
    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
      iBin++;
    }
  }
  return true;

}



bool passCutContainerSelection (readTree* reader,
				cutContainerWW & Cut,
				const string & sampleName,
				const int    & samplePosition,
				const bool   & usePuppiAsDefault,
				const float & minPtLeptonCut,
				const float & minPtLeptonCutCleaning,
				const float & leptonIsoCut_mu,
				const float & leptonIsoCut_el,
				const float & leptonIsoLooseCut,
				const float & matchingCone,
				const float & minJetCutPt,
				map<string,TH1F*> & vect,
				const string & finalStateString,
				const float & eventWeight){


  // dump all the lepton in the event                                                                                                                                          
  vector<leptonContainer> LeptonsAll;
  fillRecoLeptonsArray (LeptonsAll, *reader);

  // dump tight leptons                                                                                                                                                        
  vector<leptonContainer> leptonsIsoTight ;
  leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

  // identify loose leptons                                                                                                                                                    
  vector<leptonContainer> leptonsIsoLoose ;
  leptonsIsoLoose = dumpLeptons (LeptonsAll, leptonIsoLooseCut,minPtLeptonCut);

  // take reco jets                                                                                                                                                            
  vector<jetContainer> RecoJetsAll ;
  if(!usePuppiAsDefault)
    fillRecoJetArray (RecoJetsAll, *reader) ;
  else
    fillPuppiJetArray (RecoJetsAll, *reader) ;

  vector<leptonContainer> softMuons;
  softMuons = dumpSoftMuons(LeptonsAll,RecoJetsAll,leptonIsoCut_mu,minPtLeptonCut,softMuonPt,matchingCone);
  // skip event with less than two leptons by default                                                                                                                          
  TString nameTemp;
  string Name;

  if(TString(sampleName).Contains("Madgraph_")){
    nameTemp = sampleName + "_pos_"+to_string(samplePosition) ;
    nameTemp.ReplaceAll("Madgraph_","");
    Name = string(nameTemp);
  }
  else
    Name = sampleName + "_pos_"+to_string(samplePosition);

  int iBin = 1;

  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;
    }
  }
  else iBin++;

  if(LeptonsAll.size() < 2 )  return false ; // skip the event --> only two reco leptons are good                                                                               
  if(RecoJetsAll.size() < 2)  return false ; // skip the event with less than two reco jet                                                                                      

  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
      iBin++;
    }
  }
  else iBin++;

  // identify tight leptons and require exactly nLep                                                                                                                            
  if (int(leptonsIsoTight.size()) != Cut.nLep ) return false;

  if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("W3J") ){

    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;
    }
  }
  else iBin++;

  if (leptonsIsoTight.at(0).lepton4V_.Pt() < Cut.ptL.first) return false;
  if (fabs(leptonsIsoTight.at(0).lepton4V_.Eta()) > Cut.etaMaxL) return false ;

  bool badTrailingLepton = false;
  for( size_t iLep = 1 ; iLep < leptonsIsoTight.size(); iLep++){
    if( leptonsIsoTight.at(iLep).lepton4V_.Pt() < Cut.ptL.second or fabs(leptonsIsoTight.at(iLep).lepton4V_.Eta()) > Cut.etaMaxL){
      badTrailingLepton = true;
    }
  }
  if(badTrailingLepton) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
    iBin++;
  }

  int extraLepton = 0; // count the extra lepton number                                                                                                                         
  for(size_t iLepton = 0; iLepton < leptonsIsoLoose.size() ; iLepton++){
    if(leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(0).lepton4V_ or leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(1).lepton4V_) continue; // skip tight ones                                                                                                                                                                       
  extraLepton++;
  }

  if(extraLepton > Cut.nextra ) return false; // extra lepton cut                                                                                                               
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"extra lepton");
    iBin++;
  }

  // apply Zmass veto: if nLep == 2 is a veto (WW analysis) if nLep == 3 is a tag (WZ analysis)                                                                                 
  bool isZMassVeto = false;
  for(size_t iLept = 0; iLept < LeptonsAll.size() ; iLept++){
    if(LeptonsAll.at(iLept).lepton4V_.Pt() < minPtLeptonCut) continue;
    for(size_t jLept = iLept+1; jLept < LeptonsAll.size() ; jLept++){
      if(LeptonsAll.at(jLept).lepton4V_.Pt() < minPtLeptonCut) continue;
      if(fabs(LeptonsAll.at(jLept).flavour_) != fabs(LeptonsAll.at(iLept).flavour_)) continue;
      if((LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() > Cut.MllZVeto.first and
         (LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() < Cut.MllZVeto.second and
         LeptonsAll.at(iLept).charge_*LeptonsAll.at(jLept).charge_ == -1)
        isZMassVeto = true;
    }
  }

  if(isZMassVeto and Cut.nLep == 2) return false;
  if(not isZMassVeto and Cut.nLep == 3) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z veto");
    iBin++;
  }

  // charge cut for same sign final state : zero means no cut, one means same sign, -1 opposite sign                                                                            
  if(Cut.chargeSign != 0){
    int sign = 1;
    for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns                                                                             
      sign *= leptonsIsoTight.at(leptSize).charge_;
    if(sign != Cut.chargeSign) return false;
  }

  if(TString(finalStateString).Contains("pp") and (leptonsIsoTight.at(0).charge_ < 0 or leptonsIsoTight.at(1).charge_ < 0))
    return false;

  if(TString(finalStateString).Contains("mm") and (leptonsIsoTight.at(0).charge_ > 0 or leptonsIsoTight.at(1).charge_ > 0))
    return false;
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"same sign");
    iBin++;
  }

  // flavour selection                                                                                                                                                          
  if(Cut.flavour != 0){
    int flavour = 0;
    int sameflavour = 0;
    for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns                                                                              
      flavour += fabs(leptonsIsoTight.at(leptSize).flavour_) ;
    if(flavour/leptonsIsoTight.size() == 11 or flavour/leptonsIsoTight.size() == 13 ) sameflavour = 1;
    else sameflavour = -1;
    if(sameflavour != Cut.flavour) return false;
  }

  if(TString(finalStateString).Contains("UU")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
  }
  else if(TString(finalStateString).Contains("EE")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
  }
  else if(TString(finalStateString).Contains("EU")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
  }
  else if(TString(finalStateString).Contains("UE")){
    if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
    if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
  }

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
    iBin++;
  }

  // met cut                                                                                                                                                                    
  if(reader->pfmet_puppi < Cut.MET) return false;
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"met selection");
    iBin++;
  }


  // take jets                                                                                                                                                                  
  vector<jetContainer> RecoJets;
  RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 99., Cut.jetPUID, minPtLeptonCutCleaning, matchingCone);
  if(RecoJets.size() < 2 ) return false;

  // take track jets                                                                                                                                                            
  vector<jetContainer> trackJetsAll;
  fillTrackJetArray (trackJetsAll,*reader) ;
  vector<jetContainer> trackJets ;
  float dRThreshold = 0.5 ;
  trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
  trackJetEvent trackEvent;
  trackEvent = produceTrackJetEvent (trackJets,RecoJets);

  //jet pt cut                                                                                                                                                                  
  if(RecoJets.at(0).jet4V_.Pt() < Cut.ptJet.first) return false;
  if(RecoJets.at(1).jet4V_.Pt() < Cut.ptJet.second) return false;


  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
    iBin++;
  }


  // bveto                                                                                                                                                                      
  int nBjets = 0 ;
  for(size_t iJet = 0; iJet < RecoJets.size(); iJet++){
    if(fabs(RecoJets.at(iJet).jet4V_.Eta()) > 2.4) continue;
    if(RecoJets.at(iJet).btag_ >= Cut.bTagCut)
      nBjets ++ ;
  }

  if(nBjets > Cut.nBVeto) return false ;
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"bveto");
    iBin++;
  }


  //soft muons                                                                                                                                                                  
  if(int(softMuons.size()) > Cut.nSoftMu) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"softMu");
    iBin++;
  }
  

  ///////// Lorentz Vector                                                                                                                                                      
  TLorentzVector L_dilepton;
  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;

  // apply Mll cut                                                                                                                                                              
  if(L_dilepton.M() < Cut.Mll.first or L_dilepton.M() > Cut.Mll.second) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
    iBin++;
  }
  
  TLorentzVector L_dijet;
  if(RecoJets.size() >= 2){
  L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
  }
  
  // apply VBF cuts                                                                                                                                                             
  if(L_dijet.M() < Cut.Mjj) return false;
  
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
    iBin++;
  }
  if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < Cut.DetaJJ) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
    iBin++;
  }

  if(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()) > Cut.DetaLL) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaLL");
    iBin++;
  }
  
  // apply further cuts for fakes                                                                                                                                               
  float aveEta    = 0.5*(RecoJets.at(0).jet4V_.Eta()+RecoJets.at(1).jet4V_.Eta());
  float leadZep   = fabs((leptonsIsoTight.at(0).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta())));
  float trailZep  = fabs((leptonsIsoTight.at(1).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta())));
  

  if(L_dijet.DeltaR(L_dilepton) > Cut.dRlJ) return false;
  if(RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_) > Cut.dRlJ) return false;
  
  TLorentzVector L_met;
  L_met.SetPtEtaPhiM(reader->pfmet,0.,reader->pfmetphi,0.);
  if((L_dilepton+L_dijet+L_met).Pt() > Cut.ptJJLLMet) return false;
 
  if(trackEvent.HTTrack_ > Cut.HTTrackjet) return false;
  if(leadZep  > Cut.lZep) return false ;
  if(trailZep > Cut.lZep) return false ;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"fake bkg reduction");
    iBin++;
  }
  
  // polarized cut                                                                                                                                                             
  if(Cut.polarization != 99 or TString(sampleName).Contains("Madgraph")){
    
    int polarizationFlag = 0;
    
    if(Cut.polarization != 99){
      
      if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
	polarizationFlag = 0 ; //LL                                                                                                                                            
      else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
	polarizationFlag = 1 ; //TL                                                                                                                                             
      else
	polarizationFlag = 2 ; //TT                                                                                                                                             
    
      if(polarizationFlag != Cut.polarization) return false;

      if(vect.size()!=0){
	vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
	vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	iBin++;
      }
    }
    else if(TString(sampleName).Contains("Madgraph")){
      if(fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) == 0)
	polarizationFlag = 0 ; //LL                                                                                                                                            
      else if((fabs(reader->vbosonLHEspin1) == 1 and fabs(reader->vbosonLHEspin2) ==0) or (fabs(reader->vbosonLHEspin1) == 0 and fabs(reader->vbosonLHEspin2) ==1))
	polarizationFlag = 1 ; //TL                                                                                                                                            
      else
	polarizationFlag = 2 ; //TT                                                                                                                                             
      
      if(TString(sampleName).Contains("_LL")  and polarizationFlag != 0) return false;
      if(TString(sampleName).Contains("_TL")  and polarizationFlag != 1) return false;
      if(TString(sampleName).Contains("_TT")  and polarizationFlag != 2) return false;
      
      if(vect.size()!=0){
	vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
	vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	iBin++;
      }
    }
  }
  else{
    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
      iBin++;
    }
  }
  
  return true;
  
}

