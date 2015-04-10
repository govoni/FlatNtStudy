/////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TRandom3.h"
#include "TString.h"

#include "ConfigParser.h"
#include "readTree.h"
#include "utils.h"
#include "plotter.h"

using namespace std ;

// some basic quantites to do the analysis selections
float matchingCone ;
float minLeptonCleaningPt;
float minLeptonCutPt;
float minJetCutPt;
float leptonIsoCut_mu;
float leptonIsoCut_el;
float leptonIsoCutLoose;

bool   usePuppiAsDefault;
bool   useObjectSystematics;

int    errorType;
string finalStateString;
string scenarioString ;
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

  // take the variable list to be used for 1D analysis
  string InputVariableList1D  = gConfigParser -> readStringOption("Input::InputVariableList1D");
  vector<variableContainer> variableList1D;

  if(ReadInputVariableFile(InputVariableList1D,variableList1D) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the variable list to be used for 1D analysis
  string InputVariableList2D  = gConfigParser -> readStringOption("Input::InputVariableList2D");
  vector<variableContainer2D> variableList2D;

  if(ReadInputVariableFile(InputVariableList2D,variableList2D) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // lumi and cut values
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");

  usePuppiAsDefault    = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  useObjectSystematics = gConfigParser -> readBoolOption("Option::useObjectSystematics");

  errorType           = gConfigParser -> readIntOption("Option::errorType");

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  scenarioString      = gConfigParser -> readStringOption("Option::scenarioString");
  fakeRateFile        = gConfigParser -> readStringOption("Option::fakeRateFile");

  // output datacard directory
  string outputDataCardDirectory = gConfigParser -> readStringOption("Output::outputDataCardDirectory");

  system(("mkdir -p output/"+outputDataCardDirectory).c_str());
  system(("mkdir -p output/"+outputDataCardDirectory+"/Card1D/").c_str());
  system(("mkdir -p output/"+outputDataCardDirectory+"/Card2D/").c_str());

  system(("rm output/"+outputDataCardDirectory+"/Card1D/*_"+finalStateString+"*").c_str());
  system(("rm output/"+outputDataCardDirectory+"/Card2D/*_"+finalStateString+"*").c_str());

  
  ///// Start the analysis --> apply cut and make histos
  plotter analysisPlots (lumi,"output",useObjectSystematics) ; // use the plotter structure to make the histograms for each sample and each cutLayer and each variable
  map<string,TH1F*> histoCutEff ;

  for( unordered_map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){ // loop on each sample

    vector<readTree*> ReadTree;
    for(vector<sampleContainer>::iterator itSubSample = itSample->second.begin(); itSubSample != itSample->second.end(); itSubSample++){

      TChain* chain = new TChain (treeName.c_str()) ;  // take the file chain
      int numBefore = 0;

      numBefore += itSubSample->numBefore;
      chain->Add ((InputBaseDirectory+"/"+itSubSample->sampleName+"/*.root").c_str()) ;

      int totEvent = chain->GetEntries();

      ReadTree.push_back( new readTree((TTree*)(chain))); // reader for chain trees

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

      // Add variables to the plot 1D analysis
      for(size_t iVar = 0; iVar < variableList1D.size(); iVar++){   
	analysisPlots.addPlotToLayer (itSample->first,CutList.at(iCut).cutLayerName,variableList1D.at(iVar).variableName,
				      variableList1D.at(iVar).Nbin,variableList1D.at(iVar).min,variableList1D.at(iVar).max,variableList1D.at(iVar).label,true) ;
      }

      // Add variables to the plot 1D analysis
      for(size_t iVar = 0; iVar < variableList2D.size(); iVar++){   
      	analysisPlots.add2DPlotToLayer (itSample->first,CutList.at(iCut).cutLayerName,variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY,
      					variableList2D.at(iVar).NbinX,variableList2D.at(iVar).minX,variableList2D.at(iVar).maxX,
					variableList2D.at(iVar).NbinY,variableList2D.at(iVar).minY,variableList2D.at(iVar).maxY,
    					variableList2D.at(iVar).labelX,variableList2D.at(iVar).labelY,true);
      }
    }
    
    // 1D + 2D analysis
    string sampleName ;
    if(TString(itSample->second.at(0).sampleName).Contains("Madgraph"))
      sampleName = "Madgraph_"+itSample->first ;
    else
      sampleName = itSample->first ;

    for(size_t iRead = 0; iRead < ReadTree.size(); iRead++){
      cout<<"analyzing for sample "<<itSample->first<<" chain number "<<iRead<<endl;
      
      loopOnEvents(analysisPlots,
		   sampleName,         // sample name
		   int(iRead),
		   ReadTree.at(iRead),
		   CutList,
		   variableList1D,     // 1D variables
		   variableList2D,     // 2D variables
		   usePuppiAsDefault,  // use puppi flag
		   minLeptonCutPt,     // lepton pt cut
		   minLeptonCleaningPt,// cleaning cut
		   leptonIsoCut_mu,    // isolation for muons
		   leptonIsoCut_el,    // isolation for electrons
		   leptonIsoCutLoose,  // isolation for loose leptons
		   matchingCone,       // matching cone
		   minJetCutPt,        // min jet pt cut
		   histoCutEff,        // eff cut
		   finalStateString,   // string
		   scenarioString,
		   fakeRateFile
		   );
    }
  }
  
  // plotting
  // ---- ---- ---- ---- ---- ---- ----
  if(errorType == 0){
    cout<<"set sqrt(N) errors for all histo"<<endl;
    analysisPlots.setPoissonErrors () ;
  }
  else if(errorType == 1){
    cout<<"set poisson errors for all histo"<<endl;
    analysisPlots.setAsymmetricErrors() ;
  }
  else{
    cout<<"use sumW2 for MC histograms"<<endl;
  }

  // get the sample vector from the analysis plotter object                                                                                                                      
  vector<sample> SampleVector ;
  SampleVector = analysisPlots.getSamples();
  // make sure to merge samples with the same sampleName                                                                                                                         
  for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
    size_t jSample   = iSample+1;

    for( ; jSample < SampleVector.size(); jSample++){
      if(SampleVector.at(iSample) == SampleVector.at(jSample)){
        sample sampleTemp ;
	mergeSample(sampleTemp,SampleVector.at(iSample),SampleVector.at(jSample));
        SampleVector.at(iSample) = sampleTemp;
        SampleVector.erase(SampleVector.begin()+jSample);
	jSample--;
        continue;
      }
    }
  }

  
  ////////////////////////////////////////// 
  //####### TH1 sector Datacards #########//
  /////////////////////////////// //////////
  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){

    // loop on variables 1D
    for(size_t iVar = 0; iVar < variableList1D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+"_"+finalStateString+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+"_"+finalStateString+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+"_"+finalStateString+".root").c_str(),"RECREATE");
      outputCard->cd();

      // print head of the datacad
      datacard << "## shape analysis WW scattering" << endl;
      datacard << "imax 1 number of channels"       << endl;
      datacard << "jmax * number of background"     << endl;
      datacard << "kmax * number of nuisance parameters" << endl;
      datacard <<"------------------------------------------------------"<< endl;
      datacard <<"bin  "+CutList.at(iCut).cutLayerName+"_"+finalStateString<< endl;
      			     
      /// loop on samples to fill the total histogram (dataset)
      TH1F* hTotal   = 0;
      TH1F* observed = 0;

      /// identify if H126 and noH are simoultaneusly in the sample list and fill the signal for this analysis
      TH1F* Higgs126   = 0;
      TH1F* Higgs126_scale_muUp = 0, *Higgs126_scale_muDw = 0, *Higgs126_res_muUp = 0;
      TH1F* Higgs126_scale_elUp = 0, *Higgs126_scale_elDw = 0, *Higgs126_res_elUp = 0;
      TH1F* Higgs126_scale_jUp  = 0, *Higgs126_scale_jDw  = 0, *Higgs126_res_jUp  = 0;

      TH1F* noHiggs          = 0;
      TH1F* noHiggs_scale_muUp  = 0, *noHiggs_scale_muDw  = 0, *noHiggs_res_muUp = 0;
      TH1F* noHiggs_scale_elUp  = 0, *noHiggs_scale_elDw  = 0, *noHiggs_res_elUp = 0;
      TH1F* noHiggs_scale_jUp  = 0, *noHiggs_scale_jDw  = 0, *noHiggs_res_jUp    = 0;

      TH1F* HminusNoH          = 0;
      TH1F* HminusNoH_scale_muUp  = 0, *HminusNoH_scale_muDw  = 0, *HminusNoH_res_muUp    = 0, *HminusNoH_res_muDw    = 0;
      TH1F* HminusNoH_scale_elUp  = 0, *HminusNoH_scale_elDw  = 0, *HminusNoH_res_elUp    = 0, *HminusNoH_res_elDw    = 0;
      TH1F* HminusNoH_scale_jUp  = 0, *HminusNoH_scale_jDw  = 0, *HminusNoH_res_jUp    = 0, *HminusNoH_res_jDw    = 0;

      // fill histograms
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(TString(SampleVector.at(iSample).m_sampleName).Contains("126")){
	  
	  // central value
	  Higgs126 = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126");
	  // scale up for lepton scale
	  Higgs126_scale_muUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_muUp");
	  Higgs126_scale_elUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_elUp");

	  // scale down for lepton scale
	  Higgs126_scale_muDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_muDown");
	  Higgs126_scale_elDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_elDown");

	  // smeared by lepton resolution
	  Higgs126_res_muUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muRes[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_muUp");
	  Higgs126_res_elUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elRes[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_elUp");

	  // scale up for jet scale
	  Higgs126_scale_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_jUp");
	  // scale down for jet scale
	  Higgs126_scale_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_jDown");
	  // scale smear for jet resolution
	  Higgs126_res_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_jUp");

	  // add overflow and underflow bins to 1D histograms
	  addOverAndUnderFlow(Higgs126);
	  addOverAndUnderFlow(Higgs126_scale_muUp);
	  addOverAndUnderFlow(Higgs126_scale_muDw);
	  addOverAndUnderFlow(Higgs126_res_muUp);
	  addOverAndUnderFlow(Higgs126_scale_elUp);
	  addOverAndUnderFlow(Higgs126_scale_elDw);
	  addOverAndUnderFlow(Higgs126_res_elUp);
	  addOverAndUnderFlow(Higgs126_scale_jUp);
	  addOverAndUnderFlow(Higgs126_scale_jDw);
	  addOverAndUnderFlow(Higgs126_res_jUp);

	  avoidEmptyBins(Higgs126);
	  avoidEmptyBins(Higgs126_scale_muUp);
	  avoidEmptyBins(Higgs126_scale_muDw);
	  avoidEmptyBins(Higgs126_res_muUp);
	  avoidEmptyBins(Higgs126_scale_elUp);
	  avoidEmptyBins(Higgs126_scale_elDw);
	  avoidEmptyBins(Higgs126_res_elUp);
	  avoidEmptyBins(Higgs126_scale_jUp);
	  avoidEmptyBins(Higgs126_scale_jDw);
	  avoidEmptyBins(Higgs126_res_jUp);

	}

        else if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  // cental value
	  noHiggs = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs");
	  // scaled up by lepton scale unc
	  noHiggs_scale_muUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_muUp");
	  noHiggs_scale_elUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_elUp");

	  // scaled down by lepton scale unc
	  noHiggs_scale_muDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_muDown");
	  noHiggs_scale_elDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_elDown");

	  // smeared by lepton res
	  noHiggs_res_muUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muRes[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_muUp");
	  noHiggs_res_elUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elRes[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_elUp");

	  noHiggs_scale_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_jUp");

	  noHiggs_scale_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_jDown");

	  noHiggs_res_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_jUp");

	  //add overflow and underflow
	  addOverAndUnderFlow(noHiggs);
	  addOverAndUnderFlow(noHiggs_scale_muUp);
	  addOverAndUnderFlow(noHiggs_scale_muDw);
	  addOverAndUnderFlow(noHiggs_res_muUp);
	  addOverAndUnderFlow(noHiggs_scale_elUp);
	  addOverAndUnderFlow(noHiggs_scale_muDw);
	  addOverAndUnderFlow(noHiggs_res_elUp);
	  addOverAndUnderFlow(noHiggs_scale_jUp);
	  addOverAndUnderFlow(noHiggs_scale_jDw);
	  addOverAndUnderFlow(noHiggs_res_jUp);

	  avoidEmptyBins(noHiggs);
	  avoidEmptyBins(noHiggs_scale_muUp);
	  avoidEmptyBins(noHiggs_scale_muDw);
	  avoidEmptyBins(noHiggs_res_muUp);
	  avoidEmptyBins(noHiggs_scale_elUp);
	  avoidEmptyBins(noHiggs_scale_elDw);
	  avoidEmptyBins(noHiggs_res_elUp);
	  avoidEmptyBins(noHiggs_scale_jUp);
	  avoidEmptyBins(noHiggs_scale_jDw);
	  avoidEmptyBins(noHiggs_res_jUp);

	}
      }

      // if both 126 and noH are there make the difference as signal to test partial unitarization and make sure to be positive defined
      if(Higgs126 != 0 and noHiggs != 0){      

	HminusNoH = (TH1F*) noHiggs->Clone("histo_HminusNoH");
	HminusNoH->Add(Higgs126,-1);
        avoidEmptyBins(HminusNoH);

	HminusNoH_scale_muUp = (TH1F*) noHiggs_scale_muUp->Clone("histo_HminusNoH_CMS_scale_muUp");
	HminusNoH_scale_muUp ->Add(Higgs126_scale_muUp,-1);
        avoidEmptyBins(HminusNoH_scale_muUp);

	HminusNoH_scale_elUp = (TH1F*) noHiggs_scale_elUp->Clone("histo_HminusNoH_CMS_scale_elUp");
	HminusNoH_scale_elUp ->Add(Higgs126_scale_elUp,-1);
        avoidEmptyBins(HminusNoH_scale_elUp);

	HminusNoH_scale_muDw = (TH1F*) noHiggs_scale_muDw->Clone("histo_HminusNoH_CMS_scale_muDown");
	HminusNoH_scale_muDw->Add(Higgs126_scale_muDw,-1);
        avoidEmptyBins(HminusNoH_scale_muDw);

	HminusNoH_scale_elDw = (TH1F*) noHiggs_scale_elDw->Clone("histo_HminusNoH_CMS_scale_elDown");
	HminusNoH_scale_elDw->Add(Higgs126_scale_elDw,-1);
        avoidEmptyBins(HminusNoH_scale_elDw);

	HminusNoH_res_muUp = (TH1F*) noHiggs_res_muUp->Clone("histo_HminusNoH_CMS_res_muUp");
	HminusNoH_res_muUp->Add(Higgs126_res_muUp,-1);
        avoidEmptyBins(HminusNoH_res_muUp);

	HminusNoH_res_elUp = (TH1F*) noHiggs_res_elUp->Clone("histo_HminusNoH_CMS_res_elUp");
	HminusNoH_res_elUp->Add(Higgs126_res_elUp,-1);
        avoidEmptyBins(HminusNoH_res_elUp);

	HminusNoH_res_muDw = mirrorHistogram("histo_HminusNoH_CMS_res_muDown",HminusNoH,HminusNoH_res_muUp);
	avoidEmptyBins(HminusNoH_res_muDw);
	HminusNoH_res_elDw = mirrorHistogram("histo_HminusNoH_CMS_res_elDown",HminusNoH,HminusNoH_res_elUp);
	avoidEmptyBins(HminusNoH_res_elDw);

	HminusNoH_scale_jUp = (TH1F*) noHiggs_scale_jUp->Clone("histo_HminusNoH_CMS_scale_jUp");
	HminusNoH_scale_jUp->Add(Higgs126_scale_jUp,-1);
        avoidEmptyBins(HminusNoH_scale_jUp);

	HminusNoH_scale_jDw = (TH1F*) noHiggs_scale_jDw->Clone("histo_HminusNoH_CMS_scale_jDown");
	HminusNoH_scale_jDw->Add(Higgs126_scale_jDw,-1);
        avoidEmptyBins(HminusNoH_scale_jDw);

	HminusNoH_res_jUp = (TH1F*) noHiggs_res_jUp->Clone("histo_HminusNoH_CMS_res_jUp");
	HminusNoH_res_jUp->Add(Higgs126_res_jUp,-1);
        avoidEmptyBins(Higgs126_res_jUp);

	HminusNoH_res_jDw = mirrorHistogram("histo_HminusNoH_CMS_res_jDown",HminusNoH,HminusNoH_res_jUp);
	avoidEmptyBins(HminusNoH_res_jDw);
      }
    
         
      // make the total distribution as dataset for combine
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("126")) continue ;

        if(hTotal == 0)
	  hTotal = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("hTotal_"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName).c_str());
        else 
	  hTotal->Add(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]);
      }  
      

      // make the observed dataser
      hTotal->SetBinContent(1,hTotal->GetBinContent(1)+hTotal->GetBinContent(0)); 
      hTotal->SetBinContent(hTotal->GetNbinsX(),hTotal->GetBinContent(hTotal->GetNbinsX())+hTotal->GetBinContent(hTotal->GetNbinsX()+1)); // add overflow to the last bin
      hTotal->SetBinErrorOption(TH1::kPoisson);

      // make a toy on it + setting poisson errors
      observed = (TH1F*)  hTotal->Clone("histo_Data");
      observed->SetBinErrorOption(TH1::kPoisson);
      observed->Reset();
      observed->FillRandom(hTotal,gRandom->Poisson(hTotal->Integral())); // make a simple toy
      observed->Write();

      // print other card info
      datacard<< Form("observation %d",int(observed->Integral()))<< endl;
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+"_"+finalStateString+".root    histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+"_"+finalStateString+".root    histo_Data" <<endl;
      datacard<< "------------------------------------------------------"<< endl;
      
      // count the events and write down rates      
      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";
      
      int nSignal     = 0 ;
      int nBackground = 1 ;

      TH1F* hNominal  = 0;

      // loop on samples
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){ // for partial unitarization scenario
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += "HminusNoH   ";
          hNominal     =  HminusNoH ; // overlow and underflow already added
	  avoidEmptyBins(hNominal);
	}
	else{
	  
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += SampleVector.at(iSample).m_sampleName+"   ";

	  hNominal     = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];
	  addOverAndUnderFlow(hNominal);
	  avoidEmptyBins(hNominal);
	}
 
	// set the name of histogram
        if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))
	  hNominal->Write("histo_HminusNoH");
	else
	  hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        
        /// not for H-noH analysis
	if(HminusNoH != 0){
	  if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	    lineRate += Form("%f   ",hNominal->Integral());  
	  }
	  else{
	    lineProcess2 += Form("%d   ",nBackground);
	    nBackground++;
	    lineRate += Form("%f   ",hNominal->Integral());  	
	  }
	}
	else {
	  if(SampleVector.at(iSample).m_isSignal > 0){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	  }
	  else{
	    lineProcess2 += Form("%d   ",nBackground);
	    nBackground++;
	  }
	  lineRate += Form("%f   ",hNominal->Integral());  	
	}
      }
      
      datacard << left << lineBin << endl;
      datacard << left << lineProcess << endl;
      datacard << left << lineProcess2 << endl;
      datacard << left << lineRate << endl;
      datacard<< "------------------------------------------------------"<< endl;

      // make systematics lnN lines
      string lumiSys        = "CMS_lumi         lnN";
      string wrongChargeSys = "CMS_wrongCharge  lnN";
      string QCDScaleSys    = "QCDScale     lnN";
      string PdfqqSys       = "Pdfqq        lnN";
      string btagSys        = "CMS_btag_eff lnN";

      string lepEffSys_mu   = "CMS_lep_eff_mu  lnN";
      string lepEffSys_el   = "CMS_lep_eff_el  lnN";

      string acceptSys      = "CMS_Accept   lnN";

      string fakeLepSys_mu  = "CMS_fakeLep_mu  lnN";
      string fakeLepSys_el  = "CMS_fakeLep_el  lnN";
      
      // make systematics lines and shapes in the root file
      string muScaleShape = "CMS_scale_mu  shapeN2";
      string muResShape   = "CMS_res_mu    shapeN2";
      string elScaleShape = "CMS_scale_el  shapeN2";
      string elResShape   = "CMS_res_el    shapeN2";

      string jetScaleShape = "CMS_scale_j  shapeN2";
      string jetResShape   = "CMS_res_j    shapeN2";

      string fakeRateShape = "CMS_fakeLep_"+finalStateString+"_shape  shapeN2";

      string statShape     = "";

      // stat shapes ;
      TH1F* hStatUp       = 0, *hStatDown = 0;
      TH1F* hMuScaleUp    = 0, *hMuScaleDown  = 0, *hMuResUp      = 0, *hMuResDown    = 0;
      TH1F* hElScaleUp    = 0, *hElScaleDown  = 0, *hElResUp      = 0, *hElResDown    = 0;
      TH1F* hJetScaleUp   = 0, *hJetScaleDown = 0, *hJetResUp     = 0, *hJetResDown   = 0;
      TH1F* hfakeRateUp   = 0, *hfakeRateDown = 0;

      
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(SampleVector.at(iSample).m_isSignal < 0){  // data driven background just the central shape

	  lumiSys     = lumiSys     +"   -";
	  QCDScaleSys = QCDScaleSys +"   -";
	  PdfqqSys    = PdfqqSys    +"   -";
	  btagSys     = btagSys     +"   -";

	  if (TString(finalStateString).Contains("UU"))
	    lepEffSys_mu   = lepEffSys_mu   +"   -";
	  else if (TString(finalStateString).Contains("EE"))
	    lepEffSys_el   = lepEffSys_el   +"   -";
	  else{
	    lepEffSys_mu   = lepEffSys_mu   +"   -";
	    lepEffSys_el   = lepEffSys_el   +"   -";
	  }

	  acceptSys   = acceptSys   +"   -";
	  
	  if(SampleVector.at(iSample).m_isSignal != -2){
	      wrongChargeSys = wrongChargeSys + "  -";	
	  }  
	  else if(SampleVector.at(iSample).m_isSignal == -2){
	      wrongChargeSys = wrongChargeSys + "  1.30";
	  }

	  muScaleShape = muScaleShape + "  -  ";
	  muResShape   = muResShape   + "  -  ";
	  elScaleShape = elScaleShape + "  -  ";
	  elResShape   = elResShape   + "  -  ";

	  jetScaleShape = jetScaleShape + "  -  ";
	  jetResShape   = jetResShape   + "  -  ";

	  if(SampleVector.at(iSample).m_isSignal != -1){
	    fakeRateShape = fakeRateShape + "  -";	  
	    fakeLepSys_mu    = fakeLepSys_mu+"  -";
	    fakeLepSys_el    = fakeLepSys_el+"  -";
	  }

	  else if(SampleVector.at(iSample).m_isSignal == -1){

	    fakeRateShape = fakeRateShape + "  1";

	    fakeLepSys_mu    = fakeLepSys_mu+"  1.30";
	    fakeLepSys_el    = fakeLepSys_el+"  1.30";
	  

	    hNominal     = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];
	    addOverAndUnderFlow(hNominal);

	    hfakeRateUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_fakeLep_"+finalStateString+"_shapeUp").c_str());
	    hfakeRateDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_fakeLep_"+finalStateString+"_shapeDown").c_str());

	    for (int iBin = 0; iBin < hfakeRateUp->GetNbinsX()+1; iBin++){
	      hfakeRateUp->SetBinContent(iBin,hfakeRateUp->GetBinContent(iBin)+hfakeRateUp->GetBinError(iBin));
	      hfakeRateDown->SetBinContent(iBin,hfakeRateDown->GetBinContent(iBin)-hfakeRateDown->GetBinError(iBin));
	    }

	    avoidEmptyBins(hfakeRateUp);
	    avoidEmptyBins(hfakeRateDown);

	    hfakeRateUp->Write();
	    hfakeRateDown->Write();
	    
	  }

	  continue;
	  
	}
	

	if((HminusNoH == 0 or (HminusNoH !=0 and !TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))) and SampleVector.at(iSample).m_isSignal >= 0){

	  // nominal value
	  hNominal = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName] ;         
	  avoidEmptyBins(hNominal);

	  // scale up for lepton scale
	  unordered_map<string,TH1F*>::const_iterator itMap = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleUp.begin();
	  for( ; itMap != SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleUp.end(); itMap++)

	   hMuScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_muUp").c_str());
	  addOverAndUnderFlow(hMuScaleUp);

	  // scale down for lepton scale
	  itMap = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleDown.begin();
	  for( ; itMap != SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleDown.end(); itMap++)
	  
	  hMuScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_muDown").c_str());
	  addOverAndUnderFlow(hMuScaleDown);

	  // smear by resolution	  
	  hMuResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_muRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_muUp").c_str());
	  addOverAndUnderFlow(hMuResUp);


	  // scale up for lepton scale
	  hElScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_elUp").c_str());
	  addOverAndUnderFlow(hElScaleUp);

	  // scale down for lepton scale
	  hElScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_elDown").c_str());
	  addOverAndUnderFlow(hElScaleDown);

	  // smear by resolution	  
	  hElResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_elRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_elUp").c_str());
	  addOverAndUnderFlow(hElResUp);

	  // mirroring for down histo
	  hMuResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_muDown",hNominal,hMuResUp); // no need to add over and under
	  hElResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_elDown",hNominal,hElResUp); // no need to add over and under
	  

	  hJetScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jUp").c_str());
	  addOverAndUnderFlow(hJetScaleUp);

	  hJetScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jDown").c_str());
	  addOverAndUnderFlow(hJetScaleDown);

	  hJetResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jUp").c_str());
	  addOverAndUnderFlow(hJetResUp);

	  // mirroring for down histo
	  hJetResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jDown",hNominal,hJetResUp);
	  
	  // write the histograms in the file
	  avoidEmptyBins(hMuScaleUp);
	  avoidEmptyBins(hMuScaleDown);
	  avoidEmptyBins(hMuResUp);
	  avoidEmptyBins(hMuResDown);

	  avoidEmptyBins(hElScaleUp);
	  avoidEmptyBins(hElScaleDown);
	  avoidEmptyBins(hElResUp);
	  avoidEmptyBins(hElResDown);

	  avoidEmptyBins(hJetScaleUp);
	  avoidEmptyBins(hJetScaleDown);
	  avoidEmptyBins(hJetResUp);
	  avoidEmptyBins(hJetResDown);

	  if(TString(finalStateString).Contains("UU")){
	    hMuScaleUp->Write();
	    hMuScaleDown->Write();
	    hMuResUp->Write();
	    hMuResDown->Write();
	  }
	  else if (TString(finalStateString).Contains("EE")){
	    hElScaleUp->Write();
	    hElScaleDown->Write();
	    hElResUp->Write();
	    hElResDown->Write();
	  }
	  else {

	    hMuScaleUp->Write();
	    hMuScaleDown->Write();
	    hMuResUp->Write();
	    hMuResDown->Write();
	    hElScaleUp->Write();
	    hElScaleDown->Write();
	    hElResUp->Write();
	    hElResDown->Write();

	  }

	  hJetScaleUp->Write();
	  hJetScaleDown->Write();
	  hJetResUp->Write();
	  hJetResDown->Write();

	  // stat shapes taking into account the MC statistics
	  hStatUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shapeDown").c_str());


	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  }

	  avoidEmptyBins(hStatUp);
	  avoidEmptyBins(hStatDown);

	  hStatUp->Write();
	  hStatDown->Write();
	  
	}
		

	else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH != 0){

	  hStatUp   = (TH1F*) HminusNoH->Clone(("histo_HminusNoH_CMS_HminusNoH_"+finalStateString+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) HminusNoH->Clone(("histo_HminusNoH_CMS_HminusNoH_"+finalStateString+"_stat_shapeDown").c_str());

	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  }

	  avoidEmptyBins(hStatUp);
	  avoidEmptyBins(hStatDown);

	  avoidEmptyBins(HminusNoH_scale_muUp);
	  avoidEmptyBins(HminusNoH_scale_muDw);
	  avoidEmptyBins(HminusNoH_res_muUp);
	  avoidEmptyBins(HminusNoH_res_muDw);

	  avoidEmptyBins(HminusNoH_scale_elUp);
	  avoidEmptyBins(HminusNoH_scale_elDw);
	  avoidEmptyBins(HminusNoH_res_elUp);
	  avoidEmptyBins(HminusNoH_res_elDw);

	  avoidEmptyBins(HminusNoH_scale_jUp);
	  avoidEmptyBins(HminusNoH_scale_jDw);
	  avoidEmptyBins(HminusNoH_res_jUp);
	  avoidEmptyBins(HminusNoH_res_jDw);


	  if(TString(finalStateString).Contains("UU")){	  	    
	    HminusNoH_scale_muUp->Write();
	    HminusNoH_scale_muDw->Write();
	    HminusNoH_res_muUp->Write();
	    HminusNoH_res_muDw->Write();
	  }	  
	  else if(TString(finalStateString).Contains("EE")){
	    HminusNoH_scale_elUp->Write();
	    HminusNoH_scale_elDw->Write();
	    HminusNoH_res_elUp->Write();
	    HminusNoH_res_elDw->Write();
	  }
	  else{
	    HminusNoH_scale_muUp->Write();
	    HminusNoH_scale_muDw->Write();
	    HminusNoH_res_muUp->Write();
	    HminusNoH_res_muDw->Write();
	    HminusNoH_scale_elUp->Write();
	    HminusNoH_scale_elDw->Write();
	    HminusNoH_res_elUp->Write();
	    HminusNoH_res_elDw->Write();
	  }

	  HminusNoH_scale_jUp->Write();	  
	  HminusNoH_scale_jDw->Write();
	  HminusNoH_res_jUp->Write();
	  HminusNoH_res_jDw->Write();
	  hStatUp->Write();
	  hStatDown->Write();

	}
	
	// systematics lnN
	lumiSys     = lumiSys     +"   1.026";

	if(SampleVector.at(iSample).m_isSignal == 1){
	  QCDScaleSys = QCDScaleSys +"   -";
	  PdfqqSys    = PdfqqSys    +"   -";
	  acceptSys   = acceptSys   +"   1.020";
	}
	else {
	  QCDScaleSys = QCDScaleSys +"   1.030";
	  PdfqqSys    = PdfqqSys    +"   1.050";
	  acceptSys   = acceptSys   +"   -";
	}

	btagSys     = btagSys     +"   1.040";	

	lepEffSys_mu   = lepEffSys_mu   +"   1.020";	  
	lepEffSys_el   = lepEffSys_el   +"   1.020";	  

	fakeRateShape = fakeRateShape +   "  -";	

	wrongChargeSys = wrongChargeSys + "  -";

	fakeLepSys_mu = fakeLepSys_mu + "  -";
	fakeLepSys_el = fakeLepSys_el + "  -";

	// object systematics
	muScaleShape = muScaleShape + "  1  ";
	muResShape   = muResShape   + "  1  ";
	elScaleShape = elScaleShape + "  1  ";
	elResShape   = elResShape   + "  1  ";
	jetScaleShape = jetScaleShape + "  1  ";
	jetResShape   = jetResShape   + "  1  ";

	// shape lines
	if(not TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	  
	  statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shape  shapeN2 " ;
          
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){

	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else
	      statShape     += " 1    " ;	    
	  }
	  statShape     += "\n" ;
	}
	else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH !=0){
	  statShape     += "CMS_HminusNoH_"+finalStateString+"_stat_shape  shapeN2 " ;
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else
	      statShape     += " 1    " ;	    
	  }
	  statShape     += "\n" ;
	}	
      }

      datacard<< lumiSys +"\n" ;
      datacard<< QCDScaleSys +"\n" ;
      datacard<< PdfqqSys +"\n";
      datacard<< btagSys +"\n";
      datacard<< acceptSys +"\n";

      if (TString(finalStateString).Contains("UU")){
	datacard<< lepEffSys_mu +"\n";
	datacard<< fakeLepSys_mu+"\n";	  
	datacard<< muScaleShape +"\n" ;
	datacard<< muResShape +"\n" ;
      }
      else if(TString(finalStateString).Contains("EE")){
	datacard<< wrongChargeSys +"\n";
	datacard<< lepEffSys_el+"\n";
	datacard<< fakeLepSys_el+"\n";	  
	datacard<< elScaleShape +"\n" ;
	datacard<< elResShape +"\n" ;
      }
      else{
	datacard<< lepEffSys_mu +"\n";
	datacard<< fakeLepSys_mu+"\n";	  
	datacard<< muScaleShape +"\n" ;
	datacard<< muResShape +"\n" ;
	datacard<< wrongChargeSys +"\n";
	datacard<< lepEffSys_el+"\n";
	datacard<< fakeLepSys_el+"\n";	  
	datacard<< elScaleShape +"\n" ;
	datacard<< elResShape +"\n" ;
      }

      datacard<< jetScaleShape +"\n";
      datacard<< jetResShape +"\n";
      datacard<< fakeRateShape +"\n";
      datacard<< statShape ;

      datacard.close();
      outputCard->Close();	      

    }
  }
  
  //###########################//      
  //####### TH2 sector ########//
  //###########################//      
  
  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){

    // loop on variables 2D
    for(size_t iVar = 0; iVar < variableList2D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+"_"+finalStateString+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+"_"+finalStateString+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+"_"+finalStateString+".root").c_str(),"RECREATE");
      outputCard->cd();

      // print head of the datacad
      datacard << "## shape analysis WW scattering" << endl;
      datacard << "imax 1 number of channels"       << endl;
      datacard << "jmax * number of background"     << endl;
      datacard << "kmax * number of nuisance parameters" << endl;
      datacard <<"------------------------------------------------------"<< endl;
      datacard <<"bin  "+CutList.at(iCut).cutLayerName+"_"+finalStateString<< endl;
			     
      /// loop on samples and build the dataset
      TH2F* hTotal   = 0;
      TH1F* observed = 0;

      /// identify if H126 and noH are simoultaneusly in the sample list and fill the signal for this analysis
      TH2F* Higgs126   = 0;
      TH2F* Higgs126_scale_muUp = 0, *Higgs126_scale_muDw = 0, *Higgs126_res_muUp   = 0;
      TH2F* Higgs126_scale_elUp = 0, *Higgs126_scale_elDw = 0, *Higgs126_res_elUp   = 0;
      TH2F* Higgs126_scale_jUp  = 0, *Higgs126_scale_jDw  = 0, *Higgs126_res_jUp   = 0;

      TH2F* noHiggs            = 0;
      TH2F* noHiggs_scale_muUp = 0, *noHiggs_scale_muDw  = 0, *noHiggs_res_muUp  = 0;
      TH2F* noHiggs_scale_elUp = 0, *noHiggs_scale_elDw  = 0, *noHiggs_res_elUp  = 0;
      TH2F* noHiggs_scale_jUp  = 0, *noHiggs_scale_jDw  = 0, *noHiggs_res_jUp    = 0;

      TH2F* HminusNoH            = 0;
      TH2F* HminusNoH_scale_muUp = 0, *HminusNoH_scale_muDw = 0, *HminusNoH_res_muUp   = 0;
      TH2F* HminusNoH_scale_elUp = 0, *HminusNoH_scale_elDw = 0, *HminusNoH_res_elUp   = 0;
      TH2F* HminusNoH_scale_jUp  = 0, *HminusNoH_scale_jDw  = 0, *HminusNoH_res_jUp    = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(TString(SampleVector.at(iSample).m_sampleName).Contains("126")){

	  Higgs126 = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126");

	  Higgs126_scale_muUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_muUp");

	  Higgs126_scale_muDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_muDown");

	  Higgs126_res_muUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_muUp");


	  Higgs126_scale_elUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_elUp");

	  Higgs126_scale_elDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_elDown");

	  Higgs126_res_elUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_elUp");

	  Higgs126_scale_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_jUp");

	  Higgs126_scale_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_jDown");

	  Higgs126_res_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_jUp");

	}

        else if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  noHiggs = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs");

	  noHiggs_scale_muUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_muUp");

	  noHiggs_scale_muDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_muDown");

	  noHiggs_res_muUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_muUp");

	  noHiggs_scale_elUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_elUp");

	  noHiggs_scale_elDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_elDown");

	  noHiggs_res_elUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_elUp");


	  noHiggs_scale_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_jUp");

	  noHiggs_scale_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_jDown");

	  noHiggs_res_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_jUp");

	}
      }

      // if both are not null
      if(Higgs126 != 0 and noHiggs != 0){      

	HminusNoH = (TH2F*) noHiggs->Clone("histo_HminusNoH_2D");
	HminusNoH->Add(Higgs126,-1);

	HminusNoH_scale_muUp = (TH2F*) noHiggs_scale_muUp->Clone("histo_HminusNoH_CMS_scale_muUp_2D");
	HminusNoH_scale_muUp ->Add(Higgs126_scale_muUp,-1);

	HminusNoH_scale_muDw = (TH2F*) noHiggs_scale_muDw->Clone("histo_HminusNoH_CMS_scale_muDown_2D");
	HminusNoH_scale_muDw->Add(Higgs126_scale_muDw,-1);

	HminusNoH_res_muUp = (TH2F*) noHiggs_res_muUp->Clone("histo_HminusNoH_CMS_res_muUp_2D");
	HminusNoH_res_muUp->Add(Higgs126_res_muUp,-1);

	HminusNoH_scale_elUp = (TH2F*) noHiggs_scale_elUp->Clone("histo_HminusNoH_CMS_scale_elUp_2D");
	HminusNoH_scale_elUp ->Add(Higgs126_scale_elUp,-1);

	HminusNoH_scale_elDw = (TH2F*) noHiggs_scale_elDw->Clone("histo_HminusNoH_CMS_scale_elDown_2D");
	HminusNoH_scale_elDw->Add(Higgs126_scale_elDw,-1);

	HminusNoH_res_elUp = (TH2F*) noHiggs_res_elUp->Clone("histo_HminusNoH_CMS_res_elUp_2D");
	HminusNoH_res_elUp->Add(Higgs126_res_elUp,-1);

	HminusNoH_scale_jUp = (TH2F*) noHiggs_scale_jUp->Clone("histo_HminusNoH_CMS_scale_jUp_2D");
	HminusNoH_scale_jUp->Add(Higgs126_scale_jUp,-1);

	HminusNoH_scale_jDw = (TH2F*) noHiggs_scale_jDw->Clone("histo_HminusNoH_CMS_scale_jDown_2D");
	HminusNoH_scale_jDw->Add(Higgs126_scale_jDw,-1);

	HminusNoH_res_jUp = (TH2F*) noHiggs_res_jUp->Clone("histo_HminusNoH_CMS_res_jUp_2D");
	HminusNoH_res_jUp->Add(Higgs126_res_jUp,-1);

      }

      // make the dataset for each card         
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("126")) continue ;

        if(hTotal == 0) 
	  hTotal = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone(("hTotal_"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY).c_str());
        else 
	  hTotal->Add(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]);
      }  

      
      hTotal->SetBinErrorOption(TH1::kPoisson);

      observed = unRollingHistogram(hTotal,errorType); // rool histo in a TH1F      
      avoidEmptyBins(observed);
      observed->SetName("histo_Data");
      observed->SetBinErrorOption(TH1::kPoisson);
      TH1F* hNominal = (TH1F*) observed->Clone("hNominal");
      observed->Reset();
      observed->FillRandom(hNominal,gRandom->Poisson(hNominal->Integral()));            
      observed->Write();

      datacard<< Form("observation %d",int(observed->Integral()))<< endl;
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+"_"+finalStateString+".root    histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+"_"+finalStateString+".root    histo_Data" <<endl;
      datacard<< "------------------------------------------------------"<< endl;

            
      int nSignal     = 0;
      int nBackground = 1;

      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";

      // fill nuisances due to objects:
      TH1F* hMuScaleUp   = 0, *hMuScaleDown = 0, *hMuResUp     = 0, *hMuResDown   = 0;
      TH1F* hElScaleUp   = 0, *hElScaleDown = 0, *hElResUp     = 0, *hElResDown   = 0;
      TH1F* hJetScaleUp   = 0, *hJetScaleDown = 0, *hJetResUp     = 0, *hJetResDown   = 0;
      TH1F* hStatUp       = 0, *hStatDown = 0;
      TH1F* hfakeRateUp   = 0, *hfakeRateDown = 0;


      string muScaleShape  = "CMS_scale_mu  shapeN2";
      string elScaleShape  = "CMS_scale_el  shapeN2";
      string muResShape    = "CMS_res_mu    shapeN2";
      string elResShape    = "CMS_res_el    shapeN2";
      string jetScaleShape = "CMS_scale_j  shapeN2";
      string jetResShape   = "CMS_res_j    shapeN2";
      string fakeRateShape = "CMS_fakeLep_"+finalStateString+"_shape  shapeN2";
      string statShape     = "";

      // Make systematics lnN lines
      string lumiSys        = "CMS_lumi         lnN";
      string wrongChargeSys = "CMS_wrongCharge  lnN";
      string QCDScaleSys    = "QCDScale     lnN";
      string PdfqqSys       = "Pdfqq        lnN";
      string btagSys        = "CMS_btag_eff lnN";
      string lepEffSys_mu   = "CMS_lep_eff_mu   lnN";
      string lepEffSys_el   = "CMS_lep_eff_el   lnN";
      string acceptSys      = "CMS_accept   lnN";
      string fakeLepSys_mu  = "CMS_fakeLep_mu  lnN";
      string fakeLepSys_el  = "CMS_fakeLep_el  lnN";

      // loop on samples
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	// fill nominal histograms in the file
	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += "HminusNoH   ";
          hNominal = unRollingHistogram(HminusNoH,errorType) ;	  
	  avoidEmptyBins(hNominal);
	}
	else{
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += SampleVector.at(iSample).m_sampleName+"   ";
	  hNominal = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  avoidEmptyBins(hNominal);
	}
 
	// write in the output file
        if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))
	  hNominal->Write("histo_HminusNoH");
	else
	  hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        
	/// rate line computation
	if(HminusNoH != 0){
	  if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	    lineRate += Form("%f   ",hNominal->Integral());  
	  }
	  else{
	    lineProcess2 += Form("%d   ",nBackground);
	    nBackground++;
	    lineRate += Form("%f   ",hNominal->Integral());  	
	  }
	}
	else {
	  if(SampleVector.at(iSample).m_isSignal > 0){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	  }
	  else{
	    lineProcess2 += Form("%d   ",nBackground);
	    nBackground++;
	  }
	  lineRate += Form("%f   ",hNominal->Integral());  	
	}
	
        // make the stat histograms	
	if((HminusNoH == 0 or (HminusNoH !=0 and !TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))) and SampleVector.at(iSample).m_isSignal >= 0){

	  // make systematic variations

	  hMuScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hMuScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_muUp").c_str());

	    
	  hMuScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hMuScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_muDown").c_str());

	  hMuResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_muRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hMuResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_muUp").c_str());        
	  
	  hMuResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_muDown",hNominal,hMuResUp);


	  hElScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hElScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_elUp").c_str());

	    
	  hElScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hElScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_elDown").c_str());

	  hElResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_elRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hElResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_elUp").c_str());        
	  
	  hElResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_elDown",hNominal,hElResUp);


	  hJetScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jUp").c_str());

	  hJetScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jDown").c_str());

	  hJetResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jUp").c_str());        

	  hJetResDown = mirrorHistogram("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jDown",hNominal,hJetResUp);

	  avoidEmptyBins(hMuScaleUp);
	  avoidEmptyBins(hMuScaleDown);
	  avoidEmptyBins(hMuResUp);
	  avoidEmptyBins(hMuResDown);

	  avoidEmptyBins(hElScaleUp);
	  avoidEmptyBins(hElScaleDown);
	  avoidEmptyBins(hElResUp);
	  avoidEmptyBins(hElResDown);

	  avoidEmptyBins(hJetScaleUp);
	  avoidEmptyBins(hJetScaleDown);
	  avoidEmptyBins(hJetResUp);
	  avoidEmptyBins(hJetResDown);

	  if(TString(finalStateString).Contains("UU")){
	    hMuScaleUp   ->Write();
	    hMuScaleDown ->Write();
	    hMuResUp     ->Write();
	    hMuResDown   ->Write();
	  }
	  else if(TString(finalStateString).Contains("EE")){
	    hElScaleUp   ->Write();
	    hElScaleDown ->Write();
	    hElResUp     ->Write();
	    hElResDown   ->Write();
	  }
	  else {

	    hMuScaleUp   ->Write();
	    hMuScaleDown ->Write();
	    hMuResUp     ->Write();
	    hMuResDown   ->Write();

	    hElScaleUp   ->Write();
	    hElScaleDown ->Write();
	    hElResUp     ->Write();
	    hElResDown   ->Write();

	  }

	  hJetScaleUp   ->Write();
	  hJetScaleDown ->Write();
	  hJetResUp     ->Write();
	  hJetResDown   ->Write();

	}
	else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH != 0 and SampleVector.at(iSample).m_isSignal >= 0){

	  hStatUp   = (TH1F*) hNominal->Clone(("histo_HminusNoH_CMS_HminusNoH_"+finalStateString+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) hNominal->Clone(("histo_HminusNoH_CMS_HminusNoH_"+finalStateString+"_stat_shapeDown").c_str());

	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  }

	  // unRolling
	  hMuScaleUp = unRollingHistogram(HminusNoH_scale_muUp,errorType);
	  hMuScaleUp->SetName("histo_HminusNoH_CMS_scale_muUp");

	  hMuScaleDown = unRollingHistogram(HminusNoH_scale_muDw,errorType);
	  hMuScaleDown->SetName("histo_HminusNoH_CMS_scale_muDown");

	  hMuResUp = unRollingHistogram(HminusNoH_res_muUp,errorType);
	  hMuResUp->SetName("histo_HminusNoH_CMS_res_muUp");

	  hMuResDown = mirrorHistogram("histo_HminusNoH_CMS_res_muDown",hNominal,hMuResUp);


	  hElScaleUp = unRollingHistogram(HminusNoH_scale_elUp,errorType);
	  hElScaleUp->SetName("histo_HminusNoH_CMS_scale_elUp");

	  hElScaleDown = unRollingHistogram(HminusNoH_scale_elDw,errorType);
	  hElScaleDown->SetName("histo_HminusNoH_CMS_scale_elDown");

	  hElResUp = unRollingHistogram(HminusNoH_res_elUp,errorType);
	  hElResUp->SetName("histo_HminusNoH_CMS_res_elUp");

	  hElResDown = mirrorHistogram("histo_HminusNoH_CMS_res_elDown",hNominal,hElResUp);


	  hJetScaleUp = unRollingHistogram(HminusNoH_scale_jUp,errorType);
	  hJetScaleUp->SetName("histo_HminusNoH_CMS_scale_jUp");

	  hJetScaleDown = unRollingHistogram(HminusNoH_scale_jDw,errorType);
	  hJetScaleDown->SetName("histo_HminusNoH_CMS_scale_jDown");

	  hJetResUp = unRollingHistogram(HminusNoH_res_jUp,errorType);
	  hJetResUp->SetName("histo_HminusNoH_CMS_res_jUp");

	  hJetResDown = mirrorHistogram("histo_HminusNoH_CMS_res_jDown",hNominal,hJetResUp);

	  avoidEmptyBins(hMuScaleUp);
	  avoidEmptyBins(hMuScaleDown);
	  avoidEmptyBins(hMuResUp);
	  avoidEmptyBins(hMuResDown);

	  avoidEmptyBins(hElScaleUp);
	  avoidEmptyBins(hElScaleDown);
	  avoidEmptyBins(hElResUp);
	  avoidEmptyBins(hElResDown);

	  avoidEmptyBins(hStatUp);
	  avoidEmptyBins(hStatDown);


	  avoidEmptyBins(hJetScaleUp);
	  avoidEmptyBins(hJetScaleDown);
	  avoidEmptyBins(hJetResUp);
	  avoidEmptyBins(hJetResDown);

					      
	  if(TString(finalStateString).Contains("UU")){
	    hMuScaleUp   ->Write();
	    hMuScaleDown ->Write();
	    hMuResUp     ->Write();
	    hMuResDown   ->Write();
	  }
	  else if(TString(finalStateString).Contains("EE")){
	    hElScaleUp   ->Write();
	    hElScaleDown ->Write();
	    hElResUp     ->Write();
	    hElResDown   ->Write();
	  }
	  else {

	    hMuScaleUp   ->Write();
	    hMuScaleDown ->Write();
	    hMuResUp     ->Write();
	    hMuResDown   ->Write();

	    hElScaleUp   ->Write();
	    hElScaleDown ->Write();
	    hElResUp     ->Write();
	    hElResDown   ->Write();

	  }

	  hJetScaleUp->Write();
	  hJetScaleDown->Write();
	  hJetResUp->Write();
	  hJetResDown->Write();

	  hStatUp->Write();
	  hStatDown->Write();
	    
	}

	// inject mc stat shape variation
	if(SampleVector.at(iSample).m_isSignal >= 0){

	  hStatUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shapeDown").c_str());

	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  }

	  avoidEmptyBins(hStatUp);
	  avoidEmptyBins(hStatDown);
   
	  hStatUp->Write();
	  hStatDown->Write();

	  // systematics lnN                                                                                                                                              
	  lumiSys     = lumiSys     +"   1.026";

	  if(SampleVector.at(iSample).m_isSignal == 1){
	    QCDScaleSys = QCDScaleSys +"   -";
	    PdfqqSys    = PdfqqSys    +"   -";
	    acceptSys   = acceptSys   +"   1.020";
	  }
	  else {
	    QCDScaleSys = QCDScaleSys +"   1.030";
	    PdfqqSys    = PdfqqSys    +"   1.070";
	    acceptSys   = acceptSys   +"   -";
	  }

	  btagSys        = btagSys        +"   1.040";
	  lepEffSys_mu   = lepEffSys_mu   +"   1.020";
	  lepEffSys_el   = lepEffSys_el   +"   1.020";

	  fakeRateShape = fakeRateShape +   "  -";

	  wrongChargeSys = wrongChargeSys + "  -";

          fakeLepSys_mu  = fakeLepSys_mu +"  -";
          fakeLepSys_el  = fakeLepSys_el +"  -";

	  // object systematics                                                                                                                                            
	  muScaleShape = muScaleShape + "  1  ";
	  elScaleShape = elScaleShape + "  1  ";
	  muResShape   = muResShape   + "  1  ";
	  elResShape   = elResShape   + "  1  ";
	  jetScaleShape = jetScaleShape + "  1  ";
	  jetResShape   = jetResShape   + "  1  ";
	    
	  // shape lines
	  if(not TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	    statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_"+finalStateString+"_stat_shape  shapeN2 " ;          
	    for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	      if(iSample2!=iSample)
		statShape     += " -    " ;	    
	      else      
		statShape     += " 1    " ;	    
	    }
	    statShape     += "\n" ;
	  }

	  else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH !=0){
	    statShape     += "CMS_HminusNoH_"+finalStateString+"_stat_shape  shapeN2 " ;
	    for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	      if(iSample2!=iSample)
		statShape     += " -    " ;	    
	      else
		statShape     += " 1    " ;	    
	    }	    
	    statShape     += "\n" ;
	  }
	}
	else {
	    
	  lumiSys     = lumiSys     +"   -";
	  QCDScaleSys = QCDScaleSys +"   -";
	  PdfqqSys    = PdfqqSys    +"   -";
	  btagSys     = btagSys     +"   -";
          acceptSys   = acceptSys   +"   -";

	  lepEffSys_mu   = lepEffSys_mu   +"   -";
	  lepEffSys_el   = lepEffSys_el   +"   -";

	  if(SampleVector.at(iSample).m_isSignal != -1){
	   fakeRateShape = fakeRateShape +   "  -";
	   fakeLepSys_mu = fakeLepSys_mu + "  -";	    
	   fakeLepSys_el = fakeLepSys_el + "  -";	    
	  }
	  else if(SampleVector.at(iSample).m_isSignal == -1){

	    fakeRateShape = fakeRateShape + "  1";
            fakeLepSys_mu = fakeLepSys_mu + "  1.30";	    
            fakeLepSys_el = fakeLepSys_el + "  1.30";	    
	    hfakeRateUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_fakeLep_"+finalStateString+"_shapeUp").c_str());
	    hfakeRateDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_fakeLep_"+finalStateString+"_shapeDown").c_str());

	    for (int iBin = 0; iBin < hfakeRateUp->GetNbinsX()+1; iBin++){
	      hfakeRateUp->SetBinContent(iBin,hfakeRateUp->GetBinContent(iBin)+hfakeRateUp->GetBinError(iBin));
	      hfakeRateDown->SetBinContent(iBin,hfakeRateDown->GetBinContent(iBin)-hfakeRateDown->GetBinError(iBin));
	    }

	    avoidEmptyBins(hfakeRateUp);
	    avoidEmptyBins(hfakeRateDown);

	    hfakeRateUp->Write();
	    hfakeRateDown->Write();
	    	  	    
	  }	  

	  if(SampleVector.at(iSample).m_isSignal != -2)
	    wrongChargeSys = wrongChargeSys + "  -";
	  else if(SampleVector.at(iSample).m_isSignal == -2)
	    wrongChargeSys = wrongChargeSys + "  1.30";
	  
	  muScaleShape  = muScaleShape + "  -  ";
	  muResShape    = muResShape   + "  -  ";

	  elScaleShape  = elScaleShape + "  -  ";
	  elResShape    = elResShape   + "  -  ";

	  jetScaleShape = jetScaleShape + "  -  ";
	  jetResShape   = jetResShape   + "  -  ";

	  continue;

	}
      }
      
      datacard << lineBin << endl;
      datacard << lineProcess << endl;
      datacard << lineProcess2 << endl;
      datacard << lineRate << endl;

      datacard<< "------------------------------------------------------"<< endl;

      datacard<< lumiSys +"\n" ;
      datacard<< QCDScaleSys +"\n" ;
      datacard<< PdfqqSys +"\n";
      datacard<< btagSys +"\n";
      datacard<< acceptSys +"\n";
      datacard<< wrongChargeSys +"\n";

      if(TString(finalStateString).Contains("UU")){
	datacard<< lepEffSys_mu +"\n";
	datacard<< fakeLepSys_mu +"\n";
	datacard<< muScaleShape + "\n";
	datacard<< muResShape + "\n";
      }
      else if(TString(finalStateString).Contains("EE")){
	datacard<< lepEffSys_el +"\n";
	datacard<< fakeLepSys_el +"\n";
	datacard<< elScaleShape + "\n";
	datacard<< elResShape + "\n";
      }
      else {
	datacard<< lepEffSys_mu +"\n";
	datacard<< fakeLepSys_mu +"\n";
	datacard<< muScaleShape + "\n";
	datacard<< muResShape + "\n";
	datacard<< lepEffSys_el +"\n";
	datacard<< fakeLepSys_el +"\n";
	datacard<< elScaleShape + "\n";
	datacard<< elResShape + "\n";
      }

      datacard<< jetScaleShape + "\n";
      datacard<< jetResShape + "\n";
      datacard<< fakeRateShape + "\n";
      datacard<< statShape ;

      
      datacard.close();
      outputCard->Close(); 
      
    }
  }
  
  // Make datacards for 1D histograms
  TFile* outputEfficiency = new TFile(("output/"+outputDataCardDirectory+"/outputEfficiency_"+finalStateString+".root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    

  return 0 ;
}  
