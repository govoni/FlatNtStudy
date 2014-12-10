#ifndef utils_h
#define utils_h

#include "TChain.h"
#include "TLorentzVector.h"
#include "readTree.h"
#include <vector>

double deltaPhi (double phi1, double phi2) ;

bool closeToLeptons (float eta, float phi, std::vector<TLorentzVector> & TL_leptons, float R = 0.3) ;
bool closeToLeptons (float eta, float phi, readTree & reader, float R = 0.3) ;

/*
 - translates the 8 sets of variables into 8 sets of arrays
*/
void fillRecoJetArray (float * pt, float * eta, float * phi, 
                       float * mass, float * ID, float * btag, readTree & reader) ;
void fillPuppiJetArray (float * pt, float * eta, float * phi, 
                        float * mass, float * ID, float * btag, readTree & reader) ;
void fillGenJetArray (float * pt, float * eta, float * phi, 
                      float * mass, float * ID, float * btag, readTree & reader) ;

/*
 - translates the 4 sets of variables into 4 sets of arrays
*/
void fillRecoLeptonsArray (float * pt, float * eta, float * phi, float * iso, readTree & reader) ;

/*
 - copies leptons in the vector
 - removes leptons with the minimum pt threshold 
 - applies the isolation according to isocut
*/
void dumpLeptons (std::vector<TLorentzVector> & TL_leptons, 
                  float * pt, float * eta, float * phi, float * iso, float isocut, float minptcut) ;

/*
 - copies jets in the vector
 - removes jets closer than deltaR to the first two leptons, if the first two leptons 
   pass the pt threshold lepminptcut
 - applies the btagging
*/
void dumpJets (std::vector<TLorentzVector> & TL_jets, std::vector<TLorentzVector> & TL_leptons, 
               float * pt, float * eta, float * phi, float * mass, float * btag, 
               float minptcut, float btagcut, float lepminptcut, float deltaR, int Njets = 8) ;

/*
 - translates the 8 sets of variables into 8 sets of arrays
*/
void fillTrackJetArray (float * pt, float * eta, float * phi, float * mass, readTree & reader) ;
/*
 - copies jets in the vector
 - removes jets closer than deltaR to the first two leptons, if the first two leptons 
   pass the pt threshold lepminptcut
*/
void dumpTrackJets (std::vector<TLorentzVector> & TL_jets, std::vector<TLorentzVector> & TL_leptons, 
                    float * pt, float * eta, float * phi, float * mass, 
                    float minptcut, float lepminptcut, float deltaR) ;

#endif
