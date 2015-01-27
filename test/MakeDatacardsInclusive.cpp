//////////////////////////////////////////////////////////////////////////////////////////////////////
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
int    errorType;
string finalStateString;

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
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  errorType           = gConfigParser -> readIntOption("Option::errorType");
  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");

  // output datacard directory
  string outputDataCardDirectory = gConfigParser -> readStringOption("Output::outputDataCardDirectory");
  system(("mkdir -p output/"+outputDataCardDirectory).c_str());
  system(("rm -r output/"+outputDataCardDirectory+"/*").c_str());

  
  ///// Start the analysis --> apply cut and make histos
  plotter analysisPlots (lumi,"output") ; // use the plotter structure to make the histograms for each sample and each cutLayer and each variable
  map<string,TH1F*> histoCutEff ;

  for( map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){ // loop on each sample

    TChain* chain = new TChain (treeName.c_str()) ;  // take the file chain
    int numBefore = 0;

    // take input files
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
   fillHistos (analysisPlots,      // plotter object
	       ReadTree,           // reader in order to loop on events
	       CutList,            // list of cut to apply
	       variableList1D,       // 1D variables
	       itSample->first,    // sample name
	       usePuppiAsDefault,  // use puppi flag
	       minLeptonCutPt,     // lepton pt cut
	       minLeptonCleaningPt, // cleaning cut
	       leptonIsoCut_mu,    // isolation for muons
	       leptonIsoCut_el,    // isolation for electrons
	       leptonIsoCutLoose,  // isolation for loose leptons
	       matchingCone,       // matching cone
	       minJetCutPt,        // min jet pt cut
	       histoCutEff) ; // fill the histogram

   // 1D + 2D analysis
   fill2DHistos (analysisPlots,      // plotter object
		 ReadTree,           // reader in order to loop on events
		 CutList,            // list of cut to apply
		 variableList2D,       // 1D variables
		 itSample->first,    // sample name
		 usePuppiAsDefault,  // use puppi flag
		 minLeptonCutPt,     // lepton pt cut
		 minLeptonCleaningPt, // cleaning cut
		 leptonIsoCut_mu,    // isolation for muons
		 leptonIsoCut_el,    // isolation for electrons
		 leptonIsoCutLoose,  // isolation for loose leptons
		 matchingCone,       // matching cone
		 minJetCutPt) ; // fill the histogram

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

  //####### TH1 sector
  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    // loop on variables 1D
    for(size_t iVar = 0; iVar < variableList1D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root").c_str(),"RECREATE");
      outputCard->cd();

      datacard << "## shape analysis WW scattering" << endl;
      datacard << "imax 1 number of channels"       << endl;
      datacard << "jmax * number of background"     << endl;
      datacard << "kmax * number of nuisance parameters" << endl;
      datacard <<"------------------------------------------------------"<< endl;
      datacard <<"bin  "+CutList.at(iCut).cutLayerName+"_"+finalStateString<< endl;
      			     
      /// loop on samples
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


      observed = (TH1F*)  hTotal->Clone("histo_Data");
      observed->SetBinErrorOption(TH1::kPoisson);
      observed->Reset();
      observed->FillRandom(hTotal,gRandom->Poisson(hTotal->Integral()));            
      observed->Write();
      datacard<< Form("observation %d",int(observed->Integral()))<< endl;
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root    histo_$PROCESS histo_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList1D.at(iVar).variableName+".root    histo_Data" <<endl;
      datacard<< "------------------------------------------------------"<< endl;
      
      int nSignal = 0;
      int nBackground = 0;
      for(size_t iSignal = 0; iSignal < SampleVector.size(); iSignal++){
	if(SampleVector.at(iSignal).m_isSignal)
          nSignal++;
      }

      nSignal = -abs(nSignal);

      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
	lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	lineProcess += SampleVector.at(iSample).m_sampleName+"   ";

        TH1F* temp = SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName];
        
        temp->SetBinContent(1,temp->GetBinContent(1)+temp->GetBinContent(0)); // add underflow to the last bin
        temp->SetBinContent(temp->GetNbinsX(),temp->GetBinContent(temp->GetNbinsX())+temp->GetBinContent(hTotal->GetNbinsX()+1)); // add overflow to the last bin
        if(!temp->GetDefaultSumw2()){
	  temp->SetBinError(1,sqrt(temp->GetBinError(1)*temp->GetBinError(1)+temp->GetBinContent(0))); // add underflow to the last bin
	  temp->SetBinError(temp->GetNbinsX(),sqrt(temp->GetBinError(temp->GetNbinsX())*temp->GetBinError(temp->GetNbinsX())+temp->GetBinContent(temp->GetNbinsX()+1))); // add underflow to the last bin
	}
    
	temp->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());

        if(SampleVector.at(iSample).m_isSignal){
	  lineProcess2 += Form("%d   ",nSignal);          
          nSignal++;
	}
        else {
	  lineProcess2 += Form("%d   ",nBackground);
          nBackground++;
	}
        
        lineRate += Form("%f   ",temp->Integral());  
      }


      datacard << left << lineBin << endl;
      datacard << left << lineProcess << endl;
      datacard << left << lineProcess2 << endl;
      datacard << left << lineRate << endl;
      datacard<< "------------------------------------------------------"<< endl;

      
      // stat shapes ;
      TH1F* hStatUp   = 0;
      TH1F* hStatDown = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
	hStatUp = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeUp").c_str());
	hStatDown = (TH1F*) SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_histos[variableList1D.at(iVar).variableName]->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeDown").c_str());

        for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
          hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
          hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
          if(hStatDown->GetBinContent(iBin) < 0) 
	    hStatDown->SetBinContent(iBin,0);
	}

        // add overflow and underflow
        hStatUp->SetBinContent(hStatUp->GetNbinsX(),hStatUp->GetBinContent(hStatUp->GetNbinsX())+hStatUp->GetBinContent(hStatUp->GetNbinsX()+1));
        hStatUp->SetBinContent(1,hStatUp->GetBinContent(1)+hStatUp->GetBinContent(0));
        if(!hStatUp->GetDefaultSumw2()){
	  hStatUp->SetBinError(hStatUp->GetNbinsX(),sqrt(hStatUp->GetBinError(hStatUp->GetNbinsX())*hStatUp->GetBinError(hStatUp->GetNbinsX())+
							 hStatUp->GetBinContent(hStatUp->GetNbinsX()+1)));
	  hStatUp->SetBinError(1,sqrt(hStatUp->GetBinContent(1)*hStatUp->GetBinError(1)+hStatUp->GetBinContent(0)));
	}

        hStatDown->SetBinContent(hStatDown->GetNbinsX(),hStatDown->GetBinContent(hStatDown->GetNbinsX())+hStatDown->GetBinContent(hStatDown->GetNbinsX()+1));
        hStatDown->SetBinContent(1,hStatDown->GetBinContent(1)+hStatDown->GetBinContent(0));
        if(!hStatDown->GetDefaultSumw2()){
	  hStatDown->SetBinError(hStatDown->GetNbinsX(),sqrt(hStatDown->GetBinError(hStatDown->GetNbinsX())*hStatDown->GetBinError(hStatDown->GetNbinsX())+
							     hStatDown->GetBinContent(hStatDown->GetNbinsX()+1)));

	  hStatDown->SetBinError(1,sqrt(hStatDown->GetBinError(1)*hStatDown->GetBinError(1)+hStatDown->GetBinContent(0)));
	}

	hStatUp->Write();
	hStatDown->Write();

        datacard <<SampleVector.at(iSample).m_sampleName<<"_stat_shape   shapeN2 " ;
        for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	  if(iSample2!=iSample)
	    datacard << " -    " ;
          else       
	    datacard << " 1    " ;
	}
	datacard << "\n";
      }

      datacard<< endl;
      
      datacard.close();
      outputCard->Close(); 
      
    }
  }

  
  //####### TH2 sector
  //loop on Cuts vector
  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    // loop on variables 2D
    for(size_t iVar = 0; iVar < variableList2D.size(); iVar++){

      //create a datacard and a TFile for histograms
      system(("touch output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".txt").c_str());
      ofstream datacard(("output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".txt").c_str(),ofstream::binary);

      TFile* outputCard = new TFile(("output/"+outputDataCardDirectory+"/"+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root").c_str(),"RECREATE");
      outputCard->cd();

      datacard << "## shape analysis WW scattering" << endl;
      datacard << "imax 1 number of channels"       << endl;
      datacard << "jmax * number of background"     << endl;
      datacard << "kmax * number of nuisance parameters" << endl;
      datacard <<"------------------------------------------------------"<< endl;
      datacard <<"bin  "+CutList.at(iCut).cutLayerName+"_"+finalStateString<< endl;
			     
      /// loop on samples
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
      TH1F* htemp = (TH1F*) observed->Clone("htemp");
      observed->Reset();
      observed->FillRandom(htemp,gRandom->Poisson(htemp->Integral()));            
      observed->Write();
      datacard<< Form("observation %d",int(observed->Integral()))<< endl;
      datacard<< "shapes *          * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root    histo_$PROCESS histo_$SYSTEMATIC" <<endl;
      datacard<< "shapes data_obs   * "+CutList.at(iCut).cutLayerName+"_"+variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY+".root    histo_Data" <<endl;
      datacard<< "------------------------------------------------------"<< endl;

      
      int nSignal = 0;
      int nBackground = 0;
      for(size_t iSignal = 0; iSignal < SampleVector.size(); iSignal++){
	if(SampleVector.at(iSignal).m_isSignal)
          nSignal++;
      }

      nSignal = -abs(nSignal);

      string lineBin     = "bin       ";
      string lineProcess = "process   ";
      string lineProcess2= "process   ";
      string lineRate    = "rate      ";

      // stat shapes ;
      TH1F* hStatUp   = 0;
      TH1F* hStatDown = 0;

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
	lineBin     += CutList.at(iCut).cutLayerName+"_"+finalStateString+"   ";
	lineProcess += SampleVector.at(iSample).m_sampleName+"    ";

        TH1F* htemp = unRollingHistogram(SampleVector.at(iSample).m_sampleContent[CutList.at(iCut).cutLayerName].m_2Dhistos[variableList2D.at(iVar).variableNameX+"_"+variableList2D.at(iVar).variableNameY],errorType);
        htemp->SetName(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());
        htemp->Write(("histo_"+SampleVector.at(iSample).m_sampleName).c_str());

        if(SampleVector.at(iSample).m_isSignal){
	  lineProcess2 += Form("%d   ",nSignal);
          nSignal++;
	}
        else {
	  lineProcess2 += Form("%d   ",nBackground);
          nBackground++;
	}
        
        lineRate += Form("%f   ",htemp->Integral());  
	
	hStatUp   = (TH1F*) htemp->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeUp").c_str());
	hStatDown = (TH1F*) htemp->Clone(("histo_"+SampleVector.at(iSample).m_sampleName+"_stat_shapeDown").c_str());

	for (int iBin = 0; iBin < hStatUp->GetNbinsX()+1; iBin++){
	  hStatUp->SetBinContent(iBin,hStatUp->GetBinContent(iBin)+hStatUp->GetBinError(iBin));
	  hStatDown->SetBinContent(iBin,hStatDown->GetBinContent(iBin)-hStatDown->GetBinError(iBin));
	  if(hStatDown->GetBinContent(iBin) <0)
	    hStatDown->SetBinContent(iBin,0);
	}
   

	hStatUp->Write();
	hStatDown->Write();

      }

      datacard << lineBin << endl;
      datacard << lineProcess << endl;
      datacard << lineProcess2 << endl;
      datacard << lineRate << endl;

       

      for(size_t iSample = 0; iSample < SampleVector.size(); iSample++){
	datacard <<SampleVector.at(iSample).m_sampleName<<"_stat_shape   shapeN2 " ;
	for(size_t iSample2 = 0; iSample2 < SampleVector.size(); iSample2++){
	  if(iSample2!=iSample)
	    datacard << " -    " ;
	  else       
	    datacard << " 1    " ;
	}
	datacard << "\n";
      }

      datacard<< endl;
    
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

