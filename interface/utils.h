#ifndef utils_h
#define utils_h

// c++ libraries
#include <vector>

// root objects
#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

// plotting tools
#include "plotter.h"
// read tree info
#include "readTree.h"
// object to read info from configuration file
#include "ReadInputFile.h"
// to inglobe sys on the objects
#include "objectFormula.h"
// to dump info from our trees
#include "dumpObjectsFromTree.h"
// generate fake background
#include "fakeBackgroundUtils.h"
// calculate some kinematic variables
#include "kinematicUtils.h"

// import function to fill histograms and apply cuts for WWss analysis
#include "utilsWW.h"
// import function to fill histograms and apply cuts for ZZ analysis
#include "utilsWZ.h"

using namespace std ;

#define pdgWMass   80.385
#define pdgTopMass 173.21
#define softMuonPt 3.

// method to loop on events 
template <typename T>
void loopOnEvents (plotter & analysisPlots,
		   const string & sampleName,     
		   const int    & samplePostion,     
		   readTree* reader,
		   vector<T> & CutList,
		   vector<variableContainer>  & VariableList,
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

template<typename T>
void loopOnEvents (plotter & analysisPlots,
		   const string & sampleName,     
		   const int    & samplePostion,     
		   readTree* reader,
		   vector<T> & CutList,
		   vector<variableContainer> & VariableList,
		   vector<variableContainer2D> & VariableList2D,
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

#endif
