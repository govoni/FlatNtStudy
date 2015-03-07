#include "utilsLatino.h"
#include "utils.h"



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

bool closeToLeptons (float eta, float phi, vector<leptonContainerLatino> & TL_leptons, float R){
  for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep){
    if ((eta - TL_leptons.at(iLep).lepton4V_.Eta ()) * (eta - TL_leptons.at(iLep).lepton4V_.Eta ()) +
	deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) * deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) < R * R) 
      return true ;
  }
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


bool closeToLeptons (float eta, float phi, readTreeLatino & reader, float R){

  if ((eta - reader.eta1) * (eta - reader.eta1) +
      deltaPhi (phi, reader.phi1) * deltaPhi (phi, reader.phi1) < R * R) return true ;
  if ((eta - reader.eta2) * (eta - reader.eta2) +
      deltaPhi (phi, reader.phi2) * deltaPhi (phi, reader.phi2) < R * R) return true ;
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillRecoJetArray (vector<jetContainerLatino> & jetVector, readTreeLatino & reader) {

  jetContainerLatino dummy;
  if(reader.jetpt1 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt1,reader.jeteta1,reader.jetphi1,reader.jetmass1);
    dummy.jetPUID_  = reader.jetpileupidFlag1 ;
    dummy.btagCSV_     = reader.jetcsv1 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt2 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt2,reader.jeteta2,reader.jetphi2,reader.jetmass2);
    dummy.jetPUID_  = reader.jetpileupidFlag2 ;
    dummy.btagCSV_     = reader.jetcsv2 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt3 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt3,reader.jeteta3,reader.jetphi3,reader.jetmass3);
    dummy.jetPUID_  = reader.jetpileupidFlag3 ;
    dummy.btagCSV_     = reader.jetcsv3 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt4 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt4,reader.jeteta4,reader.jetphi4,reader.jetmass4);
    dummy.jetPUID_  = reader.jetpileupidFlag4 ;
    dummy.btagCSV_     = reader.jetcsv4 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt5 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt5,reader.jeteta5,reader.jetphi5,reader.jetmass5);
    dummy.jetPUID_  = reader.jetpileupidFlag5 ;
    dummy.btagCSV_     = reader.jetcsv5 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt6 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt6,reader.jeteta6,reader.jetphi6,reader.jetmass6);
    dummy.jetPUID_  = reader.jetpileupidFlag6 ;
    dummy.btagCSV_     = reader.jetcsv6 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt7 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt7,reader.jeteta7,reader.jetphi7,reader.jetmass7);
    dummy.jetPUID_  = reader.jetpileupidFlag7 ;
    dummy.btagCSV_     = reader.jetcsv7 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt8 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt8,reader.jeteta8,reader.jetphi8,reader.jetmass8);
    dummy.jetPUID_  = reader.jetpileupidFlag8 ;
    dummy.btagCSV_     = reader.jetcsv8 ;
    jetVector.push_back(dummy);
  }

    
  return ;
}



// 
void fillPuppiJetArray (vector<jetContainerLatino> & jetVector, readTreeLatino & reader) {

  jetContainerLatino dummy;
  if(reader.jetpt_puppi1 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi1,reader.jeteta_puppi1,reader.jetphi_puppi1,reader.jetmass_puppi1);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi1 ;
    dummy.btagCSV_     = reader.jetcsv_puppi1 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi2 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi2,reader.jeteta_puppi2,reader.jetphi_puppi2,reader.jetmass_puppi2);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi2 ;
    dummy.btagCSV_     = reader.jetcsv_puppi2 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi3 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi3,reader.jeteta_puppi3,reader.jetphi_puppi3,reader.jetmass_puppi3);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi3 ;
    dummy.btagCSV_     = reader.jetcsv_puppi3 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi4 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi4,reader.jeteta_puppi4,reader.jetphi_puppi4,reader.jetmass_puppi4);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi4 ;
    dummy.btagCSV_     = reader.jetcsv_puppi4 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi5 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi5,reader.jeteta_puppi5,reader.jetphi_puppi5,reader.jetmass_puppi5);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi5 ;
    dummy.btagCSV_     = reader.jetcsv_puppi5 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi6 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi6,reader.jeteta_puppi6,reader.jetphi_puppi6,reader.jetmass_puppi6);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi6 ;
    dummy.btagCSV_     = reader.jetcsv_puppi6 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi7 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi7,reader.jeteta_puppi7,reader.jetphi_puppi7,reader.jetmass_puppi7);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi7 ;
    dummy.btagCSV_     = reader.jetcsv_puppi7 ;
    jetVector.push_back(dummy);
  }

  if(reader.jetpt_puppi8 > 0){
    dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi8,reader.jeteta_puppi8,reader.jetphi_puppi8,reader.jetmass_puppi8);
    dummy.jetPUID_  = reader.jetpileupidFlag_puppi8 ;
    dummy.btagCSV_     = reader.jetcsv_puppi8 ;
    jetVector.push_back(dummy);
  }
    
  return ;
}

void fillGenJetArray   (vector<genJetContainerLatino> & jetVector,
                        readTreeLatino & reader) {


  genJetContainerLatino dummy;

  if(reader.jetGenpt1 > 0){
    dummy.pt_  = reader.jetGenpt1;
    dummy.eta_ = reader.jetGeneta1;
    dummy.phi_ = reader.jetGenphi1;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt2 > 0){
    dummy.pt_  = reader.jetGenpt2;
    dummy.eta_ = reader.jetGeneta2;
    dummy.phi_ = reader.jetGenphi2;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt3 > 0){
    dummy.pt_  = reader.jetGenpt3;
    dummy.eta_ = reader.jetGeneta3;
    dummy.phi_ = reader.jetGenphi3;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt4 > 0){
    dummy.pt_  = reader.jetGenpt4;
    dummy.eta_ = reader.jetGeneta4;
    dummy.phi_ = reader.jetGenphi4;
    jetVector.push_back(dummy);
  }

  if(reader.jetGenpt5 > 0){
    dummy.pt_  = reader.jetGenpt5;
    dummy.eta_ = reader.jetGeneta5;
    dummy.phi_ = reader.jetGenphi5;
    jetVector.push_back(dummy);
  }

  return ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

// isolation applied here
void fillRecoLeptonsArray (vector<leptonContainerLatino> & lepVector, readTreeLatino & reader){

  leptonContainerLatino dummy;

  if(reader.pt1 > 0 ){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt1,reader.eta1,reader.phi1,0);    
    dummy.charge_   = reader.pid1 > 0 ? 1 : -1 ;
    dummy.flavour_  = reader.pid1 ;
    lepVector.push_back(dummy);
  }

  if(reader.pt2 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt2,reader.eta2,reader.phi2,0);
    dummy.charge_   = reader.pid2 > 0 ? +1 : -1 ;
    dummy.flavour_  = reader.pid2 ;
    lepVector.push_back(dummy);
  }
   
  if(reader.pt3 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt3,reader.eta3,reader.phi3,0);
    dummy.charge_   = reader.pid3 > 0 ? +1 : -1 ;
    dummy.flavour_  = reader.pid3 ;
    lepVector.push_back(dummy);
  }

  if(reader.pt4 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.pt4,reader.eta4,reader.phi4,0);
    dummy.charge_   = reader.pid4 > 0 ? +1 : -1 ;
    dummy.flavour_  = reader.pid4 ;
    lepVector.push_back(dummy);
  }

  return ;
}

// isolation applied here
void fillGenLeptonsArray (vector<leptonContainerLatino> & lepVector, readTreeLatino & reader){

  leptonContainerLatino dummy;

  if(reader.leptonGenpt1 > 0 ){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt1,reader.leptonGeneta1,reader.leptonGenphi1,0);    
    dummy.charge_   = reader.leptonGenpid1 > 0 ? 1 : -1 ;
    dummy.flavour_  = reader.leptonGenpid1 ;
    lepVector.push_back(dummy);
  }

  if(reader.leptonGenpt2 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt2,reader.leptonGeneta2,reader.leptonGenphi2,0);
    dummy.charge_   = reader.leptonGenpid2 > 0 ? +1 : -1 ;
    dummy.flavour_  = reader.leptonGenpid2 ;
    lepVector.push_back(dummy);
  }
   
  if(reader.leptonGenpt3 > 0){
    dummy.lepton4V_.SetPtEtaPhiM(reader.leptonGenpt3,reader.leptonGeneta3,reader.leptonGenphi3,0);
    dummy.charge_   = reader.leptonGenpid3 > 0 ? +1 : -1 ;
    dummy.flavour_  = reader.leptonGenpid3 ;
    lepVector.push_back(dummy);
  }


  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


vector<leptonContainerLatino>  dumpLeptons (vector<leptonContainerLatino> & TL_leptons, 
					    vector<leptonContainerLatino> & genLeptons, 
					    float minptcut,
					    float dRmatching){
  vector<leptonContainerLatino>  goodLeptons ;
  for (size_t i = 0 ; i < TL_leptons.size() ; ++i){
    if (TL_leptons.at(i).lepton4V_.Pt() < minptcut) continue ;
    if (fabs(TL_leptons.at(i).flavour_) == 11 and (fabs(TL_leptons.at(i).lepton4V_.Eta()) > 1.45 and fabs(TL_leptons.at(i).lepton4V_.Eta()) < 1.55)) continue ;
    bool isMatched = false;
    for( size_t j = 0; j < genLeptons.size(); j++){
      if(genLeptons.at(j).lepton4V_.DeltaR(TL_leptons.at(i).lepton4V_) < dRmatching){
	isMatched = true;
	break;
      }
    }
    if (isMatched)
      goodLeptons.push_back (TL_leptons.at(i)) ;
  }
  return goodLeptons;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -

vector<jetContainerLatino> dumpJets (vector<jetContainerLatino> & TL_jets, 
				     vector<leptonContainerLatino> & TL_leptons, 
				     float minptcut, float btagcut, float jetPUIDCut, 
				     float lepminptcut, float deltaR, float etaMax){

  vector<jetContainerLatino> goodJets ;
  for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet){
    if (TL_jets.at(iJet).jet4V_.Pt()    < minptcut)   continue ;
    if (TL_jets.at(iJet).btagCSV_       > btagcut)    continue ;
    if (TL_jets.at(iJet).jetPUID_       < jetPUIDCut) continue ;
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
    goodJets.push_back(TL_jets.at(iJet));        
  }
  return goodJets ;
}
               


vector<genJetContainerLatino> dumpJets (vector<genJetContainerLatino> & TL_jets, 
					vector<leptonContainerLatino> & TL_leptons, 
					float minptcut, float btagcut, float jetPUIDCut, 
					float lepminptcut, float deltaR, float etaMax){

  vector<genJetContainerLatino> goodJets ;
  for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet){
    if (TL_jets.at(iJet).pt_    < minptcut)   continue ;
    if (fabs(TL_jets.at(iJet).eta_) > etaMax)     continue ;
    
    bool discard = false ;
    for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep){
      if (TL_leptons.at(iLep).lepton4V_.Pt () < lepminptcut) continue ;
      if (sqrt((TL_leptons.at(iLep).lepton4V_.Eta()-TL_jets.at(iJet).eta_)*(TL_leptons.at(iLep).lepton4V_.Eta()-TL_jets.at(iJet).eta_)+
	       deltaPhi(TL_leptons.at(iLep).lepton4V_.Phi(),TL_jets.at(iJet).phi_)*deltaPhi(TL_leptons.at(iLep).lepton4V_.Phi(),TL_jets.at(iJet).phi_)) < deltaR){
	discard = true ;
	break ;
      }
    }
    if (discard) continue ;
    goodJets.push_back(TL_jets.at(iJet));
  }
  return goodJets ;
}
               

               
