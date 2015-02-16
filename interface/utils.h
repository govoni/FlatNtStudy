#ifndef utils_h
#define utils_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "plotter.h"
#include "readTree.h"
#include "ReadInputFile.h"
#include "objectFormula.h"

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
			       float deltaR,
			       float etaMax = 10) ;

vector<jetContainer> dumpTrackJets (vector<jetContainer> & TL_jets, 
				    vector<leptonContainer> & TL_leptons, 
                                    float minptcut, 
				    float lepminptcut, 
				    float deltaR) ;

vector<leptonContainer> dumpSoftMuons (vector<leptonContainer> & leptonAll,
                                       vector<jetContainer> & jetAll,
                                       float isolationCut,
                                       float minLeptonCut,
                                       float minptcut,
                                       float deltaR);

/// method to get the fake weight and the new lepton kinematic
float getFakeWeight( jetContainer inputJet,
		     fakeRateContainer & fakeRate,
		     string leptonFlavour,
		     vector<jetContainer> & jetCollection);


leptonContainer createFakeLepton( jetContainer inputJet,
				  leptonContainer inputLepton,
				  fakeMigrationContainer & fakeMigration,
				  string finalStateString);


// method to loop on events
                                                                                                                                            
void loopOnEvents (plotter & analysisPlots,
		   readTree* reader,
		   vector<cutContainer> & CutList,
		   vector<variableContainer> & VariableList,
		   const string & sampleName,     
		   const bool & usePuppiAsDefault,
		   const float & minPtLeptonCut, 
		   const float & minPtLeptonCutCleaning,
		   const float & leptonIsoCut_mu,   
		   const float & leptonIsoCut_el,   
		   const float & leptonIsoLooseCut,
		   const float & matchingCone,   
		   const float & minJetCutPt,
		   map<string,TH1F*> & vect,
		   const string & finalStateString = "",
		   const string & scenarioString = "",
		   const string & fakeRateFile = "");

void loopOnEvents (plotter & analysisPlots,
		   readTree* reader,
		   vector<cutContainer> & CutList,
		   vector<variableContainer> & VariableList,
		   vector<variableContainer2D> & VariableList2D,
		   const string & sampleName,     
		   const bool & usePuppiAsDefault,
		   const float & minPtLeptonCut, 
		   const float & minPtLeptonCutCleaning,
		   const float & leptonIsoCut_mu,   
		   const float & leptonIsoCut_el,   
		   const float & leptonIsoLooseCut,
		   const float & matchingCone,   
		   const float & minJetCutPt,
		   map<string,TH1F*> & vect,
		   const string & finalStateString = "",
		   const string & scenarioString = "",
		   const string & fakeRateFile = "");

//method to fill histograms

void fillHisto( plotter & analysisPlot,
                const string & sampleName,
                const string & cutLayerName,
                vector<variableContainer> & VariableList,
                vector<leptonContainer> & leptonsIsoTight,
                vector<leptonContainer> & softMuons,
                vector<jetContainer> & RecoJets,
		vector<jetContainer> & GenJets,
		vector<jetContainer> & trackJets,
                TLorentzVector & L_met,
                const string & systematicName = "",
		const float  & eventFakeWeight = 1.
                );


void fillHisto2D( plotter & analysisPlot,
		  const string & sampleName,
		  const string & cutLayerName,
		  vector<variableContainer2D> & VariableList2D,
		  vector<leptonContainer> & leptonsIsoTight,
		  vector<leptonContainer> & softMuons,
		  vector<jetContainer> & RecoJets,
		  vector<jetContainer> & GenJets,
		  vector<jetContainer> & trackJets,
		  TLorentzVector & L_met,
		  const string & systematicName = "",
		  const float & eventFakeWeight = 1.
		  );

// method to apply cuts

bool passCutContainerSelection (cutContainer & Cut,
                                const string & sampleName,     
                                readTree* reader,     
				vector<leptonContainer> & LeptonsAll,
				vector<leptonContainer> & leptonsIsoTight,
				vector<leptonContainer> & softMuons,
				vector<jetContainer> & RecoJets,
				const TLorentzVector & L_met,
				const float & minPtLeptonCut,                    
                                const float & leptonIsoLooseCut,
				map<string,TH1F*> & vect,
				const string & finalStateString);


bool passCutContainerSelection (readTree* readTree,
                                cutContainer & Cut,
                                const string & sampleName,     
                                const bool & usePuppiAsDefault,
				const float & minPtLeptonCut, 
                                const float & minPtLeptonCutCleaning,
				const float & leptonIsoCut_mu,   
 				const float & leptonIsoCut_el,   
                                const float & leptonIsoLooseCut,
				const float & matchingCone,   
				const float & minJetCutPt,
				map<string,TH1F*> & vect,
				const string & finalStateString
				);

#endif
