#include "utils.h"
#include "kinematicUtils.h"
#include <memory>

void neutrinoP4(TLorentzVector fmet, TLorentzVector flep, int lepType, double& fmetp4_0, double& fmetp4_1, double& fmetp4_2, double& fmetp4_3 );


void fillHistoWZ( plotter & analysisPlots,
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
    TLorentzVector jet1, jet2;
    TLorentzVector L_trilepton, L_LLLmet;
    
    
    L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
    L_trilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ + leptonsIsoTight.at(2).lepton4V_ ;
    L_LLmet = L_dilepton + L_met ;
    L_LLLmet = L_trilepton + L_met ;
    
    float ASimJJ_ = -999, asimGenJ = -999;
    
    
    if(RecoJets.size() >= 2){
        jet1 = RecoJets.at(0).jet4V_;
        jet2 = RecoJets.at(1).jet4V_;
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        ASimJJ_    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    }
    
    if(GenJets.size() >= 2){
        L_dijet_gen   = GenJets.at(0).jet4V_ + GenJets.at(1).jet4V_;
        asimGenJ      = (GenJets.at(0).jet4V_.Pt()-GenJets.at(1).jet4V_.Pt())/(GenJets.at(0).jet4V_.Pt()+GenJets.at(1).jet4V_.Pt()) ;
    }
    
    
    float deltaPhiLL_Z_ = -999, deltaEtaLL_Z_ = -999, deltaRLL_Z_ = -999, ASimLL_Z_ = -999, RLL_Z_ = -999;
    float deltaPhiLL_SC_ = -999, deltaEtaLL_SC_ = -999, deltaRLL_SC_ = -999, ASimLL_SC_ = -999, RLL_SC_ = -999;
    double costheta1_ = -999, costheta2_ = -999 , costhetastar_ = -999, pol_phi_ = -999, pol_phi1_ = -999;
    double costheta1_vbf_ = -999, costheta2_vbf_ = -999 , costhetastar_vbf_ = -999, pol_phi_vbf_ = -999, pol_phi1_vbf_ = -999;
    float deltaEtaWZ_ = -999, deltaPhiWZ_ = -999, deltaEtaZl3_ = -999 , deltaPhiZl3_ = -999;
    
    bool mZ1 = false;
    bool mZ2 = false;
    bool mZ3 = false;
    
    double f_met0 = -99;  double f_met1 = -99;  double f_met2 = -99; double f_met3 = -99 ;
    
    TLorentzVector p4_W, p4_Z, p4_WZ;
    TLorentzVector p4_lep1Z, p4_lep2Z, p4_lep1W;
    TLorentzVector p4_lep[3];
    TLorentzVector p4_met, met;
    
    
    if(leptonsIsoTight.size()>=3){
        
        
        met.SetPtEtaPhiM(L_met.Pt(),0,L_met.Phi(),0);
        
        
        for(int il=0; il<3; il++){
            p4_lep[il].SetPtEtaPhiM(leptonsIsoTight.at(il).lepton4V_.Pt(),leptonsIsoTight.at(il).lepton4V_.Eta(), leptonsIsoTight.at(il).lepton4V_.Phi(),0);
        }
        
        
        Int_t ch_lep1Z = 0, ch_lep2Z = 0, ch_lep1W = 0;
        Int_t pid_lep1Z = 0, pid_lep2Z = 0, pid_lep1W = 0;
        double mass1=-999; double mass2 = -999; double mass3 = -999;
        
        if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(1).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(1).charge_) == -1) ){
            if ( fabs( (p4_lep[0] + p4_lep[1]).M() - 91) < 20  ){
                mass1 = (p4_lep[0] + p4_lep[1]).M();
                mZ1 = true;
            }
        }
        if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
            if ( fabs((p4_lep[0] + p4_lep[2]).M()- 91) < 20  ){
                mass2 = (p4_lep[0] + p4_lep[2]).M();
                mZ2 = true;
            }
        }
        if( ( abs(leptonsIsoTight.at(1).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(1).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
            if ( fabs((p4_lep[1] + p4_lep[2]).M()- 91) < 20  ){
                mass3 = (p4_lep[1] + p4_lep[2]).M();
                mZ3 = true;
            }
        }
        
            //to make lepton pairs from Z
        
        if( mZ1 && mZ2){
            if( abs(mass1 - 91) < abs(mass2 -91) ) mZ2 = false;
            else mZ1 = false;
        }
        if( mZ1 && mZ3){
            if( abs(mass1 - 91) < abs(mass3 -91) ) mZ3 = false;
            else mZ1 = false;
        }
        if( mZ2 && mZ3){
            if( abs(mass2 - 91) < abs(mass3 -91) ) mZ3 = false;
            else mZ2 = false;
        }
        
            //if( (!mZ1 && !mZ2) && !mZ3) continue;
        if( mZ1 || mZ2 || mZ3) {
            if(mZ1){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                p4_lep1W.SetPtEtaPhiE( p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(1).charge_;    ch_lep1W = leptonsIsoTight.at(2).charge_;
                pid_lep1Z = leptonsIsoTight.at(0).charge_;  pid_lep2Z = leptonsIsoTight.at(1).charge_;   pid_lep1W = leptonsIsoTight.at(2).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
            }
            
            if(mZ2){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                p4_lep1W.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(1).charge_;
                pid_lep1Z = leptonsIsoTight.at(0).charge_;  pid_lep2Z = leptonsIsoTight.at(2).charge_;   pid_lep1W = leptonsIsoTight.at(1).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
                
            }
            if(mZ3){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                p4_lep1W.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());;
                ch_lep1Z = leptonsIsoTight.at(1).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(0).charge_;
                pid_lep1Z = leptonsIsoTight.at(1).charge_;  pid_lep2Z = leptonsIsoTight.at(2).charge_;   pid_lep1W = leptonsIsoTight.at(0).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
            }
            
            
            deltaPhiLL_Z_ = deltaPhi(p4_lep1Z.Phi(),p4_lep2Z.Phi());
            deltaEtaLL_Z_ = abs(p4_lep1Z.Eta() - p4_lep2Z.Eta());
            deltaRLL_Z_ = p4_lep1Z.DeltaR(p4_lep1Z);
            
            ASimLL_Z_ = (fabs(p4_lep1Z.Pt() - p4_lep2Z.Pt()))/(p4_lep1Z.Pt() + p4_lep2Z.Pt());
            RLL_Z_ = (p4_lep1Z.Pt() * p4_lep2Z.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
            
            p4_Z = p4_lep1Z + p4_lep2Z ;
            
            deltaEtaZl3_ = fabs( p4_Z.Eta() - p4_lep1W.Eta() );
            deltaPhiZl3_ = deltaPhi(p4_Z.Phi(), p4_lep1W.Phi());
            
            p4_W = p4_lep1W + p4_met ;
            p4_WZ = p4_W + p4_Z;
            deltaEtaWZ_ = fabs( p4_W.Eta() - p4_Z.Eta() );
            deltaPhiWZ_ = deltaPhi(p4_W.Phi() , p4_Z.Phi()  );
                //do all the angles!!
            computeAnglesResonance( p4_WZ,  p4_Z, p4_lep1Z,  p4_lep2Z, p4_W, p4_lep1W, p4_met,costheta1_, costheta2_, pol_phi_, costhetastar_, pol_phi1_);
            computeAnglesVBF( p4_WZ,  p4_Z, p4_lep1Z,  p4_lep2Z, p4_W, p4_lep1W, p4_met, jet1, jet2, costheta1_vbf_, costheta2_vbf_, pol_phi_vbf_, costhetastar_vbf_, pol_phi1_vbf_);
            
            
            if( ch_lep1Z * ch_lep1W == 1){
                deltaPhiLL_SC_ = deltaPhi(p4_lep1Z.Phi(),p4_lep1W.Phi());
                deltaEtaLL_SC_ = abs(p4_lep1Z.Eta() - p4_lep1W.Eta());
                deltaRLL_SC_ = p4_lep1Z.DeltaR(p4_lep1W);
                
                ASimLL_SC_ = (fabs(p4_lep1Z.Pt() - p4_lep1W.Pt()))/(p4_lep1Z.Pt() + p4_lep1W.Pt());
                RLL_SC_ = (p4_lep1Z.Pt() * p4_lep1W.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
            }
            
            if( ch_lep2Z * ch_lep1W == 1){
                deltaPhiLL_SC_ = deltaPhi(p4_lep2Z.Phi(),p4_lep1W.Phi());
                deltaEtaLL_SC_ = abs(p4_lep2Z.Eta() - p4_lep1W.Eta());
                deltaRLL_SC_ = p4_lep2Z.DeltaR(p4_lep1W);
                ASimLL_SC_ = (fabs(p4_lep2Z.Pt() - p4_lep1W.Pt()))/(p4_lep2Z.Pt() + p4_lep1W.Pt());
                RLL_SC_ = (p4_lep2Z.Pt() * p4_lep1W.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
                
            }
            
        }// if mz1 mz2 mz3
        
    } //if leptons > 3
    
    TString Name ;
    string  NameSample ;
    if(TString(sampleName).Contains("Madgraph_")){
        Name = sampleName;
        Name.ReplaceAll("Madgraph_","");
        NameSample = string(Name) ;
    }
    else
    NameSample = sampleName ;
    
    
    
    
    for(size_t iVar = 0; iVar < VariableList.size(); iVar++){
        
            // chhange from here
        
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
        
            // jet based variables
        if(VariableList.at(iVar).variableName == "ptj1" and RecoJets.size() >= 1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptj1_gen" and GenJets.size() >= 1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptj2" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptj2_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
        }
        
        
        else if(VariableList.at(iVar).variableName == "etaj1" and RecoJets.size() >= 1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etaj1_gen" and GenJets.size() >= 1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etaj2" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etaj2_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "detajj" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "detajj_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.Eta()-GenJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "dRjj" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(RecoJets.at(1).jet4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dRjj_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.DeltaR(GenJets.at(1).jet4V_),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "mjj" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.M(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mjj_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.M(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptjj" and RecoJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptjj_gen" and GenJets.size() >= 2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet_gen.Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,ASimJJ_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "Asim_j_gen" and GenJets.size() >=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,asimGenJ,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ"       and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen"   and GenJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.DeltaPhi(GenJets.at(1).jet4V_)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "etaj1etaj2"       and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etaj1etaj2_gen"       and GenJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,GenJets.at(0).jet4V_.Eta()*GenJets.at(1).jet4V_.Eta(),eventFakeWeight,systematicName);
        }
        
            // lepton based quantities
        
        else if(VariableList.at(iVar).variableName == "ptl1"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptl2"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptl3"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "etal1"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etal2"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(1).lepton4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etal3"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(2).lepton4V_.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptlll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_trilepton.Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "phill"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Phi(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "philll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_trilepton.Phi(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dRll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etall"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dilepton.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "etalll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_trilepton.Eta(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dilepton.M(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mlll"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_trilepton.M(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "etal1etal2"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,leptonsIsoTight.at(0).lepton4V_.Eta()*leptonsIsoTight.at(1).lepton4V_.Eta(),eventFakeWeight,systematicName);
        }
            // WZ OBJECTS
        else if(VariableList.at(iVar).variableName == "mTwz"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_WZ.Mt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mwz"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_WZ.M(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mTw"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_W.Mt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mw"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_W.Mt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mTz"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_Z.Mt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mz"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,p4_Z.M(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ASimJJ"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,ASimJJ_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ASimLL_Z"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,ASimLL_Z_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ASimLL_SC"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,ASimLL_SC_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "deltaPhiLL_Z"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaPhiLL_Z_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "deltaPhiLL_SC"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaPhiLL_SC_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "deltaEtaLL_Z"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaEtaLL_Z_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "deltaEtaLL_SC"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaEtaLL_SC_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "deltaRLL_Z"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaRLL_Z_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "deltaRLL_SC"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,deltaRLL_SC_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "RLL_Z"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RLL_Z_,eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "RLL_SC"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RLL_SC_,eventFakeWeight,systematicName);
        }
        
            //angle objects
        else if(VariableList.at(iVar).variableName == "costheta1"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costheta1_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "costheta2"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costheta2_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "costhetastar"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costhetastar_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "pol_phi"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,pol_phi_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "pol_phi1"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,pol_phi1_,eventFakeWeight,systematicName);
        }
            //
        else if(VariableList.at(iVar).variableName == "costheta1_vbf"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costheta1_vbf_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "costheta2_vbf"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costheta2_vbf_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "costhetastar_vbf"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,costhetastar_vbf_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "pol_phi_vbf"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,pol_phi_vbf_,eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "pol_phi1_vbf"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,pol_phi1_vbf_,eventFakeWeight,systematicName);
        }
            // met
        else if(VariableList.at(iVar).variableName == "met"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_met.Pt(),eventFakeWeight,systematicName);
        }
        
        
            // lepton and met
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(leptonsIsoTight.at(0).lepton4V_+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptTLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(leptonsIsoTight.at(1).lepton4V_+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LLLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_trilepton.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptLLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_met).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptLLLMet"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
            // lepton and jets
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_trilepton)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_LJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_LJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_LJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(L_dilepton),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_LJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(0).jet4V_.DeltaR(L_trilepton),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptLJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptLJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptLJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+L_dilepton).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptLJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+L_trilepton).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_trilepton)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "dR_TJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_TJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_TJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(L_dilepton),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_TJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,RecoJets.at(1).jet4V_.DeltaR(L_trilepton),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptTJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptTJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptTJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+L_dilepton).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptTJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+L_trilepton).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(leptonsIsoTight.at(0).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_dilepton)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_trilepton)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(leptonsIsoTight.at(0).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(leptonsIsoTight.at(1).lepton4V_),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_dilepton),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_trilepton),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptJJL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+leptonsIsoTight.at(0).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptJJTL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+leptonsIsoTight.at(1).lepton4V_).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "ptJJLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_dilepton).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptJJLLL" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_trilepton).Pt(),eventFakeWeight,systematicName);
        }
        
        
            // jet and met
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptJJMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_gen_Met" and GenJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet_gen.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size()>=1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptLJMet" and RecoJets.size()>=1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(0).jet4V_+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_LJ_gen_Met" and GenJets.size()>=1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(GenJets.at(0).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met)),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "ptTJMet" and RecoJets.size()>=1){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(RecoJets.at(1).jet4V_+L_met).Pt(),eventFakeWeight,systematicName);
        }
        
            // Lepton JJ_LLMET
        else if(VariableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_LLmet).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_LLmet)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_LLmet),eventFakeWeight,systematicName);
        }
            // Lepton JJ_LLLMET
        else if(VariableList.at(iVar).variableName == "ptJJ_LLLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dijet+L_LLLmet).Pt(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "DeltaPhi_JJ_LLLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,fabs(L_dijet.DeltaPhi(L_LLLmet)),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "dR_JJ_LLLMet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,L_dijet.DeltaR(L_LLLmet),eventFakeWeight,systematicName);
        }
        
            // some invariant mass
        else if(VariableList.at(iVar).variableName == "mlljj" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_dijet).M(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "mlljjmet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_dilepton+L_dijet+L_met).M(),eventFakeWeight,systematicName);
        }
        else if(VariableList.at(iVar).variableName == "mllljj" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_trilepton+L_dijet).M(),eventFakeWeight,systematicName);
        }
        
        else if(VariableList.at(iVar).variableName == "mllljjmet" and RecoJets.size()>=2){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,(L_trilepton+L_dijet+L_met).M(),eventFakeWeight,systematicName);
        }
        
            // tranvserse mass
        else if(VariableList.at(iVar).variableName == "mTH"){
            analysisPlots.fillHisto (NameSample, samplePosition, cutLayerName,VariableList.at(iVar).variableName,sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
        }
    }
}





    // 2D histograms
void fillHisto2DWZ ( plotter & analysisPlots,
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
    
    
    TLorentzVector L_dilepton, L_LLmet;
    TLorentzVector L_dijet, L_dijet_gen;
    TLorentzVector jet1, jet2;
    TLorentzVector L_trilepton, L_LLLmet;
    
    
    L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
    L_trilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ + leptonsIsoTight.at(2).lepton4V_ ;
    L_LLmet = L_dilepton + L_met ;
    L_LLLmet = L_trilepton + L_met ;
    
    float ASimJJ_ = -999, asimGenJ = -999;
    
    
    if(RecoJets.size() >= 2){
        jet1 = RecoJets.at(0).jet4V_;
        jet2 = RecoJets.at(1).jet4V_;
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        ASimJJ_    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    }
    
    if(GenJets.size() >= 2){
        L_dijet_gen   = GenJets.at(0).jet4V_ + GenJets.at(1).jet4V_;
        asimGenJ      = (GenJets.at(0).jet4V_.Pt()-GenJets.at(1).jet4V_.Pt())/(GenJets.at(0).jet4V_.Pt()+GenJets.at(1).jet4V_.Pt()) ;
    }
    
    
    float deltaPhiLL_Z_ = -999, deltaEtaLL_Z_ = -999, deltaRLL_Z_ = -999, ASimLL_Z_ = -999, RLL_Z_ = -999;
    float deltaPhiLL_SC_ = -999, deltaEtaLL_SC_ = -999, deltaRLL_SC_ = -999, ASimLL_SC_ = -999, RLL_SC_ = -999;
    double costheta1_ = -999, costheta2_ = -999 , costhetastar_ = -999, pol_phi_ = -999, pol_phi1_ = -999;
    double costheta1_vbf_ = -999, costheta2_vbf_ = -999 , costhetastar_vbf_ = -999, pol_phi_vbf_ = -999, pol_phi1_vbf_ = -999;
    float deltaEtaWZ_ = -999, deltaPhiWZ_ = -999, deltaEtaZl3_ = -999 , deltaPhiZl3_ = -999;
    
    bool mZ1 = false;
    bool mZ2 = false;
    bool mZ3 = false;
    
    double f_met0 = -99;  double f_met1 = -99;  double f_met2 = -99; double f_met3 = -99 ;
    
    TLorentzVector p4_W, p4_Z, p4_WZ;
    TLorentzVector p4_lep1Z, p4_lep2Z, p4_lep1W;
    TLorentzVector p4_lep[3];
    TLorentzVector p4_met, met;
    
    if(leptonsIsoTight.size()>=3){
        
        met.SetPtEtaPhiM(L_met.Pt(),0,L_met.Phi(),0);
        
        
        for(int il=0; il<3; il++){
            p4_lep[il].SetPtEtaPhiM(leptonsIsoTight.at(il).lepton4V_.Pt(),leptonsIsoTight.at(il).lepton4V_.Eta(), leptonsIsoTight.at(il).lepton4V_.Phi(),0);
        }
        
        
        Int_t ch_lep1Z = 0, ch_lep2Z = 0, ch_lep1W = 0;
        Int_t pid_lep1Z = 0, pid_lep2Z = 0, pid_lep1W = 0;
        double mass1=-999; double mass2 = -999; double mass3 = -999;
        
        if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(1).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(1).charge_) == -1) ){
            if ( fabs( (p4_lep[0] + p4_lep[1]).M() - 91) < 20  ){
                mass1 = (p4_lep[0] + p4_lep[1]).M();
                mZ1 = true;
            }
        }
        if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
            if ( fabs((p4_lep[0] + p4_lep[2]).M()- 91) < 20  ){
                mass2 = (p4_lep[0] + p4_lep[2]).M();
                mZ2 = true;
            }
        }
        if( ( abs(leptonsIsoTight.at(1).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(1).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
            if ( fabs((p4_lep[1] + p4_lep[2]).M()- 91) < 20  ){
                mass3 = (p4_lep[1] + p4_lep[2]).M();
                mZ3 = true;
            }
        }
        
            //to make lepton pairs from Z
        
        if( mZ1 && mZ2){
            if( abs(mass1 - 91) < abs(mass2 -91) ) mZ2 = false;
            else mZ1 = false;
        }
        if( mZ1 && mZ3){
            if( abs(mass1 - 91) < abs(mass3 -91) ) mZ3 = false;
            else mZ1 = false;
        }
        if( mZ2 && mZ3){
            if( abs(mass2 - 91) < abs(mass3 -91) ) mZ3 = false;
            else mZ2 = false;
        }
        
            //if( (!mZ1 && !mZ2) && !mZ3) continue;
        if( mZ1 || mZ2 || mZ3) {
            if(mZ1){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                p4_lep1W.SetPtEtaPhiE( p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(1).charge_;    ch_lep1W = leptonsIsoTight.at(2).charge_;
                pid_lep1Z = leptonsIsoTight.at(0).charge_;  pid_lep2Z = leptonsIsoTight.at(1).charge_;   pid_lep1W = leptonsIsoTight.at(2).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
            }
            
            if(mZ2){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                p4_lep1W.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(1).charge_;
                pid_lep1Z = leptonsIsoTight.at(0).charge_;  pid_lep2Z = leptonsIsoTight.at(2).charge_;   pid_lep1W = leptonsIsoTight.at(1).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
                
            }
            if(mZ3){
                p4_lep1Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
                p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
                p4_lep1W.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());;
                ch_lep1Z = leptonsIsoTight.at(1).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(0).charge_;
                pid_lep1Z = leptonsIsoTight.at(1).charge_;  pid_lep2Z = leptonsIsoTight.at(2).charge_;   pid_lep1W = leptonsIsoTight.at(0).charge_;
                neutrinoP4(met, p4_lep1W , pid_lep1W , f_met0, f_met1, f_met2, f_met3);
                p4_met.SetPxPyPzE(f_met0, f_met1, f_met2, f_met3);
            }
            
            
            deltaPhiLL_Z_ = deltaPhi(p4_lep1Z.Phi(),p4_lep2Z.Phi());
            deltaEtaLL_Z_ = abs(p4_lep1Z.Eta() - p4_lep2Z.Eta());
            deltaRLL_Z_ = p4_lep1Z.DeltaR(p4_lep2Z);
            
            ASimLL_Z_ = (fabs(p4_lep1Z.Pt() - p4_lep2Z.Pt()))/(p4_lep1Z.Pt() + p4_lep2Z.Pt());
            RLL_Z_ = (p4_lep1Z.Pt() * p4_lep2Z.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
            
            p4_Z = p4_lep1Z + p4_lep2Z ;
            
            deltaEtaZl3_ = fabs( p4_Z.Eta() - p4_lep1W.Eta() );
            deltaPhiZl3_ = deltaPhi(p4_Z.Phi(), p4_lep1W.Phi());
            
            p4_W = p4_lep1W + p4_met ;
            p4_WZ = p4_W + p4_Z;
            deltaEtaWZ_ = fabs( p4_W.Eta() - p4_Z.Eta() );
            deltaPhiWZ_ = deltaPhi(p4_W.Phi() , p4_Z.Phi()  );
                //do all the angles!!
            computeAnglesResonance( p4_WZ,  p4_Z, p4_lep1Z,  p4_lep2Z, p4_W, p4_lep1W, p4_met,costheta1_, costheta2_, pol_phi_, costhetastar_, pol_phi1_);
            computeAnglesVBF( p4_WZ,  p4_Z, p4_lep1Z,  p4_lep2Z, p4_W, p4_lep1W, p4_met, jet1, jet2, costheta1_vbf_, costheta2_vbf_, pol_phi_vbf_, costhetastar_vbf_, pol_phi1_vbf_);
            
            
            if( ch_lep1Z * ch_lep1W == 1){
                deltaPhiLL_SC_ = deltaPhi(p4_lep1Z.Phi(),p4_lep1W.Phi());
                deltaEtaLL_SC_ = abs(p4_lep1Z.Eta() - p4_lep1W.Eta());
                deltaRLL_SC_ = p4_lep1Z.DeltaR(p4_lep1W);
                ASimLL_SC_ = (fabs(p4_lep1Z.Pt() - p4_lep1W.Pt()))/(p4_lep1Z.Pt() + p4_lep1W.Pt());
                RLL_SC_ = (p4_lep1Z.Pt() * p4_lep1W.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
            }
            
            if( ch_lep2Z * ch_lep1W == 1){
                deltaPhiLL_SC_ = deltaPhi(p4_lep2Z.Phi(),p4_lep1W.Phi());
                deltaEtaLL_SC_ = abs(p4_lep2Z.Eta() - p4_lep1W.Eta());
                deltaRLL_SC_ = p4_lep2Z.DeltaR(p4_lep1W);
                ASimLL_SC_ = (fabs(p4_lep2Z.Pt() - p4_lep1W.Pt()))/(p4_lep2Z.Pt() + p4_lep1W.Pt());
                RLL_SC_ = (p4_lep2Z.Pt() * p4_lep1W.Pt())/(RecoJets.at(0).jet4V_.Pt() * RecoJets.at(1).jet4V_.Pt());
                
            }
            
        }// if mz1 mz2 mz3
        
    } //if leptons > 3
    
    TString Name ;
    string  NameSample ;
    if(TString(sampleName).Contains("Madgraph_")){
        Name = sampleName;
        Name.ReplaceAll("Madgraph_","");
        NameSample = string(Name) ;
    }
    else
    NameSample = sampleName ;
    
    
      for(size_t iVar = 0; iVar < VariableList2D.size(); iVar++){

        //change here
    if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC" ){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z" ){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "RLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,RLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "RLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,RLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_dilepton.Pt(),eventFakeWeight,systematicName);
	}
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_trilepton.Pt(),eventFakeWeight,systematicName);
	}
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
    
        ///
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z" ){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "RLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,RLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "RLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,RLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        ///
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "RLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,RLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "RLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,RLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        ///
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "RLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,RLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_SC" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_SC_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "RLL_Z" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RLL_Z_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
    
        //////
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaPhiLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
    
        ////
    
    
        //////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_SC" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_SC_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
    
        ////////////////
    
    
        //////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,deltaRLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaRLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,deltaRLL_SC_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_Z" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_Z_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptj2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,RecoJets.at(1).jet4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaRLL_SC" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaRLL_SC_,sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptj2" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   RecoJets.at(1).jet4V_.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dijet.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "mjj" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.M(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "ptLLLMet" and VariableList2D.at(iVar).variableNameY == "ptJJ_LLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_met).Pt(),(L_trilepton+L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptLLLMet" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_met).Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptLLLMet" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_met).Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptLLLMet" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_met).Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //////////////////////
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ_LLLMet" and VariableList2D.at(iVar).variableNameY == "mll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_dijet+L_met).Pt(),L_dilepton.M(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ_LLLMet" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_dijet+L_met).Pt(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptJJ_LLLMet" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_dijet+L_met).Pt(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
        //////////////////////
    
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "mlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.M(),L_trilepton.M(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "mll" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.M(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "mlll" and VariableList2D.at(iVar).variableNameY == "mTwz"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.M(),sqrt(2*L_trilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),eventFakeWeight,systematicName);
    }
    
        //polarized section
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "costheta2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,costheta2_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "costhetastar"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,costhetastar_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "costheta1_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,costheta1_vbf_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "costheta2_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,costheta2_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "costhetastar_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,costhetastar_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "costhetastar"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,costhetastar_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "costheta1_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,costheta1_vbf_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "costheta2_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,costheta2_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "costhetastar_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,costhetastar_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        /////
        //
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "costheta1_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,costheta1_vbf_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "costheta2_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,costheta2_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "costhetastar_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,costhetastar_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        /////
        //
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "costheta2_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,costheta2_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "costhetastar_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,costhetastar_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        /////
        //
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "costhetastar_vbf"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,costhetastar_vbf_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costheta2_vbf" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta2_vbf_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        /////
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "detajj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costheta1_vbf" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costheta1_vbf_, leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "costhetastar_vbf" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   costhetastar_vbf_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        ////
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptjj"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),L_dijet.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
        ////
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "Asim_j"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), ASimJJ_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptjj" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dijet.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "DeltaPhi_JJ"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "Asim_j" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimJJ_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaPhiLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), deltaPhiLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "DeltaPhi_JJ" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        /////
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, deltaEtaLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaPhiLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaPhiLL_Z_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        ///
    
        /////
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "deltaEtaLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_, deltaEtaLL_Z_,eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_SC_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        ////
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl1"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_, leptonsIsoTight.at(0).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_, leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_, L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_, L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "deltaEtaLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   deltaEtaLL_Z_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        ////
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptl2"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(), leptonsIsoTight.at(1).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "detajj" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl1" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(0).lepton4V_.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
        ////
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptl3"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(), leptonsIsoTight.at(2).lepton4V_.Pt(),eventFakeWeight,systematicName);
    }
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl2" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(1).lepton4V_.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
        ////
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(), L_dilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptl3" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   leptonsIsoTight.at(2).lepton4V_.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
    
        ////
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptlll"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(), L_trilepton.Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptll" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_dilepton.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        ////
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ASimLL_SC"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),ASimLL_SC_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ptlll" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   L_trilepton.Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        ////
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ASimLL_Z"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,ASimLL_Z_,eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_SC" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_SC_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
        //
        ////
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptLLLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,(L_trilepton+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    else if(VariableList2D.at(iVar).variableNameX == "ASimLL_Z" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   ASimLL_Z_,(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
        //
    
    else if(VariableList2D.at(iVar).variableNameX == "ptLLLMet" and VariableList2D.at(iVar).variableNameY == "ptJJLMet"){
        analysisPlots.fill2DHisto (NameSample, samplePosition,cutLayerName,VariableList2D.at(iVar).variableNameX+"_"+VariableList2D.at(iVar).variableNameY,
                                   (L_trilepton+L_met).Pt(),(L_dijet+L_met).Pt(),eventFakeWeight,systematicName);
    }
    
    
}
}




    //// method in  order to apply cuts
bool passCutContainerSelection (cutContainerWZ & Cut,
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
    
    double mll1, mll2;
    double mz = 91;
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
    
    
    
    if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
        
        if(vect.size()!=0){
            vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
            vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
            iBin++;
        }
        
    }
    else iBin++;
    
    
    
    if(LeptonsAll.size() < 3 )  return false ; // skip the event --> only two reco leptons are good
    
    if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
        
        if(vect.size()!=0){
            vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
            vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
            iBin++;
        }
    }
    else iBin++;
    
    
    
        // identify tight leptons and require exactly nLep
    if (int(leptonsIsoTight.size()) != Cut.nLep ) return false;
    
    
    
   if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
    
    if(vect.size()!=0){
        vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
        vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
        iBin++;
    }
}
else iBin++;




if ( Cut.ptL1 != 0 and leptonsIsoTight.at(0).lepton4V_.Pt() < Cut.ptL1) return false;
if ( Cut.ptL2 != 0 and leptonsIsoTight.at(1).lepton4V_.Pt() < Cut.ptL2) return false;
if ( Cut.ptL3 != 0 and leptonsIsoTight.at(2).lepton4V_.Pt() < Cut.ptL3) return false;




if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"lepton pt");
    iBin++;
}



    // eta lepton

if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(0).lepton4V_.Eta()) > Cut.etaL) return false;
if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(1).lepton4V_.Eta()) > Cut.etaL) return false;
if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(2).lepton4V_.Eta()) > Cut.etaL) return false;



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton Eta");
    iBin++;
}



int extraLepton = 0; // count the extra lepton number
for(size_t iLepton = 0; iLepton < leptonsIsoTight.size() ; iLepton++){
    
    if(leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(0).lepton4V_ or leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(1).lepton4V_ or leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(2).lepton4V_ ) continue; // skip tight ones
    extraLepton++;
}

if( Cut.nextra !=99 and extraLepton != Cut.nextra ) return false; // extra lepton cut

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"extra lepton");
    iBin++;
}



    //wz correction
    // Z Mass for one same flavour opposite charge lepton pair
    // Mll mass cut for any same flavour opposite chrge lepton pair
bool isZMassCut1 = true;
bool isZMassCut2 = true;
bool isLLMassCut1 = false;
bool isLLMassCut2 = false;

if( leptonsIsoTight.at(2).lepton4V_.Pt()>-999 ) {
    mll1 =-999;
    mll2 =-999;
    bool mllFilled = false;
    for(int il=0; il<2; il++){
        for(int ij=1; ij<3; ij++ ){
            if(il==ij)continue;
            if( ( abs(leptonsIsoTight.at(il).flavour_) == abs(leptonsIsoTight.at(ij).flavour_) ) and ( (leptonsIsoTight.at(il).charge_ ) * ( leptonsIsoTight.at(ij).charge_) == -1) ){
                if(!mllFilled) {
                    mll1 = (leptonsIsoTight.at(il).lepton4V_ + leptonsIsoTight.at(ij).lepton4V_).M();
                    mllFilled = true;
                }
                else mll2 = (leptonsIsoTight.at(il).lepton4V_ + leptonsIsoTight.at(ij).lepton4V_).M();
            }
            
        }
    }
    
    if(abs(mll1 - mz) < Cut.MZcut) isZMassCut1 = false;
    if(abs(mll2 - mz) < Cut.MZcut) isZMassCut2 = false;
    if ( mll1 != -999 and mll1 < Cut.MLLcut ) isLLMassCut1 = true;
    if ( mll2 != -999 and mll2 < Cut.MLLcut) isLLMassCut2 = true;
    
}



if( Cut.MZcut != 0 ){
    if(isZMassCut1 and isZMassCut2) return false;
        //	if(!isZMassCut1 and !isZMassCut2) return false;
}

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z mass cut");
    iBin++;
}




if( Cut.MLLcut != 0 ){
    if(isLLMassCut1 or isLLMassCut2 ) return false;
        // 	if(isLLMassCut1 and isLLMassCut2 ) return false;
}

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
    iBin++;
}





    //opposite charge same flavour condition for two leptons
    // 0 means no cut, -1 means SF OS leptons



if(Cut.SFOS != 0){
    int isSFOS = 0;
    for(int il=0; il<2; il++){
        for(int ij=1; ij<3; ij++ ){
            if(il==ij)continue;
            if( leptonsIsoTight.at(il).charge_*leptonsIsoTight.at(ij).charge_ == -1 and  abs(leptonsIsoTight.at(il).flavour_) == abs(leptonsIsoTight.at(ij).flavour_) ) isSFOS=-1;
        }
    }
    
    
    if (isSFOS != Cut.SFOS) return false;
    
    
}




if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"SFOS");
    iBin++;
}
    //}


    //WZ  OBJECTS
    //**********************************************************************************************//
    //**********************************************************************************************//


bool mZ1 = false;
bool mZ2 = false;
bool mZ3 = false;

TLorentzVector p4_lep1Z, p4_lep2Z, p4_lep1W;
TLorentzVector p4_lep[3];

for(int il=0; il<3; il++){
    p4_lep[il].SetPtEtaPhiM(leptonsIsoTight.at(il).lepton4V_.Pt(),leptonsIsoTight.at(il).lepton4V_.Eta(), leptonsIsoTight.at(il).lepton4V_.Phi(),0);
}


Int_t ch_lep1Z = 0, ch_lep2Z = 0, ch_lep1W = 0;
Int_t pid_lep1Z = 0, pid_lep2Z = 0, pid_lep1W = 0;
double mass1=-999; double mass2 = -999; double mass3 = -999;

if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(1).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(1).charge_) == -1) ){
    if ( fabs( (p4_lep[0] + p4_lep[1]).M() - 91) < 20  ){
        mass1 = (p4_lep[0] + p4_lep[1]).M();
        mZ1 = true;
    }
}
if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
    if ( fabs((p4_lep[0] + p4_lep[2]).M()- 91) < 20  ){
        mass2 = (p4_lep[0] + p4_lep[2]).M();
        mZ2 = true;
    }
}
if( ( abs(leptonsIsoTight.at(1).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(1).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
    if ( fabs((p4_lep[1] + p4_lep[2]).M()- 91) < 20  ){
        mass3 = (p4_lep[1] + p4_lep[2]).M();
        mZ3 = true;
    }
}

    //to make lepton pairs from Z

if( mZ1 && mZ2){
    if( abs(mass1 - 91) < abs(mass2 -91) ) mZ2 = false;
    else mZ1 = false;
}
if( mZ1 && mZ3){
    if( abs(mass1 - 91) < abs(mass3 -91) ) mZ3 = false;
    else mZ1 = false;
}
if( mZ2 && mZ3){
    if( abs(mass2 - 91) < abs(mass3 -91) ) mZ3 = false;
    else mZ2 = false;
}

    //if( (!mZ1 && !mZ2) && !mZ3) continue;
if( mZ1 || mZ2 || mZ3) {
    if(mZ1){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        p4_lep1W.SetPtEtaPhiE( p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(1).charge_;    ch_lep1W = leptonsIsoTight.at(2).charge_;
        pid_lep1Z = leptonsIsoTight.at(0).flavour_;   pid_lep2Z = leptonsIsoTight.at(1).flavour_;    pid_lep1W = leptonsIsoTight.at(2).flavour_;
    }
    
    if(mZ2){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        p4_lep1W.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(1).charge_;
        pid_lep1Z = leptonsIsoTight.at(0).flavour_;   pid_lep2Z = leptonsIsoTight.at(2).flavour_;    pid_lep1W = leptonsIsoTight.at(2).flavour_;
    }
    if(mZ3){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        p4_lep1W.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());;
        ch_lep1Z = leptonsIsoTight.at(1).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(0).charge_;
        pid_lep1Z = leptonsIsoTight.at(1).flavour_;   pid_lep2Z = leptonsIsoTight.at(2).flavour_;    pid_lep1W = leptonsIsoTight.at(0).flavour_;
    }
    
    
}// if mz1 mz2 mz3



if(Cut.flavour != 0){
    
    if(finalStateString == "UUU"){
        if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
        if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
        if(fabs(leptonsIsoTight.at(2).flavour_) != 13) return false;
    }
    else if(finalStateString == "EEE"){
        if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
        if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
        if(fabs(leptonsIsoTight.at(2).flavour_) != 11) return false;
    }
    else if(finalStateString == "EEU"){
        if(fabs(pid_lep1Z) != 11) return false;
        if(fabs(pid_lep2Z) != 11) return false;
        if(fabs(pid_lep1W) != 13) return false;
    }
    else if(finalStateString == "UUE"){
        if(fabs(pid_lep1Z) != 13) return false;
        if(fabs(pid_lep2Z) != 13) return false;
        if(fabs(pid_lep1W) != 11) return false;
    }
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
    iBin++;
}



    // MET CUT

if( Cut.MET != 0 ){
    if(reader->pfmet < Cut.MET) return false;
}


if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"met selection");
    iBin++;
}



    // jets cut
if(Cut.nJet != 0){
    if(RecoJets.size() < Cut.nJet ) return false;
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
    iBin++;
}


if ( Cut.ptJet1 != 0 and RecoJets.at(0).jet4V_.Pt() < Cut.ptJet1) return false;
if ( Cut.ptJet2 != 0 and RecoJets.at(1).jet4V_.Pt() < Cut.ptJet2) return false;





if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet pt");
    iBin++;
}


if ( Cut.etaJet != 0 and fabs(RecoJets.at(0).jet4V_.Eta()) > Cut.etaJet) return false;
if ( Cut.etaJet != 0 and fabs(RecoJets.at(1).jet4V_.Eta()) > Cut.etaJet) return false;



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet eta");
    iBin++;
}


    ///////// Lorentz Vector
TLorentzVector L_dilepton;
L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;


TLorentzVector L_dijet;
if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
}



    // apply VBF cuts
if( Cut.Mjj != 0){
    if(L_dijet.M() < Cut.Mjj) return false;
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
    iBin++;
}


if( Cut.DetaJJ != 0 ){
    if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < Cut.DetaJJ) return false;
}


if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
    iBin++;
}




if( Cut.HTTrackjet != 99 and trackEvent.HTTrack_ > Cut.HTTrackjet) return false;


if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"HTTrackjet cut");
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
                                cutContainerWZ & Cut,
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
    
    double mll1, mll2;
    double mz = 91;
        // identify loose leptons
   
  vector<leptonContainer> LeptonsAll;
  fillRecoLeptonsArray (LeptonsAll, *reader);

  // dump tight leptons                                                                                                                                                        
  vector<leptonContainer> leptonsIsoTight ;
  leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
  
   vector<jetContainer> RecoJetsAll ;
  if(!usePuppiAsDefault)
    fillRecoJetArray (RecoJetsAll, *reader) ;
  else
    fillPuppiJetArray (RecoJetsAll, *reader) ;

    
    
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
    
    
    
    if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
        
        if(vect.size()!=0){
            vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
            vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"all events");
            iBin++;
        }
        
    }
    else iBin++;
    
    
    
    if(LeptonsAll.size() < 3 )  return false ; // skip the event --> only two reco leptons are good
    
    if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
        
        if(vect.size()!=0){
            vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
            vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Reco L-J");
            iBin++;
        }
    }
    else iBin++;
    
    
    
        // identify tight leptons and require exactly nLep
    if (int(leptonsIsoTight.size()) != Cut.nLep ) return false;
    
    
    
   if(not TString(sampleName).Contains("TTbar") or not TString(sampleName).Contains("ll3J") ){
    
    if(vect.size()!=0){
        vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
        vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"NLep tight");
        iBin++;
    }
}
else iBin++;




if ( Cut.ptL1 != 0 and leptonsIsoTight.at(0).lepton4V_.Pt() < Cut.ptL1) return false;
if ( Cut.ptL2 != 0 and leptonsIsoTight.at(1).lepton4V_.Pt() < Cut.ptL2) return false;
if ( Cut.ptL3 != 0 and leptonsIsoTight.at(2).lepton4V_.Pt() < Cut.ptL3) return false;




if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"lepton pt");
    iBin++;
}



    // eta lepton

if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(0).lepton4V_.Eta()) > Cut.etaL) return false;
if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(1).lepton4V_.Eta()) > Cut.etaL) return false;
if ( Cut.etaL != 0 and fabs(leptonsIsoTight.at(2).lepton4V_.Eta()) > Cut.etaL) return false;



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Lepton Eta");
    iBin++;
}



int extraLepton = 0; // count the extra lepton number
for(size_t iLepton = 0; iLepton < leptonsIsoTight.size() ; iLepton++){
    
    if(leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(0).lepton4V_ or leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(1).lepton4V_ or leptonsIsoTight.at(iLepton).lepton4V_ == leptonsIsoTight.at(2).lepton4V_ ) continue; // skip tight ones
    extraLepton++;
}

if( Cut.nextra !=99 and extraLepton != Cut.nextra ) return false; // extra lepton cut

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"extra lepton");
    iBin++;
}



    //wz correction
    // Z Mass for one same flavour opposite charge lepton pair
    // Mll mass cut for any same flavour opposite chrge lepton pair
bool isZMassCut1 = true;
bool isZMassCut2 = true;
bool isLLMassCut1 = false;
bool isLLMassCut2 = false;

if( leptonsIsoTight.at(2).lepton4V_.Pt()>-999 ) {
    mll1 =-999;
    mll2 =-999;
    bool mllFilled = false;
    for(int il=0; il<2; il++){
        for(int ij=1; ij<3; ij++ ){
            if(il==ij)continue;
            if( ( abs(leptonsIsoTight.at(il).flavour_) == abs(leptonsIsoTight.at(ij).flavour_) ) and ( (leptonsIsoTight.at(il).charge_ ) * ( leptonsIsoTight.at(ij).charge_) == -1) ){
                if(!mllFilled) {
                    mll1 = (leptonsIsoTight.at(il).lepton4V_ + leptonsIsoTight.at(ij).lepton4V_).M();
                    mllFilled = true;
                }
                else mll2 = (leptonsIsoTight.at(il).lepton4V_ + leptonsIsoTight.at(ij).lepton4V_).M();
            }
            
        }
    }
    
    if(abs(mll1 - mz) < Cut.MZcut) isZMassCut1 = false;
    if(abs(mll2 - mz) < Cut.MZcut) isZMassCut2 = false;
    if ( mll1 != -999 and mll1 < Cut.MLLcut ) isLLMassCut1 = true;
    if ( mll2 != -999 and mll2 < Cut.MLLcut) isLLMassCut2 = true;
    
}



if( Cut.MZcut != 0 ){
    if(isZMassCut1 and isZMassCut2) return false;
        //	if(!isZMassCut1 and !isZMassCut2) return false;
}

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Z mass cut");
    iBin++;
}




if( Cut.MLLcut != 0 ){
    if(isLLMassCut1 or isLLMassCut2 ) return false;
        // 	if(isLLMassCut1 and isLLMassCut2 ) return false;
}

if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mll");
    iBin++;
}





    //opposite charge same flavour condition for two leptons
    // 0 means no cut, -1 means SF OS leptons



if(Cut.SFOS != 0){
    int isSFOS = 0;
    for(int il=0; il<2; il++){
        for(int ij=1; ij<3; ij++ ){
            if(il==ij)continue;
            if( leptonsIsoTight.at(il).charge_*leptonsIsoTight.at(ij).charge_ == -1 and  abs(leptonsIsoTight.at(il).flavour_) == abs(leptonsIsoTight.at(ij).flavour_) ) isSFOS=-1;
        }
    }
    
    
    if (isSFOS != Cut.SFOS) return false;
    
    
}




if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"SFOS");
    iBin++;
}
    //}


    //WZ  OBJECTS
    //**********************************************************************************************//
    //**********************************************************************************************//


bool mZ1 = false;
bool mZ2 = false;
bool mZ3 = false;

TLorentzVector p4_lep1Z, p4_lep2Z, p4_lep1W;
TLorentzVector p4_lep[3];

for(int il=0; il<3; il++){
    p4_lep[il].SetPtEtaPhiM(leptonsIsoTight.at(il).lepton4V_.Pt(),leptonsIsoTight.at(il).lepton4V_.Eta(), leptonsIsoTight.at(il).lepton4V_.Phi(),0);
}


Int_t ch_lep1Z = 0, ch_lep2Z = 0, ch_lep1W = 0;
Int_t pid_lep1Z = 0, pid_lep2Z = 0, pid_lep1W = 0;
double mass1=-999; double mass2 = -999; double mass3 = -999;

if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(1).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(1).charge_) == -1) ){
    if ( fabs( (p4_lep[0] + p4_lep[1]).M() - 91) < 20  ){
        mass1 = (p4_lep[0] + p4_lep[1]).M();
        mZ1 = true;
    }
}
if( ( abs(leptonsIsoTight.at(0).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(0).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
    if ( fabs((p4_lep[0] + p4_lep[2]).M()- 91) < 20  ){
        mass2 = (p4_lep[0] + p4_lep[2]).M();
        mZ2 = true;
    }
}
if( ( abs(leptonsIsoTight.at(1).flavour_) == abs(leptonsIsoTight.at(2).flavour_) ) && ( (leptonsIsoTight.at(1).charge_) * (leptonsIsoTight.at(2).charge_) == -1) ){
    if ( fabs((p4_lep[1] + p4_lep[2]).M()- 91) < 20  ){
        mass3 = (p4_lep[1] + p4_lep[2]).M();
        mZ3 = true;
    }
}

    //to make lepton pairs from Z

if( mZ1 && mZ2){
    if( abs(mass1 - 91) < abs(mass2 -91) ) mZ2 = false;
    else mZ1 = false;
}
if( mZ1 && mZ3){
    if( abs(mass1 - 91) < abs(mass3 -91) ) mZ3 = false;
    else mZ1 = false;
}
if( mZ2 && mZ3){
    if( abs(mass2 - 91) < abs(mass3 -91) ) mZ3 = false;
    else mZ2 = false;
}

    //if( (!mZ1 && !mZ2) && !mZ3) continue;
if( mZ1 || mZ2 || mZ3) {
    if(mZ1){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        p4_lep1W.SetPtEtaPhiE( p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(1).charge_;    ch_lep1W = leptonsIsoTight.at(2).charge_;
        pid_lep1Z = leptonsIsoTight.at(0).flavour_;   pid_lep2Z = leptonsIsoTight.at(1).flavour_;    pid_lep1W = leptonsIsoTight.at(2).flavour_;
    }
    
    if(mZ2){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        p4_lep1W.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        ch_lep1Z = leptonsIsoTight.at(0).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(1).charge_;
        pid_lep1Z = leptonsIsoTight.at(0).flavour_;   pid_lep2Z = leptonsIsoTight.at(2).flavour_;    pid_lep1W = leptonsIsoTight.at(2).flavour_;
    }
    if(mZ3){
        p4_lep1Z.SetPtEtaPhiE(p4_lep[1].Pt(), p4_lep[1].Eta(),p4_lep[1].Phi(), p4_lep[1].E());
        p4_lep2Z.SetPtEtaPhiE(p4_lep[2].Pt(), p4_lep[2].Eta(),p4_lep[2].Phi(), p4_lep[2].E());
        p4_lep1W.SetPtEtaPhiE(p4_lep[0].Pt(), p4_lep[0].Eta(),p4_lep[0].Phi(), p4_lep[0].E());;
        ch_lep1Z = leptonsIsoTight.at(1).charge_;   ch_lep2Z = leptonsIsoTight.at(2).charge_;    ch_lep1W = leptonsIsoTight.at(0).charge_;
        pid_lep1Z = leptonsIsoTight.at(1).flavour_;   pid_lep2Z = leptonsIsoTight.at(2).flavour_;    pid_lep1W = leptonsIsoTight.at(0).flavour_;
    }
    
    
}// if mz1 mz2 mz3



if(Cut.flavour != 0){
    
    if(finalStateString == "UUU"){
        if(fabs(leptonsIsoTight.at(0).flavour_) != 13) return false;
        if(fabs(leptonsIsoTight.at(1).flavour_) != 13) return false;
        if(fabs(leptonsIsoTight.at(2).flavour_) != 13) return false;
    }
    else if(finalStateString == "EEE"){
        if(fabs(leptonsIsoTight.at(0).flavour_) != 11) return false;
        if(fabs(leptonsIsoTight.at(1).flavour_) != 11) return false;
        if(fabs(leptonsIsoTight.at(2).flavour_) != 11) return false;
    }
    else if(finalStateString == "EEU"){
        if(fabs(pid_lep1Z) != 11) return false;
        if(fabs(pid_lep2Z) != 11) return false;
        if(fabs(pid_lep1W) != 13) return false;
    }
    else if(finalStateString == "UUE"){
        if(fabs(pid_lep1Z) != 13) return false;
        if(fabs(pid_lep2Z) != 13) return false;
        if(fabs(pid_lep1W) != 11) return false;
    }
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"flavour selection");
    iBin++;
}



    // MET CUT

if( Cut.MET != 0 ){
    if(reader->pfmet < Cut.MET) return false;
}


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



    // jets cut
if(Cut.nJet != 0){
    if(RecoJets.size() < Cut.nJet ) return false;
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet counting");
    iBin++;
}


if ( Cut.ptJet1 != 0 and RecoJets.at(0).jet4V_.Pt() < Cut.ptJet1) return false;
if ( Cut.ptJet2 != 0 and RecoJets.at(1).jet4V_.Pt() < Cut.ptJet2) return false;





if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet pt");
    iBin++;
}


if ( Cut.etaJet != 0 and fabs(RecoJets.at(0).jet4V_.Eta()) > Cut.etaJet) return false;
if ( Cut.etaJet != 0 and fabs(RecoJets.at(1).jet4V_.Eta()) > Cut.etaJet) return false;



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"jet eta");
    iBin++;
}


    ///////// Lorentz Vector
TLorentzVector L_dilepton;
L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;


TLorentzVector L_dijet;
if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
}



    // apply VBF cuts
if( Cut.Mjj != 0){
    if(L_dijet.M() < Cut.Mjj) return false;
}



if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"Mjj");
    iBin++;
}


if( Cut.DetaJJ != 0 ){
    if(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()) < Cut.DetaJJ) return false;
}


if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"DetaJJ");
    iBin++;
}




if( Cut.HTTrackjet != 99 and trackEvent.HTTrack_ > Cut.HTTrackjet) return false;


if(vect.size()!=0){
    vect[Name+"_"+Cut.cutLayerName]->SetBinContent(iBin,vect[Name+"_"+Cut.cutLayerName]->GetBinContent(iBin)+eventWeight);
    vect[Name+"_"+Cut.cutLayerName]->GetXaxis()->SetBinLabel(iBin,"HTTrackjet cut");
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

void neutrinoP4(TLorentzVector met, TLorentzVector lep, int lepType, double& metp4_0, double& metp4_1, double& metp4_2, double& metp4_3 ){
        //lepType 0 ele, 1 mu
    double MW_ = 80.4;
    
    double leppt = lep.Pt();
    double lepphi = lep.Phi();
    double lepeta = lep.Eta();
    double lepenergy = lep.Energy();
    
    double metpt = met.Pt();
    double metphi = met.Phi();
    
    
    double  px = metpt*cos(metphi);
    double  py = metpt*sin(metphi);
    double  pz = 0;
    double  pxl= leppt*cos(lepphi);
    double  pyl= leppt*sin(lepphi);
    double  pzl= leppt*sinh(lepeta);
    double  El = lepenergy;
    double  a = pow(MW_,2) + pow(px+pxl,2) + pow(py+pyl,2) - px*px - py*py - El*El + pzl*pzl;
    double  b = 2.*pzl;
    double  A = b*b -4.*El*El;
    double  B = 2.*a*b;
    double  C = a*a-4.*(px*px+py*py)*El*El;
    /*
     double tmp1;
     double tmp2;
     if(B*B<4.*A*C)pz=-B/2./A;
     else {
     tmp1 = (-B + sqrt(B*B-4.*A*C))/2./A;
     tmp2 = (-B - sqrt(B*B-4.*A*C))/2./A;
     pz = (fabs(tmp1)>=fabs(tmp2))?tmp1:tmp2;
     }
     */
    
        ///////////////////////////pz for fnal
    double M_mu =  0;
    if(lepType==11)M_mu=0.105658367;//mu
    if(lepType==13)M_mu=0.00051099891;//electron
    
    int type=2; // use the small abs real root
    
    double otherSol_ = 0.;
    
    a = MW_*MW_ - M_mu*M_mu + 2.0*pxl*px + 2.0*pyl*py;
    A = 4.0*(El*El - pzl*pzl);
    B = -4.0*a*pzl;
    C = 4.0*El*El*(px*px + py*py) - a*a;
    
    double newPtneutrino1_=0;
    double newPtneutrino2_=0;
    
    double tmproot = B*B - 4.0*A*C;
    
    bool isComplex_;
    
    if (tmproot<0) {
        isComplex_= true;
        pz = - B/(2*A); // take real part of complex roots
        otherSol_ = pz;
        
            // recalculate the neutrino pT
            // solve quadratic eq. discriminator = 0 for pT of nu
        double pnu = metpt;
        double Delta = (MW_*MW_ - M_mu*M_mu);
        double alpha = (pxl*px/pnu + pyl*py/pnu);
        double ptmu = TMath::Sqrt( pxl*pxl + pyl*pyl);
        double ptnu = TMath::Sqrt( px*px + py*py); // old
        double AA = 4.*pzl*pzl - 4*El*El + 4*alpha*alpha;
        double BB = 4.*alpha*Delta;
        double CC = Delta*Delta;
        
        double tmpdisc = BB*BB - 4.0*AA*CC;
        double tmpsolpt1 = (-BB + TMath::Sqrt(tmpdisc))/(2.0*AA);
        double tmpsolpt2 = (-BB - TMath::Sqrt(tmpdisc))/(2.0*AA);
        
            //should rejetc negative root
        
        if ( tmpsolpt1 >0 ) { newPtneutrino1_ = tmpsolpt1; newPtneutrino2_ = tmpsolpt2;}
        else { newPtneutrino1_ = tmpsolpt2; newPtneutrino2_ = tmpsolpt1; }
        
    }
    else {
        isComplex_ = false;
        double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
        double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);
        
        
        if (type == 0 ) {
                // two real roots, pick the one closest to pz of muon
            if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol_ = tmpsol1;}
            else { pz = tmpsol1; otherSol_ = tmpsol2; }
                // if pz is > 300 pick the most central root
            if ( abs(pz) > 300. ) {
                if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol_ = tmpsol2; }
                else { pz = tmpsol2; otherSol_ = tmpsol1; }
            }
        }
        if (type == 1 ) {
                // two real roots, pick the one closest to pz of muon
            if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol_ = tmpsol1; }
            else {pz = tmpsol1; otherSol_ = tmpsol2; }
        }
        if (type == 2 ) {
                // pick the most central root.
            if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol_ = tmpsol2; }
            else { pz = tmpsol2; otherSol_ = tmpsol1; }
        }
        if (type == 3 ) {
                // pick the largest value of the cosine
            TVector3 p3w, p3mu;
            p3w.SetXYZ(pxl+px, pyl+py, pzl+ tmpsol1);
            p3mu.SetXYZ(pxl, pyl, pzl );
            
            double sinthcm1 = 2.*(p3mu.Perp(p3w))/MW_;
            p3w.SetXYZ(pxl+px, pyl+py, pzl+ tmpsol2);
            double sinthcm2 = 2.*(p3mu.Perp(p3w))/MW_;
            
            double costhcm1 = TMath::Sqrt(1. - sinthcm1*sinthcm1);
            double costhcm2 = TMath::Sqrt(1. - sinthcm2*sinthcm2);
            
            if ( costhcm1 > costhcm2 ) { pz = tmpsol1; otherSol_ = tmpsol2; }
            else { pz = tmpsol2;otherSol_ = tmpsol1; }
            
        }//end of type3
        
    }//endl of if real root
    
    TLorentzVector outP4(px,py,pz,sqrt(px*px+py*py+pz*pz));
    metp4_0 = px;
    metp4_1 = py;
    metp4_2 = pz;
    metp4_3 = sqrt(px*px+py*py+pz*pz);
    
    
}

