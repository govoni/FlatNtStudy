#ifdef __CINT__

using namespace std;

#pragma link off all globals;
#pragma link off all functions;
#pragma link off all classes;

// from ConfigFileLine.h
#include "ConfigFileLine.h"
#pragma link C++ class ConfigFileLine+;

#include "config.h"
#pragma link C++ class record+;
#pragma link C++ class config+;

#include "ConfigParser.h"
// from ConfigParser.h
#pragma link C++ class ConfigParser+;


#include "ReadInputFile.h"
#pragma link C++ class sampleContainer+;
#pragma link C++ class cutContainerWW+;
#pragma link C++ class cutContainerWZ+;
#pragma link C++ class variableContainer+;
#pragma link C++ class variableContainer2D+;
#pragma link C++ class variableContainerDynamic+;
#pragma link C++ class variableContainerDynamic2D+;
#pragma link C++ class histoContainer+;
#pragma link C++ class trainingContainer+;
#pragma link C++ class fakeRateContainer+;
#pragma link C++ class fakeMigrationContainer+;

#include "TBaconObjects.h"
#pragma link C++ class baconhep::TElectron+;
#pragma link C++ class baconhep::TMuon+;
#pragma link C++ class baconhep::TJet+;
#pragma link C++ class baconhep::TEventInfo+;
#pragma link C++ class baconhep::TVertex+;
#pragma link C++ class baconhep::TGenParticle+;

#include "TMVAPlotClass.h"
#pragma link C++ class significanceBox+;
#pragma link C++ class TMVAPlotClass+;

#include "TMVATrainingClass.h"
#pragma link C++ class TMVATrainingClass<cutContainerWW>+;
#pragma link C++ class TMVATrainingClass<cutContainerWZ>+;

#include "counter.h"
#pragma link C++ class counter+;

#include "dumpObjectsFromTree.h"
#pragma link C++ class leptonContainer+;
#pragma link C++ class jetContainer+;
#pragma link C++ class trackJetEvent;
#pragma link C++ class leptonContainerLatino+;
#pragma link C++ class jetContainerLatino+;

#include "fakeBackgroundUtils.h"

#pragma link C++ function void makeFakeLeptonBackground<cutContainerWW>(const string &, const int    &, const string &, plotter &,  readTree *,  cutContainerWW &,  vector<variableContainer> &,  vector<leptonContainer> &,  vector<leptonContainer> &,  vector<leptonContainer> &, vector<jetContainer> & ,  vector<jetContainer> & ,  vector<jetContainer> & ,  TLorentzVector       & ,  const float & ,  const float & ,  const float & ,  const float & ,fakeRateContainer      & ,  fakeMigrationContainer & ,  map <string,TH1F*> &,  const int & );
  
#pragma link C++ function void makeFakeLeptonBackground<cutContainerWZ>(const string &, const int    &,  const string &, plotter &, readTree *, cutContainerWZ &, vector<variableContainer> &, vector<leptonContainer> &, vector<leptonContainer> &, vector<leptonContainer> &, vector<jetContainer> & , vector<jetContainer> & , vector<jetContainer> & , TLorentzVector       & , const float & , const float & , const float & , const float & , fakeRateContainer      & , fakeMigrationContainer & ,map <string,TH1F*> &,const int & );

#pragma link C++ function void makeFakeLeptonBackground<cutContainerWW>(const string & , const int    & ,  const string & ,  plotter & , readTree*, cutContainerWW & , vector<variableContainer> & , vector<variableContainer2D> &,vector<leptonContainer> & ,vector<leptonContainer> & ,vector<leptonContainer> & ,vector<jetContainer> &,vector<jetContainer> &,vector<jetContainer> &, TLorentzVector       &,const float & ,const float & ,const float & ,const float & ,fakeRateContainer      & ,fakeMigrationContainer & ,map <string,TH1F*> &,const int & );

#pragma link C++ function void makeFakeChargeBackground<cutContainerWW>(const string &,const int    &,const string &,plotter &,readTree *,cutContainerWW &,vector<variableContainer> &,vector<leptonContainer> &,vector<leptonContainer> &,vector<leptonContainer> &,vector<jetContainer> &,vector<jetContainer> &,vector<jetContainer> &,TLorentzVector &,const float &,const float &,const float &,const float &,map <string,TH1F*> &)+;

#pragma link C++ function  void makeFakeChargeBackground<cutContainerWW>(const string &,const int    & ,const string & ,plotter & ,readTree *,cutContainerWW & ,vector<variableContainer> &,vector<variableContainer2D> &,vector<lepatonContainer> & ,vector<leptonContainer> & ,vector<leptonContainer> & ,vector<jetContainer> & ,vector<jetContainer> & ,vector<jetContainer> & ,TLorentzVector &,const float & ,const float & ,const float & ,const float & ,map <string,TH1F*> &);


#pragma link C++ function void makeFakeChargeBackground<cutContainerWZ>(const string &,const int    &,const string &, plotter &,readTree *,cutContainerWZ &,vector<variableContainer> &,vector<leptonContainer> &,  vector<leptonContainer> &,  vector<leptonContainer> &,  vector<jetContainer> &,  vector<jetContainer> &,  vector<jetContainer> &,  TLorentzVector &,  const float &,  const float &,  const float &,  const float &,  map <string,TH1F*> &)+;

#pragma link C++ function  void makeFakeChargeBackground<cutContainerWZ>(const string &,  const int    & ,  const string & ,  plotter & ,  readTree *,  cutContainerWZ & ,  vector<variableContainer> &,  vector<variableContainer2D> &,  vector<leptonContainer> & ,  vector<leptonContainer> & ,  vector<leptonContainer> & ,  vector<jetContainer> & ,  vector<jetContainer> & ,  vector<jetContainer> & ,  TLorentzVector &,  const float & ,  const float & ,  const float & ,  const float & ,  map <string,TH1F*> &);


#include "readTree.h"
#pragma link C++ class readTree+;

#include "readTreeLatino.h"
#pragma link C++ class readTreeLatino+;

#include "treeReader.h"
#pragma link C++ class treeReader+;

#include "objectFormula.h"
#pragma link C++ class objectFormula+;

#include "plotter.h"
#pragma link C++ class layer+;
#pragma link C++ class sample+;
#pragma link C++ class plotter+;

#include "utils.h"

#pragma link C++ function  void loopOnEvents<cutContainerWW> ( plotter &,const string &, const int    &, readTree*, vector<cutContainerWW> &,  vector<variableContainer>  &, const bool &, const float &, const float &,  const float &, const float &, const float &, const float &, const float &, map<string,TH1F*> &, const string &, const string &, const string &);

#pragma link C++ function  void loopOnEvents<cutContainerWZ> ( plotter &,const string &, const int    &, readTree*, vector<cutContainerWZ> &,  vector<variableContainer>  &, const bool &, const float &, const float &,  const float &, const float &, const float &, const float &, const float &, map<string,TH1F*> &, const string &, const string &, const string &);


#pragma link C++ function  void loopOnEvents<cutContainerWW> (plotter &,const string &,const int    & readTree*, vector<cutContainerWW> &, vector<variableContainer>  &,vector<variableContainer2D>  &,const bool &,const float &,const float &,const float &,const float &,const float &,const float &,const float &,map<string,TH1F*> &,const string &,const string &,const string &);


#pragma link C++ function  void loopOnEvents<cutContainerWZ> (plotter &,const string &,const int    & readTree*, vector<cutContainerWZ> &, vector<variableContainer>  &,vector<variableContainer2D>  &,const bool &,const float &,const float &,const float &,const float &,const float &,const float &,const float &,map<string,TH1F*> &,const string &,const string &,const string &);



#pragma link C++ class vector<TLorentzVector>+;


#endif
