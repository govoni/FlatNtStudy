//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Mar  6 12:22:52 2015 by ROOT version 5.34/18
// from TTree probe_tree/probe_tree
// found on file: /data2/rgerosa/LATINO_VBS/wpwpjj_qed_4_qcd_0_GEM2019Upg14DR-Phase1age1kJan23_PU140BX25_PH1_1K_FB_V3-v1/output_1_1_gaY.root
//////////////////////////////////////////////////////////

#ifndef readTreeLatino_h
#define readTreeLatino_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <Math/GenVector/LorentzVector.h>
#include <Math/GenVector/PxPyPzE4D.h>

// Fixed size dimensions of array or collections stored in the TTree if any.

class readTreeLatino {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         HEPMCweight;
   Float_t         HEPMCweightFac0;
   Float_t         HEPMCweightFac1;
   Float_t         HEPMCweightFac2;
   Float_t         HEPMCweightFac3;
   Float_t         HEPMCweightFac4;
   Float_t         HEPMCweightFac5;
   Float_t         HEPMCweightFac6;
   Float_t         HEPMCweightRen0;
   Float_t         HEPMCweightRen1;
   Float_t         HEPMCweightRen2;
   Float_t         HEPMCweightRen3;
   Float_t         HEPMCweightRen4;
   Float_t         HEPMCweightRen5;
   Float_t         HEPMCweightRen6;
   Float_t         HEPMCweightScale0;
   Float_t         HEPMCweightScale1;
   Float_t         HEPMCweightScale2;
   Float_t         HEPMCweightScale3;
   Float_t         HEPMCweightScale4;
   Float_t         HEPMCweightScale5;
   Float_t         HEPMCweightScale6;
   Float_t         MHiggs;
   Float_t         PtHiggs;
   Float_t         baseW;
   Float_t         bdt1;
   Float_t         bdt2;
   Float_t         bdt3;
   Float_t         bdt4;
   Float_t         ch1;
   Float_t         ch2;
   Float_t         ch3;
   Float_t         ch4;
   Float_t         channel;
   Float_t         cjeteta1;
   Float_t         cjeteta1_puppi;
   Float_t         cjeteta2;
   Float_t         cjeteta2_puppi;
   Float_t         cjetid1;
   Float_t         cjetid1_puppi;
   Float_t         cjetid2;
   Float_t         cjetid2_puppi;
   Float_t         cjetmva1;
   Float_t         cjetmva1_puppi;
   Float_t         cjetmva2;
   Float_t         cjetmva2_puppi;
   Float_t         cjetphi1;
   Float_t         cjetphi1_puppi;
   Float_t         cjetphi2;
   Float_t         cjetphi2_puppi;
   Float_t         cjetpileupidFlag1;
   Float_t         cjetpileupidFlag1_puppi;
   Float_t         cjetpileupidFlag2;
   Float_t         cjetpileupidFlag2_puppi;
   Float_t         cjetpileupidValue1;
   Float_t         cjetpileupidValue1_puppi;
   Float_t         cjetpileupidValue2;
   Float_t         cjetpileupidValue2_puppi;
   Float_t         cjetpt1;
   Float_t         cjetpt1_puppi;
   Float_t         cjetpt2;
   Float_t         cjetpt2_puppi;
   Float_t         dataset;
   Float_t         detajj;
   Float_t         detajj_puppi;
   Float_t         dphilMet1_puppi;
   Float_t         dphilMet2_puppi;
   Float_t         dphilMet_puppi;
   Float_t         dphill;
   Float_t         dphillJet_puppi;
   Float_t         dphillMet_puppi;
   Float_t         dphilljet;
   Float_t         dphilljetjet;
   Float_t         dphilljetjet_puppi;
   Float_t         dphillmet;
   Float_t         dphilmet;
   Float_t         dphilmet1;
   Float_t         dphilmet2;
   Float_t         drll;
   Float_t         effAW;
   Float_t         effBW;
   Float_t         effW;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         eta3;
   Float_t         eta4;
   Float_t         fakeAW;
   Float_t         fakeBW;
   Float_t         fakeW;
   Float_t         fermiW;
   Float_t         fourW;
   Float_t         gammaMRStar;
   Float_t         hardbcsv;
   Float_t         hardbcsvMVA;
   Float_t         hardbjpb;
   Float_t         hardtche;
   Float_t         hardtchp;
   Float_t         higggsGenpt;
   Float_t         higgsLHEpt;
   Float_t         isHighPtMuon1;
   Float_t         isHighPtMuon2;
   Float_t         isHighPtMuon3;
   Float_t         isHighPtMuon4;
   Float_t         isLooseElectron1;
   Float_t         isLooseElectron2;
   Float_t         isLooseElectron3;
   Float_t         isLooseElectron4;
   Float_t         isLooseMuon1;
   Float_t         isLooseMuon2;
   Float_t         isLooseMuon3;
   Float_t         isLooseMuon4;
   Float_t         isLooseRobustElectron1;
   Float_t         isLooseRobustElectron2;
   Float_t         isLooseRobustElectron3;
   Float_t         isLooseRobustElectron4;
   Float_t         isRobustHighEnergyElectron1;
   Float_t         isRobustHighEnergyElectron2;
   Float_t         isRobustHighEnergyElectron3;
   Float_t         isRobustHighEnergyElectron4;
   Float_t         isSTA1;
   Float_t         isSTA2;
   Float_t         isSTA3;
   Float_t         isSTA4;
   Float_t         isSoftMuon1;
   Float_t         isSoftMuon2;
   Float_t         isSoftMuon3;
   Float_t         isSoftMuon4;
   Float_t         isTightElectron1;
   Float_t         isTightElectron2;
   Float_t         isTightElectron3;
   Float_t         isTightElectron4;
   Float_t         isTightMuon1;
   Float_t         isTightMuon2;
   Float_t         isTightMuon3;
   Float_t         isTightMuon4;
   Float_t         isTightRobustElectron1;
   Float_t         isTightRobustElectron2;
   Float_t         isTightRobustElectron3;
   Float_t         isTightRobustElectron4;
   Float_t         isomva1;
   Float_t         isomva2;
   Float_t         isomva3;
   Float_t         isomva4;
   Float_t         jetGenPartoneta1;
   Float_t         jetGenPartoneta2;
   Float_t         jetGenPartoneta3;
   Float_t         jetGenPartonphi1;
   Float_t         jetGenPartonphi2;
   Float_t         jetGenPartonphi3;
   Float_t         jetGenPartonpid1;
   Float_t         jetGenPartonpid2;
   Float_t         jetGenPartonpid3;
   Float_t         jetGenPartonpt1;
   Float_t         jetGenPartonpt2;
   Float_t         jetGenPartonpt3;
   Float_t         jetGeneta1;
   Float_t         jetGeneta2;
   Float_t         jetGeneta3;
   Float_t         jetGeneta4;
   Float_t         jetGeneta5;
   Float_t         jetGenphi1;
   Float_t         jetGenphi2;
   Float_t         jetGenphi3;
   Float_t         jetGenphi4;
   Float_t         jetGenphi5;
   Float_t         jetGenpt1;
   Float_t         jetGenpt2;
   Float_t         jetGenpt3;
   Float_t         jetGenpt4;
   Float_t         jetGenpt5;
   Float_t         jetLHEPartoneta1;
   Float_t         jetLHEPartoneta2;
   Float_t         jetLHEPartoneta3;
   Float_t         jetLHEPartonphi1;
   Float_t         jetLHEPartonphi2;
   Float_t         jetLHEPartonphi3;
   Float_t         jetLHEPartonpid1;
   Float_t         jetLHEPartonpid2;
   Float_t         jetLHEPartonpid3;
   Float_t         jetLHEPartonpt1;
   Float_t         jetLHEPartonpt2;
   Float_t         jetLHEPartonpt3;
   Float_t         jetRho;
   Float_t         jetbjpb1;
   Float_t         jetbjpb2;
   Float_t         jetbjpb3;
   Float_t         jetbjpb4;
   Float_t         jetbjpb5;
   Float_t         jetbjpb6;
   Float_t         jetbjpb7;
   Float_t         jetbjpb8;
   Float_t         jetbjpb_puppi1;
   Float_t         jetbjpb_puppi2;
   Float_t         jetbjpb_puppi3;
   Float_t         jetbjpb_puppi4;
   Float_t         jetbjpb_puppi5;
   Float_t         jetbjpb_puppi6;
   Float_t         jetbjpb_puppi7;
   Float_t         jetbjpb_puppi8;
   Float_t         jetcsv1;
   Float_t         jetcsv2;
   Float_t         jetcsv3;
   Float_t         jetcsv4;
   Float_t         jetcsv5;
   Float_t         jetcsv6;
   Float_t         jetcsv7;
   Float_t         jetcsv8;
   Float_t         jetcsvMVA1;
   Float_t         jetcsvMVA2;
   Float_t         jetcsvMVA3;
   Float_t         jetcsvMVA4;
   Float_t         jetcsvMVA5;
   Float_t         jetcsvMVA6;
   Float_t         jetcsvMVA7;
   Float_t         jetcsvMVA8;
   Float_t         jetcsvMVA_puppi1;
   Float_t         jetcsvMVA_puppi2;
   Float_t         jetcsvMVA_puppi3;
   Float_t         jetcsvMVA_puppi4;
   Float_t         jetcsvMVA_puppi5;
   Float_t         jetcsvMVA_puppi6;
   Float_t         jetcsvMVA_puppi7;
   Float_t         jetcsvMVA_puppi8;
   Float_t         jetcsv_puppi1;
   Float_t         jetcsv_puppi2;
   Float_t         jetcsv_puppi3;
   Float_t         jetcsv_puppi4;
   Float_t         jetcsv_puppi5;
   Float_t         jetcsv_puppi6;
   Float_t         jetcsv_puppi7;
   Float_t         jetcsv_puppi8;
   Float_t         jeteta1;
   Float_t         jeteta2;
   Float_t         jeteta3;
   Float_t         jeteta4;
   Float_t         jeteta5;
   Float_t         jeteta6;
   Float_t         jeteta7;
   Float_t         jeteta8;
   Float_t         jeteta_puppi1;
   Float_t         jeteta_puppi2;
   Float_t         jeteta_puppi3;
   Float_t         jeteta_puppi4;
   Float_t         jeteta_puppi5;
   Float_t         jeteta_puppi6;
   Float_t         jeteta_puppi7;
   Float_t         jeteta_puppi8;
   Float_t         jetid1;
   Float_t         jetid2;
   Float_t         jetid3;
   Float_t         jetid4;
   Float_t         jetid5;
   Float_t         jetid6;
   Float_t         jetid7;
   Float_t         jetid8;
   Float_t         jetid_puppi1;
   Float_t         jetid_puppi2;
   Float_t         jetid_puppi4;
   Float_t         jetid_puppi5;
   Float_t         jetid_puppi6;
   Float_t         jetid_puppi7;
   Float_t         jetid_puppi8;
   Float_t         jetmass1;
   Float_t         jetmass2;
   Float_t         jetmass3;
   Float_t         jetmass4;
   Float_t         jetmass5;
   Float_t         jetmass6;
   Float_t         jetmass7;
   Float_t         jetmass8;
   Float_t         jetmass_puppi1;
   Float_t         jetmass_puppi2;
   Float_t         jetmass_puppi3;
   Float_t         jetmass_puppi4;
   Float_t         jetmass_puppi5;
   Float_t         jetmass_puppi6;
   Float_t         jetmass_puppi7;
   Float_t         jetmass_puppi8;
   Float_t         jetmva1;
   Float_t         jetmva2;
   Float_t         jetmva3;
   Float_t         jetmva4;
   Float_t         jetmva5;
   Float_t         jetmva6;
   Float_t         jetmva7;
   Float_t         jetmva8;
   Float_t         jetmva_puppi1;
   Float_t         jetmva_puppi2;
   Float_t         jetmva_puppi3;
   Float_t         jetmva_puppi4;
   Float_t         jetmva_puppi5;
   Float_t         jetmva_puppi6;
   Float_t         jetmva_puppi7;
   Float_t         jetmva_puppi8;
   Float_t         jetphi1;
   Float_t         jetphi2;
   Float_t         jetphi3;
   Float_t         jetphi4;
   Float_t         jetphi5;
   Float_t         jetphi6;
   Float_t         jetphi7;
   Float_t         jetphi8;
   Float_t         jetphi_puppi1;
   Float_t         jetphi_puppi2;
   Float_t         jetphi_puppi3;
   Float_t         jetphi_puppi4;
   Float_t         jetphi_puppi5;
   Float_t         jetphi_puppi6;
   Float_t         jetphi_puppi7;
   Float_t         jetphi_puppi8;
   Float_t         jetpileupidFlag1;
   Float_t         jetpileupidFlag2;
   Float_t         jetpileupidFlag3;
   Float_t         jetpileupidFlag4;
   Float_t         jetpileupidFlag5;
   Float_t         jetpileupidFlag6;
   Float_t         jetpileupidFlag7;
   Float_t         jetpileupidFlag8;
   Float_t         jetpileupidFlag_puppi1;
   Float_t         jetpileupidFlag_puppi2;
   Float_t         jetpileupidFlag_puppi3;
   Float_t         jetpileupidFlag_puppi4;
   Float_t         jetpileupidFlag_puppi5;
   Float_t         jetpileupidFlag_puppi6;
   Float_t         jetpileupidFlag_puppi7;
   Float_t         jetpileupidFlag_puppi8;
   Float_t         jetpileupidValue1;
   Float_t         jetpileupidValue2;
   Float_t         jetpileupidValue3;
   Float_t         jetpileupidValue4;
   Float_t         jetpileupidValue5;
   Float_t         jetpileupidValue6;
   Float_t         jetpileupidValue7;
   Float_t         jetpileupidValue8;
   Float_t         jetpileupidValue_puppi1;
   Float_t         jetpileupidValue_puppi2;
   Float_t         jetpileupidValue_puppi3;
   Float_t         jetpileupidValue_puppi4;
   Float_t         jetpileupidValue_puppi5;
   Float_t         jetpileupidValue_puppi6;
   Float_t         jetpileupidValue_puppi7;
   Float_t         jetpileupidValue_puppi8;
   Float_t         jetpt1;
   Float_t         jetpt2;
   Float_t         jetpt3;
   Float_t         jetpt4;
   Float_t         jetpt5;
   Float_t         jetpt6;
   Float_t         jetpt7;
   Float_t         jetpt8;
   Float_t         jetpt_puppi1;
   Float_t         jetpt_puppi2;
   Float_t         jetpt_puppi3;
   Float_t         jetpt_puppi4;
   Float_t         jetpt_puppi5;
   Float_t         jetpt_puppi6;
   Float_t         jetpt_puppi7;
   Float_t         jetpt_puppi8;
   Float_t         jettche1;
   Float_t         jettche2;
   Float_t         jettche3;
   Float_t         jettche4;
   Float_t         jettche5;
   Float_t         jettche6;
   Float_t         jettche7;
   Float_t         jettche8;
   Float_t         jettche_puppi1;
   Float_t         jettche_puppi2;
   Float_t         jettche_puppi3;
   Float_t         jettche_puppi4;
   Float_t         jettche_puppi5;
   Float_t         jettche_puppi6;
   Float_t         jettche_puppi7;
   Float_t         jettche_puppi8;
   Float_t         jettchp1;
   Float_t         jettchp2;
   Float_t         jettchp3;
   Float_t         jettchp4;
   Float_t         jettchp5;
   Float_t         jettchp6;
   Float_t         jettchp7;
   Float_t         jettchp8;
   Float_t         jettchp_puppi1;
   Float_t         jettchp_puppi2;
   Float_t         jettchp_puppi3;
   Float_t         jettchp_puppi4;
   Float_t         jettchp_puppi5;
   Float_t         jettchp_puppi6;
   Float_t         jettchp_puppi7;
   Float_t         jettchp_puppi8;
   Float_t         kfW;
   Float_t         leptonGeneta1;
   Float_t         leptonGeneta2;
   Float_t         leptonGeneta3;
   Float_t         leptonGenphi1;
   Float_t         leptonGenphi2;
   Float_t         leptonGenphi3;
   Float_t         leptonGenpid1;
   Float_t         leptonGenpid2;
   Float_t         leptonGenpid3;
   Float_t         leptonGenpt1;
   Float_t         leptonGenpt2;
   Float_t         leptonGenpt3;
   Float_t         leptonLHEeta1;
   Float_t         leptonLHEeta2;
   Float_t         leptonLHEeta3;
   Float_t         leptonLHEphi1;
   Float_t         leptonLHEphi2;
   Float_t         leptonLHEphi3;
   Float_t         leptonLHEpid1;
   Float_t         leptonLHEpid2;
   Float_t         leptonLHEpid3;
   Float_t         leptonLHEpt1;
   Float_t         leptonLHEpt2;
   Float_t         leptonLHEpt3;
   Float_t         mctruth;
   Float_t         metGeneta;
   Float_t         metGenphi;
   Float_t         metGenpt;
   Float_t         metLHEeta;
   Float_t         metLHEphi;
   Float_t         metLHEpt;
   Float_t         mjj;
   Float_t         mjj_puppi;
   Float_t         mll;
   Float_t         mth;
   Float_t         mth_puppi;
   Float_t         mtw1;
   Float_t         mtw1_puppi;
   Float_t         mtw2;
   Float_t         mtw2_puppi;
   Float_t         nbjet;
   Float_t         nbjet_puppi;
   Float_t         nbjetcsvl;
   Float_t         nbjetcsvlMVA;
   Float_t         nbjetcsvlMVA_puppi;
   Float_t         nbjetcsvl_puppi;
   Float_t         nbjetcsvm;
   Float_t         nbjetcsvmMVA;
   Float_t         nbjetcsvmMVA_puppi;
   Float_t         nbjetcsvm_puppi;
   Float_t         nbjetcsvt;
   Float_t         nbjetcsvtMVA;
   Float_t         nbjetcsvtMVA_puppi;
   Float_t         nbjetcsvt_puppi;
   Float_t         nbjettche;
   Float_t         nbjettche_puppi;
   Float_t         nbjettchp;
   Float_t         nbjettchp_puppi;
   Float_t         nbrem1;
   Float_t         nbrem2;
   Float_t         nbrem3;
   Float_t         nbrem4;
   Float_t         neutrinoGeneta1;
   Float_t         neutrinoGeneta2;
   Float_t         neutrinoGeneta3;
   Float_t         neutrinoGenphi1;
   Float_t         neutrinoGenphi2;
   Float_t         neutrinoGenphi3;
   Float_t         neutrinoGenpid1;
   Float_t         neutrinoGenpid2;
   Float_t         neutrinoGenpid3;
   Float_t         neutrinoGenpt1;
   Float_t         neutrinoGenpt2;
   Float_t         neutrinoGenpt3;
   Float_t         neutrinoLHEeta1;
   Float_t         neutrinoLHEeta2;
   Float_t         neutrinoLHEeta3;
   Float_t         neutrinoLHEphi1;
   Float_t         neutrinoLHEphi2;
   Float_t         neutrinoLHEphi3;
   Float_t         neutrinoLHEpid1;
   Float_t         neutrinoLHEpid2;
   Float_t         neutrinoLHEpid3;
   Float_t         neutrinoLHEpt1;
   Float_t         neutrinoLHEpt2;
   Float_t         neutrinoLHEpt3;
   Float_t         njet;
   Float_t         njet_puppi;
   Float_t         njetid;
   Float_t         njetid_puppi;
   Float_t         njetvb_puppi;
   Float_t         njetvbf;
   Float_t         numbLHE;
   Float_t         numtLHE;
   Float_t         pdfid1;
   Float_t         pdfid2;
   Float_t         pdfscalePDF;
   Float_t         pdfx1;
   Float_t         pdfx1PDF;
   Float_t         pdfx2;
   Float_t         pdfx2PDF;
   Float_t         peaking;
   Float_t         pfChargedHadronIso1;
   Float_t         pfChargedHadronIso2;
   Float_t         pfChargedHadronIso3;
   Float_t         pfChargedHadronIso4;
   Float_t         pfGammaIso1;
   Float_t         pfGammaIso2;
   Float_t         pfGammaIso3;
   Float_t         pfGammaIso4;
   Float_t         pfNeutralHadronIso1;
   Float_t         pfNeutralHadronIso2;
   Float_t         pfNeutralHadronIso3;
   Float_t         pfNeutralHadronIso4;
   Float_t         pfPUfChargedHadronIso1;
   Float_t         pfPUfChargedHadronIso2;
   Float_t         pfPUfChargedHadronIso3;
   Float_t         pfPUfChargedHadronIso4;
   Float_t         pfParticleAllIso1;
   Float_t         pfParticleAllIso2;
   Float_t         pfParticleAllIso3;
   Float_t         pfParticleAllIso4;
   Float_t         pfSumEt;
   Float_t         pfSumEt_puppi;
   Float_t         pfmet;
   Float_t         pfmetMEtSig;
   Float_t         pfmetSignificance;
   Float_t         pfmet_puppi;
   Float_t         pfmetphi;
   Float_t         pfmetphi_puppi;
   Float_t         phi1;
   Float_t         phi2;
   Float_t         phi3;
   Float_t         phi4;
   Float_t         pid1;
   Float_t         pid2;
   Float_t         pid3;
   Float_t         pid4;
   Float_t         ppfmet;
   Float_t         ppfmet_puppi;
   Float_t         pt1;
   Float_t         pt2;
   Float_t         pt3;
   Float_t         pt4;
   Float_t         ptll;
   Float_t         puW;
   Float_t         sceta1;
   Float_t         sceta2;
   Float_t         sceta3;
   Float_t         sceta4;
   Float_t         softbcsv;
   Float_t         softbcsvMVA;
   Float_t         softbjpb;
   Float_t         softtche;
   Float_t         softtchp;
   Float_t         triggAW;
   Float_t         triggBW;
   Float_t         triggW;
   Float_t         trigger;
   //   vector<float>   *std_vector_lepton_eta;
   //   vector<float>   *std_vector_lepton_phi;
   //   vector<float>   *std_vector_lepton_pt;

   /*   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet1;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet1_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet2;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet2_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet3;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet3_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet4;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet4_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet5;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet5_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet6;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet6_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet7;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet7_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet8;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_jet8_puppi;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton1;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton2;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton3;
   ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > *v_lepton4;
   */
   Float_t         worstJetLepPt;
   Float_t         yll;
   Float_t         itpu;
   Float_t         nvtx;
   Float_t         ootpum1;
   Float_t         ootpup1;
   Float_t         trpu;
   Int_t           bveto;
   Int_t           bveto_csvl;
   Int_t           bveto_csvl_ip;
   Int_t           bveto_csvl_ip_puppi;
   Int_t           bveto_csvl_nj;
   Int_t           bveto_csvl_nj05;
   Int_t           bveto_csvl_nj05_puppi;
   Int_t           bveto_csvl_nj30;
   Int_t           bveto_csvl_nj3005;
   Int_t           bveto_csvl_nj3005_puppi;
   Int_t           bveto_csvl_nj30_puppi;
   Int_t           bveto_csvl_nj_puppi;
   Int_t           bveto_csvl_puppi;
   Int_t           bveto_csvm;
   Int_t           bveto_csvm_ip;
   Int_t           bveto_csvm_ip_puppi;
   Int_t           bveto_csvm_nj;
   Int_t           bveto_csvm_nj05;
   Int_t           bveto_csvm_nj05_puppi;
   Int_t           bveto_csvm_nj30;
   Int_t           bveto_csvm_nj3005;
   Int_t           bveto_csvm_nj3005_puppi;
   Int_t           bveto_csvm_nj30_puppi;
   Int_t           bveto_csvm_nj_puppi;
   Int_t           bveto_csvm_puppi;
   Int_t           bveto_csvt;
   Int_t           bveto_csvt_ip;
   Int_t           bveto_csvt_ip_puppi;
   Int_t           bveto_csvt_nj;
   Int_t           bveto_csvt_nj05;
   Int_t           bveto_csvt_nj05_puppi;
   Int_t           bveto_csvt_nj30;
   Int_t           bveto_csvt_nj3005;
   Int_t           bveto_csvt_nj3005_puppi;
   Int_t           bveto_csvt_nj30_puppi;
   Int_t           bveto_csvt_nj_puppi;
   Int_t           bveto_csvt_puppi;
   Int_t           bveto_ip;
   Int_t           bveto_ip_puppi;
   Int_t           bveto_mu;
   Int_t           bveto_munj;
   Int_t           bveto_munj05;
   Int_t           bveto_munj30;
   Int_t           bveto_munj3005;
   Int_t           bveto_nj;
   Int_t           bveto_nj05;
   Int_t           bveto_nj05_puppi;
   Int_t           bveto_nj30;
   Int_t           bveto_nj3005;
   Int_t           bveto_nj3005_puppi;
   Int_t           bveto_nj30_puppi;
   Int_t           bveto_nj_puppi;
   Int_t           bveto_puppi;
   Int_t           dphiveto;
   Int_t           nextra;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          event;

   // List of branches
   TBranch        *b_HEPMCweight;   //!
   TBranch        *b_HEPMCweightFac0;   //!
   TBranch        *b_HEPMCweightFac1;   //!
   TBranch        *b_HEPMCweightFac2;   //!
   TBranch        *b_HEPMCweightFac3;   //!
   TBranch        *b_HEPMCweightFac4;   //!
   TBranch        *b_HEPMCweightFac5;   //!
   TBranch        *b_HEPMCweightFac6;   //!
   TBranch        *b_HEPMCweightRen0;   //!
   TBranch        *b_HEPMCweightRen1;   //!
   TBranch        *b_HEPMCweightRen2;   //!
   TBranch        *b_HEPMCweightRen3;   //!
   TBranch        *b_HEPMCweightRen4;   //!
   TBranch        *b_HEPMCweightRen5;   //!
   TBranch        *b_HEPMCweightRen6;   //!
   TBranch        *b_HEPMCweightScale0;   //!
   TBranch        *b_HEPMCweightScale1;   //!
   TBranch        *b_HEPMCweightScale2;   //!
   TBranch        *b_HEPMCweightScale3;   //!
   TBranch        *b_HEPMCweightScale4;   //!
   TBranch        *b_HEPMCweightScale5;   //!
   TBranch        *b_HEPMCweightScale6;   //!
   TBranch        *b_MHiggs;   //!
   TBranch        *b_PtHiggs;   //!
   TBranch        *b_baseW;   //!
   TBranch        *b_bdt1;   //!
   TBranch        *b_bdt2;   //!
   TBranch        *b_bdt3;   //!
   TBranch        *b_bdt4;   //!
   TBranch        *b_ch1;   //!
   TBranch        *b_ch2;   //!
   TBranch        *b_ch3;   //!
   TBranch        *b_ch4;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_cjeteta1;   //!
   TBranch        *b_cjeteta1_puppi;   //!
   TBranch        *b_cjeteta2;   //!
   TBranch        *b_cjeteta2_puppi;   //!
   TBranch        *b_cjetid1;   //!
   TBranch        *b_cjetid1_puppi;   //!
   TBranch        *b_cjetid2;   //!
   TBranch        *b_cjetid2_puppi;   //!
   TBranch        *b_cjetmva1;   //!
   TBranch        *b_cjetmva1_puppi;   //!
   TBranch        *b_cjetmva2;   //!
   TBranch        *b_cjetmva2_puppi;   //!
   TBranch        *b_cjetphi1;   //!
   TBranch        *b_cjetphi1_puppi;   //!
   TBranch        *b_cjetphi2;   //!
   TBranch        *b_cjetphi2_puppi;   //!
   TBranch        *b_cjetpileupidFlag1;   //!
   TBranch        *b_cjetpileupidFlag1_puppi;   //!
   TBranch        *b_cjetpileupidFlag2;   //!
   TBranch        *b_cjetpileupidFlag2_puppi;   //!
   TBranch        *b_cjetpileupidValue1;   //!
   TBranch        *b_cjetpileupidValue1_puppi;   //!
   TBranch        *b_cjetpileupidValue2;   //!
   TBranch        *b_cjetpileupidValue2_puppi;   //!
   TBranch        *b_cjetpt1;   //!
   TBranch        *b_cjetpt1_puppi;   //!
   TBranch        *b_cjetpt2;   //!
   TBranch        *b_cjetpt2_puppi;   //!
   TBranch        *b_dataset;   //!
   TBranch        *b_detajj;   //!
   TBranch        *b_detajj_puppi;   //!
   TBranch        *b_dphilMet1_puppi;   //!
   TBranch        *b_dphilMet2_puppi;   //!
   TBranch        *b_dphilMet_puppi;   //!
   TBranch        *b_dphill;   //!
   TBranch        *b_dphillJet_puppi;   //!
   TBranch        *b_dphillMet_puppi;   //!
   TBranch        *b_dphilljet;   //!
   TBranch        *b_dphilljetjet;   //!
   TBranch        *b_dphilljetjet_puppi;   //!
   TBranch        *b_dphillmet;   //!
   TBranch        *b_dphilmet;   //!
   TBranch        *b_dphilmet1;   //!
   TBranch        *b_dphilmet2;   //!
   TBranch        *b_drll;   //!
   TBranch        *b_effAW;   //!
   TBranch        *b_effBW;   //!
   TBranch        *b_effW;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_eta3;   //!
   TBranch        *b_eta4;   //!
   TBranch        *b_fakeAW;   //!
   TBranch        *b_fakeBW;   //!
   TBranch        *b_fakeW;   //!
   TBranch        *b_fermiW;   //!
   TBranch        *b_fourW;   //!
   TBranch        *b_gammaMRStar;   //!
   TBranch        *b_hardbcsv;   //!
   TBranch        *b_hardbcsvMVA;   //!
   TBranch        *b_hardbjpb;   //!
   TBranch        *b_hardtche;   //!
   TBranch        *b_hardtchp;   //!
   TBranch        *b_higggsGenpt;   //!
   TBranch        *b_higgsLHEpt;   //!
   TBranch        *b_isHighPtMuon1;   //!
   TBranch        *b_isHighPtMuon2;   //!
   TBranch        *b_isHighPtMuon3;   //!
   TBranch        *b_isHighPtMuon4;   //!
   TBranch        *b_isLooseElectron1;   //!
   TBranch        *b_isLooseElectron2;   //!
   TBranch        *b_isLooseElectron3;   //!
   TBranch        *b_isLooseElectron4;   //!
   TBranch        *b_isLooseMuon1;   //!
   TBranch        *b_isLooseMuon2;   //!
   TBranch        *b_isLooseMuon3;   //!
   TBranch        *b_isLooseMuon4;   //!
   TBranch        *b_isLooseRobustElectron1;   //!
   TBranch        *b_isLooseRobustElectron2;   //!
   TBranch        *b_isLooseRobustElectron3;   //!
   TBranch        *b_isLooseRobustElectron4;   //!
   TBranch        *b_isRobustHighEnergyElectron1;   //!
   TBranch        *b_isRobustHighEnergyElectron2;   //!
   TBranch        *b_isRobustHighEnergyElectron3;   //!
   TBranch        *b_isRobustHighEnergyElectron4;   //!
   TBranch        *b_isSTA1;   //!
   TBranch        *b_isSTA2;   //!
   TBranch        *b_isSTA3;   //!
   TBranch        *b_isSTA4;   //!
   TBranch        *b_isSoftMuon1;   //!
   TBranch        *b_isSoftMuon2;   //!
   TBranch        *b_isSoftMuon3;   //!
   TBranch        *b_isSoftMuon4;   //!
   TBranch        *b_isTightElectron1;   //!
   TBranch        *b_isTightElectron2;   //!
   TBranch        *b_isTightElectron3;   //!
   TBranch        *b_isTightElectron4;   //!
   TBranch        *b_isTightMuon1;   //!
   TBranch        *b_isTightMuon2;   //!
   TBranch        *b_isTightMuon3;   //!
   TBranch        *b_isTightMuon4;   //!
   TBranch        *b_isTightRobustElectron1;   //!
   TBranch        *b_isTightRobustElectron2;   //!
   TBranch        *b_isTightRobustElectron3;   //!
   TBranch        *b_isTightRobustElectron4;   //!
   TBranch        *b_isomva1;   //!
   TBranch        *b_isomva2;   //!
   TBranch        *b_isomva3;   //!
   TBranch        *b_isomva4;   //!
   TBranch        *b_jetGenPartoneta1;   //!
   TBranch        *b_jetGenPartoneta2;   //!
   TBranch        *b_jetGenPartoneta3;   //!
   TBranch        *b_jetGenPartonphi1;   //!
   TBranch        *b_jetGenPartonphi2;   //!
   TBranch        *b_jetGenPartonphi3;   //!
   TBranch        *b_jetGenPartonpid1;   //!
   TBranch        *b_jetGenPartonpid2;   //!
   TBranch        *b_jetGenPartonpid3;   //!
   TBranch        *b_jetGenPartonpt1;   //!
   TBranch        *b_jetGenPartonpt2;   //!
   TBranch        *b_jetGenPartonpt3;   //!
   TBranch        *b_jetGeneta1;   //!
   TBranch        *b_jetGeneta2;   //!
   TBranch        *b_jetGeneta3;   //!
   TBranch        *b_jetGeneta4;   //!
   TBranch        *b_jetGeneta5;   //!
   TBranch        *b_jetGenphi1;   //!
   TBranch        *b_jetGenphi2;   //!
   TBranch        *b_jetGenphi3;   //!
   TBranch        *b_jetGenphi4;   //!
   TBranch        *b_jetGenphi5;   //!
   TBranch        *b_jetGenpt1;   //!
   TBranch        *b_jetGenpt2;   //!
   TBranch        *b_jetGenpt3;   //!
   TBranch        *b_jetGenpt4;   //!
   TBranch        *b_jetGenpt5;   //!
   TBranch        *b_jetLHEPartoneta1;   //!
   TBranch        *b_jetLHEPartoneta2;   //!
   TBranch        *b_jetLHEPartoneta3;   //!
   TBranch        *b_jetLHEPartonphi1;   //!
   TBranch        *b_jetLHEPartonphi2;   //!
   TBranch        *b_jetLHEPartonphi3;   //!
   TBranch        *b_jetLHEPartonpid1;   //!
   TBranch        *b_jetLHEPartonpid2;   //!
   TBranch        *b_jetLHEPartonpid3;   //!
   TBranch        *b_jetLHEPartonpt1;   //!
   TBranch        *b_jetLHEPartonpt2;   //!
   TBranch        *b_jetLHEPartonpt3;   //!
   TBranch        *b_jetRho;   //!
   TBranch        *b_jetbjpb1;   //!
   TBranch        *b_jetbjpb2;   //!
   TBranch        *b_jetbjpb3;   //!
   TBranch        *b_jetbjpb4;   //!
   TBranch        *b_jetbjpb5;   //!
   TBranch        *b_jetbjpb6;   //!
   TBranch        *b_jetbjpb7;   //!
   TBranch        *b_jetbjpb8;   //!
   TBranch        *b_jetbjpb_puppi1;   //!
   TBranch        *b_jetbjpb_puppi2;   //!
   TBranch        *b_jetbjpb_puppi3;   //!
   TBranch        *b_jetbjpb_puppi4;   //!
   TBranch        *b_jetbjpb_puppi5;   //!
   TBranch        *b_jetbjpb_puppi6;   //!
   TBranch        *b_jetbjpb_puppi7;   //!
   TBranch        *b_jetbjpb_puppi8;   //!
   TBranch        *b_jetcsv1;   //!
   TBranch        *b_jetcsv2;   //!
   TBranch        *b_jetcsv3;   //!
   TBranch        *b_jetcsv4;   //!
   TBranch        *b_jetcsv5;   //!
   TBranch        *b_jetcsv6;   //!
   TBranch        *b_jetcsv7;   //!
   TBranch        *b_jetcsv8;   //!
   TBranch        *b_jetcsvMVA1;   //!
   TBranch        *b_jetcsvMVA2;   //!
   TBranch        *b_jetcsvMVA3;   //!
   TBranch        *b_jetcsvMVA4;   //!
   TBranch        *b_jetcsvMVA5;   //!
   TBranch        *b_jetcsvMVA6;   //!
   TBranch        *b_jetcsvMVA7;   //!
   TBranch        *b_jetcsvMVA8;   //!
   TBranch        *b_jetcsvMVA_puppi1;   //!
   TBranch        *b_jetcsvMVA_puppi2;   //!
   TBranch        *b_jetcsvMVA_puppi3;   //!
   TBranch        *b_jetcsvMVA_puppi4;   //!
   TBranch        *b_jetcsvMVA_puppi5;   //!
   TBranch        *b_jetcsvMVA_puppi6;   //!
   TBranch        *b_jetcsvMVA_puppi7;   //!
   TBranch        *b_jetcsvMVA_puppi8;   //!
   TBranch        *b_jetcsv_puppi1;   //!
   TBranch        *b_jetcsv_puppi2;   //!
   TBranch        *b_jetcsv_puppi3;   //!
   TBranch        *b_jetcsv_puppi4;   //!
   TBranch        *b_jetcsv_puppi5;   //!
   TBranch        *b_jetcsv_puppi6;   //!
   TBranch        *b_jetcsv_puppi7;   //!
   TBranch        *b_jetcsv_puppi8;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_jeteta3;   //!
   TBranch        *b_jeteta4;   //!
   TBranch        *b_jeteta5;   //!
   TBranch        *b_jeteta6;   //!
   TBranch        *b_jeteta7;   //!
   TBranch        *b_jeteta8;   //!
   TBranch        *b_jeteta_puppi1;   //!
   TBranch        *b_jeteta_puppi2;   //!
   TBranch        *b_jeteta_puppi3;   //!
   TBranch        *b_jeteta_puppi4;   //!
   TBranch        *b_jeteta_puppi5;   //!
   TBranch        *b_jeteta_puppi6;   //!
   TBranch        *b_jeteta_puppi7;   //!
   TBranch        *b_jeteta_puppi8;   //!
   TBranch        *b_jetid1;   //!
   TBranch        *b_jetid2;   //!
   TBranch        *b_jetid3;   //!
   TBranch        *b_jetid4;   //!
   TBranch        *b_jetid5;   //!
   TBranch        *b_jetid6;   //!
   TBranch        *b_jetid7;   //!
   TBranch        *b_jetid8;   //!
   TBranch        *b_jetid_puppi1;   //!
   TBranch        *b_jetid_puppi2;   //!
   TBranch        *b_jetid_puppi4;   //!
   TBranch        *b_jetid_puppi5;   //!
   TBranch        *b_jetid_puppi6;   //!
   TBranch        *b_jetid_puppi7;   //!
   TBranch        *b_jetid_puppi8;   //!
   TBranch        *b_jetmass1;   //!
   TBranch        *b_jetmass2;   //!
   TBranch        *b_jetmass3;   //!
   TBranch        *b_jetmass4;   //!
   TBranch        *b_jetmass5;   //!
   TBranch        *b_jetmass6;   //!
   TBranch        *b_jetmass7;   //!
   TBranch        *b_jetmass8;   //!
   TBranch        *b_jetmass_puppi1;   //!
   TBranch        *b_jetmass_puppi2;   //!
   TBranch        *b_jetmass_puppi3;   //!
   TBranch        *b_jetmass_puppi4;   //!
   TBranch        *b_jetmass_puppi5;   //!
   TBranch        *b_jetmass_puppi6;   //!
   TBranch        *b_jetmass_puppi7;   //!
   TBranch        *b_jetmass_puppi8;   //!
   TBranch        *b_jetmva1;   //!
   TBranch        *b_jetmva2;   //!
   TBranch        *b_jetmva3;   //!
   TBranch        *b_jetmva4;   //!
   TBranch        *b_jetmva5;   //!
   TBranch        *b_jetmva6;   //!
   TBranch        *b_jetmva7;   //!
   TBranch        *b_jetmva8;   //!
   TBranch        *b_jetmva_puppi1;   //!
   TBranch        *b_jetmva_puppi2;   //!
   TBranch        *b_jetmva_puppi3;   //!
   TBranch        *b_jetmva_puppi4;   //!
   TBranch        *b_jetmva_puppi5;   //!
   TBranch        *b_jetmva_puppi6;   //!
   TBranch        *b_jetmva_puppi7;   //!
   TBranch        *b_jetmva_puppi8;   //!
   TBranch        *b_jetphi1;   //!
   TBranch        *b_jetphi2;   //!
   TBranch        *b_jetphi3;   //!
   TBranch        *b_jetphi4;   //!
   TBranch        *b_jetphi5;   //!
   TBranch        *b_jetphi6;   //!
   TBranch        *b_jetphi7;   //!
   TBranch        *b_jetphi8;   //!
   TBranch        *b_jetphi_puppi1;   //!
   TBranch        *b_jetphi_puppi2;   //!
   TBranch        *b_jetphi_puppi3;   //!
   TBranch        *b_jetphi_puppi4;   //!
   TBranch        *b_jetphi_puppi5;   //!
   TBranch        *b_jetphi_puppi6;   //!
   TBranch        *b_jetphi_puppi7;   //!
   TBranch        *b_jetphi_puppi8;   //!
   TBranch        *b_jetpileupidFlag1;   //!
   TBranch        *b_jetpileupidFlag2;   //!
   TBranch        *b_jetpileupidFlag3;   //!
   TBranch        *b_jetpileupidFlag4;   //!
   TBranch        *b_jetpileupidFlag5;   //!
   TBranch        *b_jetpileupidFlag6;   //!
   TBranch        *b_jetpileupidFlag7;   //!
   TBranch        *b_jetpileupidFlag8;   //!
   TBranch        *b_jetpileupidFlag_puppi1;   //!
   TBranch        *b_jetpileupidFlag_puppi2;   //!
   TBranch        *b_jetpileupidFlag_puppi3;   //!
   TBranch        *b_jetpileupidFlag_puppi4;   //!
   TBranch        *b_jetpileupidFlag_puppi5;   //!
   TBranch        *b_jetpileupidFlag_puppi6;   //!
   TBranch        *b_jetpileupidFlag_puppi7;   //!
   TBranch        *b_jetpileupidFlag_puppi8;   //!
   TBranch        *b_jetpileupidValue1;   //!
   TBranch        *b_jetpileupidValue2;   //!
   TBranch        *b_jetpileupidValue3;   //!
   TBranch        *b_jetpileupidValue4;   //!
   TBranch        *b_jetpileupidValue5;   //!
   TBranch        *b_jetpileupidValue6;   //!
   TBranch        *b_jetpileupidValue7;   //!
   TBranch        *b_jetpileupidValue8;   //!
   TBranch        *b_jetpileupidValue_puppi1;   //!
   TBranch        *b_jetpileupidValue_puppi2;   //!
   TBranch        *b_jetpileupidValue_puppi3;   //!
   TBranch        *b_jetpileupidValue_puppi4;   //!
   TBranch        *b_jetpileupidValue_puppi5;   //!
   TBranch        *b_jetpileupidValue_puppi6;   //!
   TBranch        *b_jetpileupidValue_puppi7;   //!
   TBranch        *b_jetpileupidValue_puppi8;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jetpt3;   //!
   TBranch        *b_jetpt4;   //!
   TBranch        *b_jetpt5;   //!
   TBranch        *b_jetpt6;   //!
   TBranch        *b_jetpt7;   //!
   TBranch        *b_jetpt8;   //!
   TBranch        *b_jetpt_puppi1;   //!
   TBranch        *b_jetpt_puppi2;   //!
   TBranch        *b_jetpt_puppi3;   //!
   TBranch        *b_jetpt_puppi4;   //!
   TBranch        *b_jetpt_puppi5;   //!
   TBranch        *b_jetpt_puppi6;   //!
   TBranch        *b_jetpt_puppi7;   //!
   TBranch        *b_jetpt_puppi8;   //!
   TBranch        *b_jettche1;   //!
   TBranch        *b_jettche2;   //!
   TBranch        *b_jettche3;   //!
   TBranch        *b_jettche4;   //!
   TBranch        *b_jettche5;   //!
   TBranch        *b_jettche6;   //!
   TBranch        *b_jettche7;   //!
   TBranch        *b_jettche8;   //!
   TBranch        *b_jettche_puppi1;   //!
   TBranch        *b_jettche_puppi2;   //!
   TBranch        *b_jettche_puppi3;   //!
   TBranch        *b_jettche_puppi4;   //!
   TBranch        *b_jettche_puppi5;   //!
   TBranch        *b_jettche_puppi6;   //!
   TBranch        *b_jettche_puppi7;   //!
   TBranch        *b_jettche_puppi8;   //!
   TBranch        *b_jettchp1;   //!
   TBranch        *b_jettchp2;   //!
   TBranch        *b_jettchp3;   //!
   TBranch        *b_jettchp4;   //!
   TBranch        *b_jettchp5;   //!
   TBranch        *b_jettchp6;   //!
   TBranch        *b_jettchp7;   //!
   TBranch        *b_jettchp8;   //!
   TBranch        *b_jettchp_puppi1;   //!
   TBranch        *b_jettchp_puppi2;   //!
   TBranch        *b_jettchp_puppi3;   //!
   TBranch        *b_jettchp_puppi4;   //!
   TBranch        *b_jettchp_puppi5;   //!
   TBranch        *b_jettchp_puppi6;   //!
   TBranch        *b_jettchp_puppi7;   //!
   TBranch        *b_jettchp_puppi8;   //!
   TBranch        *b_kfW;   //!
   TBranch        *b_leptonGeneta1;   //!
   TBranch        *b_leptonGeneta2;   //!
   TBranch        *b_leptonGeneta3;   //!
   TBranch        *b_leptonGenphi1;   //!
   TBranch        *b_leptonGenphi2;   //!
   TBranch        *b_leptonGenphi3;   //!
   TBranch        *b_leptonGenpid1;   //!
   TBranch        *b_leptonGenpid2;   //!
   TBranch        *b_leptonGenpid3;   //!
   TBranch        *b_leptonGenpt1;   //!
   TBranch        *b_leptonGenpt2;   //!
   TBranch        *b_leptonGenpt3;   //!
   TBranch        *b_leptonLHEeta1;   //!
   TBranch        *b_leptonLHEeta2;   //!
   TBranch        *b_leptonLHEeta3;   //!
   TBranch        *b_leptonLHEphi1;   //!
   TBranch        *b_leptonLHEphi2;   //!
   TBranch        *b_leptonLHEphi3;   //!
   TBranch        *b_leptonLHEpid1;   //!
   TBranch        *b_leptonLHEpid2;   //!
   TBranch        *b_leptonLHEpid3;   //!
   TBranch        *b_leptonLHEpt1;   //!
   TBranch        *b_leptonLHEpt2;   //!
   TBranch        *b_leptonLHEpt3;   //!
   TBranch        *b_mctruth;   //!
   TBranch        *b_metGeneta;   //!
   TBranch        *b_metGenphi;   //!
   TBranch        *b_metGenpt;   //!
   TBranch        *b_metLHEeta;   //!
   TBranch        *b_metLHEphi;   //!
   TBranch        *b_metLHEpt;   //!
   TBranch        *b_mjj;   //!
   TBranch        *b_mjj_puppi;   //!
   TBranch        *b_mll;   //!
   TBranch        *b_mth;   //!
   TBranch        *b_mth_puppi;   //!
   TBranch        *b_mtw1;   //!
   TBranch        *b_mtw1_puppi;   //!
   TBranch        *b_mtw2;   //!
   TBranch        *b_mtw2_puppi;   //!
   TBranch        *b_nbjet;   //!
   TBranch        *b_nbjet_puppi;   //!
   TBranch        *b_nbjetcsvl;   //!
   TBranch        *b_nbjetcsvlMVA;   //!
   TBranch        *b_nbjetcsvlMVA_puppi;   //!
   TBranch        *b_nbjetcsvl_puppi;   //!
   TBranch        *b_nbjetcsvm;   //!
   TBranch        *b_nbjetcsvmMVA;   //!
   TBranch        *b_nbjetcsvmMVA_puppi;   //!
   TBranch        *b_nbjetcsvm_puppi;   //!
   TBranch        *b_nbjetcsvt;   //!
   TBranch        *b_nbjetcsvtMVA;   //!
   TBranch        *b_nbjetcsvtMVA_puppi;   //!
   TBranch        *b_nbjetcsvt_puppi;   //!
   TBranch        *b_nbjettche;   //!
   TBranch        *b_nbjettche_puppi;   //!
   TBranch        *b_nbjettchp;   //!
   TBranch        *b_nbjettchp_puppi;   //!
   TBranch        *b_nbrem1;   //!
   TBranch        *b_nbrem2;   //!
   TBranch        *b_nbrem3;   //!
   TBranch        *b_nbrem4;   //!
   TBranch        *b_neutrinoGeneta1;   //!
   TBranch        *b_neutrinoGeneta2;   //!
   TBranch        *b_neutrinoGeneta3;   //!
   TBranch        *b_neutrinoGenphi1;   //!
   TBranch        *b_neutrinoGenphi2;   //!
   TBranch        *b_neutrinoGenphi3;   //!
   TBranch        *b_neutrinoGenpid1;   //!
   TBranch        *b_neutrinoGenpid2;   //!
   TBranch        *b_neutrinoGenpid3;   //!
   TBranch        *b_neutrinoGenpt1;   //!
   TBranch        *b_neutrinoGenpt2;   //!
   TBranch        *b_neutrinoGenpt3;   //!
   TBranch        *b_neutrinoLHEeta1;   //!
   TBranch        *b_neutrinoLHEeta2;   //!
   TBranch        *b_neutrinoLHEeta3;   //!
   TBranch        *b_neutrinoLHEphi1;   //!
   TBranch        *b_neutrinoLHEphi2;   //!
   TBranch        *b_neutrinoLHEphi3;   //!
   TBranch        *b_neutrinoLHEpid1;   //!
   TBranch        *b_neutrinoLHEpid2;   //!
   TBranch        *b_neutrinoLHEpid3;   //!
   TBranch        *b_neutrinoLHEpt1;   //!
   TBranch        *b_neutrinoLHEpt2;   //!
   TBranch        *b_neutrinoLHEpt3;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_njet_puppi;   //!
   TBranch        *b_njetid;   //!
   TBranch        *b_njetid_puppi;   //!
   TBranch        *b_njetvb_puppi;   //!
   TBranch        *b_njetvbf;   //!
   TBranch        *b_numbLHE;   //!
   TBranch        *b_numtLHE;   //!
   TBranch        *b_pdfid1;   //!
   TBranch        *b_pdfid2;   //!
   TBranch        *b_pdfscalePDF;   //!
   TBranch        *b_pdfx1;   //!
   TBranch        *b_pdfx1PDF;   //!
   TBranch        *b_pdfx2;   //!
   TBranch        *b_pdfx2PDF;   //!
   TBranch        *b_peaking;   //!
   TBranch        *b_pfChargedHadronIso1;   //!
   TBranch        *b_pfChargedHadronIso2;   //!
   TBranch        *b_pfChargedHadronIso3;   //!
   TBranch        *b_pfChargedHadronIso4;   //!
   TBranch        *b_pfGammaIso1;   //!
   TBranch        *b_pfGammaIso2;   //!
   TBranch        *b_pfGammaIso3;   //!
   TBranch        *b_pfGammaIso4;   //!
   TBranch        *b_pfNeutralHadronIso1;   //!
   TBranch        *b_pfNeutralHadronIso2;   //!
   TBranch        *b_pfNeutralHadronIso3;   //!
   TBranch        *b_pfNeutralHadronIso4;   //!
   TBranch        *b_pfPUfChargedHadronIso1;   //!
   TBranch        *b_pfPUfChargedHadronIso2;   //!
   TBranch        *b_pfPUfChargedHadronIso3;   //!
   TBranch        *b_pfPUfChargedHadronIso4;   //!
   TBranch        *b_pfParticleAllIso1;   //!
   TBranch        *b_pfParticleAllIso2;   //!
   TBranch        *b_pfParticleAllIso3;   //!
   TBranch        *b_pfParticleAllIso4;   //!
   TBranch        *b_pfSumEt;   //!
   TBranch        *b_pfSumEt_puppi;   //!
   TBranch        *b_pfmet;   //!
   TBranch        *b_pfmetMEtSig;   //!
   TBranch        *b_pfmetSignificance;   //!
   TBranch        *b_pfmet_puppi;   //!
   TBranch        *b_pfmetphi;   //!
   TBranch        *b_pfmetphi_puppi;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_phi3;   //!
   TBranch        *b_phi4;   //!
   TBranch        *b_pid1;   //!
   TBranch        *b_pid2;   //!
   TBranch        *b_pid3;   //!
   TBranch        *b_pid4;   //!
   TBranch        *b_ppfmet;   //!
   TBranch        *b_ppfmet_puppi;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_pt3;   //!
   TBranch        *b_pt4;   //!
   TBranch        *b_ptll;   //!
   TBranch        *b_puW;   //!
   TBranch        *b_sceta1;   //!
   TBranch        *b_sceta2;   //!
   TBranch        *b_sceta3;   //!
   TBranch        *b_sceta4;   //!
   TBranch        *b_softbcsv;   //!
   TBranch        *b_softbcsvMVA;   //!
   TBranch        *b_softbjpb;   //!
   TBranch        *b_softtche;   //!
   TBranch        *b_softtchp;   //!
   //   TBranch        *b_std_vector_lepton_eta;   //!
   //   TBranch        *b_std_vector_lepton_phi;   //!
   //   TBranch        *b_std_vector_lepton_pt;   //!
   TBranch        *b_triggAW;   //!
   TBranch        *b_triggBW;   //!
   TBranch        *b_triggW;   //!
   TBranch        *b_trigger;   //!
   TBranch        *b_worstJetLepPt;   //!
   TBranch        *b_yll;   //!
   TBranch        *b_itpu;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_ootpum1;   //!
   TBranch        *b_ootpup1;   //!
   TBranch        *b_trpu;   //!
   TBranch        *b_bveto;   //!
   TBranch        *b_bveto_csvl;   //!
   TBranch        *b_bveto_csvl_ip;   //!
   TBranch        *b_bveto_csvl_ip_puppi;   //!
   TBranch        *b_bveto_csvl_nj;   //!
   TBranch        *b_bveto_csvl_nj05;   //!
   TBranch        *b_bveto_csvl_nj05_puppi;   //!
   TBranch        *b_bveto_csvl_nj30;   //!
   TBranch        *b_bveto_csvl_nj3005;   //!
   TBranch        *b_bveto_csvl_nj3005_puppi;   //!
   TBranch        *b_bveto_csvl_nj30_puppi;   //!
   TBranch        *b_bveto_csvl_nj_puppi;   //!
   TBranch        *b_bveto_csvl_puppi;   //!
   TBranch        *b_bveto_csvm;   //!
   TBranch        *b_bveto_csvm_ip;   //!
   TBranch        *b_bveto_csvm_ip_puppi;   //!
   TBranch        *b_bveto_csvm_nj;   //!
   TBranch        *b_bveto_csvm_nj05;   //!
   TBranch        *b_bveto_csvm_nj05_puppi;   //!
   TBranch        *b_bveto_csvm_nj30;   //!
   TBranch        *b_bveto_csvm_nj3005;   //!
   TBranch        *b_bveto_csvm_nj3005_puppi;   //!
   TBranch        *b_bveto_csvm_nj30_puppi;   //!
   TBranch        *b_bveto_csvm_nj_puppi;   //!
   TBranch        *b_bveto_csvm_puppi;   //!
   TBranch        *b_bveto_csvt;   //!
   TBranch        *b_bveto_csvt_ip;   //!
   TBranch        *b_bveto_csvt_ip_puppi;   //!
   TBranch        *b_bveto_csvt_nj;   //!
   TBranch        *b_bveto_csvt_nj05;   //!
   TBranch        *b_bveto_csvt_nj05_puppi;   //!
   TBranch        *b_bveto_csvt_nj30;   //!
   TBranch        *b_bveto_csvt_nj3005;   //!
   TBranch        *b_bveto_csvt_nj3005_puppi;   //!
   TBranch        *b_bveto_csvt_nj30_puppi;   //!
   TBranch        *b_bveto_csvt_nj_puppi;   //!
   TBranch        *b_bveto_csvt_puppi;   //!
   TBranch        *b_bveto_ip;   //!
   TBranch        *b_bveto_ip_puppi;   //!
   TBranch        *b_bveto_mu;   //!
   TBranch        *b_bveto_munj;   //!
   TBranch        *b_bveto_munj05;   //!
   TBranch        *b_bveto_munj30;   //!
   TBranch        *b_bveto_munj3005;   //!
   TBranch        *b_bveto_nj;   //!
   TBranch        *b_bveto_nj05;   //!
   TBranch        *b_bveto_nj05_puppi;   //!
   TBranch        *b_bveto_nj30;   //!
   TBranch        *b_bveto_nj3005;   //!
   TBranch        *b_bveto_nj3005_puppi;   //!
   TBranch        *b_bveto_nj30_puppi;   //!
   TBranch        *b_bveto_nj_puppi;   //!
   TBranch        *b_bveto_puppi;   //!
   TBranch        *b_dphiveto;   //!
   TBranch        *b_nextra;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!

   readTreeLatino(TTree *tree=0);
   virtual ~readTreeLatino();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readTreeLatino_cxx
readTreeLatino::readTreeLatino(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data2/rgerosa/LATINO_VBS/wpwpjj_qed_4_qcd_0_GEM2019Upg14DR-Phase1age1kJan23_PU140BX25_PH1_1K_FB_V3-v1/output_1_1_gaY.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data2/rgerosa/LATINO_VBS/wpwpjj_qed_4_qcd_0_GEM2019Upg14DR-Phase1age1kJan23_PU140BX25_PH1_1K_FB_V3-v1/output_1_1_gaY.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/data2/rgerosa/LATINO_VBS/wpwpjj_qed_4_qcd_0_GEM2019Upg14DR-Phase1age1kJan23_PU140BX25_PH1_1K_FB_V3-v1/output_1_1_gaY.root:/stepBTree");
      dir->GetObject("probe_tree",tree);

   }
   Init(tree);
}

readTreeLatino::~readTreeLatino()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readTreeLatino::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readTreeLatino::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void readTreeLatino::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
  //   std_vector_lepton_eta = 0;
  //   std_vector_lepton_phi = 0;
  //   std_vector_lepton_pt = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("HEPMCweight", &HEPMCweight, &b_HEPMCweight);
   fChain->SetBranchAddress("HEPMCweightFac0", &HEPMCweightFac0, &b_HEPMCweightFac0);
   fChain->SetBranchAddress("HEPMCweightFac1", &HEPMCweightFac1, &b_HEPMCweightFac1);
   fChain->SetBranchAddress("HEPMCweightFac2", &HEPMCweightFac2, &b_HEPMCweightFac2);
   fChain->SetBranchAddress("HEPMCweightFac3", &HEPMCweightFac3, &b_HEPMCweightFac3);
   fChain->SetBranchAddress("HEPMCweightFac4", &HEPMCweightFac4, &b_HEPMCweightFac4);
   fChain->SetBranchAddress("HEPMCweightFac5", &HEPMCweightFac5, &b_HEPMCweightFac5);
   fChain->SetBranchAddress("HEPMCweightFac6", &HEPMCweightFac6, &b_HEPMCweightFac6);
   fChain->SetBranchAddress("HEPMCweightRen0", &HEPMCweightRen0, &b_HEPMCweightRen0);
   fChain->SetBranchAddress("HEPMCweightRen1", &HEPMCweightRen1, &b_HEPMCweightRen1);
   fChain->SetBranchAddress("HEPMCweightRen2", &HEPMCweightRen2, &b_HEPMCweightRen2);
   fChain->SetBranchAddress("HEPMCweightRen3", &HEPMCweightRen3, &b_HEPMCweightRen3);
   fChain->SetBranchAddress("HEPMCweightRen4", &HEPMCweightRen4, &b_HEPMCweightRen4);
   fChain->SetBranchAddress("HEPMCweightRen5", &HEPMCweightRen5, &b_HEPMCweightRen5);
   fChain->SetBranchAddress("HEPMCweightRen6", &HEPMCweightRen6, &b_HEPMCweightRen6);
   fChain->SetBranchAddress("HEPMCweightScale0", &HEPMCweightScale0, &b_HEPMCweightScale0);
   fChain->SetBranchAddress("HEPMCweightScale1", &HEPMCweightScale1, &b_HEPMCweightScale1);
   fChain->SetBranchAddress("HEPMCweightScale2", &HEPMCweightScale2, &b_HEPMCweightScale2);
   fChain->SetBranchAddress("HEPMCweightScale3", &HEPMCweightScale3, &b_HEPMCweightScale3);
   fChain->SetBranchAddress("HEPMCweightScale4", &HEPMCweightScale4, &b_HEPMCweightScale4);
   fChain->SetBranchAddress("HEPMCweightScale5", &HEPMCweightScale5, &b_HEPMCweightScale5);
   fChain->SetBranchAddress("HEPMCweightScale6", &HEPMCweightScale6, &b_HEPMCweightScale6);
   fChain->SetBranchAddress("MHiggs", &MHiggs, &b_MHiggs);
   fChain->SetBranchAddress("PtHiggs", &PtHiggs, &b_PtHiggs);
   fChain->SetBranchAddress("baseW", &baseW, &b_baseW);
   fChain->SetBranchAddress("bdt1", &bdt1, &b_bdt1);
   fChain->SetBranchAddress("bdt2", &bdt2, &b_bdt2);
   fChain->SetBranchAddress("bdt3", &bdt3, &b_bdt3);
   fChain->SetBranchAddress("bdt4", &bdt4, &b_bdt4);
   fChain->SetBranchAddress("ch1", &ch1, &b_ch1);
   fChain->SetBranchAddress("ch2", &ch2, &b_ch2);
   fChain->SetBranchAddress("ch3", &ch3, &b_ch3);
   fChain->SetBranchAddress("ch4", &ch4, &b_ch4);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("cjeteta1", &cjeteta1, &b_cjeteta1);
   fChain->SetBranchAddress("cjeteta1_puppi", &cjeteta1_puppi, &b_cjeteta1_puppi);
   fChain->SetBranchAddress("cjeteta2", &cjeteta2, &b_cjeteta2);
   fChain->SetBranchAddress("cjeteta2_puppi", &cjeteta2_puppi, &b_cjeteta2_puppi);
   fChain->SetBranchAddress("cjetid1", &cjetid1, &b_cjetid1);
   fChain->SetBranchAddress("cjetid1_puppi", &cjetid1_puppi, &b_cjetid1_puppi);
   fChain->SetBranchAddress("cjetid2", &cjetid2, &b_cjetid2);
   fChain->SetBranchAddress("cjetid2_puppi", &cjetid2_puppi, &b_cjetid2_puppi);
   fChain->SetBranchAddress("cjetmva1", &cjetmva1, &b_cjetmva1);
   fChain->SetBranchAddress("cjetmva1_puppi", &cjetmva1_puppi, &b_cjetmva1_puppi);
   fChain->SetBranchAddress("cjetmva2", &cjetmva2, &b_cjetmva2);
   fChain->SetBranchAddress("cjetmva2_puppi", &cjetmva2_puppi, &b_cjetmva2_puppi);
   fChain->SetBranchAddress("cjetphi1", &cjetphi1, &b_cjetphi1);
   fChain->SetBranchAddress("cjetphi1_puppi", &cjetphi1_puppi, &b_cjetphi1_puppi);
   fChain->SetBranchAddress("cjetphi2", &cjetphi2, &b_cjetphi2);
   fChain->SetBranchAddress("cjetphi2_puppi", &cjetphi2_puppi, &b_cjetphi2_puppi);
   fChain->SetBranchAddress("cjetpileupidFlag1", &cjetpileupidFlag1, &b_cjetpileupidFlag1);
   fChain->SetBranchAddress("cjetpileupidFlag1_puppi", &cjetpileupidFlag1_puppi, &b_cjetpileupidFlag1_puppi);
   fChain->SetBranchAddress("cjetpileupidFlag2", &cjetpileupidFlag2, &b_cjetpileupidFlag2);
   fChain->SetBranchAddress("cjetpileupidFlag2_puppi", &cjetpileupidFlag2_puppi, &b_cjetpileupidFlag2_puppi);
   fChain->SetBranchAddress("cjetpileupidValue1", &cjetpileupidValue1, &b_cjetpileupidValue1);
   fChain->SetBranchAddress("cjetpileupidValue1_puppi", &cjetpileupidValue1_puppi, &b_cjetpileupidValue1_puppi);
   fChain->SetBranchAddress("cjetpileupidValue2", &cjetpileupidValue2, &b_cjetpileupidValue2);
   fChain->SetBranchAddress("cjetpileupidValue2_puppi", &cjetpileupidValue2_puppi, &b_cjetpileupidValue2_puppi);
   fChain->SetBranchAddress("cjetpt1", &cjetpt1, &b_cjetpt1);
   fChain->SetBranchAddress("cjetpt1_puppi", &cjetpt1_puppi, &b_cjetpt1_puppi);
   fChain->SetBranchAddress("cjetpt2", &cjetpt2, &b_cjetpt2);
   fChain->SetBranchAddress("cjetpt2_puppi", &cjetpt2_puppi, &b_cjetpt2_puppi);
   fChain->SetBranchAddress("dataset", &dataset, &b_dataset);
   fChain->SetBranchAddress("detajj", &detajj, &b_detajj);
   fChain->SetBranchAddress("detajj_puppi", &detajj_puppi, &b_detajj_puppi);
   fChain->SetBranchAddress("dphilMet1_puppi", &dphilMet1_puppi, &b_dphilMet1_puppi);
   fChain->SetBranchAddress("dphilMet2_puppi", &dphilMet2_puppi, &b_dphilMet2_puppi);
   fChain->SetBranchAddress("dphilMet_puppi", &dphilMet_puppi, &b_dphilMet_puppi);
   fChain->SetBranchAddress("dphill", &dphill, &b_dphill);
   fChain->SetBranchAddress("dphillJet_puppi", &dphillJet_puppi, &b_dphillJet_puppi);
   fChain->SetBranchAddress("dphillMet_puppi", &dphillMet_puppi, &b_dphillMet_puppi);
   fChain->SetBranchAddress("dphilljet", &dphilljet, &b_dphilljet);
   fChain->SetBranchAddress("dphilljetjet", &dphilljetjet, &b_dphilljetjet);
   fChain->SetBranchAddress("dphilljetjet_puppi", &dphilljetjet_puppi, &b_dphilljetjet_puppi);
   fChain->SetBranchAddress("dphillmet", &dphillmet, &b_dphillmet);
   fChain->SetBranchAddress("dphilmet", &dphilmet, &b_dphilmet);
   fChain->SetBranchAddress("dphilmet1", &dphilmet1, &b_dphilmet1);
   fChain->SetBranchAddress("dphilmet2", &dphilmet2, &b_dphilmet2);
   fChain->SetBranchAddress("drll", &drll, &b_drll);
   fChain->SetBranchAddress("effAW", &effAW, &b_effAW);
   fChain->SetBranchAddress("effBW", &effBW, &b_effBW);
   fChain->SetBranchAddress("effW", &effW, &b_effW);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("eta3", &eta3, &b_eta3);
   fChain->SetBranchAddress("eta4", &eta4, &b_eta4);
   fChain->SetBranchAddress("fakeAW", &fakeAW, &b_fakeAW);
   fChain->SetBranchAddress("fakeBW", &fakeBW, &b_fakeBW);
   fChain->SetBranchAddress("fakeW", &fakeW, &b_fakeW);
   fChain->SetBranchAddress("fermiW", &fermiW, &b_fermiW);
   fChain->SetBranchAddress("fourW", &fourW, &b_fourW);
   fChain->SetBranchAddress("gammaMRStar", &gammaMRStar, &b_gammaMRStar);
   fChain->SetBranchAddress("hardbcsv", &hardbcsv, &b_hardbcsv);
   fChain->SetBranchAddress("hardbcsvMVA", &hardbcsvMVA, &b_hardbcsvMVA);
   fChain->SetBranchAddress("hardbjpb", &hardbjpb, &b_hardbjpb);
   fChain->SetBranchAddress("hardtche", &hardtche, &b_hardtche);
   fChain->SetBranchAddress("hardtchp", &hardtchp, &b_hardtchp);
   fChain->SetBranchAddress("higggsGenpt", &higggsGenpt, &b_higggsGenpt);
   fChain->SetBranchAddress("higgsLHEpt", &higgsLHEpt, &b_higgsLHEpt);
   fChain->SetBranchAddress("isHighPtMuon1", &isHighPtMuon1, &b_isHighPtMuon1);
   fChain->SetBranchAddress("isHighPtMuon2", &isHighPtMuon2, &b_isHighPtMuon2);
   fChain->SetBranchAddress("isHighPtMuon3", &isHighPtMuon3, &b_isHighPtMuon3);
   fChain->SetBranchAddress("isHighPtMuon4", &isHighPtMuon4, &b_isHighPtMuon4);
   fChain->SetBranchAddress("isLooseElectron1", &isLooseElectron1, &b_isLooseElectron1);
   fChain->SetBranchAddress("isLooseElectron2", &isLooseElectron2, &b_isLooseElectron2);
   fChain->SetBranchAddress("isLooseElectron3", &isLooseElectron3, &b_isLooseElectron3);
   fChain->SetBranchAddress("isLooseElectron4", &isLooseElectron4, &b_isLooseElectron4);
   fChain->SetBranchAddress("isLooseMuon1", &isLooseMuon1, &b_isLooseMuon1);
   fChain->SetBranchAddress("isLooseMuon2", &isLooseMuon2, &b_isLooseMuon2);
   fChain->SetBranchAddress("isLooseMuon3", &isLooseMuon3, &b_isLooseMuon3);
   fChain->SetBranchAddress("isLooseMuon4", &isLooseMuon4, &b_isLooseMuon4);
   fChain->SetBranchAddress("isLooseRobustElectron1", &isLooseRobustElectron1, &b_isLooseRobustElectron1);
   fChain->SetBranchAddress("isLooseRobustElectron2", &isLooseRobustElectron2, &b_isLooseRobustElectron2);
   fChain->SetBranchAddress("isLooseRobustElectron3", &isLooseRobustElectron3, &b_isLooseRobustElectron3);
   fChain->SetBranchAddress("isLooseRobustElectron4", &isLooseRobustElectron4, &b_isLooseRobustElectron4);
   fChain->SetBranchAddress("isRobustHighEnergyElectron1", &isRobustHighEnergyElectron1, &b_isRobustHighEnergyElectron1);
   fChain->SetBranchAddress("isRobustHighEnergyElectron2", &isRobustHighEnergyElectron2, &b_isRobustHighEnergyElectron2);
   fChain->SetBranchAddress("isRobustHighEnergyElectron3", &isRobustHighEnergyElectron3, &b_isRobustHighEnergyElectron3);
   fChain->SetBranchAddress("isRobustHighEnergyElectron4", &isRobustHighEnergyElectron4, &b_isRobustHighEnergyElectron4);
   fChain->SetBranchAddress("isSTA1", &isSTA1, &b_isSTA1);
   fChain->SetBranchAddress("isSTA2", &isSTA2, &b_isSTA2);
   fChain->SetBranchAddress("isSTA3", &isSTA3, &b_isSTA3);
   fChain->SetBranchAddress("isSTA4", &isSTA4, &b_isSTA4);
   fChain->SetBranchAddress("isSoftMuon1", &isSoftMuon1, &b_isSoftMuon1);
   fChain->SetBranchAddress("isSoftMuon2", &isSoftMuon2, &b_isSoftMuon2);
   fChain->SetBranchAddress("isSoftMuon3", &isSoftMuon3, &b_isSoftMuon3);
   fChain->SetBranchAddress("isSoftMuon4", &isSoftMuon4, &b_isSoftMuon4);
   fChain->SetBranchAddress("isTightElectron1", &isTightElectron1, &b_isTightElectron1);
   fChain->SetBranchAddress("isTightElectron2", &isTightElectron2, &b_isTightElectron2);
   fChain->SetBranchAddress("isTightElectron3", &isTightElectron3, &b_isTightElectron3);
   fChain->SetBranchAddress("isTightElectron4", &isTightElectron4, &b_isTightElectron4);
   fChain->SetBranchAddress("isTightMuon1", &isTightMuon1, &b_isTightMuon1);
   fChain->SetBranchAddress("isTightMuon2", &isTightMuon2, &b_isTightMuon2);
   fChain->SetBranchAddress("isTightMuon3", &isTightMuon3, &b_isTightMuon3);
   fChain->SetBranchAddress("isTightMuon4", &isTightMuon4, &b_isTightMuon4);
   fChain->SetBranchAddress("isTightRobustElectron1", &isTightRobustElectron1, &b_isTightRobustElectron1);
   fChain->SetBranchAddress("isTightRobustElectron2", &isTightRobustElectron2, &b_isTightRobustElectron2);
   fChain->SetBranchAddress("isTightRobustElectron3", &isTightRobustElectron3, &b_isTightRobustElectron3);
   fChain->SetBranchAddress("isTightRobustElectron4", &isTightRobustElectron4, &b_isTightRobustElectron4);
   fChain->SetBranchAddress("isomva1", &isomva1, &b_isomva1);
   fChain->SetBranchAddress("isomva2", &isomva2, &b_isomva2);
   fChain->SetBranchAddress("isomva3", &isomva3, &b_isomva3);
   fChain->SetBranchAddress("isomva4", &isomva4, &b_isomva4);
   fChain->SetBranchAddress("jetGenPartoneta1", &jetGenPartoneta1, &b_jetGenPartoneta1);
   fChain->SetBranchAddress("jetGenPartoneta2", &jetGenPartoneta2, &b_jetGenPartoneta2);
   fChain->SetBranchAddress("jetGenPartoneta3", &jetGenPartoneta3, &b_jetGenPartoneta3);
   fChain->SetBranchAddress("jetGenPartonphi1", &jetGenPartonphi1, &b_jetGenPartonphi1);
   fChain->SetBranchAddress("jetGenPartonphi2", &jetGenPartonphi2, &b_jetGenPartonphi2);
   fChain->SetBranchAddress("jetGenPartonphi3", &jetGenPartonphi3, &b_jetGenPartonphi3);
   fChain->SetBranchAddress("jetGenPartonpid1", &jetGenPartonpid1, &b_jetGenPartonpid1);
   fChain->SetBranchAddress("jetGenPartonpid2", &jetGenPartonpid2, &b_jetGenPartonpid2);
   fChain->SetBranchAddress("jetGenPartonpid3", &jetGenPartonpid3, &b_jetGenPartonpid3);
   fChain->SetBranchAddress("jetGenPartonpt1", &jetGenPartonpt1, &b_jetGenPartonpt1);
   fChain->SetBranchAddress("jetGenPartonpt2", &jetGenPartonpt2, &b_jetGenPartonpt2);
   fChain->SetBranchAddress("jetGenPartonpt3", &jetGenPartonpt3, &b_jetGenPartonpt3);
   fChain->SetBranchAddress("jetGeneta1", &jetGeneta1, &b_jetGeneta1);
   fChain->SetBranchAddress("jetGeneta2", &jetGeneta2, &b_jetGeneta2);
   fChain->SetBranchAddress("jetGeneta3", &jetGeneta3, &b_jetGeneta3);
   fChain->SetBranchAddress("jetGeneta4", &jetGeneta4, &b_jetGeneta4);
   fChain->SetBranchAddress("jetGeneta5", &jetGeneta5, &b_jetGeneta5);
   fChain->SetBranchAddress("jetGenphi1", &jetGenphi1, &b_jetGenphi1);
   fChain->SetBranchAddress("jetGenphi2", &jetGenphi2, &b_jetGenphi2);
   fChain->SetBranchAddress("jetGenphi3", &jetGenphi3, &b_jetGenphi3);
   fChain->SetBranchAddress("jetGenphi4", &jetGenphi4, &b_jetGenphi4);
   fChain->SetBranchAddress("jetGenphi5", &jetGenphi5, &b_jetGenphi5);
   fChain->SetBranchAddress("jetGenpt1", &jetGenpt1, &b_jetGenpt1);
   fChain->SetBranchAddress("jetGenpt2", &jetGenpt2, &b_jetGenpt2);
   fChain->SetBranchAddress("jetGenpt3", &jetGenpt3, &b_jetGenpt3);
   fChain->SetBranchAddress("jetGenpt4", &jetGenpt4, &b_jetGenpt4);
   fChain->SetBranchAddress("jetGenpt5", &jetGenpt5, &b_jetGenpt5);
   fChain->SetBranchAddress("jetLHEPartoneta1", &jetLHEPartoneta1, &b_jetLHEPartoneta1);
   fChain->SetBranchAddress("jetLHEPartoneta2", &jetLHEPartoneta2, &b_jetLHEPartoneta2);
   fChain->SetBranchAddress("jetLHEPartoneta3", &jetLHEPartoneta3, &b_jetLHEPartoneta3);
   fChain->SetBranchAddress("jetLHEPartonphi1", &jetLHEPartonphi1, &b_jetLHEPartonphi1);
   fChain->SetBranchAddress("jetLHEPartonphi2", &jetLHEPartonphi2, &b_jetLHEPartonphi2);
   fChain->SetBranchAddress("jetLHEPartonphi3", &jetLHEPartonphi3, &b_jetLHEPartonphi3);
   fChain->SetBranchAddress("jetLHEPartonpid1", &jetLHEPartonpid1, &b_jetLHEPartonpid1);
   fChain->SetBranchAddress("jetLHEPartonpid2", &jetLHEPartonpid2, &b_jetLHEPartonpid2);
   fChain->SetBranchAddress("jetLHEPartonpid3", &jetLHEPartonpid3, &b_jetLHEPartonpid3);
   fChain->SetBranchAddress("jetLHEPartonpt1", &jetLHEPartonpt1, &b_jetLHEPartonpt1);
   fChain->SetBranchAddress("jetLHEPartonpt2", &jetLHEPartonpt2, &b_jetLHEPartonpt2);
   fChain->SetBranchAddress("jetLHEPartonpt3", &jetLHEPartonpt3, &b_jetLHEPartonpt3);
   fChain->SetBranchAddress("jetRho", &jetRho, &b_jetRho);
   fChain->SetBranchAddress("jetbjpb1", &jetbjpb1, &b_jetbjpb1);
   fChain->SetBranchAddress("jetbjpb2", &jetbjpb2, &b_jetbjpb2);
   fChain->SetBranchAddress("jetbjpb3", &jetbjpb3, &b_jetbjpb3);
   fChain->SetBranchAddress("jetbjpb4", &jetbjpb4, &b_jetbjpb4);
   fChain->SetBranchAddress("jetbjpb5", &jetbjpb5, &b_jetbjpb5);
   fChain->SetBranchAddress("jetbjpb6", &jetbjpb6, &b_jetbjpb6);
   fChain->SetBranchAddress("jetbjpb7", &jetbjpb7, &b_jetbjpb7);
   fChain->SetBranchAddress("jetbjpb8", &jetbjpb8, &b_jetbjpb8);
   fChain->SetBranchAddress("jetbjpb_puppi1", &jetbjpb_puppi1, &b_jetbjpb_puppi1);
   fChain->SetBranchAddress("jetbjpb_puppi2", &jetbjpb_puppi2, &b_jetbjpb_puppi2);
   fChain->SetBranchAddress("jetbjpb_puppi3", &jetbjpb_puppi3, &b_jetbjpb_puppi3);
   fChain->SetBranchAddress("jetbjpb_puppi4", &jetbjpb_puppi4, &b_jetbjpb_puppi4);
   fChain->SetBranchAddress("jetbjpb_puppi5", &jetbjpb_puppi5, &b_jetbjpb_puppi5);
   fChain->SetBranchAddress("jetbjpb_puppi6", &jetbjpb_puppi6, &b_jetbjpb_puppi6);
   fChain->SetBranchAddress("jetbjpb_puppi7", &jetbjpb_puppi7, &b_jetbjpb_puppi7);
   fChain->SetBranchAddress("jetbjpb_puppi8", &jetbjpb_puppi8, &b_jetbjpb_puppi8);
   fChain->SetBranchAddress("jetcsv1", &jetcsv1, &b_jetcsv1);
   fChain->SetBranchAddress("jetcsv2", &jetcsv2, &b_jetcsv2);
   fChain->SetBranchAddress("jetcsv3", &jetcsv3, &b_jetcsv3);
   fChain->SetBranchAddress("jetcsv4", &jetcsv4, &b_jetcsv4);
   fChain->SetBranchAddress("jetcsv5", &jetcsv5, &b_jetcsv5);
   fChain->SetBranchAddress("jetcsv6", &jetcsv6, &b_jetcsv6);
   fChain->SetBranchAddress("jetcsv7", &jetcsv7, &b_jetcsv7);
   fChain->SetBranchAddress("jetcsv8", &jetcsv8, &b_jetcsv8);
   fChain->SetBranchAddress("jetcsvMVA1", &jetcsvMVA1, &b_jetcsvMVA1);
   fChain->SetBranchAddress("jetcsvMVA2", &jetcsvMVA2, &b_jetcsvMVA2);
   fChain->SetBranchAddress("jetcsvMVA3", &jetcsvMVA3, &b_jetcsvMVA3);
   fChain->SetBranchAddress("jetcsvMVA4", &jetcsvMVA4, &b_jetcsvMVA4);
   fChain->SetBranchAddress("jetcsvMVA5", &jetcsvMVA5, &b_jetcsvMVA5);
   fChain->SetBranchAddress("jetcsvMVA6", &jetcsvMVA6, &b_jetcsvMVA6);
   fChain->SetBranchAddress("jetcsvMVA7", &jetcsvMVA7, &b_jetcsvMVA7);
   fChain->SetBranchAddress("jetcsvMVA8", &jetcsvMVA8, &b_jetcsvMVA8);
   fChain->SetBranchAddress("jetcsvMVA_puppi1", &jetcsvMVA_puppi1, &b_jetcsvMVA_puppi1);
   fChain->SetBranchAddress("jetcsvMVA_puppi2", &jetcsvMVA_puppi2, &b_jetcsvMVA_puppi2);
   fChain->SetBranchAddress("jetcsvMVA_puppi3", &jetcsvMVA_puppi3, &b_jetcsvMVA_puppi3);
   fChain->SetBranchAddress("jetcsvMVA_puppi4", &jetcsvMVA_puppi4, &b_jetcsvMVA_puppi4);
   fChain->SetBranchAddress("jetcsvMVA_puppi5", &jetcsvMVA_puppi5, &b_jetcsvMVA_puppi5);
   fChain->SetBranchAddress("jetcsvMVA_puppi6", &jetcsvMVA_puppi6, &b_jetcsvMVA_puppi6);
   fChain->SetBranchAddress("jetcsvMVA_puppi7", &jetcsvMVA_puppi7, &b_jetcsvMVA_puppi7);
   fChain->SetBranchAddress("jetcsvMVA_puppi8", &jetcsvMVA_puppi8, &b_jetcsvMVA_puppi8);
   fChain->SetBranchAddress("jetcsv_puppi1", &jetcsv_puppi1, &b_jetcsv_puppi1);
   fChain->SetBranchAddress("jetcsv_puppi2", &jetcsv_puppi2, &b_jetcsv_puppi2);
   fChain->SetBranchAddress("jetcsv_puppi3", &jetcsv_puppi3, &b_jetcsv_puppi3);
   fChain->SetBranchAddress("jetcsv_puppi4", &jetcsv_puppi4, &b_jetcsv_puppi4);
   fChain->SetBranchAddress("jetcsv_puppi5", &jetcsv_puppi5, &b_jetcsv_puppi5);
   fChain->SetBranchAddress("jetcsv_puppi6", &jetcsv_puppi6, &b_jetcsv_puppi6);
   fChain->SetBranchAddress("jetcsv_puppi7", &jetcsv_puppi7, &b_jetcsv_puppi7);
   fChain->SetBranchAddress("jetcsv_puppi8", &jetcsv_puppi8, &b_jetcsv_puppi8);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("jeteta3", &jeteta3, &b_jeteta3);
   fChain->SetBranchAddress("jeteta4", &jeteta4, &b_jeteta4);
   fChain->SetBranchAddress("jeteta5", &jeteta5, &b_jeteta5);
   fChain->SetBranchAddress("jeteta6", &jeteta6, &b_jeteta6);
   fChain->SetBranchAddress("jeteta7", &jeteta7, &b_jeteta7);
   fChain->SetBranchAddress("jeteta8", &jeteta8, &b_jeteta8);
   fChain->SetBranchAddress("jeteta_puppi1", &jeteta_puppi1, &b_jeteta_puppi1);
   fChain->SetBranchAddress("jeteta_puppi2", &jeteta_puppi2, &b_jeteta_puppi2);
   fChain->SetBranchAddress("jeteta_puppi3", &jeteta_puppi3, &b_jeteta_puppi3);
   fChain->SetBranchAddress("jeteta_puppi4", &jeteta_puppi4, &b_jeteta_puppi4);
   fChain->SetBranchAddress("jeteta_puppi5", &jeteta_puppi5, &b_jeteta_puppi5);
   fChain->SetBranchAddress("jeteta_puppi6", &jeteta_puppi6, &b_jeteta_puppi6);
   fChain->SetBranchAddress("jeteta_puppi7", &jeteta_puppi7, &b_jeteta_puppi7);
   fChain->SetBranchAddress("jeteta_puppi8", &jeteta_puppi8, &b_jeteta_puppi8);
   fChain->SetBranchAddress("jetid1", &jetid1, &b_jetid1);
   fChain->SetBranchAddress("jetid2", &jetid2, &b_jetid2);
   fChain->SetBranchAddress("jetid3", &jetid3, &b_jetid3);
   fChain->SetBranchAddress("jetid4", &jetid4, &b_jetid4);
   fChain->SetBranchAddress("jetid5", &jetid5, &b_jetid5);
   fChain->SetBranchAddress("jetid6", &jetid6, &b_jetid6);
   fChain->SetBranchAddress("jetid7", &jetid7, &b_jetid7);
   fChain->SetBranchAddress("jetid8", &jetid8, &b_jetid8);
   fChain->SetBranchAddress("jetid_puppi1", &jetid_puppi1, &b_jetid_puppi1);
   fChain->SetBranchAddress("jetid_puppi2", &jetid_puppi2, &b_jetid_puppi2);
   fChain->SetBranchAddress("jetid_puppi4", &jetid_puppi4, &b_jetid_puppi4);
   fChain->SetBranchAddress("jetid_puppi5", &jetid_puppi5, &b_jetid_puppi5);
   fChain->SetBranchAddress("jetid_puppi6", &jetid_puppi6, &b_jetid_puppi6);
   fChain->SetBranchAddress("jetid_puppi7", &jetid_puppi7, &b_jetid_puppi7);
   fChain->SetBranchAddress("jetid_puppi8", &jetid_puppi8, &b_jetid_puppi8);
   fChain->SetBranchAddress("jetmass1", &jetmass1, &b_jetmass1);
   fChain->SetBranchAddress("jetmass2", &jetmass2, &b_jetmass2);
   fChain->SetBranchAddress("jetmass3", &jetmass3, &b_jetmass3);
   fChain->SetBranchAddress("jetmass4", &jetmass4, &b_jetmass4);
   fChain->SetBranchAddress("jetmass5", &jetmass5, &b_jetmass5);
   fChain->SetBranchAddress("jetmass6", &jetmass6, &b_jetmass6);
   fChain->SetBranchAddress("jetmass7", &jetmass7, &b_jetmass7);
   fChain->SetBranchAddress("jetmass8", &jetmass8, &b_jetmass8);
   fChain->SetBranchAddress("jetmass_puppi1", &jetmass_puppi1, &b_jetmass_puppi1);
   fChain->SetBranchAddress("jetmass_puppi2", &jetmass_puppi2, &b_jetmass_puppi2);
   fChain->SetBranchAddress("jetmass_puppi3", &jetmass_puppi3, &b_jetmass_puppi3);
   fChain->SetBranchAddress("jetmass_puppi4", &jetmass_puppi4, &b_jetmass_puppi4);
   fChain->SetBranchAddress("jetmass_puppi5", &jetmass_puppi5, &b_jetmass_puppi5);
   fChain->SetBranchAddress("jetmass_puppi6", &jetmass_puppi6, &b_jetmass_puppi6);
   fChain->SetBranchAddress("jetmass_puppi7", &jetmass_puppi7, &b_jetmass_puppi7);
   fChain->SetBranchAddress("jetmass_puppi8", &jetmass_puppi8, &b_jetmass_puppi8);
   fChain->SetBranchAddress("jetmva1", &jetmva1, &b_jetmva1);
   fChain->SetBranchAddress("jetmva2", &jetmva2, &b_jetmva2);
   fChain->SetBranchAddress("jetmva3", &jetmva3, &b_jetmva3);
   fChain->SetBranchAddress("jetmva4", &jetmva4, &b_jetmva4);
   fChain->SetBranchAddress("jetmva5", &jetmva5, &b_jetmva5);
   fChain->SetBranchAddress("jetmva6", &jetmva6, &b_jetmva6);
   fChain->SetBranchAddress("jetmva7", &jetmva7, &b_jetmva7);
   fChain->SetBranchAddress("jetmva8", &jetmva8, &b_jetmva8);
   fChain->SetBranchAddress("jetmva_puppi1", &jetmva_puppi1, &b_jetmva_puppi1);
   fChain->SetBranchAddress("jetmva_puppi2", &jetmva_puppi2, &b_jetmva_puppi2);
   fChain->SetBranchAddress("jetmva_puppi3", &jetmva_puppi3, &b_jetmva_puppi3);
   fChain->SetBranchAddress("jetmva_puppi4", &jetmva_puppi4, &b_jetmva_puppi4);
   fChain->SetBranchAddress("jetmva_puppi5", &jetmva_puppi5, &b_jetmva_puppi5);
   fChain->SetBranchAddress("jetmva_puppi6", &jetmva_puppi6, &b_jetmva_puppi6);
   fChain->SetBranchAddress("jetmva_puppi7", &jetmva_puppi7, &b_jetmva_puppi7);
   fChain->SetBranchAddress("jetmva_puppi8", &jetmva_puppi8, &b_jetmva_puppi8);
   fChain->SetBranchAddress("jetphi1", &jetphi1, &b_jetphi1);
   fChain->SetBranchAddress("jetphi2", &jetphi2, &b_jetphi2);
   fChain->SetBranchAddress("jetphi3", &jetphi3, &b_jetphi3);
   fChain->SetBranchAddress("jetphi4", &jetphi4, &b_jetphi4);
   fChain->SetBranchAddress("jetphi5", &jetphi5, &b_jetphi5);
   fChain->SetBranchAddress("jetphi6", &jetphi6, &b_jetphi6);
   fChain->SetBranchAddress("jetphi7", &jetphi7, &b_jetphi7);
   fChain->SetBranchAddress("jetphi8", &jetphi8, &b_jetphi8);
   fChain->SetBranchAddress("jetphi_puppi1", &jetphi_puppi1, &b_jetphi_puppi1);
   fChain->SetBranchAddress("jetphi_puppi2", &jetphi_puppi2, &b_jetphi_puppi2);
   fChain->SetBranchAddress("jetphi_puppi3", &jetphi_puppi3, &b_jetphi_puppi3);
   fChain->SetBranchAddress("jetphi_puppi4", &jetphi_puppi4, &b_jetphi_puppi4);
   fChain->SetBranchAddress("jetphi_puppi5", &jetphi_puppi5, &b_jetphi_puppi5);
   fChain->SetBranchAddress("jetphi_puppi6", &jetphi_puppi6, &b_jetphi_puppi6);
   fChain->SetBranchAddress("jetphi_puppi7", &jetphi_puppi7, &b_jetphi_puppi7);
   fChain->SetBranchAddress("jetphi_puppi8", &jetphi_puppi8, &b_jetphi_puppi8);
   fChain->SetBranchAddress("jetpileupidFlag1", &jetpileupidFlag1, &b_jetpileupidFlag1);
   fChain->SetBranchAddress("jetpileupidFlag2", &jetpileupidFlag2, &b_jetpileupidFlag2);
   fChain->SetBranchAddress("jetpileupidFlag3", &jetpileupidFlag3, &b_jetpileupidFlag3);
   fChain->SetBranchAddress("jetpileupidFlag4", &jetpileupidFlag4, &b_jetpileupidFlag4);
   fChain->SetBranchAddress("jetpileupidFlag5", &jetpileupidFlag5, &b_jetpileupidFlag5);
   fChain->SetBranchAddress("jetpileupidFlag6", &jetpileupidFlag6, &b_jetpileupidFlag6);
   fChain->SetBranchAddress("jetpileupidFlag7", &jetpileupidFlag7, &b_jetpileupidFlag7);
   fChain->SetBranchAddress("jetpileupidFlag8", &jetpileupidFlag8, &b_jetpileupidFlag8);
   fChain->SetBranchAddress("jetpileupidFlag_puppi1", &jetpileupidFlag_puppi1, &b_jetpileupidFlag_puppi1);
   fChain->SetBranchAddress("jetpileupidFlag_puppi2", &jetpileupidFlag_puppi2, &b_jetpileupidFlag_puppi2);
   fChain->SetBranchAddress("jetpileupidFlag_puppi3", &jetpileupidFlag_puppi3, &b_jetpileupidFlag_puppi3);
   fChain->SetBranchAddress("jetpileupidFlag_puppi4", &jetpileupidFlag_puppi4, &b_jetpileupidFlag_puppi4);
   fChain->SetBranchAddress("jetpileupidFlag_puppi5", &jetpileupidFlag_puppi5, &b_jetpileupidFlag_puppi5);
   fChain->SetBranchAddress("jetpileupidFlag_puppi6", &jetpileupidFlag_puppi6, &b_jetpileupidFlag_puppi6);
   fChain->SetBranchAddress("jetpileupidFlag_puppi7", &jetpileupidFlag_puppi7, &b_jetpileupidFlag_puppi7);
   fChain->SetBranchAddress("jetpileupidFlag_puppi8", &jetpileupidFlag_puppi8, &b_jetpileupidFlag_puppi8);
   fChain->SetBranchAddress("jetpileupidValue1", &jetpileupidValue1, &b_jetpileupidValue1);
   fChain->SetBranchAddress("jetpileupidValue2", &jetpileupidValue2, &b_jetpileupidValue2);
   fChain->SetBranchAddress("jetpileupidValue3", &jetpileupidValue3, &b_jetpileupidValue3);
   fChain->SetBranchAddress("jetpileupidValue4", &jetpileupidValue4, &b_jetpileupidValue4);
   fChain->SetBranchAddress("jetpileupidValue5", &jetpileupidValue5, &b_jetpileupidValue5);
   fChain->SetBranchAddress("jetpileupidValue6", &jetpileupidValue6, &b_jetpileupidValue6);
   fChain->SetBranchAddress("jetpileupidValue7", &jetpileupidValue7, &b_jetpileupidValue7);
   fChain->SetBranchAddress("jetpileupidValue8", &jetpileupidValue8, &b_jetpileupidValue8);
   fChain->SetBranchAddress("jetpileupidValue_puppi1", &jetpileupidValue_puppi1, &b_jetpileupidValue_puppi1);
   fChain->SetBranchAddress("jetpileupidValue_puppi2", &jetpileupidValue_puppi2, &b_jetpileupidValue_puppi2);
   fChain->SetBranchAddress("jetpileupidValue_puppi3", &jetpileupidValue_puppi3, &b_jetpileupidValue_puppi3);
   fChain->SetBranchAddress("jetpileupidValue_puppi4", &jetpileupidValue_puppi4, &b_jetpileupidValue_puppi4);
   fChain->SetBranchAddress("jetpileupidValue_puppi5", &jetpileupidValue_puppi5, &b_jetpileupidValue_puppi5);
   fChain->SetBranchAddress("jetpileupidValue_puppi6", &jetpileupidValue_puppi6, &b_jetpileupidValue_puppi6);
   fChain->SetBranchAddress("jetpileupidValue_puppi7", &jetpileupidValue_puppi7, &b_jetpileupidValue_puppi7);
   fChain->SetBranchAddress("jetpileupidValue_puppi8", &jetpileupidValue_puppi8, &b_jetpileupidValue_puppi8);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jetpt3", &jetpt3, &b_jetpt3);
   fChain->SetBranchAddress("jetpt4", &jetpt4, &b_jetpt4);
   fChain->SetBranchAddress("jetpt5", &jetpt5, &b_jetpt5);
   fChain->SetBranchAddress("jetpt6", &jetpt6, &b_jetpt6);
   fChain->SetBranchAddress("jetpt7", &jetpt7, &b_jetpt7);
   fChain->SetBranchAddress("jetpt8", &jetpt8, &b_jetpt8);
   fChain->SetBranchAddress("jetpt_puppi1", &jetpt_puppi1, &b_jetpt_puppi1);
   fChain->SetBranchAddress("jetpt_puppi2", &jetpt_puppi2, &b_jetpt_puppi2);
   fChain->SetBranchAddress("jetpt_puppi3", &jetpt_puppi3, &b_jetpt_puppi3);
   fChain->SetBranchAddress("jetpt_puppi4", &jetpt_puppi4, &b_jetpt_puppi4);
   fChain->SetBranchAddress("jetpt_puppi5", &jetpt_puppi5, &b_jetpt_puppi5);
   fChain->SetBranchAddress("jetpt_puppi6", &jetpt_puppi6, &b_jetpt_puppi6);
   fChain->SetBranchAddress("jetpt_puppi7", &jetpt_puppi7, &b_jetpt_puppi7);
   fChain->SetBranchAddress("jetpt_puppi8", &jetpt_puppi8, &b_jetpt_puppi8);
   fChain->SetBranchAddress("jettche1", &jettche1, &b_jettche1);
   fChain->SetBranchAddress("jettche2", &jettche2, &b_jettche2);
   fChain->SetBranchAddress("jettche3", &jettche3, &b_jettche3);
   fChain->SetBranchAddress("jettche4", &jettche4, &b_jettche4);
   fChain->SetBranchAddress("jettche5", &jettche5, &b_jettche5);
   fChain->SetBranchAddress("jettche6", &jettche6, &b_jettche6);
   fChain->SetBranchAddress("jettche7", &jettche7, &b_jettche7);
   fChain->SetBranchAddress("jettche8", &jettche8, &b_jettche8);
   fChain->SetBranchAddress("jettche_puppi1", &jettche_puppi1, &b_jettche_puppi1);
   fChain->SetBranchAddress("jettche_puppi2", &jettche_puppi2, &b_jettche_puppi2);
   fChain->SetBranchAddress("jettche_puppi3", &jettche_puppi3, &b_jettche_puppi3);
   fChain->SetBranchAddress("jettche_puppi4", &jettche_puppi4, &b_jettche_puppi4);
   fChain->SetBranchAddress("jettche_puppi5", &jettche_puppi5, &b_jettche_puppi5);
   fChain->SetBranchAddress("jettche_puppi6", &jettche_puppi6, &b_jettche_puppi6);
   fChain->SetBranchAddress("jettche_puppi7", &jettche_puppi7, &b_jettche_puppi7);
   fChain->SetBranchAddress("jettche_puppi8", &jettche_puppi8, &b_jettche_puppi8);
   fChain->SetBranchAddress("jettchp1", &jettchp1, &b_jettchp1);
   fChain->SetBranchAddress("jettchp2", &jettchp2, &b_jettchp2);
   fChain->SetBranchAddress("jettchp3", &jettchp3, &b_jettchp3);
   fChain->SetBranchAddress("jettchp4", &jettchp4, &b_jettchp4);
   fChain->SetBranchAddress("jettchp5", &jettchp5, &b_jettchp5);
   fChain->SetBranchAddress("jettchp6", &jettchp6, &b_jettchp6);
   fChain->SetBranchAddress("jettchp7", &jettchp7, &b_jettchp7);
   fChain->SetBranchAddress("jettchp8", &jettchp8, &b_jettchp8);
   fChain->SetBranchAddress("jettchp_puppi1", &jettchp_puppi1, &b_jettchp_puppi1);
   fChain->SetBranchAddress("jettchp_puppi2", &jettchp_puppi2, &b_jettchp_puppi2);
   fChain->SetBranchAddress("jettchp_puppi3", &jettchp_puppi3, &b_jettchp_puppi3);
   fChain->SetBranchAddress("jettchp_puppi4", &jettchp_puppi4, &b_jettchp_puppi4);
   fChain->SetBranchAddress("jettchp_puppi5", &jettchp_puppi5, &b_jettchp_puppi5);
   fChain->SetBranchAddress("jettchp_puppi6", &jettchp_puppi6, &b_jettchp_puppi6);
   fChain->SetBranchAddress("jettchp_puppi7", &jettchp_puppi7, &b_jettchp_puppi7);
   fChain->SetBranchAddress("jettchp_puppi8", &jettchp_puppi8, &b_jettchp_puppi8);
   fChain->SetBranchAddress("kfW", &kfW, &b_kfW);
   fChain->SetBranchAddress("leptonGeneta1", &leptonGeneta1, &b_leptonGeneta1);
   fChain->SetBranchAddress("leptonGeneta2", &leptonGeneta2, &b_leptonGeneta2);
   fChain->SetBranchAddress("leptonGeneta3", &leptonGeneta3, &b_leptonGeneta3);
   fChain->SetBranchAddress("leptonGenphi1", &leptonGenphi1, &b_leptonGenphi1);
   fChain->SetBranchAddress("leptonGenphi2", &leptonGenphi2, &b_leptonGenphi2);
   fChain->SetBranchAddress("leptonGenphi3", &leptonGenphi3, &b_leptonGenphi3);
   fChain->SetBranchAddress("leptonGenpid1", &leptonGenpid1, &b_leptonGenpid1);
   fChain->SetBranchAddress("leptonGenpid2", &leptonGenpid2, &b_leptonGenpid2);
   fChain->SetBranchAddress("leptonGenpid3", &leptonGenpid3, &b_leptonGenpid3);
   fChain->SetBranchAddress("leptonGenpt1", &leptonGenpt1, &b_leptonGenpt1);
   fChain->SetBranchAddress("leptonGenpt2", &leptonGenpt2, &b_leptonGenpt2);
   fChain->SetBranchAddress("leptonGenpt3", &leptonGenpt3, &b_leptonGenpt3);
   fChain->SetBranchAddress("leptonLHEeta1", &leptonLHEeta1, &b_leptonLHEeta1);
   fChain->SetBranchAddress("leptonLHEeta2", &leptonLHEeta2, &b_leptonLHEeta2);
   fChain->SetBranchAddress("leptonLHEeta3", &leptonLHEeta3, &b_leptonLHEeta3);
   fChain->SetBranchAddress("leptonLHEphi1", &leptonLHEphi1, &b_leptonLHEphi1);
   fChain->SetBranchAddress("leptonLHEphi2", &leptonLHEphi2, &b_leptonLHEphi2);
   fChain->SetBranchAddress("leptonLHEphi3", &leptonLHEphi3, &b_leptonLHEphi3);
   fChain->SetBranchAddress("leptonLHEpid1", &leptonLHEpid1, &b_leptonLHEpid1);
   fChain->SetBranchAddress("leptonLHEpid2", &leptonLHEpid2, &b_leptonLHEpid2);
   fChain->SetBranchAddress("leptonLHEpid3", &leptonLHEpid3, &b_leptonLHEpid3);
   fChain->SetBranchAddress("leptonLHEpt1", &leptonLHEpt1, &b_leptonLHEpt1);
   fChain->SetBranchAddress("leptonLHEpt2", &leptonLHEpt2, &b_leptonLHEpt2);
   fChain->SetBranchAddress("leptonLHEpt3", &leptonLHEpt3, &b_leptonLHEpt3);
   fChain->SetBranchAddress("mctruth", &mctruth, &b_mctruth);
   fChain->SetBranchAddress("metGeneta", &metGeneta, &b_metGeneta);
   fChain->SetBranchAddress("metGenphi", &metGenphi, &b_metGenphi);
   fChain->SetBranchAddress("metGenpt", &metGenpt, &b_metGenpt);
   fChain->SetBranchAddress("metLHEeta", &metLHEeta, &b_metLHEeta);
   fChain->SetBranchAddress("metLHEphi", &metLHEphi, &b_metLHEphi);
   fChain->SetBranchAddress("metLHEpt", &metLHEpt, &b_metLHEpt);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   fChain->SetBranchAddress("mjj_puppi", &mjj_puppi, &b_mjj_puppi);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("mth", &mth, &b_mth);
   fChain->SetBranchAddress("mth_puppi", &mth_puppi, &b_mth_puppi);
   fChain->SetBranchAddress("mtw1", &mtw1, &b_mtw1);
   fChain->SetBranchAddress("mtw1_puppi", &mtw1_puppi, &b_mtw1_puppi);
   fChain->SetBranchAddress("mtw2", &mtw2, &b_mtw2);
   fChain->SetBranchAddress("mtw2_puppi", &mtw2_puppi, &b_mtw2_puppi);
   fChain->SetBranchAddress("nbjet", &nbjet, &b_nbjet);
   fChain->SetBranchAddress("nbjet_puppi", &nbjet_puppi, &b_nbjet_puppi);
   fChain->SetBranchAddress("nbjetcsvl", &nbjetcsvl, &b_nbjetcsvl);
   fChain->SetBranchAddress("nbjetcsvlMVA", &nbjetcsvlMVA, &b_nbjetcsvlMVA);
   fChain->SetBranchAddress("nbjetcsvlMVA_puppi", &nbjetcsvlMVA_puppi, &b_nbjetcsvlMVA_puppi);
   fChain->SetBranchAddress("nbjetcsvl_puppi", &nbjetcsvl_puppi, &b_nbjetcsvl_puppi);
   fChain->SetBranchAddress("nbjetcsvm", &nbjetcsvm, &b_nbjetcsvm);
   fChain->SetBranchAddress("nbjetcsvmMVA", &nbjetcsvmMVA, &b_nbjetcsvmMVA);
   fChain->SetBranchAddress("nbjetcsvmMVA_puppi", &nbjetcsvmMVA_puppi, &b_nbjetcsvmMVA_puppi);
   fChain->SetBranchAddress("nbjetcsvm_puppi", &nbjetcsvm_puppi, &b_nbjetcsvm_puppi);
   fChain->SetBranchAddress("nbjetcsvt", &nbjetcsvt, &b_nbjetcsvt);
   fChain->SetBranchAddress("nbjetcsvtMVA", &nbjetcsvtMVA, &b_nbjetcsvtMVA);
   fChain->SetBranchAddress("nbjetcsvtMVA_puppi", &nbjetcsvtMVA_puppi, &b_nbjetcsvtMVA_puppi);
   fChain->SetBranchAddress("nbjetcsvt_puppi", &nbjetcsvt_puppi, &b_nbjetcsvt_puppi);
   fChain->SetBranchAddress("nbjettche", &nbjettche, &b_nbjettche);
   fChain->SetBranchAddress("nbjettche_puppi", &nbjettche_puppi, &b_nbjettche_puppi);
   fChain->SetBranchAddress("nbjettchp", &nbjettchp, &b_nbjettchp);
   fChain->SetBranchAddress("nbjettchp_puppi", &nbjettchp_puppi, &b_nbjettchp_puppi);
   fChain->SetBranchAddress("nbrem1", &nbrem1, &b_nbrem1);
   fChain->SetBranchAddress("nbrem2", &nbrem2, &b_nbrem2);
   fChain->SetBranchAddress("nbrem3", &nbrem3, &b_nbrem3);
   fChain->SetBranchAddress("nbrem4", &nbrem4, &b_nbrem4);
   fChain->SetBranchAddress("neutrinoGeneta1", &neutrinoGeneta1, &b_neutrinoGeneta1);
   fChain->SetBranchAddress("neutrinoGeneta2", &neutrinoGeneta2, &b_neutrinoGeneta2);
   fChain->SetBranchAddress("neutrinoGeneta3", &neutrinoGeneta3, &b_neutrinoGeneta3);
   fChain->SetBranchAddress("neutrinoGenphi1", &neutrinoGenphi1, &b_neutrinoGenphi1);
   fChain->SetBranchAddress("neutrinoGenphi2", &neutrinoGenphi2, &b_neutrinoGenphi2);
   fChain->SetBranchAddress("neutrinoGenphi3", &neutrinoGenphi3, &b_neutrinoGenphi3);
   fChain->SetBranchAddress("neutrinoGenpid1", &neutrinoGenpid1, &b_neutrinoGenpid1);
   fChain->SetBranchAddress("neutrinoGenpid2", &neutrinoGenpid2, &b_neutrinoGenpid2);
   fChain->SetBranchAddress("neutrinoGenpid3", &neutrinoGenpid3, &b_neutrinoGenpid3);
   fChain->SetBranchAddress("neutrinoGenpt1", &neutrinoGenpt1, &b_neutrinoGenpt1);
   fChain->SetBranchAddress("neutrinoGenpt2", &neutrinoGenpt2, &b_neutrinoGenpt2);
   fChain->SetBranchAddress("neutrinoGenpt3", &neutrinoGenpt3, &b_neutrinoGenpt3);
   fChain->SetBranchAddress("neutrinoLHEeta1", &neutrinoLHEeta1, &b_neutrinoLHEeta1);
   fChain->SetBranchAddress("neutrinoLHEeta2", &neutrinoLHEeta2, &b_neutrinoLHEeta2);
   fChain->SetBranchAddress("neutrinoLHEeta3", &neutrinoLHEeta3, &b_neutrinoLHEeta3);
   fChain->SetBranchAddress("neutrinoLHEphi1", &neutrinoLHEphi1, &b_neutrinoLHEphi1);
   fChain->SetBranchAddress("neutrinoLHEphi2", &neutrinoLHEphi2, &b_neutrinoLHEphi2);
   fChain->SetBranchAddress("neutrinoLHEphi3", &neutrinoLHEphi3, &b_neutrinoLHEphi3);
   fChain->SetBranchAddress("neutrinoLHEpid1", &neutrinoLHEpid1, &b_neutrinoLHEpid1);
   fChain->SetBranchAddress("neutrinoLHEpid2", &neutrinoLHEpid2, &b_neutrinoLHEpid2);
   fChain->SetBranchAddress("neutrinoLHEpid3", &neutrinoLHEpid3, &b_neutrinoLHEpid3);
   fChain->SetBranchAddress("neutrinoLHEpt1", &neutrinoLHEpt1, &b_neutrinoLHEpt1);
   fChain->SetBranchAddress("neutrinoLHEpt2", &neutrinoLHEpt2, &b_neutrinoLHEpt2);
   fChain->SetBranchAddress("neutrinoLHEpt3", &neutrinoLHEpt3, &b_neutrinoLHEpt3);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("njet_puppi", &njet_puppi, &b_njet_puppi);
   fChain->SetBranchAddress("njetid", &njetid, &b_njetid);
   fChain->SetBranchAddress("njetid_puppi", &njetid_puppi, &b_njetid_puppi);
   fChain->SetBranchAddress("njetvb_puppi", &njetvb_puppi, &b_njetvb_puppi);
   fChain->SetBranchAddress("njetvbf", &njetvbf, &b_njetvbf);
   fChain->SetBranchAddress("numbLHE", &numbLHE, &b_numbLHE);
   fChain->SetBranchAddress("numtLHE", &numtLHE, &b_numtLHE);
   fChain->SetBranchAddress("pdfid1", &pdfid1, &b_pdfid1);
   fChain->SetBranchAddress("pdfid2", &pdfid2, &b_pdfid2);
   fChain->SetBranchAddress("pdfscalePDF", &pdfscalePDF, &b_pdfscalePDF);
   fChain->SetBranchAddress("pdfx1", &pdfx1, &b_pdfx1);
   fChain->SetBranchAddress("pdfx1PDF", &pdfx1PDF, &b_pdfx1PDF);
   fChain->SetBranchAddress("pdfx2", &pdfx2, &b_pdfx2);
   fChain->SetBranchAddress("pdfx2PDF", &pdfx2PDF, &b_pdfx2PDF);
   fChain->SetBranchAddress("peaking", &peaking, &b_peaking);
   fChain->SetBranchAddress("pfChargedHadronIso1", &pfChargedHadronIso1, &b_pfChargedHadronIso1);
   fChain->SetBranchAddress("pfChargedHadronIso2", &pfChargedHadronIso2, &b_pfChargedHadronIso2);
   fChain->SetBranchAddress("pfChargedHadronIso3", &pfChargedHadronIso3, &b_pfChargedHadronIso3);
   fChain->SetBranchAddress("pfChargedHadronIso4", &pfChargedHadronIso4, &b_pfChargedHadronIso4);
   fChain->SetBranchAddress("pfGammaIso1", &pfGammaIso1, &b_pfGammaIso1);
   fChain->SetBranchAddress("pfGammaIso2", &pfGammaIso2, &b_pfGammaIso2);
   fChain->SetBranchAddress("pfGammaIso3", &pfGammaIso3, &b_pfGammaIso3);
   fChain->SetBranchAddress("pfGammaIso4", &pfGammaIso4, &b_pfGammaIso4);
   fChain->SetBranchAddress("pfNeutralHadronIso1", &pfNeutralHadronIso1, &b_pfNeutralHadronIso1);
   fChain->SetBranchAddress("pfNeutralHadronIso2", &pfNeutralHadronIso2, &b_pfNeutralHadronIso2);
   fChain->SetBranchAddress("pfNeutralHadronIso3", &pfNeutralHadronIso3, &b_pfNeutralHadronIso3);
   fChain->SetBranchAddress("pfNeutralHadronIso4", &pfNeutralHadronIso4, &b_pfNeutralHadronIso4);
   fChain->SetBranchAddress("pfPUfChargedHadronIso1", &pfPUfChargedHadronIso1, &b_pfPUfChargedHadronIso1);
   fChain->SetBranchAddress("pfPUfChargedHadronIso2", &pfPUfChargedHadronIso2, &b_pfPUfChargedHadronIso2);
   fChain->SetBranchAddress("pfPUfChargedHadronIso3", &pfPUfChargedHadronIso3, &b_pfPUfChargedHadronIso3);
   fChain->SetBranchAddress("pfPUfChargedHadronIso4", &pfPUfChargedHadronIso4, &b_pfPUfChargedHadronIso4);
   fChain->SetBranchAddress("pfParticleAllIso1", &pfParticleAllIso1, &b_pfParticleAllIso1);
   fChain->SetBranchAddress("pfParticleAllIso2", &pfParticleAllIso2, &b_pfParticleAllIso2);
   fChain->SetBranchAddress("pfParticleAllIso3", &pfParticleAllIso3, &b_pfParticleAllIso3);
   fChain->SetBranchAddress("pfParticleAllIso4", &pfParticleAllIso4, &b_pfParticleAllIso4);
   fChain->SetBranchAddress("pfSumEt", &pfSumEt, &b_pfSumEt);
   fChain->SetBranchAddress("pfSumEt_puppi", &pfSumEt_puppi, &b_pfSumEt_puppi);
   fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
   fChain->SetBranchAddress("pfmetMEtSig", &pfmetMEtSig, &b_pfmetMEtSig);
   fChain->SetBranchAddress("pfmetSignificance", &pfmetSignificance, &b_pfmetSignificance);
   fChain->SetBranchAddress("pfmet_puppi", &pfmet_puppi, &b_pfmet_puppi);
   fChain->SetBranchAddress("pfmetphi", &pfmetphi, &b_pfmetphi);
   fChain->SetBranchAddress("pfmetphi_puppi", &pfmetphi_puppi, &b_pfmetphi_puppi);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("phi3", &phi3, &b_phi3);
   fChain->SetBranchAddress("phi4", &phi4, &b_phi4);
   fChain->SetBranchAddress("pid1", &pid1, &b_pid1);
   fChain->SetBranchAddress("pid2", &pid2, &b_pid2);
   fChain->SetBranchAddress("pid3", &pid3, &b_pid3);
   fChain->SetBranchAddress("pid4", &pid4, &b_pid4);
   fChain->SetBranchAddress("ppfmet", &ppfmet, &b_ppfmet);
   fChain->SetBranchAddress("ppfmet_puppi", &ppfmet_puppi, &b_ppfmet_puppi);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("pt3", &pt3, &b_pt3);
   fChain->SetBranchAddress("pt4", &pt4, &b_pt4);
   fChain->SetBranchAddress("ptll", &ptll, &b_ptll);
   fChain->SetBranchAddress("puW", &puW, &b_puW);
   fChain->SetBranchAddress("sceta1", &sceta1, &b_sceta1);
   fChain->SetBranchAddress("sceta2", &sceta2, &b_sceta2);
   fChain->SetBranchAddress("sceta3", &sceta3, &b_sceta3);
   fChain->SetBranchAddress("sceta4", &sceta4, &b_sceta4);
   fChain->SetBranchAddress("softbcsv", &softbcsv, &b_softbcsv);
   fChain->SetBranchAddress("softbcsvMVA", &softbcsvMVA, &b_softbcsvMVA);
   fChain->SetBranchAddress("softbjpb", &softbjpb, &b_softbjpb);
   fChain->SetBranchAddress("softtche", &softtche, &b_softtche);
   fChain->SetBranchAddress("softtchp", &softtchp, &b_softtchp);
   //   fChain->SetBranchAddress("std_vector_lepton_eta", &std_vector_lepton_eta, &b_std_vector_lepton_eta);
   //   fChain->SetBranchAddress("std_vector_lepton_phi", &std_vector_lepton_phi, &b_std_vector_lepton_phi);
   //   fChain->SetBranchAddress("std_vector_lepton_pt", &std_vector_lepton_pt, &b_std_vector_lepton_pt);
   fChain->SetBranchAddress("triggAW", &triggAW, &b_triggAW);
   fChain->SetBranchAddress("triggBW", &triggBW, &b_triggBW);
   fChain->SetBranchAddress("triggW", &triggW, &b_triggW);
   fChain->SetBranchAddress("trigger", &trigger, &b_trigger);
   fChain->SetBranchAddress("worstJetLepPt", &worstJetLepPt, &b_worstJetLepPt);
   fChain->SetBranchAddress("yll", &yll, &b_yll);
   fChain->SetBranchAddress("itpu", &itpu, &b_itpu);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("ootpum1", &ootpum1, &b_ootpum1);
   fChain->SetBranchAddress("ootpup1", &ootpup1, &b_ootpup1);
   fChain->SetBranchAddress("trpu", &trpu, &b_trpu);
   fChain->SetBranchAddress("bveto", &bveto, &b_bveto);
   fChain->SetBranchAddress("bveto_csvl", &bveto_csvl, &b_bveto_csvl);
   fChain->SetBranchAddress("bveto_csvl_ip", &bveto_csvl_ip, &b_bveto_csvl_ip);
   fChain->SetBranchAddress("bveto_csvl_ip_puppi", &bveto_csvl_ip_puppi, &b_bveto_csvl_ip_puppi);
   fChain->SetBranchAddress("bveto_csvl_nj", &bveto_csvl_nj, &b_bveto_csvl_nj);
   fChain->SetBranchAddress("bveto_csvl_nj05", &bveto_csvl_nj05, &b_bveto_csvl_nj05);
   fChain->SetBranchAddress("bveto_csvl_nj05_puppi", &bveto_csvl_nj05_puppi, &b_bveto_csvl_nj05_puppi);
   fChain->SetBranchAddress("bveto_csvl_nj30", &bveto_csvl_nj30, &b_bveto_csvl_nj30);
   fChain->SetBranchAddress("bveto_csvl_nj3005", &bveto_csvl_nj3005, &b_bveto_csvl_nj3005);
   fChain->SetBranchAddress("bveto_csvl_nj3005_puppi", &bveto_csvl_nj3005_puppi, &b_bveto_csvl_nj3005_puppi);
   fChain->SetBranchAddress("bveto_csvl_nj30_puppi", &bveto_csvl_nj30_puppi, &b_bveto_csvl_nj30_puppi);
   fChain->SetBranchAddress("bveto_csvl_nj_puppi", &bveto_csvl_nj_puppi, &b_bveto_csvl_nj_puppi);
   fChain->SetBranchAddress("bveto_csvl_puppi", &bveto_csvl_puppi, &b_bveto_csvl_puppi);
   fChain->SetBranchAddress("bveto_csvm", &bveto_csvm, &b_bveto_csvm);
   fChain->SetBranchAddress("bveto_csvm_ip", &bveto_csvm_ip, &b_bveto_csvm_ip);
   fChain->SetBranchAddress("bveto_csvm_ip_puppi", &bveto_csvm_ip_puppi, &b_bveto_csvm_ip_puppi);
   fChain->SetBranchAddress("bveto_csvm_nj", &bveto_csvm_nj, &b_bveto_csvm_nj);
   fChain->SetBranchAddress("bveto_csvm_nj05", &bveto_csvm_nj05, &b_bveto_csvm_nj05);
   fChain->SetBranchAddress("bveto_csvm_nj05_puppi", &bveto_csvm_nj05_puppi, &b_bveto_csvm_nj05_puppi);
   fChain->SetBranchAddress("bveto_csvm_nj30", &bveto_csvm_nj30, &b_bveto_csvm_nj30);
   fChain->SetBranchAddress("bveto_csvm_nj3005", &bveto_csvm_nj3005, &b_bveto_csvm_nj3005);
   fChain->SetBranchAddress("bveto_csvm_nj3005_puppi", &bveto_csvm_nj3005_puppi, &b_bveto_csvm_nj3005_puppi);
   fChain->SetBranchAddress("bveto_csvm_nj30_puppi", &bveto_csvm_nj30_puppi, &b_bveto_csvm_nj30_puppi);
   fChain->SetBranchAddress("bveto_csvm_nj_puppi", &bveto_csvm_nj_puppi, &b_bveto_csvm_nj_puppi);
   fChain->SetBranchAddress("bveto_csvm_puppi", &bveto_csvm_puppi, &b_bveto_csvm_puppi);
   fChain->SetBranchAddress("bveto_csvt", &bveto_csvt, &b_bveto_csvt);
   fChain->SetBranchAddress("bveto_csvt_ip", &bveto_csvt_ip, &b_bveto_csvt_ip);
   fChain->SetBranchAddress("bveto_csvt_ip_puppi", &bveto_csvt_ip_puppi, &b_bveto_csvt_ip_puppi);
   fChain->SetBranchAddress("bveto_csvt_nj", &bveto_csvt_nj, &b_bveto_csvt_nj);
   fChain->SetBranchAddress("bveto_csvt_nj05", &bveto_csvt_nj05, &b_bveto_csvt_nj05);
   fChain->SetBranchAddress("bveto_csvt_nj05_puppi", &bveto_csvt_nj05_puppi, &b_bveto_csvt_nj05_puppi);
   fChain->SetBranchAddress("bveto_csvt_nj30", &bveto_csvt_nj30, &b_bveto_csvt_nj30);
   fChain->SetBranchAddress("bveto_csvt_nj3005", &bveto_csvt_nj3005, &b_bveto_csvt_nj3005);
   fChain->SetBranchAddress("bveto_csvt_nj3005_puppi", &bveto_csvt_nj3005_puppi, &b_bveto_csvt_nj3005_puppi);
   fChain->SetBranchAddress("bveto_csvt_nj30_puppi", &bveto_csvt_nj30_puppi, &b_bveto_csvt_nj30_puppi);
   fChain->SetBranchAddress("bveto_csvt_nj_puppi", &bveto_csvt_nj_puppi, &b_bveto_csvt_nj_puppi);
   fChain->SetBranchAddress("bveto_csvt_puppi", &bveto_csvt_puppi, &b_bveto_csvt_puppi);
   fChain->SetBranchAddress("bveto_ip", &bveto_ip, &b_bveto_ip);
   fChain->SetBranchAddress("bveto_ip_puppi", &bveto_ip_puppi, &b_bveto_ip_puppi);
   fChain->SetBranchAddress("bveto_mu", &bveto_mu, &b_bveto_mu);
   fChain->SetBranchAddress("bveto_munj", &bveto_munj, &b_bveto_munj);
   fChain->SetBranchAddress("bveto_munj05", &bveto_munj05, &b_bveto_munj05);
   fChain->SetBranchAddress("bveto_munj30", &bveto_munj30, &b_bveto_munj30);
   fChain->SetBranchAddress("bveto_munj3005", &bveto_munj3005, &b_bveto_munj3005);
   fChain->SetBranchAddress("bveto_nj", &bveto_nj, &b_bveto_nj);
   fChain->SetBranchAddress("bveto_nj05", &bveto_nj05, &b_bveto_nj05);
   fChain->SetBranchAddress("bveto_nj05_puppi", &bveto_nj05_puppi, &b_bveto_nj05_puppi);
   fChain->SetBranchAddress("bveto_nj30", &bveto_nj30, &b_bveto_nj30);
   fChain->SetBranchAddress("bveto_nj3005", &bveto_nj3005, &b_bveto_nj3005);
   fChain->SetBranchAddress("bveto_nj3005_puppi", &bveto_nj3005_puppi, &b_bveto_nj3005_puppi);
   fChain->SetBranchAddress("bveto_nj30_puppi", &bveto_nj30_puppi, &b_bveto_nj30_puppi);
   fChain->SetBranchAddress("bveto_nj_puppi", &bveto_nj_puppi, &b_bveto_nj_puppi);
   fChain->SetBranchAddress("bveto_puppi", &bveto_puppi, &b_bveto_puppi);
   fChain->SetBranchAddress("dphiveto", &dphiveto, &b_dphiveto);
   fChain->SetBranchAddress("nextra", &nextra, &b_nextra);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   Notify();
}

Bool_t readTreeLatino::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readTreeLatino::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readTreeLatino::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readTreeLatino_cxx
