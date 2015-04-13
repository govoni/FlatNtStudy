#include "utils.h"
#include "fakeBackgroundUtils.h"
#include "kinematicUtils.h"
#include <memory>

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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


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



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


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

//----------------------
void loopOnEvents (plotter & analysisPlots, 
		   const string & sampleName,     
		   const int    & samplePosition,     
		   readTree* reader, 
		   vector<cutContainer> & CutList,
		   vector<variableContainer> & VariableList,
		   const bool & usePuppiAsDefault, 
		   const float & minPtLeptonCut, 
		   const float & minPtLeptonCutCleaning,
		   const float & leptonIsoCut_mu,
		   const float & leptonIsoCut_el, 
		   const float & leptonIsoLooseCut,
		   const float & matchingCone,   
		   const float & minJetCutPt,   
		   map <string,TH1F*> & vect,
		   const string & finalStateString,
		   const string & scenarioString,
		   const string & fakeRateFile) {

  cout << "reading sample " << sampleName << "\n" ;
  cout << "analysis object sys set to "<<analysisPlots.getSystematics()<<endl;

  int maxevents = reader->fChain->GetEntries() ;

  // define the formulat for systematics study  --> should be provided by Egamma, Muon and JME POGs
  objectFormula scenarioFormula (scenarioString); 

  // define fake rate and migration matrix  
  shared_ptr<TFile> inputFile(new TFile(fakeRateFile.c_str(),"READ"));
  fakeRateContainer       fakeRate(inputFile.get()); 
  fakeMigrationContainer* fakeMigration = new fakeMigrationContainer(inputFile.get());

  // fake rate application --> check if the fake method has to be applied
  vector<sample> vecSample = analysisPlots.getSamples();
  bool applyFake = false;
  bool applyCharge = false;
  for( size_t iSample = 0; iSample < vecSample.size(); iSample++){
    if(vecSample.at(iSample).m_sampleName != sampleName) 
      continue ;
    if(vecSample.at(iSample).m_isSignal == -1)
      applyFake = true;
    if(vecSample.at(iSample).m_isSignal == -2)
      applyCharge = true ;
  }

  if(applyCharge and applyFake){
    cerr<<"problem in the setup, both fakes and charge misid has to be applied for this sample --> please check"<<endl;
    return ;
  }

  
  // loop over events ////////                                                                                                                                               
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){                                                                                                                      

    reader->fChain->GetEntry(iEvent) ;                                                                                                                                      
           
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;                                                                                                  

    // filter LHE level leptons for madgraph polarized events /////////
    if(not applyFake){
      if(TString(sampleName).Contains("Madgraph") or TString(sampleName).Contains("WW_EWK") or TString(sampleName).Contains("WW_QCD")){
	if(TString(finalStateString).Contains("UU")){
	  if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
	    continue;
	}
	else if(TString(finalStateString).Contains("EE")){
	  if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) 
	    continue;
	}
	else if(TString(finalStateString).Contains("EU")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 11) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 13) 
	    continue ;
	}
	else if(TString(finalStateString).Contains("UE")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 13) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 11) 
	    continue ;
	}
    
	// if an event pass the cut, fill the associated map                                                                                                                 
	TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2 ;

	L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
	L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
      
	L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
	L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

	if(L_lepton1.Pt() < minPtLeptonCut or L_lepton2.Pt() < minPtLeptonCut) 
	  continue;
      } 
    }   
      
    // loop on the cut list
    for(size_t iCut = 0; iCut < CutList.size() ; iCut++){ 

      TLorentzVector L_met, L_gen_met;
      TLorentzVector L_met_muonScaleUp, L_met_muonScaleDown, L_met_muonRes;
      TLorentzVector L_met_electronScaleUp, L_met_electronScaleDown, L_met_electronRes;
      TLorentzVector L_met_jetScaleUp, L_met_jetScaleDown, L_met_jetRes;

      if(not usePuppiAsDefault)
	L_met.SetPtEtaPhiM(reader->pfmet,0.,reader->pfmetphi, 0.) ;                                                                                                        
      else
	L_met.SetPtEtaPhiM(reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;                                                                                              

      L_gen_met.SetPtEtaPhiM(reader->metGenpt,0.,reader->metGenphi, 0.) ;                                                                                             

      //Lepton Sector ///////      
      vector<leptonContainer> LeptonsAll;
      vector<leptonContainer> muonsAll,muonsAllScaleUp, muonsAllScaleDown, muonsAllRes;
      vector<leptonContainer> electronsAll,electronsAllScaleUp, electronsAllScaleDown, electronsAllRes;
      fillRecoLeptonsArray (LeptonsAll, *reader);

      // dump tight leptons                                                                                                                                        
      vector<leptonContainer> leptonsIsoTight;
      vector<leptonContainer> muonsIsoTight,muonsIsoTightScaleUp, muonsIsoTightScaleDown, muonsIsoTightRes;
      vector<leptonContainer> electronsIsoTight,electronsIsoTightScaleUp, electronsIsoTightScaleDown, electronsIsoTightRes;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

      // take reco jets                                                                                                                                                   
      vector<jetContainer> RecoJetsAll, RecoJetsAllScaleUp, RecoJetsAllScaleDown, RecoJetsAllRes;
      if(not usePuppiAsDefault){
	fillRecoJetArray (RecoJetsAll, *reader) ;
      }
      else{
	fillPuppiJetArray (RecoJetsAll, *reader) ;
      }

      // take soft muons 
      vector<leptonContainer> softMuons;
      softMuons = dumpSoftMuons(LeptonsAll,RecoJetsAll,leptonIsoCut_mu,minPtLeptonCut,softMuonPt,matchingCone);

      // take jets                                                                                                                                                          
      vector<jetContainer> RecoJets, RecoJetsScaleUp, RecoJetsScaleDown, RecoJetsRes;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);

      // take gen jets                                                                                                                                                
      vector<jetContainer> GenJetsAll ;
      fillGenJetArray (GenJetsAll, *reader) ;

      // take gen jets                                                                                                                                                     
      vector<jetContainer> GenJets;
      GenJets  = dumpJets (GenJetsAll, leptonsIsoTight, 0., 999, -999, minPtLeptonCutCleaning, matchingCone);

      // take track jets
      vector<jetContainer> trackJetsAll;
      fillTrackJetArray (trackJetsAll,*reader) ;
            
      if(applyFake){ // the sample require the fake rate application
	
	makeFakeLeptonBackground( sampleName,        // name of the sample
				  samplePosition,     // position used in case of mutiple physics process with same sample name
				  finalStateString,  // final state string
				  analysisPlots,     // plotter object
				  reader,            // reader object
				  CutList.at(iCut),  // cut container object
				  VariableList,      // vector<variableContainer>
				  leptonsIsoTight,   // tight leptons as input
				  LeptonsAll,        // all leptons as input
				  softMuons,         // softMuons
				  RecoJets,          // reco jets 
				  GenJets,           // gen jets 
				  trackJetsAll,      // all track jets
				  L_met,             // missing energy
				  minJetCutPt,        // min jet pt
				  leptonIsoLooseCut,  // loose iso cut
				  minPtLeptonCutCleaning,  // cleaning lepton-jet
				  matchingCone,       // matching for cleaning
				  fakeRate,           // fake rate map
				  *fakeMigration,     // migration
				  vect);              // efficiency histograms
      }

      else if (applyCharge){

	makeFakeChargeBackground(sampleName,        // sampleName
				 samplePosition,     // samplePostion
				 finalStateString,  // indentify final state
				 analysisPlots,     // plotter object
				 reader,            // tree reader
				 CutList.at(iCut),  // cut container
				 VariableList,      // list of variables to plot
				 leptonsIsoTight,   // tight leptons
				 LeptonsAll,        // all leptons
				 softMuons,         // softMuons
				 RecoJets,          // reco jets 
				 GenJets,           // gen jets 
				 trackJetsAll,      // all track jets
				 L_met,             // missing energy
				 minJetCutPt,        // min jet pt
				 leptonIsoLooseCut,  // loose iso cut
				 minPtLeptonCutCleaning,  // cleaning lepton-jet
				 matchingCone,       // matching for cleaning
				 vect                // efficiency histos
				 );
      }

      else { // if not fake rate sample
      
	float dRThreshold     = 0.5 ;      
	float eventFakeWeight = 1;

	vector<jetContainer> trackJets ;
	trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
	trackJetEvent trackEvent;
	trackEvent = produceTrackJetEvent (trackJets,RecoJets);

	/// insert the systematics in the analysis
	if(analysisPlots.getSystematics() and not applyFake){

	  // lepton scale and resolution sector
	  for(size_t iLep = 0; iLep < LeptonsAll.size(); iLep++){
	    float scaleValueUnc = 0;
	    float resValueUnc = 0;

	    if(abs(LeptonsAll.at(iLep).flavour_) == 13){	     

	      muonsAll.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      muonsAllRes.push_back(LeptonsAll.at(iLep));
	      
	      scaleValueUnc = scenarioFormula.evaluateMuonScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateMuonResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      muonsAllScaleUp.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							  muonsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      muonsAllScaleDown.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
							    muonsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.E()*(1-scaleValueUnc));


	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);

	      muonsAllRes.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(resUnc),							  
						      muonsAll.back().lepton4V_.Py()*(resUnc),
						      muonsAll.back().lepton4V_.Pz()*(resUnc),
						      muonsAll.back().lepton4V_.E()*(resUnc));

	      L_met_muonScaleUp   = L_met - muonsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonScaleDown = L_met - muonsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonRes       = L_met - muonsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;


	    }

	    else if (abs(LeptonsAll.at(iLep).flavour_) == 11){

	      electronsAll.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      electronsAllRes.push_back(LeptonsAll.at(iLep));

	      scaleValueUnc = scenarioFormula.evaluateElectronScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateElectronResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      electronsAllScaleUp.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							      electronsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      electronsAllScaleDown.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
								electronsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);
	      
	      electronsAllRes.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(resUnc),							  
							  electronsAll.back().lepton4V_.Py()*(resUnc),
							  electronsAll.back().lepton4V_.Pz()*(resUnc),
							  electronsAll.back().lepton4V_.E()*(resUnc));

	      L_met_electronScaleUp   = L_met - electronsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronScaleDown = L_met - electronsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronRes       = L_met - electronsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;

	    }

	  }

	  muonsIsoTightScaleUp   = dumpLeptons (muonsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightScaleDown = dumpLeptons (muonsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightRes       = dumpLeptons (muonsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTight          = dumpLeptons (muonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  electronsIsoTightScaleUp   = dumpLeptons (electronsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightScaleDown = dumpLeptons (electronsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightRes       = dumpLeptons (electronsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTight          = dumpLeptons (electronsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);


	  // jet scale and resolution sector
	  for(size_t iJet = 0; iJet < RecoJetsAll.size(); iJet++){

	    RecoJetsAllScaleUp.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllScaleDown.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllRes.push_back(RecoJetsAll.at(iJet));
	  
	    float jetScaleUnc = scenarioFormula.evaluateJetScaleUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta());
	    float jetResUnc   = scenarioFormula.evaluateJetResolutionUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta(),RecoJetsAll.at(iJet).jet4V_.E());

	    RecoJetsAllScaleUp.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(1+jetScaleUnc), 
							RecoJetsAll.at(iJet).jet4V_.Py()*(1+jetScaleUnc),
							RecoJetsAll.at(iJet).jet4V_.Pz()*(1+jetScaleUnc),
							RecoJetsAll.at(iJet).jet4V_.E()*(1+jetScaleUnc));

	    RecoJetsAllScaleDown.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(1-jetScaleUnc), 
							  RecoJetsAll.at(iJet).jet4V_.Py()*(1-jetScaleUnc),
							  RecoJetsAll.at(iJet).jet4V_.Pz()*(1-jetScaleUnc),
							  RecoJetsAll.at(iJet).jet4V_.E()*(1-jetScaleUnc));

	    float resUnc = 1 + gRandom->Gaus(0,jetResUnc);

	    RecoJetsAllRes.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(resUnc), 
						    RecoJetsAll.at(iJet).jet4V_.Py()*(resUnc),
						    RecoJetsAll.at(iJet).jet4V_.Pz()*(resUnc),
						    RecoJetsAll.at(iJet).jet4V_.E()*(resUnc));

	    L_met_jetScaleUp   = L_met - RecoJetsAllScaleUp.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetScaleDown = L_met - RecoJetsAllScaleDown.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetRes       = L_met - RecoJetsAllRes.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;

	  }

	  RecoJetsScaleUp   = dumpJets (RecoJetsAllScaleUp,leptonsIsoTight,minJetCutPt,999,CutList.at(iCut).jetPUID,minPtLeptonCutCleaning,matchingCone);
	  RecoJetsScaleDown = dumpJets (RecoJetsAllScaleDown, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning,matchingCone);
	  RecoJetsRes       = dumpJets (RecoJetsAllRes, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	}     


	// analysis with nominal objects
	if( passCutContainerSelection (CutList.at(iCut),
				       sampleName,
				       samplePosition,
				       reader,
				       LeptonsAll,
				       leptonsIsoTight,
				       softMuons,
				       RecoJets,
				       trackEvent,
				       L_met,
				       minPtLeptonCut,
				       leptonIsoLooseCut,
				       vect,
				       finalStateString)){
	  
	  fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		    leptonsIsoTight, softMuons, RecoJets, GenJets, trackEvent, L_met, "",eventFakeWeight);
	}

	/// if perform sys analysis      
	if(analysisPlots.getSystematics()){

	  // analysis scaling muons Up
	  map<string,TH1F*> tempVect;

	  vector<leptonContainer> leptonAll (muonsAllScaleUp);
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  vector<leptonContainer> leptonTight (muonsIsoTightScaleUp);
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleUp, "muScaleUp",eventFakeWeight);
	  }


	  // analysis scaling muon down
	  leptonAll.clear();
	  leptonAll = muonsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  leptonTight.clear();
	  leptonTight = muonsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	    
	  }


	  // scale electron up
	  leptonAll.clear();
	  leptonAll = electronsAllScaleUp;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());

	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleUp;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleUp, "elScaleUp",eventFakeWeight);
	  }


	  // scale electron down
	  leptonAll.clear();
	  leptonAll = electronsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleDown, "elScaleDown",eventFakeWeight);
	  }


	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = muonsAllRes;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());
	  leptonTight.clear();
	  leptonTight = muonsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonRes, "muRes",eventFakeWeight);
	  }


	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = electronsAllRes;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
		      leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_electronRes, "elRes",eventFakeWeight);
	  }

	  //// jets scale up
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleUp,
					 trackEvent,
					 L_met_jetScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, VariableList, 
		      leptonsIsoTight, softMuons, RecoJetsScaleUp, 
		      GenJets,trackEvent,
		      L_met_jetScaleUp, "jetScaleUp",eventFakeWeight);
	  }

	  // analysis scaling jets down
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,					 
					 softMuons,
					 RecoJetsScaleDown,
					 trackEvent,
					 L_met_jetScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName,VariableList, 
		      leptonsIsoTight,softMuons,RecoJetsScaleDown, 
		      GenJets,trackEvent,
		      L_met_jetScaleDown, 
		      "jetScaleDown",eventFakeWeight);
	  }


	  // analysis smearing jets
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsRes,
					 trackEvent,
					 L_met_jetRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots,sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName,VariableList, 
		      leptonsIsoTight,
		      softMuons,RecoJetsRes, 
		      GenJets, trackEvent,
		      L_met_jetRes,
		      "jetRes",eventFakeWeight);
	  }	
	}
      }
    }
  }
}

void loopOnEvents (plotter & analysisPlots, 
		   const string & sampleName,     
		   const int    & samplePosition,     
		   readTree* reader, 
		   vector<cutContainer> & CutList,
		   vector<variableContainer> & VariableList,
		   vector<variableContainer2D> & VariableList2D,
		   const bool & usePuppiAsDefault, 
		   const float & minPtLeptonCut, 
		   const float & minPtLeptonCutCleaning,
		   const float & leptonIsoCut_mu,
		   const float & leptonIsoCut_el, 
		   const float & leptonIsoLooseCut,
		   const float & matchingCone,   
		   const float & minJetCutPt,   
		   map <string,TH1F*> & vect,
		   const string & finalStateString,
		   const string & scenarioString,
		   const string & fakeRateFile) {

  cout << "reading sample " << sampleName << "\n" ;
  cout<<"analysis object sys set to "<<analysisPlots.getSystematics()<<endl;

  int maxevents = reader->fChain->GetEntries() ;

  // define the formulat for systematics study  
  objectFormula scenarioFormula (scenarioString); 

  // define fake rate and migration matrix
  shared_ptr<TFile> inputFile(new TFile(fakeRateFile.c_str(),"READ"));
  fakeRateContainer       fakeRate(inputFile.get()); 
  fakeMigrationContainer* fakeMigration = new fakeMigrationContainer(inputFile.get());


  // fake rate application --> check if the fake method has to be applied
  vector<sample> vecSample = analysisPlots.getSamples();
  bool applyFake = false;
  bool applyCharge = false;
  for( size_t iSample = 0; iSample < vecSample.size(); iSample++){
    if(vecSample.at(iSample).m_sampleName != sampleName) 
      continue ;
    if(vecSample.at(iSample).m_isSignal == -1)
      applyFake = true;
    if(vecSample.at(iSample).m_isSignal == -2)
      applyCharge = true ;
  }

  if(applyCharge and applyFake){
    cerr<<"problem in the setup, both fakes and charge misid has to be applied for this sample --> please check"<<endl;
    return ;
  }
  
  // loop over events ////////                                                                                                                                               
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){                                                                                                                      

    reader->fChain->GetEntry(iEvent) ;                                                                                                                                      
           
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;                                                                                                  

    // filter LHE level leptons for madgraph polarized events /////////
    if(not applyFake){
      if(TString(sampleName).Contains("Madgraph") or TString(sampleName).Contains("WW_EWK") or TString(sampleName).Contains("WW_QCD")){
	if(TString(finalStateString).Contains("UU")){
	  if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
	    continue;
	}
	else if(TString(finalStateString).Contains("EE")){
	  if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) 
	    continue;
	}
	else if(TString(finalStateString).Contains("EU")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 11) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 13) 
	    continue ;
	}
	else if(TString(finalStateString).Contains("UE")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 13) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 11) 
	    continue ;
	}
      }

      // if an event pass the cut, fill the associated map                                                                                                                 
      TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2 ;

      L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
      L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
      
      L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
      L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

      if(L_lepton1.Pt() < minPtLeptonCut or L_lepton2.Pt() < minPtLeptonCut) 
	continue;
    }

    
    for(size_t iCut = 0; iCut < CutList.size() ; iCut++){ 

      TLorentzVector L_met, L_gen_met;

      TLorentzVector L_met_muonScaleUp, L_met_muonScaleDown, L_met_muonRes;
      TLorentzVector L_met_electronScaleUp, L_met_electronScaleDown, L_met_electronRes;
      TLorentzVector L_met_jetScaleUp, L_met_jetScaleDown, L_met_jetRes;

      if(not usePuppiAsDefault)
	L_met.SetPtEtaPhiM       (reader->pfmet,0.,reader->pfmetphi, 0.) ;                                                                                                  
      else
	L_met.SetPtEtaPhiM (reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;                                                                                        
      
      L_gen_met.SetPtEtaPhiM   (reader->metGenpt,0.,reader->metGenphi, 0.) ;                                                                                             

      //Lepton Sector ///////      
      vector<leptonContainer> LeptonsAll;
      vector<leptonContainer> muonsAll,muonsAllScaleUp, muonsAllScaleDown, muonsAllRes;
      vector<leptonContainer> electronsAll,electronsAllScaleUp, electronsAllScaleDown, electronsAllRes;
      fillRecoLeptonsArray (LeptonsAll, *reader);

      // dump tight leptons                                                                                                                                        
      vector<leptonContainer> leptonsIsoTight;
      vector<leptonContainer> muonsIsoTight,muonsIsoTightScaleUp, muonsIsoTightScaleDown, muonsIsoTightRes;
      vector<leptonContainer> electronsIsoTight,electronsIsoTightScaleUp, electronsIsoTightScaleDown, electronsIsoTightRes;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

      // take reco jets                                                                                                                                                   
      vector<jetContainer> RecoJetsAll, RecoJetsAllScaleUp, RecoJetsAllScaleDown, RecoJetsAllRes;
      if(not usePuppiAsDefault){
	fillRecoJetArray (RecoJetsAll, *reader) ;
      }
      else{
	fillPuppiJetArray (RecoJetsAll, *reader) ;
      }

      // dump soft muons
      vector<leptonContainer> softMuons;
      softMuons = dumpSoftMuons(LeptonsAll,RecoJetsAll,leptonIsoCut_mu,minPtLeptonCut,softMuonPt,matchingCone);

      // take jets                                                                                                                                                          
      vector<jetContainer> RecoJets, RecoJetsScaleUp, RecoJetsScaleDown, RecoJetsRes;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);

      // take gen jets                                                                                                                                                
      vector<jetContainer> GenJetsAll ;
      fillGenJetArray (GenJetsAll, *reader) ;

      // take gen jets                                                                                                                                                     
      vector<jetContainer> GenJets;
      GenJets  = dumpJets (GenJetsAll, leptonsIsoTight, 0., 999, -999, minPtLeptonCutCleaning, matchingCone);
      
      // take track jets
      vector<jetContainer> trackJetsAll;
      fillTrackJetArray (trackJetsAll,*reader) ;
      vector<jetContainer> trackJets ;
      float dRThreshold = 0.5 ;      

      float eventFakeWeight = 1.;

      if(applyFake){ // the sample require the fake rate application
	

	makeFakeLeptonBackground( sampleName,        // name of the sample
				  samplePosition,     
				  finalStateString,  // final state string
				  analysisPlots,     // plotter object
				  reader,            // reader object
				  CutList.at(iCut),  // cut container object
				  VariableList,      // vector<variableContainer>
				  VariableList2D,      // vector<variableContainer>
				  leptonsIsoTight,   // tight leptons as input
				  LeptonsAll,        // all leptons as input
				  softMuons,         // softMuons
				  RecoJets,          // reco jets 
				  GenJets,           // gen jets 
				  trackJetsAll,      // all track jets
				  L_met,             // missing energy
				  minJetCutPt,        // min jet pt
				  leptonIsoLooseCut,  // loose iso cut
				  minPtLeptonCutCleaning,  // cleaning lepton-jet
				  matchingCone,       // matching for cleaning
				  fakeRate,           // fake rate map
				  *fakeMigration,     // migration
				  vect);              // efficiency histograms
      }

      else if (applyCharge){

	makeFakeChargeBackground(sampleName,        // sampleName
				 samplePosition,     
				 finalStateString,  // indentify final state
				 analysisPlots,     // plotter object
				 reader,            // tree reader
				 CutList.at(iCut),  // cut container
				 VariableList,      // list of variables to plot
				 VariableList2D,      // list of variables to plot
				 leptonsIsoTight,   // tight leptons
				 LeptonsAll,        // all leptons
				 softMuons,         // softMuons
				 RecoJets,          // reco jets 
				 GenJets,           // gen jets 
				 trackJetsAll,      // all track jets
				 L_met,             // missing energy
				 minJetCutPt,        // min jet pt
				 leptonIsoLooseCut,  // loose iso cut
				 minPtLeptonCutCleaning,  // cleaning lepton-jet
				 matchingCone,       // matching for cleaning
				 vect                // efficiency histos
				 );
      }
      else { // if not fake rate sample
      
	trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
	trackJetEvent trackEvent = produceTrackJetEvent(trackJets,RecoJets);
	
	if(analysisPlots.getSystematics()){

	  // lepton scale and resolution sector
	  for(size_t iLep = 0; iLep < LeptonsAll.size(); iLep++){
	    float scaleValueUnc = 0;
	    float resValueUnc = 0;

	    if(abs(LeptonsAll.at(iLep).flavour_) == 13){

	      muonsAll.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      muonsAllRes.push_back(LeptonsAll.at(iLep));
	      
	      scaleValueUnc = scenarioFormula.evaluateMuonScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateMuonResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      muonsAllScaleUp.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							  muonsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      muonsAllScaleDown.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
							    muonsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);


	      muonsAllRes.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(resUnc),							  
						      muonsAll.back().lepton4V_.Py()*(resUnc),
						      muonsAll.back().lepton4V_.Pz()*(resUnc),
						      muonsAll.back().lepton4V_.E()*(resUnc));
	      

	      L_met_muonScaleUp   = L_met - muonsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonScaleDown = L_met - muonsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonRes       = L_met - muonsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      
	    }
	    
	    else if (abs(LeptonsAll.at(iLep).flavour_) == 11){

	      electronsAll.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      electronsAllRes.push_back(LeptonsAll.at(iLep));

	      scaleValueUnc = scenarioFormula.evaluateElectronScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateElectronResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      electronsAllScaleUp.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							      electronsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.E()*(1+scaleValueUnc));


	      electronsAllScaleDown.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
								electronsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);
	      
	      electronsAllRes.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(resUnc),							  
							  electronsAll.back().lepton4V_.Py()*(resUnc),
							  electronsAll.back().lepton4V_.Pz()*(resUnc),
							  electronsAll.back().lepton4V_.E()*(resUnc));

	      L_met_electronScaleUp   = L_met - electronsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronScaleDown = L_met - electronsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronRes       = L_met - electronsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;


	    }

	  }

	  muonsIsoTightScaleUp   = dumpLeptons (muonsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightScaleDown = dumpLeptons (muonsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightRes       = dumpLeptons (muonsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
      	  muonsIsoTight          = dumpLeptons (muonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  electronsIsoTightScaleUp   = dumpLeptons (electronsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightScaleDown = dumpLeptons (electronsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightRes       = dumpLeptons (electronsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTight          = dumpLeptons (electronsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  for(size_t iJet = 0; iJet < RecoJetsAll.size(); iJet++){

	    RecoJetsAllScaleUp.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllScaleDown.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllRes.push_back(RecoJetsAll.at(iJet));

	    float jetScaleUnc   = scenarioFormula.evaluateJetScaleUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta());
	    float jetResUnc  = scenarioFormula.evaluateJetResolutionUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta(),RecoJetsAll.at(iJet).jet4V_.E());

	    RecoJetsAllScaleUp.back().jet4V_.SetPxPyPzE(RecoJetsAllScaleUp.back().jet4V_.Px()*(1+jetScaleUnc), 
							RecoJetsAllScaleUp.back().jet4V_.Py()*(1+jetScaleUnc),
							RecoJetsAllScaleUp.back().jet4V_.Pz()*(1+jetScaleUnc),
							RecoJetsAllScaleUp.back().jet4V_.E()*(1+jetScaleUnc));

	    RecoJetsAllScaleDown.back().jet4V_.SetPxPyPzE(RecoJetsAllScaleDown.back().jet4V_.Px()*(1-jetScaleUnc), 
							  RecoJetsAllScaleDown.back().jet4V_.Py()*(1-jetScaleUnc),
							  RecoJetsAllScaleDown.back().jet4V_.Pz()*(1-jetScaleUnc),
							  RecoJetsAllScaleDown.back().jet4V_.E()*(1-jetScaleUnc));

	    float resUnc = 1 + gRandom->Gaus(0,jetResUnc);

	    RecoJetsAllRes.back().jet4V_.SetPxPyPzE(RecoJetsAllRes.back().jet4V_.Px()*(resUnc), 
						    RecoJetsAllRes.back().jet4V_.Py()*(resUnc),
						    RecoJetsAllRes.back().jet4V_.Pz()*(resUnc),
						    RecoJetsAllRes.back().jet4V_.E()*(resUnc));

	    L_met_jetScaleUp   = L_met - RecoJetsAllScaleUp.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetScaleDown = L_met - RecoJetsAllScaleDown.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetRes       = L_met - RecoJetsAllRes.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	  }

	  RecoJetsScaleUp  = dumpJets (RecoJetsAllScaleUp, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	  RecoJetsScaleDown  = dumpJets (RecoJetsAllScaleDown, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	  RecoJetsRes  = dumpJets (RecoJetsAllRes, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	
	}     
      
	// analysis with nominal objects
	if( passCutContainerSelection (CutList.at(iCut),
				       sampleName,
				       samplePosition,     
				       reader,
				       LeptonsAll,
				       leptonsIsoTight,
				       softMuons,				       
				       RecoJets,
				       trackEvent,
				       L_met,
				       minPtLeptonCut,
				       leptonIsoLooseCut,
				       vect,
				       finalStateString)){

	  fillHisto(analysisPlots, sampleName, samplePosition,
		    CutList.at(iCut).cutLayerName,VariableList, 
		    leptonsIsoTight,softMuons,RecoJets, 
		    GenJets,trackEvent,
		    L_met,"",eventFakeWeight);

	  fillHisto2D(analysisPlots, sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName,VariableList2D, 
		      leptonsIsoTight,softMuons,RecoJets, 
		      GenJets,trackEvent,
		      L_met,"",eventFakeWeight);
	}
      
	/// if perform sys analysis
	if(analysisPlots.getSystematics()){

	  // analysis scaling leptons Up
	  map<string,TH1F*> tempVect;

	  vector<leptonContainer> leptonAll (muonsAllScaleUp);
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  vector<leptonContainer> leptonTight (muonsIsoTightScaleUp);
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_muonScaleUp, 
		      "muScaleUp",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_muonScaleUp,"muScaleUp",eventFakeWeight);
	  }

	  
	  // analysis scaling muon down
	  leptonAll.clear();
	  leptonAll = muonsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  leptonTight.clear();
	  leptonTight = muonsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());


	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){

	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_muonScaleDown, 
		      "muScaleDown",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	  }

	  // scale electron up
	  leptonAll.clear();
	  leptonAll = electronsAllScaleUp;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());

	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleUp;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_electronScaleUp, 
		      "elScaleUp",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_electronScaleUp,"elScaleUp",eventFakeWeight);
	  }

	  // scale electron down
	  leptonAll.clear();
	  leptonAll = electronsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_electronScaleDown, 
		      "elScaleDown",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_electronScaleDown,"elScaleDown",eventFakeWeight);
	  }

	  
	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = muonsAllRes;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());
	  leptonTight.clear();
	  leptonTight = muonsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());
	  
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_muonRes, 
		      "muRes",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_muonRes,"muRes",eventFakeWeight);
	  }

	  
	  leptonAll.clear();
	  leptonAll = electronsAllRes;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonTight, 
		      softMuons,
		      RecoJets, 
		      GenJets,
		      trackEvent,
		      L_met_electronRes, 
		      "elRes",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met_electronRes,"elRes",eventFakeWeight);
	  }


	  //// jets
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleUp,
					 trackEvent,
					 L_met_jetScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, 
		      samplePosition,
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonsIsoTight,
		      softMuons,
		      RecoJetsScaleUp, 
		      GenJets,
		      trackEvent,
		      L_met_jetScaleUp, 
		      "jetScaleUp",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, 
			samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonsIsoTight,
			softMuons,
			RecoJetsScaleUp, 
			GenJets,trackEvent,
			L_met_jetScaleUp,"jetScaleUp",eventFakeWeight);
	  }

	  // analysis scaling jets down
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleDown,
					 trackEvent,
					 L_met_jetScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, 
		      samplePosition,     
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonsIsoTight, 
		      softMuons,
		      RecoJetsScaleDown, 
		      GenJets,
		      trackEvent,
		      L_met_jetScaleDown, 
		      "jetScaleDown",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonsIsoTight,
			softMuons,
			RecoJetsScaleDown, 
			GenJets,trackEvent,
			L_met_jetScaleDown,"jetScaleDown",eventFakeWeight);
	  }


	  // analysis smearing jets
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsRes,
					 trackEvent,
					 L_met_jetRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString)){
	  
	    fillHisto(analysisPlots, 
		      sampleName, 
		      samplePosition,     
		      CutList.at(iCut).cutLayerName, 
		      VariableList, 
		      leptonsIsoTight, 
		      softMuons,
		      RecoJetsRes, 
		      GenJets,
		      trackEvent,
		      L_met_jetRes,
		      "jetRes",eventFakeWeight);

	    fillHisto2D(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList2D, 
			leptonsIsoTight,
			softMuons,
			RecoJetsRes, 
			GenJets,trackEvent,
			L_met_jetRes,"jetRes",eventFakeWeight);
	  
	  }
	}
      }
    }    
  }  
}


void fillHisto( plotter & analysisPlots,
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
    else if(VariableList.at(iVar).variableName == "phill"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Phi(),eventFakeWeight,systematicName);   
    }
    else if(VariableList.at(iVar).variableName == "dRll"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);   
    }
    else if(VariableList.at(iVar).variableName == "etall"){
      analysisPlots.fillHisto (NameSample, samplePosition,cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Eta(),eventFakeWeight,systematicName);   
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
void fillHisto2D ( plotter & analysisPlots,
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

    ////
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
    
    
    ///
    
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
				 sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);   
    }
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
				 fabs(L_dilepton.DeltaPhi(L_met)),sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);   
    }
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

///// method in  order to apply cuts

bool passCutContainerSelection (cutContainer & Cut,
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
				const string & finalStateString){

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
  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
    iBin++;   
  }

  if(LeptonsAll.size() < 2 )  return false ; // skip the event --> only two reco leptons are good            


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
  if(L_met.Pt() < Cut.MET) return false;  

  if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+1);   
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"met selection");
    iBin++;   
  }

  // jets cut
  if(RecoJets.size() < 2 ) return false;

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
      if(TString(sampleName).Contains("_TT")  and polarizationFlag != 2) return false;
      if((TString(sampleName).Contains("_TL") or TString(sampleName).Contains("_LT"))  and polarizationFlag != 1) return false;

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



bool passCutContainerSelection (readTree* reader,
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
  if(reader->pfmet_puppi < Cut.MET) return false;  

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
