#ifndef utilsLatino_h
#define utilsLatino_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "readTreeLatino.h"
#include "ReadInputFile.h"
#include "objectFormula.h"

using namespace std ;

#define pdgWMass   80.385
#define pdgTopMass 173.21
#define softMuonPt 3.

// class to contain lepton information
class leptonContainerLatino {

 public:

  leptonContainerLatino(){};
  ~leptonContainerLatino(){};

  leptonContainerLatino(TLorentzVector lepton4V, 
			float charge, 
			int   flavour):
   lepton4V_(lepton4V),
   charge_(charge),
   flavour_(flavour){};  

   bool operator < ( const leptonContainerLatino & lepton2) const {
     if(lepton4V_.Pt() < lepton2.lepton4V_.Pt()) return true;
     else return false;
   };

   bool operator == ( const leptonContainerLatino & lepton2) const {
     if(lepton4V_ == lepton2.lepton4V_ and charge_ == lepton2.charge_ ) return true;
     else return false;
   };


   TLorentzVector lepton4V_ ;
   float charge_ ;
   int   flavour_ ;
};

// class to contain jet information
class jetContainerLatino {

 public:

  jetContainerLatino(){};
  ~jetContainerLatino(){};


  jetContainerLatino(TLorentzVector jet4V, 
		     float jetPUID, 
		     float btagCSV):
   jet4V_(jet4V),
   jetPUID_(jetPUID),
   btagCSV_(btagCSV){};  

   jetContainerLatino(TLorentzVector jet4V):
   jet4V_(jet4V),
   jetPUID_(-999),
   btagCSV_(-999){};  

   bool operator < ( const jetContainerLatino & jet2){
     if(jet4V_.Pt() < jet2.jet4V_.Pt()) return true;
     else return false;
   };

   bool operator == ( const jetContainerLatino & jet2){
     if(jet4V_ == jet2.jet4V_ and jetPUID_ == jet2.jetPUID_ and btagCSV_ == jet2.btagCSV_ ) return true;
     else return false;
   };


   TLorentzVector jet4V_ ;
   float jetPUID_ ;
   float btagCSV_ ;

};

// class to contain jet information
class genJetContainerLatino {

 public:

  genJetContainerLatino(){};
  ~genJetContainerLatino(){};


  genJetContainerLatino(float pt,
			float eta,
			float phi):
  pt_(pt),
    eta_(eta),
    phi_(phi){};  

   bool operator < ( const genJetContainerLatino & jet2){
     if( pt_ < jet2.pt_) return true;
     else return false;
   };

   bool operator == ( const genJetContainerLatino & jet2){
     if(pt_ == jet2.pt_ and eta_ == jet2.eta_ and phi_ == jet2.phi_ ) return true;
     else return false;
   };


   float pt_ ;
   float eta_ ;
   float phi_ ;
   
};


// check if an object is within a dR with another one
bool closeToLeptons (float eta, 
		     float phi, 
		     vector<leptonContainerLatino> & TL_leptons, 
		     float R = 0.3) ;

bool closeToLeptons (float eta, 
		     float phi, 
		     readTreeLatino & reader, 
		     float R = 0.3) ;

// fill Reco Jet Array from the readTree object
void fillRecoJetArray  (vector<jetContainerLatino> & jetVector, 
			readTreeLatino & reader) ;

void fillPuppiJetArray (vector<jetContainerLatino> & jetVector, 
			readTreeLatino & reader) ;

void fillGenJetArray   (vector<genJetContainerLatino> & jetVector, 
			readTreeLatino & reader) ;


void fillRecoLeptonsArray (vector<leptonContainerLatino> & lepVector, 
			   readTreeLatino & reader) ;

void fillGenLeptonsArray (vector<leptonContainerLatino> & lepVector, 
			  readTreeLatino & reader) ;

// dump isolated leptons
vector<leptonContainerLatino> dumpLeptons(vector<leptonContainerLatino> & TL_leptons, 
					  vector<leptonContainerLatino> & genLeptons,
					  float minptcut,
					  float dRmatching) ;

// dump cleaned jets
vector<jetContainerLatino> dumpJets (vector<jetContainerLatino> & TL_jets, 
				     vector<leptonContainerLatino>  & TL_leptons, 
				     float minptcut, 
				     float btagcut, 
				     float jetPUIDcut, 
				     float lepminptcut, 
				     float deltaR,
				     float etaMax = 4.7) ;

vector<genJetContainerLatino> dumpJets (vector<genJetContainerLatino> & TL_jets, 
					vector<leptonContainerLatino>  & TL_leptons, 
					float minptcut, 
					float btagcut, 
					float jetPUIDcut, 
					float lepminptcut, 
					float deltaR,
					float etaMax = 4.7) ;

#endif
