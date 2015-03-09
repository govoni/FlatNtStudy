#ifndef TBACONOBJECTS_H
#define TBACONOBJECTS_H

#include "BaconAnaDefs.h"
#include <TObject.h>

namespace baconhep
{

  // electorn object

  class TElectron : public TObject
  {
    public:
      TElectron():
      pt(0), eta(0), phi(0),
      scEt(0), scEta(0), scPhi(0),
      ptHZZ4l(0), ptErrHZZ4l(0), scEtHZZ4l(0),
      r9(0), ecalEnergy(0),
      pfPt(0), pfEta(0), pfPhi(0),
      trkIso03(-1), ecalIso03(-1), hcalIso03(-1),
      chHadIso03(-1), gammaIso03(-1), neuHadIso03(-1),      
      chHadIso04(-1), gammaIso04(-1), neuHadIso04(-1),      
      d0(-999.), dz(-999.), sip3d(-999.),
      sieie(0), eoverp(0), hovere(0), fbrem(0),
      dEtaIn(0), dPhiIn(0),
      mva(-999.),
      q(0), classification(-999.),
      isConv(false),
      nMissingHits(0),
      typeBits(0), fiducialBits(0),
      scID(-1), trkID(-1),
      hltMatchBits(0)
      {}
      ~TElectron(){}

      bool operator == (const TElectron & ele2) const{
	if(pt == ele2.pt and 
	   eta == ele2.eta and
	   phi == ele2.phi and
	   scEt == ele2.scEt and
	   scEta == ele2.scEta and
	   scPhi == ele2.scPhi and
	   q == ele2.q) return true;
	  else return false;
      }

      bool operator < (const TElectron & ele2) const {
	if(pt < ele2.pt) return true;
	else return false;
      }
    
      float         pt, eta, phi;                         // kinematics
      float         scEt, scEta, scPhi;                   // supercluster kinematics
      float         ptHZZ4l, ptErrHZZ4l, scEtHZZ4l;       // HZZ4l lepton corrections
      float         r9, ecalEnergy;                       // ECAL energy quantities
      float         pfPt, pfEta, pfPhi;                   // matching PF-candidate kinematics
      float         trkIso03, ecalIso03, hcalIso03;       // detector isolation
      float         chHadIso03, gammaIso03, neuHadIso03;  // PF isolation variables
      float         chHadIso04, gammaIso04, neuHadIso04;
      float         d0, dz, sip3d;                        // impact parameter
      float         sieie;                                // shower shape
      float         eoverp;                               // E/p
      float         hovere;                               // H/E
      float         fbrem;                                // brem fraction
      float         dEtaIn, dPhiIn;                       // track-supercluster matching
      float         mva;                                  // electron ID MVA value
      int           q;                                    // charge
      int           classification;                       // electron classification
      bool          isConv;                               // identified by track fit based conversion finder?
      unsigned int  nMissingHits;                         // number of missing expected inner hits 
      unsigned int  typeBits;                             // electron type
      unsigned int  fiducialBits;                         // ECAL fiducial region bits
      int           scID;                                 // supercluster ID number (unique per event)
      int           trkID;                                // track ID number (unique per event)
      TriggerObjects hltMatchBits;                         // HLT matches
      
    ClassDef(TElectron,1)
  };

  enum EEleType
  {
    kEcalDriven    = 1,
    kTrackerDriven = 2
  };

  // muon object

  class TMuon : public TObject
  {
  public:
  TMuon():
    pt(0), eta(0), phi(0), ptErr(0), ptHZZ4l(0),
      staPt(0), staEta(0), staPhi(0),
      pfPt(0), pfEta(0), pfPhi(0),
      trkIso03(-1), ecalIso03(-1), hcalIso03(-1),
      chHadIso03(-1), gammaIso03(-1), neuHadIso03(-1), puIso03(-1),
      chHadIso04(-1), gammaIso04(-1), neuHadIso04(-1), puIso04(-1),
      d0(-999.), dz(-999.), sip3d(-999.),
      tkNchi2(-999.), muNchi2(-999.),
      trkKink(0), glbKink(0),
      q(0),
      nValidHits(0),
      typeBits(0), selectorBits(0),
      nTkHits(0), nPixHits(0),
      nTkLayers(0), nPixLayers(0),
      nMatchStn(0),
      trkID(-1),
      hltMatchBits(0)
	{}
    ~TMuon(){}

  bool operator == (const TMuon & mu2) const{
	if(pt == mu2.pt and 
	   eta == mu2.eta and
	   phi == mu2.phi and
	   pfPt == mu2.pfPt and
	   pfEta == mu2.pfEta and
	   pfPhi == mu2.pfPhi and
	   q == mu2.q) return true;
	  else return false;
      }

      bool operator < (const TMuon & mu2) const {
	if(pt < mu2.pt) return true;
	else return false;
      }
    
    float         pt, eta, phi, ptErr;                           // kinematics                                                                                                 
    float         ptHZZ4l;                                       // HZZ4l lepton corrections                                                                                   
    float         staPt, staEta, staPhi;                         // STA track kinematics                                                                                       
    float         pfPt, pfEta, pfPhi;                            // matched PFCandidate                                                                                        
    float         trkIso03, ecalIso03, hcalIso03;                // detector isolation                                                                                         
    float         chHadIso03, gammaIso03, neuHadIso03, puIso03;  // PF isolation variables                                                                                     
    float         chHadIso04, gammaIso04, neuHadIso04, puIso04;
    float         d0, dz, sip3d;                                 // impact parameter                                                                                           
    float         tkNchi2, muNchi2;                              // track fit normalized chi-square                                                                            
    float         trkKink, glbKink;                              // track kink                                                                                                 
    int           q;                                             // charge                                                                                                     
    int           nValidHits;                                    // number of valid muon hits in global fit                                                                    
    unsigned int  typeBits;                                      // muon type bits                                                                                             
    unsigned int  selectorBits;                                  // MuonSelector bits                                                                                          
    unsigned int  nTkHits, nPixHits;                             // number of hits in tracker                                                                                  
    unsigned int  nTkLayers, nPixLayers;                         // number of hit layers in tracker                                                                            
    unsigned int  nMatchStn;                                     // number of stations with muon segments                                                                      
    int           trkID;                                         // tracker track ID (unique per event)                                                                        
    TriggerObjects hltMatchBits;                                  // HLT matching                                                                                              

    ClassDef(TMuon,1)
      };

  enum EMuType
  {
    kGlobal     = 2,
    kTracker    = 4,
    kStandalone = 8,
    kCaloMuon   = 16,
    kPFMuon     = 32,
    kRPCMuon    = 64
  };


  enum EMuSelectorBit {
   // descriptions from DataFormats/MuonReco/interface/MuonSelectors.h                                                                                                          
    kAll                              = 0x0000001,  // dummy options - always true                                                                                             
    kAllGlobalMuons                   = 0x0000002,  // checks isGlobalMuon flag                                                                                                 
    kAllStandAloneMuons               = 0x0000004,  // checks isStandAloneMuon flag                                                                                            
    kAllTrackerMuons                  = 0x0000008,  // checks isTrackerMuon flag                                                                                               
    kTrackerMuonArbitrated            = 0x0000010,  // resolve ambiguity of sharing segments                                                                                    
    kAllArbitrated                    = 0x0000020,  // all muons with the tracker muon arbitrated                                                                              
    kGlobalMuonPromptTight            = 0x0000040,  // global muons with tighter fit requirements                                                                              
    kTMLastStationLoose               = 0x0000080,  // penetration depth loose selector                                                                                        
    kTMLastStationTight               = 0x0000100,  // penetration depth tight selector                                                                                        
    kTM2DCompatibilityLoose           = 0x0000200,  // likelihood based loose selector                                                                                         
    kTM2DCompatibilityTight           = 0x0000400,  // likelihood based tight selector                                                                                         
    kTMOneStationLoose                = 0x0000800,  // require one well matched segment                                                                                        
    kTMOneStationTight                = 0x0001000,  // require one well matched segment                                                                                        
    kTMLastStationOptimizedLowPtLoose = 0x0002000,  // combination of TMLastStation and TMOneStation                                                                            
    kTMLastStationOptimizedLowPtTight = 0x0004000,  // combination of TMLastStation and TMOneStation                                                                            
    kGMTkChiCompatibility             = 0x0008000,  // require tk stub have good chi2 relative to glb track                                                                     
    kGMStaChiCompatibility            = 0x0010000,  // require sta stub have good chi2 compatibility relative to glb track                                                      
    kGMTkKinkTight                    = 0x0020000,  // require a small kink value in the tracker stub                                                                           
    kTMLastStationAngLoose            = 0x0040000,  // TMLastStationLoose with additional angular cuts                                                                          
    kTMLastStationAngTight            = 0x0080000,  // TMLastStationTight with additional angular cuts                                                                          
    kTMOneStationAngLoose             = 0x0100000,  // TMOneStationLoose with additional angular cuts                                                                     
    kTMLastStationOptimizedBarrelLowPtLoose = 0x0400000,  // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only                         
    kTMLastStationOptimizedBarrelLowPtTight = 0x0800000,  // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only                          
    kRPCMuLoose                             = 0x1000000   // checks isRPCMuon flag (require two well matched hits in different RPC layers)                                 
  };

  // vertex

  class TVertex : public TObject
  {
  public:
  TVertex():
    nTracksFit(0),
      ndof(0), chi2(0),
      x(0), y(0), z(0)
      {}
    ~TVertex(){}

    unsigned int nTracksFit;  // number of tracks in vertex fit                                                                                                                
    float        ndof;        // vertex fit number of degrees of freedom                                                                                                       
    float        chi2;        // vertex fit chi-square                                                                                                                         
    float        x,y,z;       // position                                                                                                                                      

    ClassDef(TVertex,1)
   };

  class TEventInfo : public TObject
  {
  public:
  TEventInfo():
    runNum(0), evtNum(0), lumiSec(0),
      metFilterFailBits(0),
      nPU(0), nPUm(0), nPUp(0),
      nPUmean(0), nPUmeanm(0), nPUmeanp(0),
      pvx(0), pvy(0), pvz(0),
      bsx(0), bsy(0), bsz(0),
      pfMET(0), pfMETphi(0), pfMETCov00(0), pfMETCov01(0), pfMETCov11(0),
      pfMETC(0), pfMETCphi(0), pfMETCCov00(0), pfMETCCov01(0), pfMETCCov11(0),
      mvaMET(0), mvaMETphi(0), mvaMETCov00(0), mvaMETCov01(0), mvaMETCov11(0),
      mvaMETU(0), mvaMETUphi(0), mvaMETUCov00(0), mvaMETUCov01(0), mvaMETUCov11(0),
      mvaMET0(0), mvaMET0phi(0), mvaMET0Cov00(0), mvaMET0Cov01(0), mvaMET0Cov11(0),
      trkMET(0), trkMETphi(0),
      rhoIso(0), rhoJet(0),
      triggerBits(0),
      hasGoodPV(false)
	{}
    ~TEventInfo(){}

    unsigned int  runNum, evtNum, lumiSec;                   // run number, event number, lumi section in data
    unsigned int  metFilterFailBits;                         // MET filter fail bits
    unsigned int  nPU, nPUm, nPUp;                           // number of reconstructed pile-up vertices
    float         nPUmean, nPUmeanm, nPUmeanp;               // expected number of reconstructed pile-up vertices
    float         pvx, pvy, pvz;                             // best primary vertex
    float         bsx, bsy, bsz;                             // beamspot
    float         pfMET, pfMETphi;                           // particle flow MET
    float         pfMETCov00, pfMETCov01, pfMETCov11;
    float         pfMETC, pfMETCphi;                         // corrected particle flow MET
    float         pfMETCCov00, pfMETCCov01, pfMETCCov11;
    float         mvaMET, mvaMETphi;                         // MVA MET
    float         mvaMETCov00, mvaMETCov01, mvaMETCov11;
    float         mvaMETU, mvaMETUphi;
    float         mvaMETUCov00, mvaMETUCov01, mvaMETUCov11;
    float         mvaMET0, mvaMET0phi;
    float         mvaMET0Cov00, mvaMET0Cov01, mvaMET0Cov11;
    float         trkMET, trkMETphi;                         // track MET
    float         rhoIso, rhoJet;                            // event energy density
    TriggerBits   triggerBits;                               // fired trigger bits
    bool          hasGoodPV;                                 // event has a good PV?

    ClassDef(TEventInfo,3)
   };

  // Gen particle
  class TGenParticle : public TObject
  {
  public:
  TGenParticle():
    parent(-1), pdgId(0),status(0),
      pt(0), eta(0), phi(0), mass(0), y(0)
      {}
    ~TGenParticle(){}

    int   parent;
    int   pdgId;
    int   status;
    float pt, eta, phi, mass, y;

    ClassDef(TGenParticle,1)
   };

  // Jet
  class TJet : public TObject
  {
  public:
  TJet():
    pt(0), eta(0), phi(0), mass(0), ptRaw(0),unc(0),
      area(0), d0(-999),dz(-999),
      csv(-2),csv1(-2),csv2(-2),mva(-2),
      qgid(-2),qg1(-2),qg2(-2),
      tau1(-1),tau2(-1),tau3(-1),tau4(-1),
      prunedm(0),
      nCharged(0), nNeutrals(0), nParticles(0),
      beta(0),betaStar(0),dR2Mean(0),ptD(0),q(0),pull(0),pullAngle(0),
      chEmFrac(0), neuEmFrac(0), chHadFrac(0), neuHadFrac(0),
      mcFlavor(0),
      genpt(0),geneta(0),genphi(0),genm(0),
      hltMatchBits(0)
	{}
    ~TJet(){}

    bool operator == (const TJet & jet2) const{
      if(pt == jet2.pt and
           eta == jet2.eta and
           phi == jet2.phi and
           ptRaw == jet2.ptRaw and
           area == jet2.area and
	 mass == jet2.mass) return true;
	else return false;
    }

    bool operator < (const TJet & jet2) const {
      if(pt < jet2.pt) return true;
      else return false;
    }


    float pt, eta, phi, mass, ptRaw,unc;                   // kinematics                                                                                                       
    float area;                                        // jet area (from FastJet)                                                                                              
    float d0, dz;                                      // impact parameter of leading charged constituent                                                                      
    float csv,csv1,csv2;                               // combined secondary vertex based b-tagger and for two leading subjets                                                 
    float mva;                                         // Pile-up discriminator MVA                                                                                            
    float qgid,qg1,qg2;                                // Quark Gluon Id for jet and subjets                                                                                   
    float tau1,tau2,tau3,tau4;                         // N Subjettiness variables                                                                                             
    float prunedm;                                     // Pruned Jet Mass                                                                                                      
    unsigned int nCharged, nNeutrals, nParticles;      // constituent multiplicity                                                                                             
    float beta,betaStar,dR2Mean;                       // PU Jet Id Variables                                                                                                  
    float ptD,q,pull,pullAngle;                        // Additional color flow variables                                                                                      
    float chEmFrac, neuEmFrac, chHadFrac, neuHadFrac;  // fractional contribution by type                                                                                      
    int mcFlavor,mcFlavorPhys;                                      //Flavor                                                                                                   
    float genpt,geneta,genphi,genm;                    // Gen Matched values                                                                                                   
    TriggerObjects hltMatchBits;                        // HLT matching                                                                                                        

    ClassDef(TJet,1)
   };

}
#endif
