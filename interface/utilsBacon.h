#ifndef utilsBacon_h
#define utilsBacon_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TClonesArray.h"

#include "ReadInputFile.h"
#include "TBaconObjects.h"

using namespace std ;
using namespace baconhep ;

bool passEleID(TElectron* ele, float rho);

bool passMuonID(TMuon* mu, float rho);

void findGenLeptonsFromW(TClonesArray* genParticles, 
			 vector<int> & muIndex, 
			 vector<int> & eleIndex);

void findGenNeutrinoFromW(TClonesArray* genParticles,
                          vector<TGenParticle> & genNeutrino);

void cleanedJetsFromLeptons(vector<TJet> & cleanedJets,
                            TClonesArray & inputJets,
                            vector<TMuon> & goodTightMuons,
                            vector<TElectron> & goodTightElectrons, float minJetCutPt, float matchingCone);



void fillHistos(vector<histoContainer> & plotVector,
                vector<variableContainer> variableList,
                string name,
                float weight,
                TLorentzVector lepton1,
                TLorentzVector lepton2,
                TLorentzVector jet1,
                TLorentzVector jet2,
                TLorentzVector met);


void fillResponse(vector<histoContainer> & plotVector,
		  vector<variableContainer> variableList,
		  string name,
		  float weight,
		  TLorentzVector lepton1,
		  TLorentzVector genlepton1,
		  TLorentzVector lepton2,
		  TLorentzVector genlepton2,
		  TLorentzVector jet1,
		  TLorentzVector genjet1,
		  TLorentzVector jet2,
		  TLorentzVector genjet2,
		  TLorentzVector met,
		  TLorentzVector genmet);

#endif
