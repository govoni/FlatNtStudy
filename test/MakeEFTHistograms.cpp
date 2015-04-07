#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TPad.h"
#include "TF2.h"

#include "ConfigParser.h"
#include "readTreeEFT.h"
#include "utilsEFT.h"

using namespace std ;

float matchingCone ;
float minLeptonCleaningPt;
float minLeptonCutPt;
float minJetCutPt;
float leptonIsoCut_mu;
float leptonIsoCut_el;
float leptonIsoCutLoose;
bool   usePuppiAsDefault;

string  finalStateString;

#define nGridPoint 657

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
int main (int argc, char ** argv) {

  // check number of inpt parameters
  if(argc < 2){
    cerr<<"Forgot to parse the cfg file --> exit "<<endl;
    return -1;
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

  // import base directory where samples are located and txt file with the directory name + other info                                                                          
  string InputBaseDirectory           = gConfigParser -> readStringOption("Input::InputBaseDirectory");
  string InputSampleList              = gConfigParser -> readStringOption("Input::InputSampleList");

  unordered_map<string, vector<sampleContainer> > sampleMap ;
  if(ReadInputSampleFile(InputSampleList,sampleMap) <= 0){
    cerr<<" Empty Input Sample File or not Exisisting --> Exit "<<endl; return -1;}
  // treeName
  string treeName      = gConfigParser -> readStringOption("Input::TreeName");

  // take the cut list
  string InputCutList  = gConfigParser -> readStringOption("Input::InputCutList");

  // Read the cut file
  vector <cutContainer> CutList;
  if(ReadInputCutFile(InputCutList,CutList) <= 0){
    cerr<<" Empty Cut List File or not Exisisting --> Exit "<<endl; return -1;}

  // take the variable list to be plotted
  string InputVariableList  = gConfigParser -> readStringOption("Input::InputVariableList");
  vector<variableContainerDynamic> variableList;

  if(ReadInputVariableFileDynamicBinning(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters
  float lumi  =  gConfigParser -> readFloatOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  finalStateString    = gConfigParser -> readStringOption("Option::finalStateString");

  matchingCone        = gConfigParser -> readFloatOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readFloatOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readFloatOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readFloatOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readFloatOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readFloatOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readFloatOption("Option::leptonIsoCutLoose");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());


  ///// Start the analysis  
  map<string,TH1F*> histoCutEff ;
  vector<histoContainerEFT> plotVector;
  vector<readTreeEFT*> readTree;
  vector<float> weight;

  for( unordered_map<string,vector<sampleContainer> >::iterator itSample = sampleMap.begin() ; itSample != sampleMap.end(); itSample++){ // loop on each sample               

    weight.clear();
    readTree.clear();

    int iSub = 0;

    for(vector<sampleContainer>::iterator itSubSample = itSample->second.begin(); itSubSample != itSample->second.end(); itSubSample++){

      TChain* chain = new TChain (treeName.c_str()) ;  // take the file chain                                                                                                 
      int numBefore = 0;

      numBefore += itSubSample->numBefore;
      chain->Add ((InputBaseDirectory+"/"+itSubSample->sampleName+"/*.root").c_str()) ;
      

      int totEvent = chain->GetEntries();
      readTree.push_back( new readTreeEFT((TTree*)(chain))); // reader for chain trees                                                                                        
      cout<<"Sample name : "<<itSample->first<<" directory files "<<itSubSample->sampleName<<" Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<itSubSample->xsec<<" Nevents before selections "<<lumi*itSubSample->xsec<<" weight "<<lumi*itSubSample->xsec/totEvent<<endl;

      weight.push_back(lumi*itSubSample->xsec/totEvent);

      for(size_t iCut = 0; iCut < CutList.size(); iCut++){
	histoCutEff[itSample->first+"_pos_"+to_string(iSub)+"_"+CutList.at(iCut).cutLayerName] = new TH1F((itSample->first+"_pos_"+to_string(iSub)+"_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
	for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	  plotVector.push_back(histoContainerEFT(CutList.at(iCut).cutLayerName,variableList.at(iVar),nGridPoint));
	}
      }
      
      iSub++;
    }

    for(size_t iRead = 0; iRead < readTree.size(); iRead++){
      cout<<"analyzing for sample "<<itSample->first<<" chain number "<<iRead<<endl;

      int passingLHEFilter = 0 ;
      
      int maximumEvents = readTree.at(iRead)->fChain->GetEntries () ;
      
      // Loop on the events
      for(int iEvent = 0; iEvent < maximumEvents ; iEvent++){
	
	readTree.at(iRead)->fChain->GetEntry(iEvent) ;

	if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;
	readTreeEFT* reader = readTree.at(iRead);
	
	// filter LHE level leptons
	if( finalStateString != "All" and finalStateString != "all"){
	  if(TString(finalStateString).Contains("UU")){
	    if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
	      continue;
	  }
	  else if(TString(finalStateString).Contains("EE")){
	    if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) continue;
	    }
	  else if(TString(finalStateString).Contains("EU")){
	    if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) !=13) continue ;
	  }
	  else if(TString(finalStateString).Contains("UE")){
	      if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) !=11) continue ;
	  }    
	  else{
	    cerr<<"problem with lhe level filter definition --> skip event"<<endl;
	    continue;
	  }
	  
	  
	  passingLHEFilter++;
	  
	  // if an event pass the cut, fill the associated map                                                                                                               
	  TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2 ;
	  
	  L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
	  L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
	  
	  L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
	  L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);
	  
	  if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;
	  
	  if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;
	}
	      
	// Loop  on the cut list --> one cut for each polarization
	for(size_t iCut = 0; iCut < CutList.size(); iCut++){
	  
	  // cut the events
	  if(!passCutContainerSelection(reader,
					CutList.at(iCut),
					itSample->first,
					int(iRead),
					usePuppiAsDefault,
					minLeptonCutPt,
					minLeptonCleaningPt,
					leptonIsoCut_mu,
					leptonIsoCut_el,
					leptonIsoCutLoose,
					matchingCone,
					minJetCutPt,
					histoCutEff,
					finalStateString)) continue;
	  
	  // if an event pass the cut, fill the associated map
	  TLorentzVector L_dilepton;

	  // dump all the lepton in the event                                                                                                                                  
	  vector<leptonContainer> LeptonsAll;
	  fillRecoLeptonsArray (LeptonsAll, *reader);

	  // dump tight leptons                                                                                                                                                 
	  vector<leptonContainer> leptonsIsoTight ;
	  leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);
	  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;

	  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
	    histoContainerEFT tmpPlot;
	    tmpPlot.cutName = CutList.at(iCut).cutLayerName;
	    tmpPlot.varName = variableList.at(iVar).variableName;
	    vector<histoContainerEFT>::iterator itVec ;
	    itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
	    if(itVec == plotVector.end()){
	      cerr<<"Problem -->plot not found for "<<CutList.at(iCut).cutLayerName<<"  "<<variableList.at(iVar).variableName<<endl;
	      continue ;
	    }


	    if(variableList.at(iVar).variableName == "mll" and leptonsIsoTight.size() >= 2){
	      fillEFTWeights(itVec->histogramEFT,(leptonsIsoTight.at(0).lepton4V_+leptonsIsoTight.at(1).lepton4V_).M(),weight.at(iRead),reader);
	    }
	    if(variableList.at(iVar).variableName == "ptl1" and leptonsIsoTight.size() >= 2){
	      fillEFTWeights(itVec->histogramEFT,leptonsIsoTight.at(0).lepton4V_.Pt(),weight.at(iRead),reader);
	    }
	    if(variableList.at(iVar).variableName == "ptl2" and leptonsIsoTight.size() >= 2){
	      fillEFTWeights(itVec->histogramEFT,leptonsIsoTight.at(1).lepton4V_.Pt(),weight.at(iRead),reader);
	    }	      
	  }
	}	
      }
    }
  }
  
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    


  TFile* eftHistograms = new TFile(("output/"+outputPlotDirectory+"/histogramsEFT.root").c_str(),"RECREATE");
  eftHistograms->cd();

  for(size_t iHisto = 0; iHisto < plotVector.size(); iHisto++){
    for(size_t iEFT = 0; iEFT < plotVector.at(iHisto).histogramEFT.size(); iEFT++){
      plotVector.at(iHisto).histogramEFT.at(iEFT)->Write();
    }
  }

  eftHistograms->Close();
  
  
  // Read operator values
  vector<vector<Float_t>> opValVec(9); //1, opVal2, opVal3, opVal4, opVal9, opVal10, opVal11, opVal12, opVal13;
  const int Noperators = 9;
  Float_t opVal[Noperators] = {0,0,0,0,0,0,0,0,0};
  int opNum[Noperators] = {1,2,3,4,9,10,11,12,13};
  TString opName[Noperators+1] = {"S0","S1","M0","M1","M6","M7","T0","T1","T2","S0_LS1"};
  
  TFile* file = new TFile((InputBaseDirectory + "/" + sampleMap.begin()->second.begin()->sampleName + "/outDumper_0.root").c_str(), "READ");
  TTree* tree = (TTree*) file->Get("weightsInfo");
  
  for( int iOp = 0; iOp < Noperators; ++iOp ) {           
    TString opStr = "operatorVal"; opStr += opNum[iOp];
    tree->SetBranchAddress(opStr, &(opVal[iOp]));
  }
    
  int nevents = tree->GetEntriesFast();
  for( int iEvent=0; iEvent < nevents; ++iEvent ) {
    tree->GetEntry(iEvent);
    for( int iOp = 0; iOp < Noperators; ++iOp ) {
      opValVec[iOp].push_back(opVal[iOp]);
    }
  } 
  
  // Fit TF1 for EFT framework
  vector<double> x,y,z; // size depends on operator
  for(size_t iHisto = 0; iHisto < plotVector.size(); iHisto++){
    histoContainerEFT histoCont = plotVector.at(iHisto);
    // Set SM hist
    TH1F* hSM = histoCont.histogramEFT.at(1);

    // Loop over operators
    for( int iOp = 0; iOp < (Noperators+1); ++iOp ) {
        TFile* eftFunctionFile = new TFile(("output/"+outputPlotDirectory+"/signal_WWVBS_mll_L"+opName[iOp]+".root").Data(),"RECREATE");
        eftFunctionFile->cd();
        
        // Loop over bins ( every bin is fitted as a function of the coupling parameters )
        for( int iBin = 0; iBin < variableList[0].Nbin+1; ++iBin ) { //Nbin+1: include overflow
            
            // 1D scans
            if( iOp != Noperators ) {
                x.clear();
                y.clear();
            
                // Loop over anomalous coupling grid
                for(size_t iEFT = 0; iEFT < histoCont.histogramEFT.size(); iEFT++){
                    TH1F* histoEFT = histoCont.histogramEFT.at(iEFT);
                    if( opValVec[iOp][iEFT] != 0 ) {
                        // Skip 2D scan
                        if( ( iOp == 0 && opValVec[1][iEFT] != 0 ) || ( iOp == 1 && opValVec[0][iEFT] != 0 ) ) continue; 
                        
                        x.push_back( opValVec[iOp][iEFT]*1e12 ); // factor 1e12 for fit convergence
                        y.push_back( histoEFT->GetBinContent(iBin+1)/hSM->GetBinContent(iBin+1) );
                    }
                }
                x.push_back(0);
                y.push_back(1);
            
                TCanvas *c = new TCanvas(TString::Format("c_opt_%s_bin_%d",opName[iOp].Data(),iBin),"");
                TGraph *graph = new TGraph(x.size(), &x[0], &y[0]);
                graph->SetMarkerStyle(20);
                graph->GetYaxis()->SetTitle(Form("m_{ll}/m_{ll}^{SM} bin %d",iBin));
                graph->GetYaxis()->SetTitleOffset(1.3);
                graph->GetXaxis()->SetTitle(Form("%s operator (x 10^{-12})",opName[iOp].Data()));
                graph->Draw("AP");
                TF1* func = new TF1(TString::Format("bin_function_%d",iBin),"pol2",-1e3,1e3);
                func->SetLineWidth(2);
                graph->Fit(func,"QRME");
                c->Write();
                func->Write();
                c->SaveAs(("output/"+outputPlotDirectory+"/operator_"+string(opName[iOp])+"_bin_"+to_string(iBin)+".png").c_str(),"png");
            }
            // 2D grid
            else {
                x.clear();
                y.clear();
                z.clear();
                // Loop over anomalous coupling grid
                for(size_t iEFT = 0; iEFT < histoCont.histogramEFT.size(); iEFT++){
                    TH1F* histoEFT = histoCont.histogramEFT.at(iEFT);
                    if( opValVec[0][iEFT] != 0 || opValVec[1][iEFT] != 0 ) {                    
                        x.push_back( opValVec[0][iEFT]*1e12 ); // factor 1e12 for fit convergence
                        y.push_back( opValVec[1][iEFT]*1e12 );
                        z.push_back( histoEFT->GetBinContent(iBin+1)/hSM->GetBinContent(iBin+1) );
                    }
                }
                x.push_back(0);
                y.push_back(0);
                z.push_back(1);
                
                TCanvas *c = new TCanvas(TString::Format("c_opt_%s_bin_%d",opName[iOp].Data(),iBin),"");
                TGraph2D *graph = new TGraph2D(x.size(), &x[0], &y[0], &z[0]);
                graph->SetMarkerStyle(20);
                graph->GetZaxis()->SetTitle(Form("m_{ll}/m_{ll}^{SM} bin %d",iBin));
                graph->GetZaxis()->SetTitleOffset(1.5);
                graph->GetXaxis()->SetTitle("S0 operator (x 10^{-12})");
                graph->GetXaxis()->SetTitleOffset(1.5);
                graph->GetYaxis()->SetTitle("S1 operator (x 10^{-12})");
                graph->GetYaxis()->SetTitleOffset(1.5);
                graph->Draw("p0");
                TF2* func = new TF2(TString::Format("bin_function_%d",iBin),"[0]+[1]*x+[2]*y+[3]*x*y+[4]*x*x+[5]*y*y", -70, 70, -150, 150);
                func->SetParameter(0,1);
                graph->Fit(func,"QRME");
                func->Draw("surf1same");
                c->Write();
                func->Write();
                c->SaveAs(("output/"+outputPlotDirectory+"/operator_"+string(opName[iOp])+"_bin_"+to_string(iBin)+".png").c_str(),"png");
            }
        }// End loop over bins
        
//         eftFunctionFile->Close();
    }// End loop over operators
  }
  
  return 0 ;
}
