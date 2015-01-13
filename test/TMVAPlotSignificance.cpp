#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <istream>
#include <sstream>

#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TString.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"

#include "TMVAPlotClass.h"
#include "ReadInputFile.h"
#include "ConfigParser.h"

using namespace std;

vector<string> GetInputFileList(const vector<trainingContainer> & container){
  vector<string> outputVector ;
  vector<trainingContainer>::const_iterator itFile = container.begin(); // loop on the training list --> plot on the same roc curve
  for( ; itFile != container.end() ; ++itFile){
    outputVector.push_back(itFile->fileName);
  }
  return outputVector;
}

vector<string> GetInputMethodList(const vector<trainingContainer> & container){
  vector<string> outputVector ;
  vector<trainingContainer>::const_iterator itFile = container.begin(); // loop on the training list --> plot on the same roc curve
  for( ; itFile != container.end() ; ++itFile){
    for(size_t itMethod = 0; itMethod < itFile->methodName.size(); itMethod++)
      outputVector.push_back(itFile->methodName.at(itMethod));
  }
  return outputVector;
}


/// Main programme 
int main (int argc, char **argv){

  if(argc<2){ 
    cout<<" Not correct number of input parameter --> Need Just one cfg file exit "<<endl; 
    exit(EXIT_FAILURE) ; 
  }

  // Set Root style from global enviroment path                                                                                                                             
  string ROOTStyle;
  if(getenv ("ROOTStyle")!=NULL){
    ROOTStyle = getenv ("ROOTStyle");
    gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
  }
  
  gStyle->SetOptStat(0);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetErrorX(0.5);


  // parse config file parameter                                                                                                                                            
  if (gConfigParser) return 1 ;
  gConfigParser = new ConfigParser();

  TString config ;
  config.Form("%s",argv[1]);

  if(!(gConfigParser->init(config))){
    cout << ">>> parseConfigFile::Could not open configuration file " << config << endl;
    return -1;
  }

  // take the text file in which all the input files from TMVA training are declared
  string InputTrainingFileList = gConfigParser->readStringOption("Input::InputTrainingFileList");  
  vector<trainingContainer>  trainingList ;
  if(ReadInputTrainingFile(InputTrainingFileList,trainingList) <= 0){
    cerr<<" Empty Input Sample File or not Exisisting --> Exit "<<endl; 
    return -1;
  }


  // plotType option
  int plotType ;
  try{ 
    plotType = gConfigParser->readIntOption("Input::plotType");
  }
  catch(char* excpetionString){
    cout<<" plotType set to 0 "<<endl; 
    plotType = 0; 
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

  // output directory                                                                                                                                                          
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());
  outputPlotDirectory = "output/"+outputPlotDirectory;

  
  // Declare the object for the manipolation of the TMVA ROOT file
  TMVAPlotClass* TMVATrainingPlot = new TMVAPlotClass();

  TMVATrainingPlot->openFileInput(GetInputFileList(trainingList)); // open all the input files
  TMVATrainingPlot->SetMethodName(GetInputMethodList(trainingList)); // set all the trained methods
    
  // plot ROC curves 
  TMVATrainingPlot->makeROCsPlot(gDirectory, // give the current global directory
				 plotType, // 0 means eff sign vs eff back , 1 eff sign vs 1-eff bkg
				 trainingList.at(0).puBin.first,
				 trainingList.at(0).puBin.second); // call the plot efficiency function

  TMVATrainingPlot->PrintImageROC(gDirectory,
				  outputPlotDirectory); // make the plot
  
  vector<TFile*> inputFiles = TMVATrainingPlot->GetInputFile(); // take back the input file list
  
  for(size_t iFile = 0 ; iFile < inputFiles.size(); iFile++){

    TMVATrainingPlot->makeCorrelationMatrixPlot(inputFiles.at(iFile),
						trainingList.at(iFile).varNameReduced,
						outputPlotDirectory); // make correlation plots
    
    // make MVA output plot
    TMVATrainingPlot->makeMVAsPlot(inputFiles.at(iFile),TMVATrainingPlot->MVAType,outputPlotDirectory);
    // make TMVA probability plot
    //TMVATrainingPlot->makeMVAsPlot(inputFiles.at(iFile),TMVATrainingPlot->ProbaType,outputPlotDirectory);
    // make TMVA overtraining plot
    TMVATrainingPlot->makeMVAsPlot(inputFiles.at(iFile),TMVATrainingPlot->CompareType,outputPlotDirectory);
  
    // take signal and background expected yield from the file:
    TTree *testTree = 0;
    int classID     = 0;
    float weight    = 0;

    float nSignalEvent = 0;
    float nBackgroundEvent = 0;

    testTree = (TTree*) inputFiles.at(iFile)->Get("TestTree");
    testTree->SetBranchAddress("classID",&classID);
    testTree->SetBranchAddress("weight",&weight);
    for(int iEntry = 0; iEntry < testTree->GetEntries(); iEntry++){      
      testTree->GetEntry(iEntry);
      if(classID == 0) nSignalEvent += weight;
      else nBackgroundEvent += weight;
    }

    // make the significance plot
    TMVATrainingPlot->makeSignificancePlot(inputFiles.at(iFile),
					   trainingList.at(iFile).varNameReduced,
					   TMVATrainingPlot->SoverSqrtB, // significance type --> look at the convention in the header file
					   nSignalEvent, // number of signal events from the testing tree
					   nBackgroundEvent, // number of background events from the testing tree
					   false, // use signal efficiency instead of yield
					   false, // use background efficiency instead of yield
					   outputPlotDirectory);
    
    inputFiles.at(iFile)->Close();
  }
  
  return 0 ;
  
}

