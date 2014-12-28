#ifndef TMVATrainingClass_h
#define TMVATrainingClass_h

#include <iostream>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TROOT.h"

#include "ReadInputFile.h"
#include "readTree.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#endif

using namespace std;

class TMVATrainingClass {

 public :

  // default constructor
  TMVATrainingClass(){};

  // constructor from list of files
  TMVATrainingClass(const vector<TFile*> & signalFileList,  // list of TFile for the signal
                    const vector<TFile*> & backgroundFileList, // list of TFile for background
                    const string & TreeName,                // string name for signal tree
                    const string & outputFilePath ,         // output path for the TMVA files
		    const string & outputFileName,          // output file name
		    const string & Label,                   // label for the output file
		    const string & transformation = "");    // transformation to be applied on the input variables

  // constructor from list of trees
  TMVATrainingClass(const vector<TTree*> & signalTreeList,     // signal tree list
		    const vector<TTree*> & backgroundTreeList, // background tree list
		    const string & TreeName,                   // tree name
		    const string & outputFilePath ,            // output path for storing TMVA files
		    const string & outputFileName,             // output file name
		    const string & Label,                      // label
		    const string & transformation = "");       // transformation to be applied on the input variables

  // default de-constructor
  ~TMVATrainingClass();

  // Set global event weight for signal and background
  void BookMVATrees ( const vector<double> & signalGlobalWeight,        // global cross section weight for each signal file or tree
                      const vector<double> & backgroundGlobalWeight) ;  // global cross section weight for each background file or tree

  // Set Training and Spectator Variables
  void AddTrainingVariables ( const vector<string> & mapTrainingVariables,   // input list of training variables
                              const vector<string> & mapSpectatorVariables); // input list of spectator ones	       

  void AddTrainingVariables ( const string          & mapTrainingVariables,   // name of training variable
                              const vector<string>  & mapSpectatorVariables); // spectator ones	       

  // prepare events for training
  void AddPrepareTraining (const cutContainer & cutContainer,     // cut to be applied on the input trees
                           string weightStringSignal,     // re-weighting expression or branch for signal events 
                           string weightStringBackground, // re-weighting expression or branch for background events
                           const pair<int,int>  & PileUpBinOfTraining = make_pair(0,500), // pile-up bin of training
                           const int & nTraining    = 0, // number of events used for training
                           const int & nTesting     = 0, // number of events used for testing
                           const string & splitMode = "Random",  // split mode
                           const string & NormMode  = "NumEvents"); // normalization mode



  // Train rectangular cut methods
  void BookandTrainRectangularCuts    ( const string & FitMethod, // type of fit method
                                        string variable = ""); // variable
  // Train Likelihood
  void BookandTrainLikelihood         ( const string & LikelihoodType = "Likelihood"); // Likelihood training
  // Train Linear Discriminant
  void BookandTrainLinearDiscriminant (); // linear discriminant
  // Train Fisher Discriminant
  void BookandTrainFisherDiscriminant (); // fisher discriminant

  // Train MLP
  void BookandTrainMLP ( const int & nCycles            = 500, 
                         const string & HiddenLayers    = "N+5", 
                         const string & NeuronType      = "tanh",
                         const string & TrainingMaethod = "BP",
                         const int & TestRate           = 10, 
                         const int & ConvergenceTests   = 10);

  // Train Clemont Ferrand ANN
  void BookandTrainCFMlpANN           ( const int & nCycles         = 1000, 
                                        const string & HiddenLayers = "N+5") ;

  // Train TMVA ANN
  void BookandTrainTMlpANN            ( const int & nCycles              = 500, 
                                        const string & HiddenLayers      = "N+5",
                                        const string & TrainingMethod    = "BFGS", 
                                        const float & ValidationFraction = 0.3);

  // Train BDT
  void BookandTrainBDT                ( const int & NTrees            = 400, 
                                        const bool & optimizeMethod   = false, 
                                        const string & BoostType      = "AdaBoost", 
                                        const float & AdaBoostBeta    = 0.5, 
                                        const string & PruneMethod    = "NoPruning", 
                                        const int & PruneStrength     = 5, 
                                        const int & MaxDepth          = 5, 
                                        const string & SeparationType = "GiniIndex");

  // Train Gradient BDT
  void BookandTrainBDTG               ( const int & NTrees                 = 300, 
                                        const bool & optimizeMethod        = false, 
                                        const float & GradBaggingFraction  = 0.5, 
                                        const string & PruneMethod    = "NoPruning", 
					const int & PruneStrength          = 5,
                                        const int & MaxDepth               = 5, 
					const string & SeparationType = "GiniIndex");

  // Train Mit-Fisher BDT
  void BookandTrainBDTF               ( const int & NTrees            = 300, 
					const bool & optimizeMethod   = false, 
					const string & BoostType = "AdaBoost", 
                                        const float & AdaBoostBeta    = 0.5, 
					const string & PruneMethod = "CostComplexity", 
					const int & PruneStrength     = 5,
                                        const int & MaxDepth          = 5, 
					const string & SeparationType = "GiniIndex");


  // fill vector with variables values after cut given the list name
  void FillVariablesNtupla(vector<float> & variableValue, const vector<string> & variableList);

  // Close the output file
  void CloseTrainingAndTesting (){ outputFile_->Close();}

  // Set Signal Tree giving file
  void SetSignalTree (const vector<TFile*> & signalFileList,  
                      const string & TreeName = "easyDelphes");

  // Set Signal Tree giving tree
  void SetSignalTree (const vector<TTree*> & signalTreeList);

  // Set Background Tree giving file
  void SetBackgroundTree (const vector<TFile*> & backgroundFileList, 
                          const string & TreeName = "easyDelphes");

  // Set Background Tree giving tree
  void SetBackgroundTree (const vector<TTree*> & backgroundTreeList);

  // Set the training variables name
  void SetTrainingVariables  (const vector<string > & mapTrainingVariables);

  // Set the spectator variables name
  void SetSpectatorVariables (const vector<string > & mapSpectatorVariables);

  // Set the output file Name
  void SetOutputFile ( const string & outputFilePath , 
                       const string & outputFileName );

  // Set the name of the tree
  void SetTreeName ( const string & TreeName );

  // Set Label for the output file
  void SetLabel ( const string & Label );

  // Set Global Event re-weight due to the luminosity
  void SetGlobalSampleWeight (const vector<double> & signalGlobalWeight, 
                              const vector<double> & backgroundGlobalWeight) ;

  // Set Event re-weight : pile-Up, efficiency, cps, interference, btag .. etc
  void SetEventWeight ( const string & weightStringSignal, 
                        const string & weightStringBackground) ;

  //Set transformation to be applied on variables. By default no transformation
  void SetTransformations (const string & transformations = "");

  //Set basic event info for cut
  void SetBasicEventCutInfo ( const bool & usePuppiAsDefault = false,
			      const double & minPtLeptonCut  = 10,
			      const double & minPtLeptonCutCleaning = 15,
			      const double & leptonIsoCut      = 0.4,
			      const double & leptonIsoLooseCut = 0.7,
			      const double & matchingCone      = 0.3,
			      const double & minJetCutPt       = 30
			    );

 private : 

  // basic cut info
  bool   usePuppiAsDefault_;
  double minPtLeptonCut_;
  double minPtLeptonCutCleaning_;
  double leptonIsoCut_;
  double leptonIsoLooseCut_;
  double matchingCone_;
  double minJetCutPt_;

  // PU range of training 
  pair<double,double> npuRange_ ;

  // list of trees for signal and background
  vector<TTree*> signalTreeList_ ;
  vector<TTree*> backgroundTreeList_ ;

  // list of trees for signal and background after cut
  vector<TNtuple*> signalTNtuplaForTraining_ ;
  vector<TNtuple*> backgroundTNtuplaForTraining_ ;

  //cut container
  cutContainer cutEvent_;
  
  // list of input and spectator variables
  vector<string> mapTrainingVariables_ ;
  vector<string> mapSpectatorVariables_ ;

  // Global re-weight for luminosity
  vector<double> signalGlobalWeight_ ;
  vector<double> backgroundGlobalWeight_ ;
   
  // TreeName
  string TreeName_ ;

  // Label
  string Label_ ;

  //Transformations
  string transformations_;

  // outputFilePath
  string outputFilePath_ ;

  // output Name
  string outputFileName_ ;

  // output Complete Name = path + Name
  string outputFileNameComplete_ ;

  // Name of the final file xml with the weights
  map<string,string> outputFileWeightName_ ;

  // output file
  TFile* outputFile_ ;

  // factory object
  TMVA::Factory* factory_ ; 

  // readTree
  readTree* reader_;

};

#endif
