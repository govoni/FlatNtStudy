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
#include "TDirectory.h"
#include "TClass.h"
#include "TObject.h"
#include "TKey.h"
#include "TSystem.h"

#include "ReadInputFile.h"
#include "utils.h"
#include "dumpObjectsFromTree.h"
#include "readTree.h"
#include "fakeBackgroundUtils.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/MsgLogger.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#endif

using namespace std;

template <class T>
class TMVATrainingClass {

 public :

  // default constructor
  TMVATrainingClass(){};

  // constructor from list of files
  TMVATrainingClass(const map<sampleContainer,vector<shared_ptr<TFile>> > & signalFileList,  // list of TFile for the signal
                    const map<sampleContainer,vector<shared_ptr<TFile>> > & backgroundFileList, // list of TFile for background
                    const string & TreeName,                // string name for signal tree
                    const string & outputFilePath ,         // output path for the TMVA files
		    const string & outputFileName,          // output file name
		    const string & Label,                   // label for the output file
		    const string & transformation = "");    // transformation to be applied on the input variables

  // constructor from list of trees
  TMVATrainingClass(const map<sampleContainer,vector<shared_ptr<TTree>> > & signalTreeList,     // signal tree list
		    const map<sampleContainer,vector<shared_ptr<TTree>> > & backgroundTreeList, // background tree list
		    const string & TreeName,                   // tree name
		    const string & outputFilePath ,            // output path for storing TMVA files
		    const string & outputFileName,             // output file name
		    const string & Label,                      // label
		    const string & transformation = "");       // transformation to be applied on the input variables

  // constructor from list of trees
  TMVATrainingClass(const map<sampleContainer,vector<shared_ptr<TChain>> > & signalChainList,     // signal tree list
		    const map<sampleContainer,vector<shared_ptr<TChain>> > & backgroundChainList, // background tree list
		    const string & TreeName,                   // tree name
		    const string & outputFilePath ,            // output path for storing TMVA files
		    const string & outputFileName,             // output file name
		    const string & Label,                      // label
		    const string & transformation = "");       // transformation to be applied on the input variables

  // default de-constructor
  virtual ~TMVATrainingClass();

  // Set global event weight for signal and background
  void BookMVATrees ( const map<sampleContainer,float> & signalGlobalWeight,        // global cross section weight for each signal file or tree
                      const map<sampleContainer,float> & backgroundGlobalWeight) ;  // global cross section weight for each background file or tree

  // Set Training and Spectator Variables
  void AddTrainingVariables ( const vector<string> & trainingVariables,   // input list of training variables
                              const vector<string> & spectatorVariables,
                              const bool & trainEachVarIndependently = false); // input list of spectator ones	       

  // prepare events for training
  void AddPrepareTraining (const T & cutContainer,     // cut to be applied on the input trees
                           string weightStringSignal,     // re-weighting expression or branch for signal events 
                           string weightStringBackground, // re-weighting expression or branch for background events
                           const pair<int,int>  & PileUpBinOfTraining = make_pair(0,500), // pile-up bin of training
			   const string & finalStateString = "UU",
			   const string & fakeRateFile = "input/FakeRate.root",
                           const int    & nTraining    = 0, // number of events used for training
                           const int    & nTesting     = 0, // number of events used for testing
                           const string & splitMode = "Random",  // split mode
                           const string & NormMode  = "NumEvents"); // normalization mode


  // Train rectangular cut methods
  void BookandTrainRectangularCuts    ( const string & FitMethod);
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
  void FillVariablesNtupla(vector<float> & variableValue, 
			   const vector<string> & variableList,
			   std::vector<leptonContainer> &,
			   std::vector<jetContainer> &,
			   TLorentzVector &,
			   readTree *,
			   const float & eventWeight = 1.);

  // Set Signal Tree giving file
  void SetSignalTree (const map<sampleContainer,vector<shared_ptr<TFile>> > & signalFileList,  
                      const string & TreeName = "easyDelphes");
  // Set Signal Tree giving tree
  void SetSignalTree (const map<sampleContainer,vector<shared_ptr<TTree>> >  & signalTreeList);
  void SetSignalTree (const map<sampleContainer,vector<shared_ptr<TChain>> > & signalChainList);

  // Set Background Tree giving file
  void SetBackgroundTree (const map<sampleContainer,vector<shared_ptr<TFile>> > & backgroundFileList, 
                          const string & TreeName = "easyDelphes");

  // Set Background Tree giving tree
  void SetBackgroundTree (const map<sampleContainer,vector<shared_ptr<TTree>> > & backgroundTreeList);
  void SetBackgroundTree (const map<sampleContainer,vector<shared_ptr<TChain>> > & backgroundChainList);

  // Set the training variables name
  void SetTrainingVariables  (const vector<string > & trainingVariables);

  // Set the spectator variables name
  void SetSpectatorVariables (const vector<string > & spectatorVariables);

  // Set the output file Name
  void SetOutputFile ( const string & outputFilePath , 
                       const string & outputFileName );

  // Set the name of the tree
  void SetTreeName ( const string & TreeName );

  // Set Label for the output file
  void SetLabel ( const string & Label );

  // Set Global Event re-weight due to the luminosity
  void SetGlobalSampleWeight (const map<sampleContainer,float> & signalGlobalWeight, 
                              const map<sampleContainer,float> & backgroundGlobalWeight) ;

  // Set Event re-weight : pile-Up, efficiency, cps, interference, btag .. etc
  void SetEventWeight ( const string & weightStringSignal, 
                        const string & weightStringBackground) ;

  //Set transformation to be applied on variables. By default no transformation
  void SetTransformations (const string & transformations = "");

  //Set basic event info for cut
  void SetBasicEventCutInfo ( const bool &  usePuppiAsDefault = false,
			      const float & minPtLeptonCut  = 10,
			      const float & minPtLeptonCutCleaning = 15,
			      const float & leptonIsoCut_mu   = 0.6,
			      const float & leptonIsoCut_el   = 0.55,
			      const float & leptonIsoLooseCut = 0.7,
			      const float & matchingCone      = 0.3,
			      const float & minJetCutPt       = 30
			    );

 private : 

  // basic cut info
  bool  usePuppiAsDefault_;
  float minPtLeptonCut_;
  float minPtLeptonCutCleaning_;
  float leptonIsoCut_mu_;
  float leptonIsoCut_el_;
  float leptonIsoLooseCut_;
  float matchingCone_;
  float minJetCutPt_;

  bool trainEachVarIndependently_;

  // PU range of training 
  pair<float,float> npuRange_ ;

  // list of trees for signal and background
  map<sampleContainer,vector<shared_ptr<TTree>> > signalTreeList_ ;
  map<sampleContainer,vector<shared_ptr<TTree>> > backgroundTreeList_ ;

  // list of trees for signal and background after cut
  map<string,vector<shared_ptr<TNtuple>>> signalTNtuplaForTraining_ ;
  map<string,vector<shared_ptr<TNtuple>>> backgroundTNtuplaForTraining_ ;

  //cut container
  T cutEvent_;
  
  // list of input and spectator variables
  vector<string> trainingVariables_ ;
  vector<string> spectatorVariables_ ;

  // Global re-weight for luminosity
  map<sampleContainer,float> signalGlobalWeight_ ;
  map<sampleContainer,float> backgroundGlobalWeight_ ;
   
  // TreeName
  string TreeName_ ;

  // Label
  string Label_ ;

  //Transformations
  string transformations_;

  // outputFilePath
  vector<string> outputFilePath_ ;

  // output Name
  vector<string> outputFileName_ ;

  // output Complete Name = path + Name
  vector<string> outputFileNameComplete_ ;

  // Name of the final file xml with the weights
  map<string,string> outputFileWeightName_ ;

  // output file
  vector<shared_ptr<TFile>> outputFile_ ;

  // factory object
  vector<shared_ptr<TMVA::Factory>> factory_ ; 

  // readTree
  readTree* reader_;

  shared_ptr<TFile> inputFileFakeRate_ ;
  shared_ptr<fakeRateContainer> fakeRate_ ;
  shared_ptr<fakeMigrationContainer> fakeMigration_;


};

#endif
