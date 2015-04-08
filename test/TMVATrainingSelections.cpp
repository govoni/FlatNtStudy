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

  // take the sample map with all the information
  map<string,vector<sampleContainer> > sampleMap ;
  if(ReadInputSampleFile(InputSampleList,sampleMap) <= 0){
    cerr<<" Empty Input Sample File or not Exisisting --> Exit "<<endl; return -1;}

  // treeName                                                                                                                                                               
  string treeName  = gConfigParser -> readStringOption("Input::TreeName");

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
  InputSpectatorList = gConfigParser -> readStringOption("Input::InputSpectatorList");
  if(ReadInputVariableFile(InputSpectatorList,spectatorList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; 
    return -1;
  }

  //take input variable transformation
  string variableTransformation = gConfigParser -> readStringOption("Input::VariableTransformation");
  replace(variableTransformation.begin(),variableTransformation.end(),':',';');

  // take the method to be trained
  vector<string> TrainMVAMethodName  = gConfigParser -> readStringListOption("Input::TrainMVAMethodName");

  // label in order to name the output file
  string Label = gConfigParser -> readStringOption("Input::Label");

  // other option parameters : luminosity
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)                                                                                            
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
  string finalStateString   = gConfigParser -> readStringOption("Option::finalStateString");

  // option in order to train each variable independently
  bool isTrainEachVariable  = gConfigParser -> readBoolOption("Option::TrainEachVariable");

  // string to be used to re-weight the events
  string eventWeight  = gConfigParser -> readStringOption("Option::eventWeight");

  // binning in PU for training
  vector<int> pileUpBin  = gConfigParser -> readIntListOption("Option::pileUpBin");

  // fake rate file for fake background
  string fakeRateFile = gConfigParser -> readStringOption("Option::fakeRateFile");

  // output directory                                                                                                                                                       
  string outputFileDirectory = gConfigParser -> readStringOption("Output::outputFileDirectory");
  system(("mkdir -p output/"+outputFileDirectory).c_str());
  outputFileDirectory = "output/"+outputFileDirectory;

  string outputFileName = gConfigParser -> readStringOption("Output::outputFileName"); 

 
  // Import Sample and signal - background collections  
  map<sampleContainer,vector <shared_ptr<TChain> > > signalMap;
  map<sampleContainer,vector <shared_ptr<TChain> > > backgroundMap;

  vector <shared_ptr<TChain> > signalChainOriginal;
  vector <shared_ptr<TChain> > signalChain;
  vector <shared_ptr<TChain> > backgroundChainOriginal;
  vector <shared_ptr<TChain> > backgroundChain;

  cout<<"Building Tree List for Signal And Background  "<<endl;

  map<string,vector<sampleContainer> >::iterator itBackgroundSample = sampleMap.begin(); // loop on the sample map
  for( ; itBackgroundSample != sampleMap.end() ; ++itBackgroundSample){
    if(itBackgroundSample->second.at(0).isSignal == 1) continue ; // skip if signal

    // take input files                                                                                                                                                        
    for(size_t iContainer = 0; iContainer < itBackgroundSample->second.size(); iContainer++){  // for each reduced name loop on the different content

      backgroundChainOriginal.clear();
      backgroundChain.clear();

      backgroundChainOriginal.push_back(shared_ptr<TChain>(new TChain (treeName.c_str())));
      backgroundChain.push_back(shared_ptr<TChain>(new TChain (treeName.c_str())));

      int numBefore = 0;
      numBefore += itBackgroundSample->second.at(iContainer).numBefore;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*1.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*2.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*3.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*4.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*5.root").c_str()) ;
      backgroundChainOriginal.back()->Add ((InputBaseDirectory+"/"+itBackgroundSample->second.at(iContainer).sampleName+"/*6.root").c_str()) ;

    
      TObjArray* fileElements = 0;
      fileElements = backgroundChainOriginal.back()->GetListOfFiles();
      TIter next(fileElements);
      TChainElement* chElement = 0;
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
      if(numBefore < totEvent) itBackgroundSample->second.at(iContainer).numBefore = totEvent ;
      cout<<"Background Base Sample Name : "<<itBackgroundSample->first<<" Num Events Before "<<itBackgroundSample->second.at(iContainer).numBefore<<" XS "<<itBackgroundSample->second.at(iContainer).xsec<<" lumi "<<lumi<<endl;
      if(fileElements!=0) fileElements->Delete();
      
      backgroundMap[itBackgroundSample->second.at(iContainer)] = backgroundChain;
    }
  }

  // signal
  map<string,vector<sampleContainer> >::iterator itSignalSample = sampleMap.begin(); // loop on the sample map
  for( ; itSignalSample != sampleMap.end() ; ++itSignalSample){
    if(itSignalSample->second.at(0).isSignal != 1) continue ; // skip if signal

    // take input files                                                                                                                                                        
    for(size_t iContainer = 0; iContainer < itSignalSample->second.size(); iContainer++){  // for each reduced name loop on the different content

      signalChainOriginal.clear();
      signalChain.clear();
  
      signalChainOriginal.push_back(shared_ptr<TChain>(new TChain (treeName.c_str())));
      signalChain.push_back(shared_ptr<TChain>(new TChain (treeName.c_str())));
    
      int numBefore = 0;

      numBefore += itSignalSample->second.at(iContainer).numBefore;
      signalChainOriginal.back()->Add ((InputBaseDirectory+"/"+itSignalSample->second.at(iContainer).sampleName+"/*.root").c_str()) ;
  
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
      if(numBefore < totEvent) itSignalSample->second.at(iContainer).numBefore = totEvent ;
      if(fileElements!=0) fileElements->Delete();
      cout<<"Signal Base Sample Name : "<<itSignalSample->first<<" Num Events Before "<<itSignalSample->second.at(iContainer).numBefore<<" XS "<<itSignalSample->second.at(iContainer).xsec<<" lumi "<<lumi<<endl;
      
      signalMap[itSignalSample->second.at(iContainer)] = signalChain;
    }
  }

  // Book MVA Training Object --> one for each PU bin 
  vector<shared_ptr<TMVATrainingClass>> trainingVector ;

  // Set Global Weight and signal + background Tree for MVA Training                                                                                                       
  map<sampleContainer,float> signalGlobalWeight;
  map<sampleContainer,float> backgroundGlobalWeight;

    
  for(size_t puBin = 0; puBin+1 < pileUpBin.size() ; puBin++){

    cout<<"PU Bin of Training: Min = "<<pileUpBin.at(puBin)<<" Max = "<<pileUpBin.at(puBin+1)<<endl;
    
    TString LabelTemp ;
    if(pileUpBin.size() > 2) 
      LabelTemp = Form("%s_PU_%d_%d",Label.c_str(),int(pileUpBin.at(puBin)),int(pileUpBin.at(puBin+1))) ;
    else Label = LabelTemp ;
    
    for(size_t iCut = 0 ; iCut < CutList.size(); iCut++){

      if(CutList.size() > 1 ) outputFileName += "_"+CutList.at(iCut).cutLayerName;

      string variableTransformationTemp = ":Transformations="+variableTransformation;

      trainingVector.push_back(shared_ptr<TMVATrainingClass> (new TMVATrainingClass(signalMap, 
										    backgroundMap, 
										    treeName, 
										    outputFileDirectory, 
										    outputFileName,
										    Label,
										    variableTransformationTemp)));
      
      // Set Input and Spectator Variables
      cout<<endl;
      cout<<"Set Training and Spectator Variables  "<<endl;
      cout<<endl;
      
      trainingVector.back()->AddTrainingVariables(GetVariableList(variableList),GetVariableList(spectatorList),isTrainEachVariable);
      
      // Set Global Weight and signal + background Tree for MVA Training
      signalGlobalWeight.clear();
      backgroundGlobalWeight.clear();
	  
      cout<<"Building Global Event Weight  + Add Trees "<<endl;
      cout<<endl; 
	    
      int iBackg = 0;
      map<string,vector<sampleContainer> >::iterator itBackgroundSample = sampleMap.begin();
      for( ; itBackgroundSample != sampleMap.end() ; itBackgroundSample++){          	
	vector<sampleContainer>::iterator itBackgroundContainer = itBackgroundSample->second.begin();
	for( ; itBackgroundContainer != itBackgroundSample->second.end(); itBackgroundContainer++){
	  if(itBackgroundContainer->isSignal == 1) continue ; // skip if signal	  
	  backgroundGlobalWeight[*itBackgroundContainer] = itBackgroundContainer->xsec*lumi/(float(itBackgroundContainer->numBefore));
	  cout<<"Backg "<<itBackgroundSample->first<<" xsec "<<itBackgroundContainer->xsec<<" entries "<<itBackgroundContainer->numBefore<<" weight "<<backgroundGlobalWeight[*itBackgroundContainer]<<endl;
	  iBackg++;
	}
      }

      int iSignal = 0;
      map<string,vector<sampleContainer> >::iterator itSignalSample = sampleMap.begin();
      for( ; itSignalSample != sampleMap.end() ; itSignalSample++){          	
	vector<sampleContainer>::iterator itSignalContainer = itSignalSample->second.begin();
	for( ; itSignalContainer != itSignalSample->second.end(); itSignalContainer++){
	  if(itSignalContainer->isSignal != 1) continue ; // skip if signal
	  signalGlobalWeight[*itSignalContainer] = itSignalContainer->xsec*lumi/(float(itSignalContainer->numBefore));
	  cout<<"Signal "<<itSignalSample->first<<" xsec "<<itSignalContainer->xsec<<" entries "<<itSignalContainer->numBefore<<" weight "<<signalGlobalWeight[*itSignalContainer]<<endl;
	  iSignal++;
	}
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
						make_pair(pileUpBin.at(puBin),pileUpBin.at(puBin+1)),
						finalStateString,
						fakeRateFile						
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
    }      
  } 
  
  return 0 ;
}
