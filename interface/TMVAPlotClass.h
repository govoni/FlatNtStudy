// global TMVA style settings                                                                                                                                               
#ifndef TMVAPlotClass_H
#define TMVAPlotClass_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <istream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <memory>

using namespace std;

#include "TPad.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TSystem.h"
#include "TImage.h"
#include "TKey.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObjArray.h"
#include "TClass.h"
#include "TH2F.h"
#include "TPaveText.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TNamed.h"
#include "TFormula.h"
#include "RVersion.h"
#include "TPaletteAxis.h"

// default definition for ROC curve color, style and width                                                                                                                      
static float color []      = {1,2,4,210,6,7,9,12,3,95,28,5,46,40,55,130};
static float linestyle []  = {1,1,8,9,7,2,5,9,1,6,10,8,7,1,2,5};
static float linewidth []  = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

static std::vector<float> vec_color     (color,color+sizeof(color)/sizeof(float));
static std::vector<float> vec_linewidth (linewidth, linewidth + sizeof(linewidth)/sizeof(float));
static std::vector<float> vec_linestyle (linestyle, linestyle + sizeof(linestyle)/sizeof(float));

// significance Box class 
class significanceBox {
 
 public:

  //default constructor
  significanceBox(){
    Signal_     = NULL ;
    Background_ = NULL ;
    efficiencySignal_     = NULL ;
    efficiencyBackground_ = NULL ;  
    significance_ = NULL;
    maxSig_    = 0; 
    maxSigErr_ = 0; 
    maxBin_    = 0;
  }

  // defailt de-constructor
  ~significanceBox(){
  if(Signal_!=NULL)     delete Signal_;
    if(Background_!=NULL) delete Background_;
  if(efficiencySignal_!=NULL)     delete efficiencySignal_;
  if(efficiencyBackground_!=NULL) delete efficiencyBackground_;
  if(significance_!=NULL) delete significance_ ;
  } 

  // public members 
 
  // signal and background distribution
  TH1* Signal_;
  TH1* Background_;

  // signal and background efficiency
  TH1* efficiencySignal_;
  TH1* efficiencyBackground_;

  // chosen significance
  TH1F* significance_;  

  // method Name and Title
  TString methodName_;
  TString methodTitle_;

  // maxSignificance and error
  float maxSig_ ;
  float maxSigErr_ ;
  
  // bin related to the maximum
  int    maxBin_ ;

};

// class for global manipulation of TMVA output + ROC, correlation, output and significance plots
class TMVAPlotClass {

  public:  
  
  // default constructor
  TMVAPlotClass(); 
  // default deconstructor
  ~TMVAPlotClass();
 
  // type of plot for TMVA output
  enum TypeOfPlot { kId = 0, 
		    kNorm = 1, 
		    kDecorrelated = 2, 
		    kPCA = 3, 
		    kGaussDecorr = 4, 
		    kNumOfMethods = 5 
  };

  // type of plot for TMVA output
  enum HistType { MVAType = 0, 
		  ProbaType = 1, 
		  RarityType = 2, 
		  CompareType = 3 
  };

  // type of significance to plot
  enum SignificanceType { SoverB = 0, 
			  SoverSqrtB = 1, 
			  SoverSqrtSB = 2, 
			  Pvalue = 3
  };

  //////////////////////////////////////// Input Files

  // open one input file given the name
  void openFileInput( const string & fin );

  // open a set of input file and store them in the class
  void openFileInput( const vector<string> & fin );

  // get back a single file from the name
  TFile* GetInputFile (const string & fin );

  // get back the whole set of file considered
  vector<TFile*> GetInputFile();


  ////////////////////////////////////// Canvas methods

  // destroy exsisting canvas
  void DestroyCanvases(); 

  // create canvas and frame for the ROC plot
  void CreateCanvasandFrameROC(const int & plotType, 
			       const float & puMin, 
			       const float & puMax
			       );
  /// banner inside the ROC plot
  void banner4Plot (const bool & isLabel,
		    const float & puMin, 
		    const float & puMax, 
		    const int & plotType);

  // Print ROC Curve plot
  void PrintImageROC( TDirectory* dir = 0 , 
		      const string & outputPlotDirectory = ""
		      );    

  // Print a generic plot
  void PrintImage   ( TCanvas* c = 0, 
		      const string & outputPlotDirectory = ""
		      );    


  /////////// Normalize a given histogram
  void NormalizeHist ( TH1F* h );
  void NormalizeHists( TH1F* sig, TH1F* bkg = 0 );

  /////////// Get Method Name and Title
  void GetMethodName ( TString & name, 
		       TKey * mkey );

  void GetMethodTitle( TString & name, 
		       TKey * ikey );

  void GetMethodName ( TString & name, 
		       TDirectory * mdir );

  void GetMethodTitle( TString & name, 
		       TDirectory * idir );

  //// GetList of Methods, Title and Jobs
  int GetListOfMethods( TList & methods, 
			TDirectory *dir = 0 );

  int GetListOfTitles ( TDirectory *rfdir, 
			TList & titles );

  int GetListOfTitles ( TString & methodName, 
			TList & titles, 
			TDirectory *dir = 0 );

  // given the iterator on the file and a class name, get the next key
  TKey *NextKey   ( TIter & keyIter, 
		    string className);

  // get the inputVariable directory and the correlation plot directory
  TDirectory *GetInputVariablesDir  ( TMVAPlotClass::TypeOfPlot type, 
				      TDirectory *dir = 0 );

  /// Set the name of the method
  void SetMethodName (const vector<string> & MethodName);

  /// Set input files
  void SetInputFiles (const vector<TFile*> & inputFiles);

  // Read signal and background histogramm and effciency given an input file
  void ReadHistograms(TFile* file);

  // set the formula for the significance
  void SetFormula(const TString & f);

  // Get the formula back for calculation or latex banner
  TString GetFormula();
  TString GetFormulaString();
  TString GetLatexFormula();

  // set if use just signal or bkg efficiency or multiply them for the expected number of events before the training to get the real significance
  void SetSignalType    (const bool & type = false);
  void SetBackgroundType(const bool & type = false);

  // plot ROC curve
  void makeROCsPlot (TDirectory* dir, 
		     const int & = 0, 
		     const float & minPU = 0, 
		     const float & maxPU = 1000, 
		     const string & outputPlotDirectory = "");

  // plot correlation Matrix
  void makeCorrelationMatrixPlot (TFile* inputFile = 0, 
				  const string & inputName = "", 
				  const string & outputPlotDirectory = "");

  // plot output distribution
  void makeMVAsPlot( TFile* inputFile = 0, 
		     HistType htype = MVAType, 
		     const string & outputPlotDirectory = "");

  // plot signficance for different formula using or not the expected number of signal and background events
  void makeSignificancePlot (TFile* inputFile = 0, 
			     const string & inputName = "", 
			     SignificanceType stype = SoverSqrtB, 
			     const float & numberSignalEvents = 0., 
			     const float & numberBackgroundEvents = 0.,
			     const bool & UseSignalEfficiency = false, 
			     const bool & UseBakgroundEfficiency = false, 
			     const string & outputPlotDirectory = "");

 private:
  
  // indexes for color, method, mvasplot
  int color_index_ ;
  int method_index_;
  int mvas_index_;
  int thisMethod_;

  // list of the input files, inputMethods
  vector<TFile*> inputFiles_ ;
  vector<string> inputMethodName_ ;
  vector<string> originalMethodName_ ;


  // canvas for ROC, correlation, MVA and significance
  TCanvas* cROC_ ;
  TCanvas* cROCLog_ ;
  TH2F*    frameROC_ ; 
  TH2F*    frameROCLog_ ; 
  TLegend* legROC_ ;

  // vector of latex element of a canvas
  vector<TLatex*> canvasLabel_ ;

  TPaveText* bannerROC_ ;
  
  TCanvas* cCorrelationSignal_;
  TCanvas* cCorrelationBackground_;
  TCanvas* cMVAs_ ;
  TCanvas* cSignificance_ ;
 
  // formula string
  TString fFormula_ ;

  // store if use just efficiency or also the S and B yields
  bool    signalType_ ;
  bool    backgroundType_ ;

  // significance box tool
  vector<significanceBox*>  fSignificanceBox_;

  TH1* histoSignal_;
  TH1* histoBackground_;
  TH1* effBackground_;
  TH1* effSignal_;


};

#endif
