#ifndef kinematicUtils_h
#define kinematicUtils_h

#include <vector>
#include <cmath>

#include "TLorentzVector.h"

using namespace std ;

void computeAnglesResonance(TLorentzVector p4H, TLorentzVector p4V1, TLorentzVector p4F1, TLorentzVector p4F2, 
			    TLorentzVector p4V2, TLorentzVector p4F3, TLorentzVector p4F4, 
			    double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1);


void computeAnglesVBF(TLorentzVector p4H, TLorentzVector p4V1, TLorentzVector p4F1, TLorentzVector p4F2, 
		      TLorentzVector p4V2, TLorentzVector p4F3, TLorentzVector p4F4, 
		      TLorentzVector p4J1, TLorentzVector p4J2,
		      double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1);

TLorentzVector getNeutrinoPz( TLorentzVector & lepton4V, TLorentzVector & met);

void computeRazor (TLorentzVector & lepton1, TLorentzVector & lepton2, TLorentzVector & met, float & mTR, float & mR);

#endif
