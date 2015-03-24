/////////////////////////////////////////////////////////////////////////////////////////////////////
// compare for a single variable the expected stats uncertainty to the difference between H and noH //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TLatex.h"
#include "TPad.h"

#include "kinematicUtils.h"
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

string  finalStateString;

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

  // total number of events
  int maxEventNumber   = gConfigParser -> readFloatOption("Input::EventsNumber");

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

  TChain* chain = new TChain (treeName.c_str()) ;  
  chain->Add ((InputBaseDirectory+"/*_1.root").c_str()) ;
  int totEvent = chain->GetEntries();

  readTree* reader  = new readTree((TTree*)(chain));

  cout<<"Lumi (fb-1) "<<lumi/1000<<" entries before "<<totEvent<<" cross section "<<CrossSection<<" Nevents before selections "<<lumi*CrossSection<<" weight "<<lumi*CrossSection/float(totEvent)<<endl;

  float weight = 1.0*lumi*CrossSection/float(totEvent) ;

  // make the plot container 
  vector<histoContainer> plotVector;

  for(size_t iCut = 0; iCut < CutList.size(); iCut++){
    histoCutEff["WW_EWK_pos_"+to_string(iCut)+"_"+CutList.at(iCut).cutLayerName] = new TH1F(("WW_EWK_pos_"+to_string(iCut)+"_"+CutList.at(iCut).cutLayerName).c_str(),"",15,0,15);
    for(size_t iVar = 0; iVar < variableList.size(); iVar++){
      plotVector.push_back(histoContainer(CutList.at(iCut).cutLayerName,variableList.at(iVar)));
    }
  }

  int passingLHEFilter = 0 ;
  
  int maximumEvents = chain->GetEntries () ;
  if (maxEventNumber > 0 && maxEventNumber < maximumEvents) 
    maximumEvents = maxEventNumber ;
  
  // Loop on the events
  for(int iEvent = 0; iEvent < maximumEvents ; iEvent++){

    reader->fChain->GetEntry(iEvent) ;

    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;

    // filter LHE level leptons
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
    TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2, L_neutrino1, L_neutrino2, L_vboson1, L_vboson2 ;

    L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
    L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);

    L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
    L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

    L_neutrino1.SetPtEtaPhiM(reader->neutrinoLHEpt1,reader->neutrinoLHEeta1,reader->neutrinoLHEphi1,0.);
    L_neutrino2.SetPtEtaPhiM(reader->neutrinoLHEpt2,reader->neutrinoLHEeta2,reader->neutrinoLHEphi2,0.);

    L_vboson1.SetPtEtaPhiM(reader->vbosonLHEpt1,reader->vbosonLHEeta1,reader->vbosonLHEphi1,reader->vbosonLHEm1);
    L_vboson2.SetPtEtaPhiM(reader->vbosonLHEpt2,reader->vbosonLHEeta2,reader->vbosonLHEphi2,reader->vbosonLHEm2);

    double costheta1 = 0;
    double costheta2 = 0;
    double Phi  = 0;
    double costhetastar  = 0;
    double Phi1  = 0;

    computeAnglesResonance(L_vboson1+L_vboson2,L_vboson1,L_lepton1,L_neutrino1,L_vboson2,L_lepton2,L_neutrino2,
			   costheta1,costheta2,Phi,costhetastar,Phi1);

    double costheta1_vbf = 0;
    double costheta2_vbf = 0;
    double Phi_vbf = 0;
    double costhetastar_vbf = 0;
    double Phi1_vbf = 0;

    computeAnglesVBF(L_vboson1+L_vboson2,L_vboson1,L_vboson2,L_lepton1,L_neutrino1,L_lepton2,L_neutrino2,L_parton1,L_parton2,
		     costheta1,costheta2,Phi,costhetastar,Phi1);

    

    if(L_lepton1.Pt() < minLeptonCutPt or L_lepton2.Pt() < minLeptonCutPt) continue;

    if(L_parton1.Eta()*L_parton2.Eta() > 0) continue;

    // Loop  on the cut list --> one cut for each polarization
    for(size_t iCut = 0; iCut < CutList.size(); iCut++){

      // cut the events
      string name = "WW_EWK";
      if(!passCutContainerSelection(reader,
                                    CutList.at(iCut),
                                    name,
				    int(iCut),
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

      
      float asimL = (L_lepton1.Pt()-L_lepton2.Pt())/(L_lepton1.Pt()+L_lepton2.Pt()) ;

      TLorentzVector L_met ,L_dijet, L_dilepton, L_LLmet; 

      L_met = L_neutrino1 + L_neutrino2;
      L_dijet    = L_parton1 + L_parton2;
      L_dilepton = L_lepton1+L_lepton2;
      L_LLmet    = L_dilepton + L_met ;

      float asimJ    = (L_parton1.Pt()-L_parton2.Pt())/(L_parton1.Pt()+L_parton2.Pt()) ;
      float Rvar     = (L_lepton1.Pt()*L_lepton2.Pt())/(L_parton1.Pt()*L_parton2.Pt()) ;

      // loop on variables
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

	if(variableList.at(iVar).variableName == "ptV1"){
          itVec->histogram->Fill(L_vboson1.Pt(),1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "ptV2"){
          itVec->histogram->Fill(L_vboson2.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etaV1"){
          itVec->histogram->Fill(fabs (L_vboson1.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etaV2"){
          itVec->histogram->Fill(fabs (L_vboson2.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptVV"){
          itVec->histogram->Fill(L_dijet.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "mVV"){
          itVec->histogram->Fill(L_dijet.M(),weight) ;
        }


	if(variableList.at(iVar).variableName == "costheta1"){
          itVec->histogram->Fill(costheta1,1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "costheta2"){
          itVec->histogram->Fill(costheta2,weight) ;
        }
	if(variableList.at(iVar).variableName == "costheta1_vbf"){
          itVec->histogram->Fill(costheta1_vbf,1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "costheta2_vbf"){
          itVec->histogram->Fill(costheta2_vbf,weight) ;
        }

	if(variableList.at(iVar).variableName == "Phi"){
          itVec->histogram->Fill(Phi,1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "Phi1"){
          itVec->histogram->Fill(Phi1,weight) ;
        }
	if(variableList.at(iVar).variableName == "Phi_vbf"){
          itVec->histogram->Fill(Phi_vbf,1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "Phi1_vbf"){
          itVec->histogram->Fill(Phi1_vbf,weight) ;
        }

	else if(variableList.at(iVar).variableName == "Phi1_vbf"){
          itVec->histogram->Fill(Phi1_vbf,weight) ;
        }
	else if(variableList.at(iVar).variableName == "Phi1_vbf"){
          itVec->histogram->Fill(Phi1_vbf,weight) ;
        }
	else if(variableList.at(iVar).variableName == "costhetastar"){
          itVec->histogram->Fill(costhetastar,weight) ;
        }
	else if(variableList.at(iVar).variableName == "costhetastar_vbf"){
          itVec->histogram->Fill(costhetastar_vbf,weight) ;
        }


	if(variableList.at(iVar).variableName == "ptj1"){
          itVec->histogram->Fill(L_parton1.Pt(),1.*weight) ; 
        }
	else if(variableList.at(iVar).variableName == "ptj2"){
          itVec->histogram->Fill(L_parton2.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etaj1"){
          itVec->histogram->Fill(fabs (L_parton1.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etaj2"){
          itVec->histogram->Fill(fabs (L_parton2.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "detajj"){
          itVec->histogram->Fill(fabs(L_parton1.Eta()-L_parton2.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptjj"){
          itVec->histogram->Fill(L_dijet.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "mjj"){
          itVec->histogram->Fill(L_dijet.M(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "Asim_j"){
          itVec->histogram->Fill(asimJ,weight) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ"){
          itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_parton2)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptl1"){
          itVec->histogram->Fill(L_lepton1.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptl2"){
          itVec->histogram->Fill(L_lepton2.Pt(),weight) ;
        }
 
	else if(variableList.at(iVar).variableName == "etal1"){
          itVec->histogram->Fill(fabs (L_lepton1.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etal2"){
          itVec->histogram->Fill(fabs (L_lepton2.Eta()),weight) ;
        }
	else if(variableList.at(iVar).variableName == "mll"){
          itVec->histogram->Fill(L_dilepton.M(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptll"){
          itVec->histogram->Fill(L_dilepton.Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "etall"){
          itVec->histogram->Fill(fabs (L_dilepton.Eta()),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LL"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_lepton2)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaEta_LL"){
          itVec->histogram->Fill(fabs(L_lepton1.Eta()-L_lepton2.Eta()),weight) ;
	}
	else if(variableList.at(iVar).variableName == "Asim_l"){
          itVec->histogram->Fill(asimL,weight) ;
        }

	else if(variableList.at(iVar).variableName == "met"){
          itVec->histogram->Fill(L_met.Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "R"){
          itVec->histogram->Fill(Rvar,weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LMet"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptLMet"){
          itVec->histogram->Fill((L_lepton1 + L_met).Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_TLMet"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptTLMet"){
          itVec->histogram->Fill((L_lepton2 + L_met).Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_LLMet"){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptLLMet"){
          itVec->histogram->Fill((L_dilepton + L_met).Pt(),weight) ;
        }
	///
	else if(variableList.at(iVar).variableName == "DeltaPhi_LJL"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton1)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptLJL"){
          itVec->histogram->Fill((L_lepton1+L_parton1).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJL"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_parton2)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptTJL"){
          itVec->histogram->Fill((L_lepton1+L_parton2).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJL"){
          itVec->histogram->Fill(fabs(L_lepton1.DeltaPhi(L_dijet)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptJJL"){
          itVec->histogram->Fill((L_lepton1+L_dijet).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LJTL"){
          itVec->histogram->Fill(fabs(L_lepton2.DeltaPhi(L_parton1)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJTL"){
          itVec->histogram->Fill(fabs(L_lepton2.DeltaPhi(L_parton2)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJTL"){
          itVec->histogram->Fill(fabs(L_lepton2.DeltaPhi(L_dijet)),weight) ;
        }


	else if(variableList.at(iVar).variableName == "ptLJTL"){
          itVec->histogram->Fill((L_lepton2+L_parton1).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptTJTL"){
          itVec->histogram->Fill((L_lepton2+L_parton2).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptJJTL"){
          itVec->histogram->Fill((L_lepton2+L_dijet).Pt(),weight) ;
        }

 
	else if(variableList.at(iVar).variableName == "DeltaPhi_LJLL"){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_parton1)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJLL"){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_parton2)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJLL"){
          itVec->histogram->Fill(fabs(L_dilepton.DeltaPhi(L_dijet)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptLJLL"){
          itVec->histogram->Fill((L_dilepton+L_parton1).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptTJLL"){
          itVec->histogram->Fill((L_dilepton+L_parton2).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "ptJJLL"){
          itVec->histogram->Fill((L_dilepton+L_dijet).Pt(),weight) ;
        }

	///

	else if(variableList.at(iVar).variableName == "DeltaPhi_JJMet"){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptJJMet"){
          itVec->histogram->Fill((L_dijet+L_met).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_LJMet"){
          itVec->histogram->Fill(fabs(L_parton1.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptLJMet"){
          itVec->histogram->Fill((L_parton1+L_met).Pt(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "DeltaPhi_TJMet"){
          itVec->histogram->Fill(fabs(L_parton2.DeltaPhi(L_met)),weight) ;
        }
	else if(variableList.at(iVar).variableName == "ptTJMet"){
          itVec->histogram->Fill((L_parton2+L_met).Pt(),weight) ;
        }
 
	///
	else if(variableList.at(iVar).variableName == "ptJJ_LLMet"){
          itVec->histogram->Fill((L_dijet+L_LLmet).Pt(),weight) ;
        }
	else if(variableList.at(iVar).variableName == "DeltaPhi_JJ_LLMet"){
          itVec->histogram->Fill(fabs(L_dijet.DeltaPhi(L_LLmet)),weight) ;
        }

	else if(variableList.at(iVar).variableName == "mlljj"){
          itVec->histogram->Fill((L_dilepton+L_dijet).M(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "mlljjmet"){
          itVec->histogram->Fill((L_dilepton+L_dijet+L_met).M(),weight) ;
        }

	else if(variableList.at(iVar).variableName == "mTH"){
          itVec->histogram->Fill(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met)))),weight) ;
        } 
      } // loop on variables
      
    } // Loop  on the cut list

  } // Loop on the events

  
  TFile* outputEfficiency = new TFile(("output/"+outputPlotDirectory+"/outputEfficiency.root").c_str(),"RECREATE");

  for(map<string,TH1F*>::const_iterator itMap = histoCutEff.begin(); itMap !=  histoCutEff.end(); itMap++){
    itMap->second->Scale(1./itMap->second->GetBinContent(1));
    itMap->second->Write();
  }

  outputEfficiency->Close();    

  
  // make the canvas and basic banners                                                                                                                                         
  TCanvas *cCanvas = new TCanvas("cCanvas","",1,52,550,550);
  cCanvas->SetTicks();
  cCanvas->SetFillColor(0);
  cCanvas->SetBorderMode(0);
  cCanvas->SetBorderSize(2);
  cCanvas->SetTickx(1);
  cCanvas->SetTicky(1);
  cCanvas->SetRightMargin(0.05);
  cCanvas->SetBottomMargin(0.12);
  cCanvas->SetFrameBorderMode(0);

  TPad* upperPad = new TPad("upperPad", "upperPad", .005, .180, .995, .980);
  TPad* lowerPad = new TPad("lowerPad", "lowerPad", .005, .005, .995, .18);
  lowerPad->SetGridx();
  lowerPad->SetGridy();
  upperPad->SetLeftMargin(0.12);
  upperPad->SetRightMargin(0.1);
  lowerPad->SetLeftMargin(0.12);
  lowerPad->SetRightMargin(0.1);
  lowerPad->SetTopMargin(0.002);

  lowerPad->Draw();
  upperPad->Draw();

  TLatex * tex = new TLatex(0.88,0.92," 14 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.045);
  tex->SetLineWidth(2);
  TLatex * tex2 = new TLatex(0.14,0.92,"Delphes");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.045);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.286,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.04);
  tex3->SetLineWidth(2);

  TLegend* legend = new TLegend(0.55,0.75,0.85,0.89);
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetNColumns (3) ;
 
  // make the plot on the same canvas for each variable (legend entry is the cut layer name)  
  vector<TH1F*>  numerator ;
  vector<TH1F*>  denominator ;
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){ // loop on var
   
    numerator.clear();
    denominator.clear();

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
        itVec->histogram->GetXaxis()->SetTitleOffset(1.16);
        itVec->histogram->GetXaxis()->SetLabelSize(0.04);

        itVec->histogram->GetYaxis()->SetRangeUser(0.001,itVec->histogram->GetMaximum()*1.1);
        itVec->histogram->GetYaxis()->SetTitleSize(0.05);
        itVec->histogram->GetYaxis()->SetTitleOffset(1.20);
        itVec->histogram->GetYaxis()->SetLabelSize(0.04);

        itVec->histogram->SetLineColor(iCut+1);

        if(iCut %2 == 0)
          itVec->histogram->SetLineStyle(1);
        else
          itVec->histogram->SetLineStyle(2);

        itVec->histogram->SetLineWidth(2);
        itVec->histogram->GetYaxis()->SetTitle("#sigma x lumi");

        upperPad->cd();

        if(iCut == 0) 
          itVec->histogram->Draw("hist");
        else
          itVec->histogram->Draw("hist same");

        legend->AddEntry(itVec->histogram,CutList.at(iCut).cutLayerName.c_str(),"l");

        if(itVec->findCutByLabel("LL")) numerator.push_back(itVec->histogram);
        denominator.push_back(itVec->histogram);
    }
    
    // make ratio plot
    lowerPad->cd();
    
    TH1F* numTotal = 0;
    TH1F* denTotal = 0;
    TH1F* ratio    = 0;
    TH1F* ratioW   = 0;

    for(size_t itNum = 0; itNum < numerator.size(); itNum ++){
      if(itNum == 0 and ratio == 0) 
        numTotal = (TH1F*) numerator.at(itNum)->Clone(("Num_"+string(numerator.at(itNum)->GetName())).c_str());
      else if(ratio !=0) 
        numTotal->Add(numerator.at(itNum));
    }

    for(size_t itDen = 0; itDen < denominator.size(); itDen ++){
      if(itDen == 0 and denTotal == 0 ) {
        denTotal = (TH1F*) denominator.at(itDen)->Clone(("Den_"+string(denominator.at(itDen)->GetName())).c_str());
      }
      else if(denTotal !=0){  
        denTotal->Add(denominator.at(itDen));
      }
    }
    
    ratio = new TH1F(("Ratio_"+string(denominator.at(0)->GetName())).c_str(),"",numTotal->GetNbinsX(),numTotal->GetBinLowEdge(1),numTotal->GetBinLowEdge(numTotal->GetNbinsX()+1));
    ratio->GetYaxis()->SetTitle("S/(#sqrt{S+B})");
    ratio->SetMarkerSize(1.1);

    ratioW = new TH1F(("ratioW_"+string(denominator.at(0)->GetName())).c_str(),"",numTotal->GetNbinsX(),numTotal->GetBinLowEdge(1),numTotal->GetBinLowEdge(numTotal->GetNbinsX()+1));
    ratioW->GetYaxis()->SetTitle("weighted S/(#sqrt{S+B})");
    ratioW->SetMarkerSize(1.1);

    TString name = "norm_" ;
    name += denTotal->GetName () ;
    TH1F * norm_denTotal = (TH1F *) denTotal->Clone (name) ;
    norm_denTotal->Scale (1. / norm_denTotal->GetMaximum ()) ; 
    // weight the S/sqrt (B) by the shape of the total, 
    // so that only bins with a lot of stats become visibly significant
    for(int iBin = 0; iBin < ratio->GetNbinsX()+1; iBin++){
      if(denTotal->GetBinContent(iBin) !=0){ 
        ratioW->SetBinContent(iBin,
			      norm_denTotal->GetBinContent (iBin) * numTotal->GetBinContent(iBin) / 
			      sqrt(denTotal->GetBinContent(iBin)));
        ratio->SetBinContent(iBin,
			     numTotal->GetBinContent(iBin) / sqrt(denTotal->GetBinContent(iBin)));
      }
      else 
	ratio->SetBinContent(iBin,0.);
    }
 
    ratio->GetYaxis()->SetRangeUser(ratio->GetMinimum()*0.7,ratio->GetMaximum()*1.2);
    ratio->GetXaxis()->SetTitle("");
    ratio->SetLineColor(kBlue);
    ratio->SetLineStyle(2);
    ratio->SetLineWidth(2);
    ratio->GetXaxis()->SetLabelOffset(999);
    ratio->GetXaxis()->SetLabelSize(0);
    ratio->GetYaxis()->SetLabelSize(0.15);
    ratio->GetYaxis()->SetTitleSize(0.15);
    ratio->GetYaxis()->SetTitleOffset(0.30);
    ratio->GetYaxis()->SetNdivisions(504);

    ratioW->GetYaxis()->SetRangeUser(ratioW->GetMinimum()*0.7,ratioW->GetMaximum()*1.2);
    ratioW->GetXaxis()->SetTitle("");
    ratioW->SetLineColor(kBlack);
    ratioW->SetLineWidth(2);
    ratioW->GetXaxis()->SetLabelOffset(999);
    ratioW->GetXaxis()->SetLabelSize(0);
    ratioW->GetYaxis()->SetLabelSize(0.15);
    ratioW->GetYaxis()->SetTitleSize(0.15);
    ratioW->GetYaxis()->SetTitleOffset(0.30);
    ratioW->GetYaxis()->SetNdivisions(504);

    TH1F * frame = lowerPad->DrawFrame (ratio->GetXaxis ()->GetXmin (), 0., 
                                        ratio->GetXaxis ()->GetXmax (), 2.) ;
    frame->GetXaxis()->SetTitle (ratio->GetXaxis ()->GetTitle ()) ;
    frame->GetYaxis()->SetTitle (ratio->GetYaxis ()->GetTitle ()) ;
    frame->GetXaxis()->SetLabelOffset(999);
    frame->GetXaxis()->SetLabelSize(0);
    frame->GetYaxis()->SetLabelSize(0.15);
    frame->GetYaxis()->SetTitleSize(0.15);
    frame->GetYaxis()->SetTitleOffset(0.30);
    frame->GetYaxis()->SetNdivisions(504);
    
    ratio->Draw("Lsame");    
    ratioW->Draw("Lsame");    
    
    upperPad->cd();
    
    tex->Draw("same");
    tex2->Draw("same");
    tex3->Draw("same");
    legend->Draw("same");

    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".pdf").c_str(),"pdf");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".png").c_str(),"png");
    cCanvas->SaveAs(string("output/"+outputPlotDirectory+"/"+variableList.at(iVar).variableName+".root").c_str(),"root");
    
    legend->Clear();
    
  } // loop on var

  cout<<"LHE filter efficiency : "<<passingLHEFilter<<" totEvent "<<totEvent<<" efficiency "<<float(passingLHEFilter)/float(totEvent)*100<<" % "<<endl;
  
  //Normalize histograms
  for(size_t ihisto = 0; ihisto < plotVector.size(); ihisto++){
    if(plotVector.at(ihisto).varName == "DeltaPhi_LL")
      cout<<"Events Histo "<<plotVector.at(ihisto).histogram->GetName()<<" unweighted "<<plotVector.at(ihisto).histogram->GetEntries()<<" weighted "<<plotVector.at(ihisto).histogram->Integral(0,plotVector.at(ihisto).histogram->GetNbinsX()+1)<<endl;
  }	          

  return 0 ;
}  

