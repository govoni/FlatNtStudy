#include "fakeBackgroundUtils.h"

float getElectronMisChargeProbability (const float & pt, const float & eta){

  if(fabs(eta) < 1.5){ //Barrel                                                                                                                                                 
    if(pt < 20) return 3E-4 ;
    else if(pt > 20 and pt < 50) return 2E-4;
    else if(pt > 50) return 2E-4;
  }
  else if(fabs(eta) > 1.5){ //endcap                                                                                                                                           
    if(pt < 20) return 6E-4;
    else if(pt > 20 and pt < 50) return 1E-3;
    else if(pt > 50) return 2E-3;
  }

  return 0;
}

vector<jetContainer> dumpJetsForFake (vector<jetContainer> & RecoJets,
				      vector<jetContainer> & GenJets,
				      vector<leptonContainer> & leptonsIsoTight,
				      const float & minJetCutPt,
				      const float & btagCut,
				      const float & jetPUID,
				      const float & minPtLeptonCutCleaning,
				      const float & matchingCone,
				      const float & etaMaxL){
  vector<jetContainer> jetForFakes ;
  
  for(size_t iJet = 0; iJet < RecoJets.size(); iJet++){

    if(RecoJets.at(iJet).jet4V_.Pt() < minJetCutPt) continue;
    if(RecoJets.at(iJet).jetPUID_    < jetPUID) continue;
    if(RecoJets.at(iJet).btag_       > btagCut) continue;
    if(fabs(RecoJets.at(iJet).jet4V_.Eta()) > etaMaxL ) continue;

    bool isGoodJet = true;

    for(size_t iLep = 0; iLep < leptonsIsoTight.size(); iLep++){
      if(leptonsIsoTight.at(iLep).lepton4V_.Pt() < minPtLeptonCutCleaning) continue ;
      if(leptonsIsoTight.at(iLep).lepton4V_.DeltaR(RecoJets.at(iJet).jet4V_) > matchingCone) continue ;
      isGoodJet = false ;
    }

    if(isGoodJet){
      for(size_t iGen = 0; iGen < GenJets.size(); iGen++){
	if(RecoJets.at(iJet).jet4V_.DeltaR(GenJets.at(iGen).jet4V_) < matchingCone){ 
	  jetForFakes.push_back(RecoJets.at(iJet));
	  break;
	}
      }
    }
  }

  return jetForFakes;


}


// ---------------------                                                                                                                                                        
float getFakeWeight(  jetContainer inputJet,
                      fakeRateContainer & fakeRate,
                      string leptonFlavour,
                      vector<jetContainer> & jetCollection){

  float weight = 1. ;


  if(leptonFlavour == "U"){    
    weight *= fakeRate.getFakeRate(int(13),inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()) ;
    
    for(size_t iJet = 0; iJet < jetCollection.size(); iJet++){
      if(inputJet.jet4V_ == jetCollection.at(iJet).jet4V_)
        continue ;
      weight *= (1-fakeRate.getFakeRate(int(13),jetCollection.at(iJet).jetflavour_,jetCollection.at(iJet).jet4V_.Pt(),jetCollection.at(iJet).jet4V_.Eta()));
    }
  }
  else {
    weight *= fakeRate.getFakeRate(int(11),inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()) ;
    for(size_t iJet = 0; iJet < jetCollection.size(); iJet++){
      if(inputJet.jet4V_ == jetCollection.at(iJet).jet4V_)
        continue ;
      weight *= (1-fakeRate.getFakeRate(int(11),jetCollection.at(iJet).jetflavour_,jetCollection.at(iJet).jet4V_.Pt(),jetCollection.at(iJet).jet4V_.Eta()));
    }
  }

  return weight;
}

// ---------------------                                                                                                                                                        
leptonContainer createFakeLepton(  jetContainer inputJet,
                                   leptonContainer inputLepton,
                                   fakeMigrationContainer & fakeMigration,
                                   string scenarioString){

  TLorentzVector lepton4V ;

  if(TString(scenarioString).Contains("UU") and fabs(inputLepton.flavour_) == 13){ // if UU final state and input lepton is a muon

    if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0) // check if pt is well defined
      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),
                            0.);
    else
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    return leptonContainer(lepton4V,inputLepton.charge_,inputLepton.flavour_,0.); // return lepton with same flavour and charge

  }

  else if(TString(scenarioString).Contains("EE") and fabs(inputLepton.flavour_) == 11){

    if(fabs(inputJet.jet4V_.Eta()) > 1.45 and fabs(inputJet.jet4V_.Eta()) < 1.55)  // remove endcap barrel gap
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    else if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0)
      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),
			    0.);
    else
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    return leptonContainer(lepton4V,inputLepton.charge_,inputLepton.flavour_,0.);
  }

  else if(TString(scenarioString).Contains("UE") and fabs(inputLepton.flavour_) == 13){

    if(fabs(inputJet.jet4V_.Eta()) > 1.45 and fabs(inputJet.jet4V_.Eta()) < 1.55) // create an electron removing the gap
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    else if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0)
      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(11,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),
                            0.);
    else
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    if(inputLepton.flavour_ > 0)
      return leptonContainer(lepton4V,inputLepton.charge_,11,0.);

    else
      return leptonContainer(lepton4V,inputLepton.charge_,-11,0.);
  }

  else if(TString(scenarioString).Contains("UE") and fabs(inputLepton.flavour_) == 11){

    if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0)
      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(13,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),
                            0.);

    else
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    if(inputLepton.flavour_ > 0)
      return leptonContainer(lepton4V,inputLepton.charge_,13,0.);
    else
      return leptonContainer(lepton4V,inputLepton.charge_,-13,0.);
  }

  else if(TString(scenarioString).Contains("EU") and fabs(inputLepton.flavour_) == 11){
    if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0)

      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(13,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),0.);
    else
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    if(inputLepton.flavour_ > 0)
      return leptonContainer(lepton4V,inputLepton.charge_,13,0.);
    else
      return leptonContainer(lepton4V,inputLepton.charge_,-13,0.);
  }

  else if(TString(scenarioString).Contains("EU") and fabs(inputLepton.flavour_) == 13){

    if(fabs(inputJet.jet4V_.Eta()) > 1.45 and fabs(inputJet.jet4V_.Eta()) < 1.55) 
      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    else if( (inputJet.jet4V_.Pt()-fakeMigration.getMigration(inputLepton.flavour_,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta())) > 0)

      lepton4V.SetPtEtaPhiM(inputJet.jet4V_.Pt()-fakeMigration.getMigration(11,inputJet.jetflavour_,inputJet.jet4V_.Pt(),inputJet.jet4V_.Eta()),
                            inputJet.jet4V_.Eta(),
                            inputJet.jet4V_.Phi(),0.);
    else

      lepton4V.SetPtEtaPhiM(0.,inputJet.jet4V_.Eta(),inputJet.jet4V_.Phi(),0.);

    if(inputLepton.flavour_ > 0)
      return leptonContainer(lepton4V,inputLepton.charge_,11,0.);
    else
      return leptonContainer(lepton4V,inputLepton.charge_,-11,0.);

  }

  else leptonContainer();

  return leptonContainer();
}


// ---------------------                                                                                                                                                      
void makeFakeLeptonBackground(const string & sampleName,
			      const int    & samplePosition,
			      const string & finalStateString,
			      plotter & analysisPlots,
			      readTree *reader,
                              cutContainer & cutElement,
			      vector<variableContainer> & VariableList,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
			      vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
			      const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              fakeRateContainer & fakeRate,
                              fakeMigrationContainer & fakeMigration,
			      map <string,TH1F*> & vect
                              ){

  float eventFakeWeight = 1. ;

  if(leptonsIsoTight.size() < 1 or leptonsIsoTight.size() >= 2 ) return ; // if less than one isolated lepton over the minimum pt                                        

  // take the fake weight from the cleaned jet collection over threshold                                                                                                    
  if(TString(finalStateString).Contains("UU") and fabs(leptonsIsoTight.at(0).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("EE") and fabs(leptonsIsoTight.at(0).flavour_) != 11)
    return ;

  // tke the reco jets, check that they are not overlapping with isolated leptons and over threshold + PUID + btagging + etaCut + matched with a GenJet
  vector<jetContainer> RecoJetsForFake;
  RecoJetsForFake  = dumpJetsForFake (RecoJets, 
				      GenJets,
				      leptonsIsoTight, 
				      minJetCutPt, 
				      999, 
				      cutElement.jetPUID, 
				      minPtLeptonCutCleaning, 
				      matchingCone,
				      cutElement.etaMaxL);

  

  // fake rate                                                                                                                                                             
  for (size_t iJet = 0; iJet < RecoJetsForFake.size(); iJet++){

    eventFakeWeight = 1.;

    if(TString(finalStateString).Contains("UU")){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake); // get the fake rate from interpolation
    }
    else if(TString(finalStateString).Contains("EE")){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) == 11){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) == 13){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) == 13){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) == 11){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake);
    }
    else {
      cerr<<" problem with fake rate evaluation --> catogry problem --> fix it "<<endl;
      continue ;
    }

    // promote the tighter jets as lepton trough the migration matrix ... add also to the whole lepton collection for systematics check                                    
    vector<leptonContainer> fakeLeptonsAll;
    vector<leptonContainer> fakeLeptonsIsoTight;
    fakeLeptonsIsoTight = leptonsIsoTight ;
    fakeLeptonsAll      = LeptonsAll;
    leptonContainer fakeLepton ;
    fakeLepton = createFakeLepton(RecoJetsForFake.at(iJet),leptonsIsoTight.at(0),fakeMigration,finalStateString);

    fakeLeptonsIsoTight.push_back(fakeLepton);
    fakeLeptonsAll.push_back(fakeLepton);

    sort(fakeLeptonsAll.rbegin(),fakeLeptonsAll.rend());
    sort(fakeLeptonsIsoTight.rbegin(),fakeLeptonsIsoTight.rend());

    //re-clean jets for this new lepton                                                                               
    vector<jetContainer> fakeRecoJets;
    fakeRecoJets  = dumpJets (RecoJets, fakeLeptonsIsoTight, minJetCutPt, 999, cutElement.jetPUID, minPtLeptonCutCleaning, matchingCone);

    // take into account the charge assignment                                                                                                                             
    eventFakeWeight = eventFakeWeight*0.5;

    TLorentzVector fakeL_met ;
    fakeL_met  = L_met - fakeLepton.lepton4V_ + RecoJetsForFake.at(iJet).jet4V_;

    // track jets using the new leptons for cleaning                                     
    float dRThreshold = 0.5;
    vector<jetContainer> trackJets;
    trackJets = dumpTrackJets (trackJetsAll,fakeLeptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
    trackJetEvent trackEvent;
    trackEvent = produceTrackJetEvent (trackJets,RecoJets);

    // analysis with nominal objects                                                                                                                                       
    if( passCutContainerSelection (cutElement,
				   sampleName,
				   samplePosition,
				   reader,
				   fakeLeptonsAll,
				   fakeLeptonsIsoTight,
				   softMuons,
				   fakeRecoJets,
				   trackEvent,
				   fakeL_met,
				   minPtLeptonCutCleaning,
				   leptonIsoLooseCut,
				   vect,
				   finalStateString)){

      fillHisto(analysisPlots, 
		sampleName, 
		samplePosition,
		cutElement.cutLayerName, 
		VariableList,
		fakeLeptonsIsoTight, 
		softMuons, 
		fakeRecoJets, 
		GenJets, 
		trackEvent, 
		fakeL_met, 
		"",
		eventFakeWeight);
    }
  }
}



// ---------------------                                                                                                                                                        
void makeFakeLeptonBackground(const string & sampleName,
			      const int    & samplePosition,
			      const string & finalStateString,
			      plotter & analysisPlots,
			      readTree *reader,
                              cutContainer & cutElement,
			      vector<variableContainer> & VariableList,
			      vector<variableContainer2D> & VariableList2D,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
			      vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
			      const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              fakeRateContainer & fakeRate,
                              fakeMigrationContainer & fakeMigration,
			      map <string,TH1F*> & vect
                              ){

  float eventFakeWeight = 1. ;

  if(leptonsIsoTight.size() < 1 or leptonsIsoTight.size() >= 2 ) return ; // if less than one isolated lepton over the minimum pt                                        

  // take the fake weigh from the cleaned jet collection over threshold                                                                                                    
  if(TString(finalStateString).Contains("UU") and fabs(leptonsIsoTight.at(0).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("EE") and fabs(leptonsIsoTight.at(0).flavour_) != 11)
    return ;

  vector<jetContainer> RecoJetsForFake;
  RecoJetsForFake  = dumpJetsForFake (RecoJets,
                                      GenJets,
                                      leptonsIsoTight,
                                      minJetCutPt,
                                      999,
                                      cutElement.jetPUID,
                                      minPtLeptonCutCleaning,
                                      matchingCone,
                                      cutElement.etaMaxL);

  // fake rate                                                                                                                                                             
  for (size_t iJet = 0; iJet < RecoJetsForFake.size(); iJet++){

    eventFakeWeight = 1.;


    if(TString(finalStateString).Contains("UU")){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("EE")){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) == 11){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) == 13){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) == 13){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"E",RecoJetsForFake);
    }
    else if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) == 11){
      eventFakeWeight = getFakeWeight(RecoJetsForFake.at(iJet),fakeRate,"U",RecoJetsForFake);
    }
    else {
      cerr<<" problem with fake rate evaluation --> catogry problem --> fix it "<<endl;
      continue ;
    }

    // promote the tighter jets as lepton trough the migration matrix ... add also to the whole lepton collection for systematics check                                    
    vector<leptonContainer> fakeLeptonsAll;
    vector<leptonContainer> fakeLeptonsIsoTight;
    fakeLeptonsIsoTight = leptonsIsoTight ;
    fakeLeptonsAll = LeptonsAll;
    leptonContainer fakeLepton ;
    fakeLepton = createFakeLepton(RecoJetsForFake.at(iJet),leptonsIsoTight.at(0),fakeMigration,finalStateString);

    fakeLeptonsIsoTight.push_back(fakeLepton);
    fakeLeptonsAll.push_back(fakeLepton);

    sort(fakeLeptonsAll.rbegin(),fakeLeptonsAll.rend());
    sort(fakeLeptonsIsoTight.rbegin(),fakeLeptonsIsoTight.rend());

    //re-clean jets for this new lepton                                                                                                                                    
    vector<jetContainer> fakeRecoJets;
    fakeRecoJets  = dumpJets (RecoJets, fakeLeptonsIsoTight, minJetCutPt, 999, cutElement.jetPUID, minPtLeptonCutCleaning, matchingCone);

    // take into account the charge assignment                                                                                                                             
    eventFakeWeight = eventFakeWeight*0.5;

    TLorentzVector fakeL_met ;
    fakeL_met  = L_met - fakeLepton.lepton4V_ + RecoJetsForFake.at(iJet).jet4V_;

    // track jets using the new leptons for cleaning                                     
    float dRThreshold = 0.5;
    vector<jetContainer> trackJets;
    trackJets = dumpTrackJets (trackJetsAll,fakeLeptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
    trackJetEvent trackEvent;
    trackEvent = produceTrackJetEvent (trackJets,RecoJets);

    // analysis with nominal objects                                                                                                                                       
    if( passCutContainerSelection (cutElement,
				   sampleName,
				   samplePosition,
				   reader,
				   fakeLeptonsAll,
				   fakeLeptonsIsoTight,
				   softMuons,
				   fakeRecoJets,
				   trackEvent,
				   fakeL_met,
				   minPtLeptonCutCleaning,
				   leptonIsoLooseCut,
				   vect,
				   finalStateString)){

      fillHisto(analysisPlots, 
		sampleName, 
		samplePosition,
		cutElement.cutLayerName, 
		VariableList,
		fakeLeptonsIsoTight, 
		softMuons, 
		fakeRecoJets, 
		GenJets, 
		trackEvent, 
		fakeL_met, 
		"",
		eventFakeWeight);

    fillHisto2D(analysisPlots, 
		sampleName, 
		samplePosition,
		cutElement.cutLayerName, 
		VariableList2D,
		fakeLeptonsIsoTight, 
		softMuons, 
		fakeRecoJets, 
		GenJets, 
		trackEvent, 
		fakeL_met, 
		"",
		eventFakeWeight);
    }
  }
}


// -----------------------------------
void makeFakeChargeBackground(const string & sampleName,
			      const int    & samplePosition,
                              const string & finalStateString,
                              plotter & analysisPlots,
                              readTree *reader,
                              cutContainer & cutElement,
                              vector<variableContainer> & VariableList,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              map <string,TH1F*> & vect){


  float eventFakeWeight = 1. ;

  if(leptonsIsoTight.size() != 2) return ; // if less than one isolated lepton over the minimum pt                                                                       

  // take the fake weigh from the cleaned jet collection over threshold                                                                                                    
  if(TString(finalStateString).Contains("UU") and fabs(leptonsIsoTight.at(0).flavour_) != 13 and fabs(leptonsIsoTight.at(1).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("EE") and fabs(leptonsIsoTight.at(0).flavour_) != 11 and fabs(leptonsIsoTight.at(1).flavour_) != 11)
    return ;
  if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) != 11 and fabs(leptonsIsoTight.at(1).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) != 13 and fabs(leptonsIsoTight.at(1).flavour_) != 11)
    return ;

  if(leptonsIsoTight.at(0).charge_ == leptonsIsoTight.at(1).charge_) {

    // track jets using the new leptons for cleaning                                                                                                                       
    float dRThreshold = 0.5;
    vector<jetContainer> trackJets;
    trackJets = dumpTrackJets (trackJetsAll,leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
    trackJetEvent trackEvent;
    trackEvent = produceTrackJetEvent (trackJets,RecoJets);

    // analysis with nominal objects                                                                                                                                       
    if( passCutContainerSelection (cutElement,
				   sampleName,
				   samplePosition,
				   reader,
				   LeptonsAll,
				   leptonsIsoTight,
				   softMuons,
				   RecoJets,
				   trackEvent,
				   L_met,
				   minPtLeptonCutCleaning,
				   leptonIsoLooseCut,
				   vect,
				   finalStateString)){

      fillHisto(analysisPlots, 
		sampleName, 
		samplePosition,
		cutElement.cutLayerName, 
		VariableList,
		leptonsIsoTight, 
		softMuons, 
		RecoJets, 
		GenJets, 
		trackEvent, 
		L_met, 
		"",
		eventFakeWeight);
    }
  }

  else {


    for (size_t iLep = 0; iLep < leptonsIsoTight.size(); iLep++){

      eventFakeWeight = 1.;

      if(fabs(leptonsIsoTight.at(iLep).flavour_) == 13) continue;
      
      // mischarge ID                                                                                                                                                      
      vector<leptonContainer> newLeptonsAll;
      vector<leptonContainer> newLeptonsIsoTight;
      
      if(fabs(leptonsIsoTight.at(iLep).flavour_) == 11 and iLep == 0){

	eventFakeWeight = getElectronMisChargeProbability(leptonsIsoTight.at(iLep).lepton4V_.Pt(),leptonsIsoTight.at(iLep).lepton4V_.Eta());

	leptonContainer misIDLepton (leptonsIsoTight.at(iLep).lepton4V_,leptonsIsoTight.at(iLep+1).charge_,leptonsIsoTight.at(iLep).flavour_,leptonsIsoTight.at(iLep).iso_) ;
	newLeptonsIsoTight.push_back(misIDLepton);
	newLeptonsIsoTight.push_back(leptonsIsoTight.at(iLep+1));

	newLeptonsAll = newLeptonsIsoTight ;

      }
      else if(fabs(leptonsIsoTight.at(iLep).flavour_) == 11 and iLep == 1){

	eventFakeWeight = getElectronMisChargeProbability(leptonsIsoTight.at(iLep).lepton4V_.Pt(),leptonsIsoTight.at(iLep).lepton4V_.Eta());

	leptonContainer misIDLepton (leptonsIsoTight.at(iLep).lepton4V_,leptonsIsoTight.at(iLep-1).charge_,leptonsIsoTight.at(iLep).flavour_,leptonsIsoTight.at(iLep).iso_) ;
	newLeptonsIsoTight.push_back(leptonsIsoTight.at(iLep-1));
	newLeptonsIsoTight.push_back(misIDLepton);

	newLeptonsAll = newLeptonsIsoTight ;

      }

      else {
	cerr<<" problem with mis charge ID evaluation --> catogry problem --> fix it "<<endl;
	continue ;
      }

      for(size_t iLepton = 0 ; iLepton < LeptonsAll.size(); iLepton++){
	for(size_t jLepton = 0 ; jLepton <  newLeptonsAll.size(); jLepton++){
	  if(LeptonsAll.at(iLepton).lepton4V_ == newLeptonsAll.at(jLepton).lepton4V_)
	    continue;
	  newLeptonsAll.push_back(LeptonsAll.at(iLepton));
	}
      }

      // track jets using the new leptons for cleaning                                                                                                                     
      float dRThreshold = 0.5;
      vector<jetContainer> trackJets;
      trackJets = dumpTrackJets (trackJetsAll,newLeptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
      trackJetEvent trackEvent;
      trackEvent = produceTrackJetEvent (trackJets,RecoJets);

      // analysis with nominal objects                                                                                                                                     
      if( passCutContainerSelection (cutElement,
				     sampleName,
				     samplePosition,
				     reader,
				     newLeptonsAll,
				     newLeptonsIsoTight,
				     softMuons,
				     RecoJets,
				     trackEvent,
				     L_met,
				     minPtLeptonCutCleaning,
				     leptonIsoLooseCut,
				     vect,
				     finalStateString)){

	fillHisto(analysisPlots, 
		  sampleName, 
		  samplePosition,
		  cutElement.cutLayerName, 
		  VariableList,
		  newLeptonsIsoTight, 
		  softMuons, 
		  RecoJets, 
		  GenJets, 
		  trackEvent, 
		  L_met, 
		  "",
		  eventFakeWeight);
      }
    }
  }
}



// -----------------------------------
void makeFakeChargeBackground(const string & sampleName,
			      const int    & samplePosition,
                              const string & finalStateString,
                              plotter & analysisPlots,
                              readTree *reader,
                              cutContainer & cutElement,
                              vector<variableContainer> & VariableList,
                              vector<variableContainer2D> & VariableList2D,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              map <string,TH1F*> & vect){

  float eventFakeWeight = 1.;
      

  if(leptonsIsoTight.size() != 2) return ; // if less than one isolated lepton over the minimum pt                                                                       

  // take the fake weigh from the cleaned jet collection over threshold                                                                                                    
  if(TString(finalStateString).Contains("UU") and fabs(leptonsIsoTight.at(0).flavour_) != 13 and fabs(leptonsIsoTight.at(1).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("EE") and fabs(leptonsIsoTight.at(0).flavour_) != 11 and fabs(leptonsIsoTight.at(1).flavour_) != 11)
    return ;
  if(TString(finalStateString).Contains("EU") and fabs(leptonsIsoTight.at(0).flavour_) != 11 and fabs(leptonsIsoTight.at(1).flavour_) != 13)
    return ;
  if(TString(finalStateString).Contains("UE") and fabs(leptonsIsoTight.at(0).flavour_) != 13 and fabs(leptonsIsoTight.at(1).flavour_) != 11)
    return ;

  if(leptonsIsoTight.at(0).charge_ == leptonsIsoTight.at(1).charge_) {

    // track jets using the new leptons for cleaning                                                                                                                       
    float dRThreshold = 0.5;
    vector<jetContainer> trackJets;
    trackJets = dumpTrackJets (trackJetsAll,leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
    trackJetEvent trackEvent;
    trackEvent = produceTrackJetEvent (trackJets,RecoJets);

    // analysis with nominal objects                                                                                                                                       
    if( passCutContainerSelection (cutElement,
				   sampleName,
				   samplePosition,
				   reader,
				   LeptonsAll,
				   leptonsIsoTight,
				   softMuons,
				   RecoJets,
				   trackEvent,
				   L_met,
				   minPtLeptonCutCleaning,
				   leptonIsoLooseCut,
				   vect,
				   finalStateString)){

      fillHisto(analysisPlots, 
		sampleName, 
		samplePosition,
		cutElement.cutLayerName, 
		VariableList,
		leptonsIsoTight, 
		softMuons, 
		RecoJets, 
		GenJets, 
		trackEvent, 
		L_met, 
		"",
		eventFakeWeight);

      fillHisto2D(analysisPlots, 
		  sampleName, 
		  samplePosition,
		  cutElement.cutLayerName, 
		  VariableList2D,
		  leptonsIsoTight, 
		  softMuons, 
		  RecoJets, 
		  GenJets, 
		  trackEvent, 
		  L_met, 
		  "",
		  eventFakeWeight);
    }
  }

  else {


    for (size_t iLep = 0; iLep < leptonsIsoTight.size(); iLep++){

      eventFakeWeight = 1.;

      if(fabs(leptonsIsoTight.at(iLep).flavour_) == 13) continue;
      
      // mischarge ID                                                                                                                                                      
      vector<leptonContainer> newLeptonsAll;
      vector<leptonContainer> newLeptonsIsoTight;
      
      if(fabs(leptonsIsoTight.at(iLep).flavour_) == 11 and iLep == 0){

	eventFakeWeight = getElectronMisChargeProbability(leptonsIsoTight.at(iLep).lepton4V_.Pt(),leptonsIsoTight.at(iLep).lepton4V_.Eta());

	leptonContainer misIDLepton (leptonsIsoTight.at(iLep).lepton4V_,leptonsIsoTight.at(iLep+1).charge_,leptonsIsoTight.at(iLep).flavour_,leptonsIsoTight.at(iLep).iso_) ;
	newLeptonsIsoTight.push_back(misIDLepton);
	newLeptonsIsoTight.push_back(leptonsIsoTight.at(iLep+1));

	newLeptonsAll = newLeptonsIsoTight ;

      }
      else if(fabs(leptonsIsoTight.at(iLep).flavour_) == 11 and iLep == 1){

	eventFakeWeight = getElectronMisChargeProbability(leptonsIsoTight.at(iLep).lepton4V_.Pt(),leptonsIsoTight.at(iLep).lepton4V_.Eta());

	leptonContainer misIDLepton (leptonsIsoTight.at(iLep).lepton4V_,leptonsIsoTight.at(iLep-1).charge_,leptonsIsoTight.at(iLep).flavour_,leptonsIsoTight.at(iLep).iso_) ;
	newLeptonsIsoTight.push_back(leptonsIsoTight.at(iLep-1));
	newLeptonsIsoTight.push_back(misIDLepton);

	newLeptonsAll = newLeptonsIsoTight ;

      }

      else {
	cerr<<" problem with mis charge ID evaluation --> catogry problem --> fix it "<<endl;
	continue ;
      }

      for(size_t iLepton = 0 ; iLepton < LeptonsAll.size(); iLepton++){
	for(size_t jLepton = 0 ; jLepton <  newLeptonsAll.size(); jLepton++){
	  if(LeptonsAll.at(iLepton).lepton4V_ == newLeptonsAll.at(jLepton).lepton4V_)
	    continue;
	  newLeptonsAll.push_back(LeptonsAll.at(iLepton));
	}
      }

      // track jets using the new leptons for cleaning                                                                                                                     
      float dRThreshold = 0.5;
      vector<jetContainer> trackJets;
      trackJets = dumpTrackJets (trackJetsAll,newLeptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
      trackJetEvent trackEvent;
      trackEvent = produceTrackJetEvent (trackJets,RecoJets);

      // analysis with nominal objects                                                                                                                                     
      if( passCutContainerSelection (cutElement,
				     sampleName,
				     samplePosition,
				     reader,
				     newLeptonsAll,
				     newLeptonsIsoTight,
				     softMuons,
				     RecoJets,
				     trackEvent,
				     L_met,
				     minPtLeptonCutCleaning,
				     leptonIsoLooseCut,
				     vect,
				     finalStateString)){

	fillHisto(analysisPlots, 
		  sampleName, 
		  samplePosition,
		  cutElement.cutLayerName, 
		  VariableList,
		  newLeptonsIsoTight, 
		  softMuons, 
		  RecoJets, 
		  GenJets, 
		  trackEvent, 
		  L_met, 
		  "",
		  eventFakeWeight);

	fillHisto2D(analysisPlots, 
		    sampleName, 
		    samplePosition,
		    cutElement.cutLayerName, 
		    VariableList2D,
		    newLeptonsIsoTight, 
		    softMuons, 
		    RecoJets, 
		    GenJets, 
		    trackEvent, 
		    L_met, 
		    "",
		    eventFakeWeight);
      }
    }
  }
}


