//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 16 18:35:45 2015 by ROOT version 6.02/03
// from TTree readTree/readTree
// found on file: /data2/rgerosa/NTUPLES_TP/PHANTOM_SS_EWK_DF_126_2p5/outDumper_0.root
//////////////////////////////////////////////////////////

#ifndef readTree_h
#define readTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class readTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static const Int_t kMaxTrackJet_V4 = 49;

   // Declaration of leaf types
   Float_t         leptonLHEpt1;
   Float_t         leptonLHEeta1;
   Float_t         leptonLHEphi1;
   Int_t           leptonLHEpid1;
   Int_t           leptonLHEspin1;
   Int_t           leptonLHEch1;
   Float_t         leptonLHEm1;
   Float_t         neutrinoLHEpt1;
   Float_t         neutrinoLHEeta1;
   Float_t         neutrinoLHEphi1;
   Float_t         neutrinoLHEpid1;
   Float_t         neutrinoLHEspin1;
   Float_t         jetLHEPartonpt1;
   Float_t         jetLHEPartoneta1;
   Float_t         jetLHEPartonphi1;
   Int_t           jetLHEPartonpid1;
   Int_t           jetLHEPartonspin1;
   Float_t         leptonLHEpt2;
   Float_t         leptonLHEeta2;
   Float_t         leptonLHEphi2;
   Int_t           leptonLHEpid2;
   Int_t           leptonLHEspin2;
   Int_t           leptonLHEch2;
   Float_t         leptonLHEm2;
   Float_t         neutrinoLHEpt2;
   Float_t         neutrinoLHEeta2;
   Float_t         neutrinoLHEphi2;
   Float_t         neutrinoLHEpid2;
   Float_t         neutrinoLHEspin2;
   Float_t         jetLHEPartonpt2;
   Float_t         jetLHEPartoneta2;
   Float_t         jetLHEPartonphi2;
   Int_t           jetLHEPartonpid2;
   Int_t           jetLHEPartonspin2;
   Float_t         leptonLHEpt3;
   Float_t         leptonLHEeta3;
   Float_t         leptonLHEphi3;
   Int_t           leptonLHEpid3;
   Int_t           leptonLHEspin3;
   Int_t           leptonLHEch3;
   Float_t         leptonLHEm3;
   Float_t         neutrinoLHEpt3;
   Float_t         neutrinoLHEeta3;
   Float_t         neutrinoLHEphi3;
   Float_t         neutrinoLHEpid3;
   Float_t         neutrinoLHEspin3;
   Float_t         jetLHEPartonpt3;
   Float_t         jetLHEPartoneta3;
   Float_t         jetLHEPartonphi3;
   Int_t           jetLHEPartonpid3;
   Int_t           jetLHEPartonspin3;
   Float_t         leptonLHEpt4;
   Float_t         leptonLHEeta4;
   Float_t         leptonLHEphi4;
   Int_t           leptonLHEpid4;
   Int_t           leptonLHEspin4;
   Int_t           leptonLHEch4;
   Float_t         leptonLHEm4;
   Float_t         neutrinoLHEpt4;
   Float_t         neutrinoLHEeta4;
   Float_t         neutrinoLHEphi4;
   Float_t         neutrinoLHEpid4;
   Float_t         neutrinoLHEspin4;
   Float_t         jetLHEPartonpt4;
   Float_t         jetLHEPartoneta4;
   Float_t         jetLHEPartonphi4;
   Int_t           jetLHEPartonpid4;
   Int_t           jetLHEPartonspin4;
   Float_t         vbosonLHEpt1;
   Float_t         vbosonLHEeta1;
   Float_t         vbosonLHEphi1;
   Int_t           vbosonLHEpid1;
   Int_t           vbosonLHEspin1;
   Int_t           vbosonLHEch1;
   Float_t         vbosonLHEm1;
   Float_t         jetLHEGluonpt1;
   Float_t         jetLHEGluoneta1;
   Float_t         jetLHEGluonphi1;
   Int_t           jetLHEGluonpid1;
   Int_t           jetLHEGluonspin1;
   Float_t         vbosonLHEpt2;
   Float_t         vbosonLHEeta2;
   Float_t         vbosonLHEphi2;
   Int_t           vbosonLHEpid2;
   Int_t           vbosonLHEspin2;
   Int_t           vbosonLHEch2;
   Float_t         vbosonLHEm2;
   Float_t         jetLHEGluonpt2;
   Float_t         jetLHEGluoneta2;
   Float_t         jetLHEGluonphi2;
   Int_t           jetLHEGluonpid2;
   Int_t           jetLHEGluonspin2;
   Float_t         jetGenpt1;
   Float_t         jetGeneta1;
   Float_t         jetGenphi1;
   Float_t         jetGenm1;
   Float_t         jetGenAreaX1;
   Float_t         jetGenAreaY1;
   Float_t         jetGenAreaZ1;
   Float_t         jetGenAreaT1;
   Float_t         jetGenpt2;
   Float_t         jetGeneta2;
   Float_t         jetGenphi2;
   Float_t         jetGenm2;
   Float_t         jetGenAreaX2;
   Float_t         jetGenAreaY2;
   Float_t         jetGenAreaZ2;
   Float_t         jetGenAreaT2;
   Float_t         jetGenpt3;
   Float_t         jetGeneta3;
   Float_t         jetGenphi3;
   Float_t         jetGenm3;
   Float_t         jetGenAreaX3;
   Float_t         jetGenAreaY3;
   Float_t         jetGenAreaZ3;
   Float_t         jetGenAreaT3;
   Float_t         jetGenpt4;
   Float_t         jetGeneta4;
   Float_t         jetGenphi4;
   Float_t         jetGenm4;
   Float_t         jetGenAreaX4;
   Float_t         jetGenAreaY4;
   Float_t         jetGenAreaZ4;
   Float_t         jetGenAreaT4;
   Int_t           TrackJet_V4_;
   UInt_t          TrackJet_V4_fUniqueID[kMaxTrackJet_V4];   //[TrackJet_V4_]
   UInt_t          TrackJet_V4_fBits[kMaxTrackJet_V4];   //[TrackJet_V4_]
   UInt_t          TrackJet_V4_fP_fUniqueID[kMaxTrackJet_V4];   //[TrackJet_V4_]
   UInt_t          TrackJet_V4_fP_fBits[kMaxTrackJet_V4];   //[TrackJet_V4_]
   Double_t        TrackJet_V4_fP_fX[kMaxTrackJet_V4];   //[TrackJet_V4_]
   Double_t        TrackJet_V4_fP_fY[kMaxTrackJet_V4];   //[TrackJet_V4_]
   Double_t        TrackJet_V4_fP_fZ[kMaxTrackJet_V4];   //[TrackJet_V4_]
   Double_t        TrackJet_V4_fE[kMaxTrackJet_V4];   //[TrackJet_V4_]
   Float_t         HtSoft;
   Float_t         nSoftJets;
   Float_t         mjj;
   Float_t         detajj;
   Int_t           njet;
   Int_t           nbjet;
   Int_t           hardbjpb;
   Int_t           softbjpb;
   Int_t           njetid;
   Float_t         jetpt1;
   Float_t         jeteta1;
   Float_t         jetphi1;
   Float_t         jetmass1;
   Float_t         jetAreaX1;
   Float_t         jetAreaY1;
   Float_t         jetAreaZ1;
   Float_t         jetAreaT1;
   Float_t         jetBTagAlgo1;
   Float_t         jetBTagDefault1;
   Float_t         jetBTagPhysics1;
   Float_t         jetBTagNearest2_1;
   Float_t         jetBTagNearest3_1;
   Float_t         jetBTagHeaviest_1;
   Float_t         jetFlavourAlgo1;
   Float_t         jetFlavourDefault1;
   Float_t         jetFlavourPhysics1;
   Float_t         jetFlavourNearest2_1;
   Float_t         jetFlavourNearest3_1;
   Float_t         jetFlavourHeaviest_1;
   Float_t         jetptD1;
   Float_t         jetptDNe1;
   Float_t         jetptDCh1;
   Float_t         jetnNeutral1;
   Float_t         jetnCharged1;
   Float_t         jetneuEMfrac1;
   Float_t         jetneuHadfrac1;
   Float_t         jetbetaClassic1;
   Float_t         jetbetaClassicStar1;
   Float_t         jetbeta1;
   Float_t         jetbetaStar1;
   Float_t         jetconstituents1;
   Float_t         jetaxis2_1;
   Int_t           jetpileupIDFlagCutBased1;
   Float_t         jetpt2;
   Float_t         jeteta2;
   Float_t         jetphi2;
   Float_t         jetmass2;
   Float_t         jetAreaX2;
   Float_t         jetAreaY2;
   Float_t         jetAreaZ2;
   Float_t         jetAreaT2;
   Float_t         jetBTagAlgo2;
   Float_t         jetBTagDefault2;
   Float_t         jetBTagPhysics2;
   Float_t         jetBTagNearest2_2;
   Float_t         jetBTagNearest3_2;
   Float_t         jetBTagHeaviest_2;
   Float_t         jetFlavourAlgo2;
   Float_t         jetFlavourDefault2;
   Float_t         jetFlavourPhysics2;
   Float_t         jetFlavourNearest2_2;
   Float_t         jetFlavourNearest3_2;
   Float_t         jetFlavourHeaviest_2;
   Float_t         jetptD2;
   Float_t         jetptDNe2;
   Float_t         jetptDCh2;
   Float_t         jetnNeutral2;
   Float_t         jetnCharged2;
   Float_t         jetneuEMfrac2;
   Float_t         jetneuHadfrac2;
   Float_t         jetbetaClassic2;
   Float_t         jetbetaClassicStar2;
   Float_t         jetbeta2;
   Float_t         jetbetaStar2;
   Float_t         jetconstituents2;
   Float_t         jetaxis2_2;
   Int_t           jetpileupIDFlagCutBased2;
   Float_t         jetpt3;
   Float_t         jeteta3;
   Float_t         jetphi3;
   Float_t         jetmass3;
   Float_t         jetAreaX3;
   Float_t         jetAreaY3;
   Float_t         jetAreaZ3;
   Float_t         jetAreaT3;
   Float_t         jetBTagAlgo3;
   Float_t         jetBTagDefault3;
   Float_t         jetBTagPhysics3;
   Float_t         jetBTagNearest2_3;
   Float_t         jetBTagNearest3_3;
   Float_t         jetBTagHeaviest_3;
   Float_t         jetFlavourAlgo3;
   Float_t         jetFlavourDefault3;
   Float_t         jetFlavourPhysics3;
   Float_t         jetFlavourNearest2_3;
   Float_t         jetFlavourNearest3_3;
   Float_t         jetFlavourHeaviest_3;
   Float_t         jetptD3;
   Float_t         jetptDNe3;
   Float_t         jetptDCh3;
   Float_t         jetnNeutral3;
   Float_t         jetnCharged3;
   Float_t         jetneuEMfrac3;
   Float_t         jetneuHadfrac3;
   Float_t         jetbetaClassic3;
   Float_t         jetbetaClassicStar3;
   Float_t         jetbeta3;
   Float_t         jetbetaStar3;
   Float_t         jetconstituents3;
   Float_t         jetaxis2_3;
   Int_t           jetpileupIDFlagCutBased3;
   Float_t         jetpt4;
   Float_t         jeteta4;
   Float_t         jetphi4;
   Float_t         jetmass4;
   Float_t         jetAreaX4;
   Float_t         jetAreaY4;
   Float_t         jetAreaZ4;
   Float_t         jetAreaT4;
   Float_t         jetBTagAlgo4;
   Float_t         jetBTagDefault4;
   Float_t         jetBTagPhysics4;
   Float_t         jetBTagNearest2_4;
   Float_t         jetBTagNearest3_4;
   Float_t         jetBTagHeaviest_4;
   Float_t         jetFlavourAlgo4;
   Float_t         jetFlavourDefault4;
   Float_t         jetFlavourPhysics4;
   Float_t         jetFlavourNearest2_4;
   Float_t         jetFlavourNearest3_4;
   Float_t         jetFlavourHeaviest_4;
   Float_t         jetptD4;
   Float_t         jetptDNe4;
   Float_t         jetptDCh4;
   Float_t         jetnNeutral4;
   Float_t         jetnCharged4;
   Float_t         jetneuEMfrac4;
   Float_t         jetneuHadfrac4;
   Float_t         jetbetaClassic4;
   Float_t         jetbetaClassicStar4;
   Float_t         jetbeta4;
   Float_t         jetbetaStar4;
   Float_t         jetconstituents4;
   Float_t         jetaxis2_4;
   Int_t           jetpileupIDFlagCutBased4;
   Float_t         jetpt5;
   Float_t         jeteta5;
   Float_t         jetphi5;
   Float_t         jetmass5;
   Float_t         jetAreaX5;
   Float_t         jetAreaY5;
   Float_t         jetAreaZ5;
   Float_t         jetAreaT5;
   Float_t         jetBTagAlgo5;
   Float_t         jetBTagDefault5;
   Float_t         jetBTagPhysics5;
   Float_t         jetBTagNearest2_5;
   Float_t         jetBTagNearest3_5;
   Float_t         jetBTagHeaviest_5;
   Float_t         jetFlavourAlgo5;
   Float_t         jetFlavourDefault5;
   Float_t         jetFlavourPhysics5;
   Float_t         jetFlavourNearest2_5;
   Float_t         jetFlavourNearest3_5;
   Float_t         jetFlavourHeaviest_5;
   Float_t         jetptD5;
   Float_t         jetptDNe5;
   Float_t         jetptDCh5;
   Float_t         jetnNeutral5;
   Float_t         jetnCharged5;
   Float_t         jetneuEMfrac5;
   Float_t         jetneuHadfrac5;
   Float_t         jetbetaClassic5;
   Float_t         jetbetaClassicStar5;
   Float_t         jetbeta5;
   Float_t         jetbetaStar5;
   Float_t         jetconstituents5;
   Float_t         jetaxis2_5;
   Int_t           jetpileupIDFlagCutBased5;
   Float_t         jetpt6;
   Float_t         jeteta6;
   Float_t         jetphi6;
   Float_t         jetmass6;
   Float_t         jetAreaX6;
   Float_t         jetAreaY6;
   Float_t         jetAreaZ6;
   Float_t         jetAreaT6;
   Float_t         jetBTagAlgo6;
   Float_t         jetBTagDefault6;
   Float_t         jetBTagPhysics6;
   Float_t         jetBTagNearest2_6;
   Float_t         jetBTagNearest3_6;
   Float_t         jetBTagHeaviest_6;
   Float_t         jetFlavourAlgo6;
   Float_t         jetFlavourDefault6;
   Float_t         jetFlavourPhysics6;
   Float_t         jetFlavourNearest2_6;
   Float_t         jetFlavourNearest3_6;
   Float_t         jetFlavourHeaviest_6;
   Float_t         jetptD6;
   Float_t         jetptDNe6;
   Float_t         jetptDCh6;
   Float_t         jetnNeutral6;
   Float_t         jetnCharged6;
   Float_t         jetneuEMfrac6;
   Float_t         jetneuHadfrac6;
   Float_t         jetbetaClassic6;
   Float_t         jetbetaClassicStar6;
   Float_t         jetbeta6;
   Float_t         jetbetaStar6;
   Float_t         jetconstituents6;
   Float_t         jetaxis2_6;
   Int_t           jetpileupIDFlagCutBased6;
   Float_t         jetpt7;
   Float_t         jeteta7;
   Float_t         jetphi7;
   Float_t         jetmass7;
   Float_t         jetAreaX7;
   Float_t         jetAreaY7;
   Float_t         jetAreaZ7;
   Float_t         jetAreaT7;
   Float_t         jetBTagAlgo7;
   Float_t         jetBTagDefault7;
   Float_t         jetBTagPhysics7;
   Float_t         jetBTagNearest2_7;
   Float_t         jetBTagNearest3_7;
   Float_t         jetBTagHeaviest_7;
   Float_t         jetFlavourAlgo7;
   Float_t         jetFlavourDefault7;
   Float_t         jetFlavourPhysics7;
   Float_t         jetFlavourNearest2_7;
   Float_t         jetFlavourNearest3_7;
   Float_t         jetFlavourHeaviest_7;
   Float_t         jetptD7;
   Float_t         jetptDNe7;
   Float_t         jetptDCh7;
   Float_t         jetnNeutral7;
   Float_t         jetnCharged7;
   Float_t         jetneuEMfrac7;
   Float_t         jetneuHadfrac7;
   Float_t         jetbetaClassic7;
   Float_t         jetbetaClassicStar7;
   Float_t         jetbeta7;
   Float_t         jetbetaStar7;
   Float_t         jetconstituents7;
   Float_t         jetaxis2_7;
   Int_t           jetpileupIDFlagCutBased7;
   Float_t         jetpt8;
   Float_t         jeteta8;
   Float_t         jetphi8;
   Float_t         jetmass8;
   Float_t         jetAreaX8;
   Float_t         jetAreaY8;
   Float_t         jetAreaZ8;
   Float_t         jetAreaT8;
   Float_t         jetBTagAlgo8;
   Float_t         jetBTagDefault8;
   Float_t         jetBTagPhysics8;
   Float_t         jetBTagNearest2_8;
   Float_t         jetBTagNearest3_8;
   Float_t         jetBTagHeaviest_8;
   Float_t         jetFlavourAlgo8;
   Float_t         jetFlavourDefault8;
   Float_t         jetFlavourPhysics8;
   Float_t         jetFlavourNearest2_8;
   Float_t         jetFlavourNearest3_8;
   Float_t         jetFlavourHeaviest_8;
   Float_t         jetptD8;
   Float_t         jetptDNe8;
   Float_t         jetptDCh8;
   Float_t         jetnNeutral8;
   Float_t         jetnCharged8;
   Float_t         jetneuEMfrac8;
   Float_t         jetneuHadfrac8;
   Float_t         jetbetaClassic8;
   Float_t         jetbetaClassicStar8;
   Float_t         jetbeta8;
   Float_t         jetbetaStar8;
   Float_t         jetconstituents8;
   Float_t         jetaxis2_8;
   Int_t           jetpileupIDFlagCutBased8;
   Float_t         mjj_puppi;
   Float_t         detajj_puppi;
   Int_t           njet_puppi;
   Int_t           nbjet_puppi;
   Int_t           hardbjpb_puppi;
   Int_t           softbjpb_puppi;
   Int_t           njetid_puppi;
   Float_t         jetpt_puppi1;
   Float_t         jeteta_puppi1;
   Float_t         jetphi_puppi1;
   Float_t         jetmass_puppi1;
   Float_t         jetAreaX_puppi1;
   Float_t         jetAreaY_puppi1;
   Float_t         jetAreaZ_puppi1;
   Float_t         jetAreaT_puppi1;
   Float_t         jetBTagAlgo_puppi1;
   Float_t         jetBTagDefault_puppi1;
   Float_t         jetBTagPhysics_puppi1;
   Float_t         jetBTagNearest2_puppi1;
   Float_t         jetBTagNearest3_puppi1;
   Float_t         jetBTagHeaviest_puppi1;
   Float_t         jetFlavourAlgo_puppi1;
   Float_t         jetFlavourDefault_puppi1;
   Float_t         jetFlavourPhysics_puppi1;
   Float_t         jetFlavourNearest2_puppi1;
   Float_t         jetFlavourNearest3_puppi1;
   Float_t         jetFlavourHeaviest_puppi1;
   Float_t         jetptD_puppi1;
   Float_t         jetptDNe_puppi1;
   Float_t         jetptDCh_puppi1;
   Float_t         jetnNeutral_puppi1;
   Float_t         jetnCharged_puppi1;
   Float_t         jetneuEMfrac_puppi1;
   Float_t         jetneuHadfrac_puppi1;
   Float_t         jetbetaClassic_puppiStr1;
   Float_t         jetbetaClassicStar_puppiStr1;
   Float_t         jetbeta_puppiStr1;
   Float_t         jetbetaStar_puppiStr1;
   Float_t         jetconstituents_puppiStr1;
   Float_t         jetaxis2_puppiStr1;
   Int_t           jetpileupIDFlagCutBased_puppi1;
   Float_t         jetpt_puppi2;
   Float_t         jeteta_puppi2;
   Float_t         jetphi_puppi2;
   Float_t         jetmass_puppi2;
   Float_t         jetAreaX_puppi2;
   Float_t         jetAreaY_puppi2;
   Float_t         jetAreaZ_puppi2;
   Float_t         jetAreaT_puppi2;
   Float_t         jetBTagAlgo_puppi2;
   Float_t         jetBTagDefault_puppi2;
   Float_t         jetBTagPhysics_puppi2;
   Float_t         jetBTagNearest2_puppi2;
   Float_t         jetBTagNearest3_puppi2;
   Float_t         jetBTagHeaviest_puppi2;
   Float_t         jetFlavourAlgo_puppi2;
   Float_t         jetFlavourDefault_puppi2;
   Float_t         jetFlavourPhysics_puppi2;
   Float_t         jetFlavourNearest2_puppi2;
   Float_t         jetFlavourNearest3_puppi2;
   Float_t         jetFlavourHeaviest_puppi2;
   Float_t         jetptD_puppi2;
   Float_t         jetptDNe_puppi2;
   Float_t         jetptDCh_puppi2;
   Float_t         jetnNeutral_puppi2;
   Float_t         jetnCharged_puppi2;
   Float_t         jetneuEMfrac_puppi2;
   Float_t         jetneuHadfrac_puppi2;
   Float_t         jetbetaClassic_puppiStr2;
   Float_t         jetbetaClassicStar_puppiStr2;
   Float_t         jetbeta_puppiStr2;
   Float_t         jetbetaStar_puppiStr2;
   Float_t         jetconstituents_puppiStr2;
   Float_t         jetaxis2_puppiStr2;
   Int_t           jetpileupIDFlagCutBased_puppi2;
   Float_t         jetpt_puppi3;
   Float_t         jeteta_puppi3;
   Float_t         jetphi_puppi3;
   Float_t         jetmass_puppi3;
   Float_t         jetAreaX_puppi3;
   Float_t         jetAreaY_puppi3;
   Float_t         jetAreaZ_puppi3;
   Float_t         jetAreaT_puppi3;
   Float_t         jetBTagAlgo_puppi3;
   Float_t         jetBTagDefault_puppi3;
   Float_t         jetBTagPhysics_puppi3;
   Float_t         jetBTagNearest2_puppi3;
   Float_t         jetBTagNearest3_puppi3;
   Float_t         jetBTagHeaviest_puppi3;
   Float_t         jetFlavourAlgo_puppi3;
   Float_t         jetFlavourDefault_puppi3;
   Float_t         jetFlavourPhysics_puppi3;
   Float_t         jetFlavourNearest2_puppi3;
   Float_t         jetFlavourNearest3_puppi3;
   Float_t         jetFlavourHeaviest_puppi3;
   Float_t         jetptD_puppi3;
   Float_t         jetptDNe_puppi3;
   Float_t         jetptDCh_puppi3;
   Float_t         jetnNeutral_puppi3;
   Float_t         jetnCharged_puppi3;
   Float_t         jetneuEMfrac_puppi3;
   Float_t         jetneuHadfrac_puppi3;
   Float_t         jetbetaClassic_puppiStr3;
   Float_t         jetbetaClassicStar_puppiStr3;
   Float_t         jetbeta_puppiStr3;
   Float_t         jetbetaStar_puppiStr3;
   Float_t         jetconstituents_puppiStr3;
   Float_t         jetaxis2_puppiStr3;
   Int_t           jetpileupIDFlagCutBased_puppi3;
   Float_t         jetpt_puppi4;
   Float_t         jeteta_puppi4;
   Float_t         jetphi_puppi4;
   Float_t         jetmass_puppi4;
   Float_t         jetAreaX_puppi4;
   Float_t         jetAreaY_puppi4;
   Float_t         jetAreaZ_puppi4;
   Float_t         jetAreaT_puppi4;
   Float_t         jetBTagAlgo_puppi4;
   Float_t         jetBTagDefault_puppi4;
   Float_t         jetBTagPhysics_puppi4;
   Float_t         jetBTagNearest2_puppi4;
   Float_t         jetBTagNearest3_puppi4;
   Float_t         jetBTagHeaviest_puppi4;
   Float_t         jetFlavourAlgo_puppi4;
   Float_t         jetFlavourDefault_puppi4;
   Float_t         jetFlavourPhysics_puppi4;
   Float_t         jetFlavourNearest2_puppi4;
   Float_t         jetFlavourNearest3_puppi4;
   Float_t         jetFlavourHeaviest_puppi4;
   Float_t         jetptD_puppi4;
   Float_t         jetptDNe_puppi4;
   Float_t         jetptDCh_puppi4;
   Float_t         jetnNeutral_puppi4;
   Float_t         jetnCharged_puppi4;
   Float_t         jetneuEMfrac_puppi4;
   Float_t         jetneuHadfrac_puppi4;
   Float_t         jetbetaClassic_puppiStr4;
   Float_t         jetbetaClassicStar_puppiStr4;
   Float_t         jetbeta_puppiStr4;
   Float_t         jetbetaStar_puppiStr4;
   Float_t         jetconstituents_puppiStr4;
   Float_t         jetaxis2_puppiStr4;
   Int_t           jetpileupIDFlagCutBased_puppi4;
   Float_t         jetpt_puppi5;
   Float_t         jeteta_puppi5;
   Float_t         jetphi_puppi5;
   Float_t         jetmass_puppi5;
   Float_t         jetAreaX_puppi5;
   Float_t         jetAreaY_puppi5;
   Float_t         jetAreaZ_puppi5;
   Float_t         jetAreaT_puppi5;
   Float_t         jetBTagAlgo_puppi5;
   Float_t         jetBTagDefault_puppi5;
   Float_t         jetBTagPhysics_puppi5;
   Float_t         jetBTagNearest2_puppi5;
   Float_t         jetBTagNearest3_puppi5;
   Float_t         jetBTagHeaviest_puppi5;
   Float_t         jetFlavourAlgo_puppi5;
   Float_t         jetFlavourDefault_puppi5;
   Float_t         jetFlavourPhysics_puppi5;
   Float_t         jetFlavourNearest2_puppi5;
   Float_t         jetFlavourNearest3_puppi5;
   Float_t         jetFlavourHeaviest_puppi5;
   Float_t         jetptD_puppi5;
   Float_t         jetptDNe_puppi5;
   Float_t         jetptDCh_puppi5;
   Float_t         jetnNeutral_puppi5;
   Float_t         jetnCharged_puppi5;
   Float_t         jetneuEMfrac_puppi5;
   Float_t         jetneuHadfrac_puppi5;
   Float_t         jetbetaClassic_puppiStr5;
   Float_t         jetbetaClassicStar_puppiStr5;
   Float_t         jetbeta_puppiStr5;
   Float_t         jetbetaStar_puppiStr5;
   Float_t         jetconstituents_puppiStr5;
   Float_t         jetaxis2_puppiStr5;
   Int_t           jetpileupIDFlagCutBased_puppi5;
   Float_t         jetpt_puppi6;
   Float_t         jeteta_puppi6;
   Float_t         jetphi_puppi6;
   Float_t         jetmass_puppi6;
   Float_t         jetAreaX_puppi6;
   Float_t         jetAreaY_puppi6;
   Float_t         jetAreaZ_puppi6;
   Float_t         jetAreaT_puppi6;
   Float_t         jetBTagAlgo_puppi6;
   Float_t         jetBTagDefault_puppi6;
   Float_t         jetBTagPhysics_puppi6;
   Float_t         jetBTagNearest2_puppi6;
   Float_t         jetBTagNearest3_puppi6;
   Float_t         jetBTagHeaviest_puppi6;
   Float_t         jetFlavourAlgo_puppi6;
   Float_t         jetFlavourDefault_puppi6;
   Float_t         jetFlavourPhysics_puppi6;
   Float_t         jetFlavourNearest2_puppi6;
   Float_t         jetFlavourNearest3_puppi6;
   Float_t         jetFlavourHeaviest_puppi6;
   Float_t         jetptD_puppi6;
   Float_t         jetptDNe_puppi6;
   Float_t         jetptDCh_puppi6;
   Float_t         jetnNeutral_puppi6;
   Float_t         jetnCharged_puppi6;
   Float_t         jetneuEMfrac_puppi6;
   Float_t         jetneuHadfrac_puppi6;
   Float_t         jetbetaClassic_puppiStr6;
   Float_t         jetbetaClassicStar_puppiStr6;
   Float_t         jetbeta_puppiStr6;
   Float_t         jetbetaStar_puppiStr6;
   Float_t         jetconstituents_puppiStr6;
   Float_t         jetaxis2_puppiStr6;
   Int_t           jetpileupIDFlagCutBased_puppi6;
   Float_t         jetpt_puppi7;
   Float_t         jeteta_puppi7;
   Float_t         jetphi_puppi7;
   Float_t         jetmass_puppi7;
   Float_t         jetAreaX_puppi7;
   Float_t         jetAreaY_puppi7;
   Float_t         jetAreaZ_puppi7;
   Float_t         jetAreaT_puppi7;
   Float_t         jetBTagAlgo_puppi7;
   Float_t         jetBTagDefault_puppi7;
   Float_t         jetBTagPhysics_puppi7;
   Float_t         jetBTagNearest2_puppi7;
   Float_t         jetBTagNearest3_puppi7;
   Float_t         jetBTagHeaviest_puppi7;
   Float_t         jetFlavourAlgo_puppi7;
   Float_t         jetFlavourDefault_puppi7;
   Float_t         jetFlavourPhysics_puppi7;
   Float_t         jetFlavourNearest2_puppi7;
   Float_t         jetFlavourNearest3_puppi7;
   Float_t         jetFlavourHeaviest_puppi7;
   Float_t         jetptD_puppi7;
   Float_t         jetptDNe_puppi7;
   Float_t         jetptDCh_puppi7;
   Float_t         jetnNeutral_puppi7;
   Float_t         jetnCharged_puppi7;
   Float_t         jetneuEMfrac_puppi7;
   Float_t         jetneuHadfrac_puppi7;
   Float_t         jetbetaClassic_puppiStr7;
   Float_t         jetbetaClassicStar_puppiStr7;
   Float_t         jetbeta_puppiStr7;
   Float_t         jetbetaStar_puppiStr7;
   Float_t         jetconstituents_puppiStr7;
   Float_t         jetaxis2_puppiStr7;
   Int_t           jetpileupIDFlagCutBased_puppi7;
   Float_t         jetpt_puppi8;
   Float_t         jeteta_puppi8;
   Float_t         jetphi_puppi8;
   Float_t         jetmass_puppi8;
   Float_t         jetAreaX_puppi8;
   Float_t         jetAreaY_puppi8;
   Float_t         jetAreaZ_puppi8;
   Float_t         jetAreaT_puppi8;
   Float_t         jetBTagAlgo_puppi8;
   Float_t         jetBTagDefault_puppi8;
   Float_t         jetBTagPhysics_puppi8;
   Float_t         jetBTagNearest2_puppi8;
   Float_t         jetBTagNearest3_puppi8;
   Float_t         jetBTagHeaviest_puppi8;
   Float_t         jetFlavourAlgo_puppi8;
   Float_t         jetFlavourDefault_puppi8;
   Float_t         jetFlavourPhysics_puppi8;
   Float_t         jetFlavourNearest2_puppi8;
   Float_t         jetFlavourNearest3_puppi8;
   Float_t         jetFlavourHeaviest_puppi8;
   Float_t         jetptD_puppi8;
   Float_t         jetptDNe_puppi8;
   Float_t         jetptDCh_puppi8;
   Float_t         jetnNeutral_puppi8;
   Float_t         jetnCharged_puppi8;
   Float_t         jetneuEMfrac_puppi8;
   Float_t         jetneuHadfrac_puppi8;
   Float_t         jetbetaClassic_puppiStr8;
   Float_t         jetbetaClassicStar_puppiStr8;
   Float_t         jetbeta_puppiStr8;
   Float_t         jetbetaStar_puppiStr8;
   Float_t         jetconstituents_puppiStr8;
   Float_t         jetaxis2_puppiStr8;
   Int_t           jetpileupIDFlagCutBased_puppi8;
   Float_t         mll;
   Float_t         ptll;
   Float_t         dPhill;
   Float_t         dRll;
   Float_t         dEtall;
   Float_t         etall;
   Float_t         yll;
   Int_t           nextra;
   Int_t           nlepton;
   Int_t           sameflav;
   Int_t           channel;
   Float_t         pt1;
   Float_t         eta1;
   Float_t         phi1;
   Int_t           ch1;
   Int_t           pid1;
   Float_t         iso1;
   Float_t         isoDBeta1;
   Float_t         isoRhoCorr1;
   Float_t         sumChargedHadron1;
   Float_t         sumNeutral1;
   Float_t         sumChargedPU1;
   Float_t         sumAllParticles1;
   Float_t         pt2;
   Float_t         eta2;
   Float_t         phi2;
   Int_t           ch2;
   Int_t           pid2;
   Float_t         iso2;
   Float_t         isoDBeta2;
   Float_t         isoRhoCorr2;
   Float_t         sumChargedHadron2;
   Float_t         sumNeutral2;
   Float_t         sumChargedPU2;
   Float_t         sumAllParticles2;
   Float_t         pt3;
   Float_t         eta3;
   Float_t         phi3;
   Int_t           ch3;
   Int_t           pid3;
   Float_t         iso3;
   Float_t         isoDBeta3;
   Float_t         isoRhoCorr3;
   Float_t         sumChargedHadron3;
   Float_t         sumNeutral3;
   Float_t         sumChargedPU3;
   Float_t         sumAllParticles3;
   Float_t         pt4;
   Float_t         eta4;
   Float_t         phi4;
   Int_t           ch4;
   Int_t           pid4;
   Float_t         iso4;
   Float_t         isoDBeta4;
   Float_t         isoRhoCorr4;
   Float_t         sumChargedHadron4;
   Float_t         sumNeutral4;
   Float_t         sumChargedPU4;
   Float_t         sumAllParticles4;
   Float_t         pfmet;
   Float_t         pfmetphi;
   Float_t         metGenpt;
   Float_t         metGenphi;
   Float_t         pfmet_puppi;
   Float_t         pfmetphi_puppi;
   Float_t         npu;
   Float_t         globalRhokt4;
   Float_t         globalGridFastJet;
   Float_t         Rhokt4_0;
   Float_t         Rhokt4_1;
   Float_t         Rhokt4_2;
   Float_t         RhoGridFastJet_0;
   Float_t         RhoGridFastJet_1;
   Float_t         RhoGridFastJet_2;
   Float_t         PuppiRhokt4_0;
   Float_t         PuppiRhokt4_1;
   Float_t         PuppiRhoGridFastJet_0;
   Float_t         PuppiRhoGridFastJet_1;

   // List of branches
   TBranch        *b_leptonLHEpt1;   //!
   TBranch        *b_leptonLHEeta1;   //!
   TBranch        *b_leptonLHEphi1;   //!
   TBranch        *b_leptonLHEpid1;   //!
   TBranch        *b_leptonLHEspin1;   //!
   TBranch        *b_leptonLHEch1;   //!
   TBranch        *b_leptonLHEm1;   //!
   TBranch        *b_neutrinoLHEpt1;   //!
   TBranch        *b_neutrinoLHEeta1;   //!
   TBranch        *b_neutrinoLHEphi1;   //!
   TBranch        *b_neutrinoLHEpid1;   //!
   TBranch        *b_neutrinoLHEspin1;   //!
   TBranch        *b_jetLHEPartonpt1;   //!
   TBranch        *b_jetLHEPartoneta1;   //!
   TBranch        *b_jetLHEPartonphi1;   //!
   TBranch        *b_jetLHEPartonpid1;   //!
   TBranch        *b_jetLHEPartonspin1;   //!
   TBranch        *b_leptonLHEpt2;   //!
   TBranch        *b_leptonLHEeta2;   //!
   TBranch        *b_leptonLHEphi2;   //!
   TBranch        *b_leptonLHEpid2;   //!
   TBranch        *b_leptonLHEspin2;   //!
   TBranch        *b_leptonLHEch2;   //!
   TBranch        *b_leptonLHEm2;   //!
   TBranch        *b_neutrinoLHEpt2;   //!
   TBranch        *b_neutrinoLHEeta2;   //!
   TBranch        *b_neutrinoLHEphi2;   //!
   TBranch        *b_neutrinoLHEpid2;   //!
   TBranch        *b_neutrinoLHEspin2;   //!
   TBranch        *b_jetLHEPartonpt2;   //!
   TBranch        *b_jetLHEPartoneta2;   //!
   TBranch        *b_jetLHEPartonphi2;   //!
   TBranch        *b_jetLHEPartonpid2;   //!
   TBranch        *b_jetLHEPartonspin2;   //!
   TBranch        *b_leptonLHEpt3;   //!
   TBranch        *b_leptonLHEeta3;   //!
   TBranch        *b_leptonLHEphi3;   //!
   TBranch        *b_leptonLHEpid3;   //!
   TBranch        *b_leptonLHEspin3;   //!
   TBranch        *b_leptonLHEch3;   //!
   TBranch        *b_leptonLHEm3;   //!
   TBranch        *b_neutrinoLHEpt3;   //!
   TBranch        *b_neutrinoLHEeta3;   //!
   TBranch        *b_neutrinoLHEphi3;   //!
   TBranch        *b_neutrinoLHEpid3;   //!
   TBranch        *b_neutrinoLHEspin3;   //!
   TBranch        *b_jetLHEPartonpt3;   //!
   TBranch        *b_jetLHEPartoneta3;   //!
   TBranch        *b_jetLHEPartonphi3;   //!
   TBranch        *b_jetLHEPartonpid3;   //!
   TBranch        *b_jetLHEPartonspin3;   //!
   TBranch        *b_leptonLHEpt4;   //!
   TBranch        *b_leptonLHEeta4;   //!
   TBranch        *b_leptonLHEphi4;   //!
   TBranch        *b_leptonLHEpid4;   //!
   TBranch        *b_leptonLHEspin4;   //!
   TBranch        *b_leptonLHEch4;   //!
   TBranch        *b_leptonLHEm4;   //!
   TBranch        *b_neutrinoLHEpt4;   //!
   TBranch        *b_neutrinoLHEeta4;   //!
   TBranch        *b_neutrinoLHEphi4;   //!
   TBranch        *b_neutrinoLHEpid4;   //!
   TBranch        *b_neutrinoLHEspin4;   //!
   TBranch        *b_jetLHEPartonpt4;   //!
   TBranch        *b_jetLHEPartoneta4;   //!
   TBranch        *b_jetLHEPartonphi4;   //!
   TBranch        *b_jetLHEPartonpid4;   //!
   TBranch        *b_jetLHEPartonspin4;   //!
   TBranch        *b_vbosonLHEpt1;   //!
   TBranch        *b_vbosonLHEeta1;   //!
   TBranch        *b_vbosonLHEphi1;   //!
   TBranch        *b_vbosonLHEpid1;   //!
   TBranch        *b_vbosonLHEspin1;   //!
   TBranch        *b_vbosonLHEch1;   //!
   TBranch        *b_vbosonLHEm1;   //!
   TBranch        *b_jetLHEGluonpt1;   //!
   TBranch        *b_jetLHEGluoneta1;   //!
   TBranch        *b_jetLHEGluonphi1;   //!
   TBranch        *b_jetLHEGluonpid1;   //!
   TBranch        *b_jetLHEGluonspin1;   //!
   TBranch        *b_vbosonLHEpt2;   //!
   TBranch        *b_vbosonLHEeta2;   //!
   TBranch        *b_vbosonLHEphi2;   //!
   TBranch        *b_vbosonLHEpid2;   //!
   TBranch        *b_vbosonLHEspin2;   //!
   TBranch        *b_vbosonLHEch2;   //!
   TBranch        *b_vbosonLHEm2;   //!
   TBranch        *b_jetLHEGluonpt2;   //!
   TBranch        *b_jetLHEGluoneta2;   //!
   TBranch        *b_jetLHEGluonphi2;   //!
   TBranch        *b_jetLHEGluonpid2;   //!
   TBranch        *b_jetLHEGluonspin2;   //!
   TBranch        *b_jetGenpt1;   //!
   TBranch        *b_jetGeneta1;   //!
   TBranch        *b_jetGenphi1;   //!
   TBranch        *b_jetGenm1;   //!
   TBranch        *b_jetGenAreaX1;   //!
   TBranch        *b_jetGenAreaY1;   //!
   TBranch        *b_jetGenAreaZ1;   //!
   TBranch        *b_jetGenAreaT1;   //!
   TBranch        *b_jetGenpt2;   //!
   TBranch        *b_jetGeneta2;   //!
   TBranch        *b_jetGenphi2;   //!
   TBranch        *b_jetGenm2;   //!
   TBranch        *b_jetGenAreaX2;   //!
   TBranch        *b_jetGenAreaY2;   //!
   TBranch        *b_jetGenAreaZ2;   //!
   TBranch        *b_jetGenAreaT2;   //!
   TBranch        *b_jetGenpt3;   //!
   TBranch        *b_jetGeneta3;   //!
   TBranch        *b_jetGenphi3;   //!
   TBranch        *b_jetGenm3;   //!
   TBranch        *b_jetGenAreaX3;   //!
   TBranch        *b_jetGenAreaY3;   //!
   TBranch        *b_jetGenAreaZ3;   //!
   TBranch        *b_jetGenAreaT3;   //!
   TBranch        *b_jetGenpt4;   //!
   TBranch        *b_jetGeneta4;   //!
   TBranch        *b_jetGenphi4;   //!
   TBranch        *b_jetGenm4;   //!
   TBranch        *b_jetGenAreaX4;   //!
   TBranch        *b_jetGenAreaY4;   //!
   TBranch        *b_jetGenAreaZ4;   //!
   TBranch        *b_jetGenAreaT4;   //!
   TBranch        *b_TrackJet_V4_;   //!
   TBranch        *b_TrackJet_V4_fUniqueID;   //!
   TBranch        *b_TrackJet_V4_fBits;   //!
   TBranch        *b_TrackJet_V4_fP_fUniqueID;   //!
   TBranch        *b_TrackJet_V4_fP_fBits;   //!
   TBranch        *b_TrackJet_V4_fP_fX;   //!
   TBranch        *b_TrackJet_V4_fP_fY;   //!
   TBranch        *b_TrackJet_V4_fP_fZ;   //!
   TBranch        *b_TrackJet_V4_fE;   //!
   TBranch        *b_HtSoft;   //!
   TBranch        *b_nSoftJets;   //!
   TBranch        *b_mjj;   //!
   TBranch        *b_detajj;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_nbjet;   //!
   TBranch        *b_hardbjpb;   //!
   TBranch        *b_softbjpb;   //!
   TBranch        *b_njetid;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jetphi1;   //!
   TBranch        *b_jetmass1;   //!
   TBranch        *b_jetAreaX1;   //!
   TBranch        *b_jetAreaY1;   //!
   TBranch        *b_jetAreaZ1;   //!
   TBranch        *b_jetAreaT1;   //!
   TBranch        *b_jetBTagAlgo1;   //!
   TBranch        *b_jetBTagDefault1;   //!
   TBranch        *b_jetBTagPhysics1;   //!
   TBranch        *b_jetBTagNearest2_1;   //!
   TBranch        *b_jetBTagNearest3_1;   //!
   TBranch        *b_jetBTagHeaviest_1;   //!
   TBranch        *b_jetFlavourAlgo1;   //!
   TBranch        *b_jetFlavourDefault1;   //!
   TBranch        *b_jetFlavourPhysics1;   //!
   TBranch        *b_jetFlavourNearest2_1;   //!
   TBranch        *b_jetFlavourNearest3_1;   //!
   TBranch        *b_jetFlavourHeaviest_1;   //!
   TBranch        *b_jetptD1;   //!
   TBranch        *b_jetptDNe1;   //!
   TBranch        *b_jetptDCh1;   //!
   TBranch        *b_jetnNeutral1;   //!
   TBranch        *b_jetnCharged1;   //!
   TBranch        *b_jetneuEMfrac1;   //!
   TBranch        *b_jetneuHadfrac1;   //!
   TBranch        *b_jetbetaClassic1;   //!
   TBranch        *b_jetbetaClassicStar1;   //!
   TBranch        *b_jetbeta1;   //!
   TBranch        *b_jetbetaStar1;   //!
   TBranch        *b_jetconstituents1;   //!
   TBranch        *b_jetaxis2_1;   //!
   TBranch        *b_jetpileupIDFlagCutBased1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_jetphi2;   //!
   TBranch        *b_jetmass2;   //!
   TBranch        *b_jetAreaX2;   //!
   TBranch        *b_jetAreaY2;   //!
   TBranch        *b_jetAreaZ2;   //!
   TBranch        *b_jetAreaT2;   //!
   TBranch        *b_jetBTagAlgo2;   //!
   TBranch        *b_jetBTagDefault2;   //!
   TBranch        *b_jetBTagPhysics2;   //!
   TBranch        *b_jetBTagNearest2_2;   //!
   TBranch        *b_jetBTagNearest3_2;   //!
   TBranch        *b_jetBTagHeaviest_2;   //!
   TBranch        *b_jetFlavourAlgo2;   //!
   TBranch        *b_jetFlavourDefault2;   //!
   TBranch        *b_jetFlavourPhysics2;   //!
   TBranch        *b_jetFlavourNearest2_2;   //!
   TBranch        *b_jetFlavourNearest3_2;   //!
   TBranch        *b_jetFlavourHeaviest_2;   //!
   TBranch        *b_jetptD2;   //!
   TBranch        *b_jetptDNe2;   //!
   TBranch        *b_jetptDCh2;   //!
   TBranch        *b_jetnNeutral2;   //!
   TBranch        *b_jetnCharged2;   //!
   TBranch        *b_jetneuEMfrac2;   //!
   TBranch        *b_jetneuHadfrac2;   //!
   TBranch        *b_jetbetaClassic2;   //!
   TBranch        *b_jetbetaClassicStar2;   //!
   TBranch        *b_jetbeta2;   //!
   TBranch        *b_jetbetaStar2;   //!
   TBranch        *b_jetconstituents2;   //!
   TBranch        *b_jetaxis2_2;   //!
   TBranch        *b_jetpileupIDFlagCutBased2;   //!
   TBranch        *b_jetpt3;   //!
   TBranch        *b_jeteta3;   //!
   TBranch        *b_jetphi3;   //!
   TBranch        *b_jetmass3;   //!
   TBranch        *b_jetAreaX3;   //!
   TBranch        *b_jetAreaY3;   //!
   TBranch        *b_jetAreaZ3;   //!
   TBranch        *b_jetAreaT3;   //!
   TBranch        *b_jetBTagAlgo3;   //!
   TBranch        *b_jetBTagDefault3;   //!
   TBranch        *b_jetBTagPhysics3;   //!
   TBranch        *b_jetBTagNearest2_3;   //!
   TBranch        *b_jetBTagNearest3_3;   //!
   TBranch        *b_jetBTagHeaviest_3;   //!
   TBranch        *b_jetFlavourAlgo3;   //!
   TBranch        *b_jetFlavourDefault3;   //!
   TBranch        *b_jetFlavourPhysics3;   //!
   TBranch        *b_jetFlavourNearest2_3;   //!
   TBranch        *b_jetFlavourNearest3_3;   //!
   TBranch        *b_jetFlavourHeaviest_3;   //!
   TBranch        *b_jetptD3;   //!
   TBranch        *b_jetptDNe3;   //!
   TBranch        *b_jetptDCh3;   //!
   TBranch        *b_jetnNeutral3;   //!
   TBranch        *b_jetnCharged3;   //!
   TBranch        *b_jetneuEMfrac3;   //!
   TBranch        *b_jetneuHadfrac3;   //!
   TBranch        *b_jetbetaClassic3;   //!
   TBranch        *b_jetbetaClassicStar3;   //!
   TBranch        *b_jetbeta3;   //!
   TBranch        *b_jetbetaStar3;   //!
   TBranch        *b_jetconstituents3;   //!
   TBranch        *b_jetaxis2_3;   //!
   TBranch        *b_jetpileupIDFlagCutBased3;   //!
   TBranch        *b_jetpt4;   //!
   TBranch        *b_jeteta4;   //!
   TBranch        *b_jetphi4;   //!
   TBranch        *b_jetmass4;   //!
   TBranch        *b_jetAreaX4;   //!
   TBranch        *b_jetAreaY4;   //!
   TBranch        *b_jetAreaZ4;   //!
   TBranch        *b_jetAreaT4;   //!
   TBranch        *b_jetBTagAlgo4;   //!
   TBranch        *b_jetBTagDefault4;   //!
   TBranch        *b_jetBTagPhysics4;   //!
   TBranch        *b_jetBTagNearest2_4;   //!
   TBranch        *b_jetBTagNearest3_4;   //!
   TBranch        *b_jetBTagHeaviest_4;   //!
   TBranch        *b_jetFlavourAlgo4;   //!
   TBranch        *b_jetFlavourDefault4;   //!
   TBranch        *b_jetFlavourPhysics4;   //!
   TBranch        *b_jetFlavourNearest2_4;   //!
   TBranch        *b_jetFlavourNearest3_4;   //!
   TBranch        *b_jetFlavourHeaviest_4;   //!
   TBranch        *b_jetptD4;   //!
   TBranch        *b_jetptDNe4;   //!
   TBranch        *b_jetptDCh4;   //!
   TBranch        *b_jetnNeutral4;   //!
   TBranch        *b_jetnCharged4;   //!
   TBranch        *b_jetneuEMfrac4;   //!
   TBranch        *b_jetneuHadfrac4;   //!
   TBranch        *b_jetbetaClassic4;   //!
   TBranch        *b_jetbetaClassicStar4;   //!
   TBranch        *b_jetbeta4;   //!
   TBranch        *b_jetbetaStar4;   //!
   TBranch        *b_jetconstituents4;   //!
   TBranch        *b_jetaxis2_4;   //!
   TBranch        *b_jetpileupIDFlagCutBased4;   //!
   TBranch        *b_jetpt5;   //!
   TBranch        *b_jeteta5;   //!
   TBranch        *b_jetphi5;   //!
   TBranch        *b_jetmass5;   //!
   TBranch        *b_jetAreaX5;   //!
   TBranch        *b_jetAreaY5;   //!
   TBranch        *b_jetAreaZ5;   //!
   TBranch        *b_jetAreaT5;   //!
   TBranch        *b_jetBTagAlgo5;   //!
   TBranch        *b_jetBTagDefault5;   //!
   TBranch        *b_jetBTagPhysics5;   //!
   TBranch        *b_jetBTagNearest2_5;   //!
   TBranch        *b_jetBTagNearest3_5;   //!
   TBranch        *b_jetBTagHeaviest_5;   //!
   TBranch        *b_jetFlavourAlgo5;   //!
   TBranch        *b_jetFlavourDefault5;   //!
   TBranch        *b_jetFlavourPhysics5;   //!
   TBranch        *b_jetFlavourNearest2_5;   //!
   TBranch        *b_jetFlavourNearest3_5;   //!
   TBranch        *b_jetFlavourHeaviest_5;   //!
   TBranch        *b_jetptD5;   //!
   TBranch        *b_jetptDNe5;   //!
   TBranch        *b_jetptDCh5;   //!
   TBranch        *b_jetnNeutral5;   //!
   TBranch        *b_jetnCharged5;   //!
   TBranch        *b_jetneuEMfrac5;   //!
   TBranch        *b_jetneuHadfrac5;   //!
   TBranch        *b_jetbetaClassic5;   //!
   TBranch        *b_jetbetaClassicStar5;   //!
   TBranch        *b_jetbeta5;   //!
   TBranch        *b_jetbetaStar5;   //!
   TBranch        *b_jetconstituents5;   //!
   TBranch        *b_jetaxis2_5;   //!
   TBranch        *b_jetpileupIDFlagCutBased5;   //!
   TBranch        *b_jetpt6;   //!
   TBranch        *b_jeteta6;   //!
   TBranch        *b_jetphi6;   //!
   TBranch        *b_jetmass6;   //!
   TBranch        *b_jetAreaX6;   //!
   TBranch        *b_jetAreaY6;   //!
   TBranch        *b_jetAreaZ6;   //!
   TBranch        *b_jetAreaT6;   //!
   TBranch        *b_jetBTagAlgo6;   //!
   TBranch        *b_jetBTagDefault6;   //!
   TBranch        *b_jetBTagPhysics6;   //!
   TBranch        *b_jetBTagNearest2_6;   //!
   TBranch        *b_jetBTagNearest3_6;   //!
   TBranch        *b_jetBTagHeaviest_6;   //!
   TBranch        *b_jetFlavourAlgo6;   //!
   TBranch        *b_jetFlavourDefault6;   //!
   TBranch        *b_jetFlavourPhysics6;   //!
   TBranch        *b_jetFlavourNearest2_6;   //!
   TBranch        *b_jetFlavourNearest3_6;   //!
   TBranch        *b_jetFlavourHeaviest_6;   //!
   TBranch        *b_jetptD6;   //!
   TBranch        *b_jetptDNe6;   //!
   TBranch        *b_jetptDCh6;   //!
   TBranch        *b_jetnNeutral6;   //!
   TBranch        *b_jetnCharged6;   //!
   TBranch        *b_jetneuEMfrac6;   //!
   TBranch        *b_jetneuHadfrac6;   //!
   TBranch        *b_jetbetaClassic6;   //!
   TBranch        *b_jetbetaClassicStar6;   //!
   TBranch        *b_jetbeta6;   //!
   TBranch        *b_jetbetaStar6;   //!
   TBranch        *b_jetconstituents6;   //!
   TBranch        *b_jetaxis2_6;   //!
   TBranch        *b_jetpileupIDFlagCutBased6;   //!
   TBranch        *b_jetpt7;   //!
   TBranch        *b_jeteta7;   //!
   TBranch        *b_jetphi7;   //!
   TBranch        *b_jetmass7;   //!
   TBranch        *b_jetAreaX7;   //!
   TBranch        *b_jetAreaY7;   //!
   TBranch        *b_jetAreaZ7;   //!
   TBranch        *b_jetAreaT7;   //!
   TBranch        *b_jetBTagAlgo7;   //!
   TBranch        *b_jetBTagDefault7;   //!
   TBranch        *b_jetBTagPhysics7;   //!
   TBranch        *b_jetBTagNearest2_7;   //!
   TBranch        *b_jetBTagNearest3_7;   //!
   TBranch        *b_jetBTagHeaviest_7;   //!
   TBranch        *b_jetFlavourAlgo7;   //!
   TBranch        *b_jetFlavourDefault7;   //!
   TBranch        *b_jetFlavourPhysics7;   //!
   TBranch        *b_jetFlavourNearest2_7;   //!
   TBranch        *b_jetFlavourNearest3_7;   //!
   TBranch        *b_jetFlavourHeaviest_7;   //!
   TBranch        *b_jetptD7;   //!
   TBranch        *b_jetptDNe7;   //!
   TBranch        *b_jetptDCh7;   //!
   TBranch        *b_jetnNeutral7;   //!
   TBranch        *b_jetnCharged7;   //!
   TBranch        *b_jetneuEMfrac7;   //!
   TBranch        *b_jetneuHadfrac7;   //!
   TBranch        *b_jetbetaClassic7;   //!
   TBranch        *b_jetbetaClassicStar7;   //!
   TBranch        *b_jetbeta7;   //!
   TBranch        *b_jetbetaStar7;   //!
   TBranch        *b_jetconstituents7;   //!
   TBranch        *b_jetaxis2_7;   //!
   TBranch        *b_jetpileupIDFlagCutBased7;   //!
   TBranch        *b_jetpt8;   //!
   TBranch        *b_jeteta8;   //!
   TBranch        *b_jetphi8;   //!
   TBranch        *b_jetmass8;   //!
   TBranch        *b_jetAreaX8;   //!
   TBranch        *b_jetAreaY8;   //!
   TBranch        *b_jetAreaZ8;   //!
   TBranch        *b_jetAreaT8;   //!
   TBranch        *b_jetBTagAlgo8;   //!
   TBranch        *b_jetBTagDefault8;   //!
   TBranch        *b_jetBTagPhysics8;   //!
   TBranch        *b_jetBTagNearest2_8;   //!
   TBranch        *b_jetBTagNearest3_8;   //!
   TBranch        *b_jetBTagHeaviest_8;   //!
   TBranch        *b_jetFlavourAlgo8;   //!
   TBranch        *b_jetFlavourDefault8;   //!
   TBranch        *b_jetFlavourPhysics8;   //!
   TBranch        *b_jetFlavourNearest2_8;   //!
   TBranch        *b_jetFlavourNearest3_8;   //!
   TBranch        *b_jetFlavourHeaviest_8;   //!
   TBranch        *b_jetptD8;   //!
   TBranch        *b_jetptDNe8;   //!
   TBranch        *b_jetptDCh8;   //!
   TBranch        *b_jetnNeutral8;   //!
   TBranch        *b_jetnCharged8;   //!
   TBranch        *b_jetneuEMfrac8;   //!
   TBranch        *b_jetneuHadfrac8;   //!
   TBranch        *b_jetbetaClassic8;   //!
   TBranch        *b_jetbetaClassicStar8;   //!
   TBranch        *b_jetbeta8;   //!
   TBranch        *b_jetbetaStar8;   //!
   TBranch        *b_jetconstituents8;   //!
   TBranch        *b_jetaxis2_8;   //!
   TBranch        *b_jetpileupIDFlagCutBased8;   //!
   TBranch        *b_mjj_puppi;   //!
   TBranch        *b_detajj_puppi;   //!
   TBranch        *b_njet_puppi;   //!
   TBranch        *b_nbjet_puppi;   //!
   TBranch        *b_hardbjpb_puppi;   //!
   TBranch        *b_softbjpb_puppi;   //!
   TBranch        *b_njetid_puppi;   //!
   TBranch        *b_jetpt_puppi1;   //!
   TBranch        *b_jeteta_puppi1;   //!
   TBranch        *b_jetphi_puppi1;   //!
   TBranch        *b_jetmass_puppi1;   //!
   TBranch        *b_jetAreaX_puppi1;   //!
   TBranch        *b_jetAreaY_puppi1;   //!
   TBranch        *b_jetAreaZ_puppi1;   //!
   TBranch        *b_jetAreaT_puppi1;   //!
   TBranch        *b_jetBTagAlgo_puppi1;   //!
   TBranch        *b_jetBTagDefault_puppi1;   //!
   TBranch        *b_jetBTagPhysics_puppi1;   //!
   TBranch        *b_jetBTagNearest2_puppi1;   //!
   TBranch        *b_jetBTagNearest3_puppi1;   //!
   TBranch        *b_jetBTagHeaviest_puppi1;   //!
   TBranch        *b_jetFlavourAlgo_puppi1;   //!
   TBranch        *b_jetFlavourDefault_puppi1;   //!
   TBranch        *b_jetFlavourPhysics_puppi1;   //!
   TBranch        *b_jetFlavourNearest2_puppi1;   //!
   TBranch        *b_jetFlavourNearest3_puppi1;   //!
   TBranch        *b_jetFlavourHeaviest_puppi1;   //!
   TBranch        *b_jetptD_puppi1;   //!
   TBranch        *b_jetptDNe_puppi1;   //!
   TBranch        *b_jetptDCh_puppi1;   //!
   TBranch        *b_jetnNeutral_puppi1;   //!
   TBranch        *b_jetnCharged_puppi1;   //!
   TBranch        *b_jetneuEMfrac_puppi1;   //!
   TBranch        *b_jetneuHadfrac_puppi1;   //!
   TBranch        *b_jetbetaClassic_puppiStr1;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr1;   //!
   TBranch        *b_jetbeta_puppiStr1;   //!
   TBranch        *b_jetbetaStar_puppiStr1;   //!
   TBranch        *b_jetconstituents_puppiStr1;   //!
   TBranch        *b_jetaxis2_puppiStr1;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi1;   //!
   TBranch        *b_jetpt_puppi2;   //!
   TBranch        *b_jeteta_puppi2;   //!
   TBranch        *b_jetphi_puppi2;   //!
   TBranch        *b_jetmass_puppi2;   //!
   TBranch        *b_jetAreaX_puppi2;   //!
   TBranch        *b_jetAreaY_puppi2;   //!
   TBranch        *b_jetAreaZ_puppi2;   //!
   TBranch        *b_jetAreaT_puppi2;   //!
   TBranch        *b_jetBTagAlgo_puppi2;   //!
   TBranch        *b_jetBTagDefault_puppi2;   //!
   TBranch        *b_jetBTagPhysics_puppi2;   //!
   TBranch        *b_jetBTagNearest2_puppi2;   //!
   TBranch        *b_jetBTagNearest3_puppi2;   //!
   TBranch        *b_jetBTagHeaviest_puppi2;   //!
   TBranch        *b_jetFlavourAlgo_puppi2;   //!
   TBranch        *b_jetFlavourDefault_puppi2;   //!
   TBranch        *b_jetFlavourPhysics_puppi2;   //!
   TBranch        *b_jetFlavourNearest2_puppi2;   //!
   TBranch        *b_jetFlavourNearest3_puppi2;   //!
   TBranch        *b_jetFlavourHeaviest_puppi2;   //!
   TBranch        *b_jetptD_puppi2;   //!
   TBranch        *b_jetptDNe_puppi2;   //!
   TBranch        *b_jetptDCh_puppi2;   //!
   TBranch        *b_jetnNeutral_puppi2;   //!
   TBranch        *b_jetnCharged_puppi2;   //!
   TBranch        *b_jetneuEMfrac_puppi2;   //!
   TBranch        *b_jetneuHadfrac_puppi2;   //!
   TBranch        *b_jetbetaClassic_puppiStr2;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr2;   //!
   TBranch        *b_jetbeta_puppiStr2;   //!
   TBranch        *b_jetbetaStar_puppiStr2;   //!
   TBranch        *b_jetconstituents_puppiStr2;   //!
   TBranch        *b_jetaxis2_puppiStr2;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi2;   //!
   TBranch        *b_jetpt_puppi3;   //!
   TBranch        *b_jeteta_puppi3;   //!
   TBranch        *b_jetphi_puppi3;   //!
   TBranch        *b_jetmass_puppi3;   //!
   TBranch        *b_jetAreaX_puppi3;   //!
   TBranch        *b_jetAreaY_puppi3;   //!
   TBranch        *b_jetAreaZ_puppi3;   //!
   TBranch        *b_jetAreaT_puppi3;   //!
   TBranch        *b_jetBTagAlgo_puppi3;   //!
   TBranch        *b_jetBTagDefault_puppi3;   //!
   TBranch        *b_jetBTagPhysics_puppi3;   //!
   TBranch        *b_jetBTagNearest2_puppi3;   //!
   TBranch        *b_jetBTagNearest3_puppi3;   //!
   TBranch        *b_jetBTagHeaviest_puppi3;   //!
   TBranch        *b_jetFlavourAlgo_puppi3;   //!
   TBranch        *b_jetFlavourDefault_puppi3;   //!
   TBranch        *b_jetFlavourPhysics_puppi3;   //!
   TBranch        *b_jetFlavourNearest2_puppi3;   //!
   TBranch        *b_jetFlavourNearest3_puppi3;   //!
   TBranch        *b_jetFlavourHeaviest_puppi3;   //!
   TBranch        *b_jetptD_puppi3;   //!
   TBranch        *b_jetptDNe_puppi3;   //!
   TBranch        *b_jetptDCh_puppi3;   //!
   TBranch        *b_jetnNeutral_puppi3;   //!
   TBranch        *b_jetnCharged_puppi3;   //!
   TBranch        *b_jetneuEMfrac_puppi3;   //!
   TBranch        *b_jetneuHadfrac_puppi3;   //!
   TBranch        *b_jetbetaClassic_puppiStr3;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr3;   //!
   TBranch        *b_jetbeta_puppiStr3;   //!
   TBranch        *b_jetbetaStar_puppiStr3;   //!
   TBranch        *b_jetconstituents_puppiStr3;   //!
   TBranch        *b_jetaxis2_puppiStr3;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi3;   //!
   TBranch        *b_jetpt_puppi4;   //!
   TBranch        *b_jeteta_puppi4;   //!
   TBranch        *b_jetphi_puppi4;   //!
   TBranch        *b_jetmass_puppi4;   //!
   TBranch        *b_jetAreaX_puppi4;   //!
   TBranch        *b_jetAreaY_puppi4;   //!
   TBranch        *b_jetAreaZ_puppi4;   //!
   TBranch        *b_jetAreaT_puppi4;   //!
   TBranch        *b_jetBTagAlgo_puppi4;   //!
   TBranch        *b_jetBTagDefault_puppi4;   //!
   TBranch        *b_jetBTagPhysics_puppi4;   //!
   TBranch        *b_jetBTagNearest2_puppi4;   //!
   TBranch        *b_jetBTagNearest3_puppi4;   //!
   TBranch        *b_jetBTagHeaviest_puppi4;   //!
   TBranch        *b_jetFlavourAlgo_puppi4;   //!
   TBranch        *b_jetFlavourDefault_puppi4;   //!
   TBranch        *b_jetFlavourPhysics_puppi4;   //!
   TBranch        *b_jetFlavourNearest2_puppi4;   //!
   TBranch        *b_jetFlavourNearest3_puppi4;   //!
   TBranch        *b_jetFlavourHeaviest_puppi4;   //!
   TBranch        *b_jetptD_puppi4;   //!
   TBranch        *b_jetptDNe_puppi4;   //!
   TBranch        *b_jetptDCh_puppi4;   //!
   TBranch        *b_jetnNeutral_puppi4;   //!
   TBranch        *b_jetnCharged_puppi4;   //!
   TBranch        *b_jetneuEMfrac_puppi4;   //!
   TBranch        *b_jetneuHadfrac_puppi4;   //!
   TBranch        *b_jetbetaClassic_puppiStr4;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr4;   //!
   TBranch        *b_jetbeta_puppiStr4;   //!
   TBranch        *b_jetbetaStar_puppiStr4;   //!
   TBranch        *b_jetconstituents_puppiStr4;   //!
   TBranch        *b_jetaxis2_puppiStr4;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi4;   //!
   TBranch        *b_jetpt_puppi5;   //!
   TBranch        *b_jeteta_puppi5;   //!
   TBranch        *b_jetphi_puppi5;   //!
   TBranch        *b_jetmass_puppi5;   //!
   TBranch        *b_jetAreaX_puppi5;   //!
   TBranch        *b_jetAreaY_puppi5;   //!
   TBranch        *b_jetAreaZ_puppi5;   //!
   TBranch        *b_jetAreaT_puppi5;   //!
   TBranch        *b_jetBTagAlgo_puppi5;   //!
   TBranch        *b_jetBTagDefault_puppi5;   //!
   TBranch        *b_jetBTagPhysics_puppi5;   //!
   TBranch        *b_jetBTagNearest2_puppi5;   //!
   TBranch        *b_jetBTagNearest3_puppi5;   //!
   TBranch        *b_jetBTagHeaviest_puppi5;   //!
   TBranch        *b_jetFlavourAlgo_puppi5;   //!
   TBranch        *b_jetFlavourDefault_puppi5;   //!
   TBranch        *b_jetFlavourPhysics_puppi5;   //!
   TBranch        *b_jetFlavourNearest2_puppi5;   //!
   TBranch        *b_jetFlavourNearest3_puppi5;   //!
   TBranch        *b_jetFlavourHeaviest_puppi5;   //!
   TBranch        *b_jetptD_puppi5;   //!
   TBranch        *b_jetptDNe_puppi5;   //!
   TBranch        *b_jetptDCh_puppi5;   //!
   TBranch        *b_jetnNeutral_puppi5;   //!
   TBranch        *b_jetnCharged_puppi5;   //!
   TBranch        *b_jetneuEMfrac_puppi5;   //!
   TBranch        *b_jetneuHadfrac_puppi5;   //!
   TBranch        *b_jetbetaClassic_puppiStr5;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr5;   //!
   TBranch        *b_jetbeta_puppiStr5;   //!
   TBranch        *b_jetbetaStar_puppiStr5;   //!
   TBranch        *b_jetconstituents_puppiStr5;   //!
   TBranch        *b_jetaxis2_puppiStr5;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi5;   //!
   TBranch        *b_jetpt_puppi6;   //!
   TBranch        *b_jeteta_puppi6;   //!
   TBranch        *b_jetphi_puppi6;   //!
   TBranch        *b_jetmass_puppi6;   //!
   TBranch        *b_jetAreaX_puppi6;   //!
   TBranch        *b_jetAreaY_puppi6;   //!
   TBranch        *b_jetAreaZ_puppi6;   //!
   TBranch        *b_jetAreaT_puppi6;   //!
   TBranch        *b_jetBTagAlgo_puppi6;   //!
   TBranch        *b_jetBTagDefault_puppi6;   //!
   TBranch        *b_jetBTagPhysics_puppi6;   //!
   TBranch        *b_jetBTagNearest2_puppi6;   //!
   TBranch        *b_jetBTagNearest3_puppi6;   //!
   TBranch        *b_jetBTagHeaviest_puppi6;   //!
   TBranch        *b_jetFlavourAlgo_puppi6;   //!
   TBranch        *b_jetFlavourDefault_puppi6;   //!
   TBranch        *b_jetFlavourPhysics_puppi6;   //!
   TBranch        *b_jetFlavourNearest2_puppi6;   //!
   TBranch        *b_jetFlavourNearest3_puppi6;   //!
   TBranch        *b_jetFlavourHeaviest_puppi6;   //!
   TBranch        *b_jetptD_puppi6;   //!
   TBranch        *b_jetptDNe_puppi6;   //!
   TBranch        *b_jetptDCh_puppi6;   //!
   TBranch        *b_jetnNeutral_puppi6;   //!
   TBranch        *b_jetnCharged_puppi6;   //!
   TBranch        *b_jetneuEMfrac_puppi6;   //!
   TBranch        *b_jetneuHadfrac_puppi6;   //!
   TBranch        *b_jetbetaClassic_puppiStr6;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr6;   //!
   TBranch        *b_jetbeta_puppiStr6;   //!
   TBranch        *b_jetbetaStar_puppiStr6;   //!
   TBranch        *b_jetconstituents_puppiStr6;   //!
   TBranch        *b_jetaxis2_puppiStr6;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi6;   //!
   TBranch        *b_jetpt_puppi7;   //!
   TBranch        *b_jeteta_puppi7;   //!
   TBranch        *b_jetphi_puppi7;   //!
   TBranch        *b_jetmass_puppi7;   //!
   TBranch        *b_jetAreaX_puppi7;   //!
   TBranch        *b_jetAreaY_puppi7;   //!
   TBranch        *b_jetAreaZ_puppi7;   //!
   TBranch        *b_jetAreaT_puppi7;   //!
   TBranch        *b_jetBTagAlgo_puppi7;   //!
   TBranch        *b_jetBTagDefault_puppi7;   //!
   TBranch        *b_jetBTagPhysics_puppi7;   //!
   TBranch        *b_jetBTagNearest2_puppi7;   //!
   TBranch        *b_jetBTagNearest3_puppi7;   //!
   TBranch        *b_jetBTagHeaviest_puppi7;   //!
   TBranch        *b_jetFlavourAlgo_puppi7;   //!
   TBranch        *b_jetFlavourDefault_puppi7;   //!
   TBranch        *b_jetFlavourPhysics_puppi7;   //!
   TBranch        *b_jetFlavourNearest2_puppi7;   //!
   TBranch        *b_jetFlavourNearest3_puppi7;   //!
   TBranch        *b_jetFlavourHeaviest_puppi7;   //!
   TBranch        *b_jetptD_puppi7;   //!
   TBranch        *b_jetptDNe_puppi7;   //!
   TBranch        *b_jetptDCh_puppi7;   //!
   TBranch        *b_jetnNeutral_puppi7;   //!
   TBranch        *b_jetnCharged_puppi7;   //!
   TBranch        *b_jetneuEMfrac_puppi7;   //!
   TBranch        *b_jetneuHadfrac_puppi7;   //!
   TBranch        *b_jetbetaClassic_puppiStr7;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr7;   //!
   TBranch        *b_jetbeta_puppiStr7;   //!
   TBranch        *b_jetbetaStar_puppiStr7;   //!
   TBranch        *b_jetconstituents_puppiStr7;   //!
   TBranch        *b_jetaxis2_puppiStr7;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi7;   //!
   TBranch        *b_jetpt_puppi8;   //!
   TBranch        *b_jeteta_puppi8;   //!
   TBranch        *b_jetphi_puppi8;   //!
   TBranch        *b_jetmass_puppi8;   //!
   TBranch        *b_jetAreaX_puppi8;   //!
   TBranch        *b_jetAreaY_puppi8;   //!
   TBranch        *b_jetAreaZ_puppi8;   //!
   TBranch        *b_jetAreaT_puppi8;   //!
   TBranch        *b_jetBTagAlgo_puppi8;   //!
   TBranch        *b_jetBTagDefault_puppi8;   //!
   TBranch        *b_jetBTagPhysics_puppi8;   //!
   TBranch        *b_jetBTagNearest2_puppi8;   //!
   TBranch        *b_jetBTagNearest3_puppi8;   //!
   TBranch        *b_jetBTagHeaviest_puppi8;   //!
   TBranch        *b_jetFlavourAlgo_puppi8;   //!
   TBranch        *b_jetFlavourDefault_puppi8;   //!
   TBranch        *b_jetFlavourPhysics_puppi8;   //!
   TBranch        *b_jetFlavourNearest2_puppi8;   //!
   TBranch        *b_jetFlavourNearest3_puppi8;   //!
   TBranch        *b_jetFlavourHeaviest_puppi8;   //!
   TBranch        *b_jetptD_puppi8;   //!
   TBranch        *b_jetptDNe_puppi8;   //!
   TBranch        *b_jetptDCh_puppi8;   //!
   TBranch        *b_jetnNeutral_puppi8;   //!
   TBranch        *b_jetnCharged_puppi8;   //!
   TBranch        *b_jetneuEMfrac_puppi8;   //!
   TBranch        *b_jetneuHadfrac_puppi8;   //!
   TBranch        *b_jetbetaClassic_puppiStr8;   //!
   TBranch        *b_jetbetaClassicStar_puppiStr8;   //!
   TBranch        *b_jetbeta_puppiStr8;   //!
   TBranch        *b_jetbetaStar_puppiStr8;   //!
   TBranch        *b_jetconstituents_puppiStr8;   //!
   TBranch        *b_jetaxis2_puppiStr8;   //!
   TBranch        *b_jetpileupIDFlagCutBased_puppi8;   //!
   TBranch        *b_mll;   //!
   TBranch        *b_ptll;   //!
   TBranch        *b_dPhill;   //!
   TBranch        *b_dRll;   //!
   TBranch        *b_dEtall;   //!
   TBranch        *b_etall;   //!
   TBranch        *b_yll;   //!
   TBranch        *b_nextra;   //!
   TBranch        *b_nlepton;   //!
   TBranch        *b_sameflav;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_ch1;   //!
   TBranch        *b_pid1;   //!
   TBranch        *b_iso1;   //!
   TBranch        *b_isoDBeta1;   //!
   TBranch        *b_isoRhoCorr1;   //!
   TBranch        *b_sumChargedHadron1;   //!
   TBranch        *b_sumNeutral1;   //!
   TBranch        *b_sumChargedPU1;   //!
   TBranch        *b_sumAllParticles1;   //!
   TBranch        *b_pt2;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_ch2;   //!
   TBranch        *b_pid2;   //!
   TBranch        *b_iso2;   //!
   TBranch        *b_isoDBeta2;   //!
   TBranch        *b_isoRhoCorr2;   //!
   TBranch        *b_sumChargedHadron2;   //!
   TBranch        *b_sumNeutral2;   //!
   TBranch        *b_sumChargedPU2;   //!
   TBranch        *b_sumAllParticles2;   //!
   TBranch        *b_pt3;   //!
   TBranch        *b_eta3;   //!
   TBranch        *b_phi3;   //!
   TBranch        *b_ch3;   //!
   TBranch        *b_pid3;   //!
   TBranch        *b_iso3;   //!
   TBranch        *b_isoDBeta3;   //!
   TBranch        *b_isoRhoCorr3;   //!
   TBranch        *b_sumChargedHadron3;   //!
   TBranch        *b_sumNeutral3;   //!
   TBranch        *b_sumChargedPU3;   //!
   TBranch        *b_sumAllParticles3;   //!
   TBranch        *b_pt4;   //!
   TBranch        *b_eta4;   //!
   TBranch        *b_phi4;   //!
   TBranch        *b_ch4;   //!
   TBranch        *b_pid4;   //!
   TBranch        *b_iso4;   //!
   TBranch        *b_isoDBeta4;   //!
   TBranch        *b_isoRhoCorr4;   //!
   TBranch        *b_sumChargedHadron4;   //!
   TBranch        *b_sumNeutral4;   //!
   TBranch        *b_sumChargedPU4;   //!
   TBranch        *b_sumAllParticles4;   //!
   TBranch        *b_pfmet;   //!
   TBranch        *b_pfmetphi;   //!
   TBranch        *b_metGenpt;   //!
   TBranch        *b_metGenphi;   //!
   TBranch        *b_pfmet_puppi;   //!
   TBranch        *b_pfmetphi_puppi;   //!
   TBranch        *b_npu;   //!
   TBranch        *b_globalRhokt4;   //!
   TBranch        *b_globalGridFastJet;   //!
   TBranch        *b_Rhokt4_0;   //!
   TBranch        *b_Rhokt4_1;   //!
   TBranch        *b_Rhokt4_2;   //!
   TBranch        *b_RhoGridFastJet_0;   //!
   TBranch        *b_RhoGridFastJet_1;   //!
   TBranch        *b_RhoGridFastJet_2;   //!
   TBranch        *b_PuppiRhokt4_0;   //!
   TBranch        *b_PuppiRhokt4_1;   //!
   TBranch        *b_PuppiRhoGridFastJet_0;   //!
   TBranch        *b_PuppiRhoGridFastJet_1;   //!

   readTree(TTree *tree=0);
   virtual ~readTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readTree_cxx
readTree::readTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data2/rgerosa/NTUPLES_TP/PHANTOM_SS_EWK_DF_126_2p5/outDumper_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data2/rgerosa/NTUPLES_TP/PHANTOM_SS_EWK_DF_126_2p5/outDumper_0.root");
      }
      f->GetObject("readTree",tree);

   }
   Init(tree);
}

readTree::~readTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readTree::LoadTree(Long64_t entry)
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

void readTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("leptonLHEpt1", &leptonLHEpt1, &b_leptonLHEpt1);
   fChain->SetBranchAddress("leptonLHEeta1", &leptonLHEeta1, &b_leptonLHEeta1);
   fChain->SetBranchAddress("leptonLHEphi1", &leptonLHEphi1, &b_leptonLHEphi1);
   fChain->SetBranchAddress("leptonLHEpid1", &leptonLHEpid1, &b_leptonLHEpid1);
   fChain->SetBranchAddress("leptonLHEspin1", &leptonLHEspin1, &b_leptonLHEspin1);
   fChain->SetBranchAddress("leptonLHEch1", &leptonLHEch1, &b_leptonLHEch1);
   fChain->SetBranchAddress("leptonLHEm1", &leptonLHEm1, &b_leptonLHEm1);
   fChain->SetBranchAddress("neutrinoLHEpt1", &neutrinoLHEpt1, &b_neutrinoLHEpt1);
   fChain->SetBranchAddress("neutrinoLHEeta1", &neutrinoLHEeta1, &b_neutrinoLHEeta1);
   fChain->SetBranchAddress("neutrinoLHEphi1", &neutrinoLHEphi1, &b_neutrinoLHEphi1);
   fChain->SetBranchAddress("neutrinoLHEpid1", &neutrinoLHEpid1, &b_neutrinoLHEpid1);
   fChain->SetBranchAddress("neutrinoLHEspin1", &neutrinoLHEspin1, &b_neutrinoLHEspin1);
   fChain->SetBranchAddress("jetLHEPartonpt1", &jetLHEPartonpt1, &b_jetLHEPartonpt1);
   fChain->SetBranchAddress("jetLHEPartoneta1", &jetLHEPartoneta1, &b_jetLHEPartoneta1);
   fChain->SetBranchAddress("jetLHEPartonphi1", &jetLHEPartonphi1, &b_jetLHEPartonphi1);
   fChain->SetBranchAddress("jetLHEPartonpid1", &jetLHEPartonpid1, &b_jetLHEPartonpid1);
   fChain->SetBranchAddress("jetLHEPartonspin1", &jetLHEPartonspin1, &b_jetLHEPartonspin1);
   fChain->SetBranchAddress("leptonLHEpt2", &leptonLHEpt2, &b_leptonLHEpt2);
   fChain->SetBranchAddress("leptonLHEeta2", &leptonLHEeta2, &b_leptonLHEeta2);
   fChain->SetBranchAddress("leptonLHEphi2", &leptonLHEphi2, &b_leptonLHEphi2);
   fChain->SetBranchAddress("leptonLHEpid2", &leptonLHEpid2, &b_leptonLHEpid2);
   fChain->SetBranchAddress("leptonLHEspin2", &leptonLHEspin2, &b_leptonLHEspin2);
   fChain->SetBranchAddress("leptonLHEch2", &leptonLHEch2, &b_leptonLHEch2);
   fChain->SetBranchAddress("leptonLHEm2", &leptonLHEm2, &b_leptonLHEm2);
   fChain->SetBranchAddress("neutrinoLHEpt2", &neutrinoLHEpt2, &b_neutrinoLHEpt2);
   fChain->SetBranchAddress("neutrinoLHEeta2", &neutrinoLHEeta2, &b_neutrinoLHEeta2);
   fChain->SetBranchAddress("neutrinoLHEphi2", &neutrinoLHEphi2, &b_neutrinoLHEphi2);
   fChain->SetBranchAddress("neutrinoLHEpid2", &neutrinoLHEpid2, &b_neutrinoLHEpid2);
   fChain->SetBranchAddress("neutrinoLHEspin2", &neutrinoLHEspin2, &b_neutrinoLHEspin2);
   fChain->SetBranchAddress("jetLHEPartonpt2", &jetLHEPartonpt2, &b_jetLHEPartonpt2);
   fChain->SetBranchAddress("jetLHEPartoneta2", &jetLHEPartoneta2, &b_jetLHEPartoneta2);
   fChain->SetBranchAddress("jetLHEPartonphi2", &jetLHEPartonphi2, &b_jetLHEPartonphi2);
   fChain->SetBranchAddress("jetLHEPartonpid2", &jetLHEPartonpid2, &b_jetLHEPartonpid2);
   fChain->SetBranchAddress("jetLHEPartonspin2", &jetLHEPartonspin2, &b_jetLHEPartonspin2);
   fChain->SetBranchAddress("leptonLHEpt3", &leptonLHEpt3, &b_leptonLHEpt3);
   fChain->SetBranchAddress("leptonLHEeta3", &leptonLHEeta3, &b_leptonLHEeta3);
   fChain->SetBranchAddress("leptonLHEphi3", &leptonLHEphi3, &b_leptonLHEphi3);
   fChain->SetBranchAddress("leptonLHEpid3", &leptonLHEpid3, &b_leptonLHEpid3);
   fChain->SetBranchAddress("leptonLHEspin3", &leptonLHEspin3, &b_leptonLHEspin3);
   fChain->SetBranchAddress("leptonLHEch3", &leptonLHEch3, &b_leptonLHEch3);
   fChain->SetBranchAddress("leptonLHEm3", &leptonLHEm3, &b_leptonLHEm3);
   fChain->SetBranchAddress("neutrinoLHEpt3", &neutrinoLHEpt3, &b_neutrinoLHEpt3);
   fChain->SetBranchAddress("neutrinoLHEeta3", &neutrinoLHEeta3, &b_neutrinoLHEeta3);
   fChain->SetBranchAddress("neutrinoLHEphi3", &neutrinoLHEphi3, &b_neutrinoLHEphi3);
   fChain->SetBranchAddress("neutrinoLHEpid3", &neutrinoLHEpid3, &b_neutrinoLHEpid3);
   fChain->SetBranchAddress("neutrinoLHEspin3", &neutrinoLHEspin3, &b_neutrinoLHEspin3);
   fChain->SetBranchAddress("jetLHEPartonpt3", &jetLHEPartonpt3, &b_jetLHEPartonpt3);
   fChain->SetBranchAddress("jetLHEPartoneta3", &jetLHEPartoneta3, &b_jetLHEPartoneta3);
   fChain->SetBranchAddress("jetLHEPartonphi3", &jetLHEPartonphi3, &b_jetLHEPartonphi3);
   fChain->SetBranchAddress("jetLHEPartonpid3", &jetLHEPartonpid3, &b_jetLHEPartonpid3);
   fChain->SetBranchAddress("jetLHEPartonspin3", &jetLHEPartonspin3, &b_jetLHEPartonspin3);
   fChain->SetBranchAddress("leptonLHEpt4", &leptonLHEpt4, &b_leptonLHEpt4);
   fChain->SetBranchAddress("leptonLHEeta4", &leptonLHEeta4, &b_leptonLHEeta4);
   fChain->SetBranchAddress("leptonLHEphi4", &leptonLHEphi4, &b_leptonLHEphi4);
   fChain->SetBranchAddress("leptonLHEpid4", &leptonLHEpid4, &b_leptonLHEpid4);
   fChain->SetBranchAddress("leptonLHEspin4", &leptonLHEspin4, &b_leptonLHEspin4);
   fChain->SetBranchAddress("leptonLHEch4", &leptonLHEch4, &b_leptonLHEch4);
   fChain->SetBranchAddress("leptonLHEm4", &leptonLHEm4, &b_leptonLHEm4);
   fChain->SetBranchAddress("neutrinoLHEpt4", &neutrinoLHEpt4, &b_neutrinoLHEpt4);
   fChain->SetBranchAddress("neutrinoLHEeta4", &neutrinoLHEeta4, &b_neutrinoLHEeta4);
   fChain->SetBranchAddress("neutrinoLHEphi4", &neutrinoLHEphi4, &b_neutrinoLHEphi4);
   fChain->SetBranchAddress("neutrinoLHEpid4", &neutrinoLHEpid4, &b_neutrinoLHEpid4);
   fChain->SetBranchAddress("neutrinoLHEspin4", &neutrinoLHEspin4, &b_neutrinoLHEspin4);
   fChain->SetBranchAddress("jetLHEPartonpt4", &jetLHEPartonpt4, &b_jetLHEPartonpt4);
   fChain->SetBranchAddress("jetLHEPartoneta4", &jetLHEPartoneta4, &b_jetLHEPartoneta4);
   fChain->SetBranchAddress("jetLHEPartonphi4", &jetLHEPartonphi4, &b_jetLHEPartonphi4);
   fChain->SetBranchAddress("jetLHEPartonpid4", &jetLHEPartonpid4, &b_jetLHEPartonpid4);
   fChain->SetBranchAddress("jetLHEPartonspin4", &jetLHEPartonspin4, &b_jetLHEPartonspin4);
   fChain->SetBranchAddress("vbosonLHEpt1", &vbosonLHEpt1, &b_vbosonLHEpt1);
   fChain->SetBranchAddress("vbosonLHEeta1", &vbosonLHEeta1, &b_vbosonLHEeta1);
   fChain->SetBranchAddress("vbosonLHEphi1", &vbosonLHEphi1, &b_vbosonLHEphi1);
   fChain->SetBranchAddress("vbosonLHEpid1", &vbosonLHEpid1, &b_vbosonLHEpid1);
   fChain->SetBranchAddress("vbosonLHEspin1", &vbosonLHEspin1, &b_vbosonLHEspin1);
   fChain->SetBranchAddress("vbosonLHEch1", &vbosonLHEch1, &b_vbosonLHEch1);
   fChain->SetBranchAddress("vbosonLHEm1", &vbosonLHEm1, &b_vbosonLHEm1);
   fChain->SetBranchAddress("jetLHEGluonpt1", &jetLHEGluonpt1, &b_jetLHEGluonpt1);
   fChain->SetBranchAddress("jetLHEGluoneta1", &jetLHEGluoneta1, &b_jetLHEGluoneta1);
   fChain->SetBranchAddress("jetLHEGluonphi1", &jetLHEGluonphi1, &b_jetLHEGluonphi1);
   fChain->SetBranchAddress("jetLHEGluonpid1", &jetLHEGluonpid1, &b_jetLHEGluonpid1);
   fChain->SetBranchAddress("jetLHEGluonspin1", &jetLHEGluonspin1, &b_jetLHEGluonspin1);
   fChain->SetBranchAddress("vbosonLHEpt2", &vbosonLHEpt2, &b_vbosonLHEpt2);
   fChain->SetBranchAddress("vbosonLHEeta2", &vbosonLHEeta2, &b_vbosonLHEeta2);
   fChain->SetBranchAddress("vbosonLHEphi2", &vbosonLHEphi2, &b_vbosonLHEphi2);
   fChain->SetBranchAddress("vbosonLHEpid2", &vbosonLHEpid2, &b_vbosonLHEpid2);
   fChain->SetBranchAddress("vbosonLHEspin2", &vbosonLHEspin2, &b_vbosonLHEspin2);
   fChain->SetBranchAddress("vbosonLHEch2", &vbosonLHEch2, &b_vbosonLHEch2);
   fChain->SetBranchAddress("vbosonLHEm2", &vbosonLHEm2, &b_vbosonLHEm2);
   fChain->SetBranchAddress("jetLHEGluonpt2", &jetLHEGluonpt2, &b_jetLHEGluonpt2);
   fChain->SetBranchAddress("jetLHEGluoneta2", &jetLHEGluoneta2, &b_jetLHEGluoneta2);
   fChain->SetBranchAddress("jetLHEGluonphi2", &jetLHEGluonphi2, &b_jetLHEGluonphi2);
   fChain->SetBranchAddress("jetLHEGluonpid2", &jetLHEGluonpid2, &b_jetLHEGluonpid2);
   fChain->SetBranchAddress("jetLHEGluonspin2", &jetLHEGluonspin2, &b_jetLHEGluonspin2);
   fChain->SetBranchAddress("jetGenpt1", &jetGenpt1, &b_jetGenpt1);
   fChain->SetBranchAddress("jetGeneta1", &jetGeneta1, &b_jetGeneta1);
   fChain->SetBranchAddress("jetGenphi1", &jetGenphi1, &b_jetGenphi1);
   fChain->SetBranchAddress("jetGenm1", &jetGenm1, &b_jetGenm1);
   fChain->SetBranchAddress("jetGenAreaX1", &jetGenAreaX1, &b_jetGenAreaX1);
   fChain->SetBranchAddress("jetGenAreaY1", &jetGenAreaY1, &b_jetGenAreaY1);
   fChain->SetBranchAddress("jetGenAreaZ1", &jetGenAreaZ1, &b_jetGenAreaZ1);
   fChain->SetBranchAddress("jetGenAreaT1", &jetGenAreaT1, &b_jetGenAreaT1);
   fChain->SetBranchAddress("jetGenpt2", &jetGenpt2, &b_jetGenpt2);
   fChain->SetBranchAddress("jetGeneta2", &jetGeneta2, &b_jetGeneta2);
   fChain->SetBranchAddress("jetGenphi2", &jetGenphi2, &b_jetGenphi2);
   fChain->SetBranchAddress("jetGenm2", &jetGenm2, &b_jetGenm2);
   fChain->SetBranchAddress("jetGenAreaX2", &jetGenAreaX2, &b_jetGenAreaX2);
   fChain->SetBranchAddress("jetGenAreaY2", &jetGenAreaY2, &b_jetGenAreaY2);
   fChain->SetBranchAddress("jetGenAreaZ2", &jetGenAreaZ2, &b_jetGenAreaZ2);
   fChain->SetBranchAddress("jetGenAreaT2", &jetGenAreaT2, &b_jetGenAreaT2);
   fChain->SetBranchAddress("jetGenpt3", &jetGenpt3, &b_jetGenpt3);
   fChain->SetBranchAddress("jetGeneta3", &jetGeneta3, &b_jetGeneta3);
   fChain->SetBranchAddress("jetGenphi3", &jetGenphi3, &b_jetGenphi3);
   fChain->SetBranchAddress("jetGenm3", &jetGenm3, &b_jetGenm3);
   fChain->SetBranchAddress("jetGenAreaX3", &jetGenAreaX3, &b_jetGenAreaX3);
   fChain->SetBranchAddress("jetGenAreaY3", &jetGenAreaY3, &b_jetGenAreaY3);
   fChain->SetBranchAddress("jetGenAreaZ3", &jetGenAreaZ3, &b_jetGenAreaZ3);
   fChain->SetBranchAddress("jetGenAreaT3", &jetGenAreaT3, &b_jetGenAreaT3);
   fChain->SetBranchAddress("jetGenpt4", &jetGenpt4, &b_jetGenpt4);
   fChain->SetBranchAddress("jetGeneta4", &jetGeneta4, &b_jetGeneta4);
   fChain->SetBranchAddress("jetGenphi4", &jetGenphi4, &b_jetGenphi4);
   fChain->SetBranchAddress("jetGenm4", &jetGenm4, &b_jetGenm4);
   fChain->SetBranchAddress("jetGenAreaX4", &jetGenAreaX4, &b_jetGenAreaX4);
   fChain->SetBranchAddress("jetGenAreaY4", &jetGenAreaY4, &b_jetGenAreaY4);
   fChain->SetBranchAddress("jetGenAreaZ4", &jetGenAreaZ4, &b_jetGenAreaZ4);
   fChain->SetBranchAddress("jetGenAreaT4", &jetGenAreaT4, &b_jetGenAreaT4);
   fChain->SetBranchAddress("TrackJet_V4", &TrackJet_V4_, &b_TrackJet_V4_);
   fChain->SetBranchAddress("TrackJet_V4.fUniqueID", TrackJet_V4_fUniqueID, &b_TrackJet_V4_fUniqueID);
   fChain->SetBranchAddress("TrackJet_V4.fBits", TrackJet_V4_fBits, &b_TrackJet_V4_fBits);
   fChain->SetBranchAddress("TrackJet_V4.fP.fUniqueID", TrackJet_V4_fP_fUniqueID, &b_TrackJet_V4_fP_fUniqueID);
   fChain->SetBranchAddress("TrackJet_V4.fP.fBits", TrackJet_V4_fP_fBits, &b_TrackJet_V4_fP_fBits);
   fChain->SetBranchAddress("TrackJet_V4.fP.fX", TrackJet_V4_fP_fX, &b_TrackJet_V4_fP_fX);
   fChain->SetBranchAddress("TrackJet_V4.fP.fY", TrackJet_V4_fP_fY, &b_TrackJet_V4_fP_fY);
   fChain->SetBranchAddress("TrackJet_V4.fP.fZ", TrackJet_V4_fP_fZ, &b_TrackJet_V4_fP_fZ);
   fChain->SetBranchAddress("TrackJet_V4.fE", TrackJet_V4_fE, &b_TrackJet_V4_fE);
   fChain->SetBranchAddress("HtSoft", &HtSoft, &b_HtSoft);
   fChain->SetBranchAddress("nSoftJets", &nSoftJets, &b_nSoftJets);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   fChain->SetBranchAddress("detajj", &detajj, &b_detajj);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("nbjet", &nbjet, &b_nbjet);
   fChain->SetBranchAddress("hardbjpb", &hardbjpb, &b_hardbjpb);
   fChain->SetBranchAddress("softbjpb", &softbjpb, &b_softbjpb);
   fChain->SetBranchAddress("njetid", &njetid, &b_njetid);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jetphi1", &jetphi1, &b_jetphi1);
   fChain->SetBranchAddress("jetmass1", &jetmass1, &b_jetmass1);
   fChain->SetBranchAddress("jetAreaX1", &jetAreaX1, &b_jetAreaX1);
   fChain->SetBranchAddress("jetAreaY1", &jetAreaY1, &b_jetAreaY1);
   fChain->SetBranchAddress("jetAreaZ1", &jetAreaZ1, &b_jetAreaZ1);
   fChain->SetBranchAddress("jetAreaT1", &jetAreaT1, &b_jetAreaT1);
   fChain->SetBranchAddress("jetBTagAlgo1", &jetBTagAlgo1, &b_jetBTagAlgo1);
   fChain->SetBranchAddress("jetBTagDefault1", &jetBTagDefault1, &b_jetBTagDefault1);
   fChain->SetBranchAddress("jetBTagPhysics1", &jetBTagPhysics1, &b_jetBTagPhysics1);
   fChain->SetBranchAddress("jetBTagNearest2_1", &jetBTagNearest2_1, &b_jetBTagNearest2_1);
   fChain->SetBranchAddress("jetBTagNearest3_1", &jetBTagNearest3_1, &b_jetBTagNearest3_1);
   fChain->SetBranchAddress("jetBTagHeaviest_1", &jetBTagHeaviest_1, &b_jetBTagHeaviest_1);
   fChain->SetBranchAddress("jetFlavourAlgo1", &jetFlavourAlgo1, &b_jetFlavourAlgo1);
   fChain->SetBranchAddress("jetFlavourDefault1", &jetFlavourDefault1, &b_jetFlavourDefault1);
   fChain->SetBranchAddress("jetFlavourPhysics1", &jetFlavourPhysics1, &b_jetFlavourPhysics1);
   fChain->SetBranchAddress("jetFlavourNearest2_1", &jetFlavourNearest2_1, &b_jetFlavourNearest2_1);
   fChain->SetBranchAddress("jetFlavourNearest3_1", &jetFlavourNearest3_1, &b_jetFlavourNearest3_1);
   fChain->SetBranchAddress("jetFlavourHeaviest_1", &jetFlavourHeaviest_1, &b_jetFlavourHeaviest_1);
   fChain->SetBranchAddress("jetptD1", &jetptD1, &b_jetptD1);
   fChain->SetBranchAddress("jetptDNe1", &jetptDNe1, &b_jetptDNe1);
   fChain->SetBranchAddress("jetptDCh1", &jetptDCh1, &b_jetptDCh1);
   fChain->SetBranchAddress("jetnNeutral1", &jetnNeutral1, &b_jetnNeutral1);
   fChain->SetBranchAddress("jetnCharged1", &jetnCharged1, &b_jetnCharged1);
   fChain->SetBranchAddress("jetneuEMfrac1", &jetneuEMfrac1, &b_jetneuEMfrac1);
   fChain->SetBranchAddress("jetneuHadfrac1", &jetneuHadfrac1, &b_jetneuHadfrac1);
   fChain->SetBranchAddress("jetbetaClassic1", &jetbetaClassic1, &b_jetbetaClassic1);
   fChain->SetBranchAddress("jetbetaClassicStar1", &jetbetaClassicStar1, &b_jetbetaClassicStar1);
   fChain->SetBranchAddress("jetbeta1", &jetbeta1, &b_jetbeta1);
   fChain->SetBranchAddress("jetbetaStar1", &jetbetaStar1, &b_jetbetaStar1);
   fChain->SetBranchAddress("jetconstituents1", &jetconstituents1, &b_jetconstituents1);
   fChain->SetBranchAddress("jetaxis2_1", &jetaxis2_1, &b_jetaxis2_1);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased1", &jetpileupIDFlagCutBased1, &b_jetpileupIDFlagCutBased1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("jetphi2", &jetphi2, &b_jetphi2);
   fChain->SetBranchAddress("jetmass2", &jetmass2, &b_jetmass2);
   fChain->SetBranchAddress("jetAreaX2", &jetAreaX2, &b_jetAreaX2);
   fChain->SetBranchAddress("jetAreaY2", &jetAreaY2, &b_jetAreaY2);
   fChain->SetBranchAddress("jetAreaZ2", &jetAreaZ2, &b_jetAreaZ2);
   fChain->SetBranchAddress("jetAreaT2", &jetAreaT2, &b_jetAreaT2);
   fChain->SetBranchAddress("jetBTagAlgo2", &jetBTagAlgo2, &b_jetBTagAlgo2);
   fChain->SetBranchAddress("jetBTagDefault2", &jetBTagDefault2, &b_jetBTagDefault2);
   fChain->SetBranchAddress("jetBTagPhysics2", &jetBTagPhysics2, &b_jetBTagPhysics2);
   fChain->SetBranchAddress("jetBTagNearest2_2", &jetBTagNearest2_2, &b_jetBTagNearest2_2);
   fChain->SetBranchAddress("jetBTagNearest3_2", &jetBTagNearest3_2, &b_jetBTagNearest3_2);
   fChain->SetBranchAddress("jetBTagHeaviest_2", &jetBTagHeaviest_2, &b_jetBTagHeaviest_2);
   fChain->SetBranchAddress("jetFlavourAlgo2", &jetFlavourAlgo2, &b_jetFlavourAlgo2);
   fChain->SetBranchAddress("jetFlavourDefault2", &jetFlavourDefault2, &b_jetFlavourDefault2);
   fChain->SetBranchAddress("jetFlavourPhysics2", &jetFlavourPhysics2, &b_jetFlavourPhysics2);
   fChain->SetBranchAddress("jetFlavourNearest2_2", &jetFlavourNearest2_2, &b_jetFlavourNearest2_2);
   fChain->SetBranchAddress("jetFlavourNearest3_2", &jetFlavourNearest3_2, &b_jetFlavourNearest3_2);
   fChain->SetBranchAddress("jetFlavourHeaviest_2", &jetFlavourHeaviest_2, &b_jetFlavourHeaviest_2);
   fChain->SetBranchAddress("jetptD2", &jetptD2, &b_jetptD2);
   fChain->SetBranchAddress("jetptDNe2", &jetptDNe2, &b_jetptDNe2);
   fChain->SetBranchAddress("jetptDCh2", &jetptDCh2, &b_jetptDCh2);
   fChain->SetBranchAddress("jetnNeutral2", &jetnNeutral2, &b_jetnNeutral2);
   fChain->SetBranchAddress("jetnCharged2", &jetnCharged2, &b_jetnCharged2);
   fChain->SetBranchAddress("jetneuEMfrac2", &jetneuEMfrac2, &b_jetneuEMfrac2);
   fChain->SetBranchAddress("jetneuHadfrac2", &jetneuHadfrac2, &b_jetneuHadfrac2);
   fChain->SetBranchAddress("jetbetaClassic2", &jetbetaClassic2, &b_jetbetaClassic2);
   fChain->SetBranchAddress("jetbetaClassicStar2", &jetbetaClassicStar2, &b_jetbetaClassicStar2);
   fChain->SetBranchAddress("jetbeta2", &jetbeta2, &b_jetbeta2);
   fChain->SetBranchAddress("jetbetaStar2", &jetbetaStar2, &b_jetbetaStar2);
   fChain->SetBranchAddress("jetconstituents2", &jetconstituents2, &b_jetconstituents2);
   fChain->SetBranchAddress("jetaxis2_2", &jetaxis2_2, &b_jetaxis2_2);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased2", &jetpileupIDFlagCutBased2, &b_jetpileupIDFlagCutBased2);
   fChain->SetBranchAddress("jetpt3", &jetpt3, &b_jetpt3);
   fChain->SetBranchAddress("jeteta3", &jeteta3, &b_jeteta3);
   fChain->SetBranchAddress("jetphi3", &jetphi3, &b_jetphi3);
   fChain->SetBranchAddress("jetmass3", &jetmass3, &b_jetmass3);
   fChain->SetBranchAddress("jetAreaX3", &jetAreaX3, &b_jetAreaX3);
   fChain->SetBranchAddress("jetAreaY3", &jetAreaY3, &b_jetAreaY3);
   fChain->SetBranchAddress("jetAreaZ3", &jetAreaZ3, &b_jetAreaZ3);
   fChain->SetBranchAddress("jetAreaT3", &jetAreaT3, &b_jetAreaT3);
   fChain->SetBranchAddress("jetBTagAlgo3", &jetBTagAlgo3, &b_jetBTagAlgo3);
   fChain->SetBranchAddress("jetBTagDefault3", &jetBTagDefault3, &b_jetBTagDefault3);
   fChain->SetBranchAddress("jetBTagPhysics3", &jetBTagPhysics3, &b_jetBTagPhysics3);
   fChain->SetBranchAddress("jetBTagNearest2_3", &jetBTagNearest2_3, &b_jetBTagNearest2_3);
   fChain->SetBranchAddress("jetBTagNearest3_3", &jetBTagNearest3_3, &b_jetBTagNearest3_3);
   fChain->SetBranchAddress("jetBTagHeaviest_3", &jetBTagHeaviest_3, &b_jetBTagHeaviest_3);
   fChain->SetBranchAddress("jetFlavourAlgo3", &jetFlavourAlgo3, &b_jetFlavourAlgo3);
   fChain->SetBranchAddress("jetFlavourDefault3", &jetFlavourDefault3, &b_jetFlavourDefault3);
   fChain->SetBranchAddress("jetFlavourPhysics3", &jetFlavourPhysics3, &b_jetFlavourPhysics3);
   fChain->SetBranchAddress("jetFlavourNearest2_3", &jetFlavourNearest2_3, &b_jetFlavourNearest2_3);
   fChain->SetBranchAddress("jetFlavourNearest3_3", &jetFlavourNearest3_3, &b_jetFlavourNearest3_3);
   fChain->SetBranchAddress("jetFlavourHeaviest_3", &jetFlavourHeaviest_3, &b_jetFlavourHeaviest_3);
   fChain->SetBranchAddress("jetptD3", &jetptD3, &b_jetptD3);
   fChain->SetBranchAddress("jetptDNe3", &jetptDNe3, &b_jetptDNe3);
   fChain->SetBranchAddress("jetptDCh3", &jetptDCh3, &b_jetptDCh3);
   fChain->SetBranchAddress("jetnNeutral3", &jetnNeutral3, &b_jetnNeutral3);
   fChain->SetBranchAddress("jetnCharged3", &jetnCharged3, &b_jetnCharged3);
   fChain->SetBranchAddress("jetneuEMfrac3", &jetneuEMfrac3, &b_jetneuEMfrac3);
   fChain->SetBranchAddress("jetneuHadfrac3", &jetneuHadfrac3, &b_jetneuHadfrac3);
   fChain->SetBranchAddress("jetbetaClassic3", &jetbetaClassic3, &b_jetbetaClassic3);
   fChain->SetBranchAddress("jetbetaClassicStar3", &jetbetaClassicStar3, &b_jetbetaClassicStar3);
   fChain->SetBranchAddress("jetbeta3", &jetbeta3, &b_jetbeta3);
   fChain->SetBranchAddress("jetbetaStar3", &jetbetaStar3, &b_jetbetaStar3);
   fChain->SetBranchAddress("jetconstituents3", &jetconstituents3, &b_jetconstituents3);
   fChain->SetBranchAddress("jetaxis2_3", &jetaxis2_3, &b_jetaxis2_3);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased3", &jetpileupIDFlagCutBased3, &b_jetpileupIDFlagCutBased3);
   fChain->SetBranchAddress("jetpt4", &jetpt4, &b_jetpt4);
   fChain->SetBranchAddress("jeteta4", &jeteta4, &b_jeteta4);
   fChain->SetBranchAddress("jetphi4", &jetphi4, &b_jetphi4);
   fChain->SetBranchAddress("jetmass4", &jetmass4, &b_jetmass4);
   fChain->SetBranchAddress("jetAreaX4", &jetAreaX4, &b_jetAreaX4);
   fChain->SetBranchAddress("jetAreaY4", &jetAreaY4, &b_jetAreaY4);
   fChain->SetBranchAddress("jetAreaZ4", &jetAreaZ4, &b_jetAreaZ4);
   fChain->SetBranchAddress("jetAreaT4", &jetAreaT4, &b_jetAreaT4);
   fChain->SetBranchAddress("jetBTagAlgo4", &jetBTagAlgo4, &b_jetBTagAlgo4);
   fChain->SetBranchAddress("jetBTagDefault4", &jetBTagDefault4, &b_jetBTagDefault4);
   fChain->SetBranchAddress("jetBTagPhysics4", &jetBTagPhysics4, &b_jetBTagPhysics4);
   fChain->SetBranchAddress("jetBTagNearest2_4", &jetBTagNearest2_4, &b_jetBTagNearest2_4);
   fChain->SetBranchAddress("jetBTagNearest3_4", &jetBTagNearest3_4, &b_jetBTagNearest3_4);
   fChain->SetBranchAddress("jetBTagHeaviest_4", &jetBTagHeaviest_4, &b_jetBTagHeaviest_4);
   fChain->SetBranchAddress("jetFlavourAlgo4", &jetFlavourAlgo4, &b_jetFlavourAlgo4);
   fChain->SetBranchAddress("jetFlavourDefault4", &jetFlavourDefault4, &b_jetFlavourDefault4);
   fChain->SetBranchAddress("jetFlavourPhysics4", &jetFlavourPhysics4, &b_jetFlavourPhysics4);
   fChain->SetBranchAddress("jetFlavourNearest2_4", &jetFlavourNearest2_4, &b_jetFlavourNearest2_4);
   fChain->SetBranchAddress("jetFlavourNearest3_4", &jetFlavourNearest3_4, &b_jetFlavourNearest3_4);
   fChain->SetBranchAddress("jetFlavourHeaviest_4", &jetFlavourHeaviest_4, &b_jetFlavourHeaviest_4);
   fChain->SetBranchAddress("jetptD4", &jetptD4, &b_jetptD4);
   fChain->SetBranchAddress("jetptDNe4", &jetptDNe4, &b_jetptDNe4);
   fChain->SetBranchAddress("jetptDCh4", &jetptDCh4, &b_jetptDCh4);
   fChain->SetBranchAddress("jetnNeutral4", &jetnNeutral4, &b_jetnNeutral4);
   fChain->SetBranchAddress("jetnCharged4", &jetnCharged4, &b_jetnCharged4);
   fChain->SetBranchAddress("jetneuEMfrac4", &jetneuEMfrac4, &b_jetneuEMfrac4);
   fChain->SetBranchAddress("jetneuHadfrac4", &jetneuHadfrac4, &b_jetneuHadfrac4);
   fChain->SetBranchAddress("jetbetaClassic4", &jetbetaClassic4, &b_jetbetaClassic4);
   fChain->SetBranchAddress("jetbetaClassicStar4", &jetbetaClassicStar4, &b_jetbetaClassicStar4);
   fChain->SetBranchAddress("jetbeta4", &jetbeta4, &b_jetbeta4);
   fChain->SetBranchAddress("jetbetaStar4", &jetbetaStar4, &b_jetbetaStar4);
   fChain->SetBranchAddress("jetconstituents4", &jetconstituents4, &b_jetconstituents4);
   fChain->SetBranchAddress("jetaxis2_4", &jetaxis2_4, &b_jetaxis2_4);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased4", &jetpileupIDFlagCutBased4, &b_jetpileupIDFlagCutBased4);
   fChain->SetBranchAddress("jetpt5", &jetpt5, &b_jetpt5);
   fChain->SetBranchAddress("jeteta5", &jeteta5, &b_jeteta5);
   fChain->SetBranchAddress("jetphi5", &jetphi5, &b_jetphi5);
   fChain->SetBranchAddress("jetmass5", &jetmass5, &b_jetmass5);
   fChain->SetBranchAddress("jetAreaX5", &jetAreaX5, &b_jetAreaX5);
   fChain->SetBranchAddress("jetAreaY5", &jetAreaY5, &b_jetAreaY5);
   fChain->SetBranchAddress("jetAreaZ5", &jetAreaZ5, &b_jetAreaZ5);
   fChain->SetBranchAddress("jetAreaT5", &jetAreaT5, &b_jetAreaT5);
   fChain->SetBranchAddress("jetBTagAlgo5", &jetBTagAlgo5, &b_jetBTagAlgo5);
   fChain->SetBranchAddress("jetBTagDefault5", &jetBTagDefault5, &b_jetBTagDefault5);
   fChain->SetBranchAddress("jetBTagPhysics5", &jetBTagPhysics5, &b_jetBTagPhysics5);
   fChain->SetBranchAddress("jetBTagNearest2_5", &jetBTagNearest2_5, &b_jetBTagNearest2_5);
   fChain->SetBranchAddress("jetBTagNearest3_5", &jetBTagNearest3_5, &b_jetBTagNearest3_5);
   fChain->SetBranchAddress("jetBTagHeaviest_5", &jetBTagHeaviest_5, &b_jetBTagHeaviest_5);
   fChain->SetBranchAddress("jetFlavourAlgo5", &jetFlavourAlgo5, &b_jetFlavourAlgo5);
   fChain->SetBranchAddress("jetFlavourDefault5", &jetFlavourDefault5, &b_jetFlavourDefault5);
   fChain->SetBranchAddress("jetFlavourPhysics5", &jetFlavourPhysics5, &b_jetFlavourPhysics5);
   fChain->SetBranchAddress("jetFlavourNearest2_5", &jetFlavourNearest2_5, &b_jetFlavourNearest2_5);
   fChain->SetBranchAddress("jetFlavourNearest3_5", &jetFlavourNearest3_5, &b_jetFlavourNearest3_5);
   fChain->SetBranchAddress("jetFlavourHeaviest_5", &jetFlavourHeaviest_5, &b_jetFlavourHeaviest_5);
   fChain->SetBranchAddress("jetptD5", &jetptD5, &b_jetptD5);
   fChain->SetBranchAddress("jetptDNe5", &jetptDNe5, &b_jetptDNe5);
   fChain->SetBranchAddress("jetptDCh5", &jetptDCh5, &b_jetptDCh5);
   fChain->SetBranchAddress("jetnNeutral5", &jetnNeutral5, &b_jetnNeutral5);
   fChain->SetBranchAddress("jetnCharged5", &jetnCharged5, &b_jetnCharged5);
   fChain->SetBranchAddress("jetneuEMfrac5", &jetneuEMfrac5, &b_jetneuEMfrac5);
   fChain->SetBranchAddress("jetneuHadfrac5", &jetneuHadfrac5, &b_jetneuHadfrac5);
   fChain->SetBranchAddress("jetbetaClassic5", &jetbetaClassic5, &b_jetbetaClassic5);
   fChain->SetBranchAddress("jetbetaClassicStar5", &jetbetaClassicStar5, &b_jetbetaClassicStar5);
   fChain->SetBranchAddress("jetbeta5", &jetbeta5, &b_jetbeta5);
   fChain->SetBranchAddress("jetbetaStar5", &jetbetaStar5, &b_jetbetaStar5);
   fChain->SetBranchAddress("jetconstituents5", &jetconstituents5, &b_jetconstituents5);
   fChain->SetBranchAddress("jetaxis2_5", &jetaxis2_5, &b_jetaxis2_5);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased5", &jetpileupIDFlagCutBased5, &b_jetpileupIDFlagCutBased5);
   fChain->SetBranchAddress("jetpt6", &jetpt6, &b_jetpt6);
   fChain->SetBranchAddress("jeteta6", &jeteta6, &b_jeteta6);
   fChain->SetBranchAddress("jetphi6", &jetphi6, &b_jetphi6);
   fChain->SetBranchAddress("jetmass6", &jetmass6, &b_jetmass6);
   fChain->SetBranchAddress("jetAreaX6", &jetAreaX6, &b_jetAreaX6);
   fChain->SetBranchAddress("jetAreaY6", &jetAreaY6, &b_jetAreaY6);
   fChain->SetBranchAddress("jetAreaZ6", &jetAreaZ6, &b_jetAreaZ6);
   fChain->SetBranchAddress("jetAreaT6", &jetAreaT6, &b_jetAreaT6);
   fChain->SetBranchAddress("jetBTagAlgo6", &jetBTagAlgo6, &b_jetBTagAlgo6);
   fChain->SetBranchAddress("jetBTagDefault6", &jetBTagDefault6, &b_jetBTagDefault6);
   fChain->SetBranchAddress("jetBTagPhysics6", &jetBTagPhysics6, &b_jetBTagPhysics6);
   fChain->SetBranchAddress("jetBTagNearest2_6", &jetBTagNearest2_6, &b_jetBTagNearest2_6);
   fChain->SetBranchAddress("jetBTagNearest3_6", &jetBTagNearest3_6, &b_jetBTagNearest3_6);
   fChain->SetBranchAddress("jetBTagHeaviest_6", &jetBTagHeaviest_6, &b_jetBTagHeaviest_6);
   fChain->SetBranchAddress("jetFlavourAlgo6", &jetFlavourAlgo6, &b_jetFlavourAlgo6);
   fChain->SetBranchAddress("jetFlavourDefault6", &jetFlavourDefault6, &b_jetFlavourDefault6);
   fChain->SetBranchAddress("jetFlavourPhysics6", &jetFlavourPhysics6, &b_jetFlavourPhysics6);
   fChain->SetBranchAddress("jetFlavourNearest2_6", &jetFlavourNearest2_6, &b_jetFlavourNearest2_6);
   fChain->SetBranchAddress("jetFlavourNearest3_6", &jetFlavourNearest3_6, &b_jetFlavourNearest3_6);
   fChain->SetBranchAddress("jetFlavourHeaviest_6", &jetFlavourHeaviest_6, &b_jetFlavourHeaviest_6);
   fChain->SetBranchAddress("jetptD6", &jetptD6, &b_jetptD6);
   fChain->SetBranchAddress("jetptDNe6", &jetptDNe6, &b_jetptDNe6);
   fChain->SetBranchAddress("jetptDCh6", &jetptDCh6, &b_jetptDCh6);
   fChain->SetBranchAddress("jetnNeutral6", &jetnNeutral6, &b_jetnNeutral6);
   fChain->SetBranchAddress("jetnCharged6", &jetnCharged6, &b_jetnCharged6);
   fChain->SetBranchAddress("jetneuEMfrac6", &jetneuEMfrac6, &b_jetneuEMfrac6);
   fChain->SetBranchAddress("jetneuHadfrac6", &jetneuHadfrac6, &b_jetneuHadfrac6);
   fChain->SetBranchAddress("jetbetaClassic6", &jetbetaClassic6, &b_jetbetaClassic6);
   fChain->SetBranchAddress("jetbetaClassicStar6", &jetbetaClassicStar6, &b_jetbetaClassicStar6);
   fChain->SetBranchAddress("jetbeta6", &jetbeta6, &b_jetbeta6);
   fChain->SetBranchAddress("jetbetaStar6", &jetbetaStar6, &b_jetbetaStar6);
   fChain->SetBranchAddress("jetconstituents6", &jetconstituents6, &b_jetconstituents6);
   fChain->SetBranchAddress("jetaxis2_6", &jetaxis2_6, &b_jetaxis2_6);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased6", &jetpileupIDFlagCutBased6, &b_jetpileupIDFlagCutBased6);
   fChain->SetBranchAddress("jetpt7", &jetpt7, &b_jetpt7);
   fChain->SetBranchAddress("jeteta7", &jeteta7, &b_jeteta7);
   fChain->SetBranchAddress("jetphi7", &jetphi7, &b_jetphi7);
   fChain->SetBranchAddress("jetmass7", &jetmass7, &b_jetmass7);
   fChain->SetBranchAddress("jetAreaX7", &jetAreaX7, &b_jetAreaX7);
   fChain->SetBranchAddress("jetAreaY7", &jetAreaY7, &b_jetAreaY7);
   fChain->SetBranchAddress("jetAreaZ7", &jetAreaZ7, &b_jetAreaZ7);
   fChain->SetBranchAddress("jetAreaT7", &jetAreaT7, &b_jetAreaT7);
   fChain->SetBranchAddress("jetBTagAlgo7", &jetBTagAlgo7, &b_jetBTagAlgo7);
   fChain->SetBranchAddress("jetBTagDefault7", &jetBTagDefault7, &b_jetBTagDefault7);
   fChain->SetBranchAddress("jetBTagPhysics7", &jetBTagPhysics7, &b_jetBTagPhysics7);
   fChain->SetBranchAddress("jetBTagNearest2_7", &jetBTagNearest2_7, &b_jetBTagNearest2_7);
   fChain->SetBranchAddress("jetBTagNearest3_7", &jetBTagNearest3_7, &b_jetBTagNearest3_7);
   fChain->SetBranchAddress("jetBTagHeaviest_7", &jetBTagHeaviest_7, &b_jetBTagHeaviest_7);
   fChain->SetBranchAddress("jetFlavourAlgo7", &jetFlavourAlgo7, &b_jetFlavourAlgo7);
   fChain->SetBranchAddress("jetFlavourDefault7", &jetFlavourDefault7, &b_jetFlavourDefault7);
   fChain->SetBranchAddress("jetFlavourPhysics7", &jetFlavourPhysics7, &b_jetFlavourPhysics7);
   fChain->SetBranchAddress("jetFlavourNearest2_7", &jetFlavourNearest2_7, &b_jetFlavourNearest2_7);
   fChain->SetBranchAddress("jetFlavourNearest3_7", &jetFlavourNearest3_7, &b_jetFlavourNearest3_7);
   fChain->SetBranchAddress("jetFlavourHeaviest_7", &jetFlavourHeaviest_7, &b_jetFlavourHeaviest_7);
   fChain->SetBranchAddress("jetptD7", &jetptD7, &b_jetptD7);
   fChain->SetBranchAddress("jetptDNe7", &jetptDNe7, &b_jetptDNe7);
   fChain->SetBranchAddress("jetptDCh7", &jetptDCh7, &b_jetptDCh7);
   fChain->SetBranchAddress("jetnNeutral7", &jetnNeutral7, &b_jetnNeutral7);
   fChain->SetBranchAddress("jetnCharged7", &jetnCharged7, &b_jetnCharged7);
   fChain->SetBranchAddress("jetneuEMfrac7", &jetneuEMfrac7, &b_jetneuEMfrac7);
   fChain->SetBranchAddress("jetneuHadfrac7", &jetneuHadfrac7, &b_jetneuHadfrac7);
   fChain->SetBranchAddress("jetbetaClassic7", &jetbetaClassic7, &b_jetbetaClassic7);
   fChain->SetBranchAddress("jetbetaClassicStar7", &jetbetaClassicStar7, &b_jetbetaClassicStar7);
   fChain->SetBranchAddress("jetbeta7", &jetbeta7, &b_jetbeta7);
   fChain->SetBranchAddress("jetbetaStar7", &jetbetaStar7, &b_jetbetaStar7);
   fChain->SetBranchAddress("jetconstituents7", &jetconstituents7, &b_jetconstituents7);
   fChain->SetBranchAddress("jetaxis2_7", &jetaxis2_7, &b_jetaxis2_7);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased7", &jetpileupIDFlagCutBased7, &b_jetpileupIDFlagCutBased7);
   fChain->SetBranchAddress("jetpt8", &jetpt8, &b_jetpt8);
   fChain->SetBranchAddress("jeteta8", &jeteta8, &b_jeteta8);
   fChain->SetBranchAddress("jetphi8", &jetphi8, &b_jetphi8);
   fChain->SetBranchAddress("jetmass8", &jetmass8, &b_jetmass8);
   fChain->SetBranchAddress("jetAreaX8", &jetAreaX8, &b_jetAreaX8);
   fChain->SetBranchAddress("jetAreaY8", &jetAreaY8, &b_jetAreaY8);
   fChain->SetBranchAddress("jetAreaZ8", &jetAreaZ8, &b_jetAreaZ8);
   fChain->SetBranchAddress("jetAreaT8", &jetAreaT8, &b_jetAreaT8);
   fChain->SetBranchAddress("jetBTagAlgo8", &jetBTagAlgo8, &b_jetBTagAlgo8);
   fChain->SetBranchAddress("jetBTagDefault8", &jetBTagDefault8, &b_jetBTagDefault8);
   fChain->SetBranchAddress("jetBTagPhysics8", &jetBTagPhysics8, &b_jetBTagPhysics8);
   fChain->SetBranchAddress("jetBTagNearest2_8", &jetBTagNearest2_8, &b_jetBTagNearest2_8);
   fChain->SetBranchAddress("jetBTagNearest3_8", &jetBTagNearest3_8, &b_jetBTagNearest3_8);
   fChain->SetBranchAddress("jetBTagHeaviest_8", &jetBTagHeaviest_8, &b_jetBTagHeaviest_8);
   fChain->SetBranchAddress("jetFlavourAlgo8", &jetFlavourAlgo8, &b_jetFlavourAlgo8);
   fChain->SetBranchAddress("jetFlavourDefault8", &jetFlavourDefault8, &b_jetFlavourDefault8);
   fChain->SetBranchAddress("jetFlavourPhysics8", &jetFlavourPhysics8, &b_jetFlavourPhysics8);
   fChain->SetBranchAddress("jetFlavourNearest2_8", &jetFlavourNearest2_8, &b_jetFlavourNearest2_8);
   fChain->SetBranchAddress("jetFlavourNearest3_8", &jetFlavourNearest3_8, &b_jetFlavourNearest3_8);
   fChain->SetBranchAddress("jetFlavourHeaviest_8", &jetFlavourHeaviest_8, &b_jetFlavourHeaviest_8);
   fChain->SetBranchAddress("jetptD8", &jetptD8, &b_jetptD8);
   fChain->SetBranchAddress("jetptDNe8", &jetptDNe8, &b_jetptDNe8);
   fChain->SetBranchAddress("jetptDCh8", &jetptDCh8, &b_jetptDCh8);
   fChain->SetBranchAddress("jetnNeutral8", &jetnNeutral8, &b_jetnNeutral8);
   fChain->SetBranchAddress("jetnCharged8", &jetnCharged8, &b_jetnCharged8);
   fChain->SetBranchAddress("jetneuEMfrac8", &jetneuEMfrac8, &b_jetneuEMfrac8);
   fChain->SetBranchAddress("jetneuHadfrac8", &jetneuHadfrac8, &b_jetneuHadfrac8);
   fChain->SetBranchAddress("jetbetaClassic8", &jetbetaClassic8, &b_jetbetaClassic8);
   fChain->SetBranchAddress("jetbetaClassicStar8", &jetbetaClassicStar8, &b_jetbetaClassicStar8);
   fChain->SetBranchAddress("jetbeta8", &jetbeta8, &b_jetbeta8);
   fChain->SetBranchAddress("jetbetaStar8", &jetbetaStar8, &b_jetbetaStar8);
   fChain->SetBranchAddress("jetconstituents8", &jetconstituents8, &b_jetconstituents8);
   fChain->SetBranchAddress("jetaxis2_8", &jetaxis2_8, &b_jetaxis2_8);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased8", &jetpileupIDFlagCutBased8, &b_jetpileupIDFlagCutBased8);
   fChain->SetBranchAddress("mjj_puppi", &mjj_puppi, &b_mjj_puppi);
   fChain->SetBranchAddress("detajj_puppi", &detajj_puppi, &b_detajj_puppi);
   fChain->SetBranchAddress("njet_puppi", &njet_puppi, &b_njet_puppi);
   fChain->SetBranchAddress("nbjet_puppi", &nbjet_puppi, &b_nbjet_puppi);
   fChain->SetBranchAddress("hardbjpb_puppi", &hardbjpb_puppi, &b_hardbjpb_puppi);
   fChain->SetBranchAddress("softbjpb_puppi", &softbjpb_puppi, &b_softbjpb_puppi);
   fChain->SetBranchAddress("njetid_puppi", &njetid_puppi, &b_njetid_puppi);
   fChain->SetBranchAddress("jetpt_puppi1", &jetpt_puppi1, &b_jetpt_puppi1);
   fChain->SetBranchAddress("jeteta_puppi1", &jeteta_puppi1, &b_jeteta_puppi1);
   fChain->SetBranchAddress("jetphi_puppi1", &jetphi_puppi1, &b_jetphi_puppi1);
   fChain->SetBranchAddress("jetmass_puppi1", &jetmass_puppi1, &b_jetmass_puppi1);
   fChain->SetBranchAddress("jetAreaX_puppi1", &jetAreaX_puppi1, &b_jetAreaX_puppi1);
   fChain->SetBranchAddress("jetAreaY_puppi1", &jetAreaY_puppi1, &b_jetAreaY_puppi1);
   fChain->SetBranchAddress("jetAreaZ_puppi1", &jetAreaZ_puppi1, &b_jetAreaZ_puppi1);
   fChain->SetBranchAddress("jetAreaT_puppi1", &jetAreaT_puppi1, &b_jetAreaT_puppi1);
   fChain->SetBranchAddress("jetBTagAlgo_puppi1", &jetBTagAlgo_puppi1, &b_jetBTagAlgo_puppi1);
   fChain->SetBranchAddress("jetBTagDefault_puppi1", &jetBTagDefault_puppi1, &b_jetBTagDefault_puppi1);
   fChain->SetBranchAddress("jetBTagPhysics_puppi1", &jetBTagPhysics_puppi1, &b_jetBTagPhysics_puppi1);
   fChain->SetBranchAddress("jetBTagNearest2_puppi1", &jetBTagNearest2_puppi1, &b_jetBTagNearest2_puppi1);
   fChain->SetBranchAddress("jetBTagNearest3_puppi1", &jetBTagNearest3_puppi1, &b_jetBTagNearest3_puppi1);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi1", &jetBTagHeaviest_puppi1, &b_jetBTagHeaviest_puppi1);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi1", &jetFlavourAlgo_puppi1, &b_jetFlavourAlgo_puppi1);
   fChain->SetBranchAddress("jetFlavourDefault_puppi1", &jetFlavourDefault_puppi1, &b_jetFlavourDefault_puppi1);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi1", &jetFlavourPhysics_puppi1, &b_jetFlavourPhysics_puppi1);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi1", &jetFlavourNearest2_puppi1, &b_jetFlavourNearest2_puppi1);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi1", &jetFlavourNearest3_puppi1, &b_jetFlavourNearest3_puppi1);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi1", &jetFlavourHeaviest_puppi1, &b_jetFlavourHeaviest_puppi1);
   fChain->SetBranchAddress("jetptD_puppi1", &jetptD_puppi1, &b_jetptD_puppi1);
   fChain->SetBranchAddress("jetptDNe_puppi1", &jetptDNe_puppi1, &b_jetptDNe_puppi1);
   fChain->SetBranchAddress("jetptDCh_puppi1", &jetptDCh_puppi1, &b_jetptDCh_puppi1);
   fChain->SetBranchAddress("jetnNeutral_puppi1", &jetnNeutral_puppi1, &b_jetnNeutral_puppi1);
   fChain->SetBranchAddress("jetnCharged_puppi1", &jetnCharged_puppi1, &b_jetnCharged_puppi1);
   fChain->SetBranchAddress("jetneuEMfrac_puppi1", &jetneuEMfrac_puppi1, &b_jetneuEMfrac_puppi1);
   fChain->SetBranchAddress("jetneuHadfrac_puppi1", &jetneuHadfrac_puppi1, &b_jetneuHadfrac_puppi1);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr1", &jetbetaClassic_puppiStr1, &b_jetbetaClassic_puppiStr1);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr1", &jetbetaClassicStar_puppiStr1, &b_jetbetaClassicStar_puppiStr1);
   fChain->SetBranchAddress("jetbeta_puppiStr1", &jetbeta_puppiStr1, &b_jetbeta_puppiStr1);
   fChain->SetBranchAddress("jetbetaStar_puppiStr1", &jetbetaStar_puppiStr1, &b_jetbetaStar_puppiStr1);
   fChain->SetBranchAddress("jetconstituents_puppiStr1", &jetconstituents_puppiStr1, &b_jetconstituents_puppiStr1);
   fChain->SetBranchAddress("jetaxis2_puppiStr1", &jetaxis2_puppiStr1, &b_jetaxis2_puppiStr1);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi1", &jetpileupIDFlagCutBased_puppi1, &b_jetpileupIDFlagCutBased_puppi1);
   fChain->SetBranchAddress("jetpt_puppi2", &jetpt_puppi2, &b_jetpt_puppi2);
   fChain->SetBranchAddress("jeteta_puppi2", &jeteta_puppi2, &b_jeteta_puppi2);
   fChain->SetBranchAddress("jetphi_puppi2", &jetphi_puppi2, &b_jetphi_puppi2);
   fChain->SetBranchAddress("jetmass_puppi2", &jetmass_puppi2, &b_jetmass_puppi2);
   fChain->SetBranchAddress("jetAreaX_puppi2", &jetAreaX_puppi2, &b_jetAreaX_puppi2);
   fChain->SetBranchAddress("jetAreaY_puppi2", &jetAreaY_puppi2, &b_jetAreaY_puppi2);
   fChain->SetBranchAddress("jetAreaZ_puppi2", &jetAreaZ_puppi2, &b_jetAreaZ_puppi2);
   fChain->SetBranchAddress("jetAreaT_puppi2", &jetAreaT_puppi2, &b_jetAreaT_puppi2);
   fChain->SetBranchAddress("jetBTagAlgo_puppi2", &jetBTagAlgo_puppi2, &b_jetBTagAlgo_puppi2);
   fChain->SetBranchAddress("jetBTagDefault_puppi2", &jetBTagDefault_puppi2, &b_jetBTagDefault_puppi2);
   fChain->SetBranchAddress("jetBTagPhysics_puppi2", &jetBTagPhysics_puppi2, &b_jetBTagPhysics_puppi2);
   fChain->SetBranchAddress("jetBTagNearest2_puppi2", &jetBTagNearest2_puppi2, &b_jetBTagNearest2_puppi2);
   fChain->SetBranchAddress("jetBTagNearest3_puppi2", &jetBTagNearest3_puppi2, &b_jetBTagNearest3_puppi2);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi2", &jetBTagHeaviest_puppi2, &b_jetBTagHeaviest_puppi2);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi2", &jetFlavourAlgo_puppi2, &b_jetFlavourAlgo_puppi2);
   fChain->SetBranchAddress("jetFlavourDefault_puppi2", &jetFlavourDefault_puppi2, &b_jetFlavourDefault_puppi2);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi2", &jetFlavourPhysics_puppi2, &b_jetFlavourPhysics_puppi2);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi2", &jetFlavourNearest2_puppi2, &b_jetFlavourNearest2_puppi2);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi2", &jetFlavourNearest3_puppi2, &b_jetFlavourNearest3_puppi2);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi2", &jetFlavourHeaviest_puppi2, &b_jetFlavourHeaviest_puppi2);
   fChain->SetBranchAddress("jetptD_puppi2", &jetptD_puppi2, &b_jetptD_puppi2);
   fChain->SetBranchAddress("jetptDNe_puppi2", &jetptDNe_puppi2, &b_jetptDNe_puppi2);
   fChain->SetBranchAddress("jetptDCh_puppi2", &jetptDCh_puppi2, &b_jetptDCh_puppi2);
   fChain->SetBranchAddress("jetnNeutral_puppi2", &jetnNeutral_puppi2, &b_jetnNeutral_puppi2);
   fChain->SetBranchAddress("jetnCharged_puppi2", &jetnCharged_puppi2, &b_jetnCharged_puppi2);
   fChain->SetBranchAddress("jetneuEMfrac_puppi2", &jetneuEMfrac_puppi2, &b_jetneuEMfrac_puppi2);
   fChain->SetBranchAddress("jetneuHadfrac_puppi2", &jetneuHadfrac_puppi2, &b_jetneuHadfrac_puppi2);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr2", &jetbetaClassic_puppiStr2, &b_jetbetaClassic_puppiStr2);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr2", &jetbetaClassicStar_puppiStr2, &b_jetbetaClassicStar_puppiStr2);
   fChain->SetBranchAddress("jetbeta_puppiStr2", &jetbeta_puppiStr2, &b_jetbeta_puppiStr2);
   fChain->SetBranchAddress("jetbetaStar_puppiStr2", &jetbetaStar_puppiStr2, &b_jetbetaStar_puppiStr2);
   fChain->SetBranchAddress("jetconstituents_puppiStr2", &jetconstituents_puppiStr2, &b_jetconstituents_puppiStr2);
   fChain->SetBranchAddress("jetaxis2_puppiStr2", &jetaxis2_puppiStr2, &b_jetaxis2_puppiStr2);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi2", &jetpileupIDFlagCutBased_puppi2, &b_jetpileupIDFlagCutBased_puppi2);
   fChain->SetBranchAddress("jetpt_puppi3", &jetpt_puppi3, &b_jetpt_puppi3);
   fChain->SetBranchAddress("jeteta_puppi3", &jeteta_puppi3, &b_jeteta_puppi3);
   fChain->SetBranchAddress("jetphi_puppi3", &jetphi_puppi3, &b_jetphi_puppi3);
   fChain->SetBranchAddress("jetmass_puppi3", &jetmass_puppi3, &b_jetmass_puppi3);
   fChain->SetBranchAddress("jetAreaX_puppi3", &jetAreaX_puppi3, &b_jetAreaX_puppi3);
   fChain->SetBranchAddress("jetAreaY_puppi3", &jetAreaY_puppi3, &b_jetAreaY_puppi3);
   fChain->SetBranchAddress("jetAreaZ_puppi3", &jetAreaZ_puppi3, &b_jetAreaZ_puppi3);
   fChain->SetBranchAddress("jetAreaT_puppi3", &jetAreaT_puppi3, &b_jetAreaT_puppi3);
   fChain->SetBranchAddress("jetBTagAlgo_puppi3", &jetBTagAlgo_puppi3, &b_jetBTagAlgo_puppi3);
   fChain->SetBranchAddress("jetBTagDefault_puppi3", &jetBTagDefault_puppi3, &b_jetBTagDefault_puppi3);
   fChain->SetBranchAddress("jetBTagPhysics_puppi3", &jetBTagPhysics_puppi3, &b_jetBTagPhysics_puppi3);
   fChain->SetBranchAddress("jetBTagNearest2_puppi3", &jetBTagNearest2_puppi3, &b_jetBTagNearest2_puppi3);
   fChain->SetBranchAddress("jetBTagNearest3_puppi3", &jetBTagNearest3_puppi3, &b_jetBTagNearest3_puppi3);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi3", &jetBTagHeaviest_puppi3, &b_jetBTagHeaviest_puppi3);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi3", &jetFlavourAlgo_puppi3, &b_jetFlavourAlgo_puppi3);
   fChain->SetBranchAddress("jetFlavourDefault_puppi3", &jetFlavourDefault_puppi3, &b_jetFlavourDefault_puppi3);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi3", &jetFlavourPhysics_puppi3, &b_jetFlavourPhysics_puppi3);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi3", &jetFlavourNearest2_puppi3, &b_jetFlavourNearest2_puppi3);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi3", &jetFlavourNearest3_puppi3, &b_jetFlavourNearest3_puppi3);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi3", &jetFlavourHeaviest_puppi3, &b_jetFlavourHeaviest_puppi3);
   fChain->SetBranchAddress("jetptD_puppi3", &jetptD_puppi3, &b_jetptD_puppi3);
   fChain->SetBranchAddress("jetptDNe_puppi3", &jetptDNe_puppi3, &b_jetptDNe_puppi3);
   fChain->SetBranchAddress("jetptDCh_puppi3", &jetptDCh_puppi3, &b_jetptDCh_puppi3);
   fChain->SetBranchAddress("jetnNeutral_puppi3", &jetnNeutral_puppi3, &b_jetnNeutral_puppi3);
   fChain->SetBranchAddress("jetnCharged_puppi3", &jetnCharged_puppi3, &b_jetnCharged_puppi3);
   fChain->SetBranchAddress("jetneuEMfrac_puppi3", &jetneuEMfrac_puppi3, &b_jetneuEMfrac_puppi3);
   fChain->SetBranchAddress("jetneuHadfrac_puppi3", &jetneuHadfrac_puppi3, &b_jetneuHadfrac_puppi3);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr3", &jetbetaClassic_puppiStr3, &b_jetbetaClassic_puppiStr3);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr3", &jetbetaClassicStar_puppiStr3, &b_jetbetaClassicStar_puppiStr3);
   fChain->SetBranchAddress("jetbeta_puppiStr3", &jetbeta_puppiStr3, &b_jetbeta_puppiStr3);
   fChain->SetBranchAddress("jetbetaStar_puppiStr3", &jetbetaStar_puppiStr3, &b_jetbetaStar_puppiStr3);
   fChain->SetBranchAddress("jetconstituents_puppiStr3", &jetconstituents_puppiStr3, &b_jetconstituents_puppiStr3);
   fChain->SetBranchAddress("jetaxis2_puppiStr3", &jetaxis2_puppiStr3, &b_jetaxis2_puppiStr3);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi3", &jetpileupIDFlagCutBased_puppi3, &b_jetpileupIDFlagCutBased_puppi3);
   fChain->SetBranchAddress("jetpt_puppi4", &jetpt_puppi4, &b_jetpt_puppi4);
   fChain->SetBranchAddress("jeteta_puppi4", &jeteta_puppi4, &b_jeteta_puppi4);
   fChain->SetBranchAddress("jetphi_puppi4", &jetphi_puppi4, &b_jetphi_puppi4);
   fChain->SetBranchAddress("jetmass_puppi4", &jetmass_puppi4, &b_jetmass_puppi4);
   fChain->SetBranchAddress("jetAreaX_puppi4", &jetAreaX_puppi4, &b_jetAreaX_puppi4);
   fChain->SetBranchAddress("jetAreaY_puppi4", &jetAreaY_puppi4, &b_jetAreaY_puppi4);
   fChain->SetBranchAddress("jetAreaZ_puppi4", &jetAreaZ_puppi4, &b_jetAreaZ_puppi4);
   fChain->SetBranchAddress("jetAreaT_puppi4", &jetAreaT_puppi4, &b_jetAreaT_puppi4);
   fChain->SetBranchAddress("jetBTagAlgo_puppi4", &jetBTagAlgo_puppi4, &b_jetBTagAlgo_puppi4);
   fChain->SetBranchAddress("jetBTagDefault_puppi4", &jetBTagDefault_puppi4, &b_jetBTagDefault_puppi4);
   fChain->SetBranchAddress("jetBTagPhysics_puppi4", &jetBTagPhysics_puppi4, &b_jetBTagPhysics_puppi4);
   fChain->SetBranchAddress("jetBTagNearest2_puppi4", &jetBTagNearest2_puppi4, &b_jetBTagNearest2_puppi4);
   fChain->SetBranchAddress("jetBTagNearest3_puppi4", &jetBTagNearest3_puppi4, &b_jetBTagNearest3_puppi4);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi4", &jetBTagHeaviest_puppi4, &b_jetBTagHeaviest_puppi4);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi4", &jetFlavourAlgo_puppi4, &b_jetFlavourAlgo_puppi4);
   fChain->SetBranchAddress("jetFlavourDefault_puppi4", &jetFlavourDefault_puppi4, &b_jetFlavourDefault_puppi4);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi4", &jetFlavourPhysics_puppi4, &b_jetFlavourPhysics_puppi4);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi4", &jetFlavourNearest2_puppi4, &b_jetFlavourNearest2_puppi4);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi4", &jetFlavourNearest3_puppi4, &b_jetFlavourNearest3_puppi4);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi4", &jetFlavourHeaviest_puppi4, &b_jetFlavourHeaviest_puppi4);
   fChain->SetBranchAddress("jetptD_puppi4", &jetptD_puppi4, &b_jetptD_puppi4);
   fChain->SetBranchAddress("jetptDNe_puppi4", &jetptDNe_puppi4, &b_jetptDNe_puppi4);
   fChain->SetBranchAddress("jetptDCh_puppi4", &jetptDCh_puppi4, &b_jetptDCh_puppi4);
   fChain->SetBranchAddress("jetnNeutral_puppi4", &jetnNeutral_puppi4, &b_jetnNeutral_puppi4);
   fChain->SetBranchAddress("jetnCharged_puppi4", &jetnCharged_puppi4, &b_jetnCharged_puppi4);
   fChain->SetBranchAddress("jetneuEMfrac_puppi4", &jetneuEMfrac_puppi4, &b_jetneuEMfrac_puppi4);
   fChain->SetBranchAddress("jetneuHadfrac_puppi4", &jetneuHadfrac_puppi4, &b_jetneuHadfrac_puppi4);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr4", &jetbetaClassic_puppiStr4, &b_jetbetaClassic_puppiStr4);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr4", &jetbetaClassicStar_puppiStr4, &b_jetbetaClassicStar_puppiStr4);
   fChain->SetBranchAddress("jetbeta_puppiStr4", &jetbeta_puppiStr4, &b_jetbeta_puppiStr4);
   fChain->SetBranchAddress("jetbetaStar_puppiStr4", &jetbetaStar_puppiStr4, &b_jetbetaStar_puppiStr4);
   fChain->SetBranchAddress("jetconstituents_puppiStr4", &jetconstituents_puppiStr4, &b_jetconstituents_puppiStr4);
   fChain->SetBranchAddress("jetaxis2_puppiStr4", &jetaxis2_puppiStr4, &b_jetaxis2_puppiStr4);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi4", &jetpileupIDFlagCutBased_puppi4, &b_jetpileupIDFlagCutBased_puppi4);
   fChain->SetBranchAddress("jetpt_puppi5", &jetpt_puppi5, &b_jetpt_puppi5);
   fChain->SetBranchAddress("jeteta_puppi5", &jeteta_puppi5, &b_jeteta_puppi5);
   fChain->SetBranchAddress("jetphi_puppi5", &jetphi_puppi5, &b_jetphi_puppi5);
   fChain->SetBranchAddress("jetmass_puppi5", &jetmass_puppi5, &b_jetmass_puppi5);
   fChain->SetBranchAddress("jetAreaX_puppi5", &jetAreaX_puppi5, &b_jetAreaX_puppi5);
   fChain->SetBranchAddress("jetAreaY_puppi5", &jetAreaY_puppi5, &b_jetAreaY_puppi5);
   fChain->SetBranchAddress("jetAreaZ_puppi5", &jetAreaZ_puppi5, &b_jetAreaZ_puppi5);
   fChain->SetBranchAddress("jetAreaT_puppi5", &jetAreaT_puppi5, &b_jetAreaT_puppi5);
   fChain->SetBranchAddress("jetBTagAlgo_puppi5", &jetBTagAlgo_puppi5, &b_jetBTagAlgo_puppi5);
   fChain->SetBranchAddress("jetBTagDefault_puppi5", &jetBTagDefault_puppi5, &b_jetBTagDefault_puppi5);
   fChain->SetBranchAddress("jetBTagPhysics_puppi5", &jetBTagPhysics_puppi5, &b_jetBTagPhysics_puppi5);
   fChain->SetBranchAddress("jetBTagNearest2_puppi5", &jetBTagNearest2_puppi5, &b_jetBTagNearest2_puppi5);
   fChain->SetBranchAddress("jetBTagNearest3_puppi5", &jetBTagNearest3_puppi5, &b_jetBTagNearest3_puppi5);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi5", &jetBTagHeaviest_puppi5, &b_jetBTagHeaviest_puppi5);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi5", &jetFlavourAlgo_puppi5, &b_jetFlavourAlgo_puppi5);
   fChain->SetBranchAddress("jetFlavourDefault_puppi5", &jetFlavourDefault_puppi5, &b_jetFlavourDefault_puppi5);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi5", &jetFlavourPhysics_puppi5, &b_jetFlavourPhysics_puppi5);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi5", &jetFlavourNearest2_puppi5, &b_jetFlavourNearest2_puppi5);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi5", &jetFlavourNearest3_puppi5, &b_jetFlavourNearest3_puppi5);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi5", &jetFlavourHeaviest_puppi5, &b_jetFlavourHeaviest_puppi5);
   fChain->SetBranchAddress("jetptD_puppi5", &jetptD_puppi5, &b_jetptD_puppi5);
   fChain->SetBranchAddress("jetptDNe_puppi5", &jetptDNe_puppi5, &b_jetptDNe_puppi5);
   fChain->SetBranchAddress("jetptDCh_puppi5", &jetptDCh_puppi5, &b_jetptDCh_puppi5);
   fChain->SetBranchAddress("jetnNeutral_puppi5", &jetnNeutral_puppi5, &b_jetnNeutral_puppi5);
   fChain->SetBranchAddress("jetnCharged_puppi5", &jetnCharged_puppi5, &b_jetnCharged_puppi5);
   fChain->SetBranchAddress("jetneuEMfrac_puppi5", &jetneuEMfrac_puppi5, &b_jetneuEMfrac_puppi5);
   fChain->SetBranchAddress("jetneuHadfrac_puppi5", &jetneuHadfrac_puppi5, &b_jetneuHadfrac_puppi5);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr5", &jetbetaClassic_puppiStr5, &b_jetbetaClassic_puppiStr5);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr5", &jetbetaClassicStar_puppiStr5, &b_jetbetaClassicStar_puppiStr5);
   fChain->SetBranchAddress("jetbeta_puppiStr5", &jetbeta_puppiStr5, &b_jetbeta_puppiStr5);
   fChain->SetBranchAddress("jetbetaStar_puppiStr5", &jetbetaStar_puppiStr5, &b_jetbetaStar_puppiStr5);
   fChain->SetBranchAddress("jetconstituents_puppiStr5", &jetconstituents_puppiStr5, &b_jetconstituents_puppiStr5);
   fChain->SetBranchAddress("jetaxis2_puppiStr5", &jetaxis2_puppiStr5, &b_jetaxis2_puppiStr5);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi5", &jetpileupIDFlagCutBased_puppi5, &b_jetpileupIDFlagCutBased_puppi5);
   fChain->SetBranchAddress("jetpt_puppi6", &jetpt_puppi6, &b_jetpt_puppi6);
   fChain->SetBranchAddress("jeteta_puppi6", &jeteta_puppi6, &b_jeteta_puppi6);
   fChain->SetBranchAddress("jetphi_puppi6", &jetphi_puppi6, &b_jetphi_puppi6);
   fChain->SetBranchAddress("jetmass_puppi6", &jetmass_puppi6, &b_jetmass_puppi6);
   fChain->SetBranchAddress("jetAreaX_puppi6", &jetAreaX_puppi6, &b_jetAreaX_puppi6);
   fChain->SetBranchAddress("jetAreaY_puppi6", &jetAreaY_puppi6, &b_jetAreaY_puppi6);
   fChain->SetBranchAddress("jetAreaZ_puppi6", &jetAreaZ_puppi6, &b_jetAreaZ_puppi6);
   fChain->SetBranchAddress("jetAreaT_puppi6", &jetAreaT_puppi6, &b_jetAreaT_puppi6);
   fChain->SetBranchAddress("jetBTagAlgo_puppi6", &jetBTagAlgo_puppi6, &b_jetBTagAlgo_puppi6);
   fChain->SetBranchAddress("jetBTagDefault_puppi6", &jetBTagDefault_puppi6, &b_jetBTagDefault_puppi6);
   fChain->SetBranchAddress("jetBTagPhysics_puppi6", &jetBTagPhysics_puppi6, &b_jetBTagPhysics_puppi6);
   fChain->SetBranchAddress("jetBTagNearest2_puppi6", &jetBTagNearest2_puppi6, &b_jetBTagNearest2_puppi6);
   fChain->SetBranchAddress("jetBTagNearest3_puppi6", &jetBTagNearest3_puppi6, &b_jetBTagNearest3_puppi6);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi6", &jetBTagHeaviest_puppi6, &b_jetBTagHeaviest_puppi6);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi6", &jetFlavourAlgo_puppi6, &b_jetFlavourAlgo_puppi6);
   fChain->SetBranchAddress("jetFlavourDefault_puppi6", &jetFlavourDefault_puppi6, &b_jetFlavourDefault_puppi6);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi6", &jetFlavourPhysics_puppi6, &b_jetFlavourPhysics_puppi6);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi6", &jetFlavourNearest2_puppi6, &b_jetFlavourNearest2_puppi6);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi6", &jetFlavourNearest3_puppi6, &b_jetFlavourNearest3_puppi6);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi6", &jetFlavourHeaviest_puppi6, &b_jetFlavourHeaviest_puppi6);
   fChain->SetBranchAddress("jetptD_puppi6", &jetptD_puppi6, &b_jetptD_puppi6);
   fChain->SetBranchAddress("jetptDNe_puppi6", &jetptDNe_puppi6, &b_jetptDNe_puppi6);
   fChain->SetBranchAddress("jetptDCh_puppi6", &jetptDCh_puppi6, &b_jetptDCh_puppi6);
   fChain->SetBranchAddress("jetnNeutral_puppi6", &jetnNeutral_puppi6, &b_jetnNeutral_puppi6);
   fChain->SetBranchAddress("jetnCharged_puppi6", &jetnCharged_puppi6, &b_jetnCharged_puppi6);
   fChain->SetBranchAddress("jetneuEMfrac_puppi6", &jetneuEMfrac_puppi6, &b_jetneuEMfrac_puppi6);
   fChain->SetBranchAddress("jetneuHadfrac_puppi6", &jetneuHadfrac_puppi6, &b_jetneuHadfrac_puppi6);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr6", &jetbetaClassic_puppiStr6, &b_jetbetaClassic_puppiStr6);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr6", &jetbetaClassicStar_puppiStr6, &b_jetbetaClassicStar_puppiStr6);
   fChain->SetBranchAddress("jetbeta_puppiStr6", &jetbeta_puppiStr6, &b_jetbeta_puppiStr6);
   fChain->SetBranchAddress("jetbetaStar_puppiStr6", &jetbetaStar_puppiStr6, &b_jetbetaStar_puppiStr6);
   fChain->SetBranchAddress("jetconstituents_puppiStr6", &jetconstituents_puppiStr6, &b_jetconstituents_puppiStr6);
   fChain->SetBranchAddress("jetaxis2_puppiStr6", &jetaxis2_puppiStr6, &b_jetaxis2_puppiStr6);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi6", &jetpileupIDFlagCutBased_puppi6, &b_jetpileupIDFlagCutBased_puppi6);
   fChain->SetBranchAddress("jetpt_puppi7", &jetpt_puppi7, &b_jetpt_puppi7);
   fChain->SetBranchAddress("jeteta_puppi7", &jeteta_puppi7, &b_jeteta_puppi7);
   fChain->SetBranchAddress("jetphi_puppi7", &jetphi_puppi7, &b_jetphi_puppi7);
   fChain->SetBranchAddress("jetmass_puppi7", &jetmass_puppi7, &b_jetmass_puppi7);
   fChain->SetBranchAddress("jetAreaX_puppi7", &jetAreaX_puppi7, &b_jetAreaX_puppi7);
   fChain->SetBranchAddress("jetAreaY_puppi7", &jetAreaY_puppi7, &b_jetAreaY_puppi7);
   fChain->SetBranchAddress("jetAreaZ_puppi7", &jetAreaZ_puppi7, &b_jetAreaZ_puppi7);
   fChain->SetBranchAddress("jetAreaT_puppi7", &jetAreaT_puppi7, &b_jetAreaT_puppi7);
   fChain->SetBranchAddress("jetBTagAlgo_puppi7", &jetBTagAlgo_puppi7, &b_jetBTagAlgo_puppi7);
   fChain->SetBranchAddress("jetBTagDefault_puppi7", &jetBTagDefault_puppi7, &b_jetBTagDefault_puppi7);
   fChain->SetBranchAddress("jetBTagPhysics_puppi7", &jetBTagPhysics_puppi7, &b_jetBTagPhysics_puppi7);
   fChain->SetBranchAddress("jetBTagNearest2_puppi7", &jetBTagNearest2_puppi7, &b_jetBTagNearest2_puppi7);
   fChain->SetBranchAddress("jetBTagNearest3_puppi7", &jetBTagNearest3_puppi7, &b_jetBTagNearest3_puppi7);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi7", &jetBTagHeaviest_puppi7, &b_jetBTagHeaviest_puppi7);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi7", &jetFlavourAlgo_puppi7, &b_jetFlavourAlgo_puppi7);
   fChain->SetBranchAddress("jetFlavourDefault_puppi7", &jetFlavourDefault_puppi7, &b_jetFlavourDefault_puppi7);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi7", &jetFlavourPhysics_puppi7, &b_jetFlavourPhysics_puppi7);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi7", &jetFlavourNearest2_puppi7, &b_jetFlavourNearest2_puppi7);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi7", &jetFlavourNearest3_puppi7, &b_jetFlavourNearest3_puppi7);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi7", &jetFlavourHeaviest_puppi7, &b_jetFlavourHeaviest_puppi7);
   fChain->SetBranchAddress("jetptD_puppi7", &jetptD_puppi7, &b_jetptD_puppi7);
   fChain->SetBranchAddress("jetptDNe_puppi7", &jetptDNe_puppi7, &b_jetptDNe_puppi7);
   fChain->SetBranchAddress("jetptDCh_puppi7", &jetptDCh_puppi7, &b_jetptDCh_puppi7);
   fChain->SetBranchAddress("jetnNeutral_puppi7", &jetnNeutral_puppi7, &b_jetnNeutral_puppi7);
   fChain->SetBranchAddress("jetnCharged_puppi7", &jetnCharged_puppi7, &b_jetnCharged_puppi7);
   fChain->SetBranchAddress("jetneuEMfrac_puppi7", &jetneuEMfrac_puppi7, &b_jetneuEMfrac_puppi7);
   fChain->SetBranchAddress("jetneuHadfrac_puppi7", &jetneuHadfrac_puppi7, &b_jetneuHadfrac_puppi7);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr7", &jetbetaClassic_puppiStr7, &b_jetbetaClassic_puppiStr7);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr7", &jetbetaClassicStar_puppiStr7, &b_jetbetaClassicStar_puppiStr7);
   fChain->SetBranchAddress("jetbeta_puppiStr7", &jetbeta_puppiStr7, &b_jetbeta_puppiStr7);
   fChain->SetBranchAddress("jetbetaStar_puppiStr7", &jetbetaStar_puppiStr7, &b_jetbetaStar_puppiStr7);
   fChain->SetBranchAddress("jetconstituents_puppiStr7", &jetconstituents_puppiStr7, &b_jetconstituents_puppiStr7);
   fChain->SetBranchAddress("jetaxis2_puppiStr7", &jetaxis2_puppiStr7, &b_jetaxis2_puppiStr7);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi7", &jetpileupIDFlagCutBased_puppi7, &b_jetpileupIDFlagCutBased_puppi7);
   fChain->SetBranchAddress("jetpt_puppi8", &jetpt_puppi8, &b_jetpt_puppi8);
   fChain->SetBranchAddress("jeteta_puppi8", &jeteta_puppi8, &b_jeteta_puppi8);
   fChain->SetBranchAddress("jetphi_puppi8", &jetphi_puppi8, &b_jetphi_puppi8);
   fChain->SetBranchAddress("jetmass_puppi8", &jetmass_puppi8, &b_jetmass_puppi8);
   fChain->SetBranchAddress("jetAreaX_puppi8", &jetAreaX_puppi8, &b_jetAreaX_puppi8);
   fChain->SetBranchAddress("jetAreaY_puppi8", &jetAreaY_puppi8, &b_jetAreaY_puppi8);
   fChain->SetBranchAddress("jetAreaZ_puppi8", &jetAreaZ_puppi8, &b_jetAreaZ_puppi8);
   fChain->SetBranchAddress("jetAreaT_puppi8", &jetAreaT_puppi8, &b_jetAreaT_puppi8);
   fChain->SetBranchAddress("jetBTagAlgo_puppi8", &jetBTagAlgo_puppi8, &b_jetBTagAlgo_puppi8);
   fChain->SetBranchAddress("jetBTagDefault_puppi8", &jetBTagDefault_puppi8, &b_jetBTagDefault_puppi8);
   fChain->SetBranchAddress("jetBTagPhysics_puppi8", &jetBTagPhysics_puppi8, &b_jetBTagPhysics_puppi8);
   fChain->SetBranchAddress("jetBTagNearest2_puppi8", &jetBTagNearest2_puppi8, &b_jetBTagNearest2_puppi8);
   fChain->SetBranchAddress("jetBTagNearest3_puppi8", &jetBTagNearest3_puppi8, &b_jetBTagNearest3_puppi8);
   fChain->SetBranchAddress("jetBTagHeaviest_puppi8", &jetBTagHeaviest_puppi8, &b_jetBTagHeaviest_puppi8);
   fChain->SetBranchAddress("jetFlavourAlgo_puppi8", &jetFlavourAlgo_puppi8, &b_jetFlavourAlgo_puppi8);
   fChain->SetBranchAddress("jetFlavourDefault_puppi8", &jetFlavourDefault_puppi8, &b_jetFlavourDefault_puppi8);
   fChain->SetBranchAddress("jetFlavourPhysics_puppi8", &jetFlavourPhysics_puppi8, &b_jetFlavourPhysics_puppi8);
   fChain->SetBranchAddress("jetFlavourNearest2_puppi8", &jetFlavourNearest2_puppi8, &b_jetFlavourNearest2_puppi8);
   fChain->SetBranchAddress("jetFlavourNearest3_puppi8", &jetFlavourNearest3_puppi8, &b_jetFlavourNearest3_puppi8);
   fChain->SetBranchAddress("jetFlavourHeaviest_puppi8", &jetFlavourHeaviest_puppi8, &b_jetFlavourHeaviest_puppi8);
   fChain->SetBranchAddress("jetptD_puppi8", &jetptD_puppi8, &b_jetptD_puppi8);
   fChain->SetBranchAddress("jetptDNe_puppi8", &jetptDNe_puppi8, &b_jetptDNe_puppi8);
   fChain->SetBranchAddress("jetptDCh_puppi8", &jetptDCh_puppi8, &b_jetptDCh_puppi8);
   fChain->SetBranchAddress("jetnNeutral_puppi8", &jetnNeutral_puppi8, &b_jetnNeutral_puppi8);
   fChain->SetBranchAddress("jetnCharged_puppi8", &jetnCharged_puppi8, &b_jetnCharged_puppi8);
   fChain->SetBranchAddress("jetneuEMfrac_puppi8", &jetneuEMfrac_puppi8, &b_jetneuEMfrac_puppi8);
   fChain->SetBranchAddress("jetneuHadfrac_puppi8", &jetneuHadfrac_puppi8, &b_jetneuHadfrac_puppi8);
   fChain->SetBranchAddress("jetbetaClassic_puppiStr8", &jetbetaClassic_puppiStr8, &b_jetbetaClassic_puppiStr8);
   fChain->SetBranchAddress("jetbetaClassicStar_puppiStr8", &jetbetaClassicStar_puppiStr8, &b_jetbetaClassicStar_puppiStr8);
   fChain->SetBranchAddress("jetbeta_puppiStr8", &jetbeta_puppiStr8, &b_jetbeta_puppiStr8);
   fChain->SetBranchAddress("jetbetaStar_puppiStr8", &jetbetaStar_puppiStr8, &b_jetbetaStar_puppiStr8);
   fChain->SetBranchAddress("jetconstituents_puppiStr8", &jetconstituents_puppiStr8, &b_jetconstituents_puppiStr8);
   fChain->SetBranchAddress("jetaxis2_puppiStr8", &jetaxis2_puppiStr8, &b_jetaxis2_puppiStr8);
   fChain->SetBranchAddress("jetpileupIDFlagCutBased_puppi8", &jetpileupIDFlagCutBased_puppi8, &b_jetpileupIDFlagCutBased_puppi8);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("ptll", &ptll, &b_ptll);
   fChain->SetBranchAddress("dPhill", &dPhill, &b_dPhill);
   fChain->SetBranchAddress("dRll", &dRll, &b_dRll);
   fChain->SetBranchAddress("dEtall", &dEtall, &b_dEtall);
   fChain->SetBranchAddress("etall", &etall, &b_etall);
   fChain->SetBranchAddress("yll", &yll, &b_yll);
   fChain->SetBranchAddress("nextra", &nextra, &b_nextra);
   fChain->SetBranchAddress("nlepton", &nlepton, &b_nlepton);
   fChain->SetBranchAddress("sameflav", &sameflav, &b_sameflav);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("ch1", &ch1, &b_ch1);
   fChain->SetBranchAddress("pid1", &pid1, &b_pid1);
   fChain->SetBranchAddress("iso1", &iso1, &b_iso1);
   fChain->SetBranchAddress("isoDBeta1", &isoDBeta1, &b_isoDBeta1);
   fChain->SetBranchAddress("isoRhoCorr1", &isoRhoCorr1, &b_isoRhoCorr1);
   fChain->SetBranchAddress("sumChargedHadron1", &sumChargedHadron1, &b_sumChargedHadron1);
   fChain->SetBranchAddress("sumNeutral1", &sumNeutral1, &b_sumNeutral1);
   fChain->SetBranchAddress("sumChargedPU1", &sumChargedPU1, &b_sumChargedPU1);
   fChain->SetBranchAddress("sumAllParticles1", &sumAllParticles1, &b_sumAllParticles1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt2);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("ch2", &ch2, &b_ch2);
   fChain->SetBranchAddress("pid2", &pid2, &b_pid2);
   fChain->SetBranchAddress("iso2", &iso2, &b_iso2);
   fChain->SetBranchAddress("isoDBeta2", &isoDBeta2, &b_isoDBeta2);
   fChain->SetBranchAddress("isoRhoCorr2", &isoRhoCorr2, &b_isoRhoCorr2);
   fChain->SetBranchAddress("sumChargedHadron2", &sumChargedHadron2, &b_sumChargedHadron2);
   fChain->SetBranchAddress("sumNeutral2", &sumNeutral2, &b_sumNeutral2);
   fChain->SetBranchAddress("sumChargedPU2", &sumChargedPU2, &b_sumChargedPU2);
   fChain->SetBranchAddress("sumAllParticles2", &sumAllParticles2, &b_sumAllParticles2);
   fChain->SetBranchAddress("pt3", &pt3, &b_pt3);
   fChain->SetBranchAddress("eta3", &eta3, &b_eta3);
   fChain->SetBranchAddress("phi3", &phi3, &b_phi3);
   fChain->SetBranchAddress("ch3", &ch3, &b_ch3);
   fChain->SetBranchAddress("pid3", &pid3, &b_pid3);
   fChain->SetBranchAddress("iso3", &iso3, &b_iso3);
   fChain->SetBranchAddress("isoDBeta3", &isoDBeta3, &b_isoDBeta3);
   fChain->SetBranchAddress("isoRhoCorr3", &isoRhoCorr3, &b_isoRhoCorr3);
   fChain->SetBranchAddress("sumChargedHadron3", &sumChargedHadron3, &b_sumChargedHadron3);
   fChain->SetBranchAddress("sumNeutral3", &sumNeutral3, &b_sumNeutral3);
   fChain->SetBranchAddress("sumChargedPU3", &sumChargedPU3, &b_sumChargedPU3);
   fChain->SetBranchAddress("sumAllParticles3", &sumAllParticles3, &b_sumAllParticles3);
   fChain->SetBranchAddress("pt4", &pt4, &b_pt4);
   fChain->SetBranchAddress("eta4", &eta4, &b_eta4);
   fChain->SetBranchAddress("phi4", &phi4, &b_phi4);
   fChain->SetBranchAddress("ch4", &ch4, &b_ch4);
   fChain->SetBranchAddress("pid4", &pid4, &b_pid4);
   fChain->SetBranchAddress("iso4", &iso4, &b_iso4);
   fChain->SetBranchAddress("isoDBeta4", &isoDBeta4, &b_isoDBeta4);
   fChain->SetBranchAddress("isoRhoCorr4", &isoRhoCorr4, &b_isoRhoCorr4);
   fChain->SetBranchAddress("sumChargedHadron4", &sumChargedHadron4, &b_sumChargedHadron4);
   fChain->SetBranchAddress("sumNeutral4", &sumNeutral4, &b_sumNeutral4);
   fChain->SetBranchAddress("sumChargedPU4", &sumChargedPU4, &b_sumChargedPU4);
   fChain->SetBranchAddress("sumAllParticles4", &sumAllParticles4, &b_sumAllParticles4);
   fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
   fChain->SetBranchAddress("pfmetphi", &pfmetphi, &b_pfmetphi);
   fChain->SetBranchAddress("metGenpt", &metGenpt, &b_metGenpt);
   fChain->SetBranchAddress("metGenphi", &metGenphi, &b_metGenphi);
   fChain->SetBranchAddress("pfmet_puppi", &pfmet_puppi, &b_pfmet_puppi);
   fChain->SetBranchAddress("pfmetphi_puppi", &pfmetphi_puppi, &b_pfmetphi_puppi);
   fChain->SetBranchAddress("npu", &npu, &b_npu);
   fChain->SetBranchAddress("globalRhokt4", &globalRhokt4, &b_globalRhokt4);
   fChain->SetBranchAddress("globalGridFastJet", &globalGridFastJet, &b_globalGridFastJet);
   fChain->SetBranchAddress("Rhokt4_0", &Rhokt4_0, &b_Rhokt4_0);
   fChain->SetBranchAddress("Rhokt4_1", &Rhokt4_1, &b_Rhokt4_1);
   fChain->SetBranchAddress("Rhokt4_2", &Rhokt4_2, &b_Rhokt4_2);
   fChain->SetBranchAddress("RhoGridFastJet_0", &RhoGridFastJet_0, &b_RhoGridFastJet_0);
   fChain->SetBranchAddress("RhoGridFastJet_1", &RhoGridFastJet_1, &b_RhoGridFastJet_1);
   fChain->SetBranchAddress("RhoGridFastJet_2", &RhoGridFastJet_2, &b_RhoGridFastJet_2);
   fChain->SetBranchAddress("PuppiRhokt4_0", &PuppiRhokt4_0, &b_PuppiRhokt4_0);
   fChain->SetBranchAddress("PuppiRhokt4_1", &PuppiRhokt4_1, &b_PuppiRhokt4_1);
   fChain->SetBranchAddress("PuppiRhoGridFastJet_0", &PuppiRhoGridFastJet_0, &b_PuppiRhoGridFastJet_0);
   fChain->SetBranchAddress("PuppiRhoGridFastJet_1", &PuppiRhoGridFastJet_1, &b_PuppiRhoGridFastJet_1);
   Notify();
}

Bool_t readTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readTree_cxx
