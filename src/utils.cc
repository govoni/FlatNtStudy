#include "utils.h"
#include <iostream>
using namespace std ;

double 
deltaPhi (double phi1, double phi2)
{
  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


bool closeToLeptons (float eta, float phi, vector<leptonContainer> & TL_leptons, float R)
{
  for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
    {
      if ((eta - TL_leptons.at(iLep).lepton4V_.Eta ()) * (eta - TL_leptons.at(iLep).lepton4V_.Eta ()) +
          deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) * deltaPhi (phi, TL_leptons.at(iLep).lepton4V_.Phi ()) < R * R) 
          return true ;
    }
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


bool closeToLeptons (float eta, float phi, readTree & reader, float R)
{
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
     dummy.btag_     = reader.jetBTagAlgo1 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt2 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt2,reader.jeteta2,reader.jetphi2,reader.jetmass2);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased2 ;
     dummy.btag_     = reader.jetBTagAlgo2 ;
     jetVector.push_back(dummy);
    }
      
    if(reader.jetpt3 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt3,reader.jeteta3,reader.jetphi3,reader.jetmass3);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased3 ;
     dummy.btag_     = reader.jetBTagAlgo3 ;
     jetVector.push_back(dummy);
    }


    if(reader.jetpt4 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt4,reader.jeteta4,reader.jetphi4,reader.jetmass4);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased4 ;
     dummy.btag_     = reader.jetBTagAlgo4 ;
     jetVector.push_back(dummy);
    }
  
    if(reader.jetpt5 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt5,reader.jeteta5,reader.jetphi5,reader.jetmass5);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased5 ;
     dummy.btag_     = reader.jetBTagAlgo5 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt6 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt6,reader.jeteta6,reader.jetphi6,reader.jetmass6);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased6 ;
     dummy.btag_     = reader.jetBTagAlgo6 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt7 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt7,reader.jeteta7,reader.jetphi7,reader.jetmass7);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased7 ;
     dummy.btag_     = reader.jetBTagAlgo7 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt8 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt8,reader.jeteta8,reader.jetphi8,reader.jetmass8);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased8 ;
     dummy.btag_     = reader.jetBTagAlgo8 ;
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
     dummy.btag_     = reader.jetBTagAlgo_puppi1 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi2 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi2,reader.jeteta_puppi2,reader.jetphi_puppi2,reader.jetmass_puppi2);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi2 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi2 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi3 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi3,reader.jeteta_puppi3,reader.jetphi_puppi3,reader.jetmass_puppi3);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi3 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi3 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi4 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi4,reader.jeteta_puppi4,reader.jetphi_puppi4,reader.jetmass_puppi4);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi4 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi4 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi5 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi5,reader.jeteta_puppi5,reader.jetphi_puppi5,reader.jetmass_puppi5);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi5 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi5 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi6 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi6,reader.jeteta_puppi6,reader.jetphi_puppi6,reader.jetmass_puppi6);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi6 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi6 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi7 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi7,reader.jeteta_puppi7,reader.jetphi_puppi7,reader.jetmass_puppi7);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi7 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi7 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetpt_puppi8 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetpt_puppi8,reader.jeteta_puppi8,reader.jetphi_puppi8,reader.jetmass_puppi8);
     dummy.jetPUID_  = reader.jetpileupIDFlagCutBased_puppi8 ;
     dummy.btag_     = reader.jetBTagAlgo_puppi8 ;
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
     jetVector.push_back(dummy);
    }

    if(reader.jetGenpt2 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt2,reader.jetGeneta2,reader.jetGenphi2,reader.jetGenm2);
     dummy.jetPUID_  = -999 ;
     dummy.btag_     = -999 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetGenpt3 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt3,reader.jetGeneta3,reader.jetGenphi3,reader.jetGenm3);
     dummy.jetPUID_  = -999 ;
     dummy.btag_     = -999 ;
     jetVector.push_back(dummy);
    }

    if(reader.jetGenpt4 > 0){
     dummy.jet4V_.SetPtEtaPhiM(reader.jetGenpt4,reader.jetGeneta4,reader.jetGenphi4,reader.jetGenm4);
     dummy.jetPUID_  = -999 ;
     dummy.btag_     = -999 ;
     jetVector.push_back(dummy);
    }

    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


// isolation applied here
void fillRecoLeptonsArray (vector<leptonContainer> & lepVector, readTree & reader)
  {
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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


vector<leptonContainer>  dumpLeptons (vector<leptonContainer> & TL_leptons, float isocut, float minptcut)
  {
    vector<leptonContainer>  goodLeptons ;
    for (size_t i = 0 ; i < TL_leptons.size() ; ++i)
      {
        if (TL_leptons.at(i).iso_  > isocut) continue ;
        if (TL_leptons.at(i).lepton4V_.Pt() < minptcut) continue ;
        goodLeptons.push_back (TL_leptons.at(i)) ;
      }
    return goodLeptons;
  }



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


vector<jetContainer> dumpJets (std::vector<jetContainer> & TL_jets, std::vector<leptonContainer> & TL_leptons, 
                               float minptcut, float btagcut, float jetPUIDCut, float lepminptcut, float deltaR)
  {
    vector<jetContainer> goodJets ;
    for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet)
      {
        if (TL_jets.at(iJet).jet4V_.Pt() < minptcut)   continue ;
        if (TL_jets.at(iJet).btag_       > btagcut)    continue ;
        if (TL_jets.at(iJet).jetPUID_    < jetPUIDCut) continue ;

        bool discard = false ;
        for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
          {
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


void fillTrackJetArray (vector<jetContainer> & jetVector, readTree & reader)
  {
    for( size_t iJet = 0; iJet < reader.kMaxTrackJet_V4 ; iJet++){
      jetContainer dummy;
      dummy.jet4V_.SetPxPyPzE(reader.TrackJet_V4_fP_fX[iJet],reader.TrackJet_V4_fP_fY[iJet],reader.TrackJet_V4_fP_fZ[iJet],reader.TrackJet_V4_fE[iJet]);
      dummy.btag_    = -999 ;
      dummy.jetPUID_ = -999;     
      jetVector.push_back(dummy);
    }
    return ;
  }
               
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


vector<jetContainer> dumpTrackJets (std::vector<jetContainer> & TL_jets, std::vector<leptonContainer> & TL_leptons, 
                    float minptcut, float lepminptcut, float deltaR)
  {
    vector<jetContainer> goodJets ; 
    for (size_t iJet = 0 ; iJet < TL_jets.size() ; ++iJet)
      {
        if (TL_jets.at(iJet).jet4V_.Pt() < minptcut) continue ;
        bool discard = false ;
        for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
          {
            if (TL_leptons.at(iLep).lepton4V_.Pt() < lepminptcut) continue ;
            if (TL_leptons.at(iLep).lepton4V_.DeltaR(TL_jets.at(iJet).jet4V_) < deltaR) 
              {
                discard = true ;
                break ;
              }
          }
        if (discard) continue ;
        goodJets.push_back (TL_jets.at(iJet)) ;        
      }
    return goodJets;
  }


//----------------------
void fillHistos (plotter & analysisPlots, 
                 readTree* reader, 
                 vector<cutContainer> & CutList,
                 vector<variableContainer> & VariableList,
                 const string & sampleName,     const bool & usePuppiAsDefault, 
                 const double & minPtLeptonCut, const double & minPtLeptonCutCleaning,
                 const double & leptonIsoCut,   const double & leptonIsoLooseCut,
                 const double & matchingCone,   const double & minJetCutPt,   
                 map <string,TH1F*> & vect) {

  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader->fChain->GetEntries() ;

  // loop over events                                                                                                                                                       
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){                                                                                                                      

    reader->fChain->GetEntry(iEvent) ;                                                                                                                                      
           
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;                                                                                                  
    
    // skip event with less than two leptons by default                                                                                                                   
           
    if(reader->pt1 < 0          or reader->pt2 < 0)          continue ; // skip the event --> only two reco leptons are good            
    if(reader->jetpt1 < 0       or reader->jetpt2 < 0)       continue ; // skip the event with less than two reco jet                                                     
    if(reader->jetpt_puppi1 < 0 or reader->jetpt_puppi2 < 0) continue ; // skip the event with less than two reco jet                                                     

    // dump all the lepton in the event
    std::vector<leptonContainer> LeptonsAll;
    fillRecoLeptonsArray (LeptonsAll, *reader); 

    // dump tight leptons
    vector<leptonContainer> leptonsIsoTight ;                                                                                                                             
    leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut, minPtLeptonCut);                                                                               

    // identify loose leptons                                                                                                                                             
    vector<leptonContainer> leptonsIsoLoose ;                                                                                                                              
    leptonsIsoLoose = dumpLeptons (LeptonsAll, leptonIsoLooseCut,minPtLeptonCut);                                          

    // take reco jets 
    vector<jetContainer> RecoJetsAll ;                                                                                                                                     
    fillRecoJetArray (RecoJetsAll, *reader) ;                                                                                              

    // Take Puppi jets 
    vector<jetContainer> PuppiJetsAll ;                                                                                                                                    
    fillPuppiJetArray (PuppiJetsAll, *reader) ;                                                                                              

    // take gen jets 
    vector<jetContainer> GenJetsAll ;                                                                                                                                      
    fillGenJetArray (GenJetsAll, *reader) ;                                                                                              

   
    for(size_t iCut = 0; iCut < CutList.size() ; iCut++){ 

      int iBin = 1;
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
      iBin++;   

      // identify tight leptons and require exactly nLep                                                                                                                       
      if (int(leptonsIsoTight.size()) != CutList.at(iCut).nLep ) continue ;                                                                                                    
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
      iBin++;   
      if (leptonsIsoTight.at(0).lepton4V_.Pt() < CutList.at(iCut).ptL.first) continue;
 
      bool badTrailingLepton = false;
      for( size_t iLep = 1 ; iLep < leptonsIsoTight.size(); iLep++){
        if( leptonsIsoTight.at(iLep).lepton4V_.Pt() < CutList.at(iCut).ptL.second) 
        badTrailingLepton = true;
      }                         
      if(badTrailingLepton) continue ;

      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton PT");
      iBin++;   

      int extraLepton = 0; // count the extra lepton number                                                                                                            
            
      for(size_t iLepton = 0; iLepton < leptonsIsoLoose.size() ; iLepton++){                                                                                             
           
       if(leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(0).lepton4V_ or leptonsIsoLoose.at(iLepton).lepton4V_ == leptonsIsoLoose.at(1).lepton4V_) continue; // skip tight ones
       extraLepton++;                                                                                                                                                         
           
      }                                                                                                                                                              
                                                                                                                                                                               
      if(extraLepton > CutList.at(iCut).nextra ) continue; // extra lepton cut                                                                                                 
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"extra lepton");
      iBin++;   

      // apply Zmass veto: if nLep == 2 is a veto (WW analysis) if nLep == 3 is a tag (WZ analysis)
      bool isZMassVeto = false;                                                                                                                                            
      for(size_t iLept = 0; iLept < LeptonsAll.size() ; iLept++){                                                                                                             
       if(LeptonsAll.at(iLept).lepton4V_.Pt() < minPtLeptonCut) continue;
       for(size_t jLept = iLept+1; jLept < LeptonsAll.size() ; jLept++){                                                                                                   
        if(LeptonsAll.at(jLept).lepton4V_.Pt() < minPtLeptonCut) continue; 
        if(fabs(LeptonsAll.at(jLept).flavour_) != fabs(LeptonsAll.at(iLept).flavour_)) continue; 
	if((LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() > CutList.at(iCut).MllZVeto.first and                                                         
	   (LeptonsAll.at(iLept).lepton4V_ + LeptonsAll.at(jLept).lepton4V_).M() < CutList.at(iCut).MllZVeto.second and                                                       
  	    LeptonsAll.at(iLept).charge_*LeptonsAll.at(jLept).charge_ == -1)                                                                                   
	  isZMassVeto = true;                                                                                                                                            
       }
      }                                                                                                                                                                       

      if(isZMassVeto and CutList.at(iCut).nLep == 2) continue ;                                                                                                                      if(not isZMassVeto and CutList.at(iCut).nLep == 3) continue;
                             
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
      vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z veto");
      iBin++;   
                                                                                                                                                                          
     // charge cut for same sign final state : zero means no cut, one means same sign, -1 opposite sign                                                                        
     if(CutList.at(iCut).chargeSign != 0){
       int sign = 1;
       for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns
	 sign *= leptonsIsoTight.at(leptSize).charge_;
       if(sign != CutList.at(iCut).chargeSign) continue ;
     }

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"same sign");
     iBin++;   

     // flavour selection
     if(CutList.at(iCut).flavour != 0){
       int flavour = 0;
       int sameflavour = 0;
       for(size_t leptSize = 0; leptSize < leptonsIsoTight.size(); leptSize++) // loop on tight leptns
	 flavour = fabs(leptonsIsoTight.at(leptSize).flavour_) ;
       if(flavour/leptonsIsoTight.size() == 11 or flavour/leptonsIsoTight.size() == 13 ) sameflavour = 1;
       else sameflavour = -1;
       if(sameflavour != CutList.at(iCut).flavour) continue;
     }

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
     iBin++;   
    
     // take jets
     vector<jetContainer> RecoJets;
     RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);   
     if(!usePuppiAsDefault and RecoJets.size() < 2 ) continue ;

     // take puppi jets
     vector<jetContainer> PuppiJets;
     PuppiJets  = dumpJets (PuppiJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);   
     if(usePuppiAsDefault and PuppiJets.size() < 2 ) continue ;

     // take gen jets
     vector<jetContainer> GenJets;
     GenJets  = dumpJets (GenJetsAll, leptonsIsoTight, minJetCutPt, -999, -999, minPtLeptonCutCleaning, matchingCone);   

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
     iBin++;   

     ///////// Lorentz Vector
     TLorentzVector L_dilepton, L_met, L_gen_met, L_puppi_met ;

     L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;               
     L_met.SetPtEtaPhiM       (reader->pfmet,0.,reader->pfmetphi, 0.) ;                                                                                                      

     L_gen_met.SetPtEtaPhiM   (reader->metGenpt,0.,reader->metGenphi, 0.) ;                                                                                                   
     L_puppi_met.SetPtEtaPhiM (reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;                                                                                        
                                                                                                                                                                           
     TLorentzVector L_dijet, L_dijet_gen, L_dijet_puppi;
     float asimJ = 0, asimL = 0, Rvar = 0, asimPuppiJ = 0, RvarPuppi = 0, asimGenJ = 0, RvarGen = 0;

     asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;      

     if(RecoJets.size() > 2){
      L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;                                                                                                    
      asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;                                        
      Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;                  
     }
    
     if(GenJets.size() > 2){
      L_dijet_gen   = GenJets.at(0).jet4V_ + GenJets.at(1).jet4V_;                                                                                              
      asimGenJ      = (GenJets.at(0).jet4V_.Pt()-GenJets.at(1).jet4V_.Pt())/(GenJets.at(0).jet4V_.Pt()+GenJets.at(1).jet4V_.Pt()) ;                             
      RvarGen       = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(GenJets.at(0).jet4V_.Pt()*GenJets.at(1).jet4V_.Pt()) ;                      
     }

     if(PuppiJets.size() > 2){
       L_dijet_puppi = PuppiJets.at(0).jet4V_ + PuppiJets.at(1).jet4V_ ;                                                                                                 
       asimPuppiJ    = (PuppiJets.at(0).jet4V_.Pt()-PuppiJets.at(1).jet4V_.Pt())/(PuppiJets.at(0).jet4V_.Pt()+PuppiJets.at(1).jet4V_.Pt()) ;                                  
       RvarPuppi     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(PuppiJets.at(0).jet4V_.Pt()*PuppiJets.at(1).jet4V_.Pt()) ;                
       }                                                                                                                                                                   

     // apply Mll cut
     if(L_dilepton.M() < CutList.at(iCut).Mll.first or L_dilepton.M() > CutList.at(iCut).Mll.second) continue ;

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
     iBin++;   

     // apply VBF cuts
     if( usePuppiAsDefault and L_dijet_puppi.M() < CutList.at(iCut).Mjj) continue ;
     else if(not usePuppiAsDefault and L_dijet.M() < CutList.at(iCut).Mjj) continue ;

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
     iBin++;   

     if( usePuppiAsDefault and fabs(PuppiJets.at(0).jet4V_.Eta()-PuppiJets.at(1).jet4V_.Eta()) < CutList.at(iCut).DetaJJ) continue ;
     else if(not usePuppiAsDefault and fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < CutList.at(iCut).DetaJJ) continue ;

     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->SetBinContent(iBin,vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetBinContent(iBin)+1);   
     vect[sampleName+"_"+CutList.at(iCut).cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
     iBin++;   
    
     
     for(size_t iVar = 0; iVar < VariableList.size(); iVar++){
       if(VariableList.at(iVar).variableName == "detajj" and RecoJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),1);   
       }
       else if(VariableList.at(iVar).variableName == "detajj_puppi" and PuppiJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,fabs(PuppiJets.at(0).jet4V_.Eta()-PuppiJets.at(1).jet4V_.Eta()),1);   
       }
       else if(VariableList.at(iVar).variableName == "detajj_gen" and GenJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.Eta()-GenJets.at(1).jet4V_.Eta()),1);   
       }       
       else if(VariableList.at(iVar).variableName == "mjj" and RecoJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet.M(),1);   
       }
       else if(VariableList.at(iVar).variableName == "mjj_gen" and GenJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.M(),1);   
       }
       else if(VariableList.at(iVar).variableName == "mjj_puppi" and PuppiJets.size() >= 2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_puppi.M(),1);   
       }
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LL"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_),1);   
       }       
       else if(VariableList.at(iVar).variableName == "mll"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dilepton.M(),1);   
       }
       else if(VariableList.at(iVar).variableName == "Asim_j"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,asimJ,1);   
       }
       else if(VariableList.at(iVar).variableName == "Asim_j_puppi"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,asimPuppiJ,1);   
       }
       else if(VariableList.at(iVar).variableName == "Asim_j_gen"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,asimGenJ,1);   
       }
       else if(VariableList.at(iVar).variableName == "Asim_l"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,asimL,1);   
       }
       else if(VariableList.at(iVar).variableName == "R"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,Rvar,1);   
       }
       else if(VariableList.at(iVar).variableName == "R_gen"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RvarGen,1);   
       }
       else if(VariableList.at(iVar).variableName == "R_puppi"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RvarPuppi,1);   
       }
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LL"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_),1);   
       }
       
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LMet"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LMet_puppi"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LMet_gen"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_gen_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_LLMet"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dilepton.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LLMet_puppi"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dilepton.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LLMet_gen"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dilepton.DeltaPhi(L_gen_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_TLMet"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_TLMet_puppi"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_TLMet_gen"){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_gen_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ"       and RecoJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_puppi" and PuppiJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,PuppiJets.at(0).jet4V_.DeltaPhi(PuppiJets.at(1).jet4V_),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen"   and GenJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.DeltaPhi(GenJets.at(1).jet4V_),1);   
       } 
       
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met" and GenJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_puppi_Met" and PuppiJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_puppi.DeltaPhi(L_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_Met_puppi" and RecoJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met_puppi" and GenJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_puppi_Met_puppi" and PuppiJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_puppi.DeltaPhi(L_puppi_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_Met_gen" and RecoJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaPhi(L_gen_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met_gen" and GenJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.DeltaPhi(L_gen_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_puppi_Met_gen" and PuppiJets.size()>=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,L_dijet_puppi.DeltaPhi(L_gen_met),1);   
       } 


       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size()>=1){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_puppi_Met" and PuppiJets.size()>=1){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,PuppiJets.at(0).jet4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_gen_Met" and GenJets.size()>=1){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.DeltaPhi(L_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJMet_puppi" and RecoJets.size()>=1){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_puppi_Met_puppi" and PuppiJets.size()>=1){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,PuppiJets.at(0).jet4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_gen_Met_puppi" and GenJets.size()>=1){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.DeltaPhi(L_puppi_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_TJ_puppi_Met" and PuppiJets.size() >=2){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,PuppiJets.at(1).jet4V_.DeltaPhi(L_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met" and GenJets.size() >=2){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.DeltaPhi(L_met),1);   
       } 

       else if(VariableList.at(iVar).variableName == "DeltaPhi_TJMet_puppi" and RecoJets.size() >=2){
	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_TJ_puppi_Met_puppi" and PuppiJets.size() >=2){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,PuppiJets.at(1).jet4V_.DeltaPhi(L_puppi_met),1);   
       } 
       else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met_puppi" and GenJets.size() >=2){
 	 analysisPlots.fillHisto (sampleName,CutList.at(iCut).cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.DeltaPhi(L_puppi_met),1);   
       }
       
     }                
    }
  }
}
