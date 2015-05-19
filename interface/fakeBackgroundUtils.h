#ifndef fakeBackgroundUtils_h
#define fakeBackgroundUtils_h

// include generic c++
#include <vector>

// include root libraries
#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

// include other packages
#include "plotter.h"
#include "readTree.h"
#include "ReadInputFile.h"
#include "dumpObjectsFromTree.h"
#include "utils.h"
#include "utilsWW.h"
#include "utilsWZ.h"

using namespace std ;

// mis-charge probability
float getElectronMisChargeProbability (const float & pt, const float & eta);

// dump jets for fake rate evaluation
vector<jetContainer> dumpJetsForFake (vector<jetContainer> & RecoJets,
                                      vector<jetContainer> & GenJets,
                                      vector<leptonContainer> & leptonsIsoTight,
                                      const float & minJetCutPt,
                                      const float & btagCut,
                                      const float & jetPUID,
                                      const float & minPtLeptonCutCleaning,
                                      const float & matchingCone,
                                      const float & etaMaxL);

/// method to get the fake weight and the new lepton kinematic                                                                                                                  
float getFakeWeight( jetContainer inputJet,
                     fakeRateContainer & fakeRate,
                     string leptonFlavour,
                     vector<jetContainer> & jetCollection);

// create a fake lepton in events with just one real prompt lepton : used in WW analysis
leptonContainer createFakeLepton( jetContainer inputJet,
                                  leptonContainer inputLepton,
                                  fakeMigrationContainer & fakeMigration,
                                  string finalStateString,
				  int flavour = -1);

// create a fake lepton in events with two real prompt leptons : used in WZ analysis
leptonContainer createFakeLepton( jetContainer inputJet,
                                  leptonContainer inputLepton_1,
                                  leptonContainer inputLepton_2,
                                  fakeMigrationContainer & fakeMigration,
                                  string finalStateString,
				  int flavour = -1);

// produce histograms for fake lepton backgrounds taking a generic cut container : to be generic for WW and WZ analysis
template <typename T>
void makeFakeLeptonBackground(const string & sampleName,       // name of the sample
			      const int    & samplePosition,   // position in the sample list
                              const string & finalStateString, // to specify the final state you want to select
                              plotter & analysisPlots,         // plotter object
                              readTree *reader,                // tree reader
                              T & cutElement,                  // cut element
                              vector<variableContainer> & VariableList,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector       & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              fakeRateContainer      & fakeRate,
                              fakeMigrationContainer & fakeMigration,
                              map <string,TH1F*> & vect,
			      const int & numberPromtLeptons = 1
                              );

template <typename T>
void makeFakeLeptonBackground(const string & sampleName,
			      const int    & samplePosition,
                              const string & finalStateString,
                              plotter      & analysisPlots,
                              readTree *reader,
                              T & cutElement,
                              vector<variableContainer> & VariableList,
                              vector<variableContainer2D> & VariableList2D,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              fakeRateContainer & fakeRate,
                              fakeMigrationContainer & fakeMigration,
                              map <string,TH1F*> & vect,
			      const int & numberPromtLeptons = 1
                              );

// -------------------

template <typename T>
void makeFakeChargeBackground(const string & sampleName,
			      const int    & samplePosition,
                              const string & finalStateString,
                              plotter & analysisPlots,
                              readTree *reader,
                              T & cutElement,
                              vector<variableContainer> & VariableList,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              map <string,TH1F*> & vect);

template <typename T>
void makeFakeChargeBackground(const string & sampleName,
			      const int    & samplePosition,
                              const string & finalStateString,
                              plotter  & analysisPlots,
                              readTree *reader,
                              T & cutElement,
                              vector<variableContainer> & VariableList,
                              vector<variableContainer2D> & VariableList2D,
                              vector<leptonContainer> & leptonsIsoTight,
                              vector<leptonContainer> & LeptonsAll,
                              vector<leptonContainer> & softMuons,
                              vector<jetContainer> & RecoJets,
                              vector<jetContainer> & GenJets,
                              vector<jetContainer> & trackJetsAll,
                              TLorentzVector & L_met,
                              const float & minJetCutPt,
                              const float & leptonIsoLooseCut,
                              const float & minPtLeptonCutCleaning,
                              const float & matchingCone,
                              map <string,TH1F*> & vect);




#endif
