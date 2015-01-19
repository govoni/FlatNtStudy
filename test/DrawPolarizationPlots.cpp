/////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TLatex.h"

#include "plotter.h"
#include "ConfigParser.h"
#include "readTree.h"
#include "utils.h"

using namespace std ;

double matchingCone ;
double minLeptonCleaningPt;
double minLeptonCutPt;
double minJetCutPt;
double leptonIsoCut_mu;
double leptonIsoCut_el;
double leptonIsoCutLoose;
bool   usePuppiAsDefault;


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
  string InputBaseDirectory  = gConfigParser -> readStringOption("Input::InputBaseDirectory");

  // import from cfg file the cross section value for this sample
  float CrossSection   = gConfigParser -> readFloatOption("Input::CrossSection");

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
  vector<variableContainer> variableList;

  if(ReadInputVariableFile(InputVariableList,variableList) <= 0 ){
    cerr<<" Empty Variable List File or not Exisisting --> Exit "<<endl; return -1;}

  // take lumi and other parameters
  double lumi  =  gConfigParser -> readDoubleOption("Option::Lumi"); // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)

  matchingCone        = gConfigParser -> readDoubleOption("Option::matchingCone"); 
  minLeptonCleaningPt = gConfigParser -> readDoubleOption("Option::minLeptonCleaningPt"); 
  minLeptonCutPt      = gConfigParser -> readDoubleOption("Option::minLeptonCutPt");
  minJetCutPt         = gConfigParser -> readDoubleOption("Option::minJetCutPt");
  usePuppiAsDefault   = gConfigParser -> readBoolOption("Option::usePuppiAsDefault");
  leptonIsoCut_mu     = gConfigParser -> readDoubleOption("Option::leptonIsoCutMu");
  leptonIsoCut_el     = gConfigParser -> readDoubleOption("Option::leptonIsoCutEl");
  leptonIsoCutLoose   = gConfigParser -> readDoubleOption("Option::leptonIsoCutLoose");

  // output directory
  string outputPlotDirectory = gConfigParser -> readStringOption("Output::outputPlotDirectory");
  system(("mkdir -p output/"+outputPlotDirectory).c_str());
  system(("rm -r output/"+outputPlotDirectory+"/*").c_str());


  ///// Start the analysis  
  map<string,TH1F*> histoCutEff ;

  TChain* chain = new TChain (treeName.c_str()) ;  
  chain->Add ((InputBaseDirectory+"/*.root").c_str()) ;
  int totEvent = chain->GetEntries();

  readTree* reader  = new readTree((TTree*)(chain));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<endl;

  
  vector<int> eventPassingSelection;
  eventPassingSelection.assign(CutList.size(),0);

  // make the plot container 
  vector<histoContainer> plotVector;

  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEff["WW_EWK_"+CutList.at(iCut).cutLayerName] = new TH1F(("WW_EWK_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVector.push_back(histoContainer(CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  
  // Loop on the events
  for(int iEvent = 0; iEvent < chain->GetEntries()/10; iEvent++){

    reader->fChain->GetEntry(iEvent) ;

    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;

    // skip event with less than two leptons by default                                                                                                                        
    if(reader->pt1 < 0          or reader->pt2 < 0)          continue ; // skip the event --> only two reco leptons are good                                                    
    if(reader->jetpt1 < 0       or reader->jetpt2 < 0)       continue ; // skip the event with less than two reco jet                                                         
    if(reader->jetpt_puppi1 < 0 or reader->jetpt_puppi2 < 0) continue ; // skip the event with less than two reco jet                                                         

    // Loop  on the cut list --> one cut for each polarization
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){
      if(!passCutContainerSelection(reader,
                                    CutList.at(iCut),
                                    "WW_EWK",
                                    usePuppiAsDefault,
                                    minLeptonCutPt,
                                    minLeptonCleaningPt,
                                    leptonIsoCut_mu,
                                    leptonIsoCut_el,
                                    leptonIsoCutLoose,
                                    matchingCone,
                                    minJetCutPt,
                                    histoCutEff)) continue;
      
      eventPassingSelection.at(iCut) += 1;
      
      // if an event pass the cut, fill the associated map
      TLorentzVector L_dilepton, L_met, L_puppi_met;
      TLorentzVector L_dijet, L_LLmet;

      // dump all the lepton in the event                                                                                                                                       
      vector<leptonContainer> LeptonsAll;
      fillRecoLeptonsArray (LeptonsAll, *reader);

      // dump tight leptons                                                                                                                                                     
      vector<leptonContainer> leptonsIsoTight ;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minLeptonCutPt);

      L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
      L_met.SetPtEtaPhiM       (reader->pfmet,0.,reader->pfmetphi, 0.) ;
      L_LLmet = L_dilepton + L_met ;
      L_puppi_met.SetPtEtaPhiM (reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;


      float asimJ = 0, asimL = 0, Rvar = 0;

      // take reco jets                                                                                                                                                         
      vector<jetContainer> RecoJetsAll ;
      if(not usePuppiAsDefault)
	fillRecoJetArray (RecoJetsAll, *reader) ;
      else 
	fillPuppiJetArray (RecoJetsAll, *reader) ;

      // take jets                                                                                                                                                         
      vector<jetContainer> RecoJets;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, CutList.at(iCut).bTagVeto, CutList.at(iCut).jetPUID, minLeptonCleaningPt, matchingCone);

      asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt()) ;

      if(RecoJets.size() >= 2){
        L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
        asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
        Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
      }

      
      for(size_t iVar = 0; iVar < variableList.size(); iVar++){
        histoContainer tmpPlot;
        tmpPlot.cutName = CutList.at(iCut).cutLayerName;
        tmpPlot.varName = variableList.at(iVar).variableName;
        vector<histoContainer>::iterator itVec ;
	itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
        if(itVec == plotVector.end()){
          cerr<<"Problem -->plot not found for "<<CutList.at(iCut).cutLayerName<<"  "<<variableList.at(iVar).variableName<<endl;
          continue ;
	}

	if(variableList.at(iVar).variableName == "ptj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "etaj1" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "detajj" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()),1) ;
        }
	else if(variableList.at(iVar).variableName == "dRjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.DeltaR(RecoJets.at(1).jet4V_),1) ;
        }
	else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "mjj" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.M(),1) ;
        }
	else if(variableList.at(iVar).variableName == "Asim_j" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimJ,1) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_),1) ;
        }
	else if(variableList.at(iVar).variableName == "etaj1etaj2" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptl1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptl2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Pt(),1) ;
        }
 
	else if(variableList.at(iVar).variableName == "etal1" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "etall" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Eta(),1) ;
        }
	else if(variableList.at(iVar).variableName == "phill" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.Phi(),1) ;
        }
	else if(variableList.at(iVar).variableName == "mll" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.M(),1) ;
        }

	else if(variableList.at(iVar).variableName == "dRll" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(leptonsIsoTight.at(1).lepton4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "etal1etal2" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaEta_LL" and RecoJets.size() >=2){
          itVec->histogram->Fill(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta()),1) ;
	}
	else if(variableList.at(iVar).variableName == "Asim_l" and RecoJets.size() >=2){
          itVec->histogram->Fill(asimL,1) ;
        }

	else if(variableList.at(iVar).variableName == "met" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_met.Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "R" and RecoJets.size() >=2){
          itVec->histogram->Fill(Rvar,1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaPhi(L_met),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_ + L_met).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptTLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_ + L_met).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptLLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),1) ;
        }

	///
	else if(variableList.at(iVar).variableName == "DeltaPhi_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_dijet),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_LJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_TJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_JJL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(0).lepton4V_.DeltaR(L_dijet),1) ;
        }


	else if(variableList.at(iVar).variableName == "ptLJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(0).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptTJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+RecoJets.at(1).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptJJL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(0).lepton4V_+L_dijet).Pt(),1) ;
        }


	else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_dijet),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_LJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_TJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_JJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill(leptonsIsoTight.at(1).lepton4V_.DeltaR(L_dijet),1) ;
        }


	else if(variableList.at(iVar).variableName == "ptLJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(0).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptTJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+RecoJets.at(1).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptJJTL" and RecoJets.size() >=2){
          itVec->histogram->Fill((leptonsIsoTight.at(1).lepton4V_+L_dijet).Pt(),1) ;
        }

 
	else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaPhi(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaPhi(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaPhi(L_dijet),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_LJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(0).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_TJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(RecoJets.at(1).jet4V_),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaR_JJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dilepton.DeltaR(L_dijet),1) ;
        }


	else if(variableList.at(iVar).variableName == "ptLJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(0).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptTJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+RecoJets.at(1).jet4V_).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "ptJJLL" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),1) ;
        }

	///

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.DeltaPhi(L_met),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptJJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(0).jet4V_.DeltaPhi(L_met),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptLJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(0).jet4V_+L_met).Pt(),1) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(RecoJets.at(1).jet4V_.DeltaPhi(L_met),1) ;
        }
	else if(variableList.at(iVar).variableName == "ptTJMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((RecoJets.at(1).jet4V_+L_met).Pt(),1) ;
        }
 
	///
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),1) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.DeltaPhi(L_LLmet),1) ;
        }
	else if(variableList.at(iVar).variableName == "dR_JJ_LLMet" and RecoJets.size() >=2){
          itVec->histogram->Fill(L_dijet.DeltaR(L_LLmet),1) ;
        }

	else if(variableList.at(iVar).variableName == "mlljj" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet).M(),1) ;
        }

	else if(variableList.at(iVar).variableName == "mlljjmet" and RecoJets.size() >=2){
          itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),1) ;
        }

	else if(variableList.at(iVar).variableName == "mTH" and RecoJets.size() >=2){
          itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),1) ;
        } 
      }    
    }
  }

  // events after each cut set
  for(size_t iCut = 0; iCut < CutList.size(); iCut++)
    cout<<" Nevents after selection "<<CutList.at(iCut).cutLayerName<<" "<<lumi*CrossSection*eventPassingSelection.at(iCut)/chain->GetEntries()<<endl;


  //Normalize histograms
  for(size_t ihisto = 0; ihisto < plotVector.size(); ihisto++)
    plotVector.at(ihisto).histogram->Scale(1./(plotVector.at(ihisto).histogram->GetEntries()*lumi*CrossSection/chain->GetEntries()));

	          
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    

  // make the canvas and basic banners                                                                                                                                         
  TCanvas *cCanvas = new TCanvas("cCanvas","",180,52,550,550);
  cCanvas->SetTicks();
  cCanvas->SetFillColor(0);
  cCanvas->SetBorderMode(0);
  cCanvas->SetBorderSize(2);
  cCanvas->SetTickx(1);
  cCanvas->SetTicky(1);
  cCanvas->SetRightMargin(0.05);
  cCanvas->SetBottomMargin(0.12);
  cCanvas->SetFrameBorderMode(0);

  TLatex * tex = new TLatex(0.94,0.92," 14 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  TLatex * tex2 = new TLatex(0.14,0.92,"Delphes");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.04);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.286,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.035);
  tex3->SetLineWidth(2);

  TLegend* legend = new TLegend(0.55,0.82,0.85,0.89);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.031);
  legend->SetTextFont(42);
  legend->SetNColumns (3) ;
 


  // make the plot on the same canvas for each variable (legend entry is the cut layer name)  
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){ // loop on cuts
        histoContainer tmpPlot;
        tmpPlot.cutName = CutList.at(iCut).cutLayerName;
        tmpPlot.varName = variableList.at(iVar).variableName;
        vector<histoContainer>::iterator itVec ;
	itVec = find(plotVector.begin(),plotVector.end(),tmpPlot);
        if(itVec == plotVector.end()){
          cerr<<"Problem -->plot not found for "<<CutList.at(iCut).cutLayerName<<"  "<<variableList.at(iVar).variableName<<endl;
	}

       
        itVec->histogram->GetXaxis()->SetTitleSize(0.04);
        itVec->histogram->GetXaxis()->SetLabelSize(0.04);
        itVec->histogram->GetYaxis()->SetRangeUser(0.001,itVec->histogram->GetMaximum()*1.2);
        itVec->histogram->GetXaxis()->SetLabelSize(0.04);
        itVec->histogram->GetYaxis()->SetTitleSize(0.04);
        itVec->histogram->SetLineColor(iCut+1);
        if(iCut %2 == 0)
	  itVec->histogram->SetLineStyle(1);
        else
	  itVec->histogram->SetLineStyle(2);

        itVec->histogram->SetLineWidth(2);
        itVec->histogram->GetYaxis()->SetTitle("#sigma x lumi");

        if(iCut == 0) 
	  itVec->histogram->Draw("hist");
        else
	  itVec->histogram->Draw("hist same");

        legend->AddEntry(itVec->histogram,CutList.at(iCut).cutLayerName.c_str(),"l");
    }

    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");
    legend->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".root").c_str(),"root");

    cCanvas->SetLogy();

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+"_log.root").c_str(),"root");

    cCanvas->SetLogy(0);

    gPad->Update();

    legend->Clear();

  }
  
  return 0 ;
}  

