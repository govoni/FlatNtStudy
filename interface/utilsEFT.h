#ifndef utilsEFT_h
#define utilsEFT_h

#include <vector>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "readTreeEFT.h"
#include "ReadInputFile.h"
#include "utils.h"

using namespace std ;

#define pdgWMass   80.385
#define pdgTopMass 173.21
#define softMuonPt 3.



class histoContainerEFT {

 public :

  histoContainerEFT(){};

  ~histoContainerEFT(){};

 histoContainerEFT(string cutName, variableContainer container, int nGridPoints):
  cutName(cutName),
  varName(container.variableName),
    nGridPoints(nGridPoints){
    for( int iGrid = 0; iGrid < nGridPoints; iGrid++){
      histogramEFT.push_back(new TH1F((cutName+"_"+varName+"_eft_"+to_string(iGrid)).c_str(),"",container.Nbin,container.min,container.max));
      histogramEFT.back()->GetXaxis()->SetTitle(container.label.c_str());
      histogramEFT.back()->StatOverflows(1);
      histogramEFT.back()->Sumw2();
    }

  }



 histoContainerEFT(string cutName, variableContainerDynamic container, int nGridPoints):
  cutName(cutName),
  varName(container.variableName),
    nGridPoints(nGridPoints){

    for( int iGrid = 0; iGrid < nGridPoints; iGrid++){
      histogramEFT.push_back(new TH1F((cutName+"_"+varName+"_eft_"+to_string(iGrid)).c_str(),"",container.Nbin,&container.binning[0]));
      histogramEFT.back()->GetXaxis()->SetTitle(container.label.c_str());
      histogramEFT.back()->StatOverflows(1);
      histogramEFT.back()->Sumw2();
    }

  }

  bool operator == (const histoContainerEFT & plot2) const {
    if(plot2.cutName == cutName and plot2.varName == varName ) return true;
    else return false;
  }

  bool findCutByLabel ( const string & cutString){
    TString name = Form("%s",cutName.c_str());
    if(name.Contains(cutString.c_str())) return true;
    else return false;

  }

  string cutName;
  string varName;
  int    nGridPoints;
  vector<TH1F*> histogramEFT;
};



// fill Reco Jet Array from the readTree object
void fillRecoJetArray  (vector<jetContainer> & jetVector, 
			readTreeEFT & reader) ;

void fillPuppiJetArray (vector<jetContainer> & jetVector, 
			readTreeEFT & reader) ;

void fillGenJetArray   (vector<jetContainer> & jetVector, 
			readTreeEFT & reader) ;

void fillRecoLeptonsArray (vector<leptonContainer> & jetVector, 
			   readTreeEFT & reader) ;

void fillTrackJetArray (vector<jetContainer> & jetVector, 
			readTreeEFT & reader) ;


bool passCutContainerSelection (readTreeEFT* readTree,
                                cutContainer & Cut,
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
				const string & finalStateString
				);


void fillEFTWeights(vector<TH1F*> & histogramEFT, 
		    const float & value, 
		    const float & eventWeight, 
		    readTreeEFT* reader);


#endif
