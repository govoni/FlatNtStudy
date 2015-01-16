#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TMath.h"
#include "TSystem.h"
#include "TROOT.h"

#include "ConfigParser.h"
#include "ReadInputFile.h"
#include "utils.h"
#include "TMVATrainingClass.h"

using namespace std ;

vector<string> GetVariableList( const vector<variableContainer> & vecVar){

  vector<string> varList ;
  for(size_t iVar = 0; iVar < vecVar.size(); iVar++)
    varList.push_back(vecVar.at(iVar).variableName);
  return varList;
}

/// Main Programme                                                                                                                                         
int main (int argc, char** argv){

  if (argc < 2){
    cerr << ">>> Usage:   " << argv[1] << "   cfg file" <<  endl;
    exit(EXIT_FAILURE);
  }

  TMVA::Tools::Instance();

  // parse config file parameter                                                                                                                                            
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();

  TString config ;
  config.Form("%s",argv[1]);

  if(!(gConfigParser->init(config))){
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl;
    return -1;
  }
  

  // import base directory where samples are located and txt file with the directory name + other info                                                                      
  string InputBaseDirectory   = gConfigParser -> readStringOption("Input::InputBaseDirectory");
  // import the sample list where info like: directory, cross section, numberBefore and if signal or background are stored
  string InputSampleList      = gConfigParser -> readStringOption("Input::InputSampleList");

  map<string,vector<sampleContainer> > sampleMap ;
  if(ReadInputSampleFile(InputSampleList,sampleMap) <= 0){
    cerr<<" Empty Input Sample File or not Exisisting --> Exit "<<endl; return -1;}

  // treeName                                                                                                                                                               
  string treeName ;
  try{ 
    treeName  = gConfigParser -> readStringOption("Input::TreeName");
  }
  catch(char* exceptionString){
    treeName = "easyDelphes";
    cerr<<" set treeName to the default : easyDelphes "<<endl;
  } 

  // take the cut list                                                                                                                                                         
  string InputCutList       = gConfigParser -> readStringOption("Input::InputCutList");

  vector <cutContainer> CutList;
  if(ReadInputCutFile(InputCutList,CutList) <= 0){
    cerr<<" Empty Cut List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the variable list to be plotted                                                                                                                                  
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the spectator variable
  string InputSpectatorList;
  vector<variableContainer> spectatorList;
  try {
    InputSpectatorList = gConfigParser -> readStringOption("Input::InputSpectatorList");
    if(ReadInputVariableFile(InputSpectatorList,spectatorList) <= 0 ){
      cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; 
      return -1;
    }
  }
  catch(char* exceptionString){ 
    cerr<<" spectator list not found : set to default --> npu "<<endl;
    spectatorList.push_back(variableContainer("npu",300,0,300,"N_{PU}"));
  } 

  //take input variable transformation
  string variableTransformation ;
  try {
    variableTransformation = gConfigParser -> readStringOption("Input::VariableTransformation");
    replace(variableTransformation.begin(),variableTransformation.end(),':',';');
  }
  catch(char* exceptionString){
    variableTransformation = "I;N";
  }

  // take the method to be trained
  vector<string> TrainMVAMethodName;
  try{
    TrainMVAMethodName  = gConfigParser -> readStringListOption("Input::TrainMVAMethodName");
  }
  catch(char* exceptionString){
    cerr<<" UseMethodName --> not found --> exit from the code "<<endl; 
    exit(EXIT_FAILURE) ; 
  }

  // label in order to name the output file
  string Label;
  try{
    Label = gConfigParser -> readStringOption("Input::Label");
  }
  catch(char* exceptionString){
    cerr<<" Label set to Test "<<endl; 
    Label = "Test"; 
  }

  // other option parameters : luminosity
  float lumi ;
  try {
    lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)                                                                                            
  }
  catch(char* excpetionString) {
    lumi = 1000;
    cerr<<" lumi set to default value : 1000 fb^{-1} "<<endl;
  }
  lumi *= 1000. ;   // transform into pb^(-1)                                                                                                                                  

  // other option parameter: basic cuts
  float matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone");
  float minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt");
  float minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  float minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  float usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  float leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  float leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  float leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");

  // option in order to train each variable independently
  bool isTrainEachVariable = false ;
  try {
    isTrainEachVariable  = gConfigParser -> readBoolOption("Option::TrainEachVariable");
  }
  catch(char* exceptionString){
    cout<<" TrainEachVariable --> set to false "<<endl;
  }

  // string to be used to re-weight the events
  string eventWeight ;
  try{ 
    eventWeight  = gConfigParser -> readStringOption("Option::eventWeight");
  }
  catch(char* exceptionString){
    cerr<<" eventWeight --> not found --> empty string "<<endl;
  }

  vector<int> pileUpBin;
  try{
    pileUpBin  = gConfigParser -> readIntListOption("Option::pileUpBin");
  }
  catch(char* exceptionString){
    cout<<" pileUpBin --> not found --> set 1 bin [0,100] "<<endl;
    pileUpBin.push_back(0);
    pileUpBin.push_back(2000);
  }

  // output directory                                                                                                                                                       
  string outputFileDirectory = gConfigParser -> readStringOption("Output::outputFileDirectory");
  system(("mkdir -p output/"+outputFileDirectory).c_str());
  outputFileDirectory = "output/"+outputFileDirectory;

  string outputFileName ;
  try{
    outputFileName = gConfigParser -> readStringOption("Output::outputFileName"); 
  }
  catch(char* exceptionString){
    cout<<" Default output file name "<<endl; 
    outputFileName = "TMVAoutput";
  }

 
  // Import Sample and signal - background collections  
  vector <TChain*> signalChainOriginal;
  vector <TChain*> backgroundChainOriginal;
  vector <TChain*> signalChain;
  vector <TChain*> backgroundChain;

  cout<<"Building Tree List for Signal And Background  "<<endl;

  map<string,vector<sampleContainer> >::iterator itBackgroundSample = sampleMap.begin(); // loop on the sample map
  for( ; itBackgroundSample != sampleMap.end() ; ++itBackgroundSample){
    if(itBackgroundSample->second.at(0).isSignal == 1) continue ; // skip if signal

    backgroundChainOriginal.push_back(new TChain (treeName.c_str()));
    backgroundChain.push_back(new TChain (treeName.c_str()));

    int numBefore = 0;

    // take input files                                                                                                                                                        
    for(size_t iContainer = 0; iContainer < itBackgroundSample->second.size(); iContainer++){  // for each reduced name loop on the different content
      numBefore += itBackgroundSample->second.at(iContainer).numBefore;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*1.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*2.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*3.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*4.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*5.root").c_str()) ;
    }

    
    TObjArray *fileElements = 0;
    fileElements = backgroundChainOriginal.back()->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chElement = 0;
    while (( chElement = (TChainElement*) next())) {
      TFile* fFileTemp = 0;
      fFileTemp =  TFile::Open(chElement->GetTitle(),"READ");
      if (!fFileTemp) continue ;                            //file does not exist
      if (fFileTemp->IsZombie()){
        if(fFileTemp!=0) fFileTemp->Delete();
        continue;                  //file is unusable
      }      
      if (fFileTemp->TestBit(TFile::kRecovered)){
	if(fFileTemp!=0) fFileTemp->Delete();
	continue;  //file has been recovered
      }
      backgroundChain.back()->Add(chElement->GetTitle());
      if(fFileTemp!=0) fFileTemp->Delete();     
    }
    
    int totEvent = backgroundChain.back()->GetEntries();
    // add  sample to the analysis plot container                                                                                                                              
    if(numBefore < totEvent) itBackgroundSample->second.at(0).numBefore = totEvent ;
    cout<<"Background Base Sample Name : "<<itBackgroundSample->first<<" Num Events Before "<<itBackgroundSample->second.at(0).numBefore<<" XS "<<itBackgroundSample->second.at(0).xsec<<" lumi "<<lumi<<endl;
    if(fileElements!=0) fileElements->Delete();
   
  }

  for_each(backgroundChainOriginal.begin(),backgroundChainOriginal.end(), default_delete<TChain>());
  backgroundChainOriginal.clear();
  
  // signal
  map<string,vector<sampleContainer> >::iterator itSignalSample = sampleMap.begin(); // loop on the sample map
  for( ; itSignalSample != sampleMap.end() ; ++itSignalSample){
    if(itSignalSample->second.at(0).isSignal == 0) continue ; // skip if signal

    signalChainOriginal.push_back(new TChain (treeName.c_str()));
    signalChain.push_back(new TChain (treeName.c_str()));

    int numBefore = 0;

    // take input files                                                                                                                                                        
    for(size_t iContainer = 0; iContainer < itSignalSample->second.size(); iContainer++){  // for each reduced name loop on the different content
      numBefore += itSignalSample->second.at(iContainer).numBefore;
      signalChainOriginal.back()->Add ((InputBaseDirectory+"/"+itSignalSample->second.at(iContainer).sampleName+"/*1.root").c_str()) ;
      signalChainOriginal.back()->Add ((InputBaseDirectory+"/"+itSignalSample->second.at(iContainer).sampleName+"/*2.root").c_str()) ;
      signalChainOriginal.back()->Add ((InputBaseDirectory+"/"+itSignalSample->second.at(iContainer).sampleName+"/*3.root").c_str()) ;
      signalChainOriginal.back()->Add ((InputBaseDirectory+"/"+itSignalSample->second.at(iContainer).sampleName+"/*4.root").c_str()) ;
    }

    TObjArray *fileElements = 0;
    fileElements = signalChainOriginal.back()->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chElement = 0;
    while (( chElement = (TChainElement*) next())) {
      TFile* fFileTemp = 0;
      fFileTemp = TFile::Open(chElement->GetTitle(),"READ");
      if (!fFileTemp) continue ;                            //file does not exist
      if (fFileTemp->IsZombie()){
        if(fFileTemp!=0) fFileTemp->Delete();
        continue;                  //file is unusable
      }
      if (fFileTemp->TestBit(TFile::kRecovered)){
	if(fFileTemp!=0) fFileTemp->Delete();
	continue;  //file has been recovered
      }
      signalChain.back()->Add(chElement->GetTitle());
      if(fFileTemp!=0) fFileTemp->Delete();
    }

    int totEvent = signalChain.back()->GetEntries();
    // add  sample to the analysis plot container                                                                                                                              
    if(numBefore < totEvent) itSignalSample->second.at(0).numBefore = totEvent ;
    if(fileElements!=0) fileElements->Delete();
    cout<<"Signal Base Sample Name : "<<itSignalSample->first<<" Num Events Before "<<itSignalSample->second.at(0).numBefore<<" XS "<<itSignalSample->second.at(0).xsec<<" lumi "<<lumi<<endl;
  }

  for_each(signalChainOriginal.begin(),signalChainOriginal.end(), default_delete<TChain>());
  signalChainOriginal.clear();
  
  // Book MVA Training Object --> one for each PU bin 
  vector<TMVATrainingClass*> trainingVector ;

  // Set Global Weight and signal + background Tree for MVA Training                                                                                                       
  vector<float> signalGlobalWeight;
  vector<float> backgroundGlobalWeight;

  
  for(size_t puBin = 0; puBin+1 < pileUpBin.size() ; puBin++){

    cout<<"PU Bin of Training: Min = "<<pileUpBin.at(puBin)<<" Max = "<<pileUpBin.at(puBin+1)<<endl;
    
    TString LabelTemp ;
    if(pileUpBin.size() > 2) 
      LabelTemp = Form("%s_PU_%d_%d",Label.c_str(),int(pileUpBin.at(puBin)),int(pileUpBin.at(puBin+1))) ;
    else Label = LabelTemp ;
    
    for(size_t iCut = 0 ; iCut < CutList.size(); iCut++){

      if(CutList.size() > 1 ) outputFileName += "_"+CutList.at(iCut).cutLayerName;

      trainingVector.push_back(new TMVATrainingClass(signalChain, 
						     backgroundChain, 
						     treeName, 
						     outputFileDirectory, 
						     outputFileName,
						     Label,
						     ":Transformations="+variableTransformation));
      // Set Input and Spectator Variables
      cout<<endl;
      cout<<"Set Training and Spectator Variables  "<<endl;
      cout<<endl;
      
      trainingVector.back()->AddTrainingVariables(GetVariableList(variableList),GetVariableList(spectatorList),isTrainEachVariable);
      
      // Set Global Weight and signal + background Tree for MVA Training
      signalGlobalWeight.clear();
      backgroundGlobalWeight.clear();
      signalGlobalWeight.assign(signalChain.size(),0.);
      backgroundGlobalWeight.assign(backgroundChain.size(),0.);
	  
      cout<<"Building Global Event Weight  + Add Trees "<<endl;
      cout<<endl; 
	    
      int iBackg = 0;
      map<string,vector<sampleContainer> >::iterator itBackgroundSample = sampleMap.begin();
      for( ; itBackgroundSample != sampleMap.end() ; itBackgroundSample++){          
	if(itBackgroundSample->second.at(0).isSignal == 1) continue ; // skip if signal
	backgroundGlobalWeight.at(iBackg) = itBackgroundSample->second.at(0).xsec*lumi/(float(itBackgroundSample->second.at(0).numBefore));
	cout<<"Backg "<<itBackgroundSample->first<<" weight "<<backgroundGlobalWeight.at(iBackg)<<endl;
	iBackg++;
      }

      int iSignal = 0;
      map<string,vector<sampleContainer> >::iterator itSignalSample = sampleMap.begin();
      for( ; itSignalSample != sampleMap.end() ; itSignalSample++){          
	if(itSignalSample->second.at(0).isSignal == 0) continue ; // skip if signal
	signalGlobalWeight.at(iSignal) = itSignalSample->second.at(0).xsec*lumi/(float(itSignalSample->second.at(0).numBefore));
	cout<<"Signal "<<itSignalSample->first<<" weight "<<signalGlobalWeight.at(iSignal)<< endl;
	iSignal++;
      }

      // Prepare and Set the MVA Factory
      cout<<endl;
      cout<<"Prepare MVA  "<<endl;
      cout<<endl;

      //set the basic cut information
      trainingVector.back()->SetBasicEventCutInfo(usePuppiAsDefault,
						  minLeptonCutPt,
						  minLeptonCleaningPt,
						  leptonIsoCut_mu,
						  leptonIsoCut_el,
						  leptonIsoCutLoose,
						  matchingCone,
						  minJetCutPt);
      
      
      //prepare the TNtuple looping on the events before the training
      trainingVector.back()->AddPrepareTraining(CutList.at(iCut),
						eventWeight,
						eventWeight,
						make_pair(pileUpBin.at(puBin),pileUpBin.at(puBin+1))
						);

      // Add the ntuple to the MVA factory with the right cross section weight
      trainingVector.back()->BookMVATrees(signalGlobalWeight, 
					  backgroundGlobalWeight);  
     
      
      // Book and Run TMVA Training and testing for the selected methods
      cout<<"Loop on the Selected Methods  "<<endl;
      cout<<endl;

      for(size_t iMethod = 0; iMethod < TrainMVAMethodName.size(); iMethod++){
	// Rectangular Cuts
	if(TrainMVAMethodName.at(iMethod) == "CutsMC" )      
	  trainingVector.back()->BookandTrainRectangularCuts("MC");
	else if(TrainMVAMethodName.at(iMethod) == "CutsGA" ) 
	  trainingVector.back()->BookandTrainRectangularCuts("GA");
	else if(TrainMVAMethodName.at(iMethod) == "CutsSA" ) 
	  trainingVector.back()->BookandTrainRectangularCuts("SA");
	// Likelihoods
	else if(TrainMVAMethodName.at(iMethod) == "Likelihood")     
	  trainingVector.back()->BookandTrainLikelihood(); 
	else if(TrainMVAMethodName.at(iMethod) == "LikelihoodKDE")  
	  trainingVector.back()->BookandTrainLikelihood("LikelihoodKDE"); 
	else if(TrainMVAMethodName.at(iMethod) == "PDERS")          
	  trainingVector.back()->BookandTrainLikelihood("PDERS"); 
	else if(TrainMVAMethodName.at(iMethod) == "PDEFoam")        
	  trainingVector.back()->BookandTrainLikelihood("PDEFoam"); 
	else if(TrainMVAMethodName.at(iMethod) == "PDEFoamBoost")   
	  trainingVector.back()->BookandTrainLikelihood("PDEFoamBoost"); 	    
	// Fisher Discriminant
	else if(TrainMVAMethodName.at(iMethod) == "Fisher")  
	  trainingVector.back()->BookandTrainFisherDiscriminant(); 
	// Linear Discriminant
	else if(TrainMVAMethodName.at(iMethod) == "LD")      
	  trainingVector.back()->BookandTrainLinearDiscriminant();
	// MLP
	else if(TrainMVAMethodName.at(iMethod) == "MLP")        
	  trainingVector.back()->BookandTrainMLP();
	else if(TrainMVAMethodName.at(iMethod) == "MLPBFG")     
	  trainingVector.back()->BookandTrainMLP(1000,"N+5","sigmoid","BFGS",10,10);
	else if(TrainMVAMethodName.at(iMethod) == "CFMlpANN")   
	  trainingVector.back()->BookandTrainCFMlpANN();
	else if(TrainMVAMethodName.at(iMethod) == "TMlpANN")    
	  trainingVector.back()->BookandTrainTMlpANN();
	// BDT
	else if(TrainMVAMethodName.at(iMethod) == "BDT")     
	  trainingVector.back()->BookandTrainBDT();
	// BDTG
	else if(TrainMVAMethodName.at(iMethod) == "BDTG")    
	  trainingVector.back()->BookandTrainBDTG();
	// BDTF
	else if(TrainMVAMethodName.at(iMethod) == "BDTF")    
	  trainingVector.back()->BookandTrainBDTF();
	else { 
	  cerr<<"Training Method not implemented in the TMVATrainingClass --> Go to the next one"<<endl; 
	  cout<<endl;
	}
      }

      trainingVector.back()->CloseTrainingAndTesting();
     
    }

  } 
    
  return 0 ;
}
