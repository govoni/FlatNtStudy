#ifndef dumpObjectsFromTree_h
#define dumpObjectsFromTree_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "readTree.h"
#include "ReadInputFile.h"

using namespace std ;

// class to contain lepton information                                                                                                                                           
class leptonContainer {

 public:

  leptonContainer(){};
  ~leptonContainer(){};

  leptonContainer(TLorentzVector lepton4V,
                  float charge,
                  int   flavour,
                  float iso):
  lepton4V_(lepton4V),
    charge_(charge),
    flavour_(flavour),
    iso_(iso){};

  leptonContainer(TLorentzVector lepton4V,
                  float iso):
  lepton4V_(lepton4V),
    charge_(-999),
    flavour_(-999),
    iso_(iso){};

  bool operator < ( const leptonContainer & lepton2) const {
    if(lepton4V_.Pt() < lepton2.lepton4V_.Pt()) return true;
    else return false;
  };

  bool operator == ( const leptonContainer & lepton2) const {
    if(lepton4V_ == lepton2.lepton4V_ and charge_ == lepton2.charge_ ) return true;
    else return false;
  };


  TLorentzVector lepton4V_ ;
  float charge_ ;
  int   flavour_ ;
  float iso_ ;
};


// class to contain jet information                                                                                                                                              
class jetContainer {

 public:

  jetContainer(){};
  ~jetContainer(){};


  jetContainer(TLorentzVector jet4V,
               float jetPUID,
               float btag,
               int jetflavour):
  jet4V_(jet4V),
    jetPUID_(jetPUID),
    btag_(btag),
    jetflavour_(jetflavour) {};

 jetContainer(TLorentzVector jet4V):
  jet4V_(jet4V),
    jetPUID_(-999),
    btag_(-999),
    jetflavour_(-999){};

  bool operator < ( const jetContainer & jet2) const {
    if(jet4V_.Pt() < jet2.jet4V_.Pt()) return true;
    else return false;
  };

  bool operator == ( const jetContainer & jet2) const{
    if(jet4V_ == jet2.jet4V_ and jetPUID_ == jet2.jetPUID_ and btag_ == jet2.btag_ and jetflavour_ == jet2.jetflavour_ ) return true;
    else return false;
  };


  TLorentzVector jet4V_ ;
  float jetPUID_ ;
  float btag_ ;
  int   jetflavour_;
};

// class to contain track jet event information                                                                                                                                  
class trackJetEvent {

 public:

  trackJetEvent(){};
  ~trackJetEvent(){};


  trackJetEvent(
		int   numTrack,
		int   numTrackIn,
		int   numTrackOut,
		int   HTTrack,
		int   HTTrackIn,
		int   HTTrackOut):
  numTrack_(numTrack),
    numTrackIn_(numTrackIn),
    numTrackOut_(numTrackOut),
    HTTrack_(HTTrack),
    HTTrackIn_(HTTrackIn),
    HTTrackOut_(HTTrackOut){};

  int numTrack_;
  int numTrackIn_;
  int numTrackOut_;

  float HTTrack_;
  float HTTrackIn_;
  float HTTrackOut_;

};

// delta phi function                                                                                                                                                           
float deltaPhi (float phi1,
		float phi2) ;

// check if an object is within a dR with another one                                                                                                                           
bool closeToLeptons (float eta,
                     float phi,
                     vector<leptonContainer> & TL_leptons,
                     float R = 0.3) ;

bool closeToLeptons (float eta,
                     float phi,
                     readTree & reader,
                     float R = 0.3) ;


// fill Reco Jet Array from the readTree object                                                                                                                               
void fillRecoJetArray  (vector<jetContainer> & jetVector,
                        readTree & reader) ;

void fillPuppiJetArray (vector<jetContainer> & jetVector,
                        readTree & reader) ;

void fillGenJetArray   (vector<jetContainer> & jetVector,
                        readTree & reader) ;

void fillRecoLeptonsArray (vector<leptonContainer> & lepVector,
                           readTree & reader) ;

void fillGenLeptonsArray(vector<leptonContainer> & lepVector,
                         readTree & reader);

void fillTrackJetArray (vector<jetContainer> & jetVector,
                        readTree & reader) ;

// dump isolated leptons                                                                                                                                                      
vector<leptonContainer> dumpLeptons(vector<leptonContainer> & TL_leptons,
                                    float isocut,
                                    float minptcut) ;

vector<leptonContainer> dumpLeptons(vector<leptonContainer> & TL_leptons,
                                    float isocut_mu,
                                    float isocut_el,
                                    float minptcut) ;

// dump cleaned jets                                                                                                                                                           
vector<jetContainer> dumpJets (vector<jetContainer> & TL_jets,
                               vector<leptonContainer>  & TL_leptons,
                               float minptcut,
                               float btagcut,
                               float jetPUIDcut,
                               float lepminptcut,
                               float deltaR,
                               float etaMax = 4.7) ;


vector<jetContainer> dumpTrackJets (vector<jetContainer> & TL_jets,
                                    vector<leptonContainer> & TL_leptons,
                                    float minptcut,
                                    float lepminptcut,
                                    float deltaR) ;

// produce track jet info for an event                                                                                                                                       
trackJetEvent produceTrackJetEvent (vector<jetContainer> & trackJets,
                                    vector<jetContainer> & RecoJets);

// dump soft muon information                                                                                                                                                
vector<leptonContainer> dumpSoftMuons (vector<leptonContainer> & leptonAll,
                                       vector<jetContainer> & jetAll,
                                       float isolationCut,
                                       float minLeptonCut,
                                       float minptcut,
                                       float deltaR);



#endif
