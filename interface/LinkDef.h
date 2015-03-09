#include "config.h"
#include "counter.h"
#include "plotter.h"
#include "readTree.h"
#include "treeReader.h"
#include "objectFormula.h"
#include "utils.h"
#include "ConfigFileLine.h"
#include "ConfigParser.h"
#include "ReadInputFile.h"
#include "TMVATrainingClass.h"
#include "TMVAPlotClass.h"
#include "TBaconObjects.h"
#include "readTreeLatino.h"
#include "utilsLatino.h"
#include "objectFormula.h"

using namespace std;

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all functions;
#pragma link off all classes;

// from ConfigFileLine.h
#pragma link C++ class ConfigFileLine+;
// from ConfigParser.h
#pragma link C++ class ConfigParser+;

// from ReadInputFile.h
#pragma link C++ class sampleContainer+;
#pragma link C++ class cutContainer+;
#pragma link C++ class variableContainer+;
#pragma link C++ class variableContainer2D+;
#pragma link C++ class variableContainerDynamic+;
#pragma link C++ class variableContainerDynamic2D+;
#pragma link C++ class histoContainer+;
#pragma link C++ class trainingContainer+;
#pragma link C++ class fakeRateContainer+;
#pragma link C++ class fakeMigrationContainer+;

// TObjects
#pragma link C++ class baconhep::TElectron+;
#pragma link C++ class baconhep::TMuon+;
#pragma link C++ class baconhep::TJet+;
#pragma link C++ class baconhep::TEventInfo+;
#pragma link C++ class baconhep::TVertex+;
#pragma link C++ class baconhep::TGenParticle+;

#pragma link C++ class record+;
#pragma link C++ class config+;
#pragma link C++ class counter+;

#pragma link C++ class readTree+;
#pragma link C++ class readTreeLatino+;

#pragma link C++ class objectFormula+;

// from plotter.h
#pragma link C++ class layer+;
#pragma link C++ class sample+;
#pragma link C++ class plotter+;

#pragma link C++ class treeReader+;

//from utils
#pragma link C++ class leptonContainer+;
#pragma link C++ class jetContainer+;
#pragma link C++ class leptonContainerLatino+;
#pragma link C++ class jetContainerLatino+;

// TMVA
#pragma link C++ class TMVATrainingClass+;
#pragma link C++ class significanceBox+;
#pragma link C++ class TMVAPlotClass+;

#pragma link C++ class vector<TLorentzVector>+;

#endif
