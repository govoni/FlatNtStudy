#include "dumpObjectsFromTree.h"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                          
float deltaPhi (float phi1, float phi2){
  float deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                         
bool closeToLeptons (float eta, float phi, vector<leptonContainer> & TL_leptons, float R){
  for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep){
    if ((eta - TL_leptons.at(iLep).lepton4V_.Eta ()) * (eta - TL_leptons.at(iLep).lepton4V_.Eta ()) +
        deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) * deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) < R * R)
      return true ;
  }
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                         
bool closeToLeptons (float eta, float phi, readTree & reader, float R){

  if ((eta - reader.eta1) * (eta - reader.eta1) +
      deltaPhi (phi, reader.phi1) * deltaPhi (phi, reader.phi1) < R * R) return true ;
  if ((eta - reader.eta2) * (eta - reader.eta2) +
      deltaPhi (phi, reader.phi2) * deltaPhi (phi, reader.phi2) < R * R) return true ;
  return false ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                           
void fillRecoJetArray (vector<jetContainer> & jetVector, readTree & reader) {

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
void fillPuppiJetArray (vector<jetContainer> & jetVector, readTree & reader) {

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


void fillGenJetArray (vector<jetContainer> & jetVector, readTree & reader) {

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

  if(reader.jetGenpt5 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt5,reader.jetGeneta5,reader.jetGenphi5,reader.jetGenm5);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt6 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt6,reader.jetGeneta6,reader.jetGenphi6,reader.jetGenm6);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt7 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt7,reader.jetGeneta7,reader.jetGenphi7,reader.jetGenm7);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt8 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt8,reader.jetGeneta8,reader.jetGenphi8,reader.jetGenm8);
    dummy.jetPUID_  = -999 ;
    dummy.btag_     = -999 ;
    dummy.jetflavour_     = -999 ;
    jetVector.push_back(dummy);
  }

  return ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                         
void fillTrackJetArray (vector<jetContainer> & jetVector, readTree & reader){

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


// isolation applied here                                                                                                                                                        
void fillRecoLeptonsArray (vector<leptonContainer> & lepVector, readTree & reader){

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


void fillGenLeptonsArray(vector<leptonContainer> & lepVector, readTree & reader){

  leptonContainer dummy;

  if(reader.leptonGenpt1 > 0 ){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt1,reader.leptonGeneta1,reader.leptonGenphi1,0);
    dummy.charge_   = 0. ;
    dummy.iso_      = 0. ;
    dummy.flavour_ = reader.leptonGenpid1;
    lepVector.push_back(dummy);
  }

  if(reader.leptonGenpt2 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt2,reader.leptonGeneta2,reader.leptonGenphi2,0);
    dummy.charge_   = 0. ;
    dummy.iso_      = 0. ;
    dummy.flavour_ = reader.leptonGenpid2;
    lepVector.push_back(dummy);
  }

  if(reader.leptonGenpt3 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt3,reader.leptonGeneta3,reader.leptonGenphi3,0);
    dummy.charge_   = 0. ;
    dummy.iso_      = 0. ;
    dummy.flavour_ = reader.leptonGenpid3;
    lepVector.push_back(dummy);
  }

  if(reader.leptonGenpt4 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt4,reader.leptonGeneta4,reader.leptonGenphi4,0);
    dummy.charge_   = 0. ;
    dummy.iso_      = 0 ;
    dummy.flavour_  = reader.leptonGenpid4;
    lepVector.push_back(dummy);
  }

  return ;


}


vector<leptonContainer>  dumpLeptons (vector<leptonContainer> & TL_leptons,
                                      float isocut, float minptcut){
  vector<leptonContainer>  goodLeptons ;
  for (size_t i = 0 ; i < TL_leptons.size() ; ++i){
    if (TL_leptons.at(i).iso_  > isocut) continue ;
    if (TL_leptons.at(i).lepton4V_.Pt() < minptcut) continue ;
    if (fabs(TL_leptons.at(i).flavour_) == 11 and (fabs(TL_leptons.at(i).lepton4V_.Eta()) > 1.45 and fabs(TL_leptons.at(i).lepton4V_.Eta()) < 1.55)) continue ;
    goodLeptons.push_back (TL_leptons.at(i)) ;
  }
  return goodLeptons;
}

vector<leptonContainer>  dumpLeptons (vector<leptonContainer> & TL_leptons,
                                      float isocut_mu, float isocut_el, float minptcut){
  vector<leptonContainer>  goodLeptons ;
  for (size_t i = 0 ; i < TL_leptons.size() ; ++i){
    if (TL_leptons.at(i).lepton4V_.Pt() < minptcut) continue ;
    if (TL_leptons.at(i).iso_  > isocut_mu and fabs(TL_leptons.at(i).flavour_) == 13) continue ;
    if (TL_leptons.at(i).iso_  > isocut_el and fabs(TL_leptons.at(i).flavour_) == 11) continue ;
    if (fabs(TL_leptons.at(i).flavour_) == 11 and (fabs(TL_leptons.at(i).lepton4V_.Eta()) > 1.45 and fabs(TL_leptons.at(i).lepton4V_.Eta()) < 1.55)) continue ;
    goodLeptons.push_back (TL_leptons.at(i)) ;
  }
  return goodLeptons;
}


vector<jetContainer> dumpJets (vector<jetContainer> & TL_jets,
                               vector<leptonContainer> & TL_leptons,
                               float minptcut, float btagcut, float jetPUIDCut,
                               float lepminptcut, float deltaR, float etaMax){

  vector<jetContainer> goodJets ;
  for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet){
    if (TL_jets.at(iJet).jet4V_.Pt() < minptcut)   continue ;
    if (TL_jets.at(iJet).btag_       > btagcut)    continue ;
    if (TL_jets.at(iJet).jetPUID_    < jetPUIDCut) continue ;
    if (fabs(TL_jets.at(iJet).jet4V_.Eta()) > etaMax)     continue ;

    bool discard = false ;
    for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep){
      if (TL_leptons.at(iLep).lepton4V_.Pt () < lepminptcut) continue ;
      if (TL_leptons.at(iLep).lepton4V_.DeltaR(TL_jets.at(iJet).jet4V_) < deltaR){
        discard = true ;
        break ;
      }
    }
    if (discard) continue ;
    goodJets.push_back(TL_jets.at(iJet));;
  }
  return goodJets ;
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -                                                                                             
vector<jetContainer> dumpTrackJets (vector<jetContainer> & TL_jets, vector<leptonContainer> & TL_leptons,
				    float minptcut, float lepminptcut, float deltaR){

  vector<jetContainer> goodJets ;
  for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet){
    if (TL_jets.at(iJet).jet4V_.Pt() < minptcut) continue ;
    bool discard = false ;
    for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep){
      if (TL_leptons.at(iLep).lepton4V_.Pt() < lepminptcut) continue ;
      if (TL_leptons.at(iLep).lepton4V_.DeltaR(TL_jets.at(iJet).jet4V_) < deltaR) {
	discard = true ;
	break ;
      }
    }
    if (discard) continue ;
    goodJets.push_back (TL_jets.at(iJet)) ;
  }
  return goodJets;
}


// -------------------------                                                                                                                                                     

trackJetEvent produceTrackJetEvent (vector<jetContainer> & trackJets, vector<jetContainer> & RecoJets){

  // track jet info                                                                                                                                                             
  float TKJ_SumHT = 0.,TKJ_SumHT_IN = 0., TKJ_SumHT_OUT = 0. ;
  int   TKJ_num   = 0, TKJ_num_IN   = 0,  TKJ_num_OUT  = 0 ;

  if(RecoJets.size() >=2){

    float aveEta = 0.5*(RecoJets.at(0).jet4V_.Eta()+RecoJets.at(1).jet4V_.Eta());

    float dRThreshold = 0.5;
    float TJ_etaMin = RecoJets.at (0).jet4V_.Eta () ;
    float TJ_etaMax = RecoJets.at (1).jet4V_.Eta () ;
    float TJ_phiMin = RecoJets.at (0).jet4V_.Phi () ;
    float TJ_phiMax = RecoJets.at (1).jet4V_.Phi () ;
    if (TJ_etaMin > TJ_etaMax){
      swap (TJ_etaMin, TJ_etaMax) ;
      swap (TJ_phiMin, TJ_phiMax) ;
    }

    // loop over track jets                                                                                                                                                    
    for (size_t iJet = 0 ; iJet < trackJets.size () ; ++iJet){

      float iJetPhi = trackJets.at (iJet).jet4V_.Phi () ;
      float iJetEta = trackJets.at (iJet).jet4V_.Eta () ;
      float iJetPt  = trackJets.at (iJet).jet4V_.Pt () ;

      float dR2_Min = deltaPhi(TJ_phiMin, iJetPhi);
      dR2_Min *= dR2_Min ;
      dR2_Min += (iJetEta - TJ_etaMin) * (iJetEta - TJ_etaMin) ;
      float dR2_Max = deltaPhi (TJ_phiMax, iJetPhi) ;
      dR2_Max *= dR2_Max ;
      dR2_Max += (iJetEta - TJ_etaMax) * (iJetEta - TJ_etaMax) ;
      // veto the tag jets                                                                                                                                                     
      if (sqrt(dR2_Max) < dRThreshold || sqrt(dR2_Min) < dRThreshold) continue ;

      float iJetModPhi = iJetPhi ;
      float iJetZep    = (trackJets.at (iJet).jet4V_.Eta () - aveEta) /(TJ_etaMax - TJ_etaMin);
      if (iJetZep < -0.5)     iJetModPhi -= TJ_phiMin ;
      else if (iJetZep > 0.5) iJetModPhi -= TJ_phiMax ;

      ++TKJ_num ;
      TKJ_SumHT += iJetPt ;

      if (iJetEta > TJ_etaMin && iJetEta < TJ_etaMax){
        ++TKJ_num_IN ;
        TKJ_SumHT_IN += iJetPt ;
      }

      else if (iJetEta < TJ_etaMin || iJetEta > TJ_etaMax){
        ++TKJ_num_OUT ;
        TKJ_SumHT_OUT += iJetPt ;
      }
    }
  }

  return trackJetEvent(TKJ_num,TKJ_num_IN,TKJ_num_OUT,
                       TKJ_SumHT,TKJ_SumHT_IN,TKJ_SumHT_OUT);

}


// -------------------------                                                                                                                                                     
vector<leptonContainer> dumpSoftMuons (vector<leptonContainer> & leptonAll,
                                       vector<jetContainer> & jetAll,
                                       float isolationCut,
                                       float minLeptonCut,
                                       float minptcut,
                                       float deltaR){

  vector<leptonContainer> softMuons ;

  for( size_t iLep = 0; iLep < leptonAll.size(); iLep++){

    if(leptonAll.at(iLep).lepton4V_.Pt() < minptcut) continue;
    if(fabs(leptonAll.at(iLep).flavour_) != 13)      continue;
    if(leptonAll.at(iLep).lepton4V_.Pt() > minLeptonCut and leptonAll.at(iLep).iso_ <  isolationCut) continue;

    bool goodSoftMu = false;
    for(size_t iJet = 0; iJet < jetAll.size(); iJet++){
      if( leptonAll.at(iLep).lepton4V_.DeltaR(jetAll.at(iJet).jet4V_) > deltaR )
        continue ;
      goodSoftMu = true ;
    }

    if(goodSoftMu)
      softMuons.push_back(leptonAll.at(iLep));
  }

  return softMuons;

}
