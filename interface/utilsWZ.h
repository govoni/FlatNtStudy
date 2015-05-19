#ifndef utilsWZ_h
#define utilsWZ_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "plotter.h"
#include "readTree.h"
#include "ReadInputFile.h"
#include "kinematicUtils.h"
#include "dumpObjectsFromTree.h"

//method to fill histograms                                                                                                                                                     
void fillHistoWZ( plotter & analysisPlot,
		  const string & sampleName,
		  const int    & samplePosition,
		  const string & cutLayerName,
		  vector<variableContainer> & VariableList,
		  vector<leptonContainer>   & leptonsIsoTight,
		  vector<leptonContainer>   & softMuons,
		  vector<jetContainer> & RecoJets,
		  vector<jetContainer> & GenJets,
		  trackJetEvent  & trackEvent,
		  TLorentzVector & L_met,
		  const string   & systematicName = "",
		  const float    & eventFakeWeight = 1.
		  );


void fillHisto2DWZ( plotter & analysisPlot,
		    const string & sampleName,
		    const int    & samplePosition,
		    const string & cutLayerName,
		    vector<variableContainer2D> & VariableList2D,
		    vector<leptonContainer> & leptonsIsoTight,
		    vector<leptonContainer> & softMuons,
		    vector<jetContainer>    & RecoJets,
		    vector<jetContainer>    & GenJets,
		    trackJetEvent  & trackEvent,
		    TLorentzVector & L_met,
		    const string   & systematicName = "",
		    const float    & eventFakeWeight = 1.
		    );


// method to apply cuts for the WZ analysis                                                                                                                                      
bool passCutContainerSelection (cutContainerWZ & Cut,
				const string & sampleName,
				const int & samplePosition,
				readTree* reader,
				vector<leptonContainer> & LeptonsAll,
				vector<leptonContainer> & leptonsIsoTight,
				vector<leptonContainer> & softMuons,
				vector<jetContainer> & RecoJets,
				trackJetEvent & trackEvent,
				const TLorentzVector & L_met,
				const float & minPtLeptonCut,
				const float & leptonIsoLooseCut,
				map<string,TH1F*> & vect,
				const string & finalStateStringw,
				const float & eventWeight = 1
				);


bool passCutContainerSelection (readTree* readTree,
				cutContainerWZ & Cut,
				const string & sampleName,
				const int & samplePosition,
				const bool & usePuppiAsDefault,
				const float & minPtLeptonCut,
				const float & minPtLeptonCutCleaning,
				const float & leptonIsoCut_mu,
				const float & leptonIsoCut_el,
				const float & leptonIsoLooseCut,
				const float & matchingCone,
				const float & minJetCutPt,
				map<string,TH1F*> & vect,
				const string & finalStateString,
				const float & eventWeight = 1
				);



#endif
