#include "utils.h"
#include <memory>

void fillHistoWZ( plotter & analysisPlots,
		  const string & sampleName,
		  const int    & samplePosition,
		  const string & cutLayerName,
		  vector<variableContainer> & VariableList,
		  vector<leptonContainer> & leptonsIsoTight,
		  vector<leptonContainer> & softMuons,
		  vector<jetContainer> & RecoJets,
		  vector<jetContainer> & GenJets,
		  trackJetEvent & trackEvent,
		  TLorentzVector & L_met,
		  const string & systematicName,
		  const float & eventFakeWeight
                ){

}


// 2D histograms                                                                                                                                                                
void fillHisto2DWZ ( plotter & analysisPlots,
		     const string & sampleName,
		     const int    & samplePosition,
		     const string & cutLayerName,
		     vector<variableContainer2D> & VariableList2D,
		     vector<leptonContainer> & leptonsIsoTight,
		     vector<leptonContainer> & softMuons,
		     vector<jetContainer> & RecoJets,
		     vector<jetContainer> & GenJets,
		     trackJetEvent & trackEvent,
		     TLorentzVector & L_met,
		     const string & systematicName,
		     const float & eventFakeWeight){

}


//// method in  order to apply cuts                                                                                                                                             
bool passCutContainerSelection (cutContainerWZ & Cut,
				const string & sampleName,
				const int    & samplePosition,
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
				const string & finalStateString,
				const float & eventWeight){

  return true;
}



bool passCutContainerSelection (readTree* reader,
				cutContainerWZ & Cut,
				const string & sampleName,
				const int    & samplePosition,
				const bool   & usePuppiAsDefault,
				const float & minPtLeptonCut,
				const float & minPtLeptonCutCleaning,
				const float & leptonIsoCut_mu,
				const float & leptonIsoCut_el,
				const float & leptonIsoLooseCut,
				const float & matchingCone,
				const float & minJetCutPt,
				map<string,TH1F*> & vect,
				const string & finalStateString,
				const float & eventWeight){

  return true;
  
}

