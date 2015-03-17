#include "utilsEFT.h"

void fillRecoJetArray (vector<jetContainer> & jetVector, readTreeEFT & reader) {

  jetContainer dummy;
  if(reader.jetpt1 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt1,reader.jeteta1,reader.jetphi1,reader.jetmass1);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased1 ;
    dummy.btag_     = reader.jetBTagPhysics1 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics1;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt2 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt2,reader.jeteta2,reader.jetphi2,reader.jetmass2);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased2 ;
    dummy.btag_     = reader.jetBTagPhysics2 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics2;
    jetVector.push_back(dummy);
  }
      
  if(reader.jetpt3 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt3,reader.jeteta3,reader.jetphi3,reader.jetmass3);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased3 ;
    dummy.btag_     = reader.jetBTagPhysics3 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics3;
    jetVector.push_back(dummy);
  }


  if(reader.jetpt4 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt4,reader.jeteta4,reader.jetphi4,reader.jetmass4);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased4 ;
    dummy.btag_     = reader.jetBTagPhysics4 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics4;
    jetVector.push_back(dummy);
  }
  
  if(reader.jetpt5 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt5,reader.jeteta5,reader.jetphi5,reader.jetmass5);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased5 ;
    dummy.btag_     = reader.jetBTagPhysics5 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics5;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt6 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt6,reader.jeteta6,reader.jetphi6,reader.jetmass6);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased6 ;
    dummy.btag_     = reader.jetBTagPhysics6 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics6;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt7 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt7,reader.jeteta7,reader.jetphi7,reader.jetmass7);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased7 ;
    dummy.btag_     = reader.jetBTagPhysics7 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics7;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt8 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt8,reader.jeteta8,reader.jetphi8,reader.jetmass8);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased8 ;
    dummy.btag_     = reader.jetBTagPhysics8 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics8;
    jetVector.push_back(dummy);
  }
    
  return ;
}



// 
void fillPuppiJetArray (vector<jetContainer> & jetVector, readTreeEFT & reader) {

  jetContainer dummy;
  if(reader.jetpt_puppi1 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi1,reader.jeteta_puppi1,reader.jetphi_puppi1,reader.jetmass_puppi1);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi1 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi1 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi1;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi2 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi2,reader.jeteta_puppi2,reader.jetphi_puppi2,reader.jetmass_puppi2);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi2 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi2 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi2;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi3 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi3,reader.jeteta_puppi3,reader.jetphi_puppi3,reader.jetmass_puppi3);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi3 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi3 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi3;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi4 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi4,reader.jeteta_puppi4,reader.jetphi_puppi4,reader.jetmass_puppi4);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi4 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi4 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi4;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi5 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi5,reader.jeteta_puppi5,reader.jetphi_puppi5,reader.jetmass_puppi5);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi5 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi5 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi5;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi6 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi6,reader.jeteta_puppi6,reader.jetphi_puppi6,reader.jetmass_puppi6);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi6 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi6 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi6;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi7 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi7,reader.jeteta_puppi7,reader.jetphi_puppi7,reader.jetmass_puppi7);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi7 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi7 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi7;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi8 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi8,reader.jeteta_puppi8,reader.jetphi_puppi8,reader.jetmass_puppi8);
    dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi8 ;
    dummy.btag_     = reader.jetBTagPhysics_puppi8 ;
    dummy.jetflavour_ = reader.jetFlavourPhysics_puppi8;
    jetVector.push_back(dummy);
  }
    
  return ;
}

void fillGenJetArray (vector<jetContainer> & jetVector, readTreeEFT & reader) {

  jetContainer dummy;
  if(reader.jetGenpt1 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt1,reader.jetGeneta1,reader.jetGenphi1,reader.jetGenm1);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt2 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt2,reader.jetGeneta2,reader.jetGenphi2,reader.jetGenm2);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt3 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt3,reader.jetGeneta3,reader.jetGenphi3,reader.jetGenm3);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt4 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt4,reader.jetGeneta4,reader.jetGenphi4,reader.jetGenm4);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillTrackJetArray (vector<jetContainer> & jetVector, readTreeEFT & reader){
  for( int iJet = 0; iJet < reader.TrackJet_V4_ ; iJet++){    
    jetContainer dummy;
    dummy.jet4V_.SetPxPyPzE(reader.TrackJet_V4_fP_fX[iJet],reader.TrackJet_V4_fP_fY[iJet],reader.TrackJet_V4_fP_fZ[iJet],reader.TrackJet_V4_fE[iJet]);
    dummy.btag_    = -999 ;
    dummy.jetPUID_ = -999;     
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
    
  }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


// isolation applied here
void fillRecoLeptonsArray (vector<leptonContainer> & lepVector, readTreeEFT & reader){

  leptonContainer dummy;

  if(reader.pt1 > 0 ){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt1,reader.eta1,reader.phi1,0);    
    dummy.charge_   = reader.ch1 ;
    dummy.iso_      = reader.isoRhoCorr1 ;
    dummy.flavour_ = reader.pid1;
    lepVector.push_back(dummy);
  }

  if(reader.pt2 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt2,reader.eta2,reader.phi2,0);
    dummy.charge_   = reader.ch2 ;
    dummy.iso_      = reader.isoRhoCorr2 ;
    dummy.flavour_ = reader.pid2;
    lepVector.push_back(dummy);
  }
   
  if(reader.pt3 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt3,reader.eta3,reader.phi3,0);
    dummy.charge_   = reader.ch3 ;
    dummy.iso_      = reader.isoRhoCorr3 ;
    dummy.flavour_ = reader.pid3;
    lepVector.push_back(dummy);
  }

  if(reader.pt4 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt4,reader.eta4,reader.phi4,0);
    dummy.charge_   = reader.ch4 ;
    dummy.iso_      = reader.isoRhoCorr4 ;
    dummy.flavour_ = reader.pid4;
    lepVector.push_back(dummy);
  }

  return ;
}


bool passCutContainerSelection (readTreeEFT* reader,
				cutContainer & Cut,
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
				const string & finalStateString){


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

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
    iBin++;   
  }

  if(LeptonsAll.size() < 2 )  return false ; // skip the event --> only two reco leptons are good            
  if(RecoJetsAll.size() < 2)  return false ; // skip the event with less than two reco jet                                                     

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
    iBin++;   
  }

  // identify tight leptons and require exactly nLep                                                                                                                       
  if (int(leptonsIsoTight.size()) != Cut.nLep ) return false;                                                                                                    

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
    iBin++;   
  }

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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
    iBin++;   
  }

  // met cut
  if(reader->pfmet < Cut.MET) return false;  

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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


  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
    iBin++;   
  }

  // bveto
  int nBjets = 0 ;
  for(size_t iJet = 0; iJet < RecoJets.size(); iJet++){
    if(RecoJets.at(iJet).btag_ >= Cut.bTagCut)
      nBjets ++ ;
  }

  if(nBjets > Cut.nBVeto) return false ;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"bveto");
    iBin++;   
  }

  //soft muons
  if(int(softMuons.size()) > Cut.nSoftMu) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"softMu");
    iBin++;   
  }


  ///////// Lorentz Vector
  TLorentzVector L_dilepton;
  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;               
  
  // apply Mll cut
  if(L_dilepton.M() < Cut.Mll.first or L_dilepton.M() > Cut.Mll.second) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
    iBin++;   
  }

  if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < Cut.DetaJJ) return false;
  
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
    iBin++;   
  }

  if(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()) > Cut.DetaLL) return false;

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
	vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
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
	vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
	vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
	iBin++;   
      }
    }
  }
  else{    
    if(vect.size()!=0){
      vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
      vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Polarization");
      iBin++;   
    }
  } 

  return true;

}


void fillEFTWeights(vector<TH1F*> & histogramEFT, const float & value, const float & eventWeight, readTreeEFT* reader){

  if(histogramEFT.size() != reader->eventLHEweight->size()){
    cerr<<"EFT histograms not matching events weights --> skip : EFT histo "<<histogramEFT.size()<<" weights "<<reader->eventLHEweight->size()<<endl;
  }

  for(size_t iHisto = 0; iHisto < histogramEFT.size(); iHisto++){
    histogramEFT.at(iHisto)->Fill(value,eventWeight*reader->eventLHEweight->at(iHisto)/reader->LHEweight);
  }

  return ; 

}
