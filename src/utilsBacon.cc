#include "utilsBacon.h"

// --------------------------------                                                                                                                                           
bool passEleID(TElectron* ele, float rho){

  if(fabs(ele->eta) < 1.5){
    if(fabs(ele->dz) < 0.015 and
       fabs(ele->d0) < 0.003 and
       ele->nMissingHits <= 1 and
       ele->isConv == 0 and
       (ele->chHadIso03 + max(ele->gammaIso03+ele->neuHadIso03-rho*0.3*0.3*TMath::Pi(),0.0))/ele->pt < 0.55 ) return true;
  }
  else {
    if(fabs(ele->dz) < 0.15 and
       fabs(ele->d0) < 0.03 and
       ele->nMissingHits <= 1 and
       ele->isConv == 0 and
       (ele->chHadIso03 + max(ele->gammaIso03+ele->neuHadIso03-rho*0.3*0.3*TMath::Pi(),0.0))/ele->pt < 0.60 ) return true;
  }

  return false;

}


// --------------------------------                                                                                                                                            
bool passMuonID(TMuon* mu, float rho){

  return (
	  ((mu->typeBits)/2)%2 and
	  ((mu->typeBits)/32)%2 and
           mu->tkNchi2 < 10 and
           mu->nValidHits > 0 and
           mu->nMatchStn > 1 and
	  fabs(mu->d0) < 0.2 and
	  fabs(mu->dz) < 0.5 and
           mu->nPixHits > 0 and
           mu->nTkLayers > 5 and
	  ((mu->chHadIso03 + max(mu->gammaIso03+mu->neuHadIso03-rho*0.3*0.3*TMath::Pi(),0.0))/mu->pt) < 0.55);

}

// --------------------------------                                                                                                                                              
void findGenLeptonsFromW(TClonesArray* genParticles, vector<int> & eleIndex, vector<int> & muIndex){

  int parentId = 24; // for W                                                                                                                                                    

  for( int iGen = 0; iGen < genParticles->GetEntriesFast() ; ++iGen ) {
    TGenParticle *genP = (TGenParticle*)((*genParticles)[iGen]);
    if( genP->status == 1 && genP->parent!=-1){
      if( abs(genP->pdgId) == 11 ) {
        bool foundW = false, stuck = false;
        do {
          if(genP->parent!=-1) {
            TGenParticle *genTemp = (TGenParticle*)((*genParticles)[genP->parent]);
            genP = genTemp;
          }
          else stuck = true;
          if( abs(genP->pdgId) == parentId) {
            foundW = true;
          }
        } while( !stuck && !foundW );
        if( foundW ) {
          eleIndex.push_back(iGen);
        }
      }
      if( abs(genP->pdgId) == 13 ) {
        bool foundW = false, stuck = false;
        do {
          if(genP->parent!=-1) {
            TGenParticle *genTemp = (TGenParticle*)((*genParticles)[genP->parent]);
            genP = genTemp;
          }
          else stuck = true;
          if( abs(genP->pdgId) == parentId) {
            foundW = true;
          }
        } while( !stuck && !foundW );
        if( foundW ) {
          muIndex.push_back(iGen);
        }
      }
    }
  }
  }


void findGenNeutrinoFromW(TClonesArray* genParticles,
                          vector<TGenParticle> & genNeutrino){

  int parentId = 24;

  for( int iGen = 0; iGen < genParticles->GetEntriesFast() ; ++iGen ) {
    TGenParticle *genP = (TGenParticle*)((*genParticles)[iGen]);
    if( genP->parent!=-1 and genP->status ==1){
      if( abs(genP->pdgId) == 12 or abs(genP->pdgId) == 14 or abs(genP->pdgId) == 16 ) {
        bool foundW = false, stuck = false;
	TGenParticle *genP2 = (TGenParticle*)((*genParticles)[iGen]);
        do {
          if(genP2->parent!=-1) {
            TGenParticle *genTemp = (TGenParticle*)((*genParticles)[genP2->parent]);
            genP2 = genTemp;
          }
          else stuck = true;
          if( abs(genP2->pdgId) == parentId) {
            foundW = true;
          }
        } while( !stuck && !foundW );
        if( foundW ) {
          genNeutrino.push_back(*genP);
        }
      }
    }
  }

}


// ----------------------------------                                                                                                                                         
void cleanedJetsFromLeptons(// look for jets cleaning leptons                                                                                                                  
                            vector<TJet> & cleanedJets,
                            TClonesArray & inputJets,
                            vector<TMuon> & goodTightMuons,
                            vector<TElectron> & goodTightElectrons, float minJetCutPt, float matchingCone){


  for( int iJet = 0; iJet < inputJets.GetEntriesFast(); iJet++){

    TJet* jet = (TJet*)(inputJets.At(iJet));
    if(jet->pt < minJetCutPt or fabs(jet->eta) > 4.7) continue;
    TLorentzVector jet4V;
    jet4V.SetPtEtaPhiM(jet->pt,jet->eta,jet->phi,jet->mass);

    bool badJet = false;

    for(size_t iMu = 0; iMu < goodTightMuons.size(); iMu++){
      TMuon mu = goodTightMuons.at(iMu);
      TLorentzVector mu4V ;
      mu4V.SetPtEtaPhiM(mu.pt,mu.eta,mu.phi,0);
      if(mu4V.DeltaR(jet4V) < matchingCone){
        badJet = true;
	break;
      }
    }

    for(size_t iEle = 0; iEle < goodTightElectrons.size(); iEle++){
      TElectron ele = goodTightElectrons.at(iEle);
      TLorentzVector ele4V ;
      ele4V.SetPtEtaPhiM(ele.pt,ele.eta,ele.phi,0);
      if(ele4V.DeltaR(jet4V) < matchingCone){
        badJet = true;
        break;
      }
    }

    if(badJet)
      continue;

    cleanedJets.push_back(*jet);
  }
}


void fillHistos(vector<histoContainer> & plotVector,
	       vector<variableContainer> variableList,
	       string name,
	       float weight,
	       TLorentzVector lepton1,
	       TLorentzVector lepton2,
	       TLorentzVector jet1,
	       TLorentzVector jet2,
                TLorentzVector met
	       ){

  // loop on variables                                                                                                                                                           
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    histoContainer tmpPlot;
    tmpPlot.cutName = name;
    tmpPlot.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec ;
    itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
    if(itVec == plotVector.end()){
      cerr<<"Problem -->plot not found for "<<name<<"  "<<variableList.at(iVar).variableName<<endl;
      continue ;
    }

    if(variableList.at(iVar).variableName == "ptj1"){
      itVec->histogram->Fill(jet1.Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptj2"){
      itVec->histogram->Fill(jet2.Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etaj1"){
      itVec->histogram->Fill(jet1.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etaj2" ){
      itVec->histogram->Fill(jet2.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "detajj" ){
      itVec->histogram->Fill(fabs(jet1.Eta()-jet2.Eta()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mjj" ){
      itVec->histogram->Fill((jet1+jet2).M(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
      itVec->histogram->Fill(fabs(jet1.DeltaPhi(jet2)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptl1" ){
      itVec->histogram->Fill(lepton1.Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptl2" ){
      itVec->histogram->Fill(lepton2.Pt(),weight) ;
    }

    else if(variableList.at(iVar).variableName == "etal1" ){
      itVec->histogram->Fill(lepton1.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etal2" ){
      itVec->histogram->Fill(lepton2.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mll" ){
      itVec->histogram->Fill((lepton1+lepton2).M(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptll" ){
      itVec->histogram->Fill((lepton1+lepton2).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etall" ){
      itVec->histogram->Fill(fabs ((lepton1+lepton2).Eta()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
      itVec->histogram->Fill(fabs(lepton1.DeltaPhi(lepton2)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "met" ){
      itVec->histogram->Fill(met.Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
      itVec->histogram->Fill(fabs(lepton1.DeltaPhi(met)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptLMet" ){
      itVec->histogram->Fill((lepton1 + met).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
      itVec->histogram->Fill(fabs(lepton2.DeltaPhi(met)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptTLMet" ){
      itVec->histogram->Fill((lepton2 + met).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
      itVec->histogram->Fill(fabs((lepton1+lepton2).DeltaPhi(met)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptTLMet" ){
      itVec->histogram->Fill((lepton2 + met).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
      itVec->histogram->Fill(fabs((lepton1+lepton2).DeltaPhi(met)),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptLLMet" ){
      itVec->histogram->Fill(((lepton1+lepton2) + met).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mTH" ){
      itVec->histogram->Fill(sqrt(2*(lepton1+lepton2).Pt()*met.Pt()*(1-TMath::Cos((lepton1+lepton2).DeltaPhi(met)))),weight) ;
    }
  }
}


// response plot

void fillResponse(vector<histoContainer> & plotVector,
		  vector<variableContainer> variableList,
		  string name,
		  float weight,
		  TLorentzVector lepton1,
		  TLorentzVector genlepton1,
		  TLorentzVector lepton2,
		  TLorentzVector genlepton2,
		  TLorentzVector jet1,
		  TLorentzVector genjet1,
		  TLorentzVector jet2,
		  TLorentzVector genjet2,
		  TLorentzVector met,
		  TLorentzVector genmet
	       ){

  // loop on variables                                                                                                                                                           
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    histoContainer tmpPlot;
    tmpPlot.cutName = name;
    tmpPlot.varName = variableList.at(iVar).variableName;
    vector<histoContainer>::iterator itVec ;
    itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
    if(itVec == plotVector.end()){
      cerr<<"Problem -->plot not found for "<<name<<"  "<<variableList.at(iVar).variableName<<endl;
      continue ;
    }

    if(variableList.at(iVar).variableName == "ptj1"){
      itVec->histogram->Fill((jet1.Pt()-genjet1.Pt())/(genjet1.Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptj2"){
      itVec->histogram->Fill((jet2.Pt()-genjet2.Pt())/(genjet2.Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etaj1"){
      itVec->histogram->Fill((jet1.Eta()-genjet1.Eta())/genjet1.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etaj2" ){
      itVec->histogram->Fill((jet2.Eta()-genjet2.Eta())/genjet2.Eta(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "detajj" ){
      itVec->histogram->Fill((fabs(jet1.Eta()-jet2.Eta())-fabs(genjet1.Eta()-genjet2.Eta()))/(fabs(genjet1.Eta()-genjet2.Eta())),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mjj" ){
      itVec->histogram->Fill(((jet1+jet2).M()-(genjet1+genjet2).M())/((genjet1+genjet2).M()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" ){
      itVec->histogram->Fill((fabs(jet1.DeltaPhi(jet2))-fabs(genjet1.DeltaPhi(genjet2)))/(fabs(genjet1.DeltaPhi(genjet2))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptl1" ){
      itVec->histogram->Fill((lepton1.Pt()-genlepton1.Pt())/(genlepton1.Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptl2" ){
      itVec->histogram->Fill((lepton2.Pt()-genlepton2.Pt())/(genlepton2.Pt()),weight) ;
    }

    else if(variableList.at(iVar).variableName == "etal1" ){
      itVec->histogram->Fill((lepton1.Eta()-genlepton1.Eta())/(genlepton1.Eta()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etal2" ){
      itVec->histogram->Fill((lepton2.Eta()-genlepton2.Eta())/(genlepton2.Eta()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mll" ){
      itVec->histogram->Fill(((lepton1+lepton2).M()-(genlepton1+genlepton2).M())/((genlepton1+genlepton2).M()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptll" ){
      itVec->histogram->Fill(((lepton1+lepton2).Pt()-(genlepton1+genlepton2).Pt())/((genlepton1+genlepton2).Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "etall" ){
      itVec->histogram->Fill((fabs((lepton1+lepton2).Eta())-fabs((genlepton1+genlepton2).Eta()))/(fabs((genlepton1+genlepton2).Eta())),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LL" ){
      itVec->histogram->Fill((fabs(lepton1.DeltaPhi(lepton2))-fabs(genlepton1.DeltaPhi(genlepton2)))/(fabs(genlepton1.DeltaPhi(genlepton2))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "met" ){
      itVec->histogram->Fill((met.Pt()-genmet.Pt())/genmet.Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" ){
      itVec->histogram->Fill((fabs(lepton1.DeltaPhi(met))-fabs(genlepton1.DeltaPhi(genmet)))/(fabs(genlepton1.DeltaPhi(genmet))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptLMet" ){
      itVec->histogram->Fill(((lepton1 + met).Pt()-(genlepton1 + genmet).Pt())/(genlepton1 + genmet).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" ){
      itVec->histogram->Fill((fabs(lepton2.DeltaPhi(met))-fabs(genlepton2.DeltaPhi(genmet)))/(fabs(genlepton2.DeltaPhi(genmet))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptTLMet" ){
      itVec->histogram->Fill(((lepton2 + met).Pt()-(genlepton2 + genmet).Pt())/((genlepton2 + genmet).Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
      itVec->histogram->Fill((fabs((lepton1+lepton2).DeltaPhi(met))-fabs((genlepton1+genlepton2).DeltaPhi(genmet)))/(fabs((genlepton1+genlepton2).DeltaPhi(genmet))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptTLMet" ){
      itVec->histogram->Fill(((lepton2 + met).Pt()-(genlepton2 + genmet).Pt())/((genlepton2 + genmet).Pt()),weight) ;
    }
    else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" ){
      itVec->histogram->Fill((fabs((lepton1+lepton2).DeltaPhi(met))-fabs((genlepton1+genlepton2).DeltaPhi(genmet)))/(fabs((genlepton1+genlepton2).DeltaPhi(genmet))),weight) ;
    }
    else if(variableList.at(iVar).variableName == "ptLLMet" ){
      itVec->histogram->Fill((((lepton1+lepton2) + met).Pt()-((genlepton1+genlepton2) + genmet).Pt())/((genlepton1+genlepton2) + genmet).Pt(),weight) ;
    }
    else if(variableList.at(iVar).variableName == "mTH" ){
      itVec->histogram->Fill((sqrt(2*(lepton1+lepton2).Pt()*met.Pt()*(1-TMath::Cos((lepton1+lepton2).DeltaPhi(met))))-
			      sqrt(2*(genlepton1+genlepton2).Pt()*genmet.Pt()*(1-TMath::Cos((genlepton1+genlepton2).DeltaPhi(genmet)))))/
			      sqrt(2*(genlepton1+genlepton2).Pt()*genmet.Pt()*(1-TMath::Cos((genlepton1+genlepton2).DeltaPhi(genmet)))),weight) ;
    }
  }
}
