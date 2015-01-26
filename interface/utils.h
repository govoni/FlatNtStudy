#ifndef utils_h
#define utils_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "plotter.h"

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
		  int flavour, 
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

   bool operator < ( const leptonContainer & lepton2){
     if(lepton4V_.Pt() < lepton2.lepton4V_.Pt()) return true;
     else return false;
   };

   bool operator == ( const leptonContainer & lepton2){
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
	       float btag):
   jet4V_(jet4V),
   jetPUID_(jetPUID),
   btag_(btag){};  

   jetContainer(TLorentzVector jet4V):
   jet4V_(jet4V),
   jetPUID_(-999),
   btag_(-999){};  

   bool operator < ( const jetContainer & jet2){
     if(jet4V_.Pt() < jet2.jet4V_.Pt()) return true;
     else return false;
   };

   bool operator == ( const jetContainer & jet2){
     if(jet4V_ == jet2.jet4V_ and jetPUID_ == jet2.jetPUID_ and btag_ == jet2.btag_ ) return true;
     else return false;
   };


   TLorentzVector jet4V_ ;
   float jetPUID_ ;
   float btag_ ;

};

// delta phi function
double deltaPhi (double phi1, 
		 double phi2) ;

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

void fillRecoLeptonsArray (vector<leptonContainer> & jetVector, 
			   readTree & reader) ;

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
			       float deltaR) ;

vector<jetContainer> dumpTrackJets (vector<jetContainer> & TL_jets, 
				    vector<leptonContainer> & TL_leptons, 
                                    float minptcut, 
				    float lepminptcut, 
				    float deltaR) ;

// method to fill histograms                                                                                                                                            
            
void fillHistos (plotter & analysisPlots,
                 readTree* reader,
                 vector<cutContainer> & CutList,
                 vector<variableContainer> & VariableList,
                 const string & sampleName,     
		 const bool & usePuppiAsDefault,
                 const double & minPtLeptonCut, 
		 const double & minPtLeptonCutCleaning,
                 const double & leptonIsoCut_mu,   
                 const double & leptonIsoCut_el,   
		 const double & leptonIsoLooseCut,
                 const double & matchingCone,   
		 const double & minJetCutPt,
                 map<string,TH1F*> & vect);

void fill2DHistos (plotter & analysisPlots,
		   readTree* reader,
		   vector<cutContainer> & CutList,
		   vector<variableContainer2D> & VariableList2D,
		   const string & sampleName,     
		   const bool & usePuppiAsDefault,
		   const double & minPtLeptonCut, 
		   const double & minPtLeptonCutCleaning,
		   const double & leptonIsoCut_mu,   
		   const double & leptonIsoCut_el,   
		   const double & leptonIsoLooseCut,
		   const double & matchingCone,   
		   const double & minJetCutPt);

bool passCutContainerSelection (readTree* readTree,
                                cutContainer & Cut,
                                const string & sampleName,     
                                const bool & usePuppiAsDefault,
				const double & minPtLeptonCut, 
                                const double & minPtLeptonCutCleaning,
				const double & leptonIsoCut_mu,   
 				const double & leptonIsoCut_el,   
                                const double & leptonIsoLooseCut,
				const double & matchingCone,   
				const double & minJetCutPt,
				map<string,TH1F*> & vect);

#endif
