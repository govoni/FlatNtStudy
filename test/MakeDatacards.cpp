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
double matchingCone ;
double minLeptonCleaningPt;
double minLeptonCutPt;
double minJetCutPt;
double leptonIsoCut_mu;
double leptonIsoCut_el;
double leptonIsoCutLoose;

bool   usePuppiAsDefault;
bool   useObjectSystematics;

int    errorType;
string finalStateString;
string scenarioString ;

void addOverAndUnderFlow (TH1F* histo);

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
  double lumi  =  gConfigParser -> readDoubleOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  matchingCone        = gConfigParser -> readDoubleOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readDoubleOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readDoubleOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readDoubleOption("Option::minJetCutPt");
  leptonIsoCut_mu     = gConfigParser -> readDoubleOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readDoubleOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readDoubleOption("Option::leptonIsoCutLoose");

  usePuppiAsDefault    = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  useObjectSystematics = gConfigParser -> readBoolOption("Option::useObjectSystematics");

  errorType           = gConfigParser -> readIntOption("Option::errorType");

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");
  scenarioString      = gConfigParser -> readStringOption("Option::scenarioString");

  // output datacard directory
  string outputDataCardDirectory = gConfigParser -> readStringOption("Output::outputDataCardDirectory");
  system(("mkdir -p output/"+outputDataCardDirectory).c_str());
  system(("rm -r output/"+outputDataCardDirectory+"/*").c_str());

  system(("mkdir -p output/"+outputDataCardDirectory+"/Card1D/").c_str());
  system(("mkdir -p output/"+outputDataCardDirectory+"/Card2D/").c_str());

  
  ///// Start the analysis --> apply cut and make histos
  plotter analysisPlots (lumi,"output",useObjectSystematics) ; // use the plotter structure to make the histograms for each sample and each cutLayer and each variable
  map<string,TH1F*> histoCutEff ;

  for( map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){ // loop on each sample

    TChain* chain = new TChain (treeName.c_str()) ;  // take the file chain
    int numBefore = 0;

    // take input files
    for(size_t iContainer = 0; iContainer < itSample->second.size(); iContainer++){     
      numBefore += itSample->second.at(iContainer).numBefore; 
      chain->Add ((InputBaseDirectory+"/"+itSample->second.at(iContainer).sampleName+"/*_1.root").c_str()) ;
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

  //####### TH1 sector /////

  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){

    // loop on variables 1D
    for(size_t iVar = 0; iVar < variableList1D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/Card1D/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root").c_str(),"RECREATE");
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

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
        if(iSample == 0) 
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

      datacard<< Form("observation %d",int(observed->Integral()))<< endl;
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root    histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root    histo_Data" <<endl;
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

	lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	lineProcess += SampleVector.at(iSample).m_sampleName+"   ";

        hNominal = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];

        addOverAndUnderFlow(hNominal);

	hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());

        if(SampleVector.at(iSample).m_isSignal){
	  lineProcess2 += Form("%d   ",nSignal);          
          nSignal--;
	}
        else {
	  lineProcess2 += Form("%d   ",nBackground);
          nBackground++;
	}
        
        lineRate += Form("%f   ",hNominal->Integral());  
      }


      datacard << left << lineBin << endl;
      datacard << left << lineProcess << endl;
      datacard << left << lineProcess2 << endl;
      datacard << left << lineRate << endl;
      datacard<< "------------------------------------------------------"<< endl;


      // fill nuisances due to objects:
      TH1F* hLepScaleUp   = 0;
      TH1F* hLepScaleDown = 0;
      TH1F* hLepResUp     = 0;
      TH1F* hLepResDown   = 0;
      TH1F* hJetScaleUp   = 0;
      TH1F* hJetScaleDown = 0;
      TH1F* hJetResUp     = 0;
      TH1F* hJetResDown   = 0;

      string lepScale = "CMS_scale_l   lnN    ";
      string lepRes   = "CMS_res_l     lnN    ";
      string jetScale = "CMS_scale_j   lnN    ";
      string jetRes   = "CMS_res_j     lnN    ";

      string lepScaleShape = "";
      string lepResShape   = "";
      string jetScaleShape = "";
      string jetResShape   = "";


      // stat shapes ;
      TH1F* hStatUp   = 0;
      TH1F* hStatDown = 0;

      string statShape = "";


      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

        hNominal = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];

	hLepScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scaleUp").c_str());
	hLepScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scaleDown").c_str());
	hLepResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_lepRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_resUp").c_str());
	hLepResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_resDown").c_str());

	hJetScaleUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleUp[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scaleUp").c_str());
	hJetScaleDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetScaleDown[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scaleDown").c_str());
	hJetResUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos_jetRes[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_resUp").c_str());
	hJetResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_resDown").c_str());

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

 
        // add overflow and underflow
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

     
        // norm nuisances
        lepScale += Form("%0.4f   ",1.+(fabs(hLepScaleUp->Integral()-hNominal->Integral())+fabs(hLepScaleDown->Integral()-hNominal->Integral()))/hNominal->Integral());
        lepRes   += Form("%0.4f   ",1.+2*fabs(hLepResUp->Integral()-hNominal->Integral())/hNominal->Integral());

        jetScale += Form("%0.4f   ",1.+(fabs(hJetScaleUp->Integral()-hNominal->Integral())+fabs(hJetScaleDown->Integral()-hNominal->Integral()))/hNominal->Integral());
        jetRes   += Form("%0.4f   ",1.+2*fabs(hJetResUp->Integral()-hNominal->Integral())/hNominal->Integral());

        // shape lines
        lepScaleShape += "CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scale    shapeN2 ";
        lepResShape   += "CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_res      shapeN2 ";
        jetScaleShape += "CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scale    shapeN2 ";
        jetResShape   += "CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_res      shapeN2 ";
	statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shape  shapeN2 " ;

           
	for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	  if(iSample2!=iSample){
	    lepScaleShape += " -    " ;
	    lepResShape   += " -    " ;
	    jetScaleShape += " -    " ;
	    jetResShape   += " -    " ;
            statShape     += " -    " ;
	  }
	  else{       
	    lepScaleShape += " 1    " ;
	    lepResShape   += " 1    " ;
	    jetScaleShape += " 1    " ;
	    jetResShape   += " 1    " ;
            statShape     += " 1    " ;
	  }
	}

	lepScaleShape += "\n" ;
	lepResShape   += "\n" ;
	jetScaleShape += "\n" ;
	jetResShape   += "\n" ;
	statShape     += "\n" ;
      }

      //      datacard<< lepScale << endl;
      //      datacard<< lepRes << endl;
      //      datacard<< jetScale << endl;
      //      datacard<< jetRes << endl;

      datacard<< lepScaleShape ;
      datacard<< lepResShape ;
      datacard<< jetScaleShape ;
      datacard<< jetResShape ;
      datacard<< statShape ;
      
      datacard.close();
      outputCard->Close(); 
    }
  }
    
  //####### TH2 sector ////////

  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){

    // loop on variables 2D
    for(size_t iVar = 0; iVar < variableList2D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/Card2D/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root").c_str(),"RECREATE");
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
      
      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
        if(iSample == 0) 
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
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root    histo_$PROCESS histo_$PROCESS_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root    histo_Data" <<endl;
      datacard<< "------------------------------------------------------"<< endl;

            
      int nSignal = 0;
      int nBackground = 1;

      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";


      // fill nuisances due to objects:
      TH1F* hLepScaleUp   = 0;
      TH1F* hLepScaleDown = 0;
      TH1F* hLepResUp     = 0;
      TH1F* hLepResDown   = 0;
      TH1F* hJetScaleUp   = 0;
      TH1F* hJetScaleDown = 0;
      TH1F* hJetResUp     = 0;
      TH1F* hJetResDown   = 0;

      string lepScale = "CMS_scale_l   lnN    ";
      string lepRes   = "CMS_res_l     lnN    ";
      string jetScale = "CMS_scale_j   lnN    ";
      string jetRes   = "CMS_res_j     lnN    ";

      string lepScaleShape = "";
      string lepResShape   = "";
      string jetScaleShape = "";
      string jetResShape   = "";


      // stat shapes ;
      TH1F* hStatUp   = 0;
      TH1F* hStatDown = 0;

      string statShape = "";

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){

	lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	lineProcess += SampleVector.at(iSample).m_sampleName+"    ";

        ///un-rool the TH2 in a TH1
        
        hNominal = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);

        hNominal->SetName(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        hNominal->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());

        if(SampleVector.at(iSample).m_isSignal){
	  lineProcess2 += Form("%d   ",nSignal);
          nSignal--;
	}
        else {
	  lineProcess2 += Form("%d   ",nBackground);
          nBackground++;
	}
        
        lineRate += Form("%f   ",hNominal->Integral());  

        // make the sta histograms	
	hStatUp   = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeUp").c_str());
	hStatDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeDown").c_str());

	for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	  hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	  hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  if(hStatDown->GetBinContent(iBin) <0)
	    hStatDown->SetBinContent(iBin,0);
	}
   
	hStatUp->Write();
	hStatDown->Write();

        // make systematic variations
	hLepScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	hLepScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	hLepResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_lepRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
        hLepScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scaleUp").c_str());
        hLepScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scaleDown").c_str());
        hLepResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_resUp").c_str());
        
        hLepResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_resDown").c_str());

	hJetScaleUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleUp[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	hJetScaleDown = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetScaleDown[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
	hJetResUp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos_jetRes[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
        hJetScaleUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scaleUp").c_str());
        hJetScaleDown->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scaleDown").c_str());
        hJetResUp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_resUp").c_str());
        
        hJetResDown = (TH1F*) hNominal->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_resDown").c_str());


	hLepScaleUp   ->Write();
	hLepScaleDown ->Write();
	hLepResUp     ->Write();
	hLepResDown   ->Write();
	hJetScaleUp   ->Write();
	hJetScaleDown ->Write();
	hJetResUp     ->Write();
	hJetResDown   ->Write();

	// norm nuisances                                                                                                                                                       
        lepScale += Form("%0.4f   ",1.+(fabs(hLepScaleUp->Integral()-hNominal->Integral())+fabs(hLepScaleDown->Integral()-hNominal->Integral()))/hNominal->Integral());
        lepRes   += Form("%0.4f   ",1.+2*fabs(hLepResUp->Integral()-hNominal->Integral())/hNominal->Integral());

        jetScale += Form("%0.4f   ",1.+(fabs(hJetScaleUp->Integral()-hNominal->Integral())+fabs(hJetScaleDown->Integral()-hNominal->Integral()))/hNominal->Integral());
        jetRes   += Form("%0.4f   ",1.+2*fabs(hJetResUp->Integral()-hNominal->Integral())/hNominal->Integral());

        // shape lines                                                                                                                                                          
        lepScaleShape += "CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_scale    shapeN2 ";
        lepResShape   += "CMS_"+SampleVector.at(iSample).m_sampleName+"_lep_res      shapeN2 ";
        jetScaleShape += "CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_scale    shapeN2 ";
        jetResShape   += "CMS_"+SampleVector.at(iSample).m_sampleName+"_jet_res      shapeN2 ";
        statShape     += "CMS_"+SampleVector.at(iSample).m_sampleName+"_stat_shape  shapeN2 " ;

	for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
          if(iSample2!=iSample){
            lepScaleShape += " -    " ;
            lepResShape   += " -    " ;
            jetScaleShape += " -    " ;
            jetResShape   += " -    " ;
            statShape     += " -    " ;
          }
          else{
            lepScaleShape += " 1    " ;
            lepResShape   += " 1    " ;
            jetScaleShape += " 1    " ;
            jetResShape   += " 1    " ;
            statShape     += " 1    " ;
          }
        }

        lepScaleShape += "\n" ;
        lepResShape   += "\n" ;
        jetScaleShape += "\n" ;
        jetResShape   += "\n" ;
        statShape     += "\n" ;
      }

      datacard << lineBin << endl;
      datacard << lineProcess << endl;
      datacard << lineProcess2 << endl;
      datacard << lineRate << endl;

      //      datacard<< lepScale << endl;
      //      datacard<< lepRes << endl;
      //      datacard<< jetScale << endl;
      //      datacard<< jetRes << endl;

      datacard<< lepScaleShape ;
      datacard<< lepResShape ;
      datacard<< jetScaleShape ;
      datacard<< jetResShape ;
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
