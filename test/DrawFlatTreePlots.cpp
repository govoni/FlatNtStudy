//////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"

#include "plotter.h"
#include "ConfigParser.h"
#include "readTree.h"
#include "utils.h"

using namespace std ;

float matchingCone ;
float minLeptonCleaningPt;
float minLeptonCutPt;
float minJetCutPt;
float leptonIsoCut_mu;
float leptonIsoCut_el;
float leptonIsoCutLoose;

bool   usePuppiAsDefault;

string finalStateString;
string fakeRateFile;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main (int argc, char ** argv) {

  // check number of inpt parameters
  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<endl;
    return -1;
  }

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
  string InputBaseDirectory           = gConfigParser -> readStringOption("Input::InputBaseDirectory");
  string InputSampleList              = gConfigParser -> readStringOption("Input::InputSampleList");

  unordered_map<string,vector<sampleContainer> > sampleMap ;
  if(ReadInputSampleFile(InputSampleList,sampleMap) <= 0){
    cerr<<" Empty Input Sample File or not Exisisting --> Exit "<<endl; return -1;}
  
  // treeName
  string treeName           = gConfigParser -> readStringOption("Input::TreeName");

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

  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  fakeRateFile        = gConfigParser -> readStringOption("Option::fakeRateFile");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());

  ///// Start the analysis
  
  plotter analysisPlots (lumi,"output") ;

  map<string,TH1F*> histoCutEff ;

  for( unordered_map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){

    // for each sample there can be sub samples
    vector<readTree*> ReadTree;
    for(vector<sampleContainer>::iterator itSubSample = itSample->second.begin(); itSubSample != itSample->second.end(); itSubSample++){
    
      TChain* chain = new TChain (treeName.c_str()) ;  

      int numBefore = 0;
      // take input files
      numBefore += itSubSample->numBefore; 
      chain->Add ((InputBaseDirectory+"/"+itSubSample->sampleName+"/*_1.root").c_str()) ;

      int totEvent = chain->GetEntries();

      ReadTree.push_back(new readTree((TTree*)(chain)));

      cout<<"Sample name : "<<itSample->first<<" directory files "<<itSubSample->sampleName<<" Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<itSubSample->xsec<<" Nevents before selections "<<lumi*itSubSample->xsec<<" weight "<<lumi*itSubSample->xsec/totEvent<<endl;

      // add  sample to the analysis plot container
      if(numBefore > totEvent)
	analysisPlots.addSample(itSample->first,itSubSample->xsec, numBefore, itSubSample->isSignal, itSubSample->color) ;   // create the sample to analyze      
      else
	analysisPlots.addSample(itSample->first,itSubSample->xsec, totEvent,  itSubSample->isSignal, itSubSample->color) ;   // create the sample to analyze
 
    }

    // Add cuts to the analysis plot container
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){
      analysisPlots.addLayerToSample  (itSample->first,CutList.at(iCut).cutLayerName) ;      
      for(size_t iSub = 0; iSub < itSample->second.size(); iSub++){
	histoCutEff[itSample->first+"_pos_"+to_string(iSub)+"_"+CutList.at(iCut).cutLayerName] = new TH1F((itSample->first+"_pos_"+to_string(iSub)+"_"+CutList.at(iCut).cutLayerName).c_str(),"",10,0,10);
      }


      // Add variables to the plot
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){   
	analysisPlots.addPlotToLayer (itSample->first,CutList.at(iCut).cutLayerName,variableList.at(iVar).variableName,
				      variableList.at(iVar).Nbin,variableList.at(iVar).min,variableList.at(iVar).max,variableList.at(iVar).label) ;
      }

    }
  
     
    string sampleName = itSample->first;

    for(size_t iRead = 0; iRead < ReadTree.size(); iRead++){
      cout<<"analyzing for sample "<<itSample->first<<" chain number "<<iRead<<endl;
      loopOnEvents (analysisPlots,  // analysis plot element
		    sampleName,     // sample name used as reference		    
		    int(iRead),     // directories in case with the same sample name you want to merge different physics process with different xs
		    ReadTree.at(iRead), // reader for that tree
		    CutList,        // list of cuts
		    variableList,   // list of variables to plot
		    usePuppiAsDefault, // some options
		    minLeptonCutPt,
		    minLeptonCleaningPt,
		    leptonIsoCut_mu,
		    leptonIsoCut_el,
		    leptonIsoCutLoose,
		    matchingCone,
		    minJetCutPt,
		    histoCutEff, // efficiency histogram map vs cut
		    finalStateString, // final state string
		    "",
		    fakeRateFile) ; // fill the histogram      
    }
    
  }

  
  // plotting
  // ---- ---- ---- ---- ---- ---- ----
  analysisPlots.setPoissonErrors () ;
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    analysisPlots.printEventNumber(CutList.at(iCut).cutLayerName,"DeltaPhi_LL");
    analysisPlots.plotRelativeExcessFullLayer (CutList.at(iCut).cutLayerName, outputPlotDirectory) ;
  }
  
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    
  
  return 0 ;
  
}  

