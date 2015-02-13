/////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <map>
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

  map<string,vector<sampleContainer> > sampleMap ;
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

  for( map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){ // loop on each sample

    TChain* chain = new TChain (treeName.c_str()) ;  // take the file chain
    int numBefore = 0;

    for(size_t iContainer = 0; iContainer < itSample->second.size(); iContainer++){     
      numBefore += itSample->second.at(iContainer).numBefore; 
      chain->Add ((InputBaseDirectory+"/"+itSample->second.at(iContainer).sampleName+"/*.root").c_str()) ;
    }

    int totEvent = chain->GetEntries();

    readTree* ReadTree  = new readTree((TTree*)(chain)); // reader for chain trees

    cout<<"Sample name : "<<itSample->first<<" Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<itSample->second.at(0).xsec<<" Nevents before selections "<<lumi*itSample->second.at(0).xsec<<" weight "<<lumi*itSample->second.at(0).xsec/totEvent<<endl;

    // add  sample to the analysis plot container
    if(numBefore > totEvent)
      analysisPlots.addSample(itSample->first,itSample->second.at(0).xsec,numBefore, itSample->second.at(0).isSignal, itSample->second.at(0).color) ;   // create the sample to analyze
    else
      analysisPlots.addSample(itSample->first,itSample->second.at(0).xsec,totEvent, itSample->second.at(0).isSignal, itSample->second.at(0).color) ;   // create the sample to analyze
 

   // Add cuts to the analysis plot container
   for(size_t iCut = 0; iCut < CutList.size(); iCut++){
     analysisPlots.addLayerToSample  (itSample->first,CutList.at(iCut).cutLayerName) ;      
     histoCutEff[itSample->first+"_"+CutList.at(iCut).cutLayerName] = new TH1F((itSample->first+"_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);

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


   loopOnEvents(analysisPlots,
		ReadTree,
		CutList,
		variableList1D,     // 1D variables
		variableList2D,     // 2D variables
		sampleName,         // sample name
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
		scenarioString
		);

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
      TH1F* Higgs126_scale_lUp = 0, *Higgs126_scale_lDw = 0, *Higgs126_res_lUp   = 0, *Higgs126_res_lDw   = 0;
      TH1F* Higgs126_scale_jUp = 0, *Higgs126_scale_jDw = 0,* Higgs126_res_jUp   = 0, *Higgs126_res_jDw   = 0;

      TH1F* noHiggs          = 0;
      TH1F* noHiggs_scale_lUp  = 0, *noHiggs_scale_lDw  = 0, *noHiggs_res_lUp    = 0, *noHiggs_res_lDw    = 0;
      TH1F* noHiggs_scale_jUp  = 0, *noHiggs_scale_jDw  = 0, *noHiggs_res_jUp    = 0, *noHiggs_res_jDw    = 0;

      TH1F* HminusNoH          = 0;
      TH1F* HminusNoH_scale_lUp  = 0, *HminusNoH_scale_lDw  = 0, *HminusNoH_res_lUp    = 0, *HminusNoH_res_lDw    = 0;
      TH1F* HminusNoH_scale_jUp  = 0, *HminusNoH_scale_jDw  = 0, *HminusNoH_res_jUp    = 0, *HminusNoH_res_jDw    = 0;

      // fill histograms
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(TString(SampleVector.at(iSample).m_sampleName).Contains("126")){

	  Higgs126 = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126");

	  Higgs126_scale_lUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_lUp");

	  Higgs126_scale_lDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_lDown");

	  Higgs126_res_lUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepRes[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_lUp");

	  Higgs126_res_lDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_lDown");

	  Higgs126_scale_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_jUp");

	  Higgs126_scale_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_scale_jDown");

	  Higgs126_res_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_jUp");

	  Higgs126_res_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_Higgs126_CMS_res_jDown");	  

	  // add overflow and underflow bins
	  addOverAndUnderFlow(Higgs126);
	  addOverAndUnderFlow(Higgs126_scale_lUp);
	  addOverAndUnderFlow(Higgs126_scale_lDw);
	  addOverAndUnderFlow(Higgs126_res_lUp);
	  addOverAndUnderFlow(Higgs126_res_lDw);
	  addOverAndUnderFlow(Higgs126_scale_jUp);
	  addOverAndUnderFlow(Higgs126_scale_jDw);
	  addOverAndUnderFlow(Higgs126_res_jUp);
	  addOverAndUnderFlow(Higgs126_res_jDw);

	}

        else if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  noHiggs = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs");

	  noHiggs_scale_lUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_lUp");

	  noHiggs_scale_lDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_lDown");

	  noHiggs_res_lUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepRes[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_lUp");

	  noHiggs_res_lDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_lDown");

	  noHiggs_scale_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_jUp");

	  noHiggs_scale_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_scale_jDown");

	  noHiggs_res_jUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_jUp");

	  noHiggs_res_jDw = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone("histo_noHiggs_CMS_res_jDown");

	  //add overflow and underflow
	  addOverAndUnderFlow(noHiggs);
	  addOverAndUnderFlow(noHiggs_scale_lUp);
	  addOverAndUnderFlow(noHiggs_scale_lDw);
	  addOverAndUnderFlow(noHiggs_res_lUp);
	  addOverAndUnderFlow(noHiggs_res_lDw);
	  addOverAndUnderFlow(noHiggs_scale_jUp);
	  addOverAndUnderFlow(noHiggs_scale_jDw);
	  addOverAndUnderFlow(noHiggs_res_jUp);
	  addOverAndUnderFlow(noHiggs_res_jDw);

	}
      }

      // if both 126 and noH are there make the difference as signal to test partial unitarization and make sure to be positive defined
      if(Higgs126 != 0 and noHiggs != 0){      

	HminusNoH = (TH1F*) noHiggs->Clone("histo_HminusNoH");
	HminusNoH->Add(Higgs126,-1);
        makePositiveDefine(HminusNoH);

	HminusNoH_scale_lUp = (TH1F*) noHiggs_scale_lUp->Clone("histo_HminusNoH_CMS_scale_lUp");
	HminusNoH_scale_lUp ->Add(Higgs126_scale_lUp,-1);
        makePositiveDefine(HminusNoH_scale_lUp);

	HminusNoH_scale_lDw = (TH1F*) noHiggs_scale_lDw->Clone("histo_HminusNoH_CMS_scale_lDown");
	HminusNoH_scale_lDw->Add(Higgs126_scale_lDw,-1);
        makePositiveDefine(HminusNoH_scale_lDw);

	HminusNoH_res_lUp = (TH1F*) noHiggs_res_lUp->Clone("histo_HminusNoH_CMS_res_lUp");
	HminusNoH_res_lUp->Add(Higgs126_res_lUp,-1);
        makePositiveDefine(HminusNoH_res_lUp);

	HminusNoH_res_lDw = (TH1F*) noHiggs_res_lDw->Clone("histo_HminusNoH_CMS_res_lDown");
	HminusNoH_res_lDw->Add(Higgs126_res_lDw,-1);
        makePositiveDefine(HminusNoH_res_lDw);

	HminusNoH_scale_jUp = (TH1F*) noHiggs_scale_jUp->Clone("histo_HminusNoH_CMS_scale_jUp");
	HminusNoH_scale_jUp->Add(Higgs126_scale_jUp,-1);
        makePositiveDefine(HminusNoH_scale_jUp);

	HminusNoH_scale_jDw = (TH1F*) noHiggs_scale_jDw->Clone("histo_HminusNoH_CMS_scale_jDown");
	HminusNoH_scale_jDw->Add(Higgs126_scale_jDw,-1);
        makePositiveDefine(HminusNoH_scale_jDw);

	HminusNoH_res_jUp = (TH1F*) noHiggs_res_jUp->Clone("histo_HminusNoH_CMS_res_jUp");
	HminusNoH_res_jUp->Add(Higgs126_res_jUp,-1);
        makePositiveDefine(Higgs126_res_jUp);

	HminusNoH_res_jDw = (TH1F*) noHiggs_res_jDw->Clone("histo_HminusNoH_CMS_res_jDown");
	HminusNoH_res_jDw->Add(Higgs126_res_jDw,-1);
        makePositiveDefine(HminusNoH_res_jDw);

      }
      
      // make the total distribution
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("126")) continue ;

        if(hTotal == 0)
	  hTotal = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("hTotal_"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName).c_str());
        else 
	  hTotal->Add(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]);
      }  


      hTotal->SetBinContent(1,hTotal->GetBinContent(1)+hTotal->GetBinContent(0)); // add underflow to the last bin      
      hTotal->SetBinContent(hTotal->GetNbinsX(),hTotal->GetBinContent(hTotal->GetNbinsX())+hTotal->GetBinContent(hTotal->GetNbinsX()+1)); // add overflow to the last bin
      hTotal->SetBinErrorOption(TH1::kPoisson);

      // make a toy on it + setting poisson errors
      observed = (TH1F*)  hTotal->Clone("histo_Data");
      observed->SetBinErrorOption(TH1::kPoisson);
      observed->Reset();
      observed->FillRandom(hTotal,gRandom->Poisson(hTotal->Integral()));            
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

      int nSignal = 0 ;
      int nBackground = 1 ;

      TH1F* hNominal      = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += "HminusNoH   ";
          hNominal = HminusNoH ;
	}
	else{
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += SampleVector.at(iSample).m_sampleName+"   ";
	  hNominal = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];
	  addOverAndUnderFlow(hNominal);
	}
 

        if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))
	  hNominal->Write("histo_HminusNoH");
	else
	  hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        
        /// not for H-noH analysis
	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains(" noH")){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	    lineRate += Form("%f   ",hNominal->Integral());  
	}
	else {
	  if(SampleVector.at(iSample).m_isSignal){
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
      
      // make systematics lines and shapes in the root file
      string lepScaleShape = "CMS_scale_l  shapeN2";
      string lepResShape   = "CMS_res_l    shapeN2";
      string jetScaleShape = "CMS_scale_j  shapeN2";
      string jetResShape   = "CMS_res_j    shapeN2";
      string statShape     = "";

      // stat shapes ;
      TH1F* hStatUp   = 0, *hStatDown = 0;
      TH1F* hLepScaleUp   = 0, *hLepScaleDown = 0, *hLepResUp     = 0, *hLepResDown   = 0;
      TH1F* hJetScaleUp   = 0, *hJetScaleDown = 0, *hJetResUp     = 0, *hJetResDown   = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
	
	if(HminusNoH == 0 or (HminusNoH !=0 and !TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))){

	  hNominal = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName] ;         

	  hLepScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_lUp").c_str());
	  hLepScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_lDown").c_str());
	  hLepResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_lUp").c_str());
	  hLepResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_lDown").c_str());

	  hJetScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jUp").c_str());
	  hJetScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jDown").c_str());
	  hJetResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jUp").c_str());
	  hJetResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jDown").c_str());

	  // stat shapes
	  hStatUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeDown").c_str());

	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	    if(hStatDown->GetBinContent(iBin) < 0) 
	      hStatDown->SetBinContent(iBin,0);
	  }

	  // add overflow and underflow         
	  addOverAndUnderFlow(hLepScaleUp);
	  addOverAndUnderFlow(hLepScaleDown);
	  addOverAndUnderFlow(hLepResUp);
	  addOverAndUnderFlow(hLepResDown);
	  addOverAndUnderFlow(hJetScaleUp);
	  addOverAndUnderFlow(hJetScaleDown);
	  addOverAndUnderFlow(hJetResUp);
	  addOverAndUnderFlow(hJetResDown);
	  addOverAndUnderFlow(hStatUp);
	  addOverAndUnderFlow(hStatDown);

	  // write the histograms in the file
	  hLepScaleUp->Write();
	  hLepScaleDown->Write();
	  hLepResUp->Write();
	  hLepResDown->Write();
	  hJetScaleUp->Write();
	  hJetScaleDown->Write();
	  hJetResUp->Write();
	  hJetResDown->Write();
	  hStatUp->Write();
	  hStatDown->Write();
	}

	else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH != 0){
	  hStatUp   = (TH1F*) HminusNoH->Clone("histo_HminusNoH_CMS_HminusNoH_stat_shapeUp");
	  hStatDown = (TH1F*) HminusNoH->Clone("histo_HminusNoH_CMS_HminusNoH_stat_shapeDown");

	  for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	    hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	    hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	    if(hStatDown->GetBinContent(iBin) < 0) 
	      hStatDown->SetBinContent(iBin,0);
	  }
	
	  HminusNoH_scale_lUp->Write();
	  HminusNoH_scale_lDw->Write();
	  HminusNoH_res_lUp->Write();
	  HminusNoH_res_lDw->Write();
	  HminusNoH_scale_jUp->Write();
	  HminusNoH_scale_jDw->Write();
	  HminusNoH_res_jUp->Write();
	  HminusNoH_res_jDw->Write();
	  hStatUp->Write();
	  hStatDown->Write();

	}

	
	// object systematics
	lepScaleShape = lepScaleShape + "  1  ";
	lepResShape   = lepResShape + "  1  ";
	jetScaleShape = jetScaleShape + "  1  ";
	jetResShape   = jetResShape + "  1  ";

        // shape lines
	if(not TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shape  shapeN2 " ;
           
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else
	      statShape     += " 1    " ;	    
	  }
	  statShape     += "\n" ;
	}
        else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH !=0){
	  statShape     += "CMS_HminusNoH_stat_shape  shapeN2 " ;
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else
	      statShape     += " 1    " ;	    
	  }
	  statShape     += "\n" ;
	}
      }

      datacard<< lepScaleShape +"\n" ;
      datacard<< lepResShape +"\n" ;
      datacard<< jetScaleShape +"\n";
      datacard<< jetResShape +"\n";
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
      TH2F* Higgs126_scale_lUp = 0, *Higgs126_scale_lDw = 0, *Higgs126_res_lUp   = 0, *Higgs126_res_lDw   = 0;
      TH2F* Higgs126_scale_jUp = 0, *Higgs126_scale_jDw = 0, *Higgs126_res_jUp   = 0, *Higgs126_res_jDw   = 0;

      TH2F* noHiggs            = 0;
      TH2F* noHiggs_scale_lUp  = 0, *noHiggs_scale_lDw  = 0, *noHiggs_res_lUp    = 0, *noHiggs_res_lDw    = 0;
      TH2F* noHiggs_scale_jUp  = 0, *noHiggs_scale_jDw  = 0, *noHiggs_res_jUp    = 0, *noHiggs_res_jDw    = 0;

      TH2F* HminusNoH            = 0;
      TH2F* HminusNoH_scale_lUp  = 0, *HminusNoH_scale_lDw  = 0, *HminusNoH_res_lUp    = 0, *HminusNoH_res_lDw    = 0;
      TH2F* HminusNoH_scale_jUp  = 0, *HminusNoH_scale_jDw  = 0, *HminusNoH_res_jUp    = 0, *HminusNoH_res_jDw    = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	if(TString(SampleVector.at(iSample).m_sampleName).Contains("126")){

	  Higgs126 = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126");

	  Higgs126_scale_lUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_lUp");

	  Higgs126_scale_lDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_lDown");

	  Higgs126_res_lUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_lUp");

	  Higgs126_res_lDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_lDown");

	  Higgs126_scale_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_jUp");

	  Higgs126_scale_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_scale_jDown");

	  Higgs126_res_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_jUp");

	  Higgs126_res_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_Higgs126_CMS_res_jDown");	  

	}

        else if(TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  noHiggs = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs");

	  noHiggs_scale_lUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_lUp");

	  noHiggs_scale_lDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_lDown");

	  noHiggs_res_lUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_lUp");

	  noHiggs_res_lDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_lDown");

	  noHiggs_scale_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_jUp");

	  noHiggs_scale_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_scale_jDown");

	  noHiggs_res_jUp = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_jUp");

	  noHiggs_res_jDw = (TH2F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY]->Clone("histo_noHiggs_CMS_res_jDown");

	}
      }

      // if both are not null
      if(Higgs126 != 0 and noHiggs != 0){      

	HminusNoH = (TH2F*) noHiggs->Clone("histo_HminusNoH_2D");
	HminusNoH->Add(Higgs126,-1);
        makePositiveDefine(HminusNoH);

	HminusNoH_scale_lUp = (TH2F*) noHiggs_scale_lUp->Clone("histo_HminusNoH_CMS_scale_lUp_2D");
	HminusNoH_scale_lUp ->Add(Higgs126_scale_lUp,-1);
        makePositiveDefine(HminusNoH_scale_lUp);

	HminusNoH_scale_lDw = (TH2F*) noHiggs_scale_lDw->Clone("histo_HminusNoH_CMS_scale_lDown_2D");
	HminusNoH_scale_lDw->Add(Higgs126_scale_lDw,-1);
        makePositiveDefine(HminusNoH_scale_lDw);

	HminusNoH_res_lUp = (TH2F*) noHiggs_res_lUp->Clone("histo_HminusNoH_CMS_res_lUp_2D");
	HminusNoH_res_lUp->Add(Higgs126_res_lUp,-1);
        makePositiveDefine(HminusNoH_res_lUp);

	HminusNoH_res_lDw = (TH2F*) noHiggs_res_lDw->Clone("histo_HminusNoH_CMS_res_lDown_2D");
	HminusNoH_res_lDw->Add(Higgs126_res_lDw,-1);
        makePositiveDefine(HminusNoH_res_lDw);

	HminusNoH_scale_jUp = (TH2F*) noHiggs_scale_jUp->Clone("histo_HminusNoH_CMS_scale_jUp_2D");
	HminusNoH_scale_jUp->Add(Higgs126_scale_jUp,-1);
        makePositiveDefine(HminusNoH_scale_jUp);

	HminusNoH_scale_jDw = (TH2F*) noHiggs_scale_jDw->Clone("histo_HminusNoH_CMS_scale_jDown_2D");
	HminusNoH_scale_jDw->Add(Higgs126_scale_jDw,-1);
        makePositiveDefine(HminusNoH_scale_jDw);

	HminusNoH_res_jUp = (TH2F*) noHiggs_res_jUp->Clone("histo_HminusNoH_CMS_res_jUp_2D");
	HminusNoH_res_jUp->Add(Higgs126_res_jUp,-1);
        makePositiveDefine(Higgs126_res_jUp);

	HminusNoH_res_jDw = (TH2F*) noHiggs_res_jDw->Clone("histo_HminusNoH_CMS_res_jDown_2D");
	HminusNoH_res_jDw->Add(Higgs126_res_jDw,-1);
        makePositiveDefine(HminusNoH_res_jDw);

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

            
      int nSignal = 0;
      int nBackground = 1;

      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";

      // fill nuisances due to objects:
      TH1F* hLepScaleUp   = 0, *hLepScaleDown = 0, *hLepResUp     = 0, *hLepResDown   = 0;
      TH1F* hJetScaleUp   = 0, *hJetScaleDown = 0, *hJetResUp     = 0, *hJetResDown   = 0;
      TH1F* hStatUp       = 0, *hStatDown = 0;


      string lepScaleShape = "CMS_scale_l  shapeN2";
      string lepResShape   = "CMS_res_l    shapeN2";
      string jetScaleShape = "CMS_scale_j  shapeN2";
      string jetResShape   = "CMS_res_j    shapeN2";
      string statShape = "";

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	// fill nominal histograms in the file
	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += "HminusNoH   ";
          hNominal = unRollingHistogram(HminusNoH,errorType) ;
	}
	else{
	  lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	  lineProcess += SampleVector.at(iSample).m_sampleName+"   ";
	  hNominal = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	}
 

        if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))
	  hNominal->Write("histo_HminusNoH");
	else
	  hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        
        /// not for H-noH analysis
	if(HminusNoH != 0 and TString(SampleVector.at(iSample).m_sampleName).Contains(" noH")){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	    lineRate += Form("%f   ",hNominal->Integral());  
	}
	else {
	  if(SampleVector.at(iSample).m_isSignal){
	    lineProcess2 += Form("%d   ",nSignal);          
	    nSignal--;
	  }
	  else{
	    lineProcess2 += Form("%d   ",nBackground);
	    nBackground++;
	  }
	  lineRate += Form("%f   ",hNominal->Integral());  	
	}


        // make the sta histograms	
	if(HminusNoH == 0 or (HminusNoH !=0 and !TString(SampleVector.at(iSample).m_sampleName).Contains("noH"))){

	  hStatUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeUp").c_str());
	  hStatDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeDown").c_str());

	  // make systematic variations
	  hLepScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hLepScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_lUp").c_str());

	  hLepScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hLepScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_lDown").c_str());

	  hLepResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hLepResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_lUp").c_str());        
	  
	  hLepResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_lDown").c_str());

	  hJetScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jUp").c_str());

	  hJetScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_scale_jDown").c_str());

	  hJetResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	  hJetResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jUp").c_str());        

	  hJetResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_res_jDown").c_str());

	  hLepScaleUp   ->Write();
	  hLepScaleDown ->Write();
	  hLepResUp     ->Write();
	  hLepResDown   ->Write();
	  hJetScaleUp   ->Write();
	  hJetScaleDown ->Write();
	  hJetResUp     ->Write();
	  hJetResDown   ->Write();
	}
	else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH != 0){
	  hStatUp   = (TH1F*) hNominal->Clone("histo_HminusNoH_CMS_HminusNoH_stat_shapeUp");
	  hStatDown = (TH1F*) hNominal->Clone("histo_HminusNoH_CMS_HminusNoH_stat_shapeDown");

	  // unRolling
	  hLepScaleUp = unRollingHistogram(HminusNoH_scale_lUp,errorType);
	  hLepScaleUp->SetName("histo_HminusNoH_CMS_scale_lUp");

	  hLepScaleDown = unRollingHistogram(HminusNoH_scale_lDw,errorType);
	  hLepScaleDown->SetName("histo_HminusNoH_CMS_scale_lDown");

	  hLepResUp = unRollingHistogram(HminusNoH_res_lUp,errorType);
	  hLepResUp->SetName("histo_HminusNoH_CMS_res_lUp");

	  hLepResDown = (TH1F*) hNominal->Clone("histo_HminusNoH_CMS_res_lDown");

	  hJetScaleUp = unRollingHistogram(HminusNoH_scale_jUp,errorType);
	  hJetScaleUp->SetName("histo_HminusNoH_CMS_scale_jUp");

	  hJetScaleDown = unRollingHistogram(HminusNoH_scale_jDw,errorType);
	  hJetScaleDown->SetName("histo_HminusNoH_CMS_scale_jDown");

	  hJetResUp = unRollingHistogram(HminusNoH_res_jUp,errorType);
	  hJetResUp->SetName("histo_HminusNoH_CMS_res_jUp");

	  hJetResDown = (TH1F*) hNominal->Clone("histo_HminusNoH_CMS_res_jDown");

	  hLepScaleUp->Write();
	  hLepScaleDown->Write();
	  hLepResUp->Write();
	  hLepResDown->Write();
	  hJetScaleUp->Write();
	  hJetScaleDown->Write();
	  hJetResUp->Write();
	  hJetResDown->Write();

	}

	for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	  hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	  hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  if(hStatDown->GetBinContent(iBin) <0)
	    hStatDown->SetBinContent(iBin,0);
	}
   
	hStatUp->Write();
	hStatDown->Write();


	// object systematics
	lepScaleShape = lepScaleShape + "  1  ";
	lepResShape   = lepResShape + "  1  ";
	jetScaleShape = jetScaleShape + "  1  ";
	jetResShape   = jetResShape + "  1  ";

        // shape lines
	if(not TString(SampleVector.at(iSample).m_sampleName).Contains("noH")){

	  statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shape  shapeN2 " ;
           
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else      
	      statShape     += " 1    " ;	    
	  }
	  statShape     += "\n" ;
	}

        else if (TString(SampleVector.at(iSample).m_sampleName).Contains("noH") and HminusNoH !=0){
	  statShape     += "CMS_HminusNoH_stat_shape  shapeN2 " ;
	  for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	    if(iSample2!=iSample)
	      statShape     += " -    " ;	    
	    else
	      statShape     += " 1    " ;	    
	  }

	  statShape     += "\n" ;
	}
      }

      datacard << lineBin << endl;
      datacard << lineProcess << endl;
      datacard << lineProcess2 << endl;
      datacard << lineRate << endl;

      datacard<< "------------------------------------------------------"<< endl;

      datacard<< lepScaleShape + "\n";
      datacard<< lepResShape + "\n";
      datacard<< jetScaleShape + "\n";
      datacard<< jetResShape + "\n";
      datacard<< statShape ;
      
      datacard.close();
      outputCard->Close(); 
      
    }
  }
      
  // Make datacards for 1D histograms
  TFile* outputEfficiency = new TFile(("output/"+outputDataCardDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    
  
  return 0 ;
}  
