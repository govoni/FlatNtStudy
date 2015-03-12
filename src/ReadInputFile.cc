#include "ReadInputFile.h"

using namespace std;

// take the fake rate 2D map + interpolation

#define upperPtBound  1000
#define upperEtaBound 3

// de-constructor
fakeRateContainer::~fakeRateContainer(){}

// default contructor
fakeRateContainer::fakeRateContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

  cout<<"start to build the fake rate container"<<endl;

  // electron case
  TH2F* Denominator_bjet_to_e = (TH2F*) inputFile->Get("Denominator_b_jet_to_e") ;
  TH2F* Denominator_jet_to_e  = (TH2F*) inputFile->Get("Denominator_jet_to_e");

  TH2F* Numerator_bjet_to_e = (TH2F*) inputFile->Get("Jet_Numerator_b_jet_to_e") ;
  TH2F* Numerator_jet_to_e  = (TH2F*) inputFile->Get("Jet_Numerator_jet_to_e");

  eFakeRate_bjet = (TH2F*) Numerator_bjet_to_e->Clone("eFakeRate_bjet");
  eFakeRate_bjet->Divide(Denominator_bjet_to_e);
  eFakeRate_bjet->Smooth();

  eFakeRate_jet = (TH2F*) Numerator_jet_to_e->Clone("eFakeRate_jet");
  eFakeRate_jet->Divide(Denominator_jet_to_e);
  eFakeRate_jet->Smooth();

  // center bin histo
  PtCentre_bjet_to_e = (TH2F*) inputFile->Get("Pt_centre_b_jet_to_e");
  PtCentre_jet_to_e = (TH2F*) inputFile->Get("Pt_centre_jet_to_e");

  electronFakeRate_bjet = new TGraph2D();
  electronFakeRate_jet = new TGraph2D();

  // get the correct map
  getFakeRateInterpolation(electronFakeRate_bjet,eFakeRate_bjet,PtCentre_bjet_to_e);
  getFakeRateInterpolation(electronFakeRate_jet,eFakeRate_jet,PtCentre_jet_to_e);

  // muon case
  TH2F* Denominator_bjet_to_mu = (TH2F*) inputFile->Get("Denominator_b_jet_to_mu") ;
  TH2F* Denominator_jet_to_mu  = (TH2F*) inputFile->Get("Denominator_jet_to_mu");

  TH2F* Numerator_bjet_to_mu = (TH2F*) inputFile->Get("Jet_Numerator_b_jet_to_mu") ;
  TH2F* Numerator_jet_to_mu  = (TH2F*) inputFile->Get("Jet_Numerator_jet_to_mu");

  mFakeRate_bjet = (TH2F*) Numerator_bjet_to_mu->Clone("mFakeRate_bjet");
  mFakeRate_bjet->Divide(Denominator_bjet_to_mu);
  mFakeRate_bjet->Smooth();

  mFakeRate_jet = (TH2F*) Numerator_jet_to_mu->Clone("mFakeRate_jet");
  mFakeRate_jet->Divide(Denominator_jet_to_mu);
  mFakeRate_jet->Smooth();
  
  // center bin histo
  PtCentre_bjet_to_mu = (TH2F*) inputFile->Get("Pt_centre_b_jet_to_mu");
  PtCentre_jet_to_mu = (TH2F*) inputFile->Get("Pt_centre_jet_to_mu");
  
  muonFakeRate_bjet = new TGraph2D();
  muonFakeRate_jet = new TGraph2D();

  // get the correct map
  getFakeRateInterpolation(muonFakeRate_bjet,mFakeRate_bjet,PtCentre_bjet_to_mu);
  getFakeRateInterpolation(muonFakeRate_jet,mFakeRate_jet,PtCentre_jet_to_mu);
}


void fakeRateContainer::getFakeRateInterpolation( TGraph2D* fakeRate, TH2F* fakeMap, TH2F* ptCentre){

  int nPoint = 0;

  for(int iBinX = 0; iBinX <= ptCentre->GetNbinsX()+1; iBinX++){
    for(int iBinY = 0; iBinY <= ptCentre->GetNbinsY()+1; iBinY++){

      float etaPoint = 0;
      float ptPoint  = 0;
      float valuePoint = 0;

      nPoint++;

      if(iBinX == 0)
	etaPoint = -0.1 ;
      if(iBinX == ptCentre->GetNbinsX()+1)
	etaPoint = upperEtaBound ;
      else
	etaPoint = fakeMap->GetXaxis()->GetBinCenter(iBinX);

      if(iBinY == 0)
	ptPoint = 0 ;
      if(iBinY == ptCentre->GetNbinsY()+1)
	ptPoint = upperPtBound ;
      else{
	if(iBinX == 0)
	  ptPoint = ptCentre->GetBinContent(iBinX+1,iBinY);
	else if(iBinX == ptCentre->GetNbinsX()+1)
	  ptPoint = ptCentre->GetBinContent(iBinX-1,iBinY);
	else
	  ptPoint = ptCentre->GetBinContent(iBinX,iBinY);
      }

     
      if(iBinX == 0 and iBinY == 0){
	valuePoint = fakeMap->GetBinContent(iBinX+1,iBinY+1);
      }
      else if(iBinY == ptCentre->GetNbinsY()+1 and iBinX == ptCentre->GetNbinsX()+1 ){
	valuePoint = fakeMap->GetBinContent(iBinX-1,iBinY-1);
      }
      else if(iBinX == 0 and iBinY == ptCentre->GetNbinsY()+1){
	valuePoint = fakeMap->GetBinContent(iBinX+1,iBinY-1);
      }
     else if(iBinY == 0 and iBinX == ptCentre->GetNbinsX()+1){
	valuePoint = fakeMap->GetBinContent(iBinX-1,iBinY+1);
      }

      else if(iBinX == 0){
	valuePoint = fakeMap->GetBinContent(iBinX+1,iBinY);
      }
      else if(iBinY == 0){
	valuePoint = fakeMap->GetBinContent(iBinX,iBinY+1);
      }
      else if(iBinX == ptCentre->GetNbinsX()+1 ){
	valuePoint = fakeMap->GetBinContent(iBinX-1,iBinY);
      }
      else if(iBinY == ptCentre->GetNbinsY()+1){
	valuePoint = fakeMap->GetBinContent(iBinX,iBinY-1);
      }
      
      else {
	valuePoint = fakeMap->GetBinContent(iBinX,iBinY);
      }

      fakeRate->SetPoint(nPoint,etaPoint,ptPoint,valuePoint);

    }
  }
}


float fakeRateContainer::getFakeRate (const int & leptonPID, const int & jetflavour, const float & pt, const float & eta){
  if(fabs(leptonPID) == 11){ // electron case                                                                                                                                
    if(fabs(jetflavour) == 5 or jetflavour == 4)
      return electronFakeRate_bjet->Interpolate(fabs(eta),pt);
    else
      return electronFakeRate_jet->Interpolate(fabs(eta),pt);
  }
  else if (fabs(leptonPID) == 13){ // muon case                                                                                                                               
    if(fabs(jetflavour) == 5 or jetflavour == 4)
      return muonFakeRate_bjet->Interpolate(fabs(eta),pt);
    else
      return muonFakeRate_jet->Interpolate(fabs(eta),pt);
  }
  else return 1 ;

}

float fakeRateContainer::getFakeRateUncertainty (const int & pdgId_lepton, const int & jetflavour, const float & pt, const float & eta){

  if(fabs(pdgId_lepton) == 11){ // electron case                                                                                                                     
    if(fabs(jetflavour) == 5 or jetflavour == 4)
      return eFakeRate_bjet->GetBinError(eFakeRate_bjet->GetXaxis()->FindBin(fabs(eta)),eFakeRate_bjet->GetYaxis()->FindBin(pt));
    else
      return eFakeRate_jet->GetBinError(eFakeRate_jet->GetXaxis()->FindBin(fabs(eta)),eFakeRate_jet->GetYaxis()->FindBin(pt));
  }  
  else if (fabs(pdgId_lepton) == 13){ // electron case                                                                                                                        
    if(fabs(jetflavour) == 5 or jetflavour == 4)
      return mFakeRate_bjet->GetBinError(mFakeRate_bjet->GetXaxis()->FindBin(fabs(eta)),mFakeRate_bjet->GetYaxis()->FindBin(pt));
    else
      return mFakeRate_jet->GetBinError(mFakeRate_jet->GetXaxis()->FindBin(fabs(eta)),mFakeRate_jet->GetYaxis()->FindBin(pt));
  }
  else return 1 ;

}

// fake rate Migration
fakeMigrationContainer::~fakeMigrationContainer(){}

fakeMigrationContainer::fakeMigrationContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

  cout<<"start to build the fake rate migration container"<<endl;

  Pt_migration_centre_jet_to_mu = (TH1F*) inputFile->Get("Pt_migration_centre_jet_to_mu");
  Pt_migration_centre_jet_to_e = (TH1F*) inputFile->Get("Pt_migration_centre_jet_to_e");

  Pt_migration_centre_bjet_to_mu = (TH1F*) inputFile->Get("Pt_migration_b_centre_jet_to_mu");
  Pt_migration_centre_bjet_to_e = (TH1F*) inputFile->Get("Pt_migration_b_centre_jet_to_e");

  Pt_migration_jet_to_mu = (TH2F*) inputFile->Get("Pt_migration_jet_to_mu");
  Pt_migration_jet_to_e = (TH2F*) inputFile->Get("Pt_migration_jet_to_e");

  Pt_migration_bjet_to_mu = (TH2F*) inputFile->Get("Pt_migration_b_jet_to_mu");
  Pt_migration_bjet_to_e = (TH2F*) inputFile->Get("Pt_migration_b_jet_to_e");


  profile_jet_to_e = Pt_migration_jet_to_e->ProfileX("profile_jet_to_e_fX");
  profile_jet_to_mu = Pt_migration_jet_to_mu->ProfileX("profile_jet_to_mu_fX");

  profile_bjet_to_e = Pt_migration_bjet_to_e->ProfileX("profile_bjet_to_e_fX");
  profile_bjet_to_mu = Pt_migration_bjet_to_mu->ProfileX("profile_bjet_to_mu_fX");

  migration_jet_to_mu = new TGraph();
  migration_jet_to_e  = new TGraph();

  migration_bjet_to_mu = new TGraph();
  migration_bjet_to_e  = new TGraph();

  for(int iBin = 0; iBin < profile_jet_to_e->GetNbinsX(); iBin++){
    migration_jet_to_e->SetPoint(iBin+1,Pt_migration_centre_jet_to_e->GetBinContent(iBin+1),profile_jet_to_e->GetBinContent(iBin+1));
  }

  for(int iBin = 0; iBin < profile_jet_to_mu->GetNbinsX(); iBin++){
    migration_jet_to_mu->SetPoint(iBin+1,Pt_migration_centre_jet_to_mu->GetBinContent(iBin+1),profile_jet_to_mu->GetBinContent(iBin+1));
  }

  for(int iBin = 0; iBin < profile_bjet_to_e->GetNbinsX(); iBin++){
    migration_bjet_to_e->SetPoint(iBin+1,Pt_migration_centre_bjet_to_e->GetBinContent(iBin+1),profile_bjet_to_e->GetBinContent(iBin+1));
  }

  for(int iBin = 0; iBin < profile_bjet_to_mu->GetNbinsX(); iBin++){
    migration_bjet_to_mu->SetPoint(iBin+1,Pt_migration_centre_bjet_to_mu->GetBinContent(iBin+1),profile_bjet_to_mu->GetBinContent(iBin+1));
  }

}

float fakeMigrationContainer::getMigration (const int & leptonPID, const int & jetflavour, const float & pt, const float & eta){

  if(fabs(leptonPID) == 11 ){
    if(fabs(jetflavour) == 5 or fabs(jetflavour) == 4)
      return migration_bjet_to_e->Eval(pt);
    else
      return migration_jet_to_e->Eval(pt);

  }

  if(fabs(leptonPID) == 13 ){
    if(fabs(jetflavour) == 5 or fabs(jetflavour) == 4)
      return migration_bjet_to_mu->Eval(pt);
    else
      return migration_jet_to_mu->Eval(pt);

  }

  else return 0;

}


// Input Sample normal parser (bin,min,max)
int ReadInputSampleFile(const string & InputSampleList, map<string,vector<sampleContainer> > & SampleContainer){

  ifstream inputFile (InputSampleList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 

  vector<string> NameSample;
  vector<string> NameReducedSample;
  vector<int>    ColorSample;
  vector<float>  SampleCrossSection;
  vector<int>    NumEntriesBefore;
  vector<int>    isSignal;

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" " ) continue ;
    stringstream line(buffer);

    string  NameSampleTemp;
    string  NameReducedSampleTemp;
    string  ColorSampleTemp;
    string  SampleCrossSectionTemp;
    string  NumEntresBeforeTemp;
    string  isSignalTemp;

    line >> NameSampleTemp >> NameReducedSampleTemp >> ColorSampleTemp >> SampleCrossSectionTemp >> NumEntresBeforeTemp >> isSignalTemp;

    NameSample.push_back(NameSampleTemp);   
    NameReducedSample.push_back(NameReducedSampleTemp);
    ColorSample.push_back(stoi(ColorSampleTemp)); 
    SampleCrossSection.push_back(stof(SampleCrossSectionTemp)); 
    NumEntriesBefore.push_back(stoi(NumEntresBeforeTemp));  
    isSignal.push_back(stoi(isSignalTemp));
  }

  
  for(size_t iSample = 0; iSample < NameReducedSample.size(); iSample++){
    SampleContainer[NameReducedSample.at(iSample)].push_back(
           sampleContainer(NameSample.at(iSample),ColorSample.at(iSample),SampleCrossSection.at(iSample),NumEntriesBefore.at(iSample),isSignal.at(iSample)));
  }


  NameSample.clear();
  NameReducedSample.clear();
  ColorSample.clear();
  SampleCrossSection.clear();
  NumEntriesBefore.clear();
  isSignal.clear();

  return SampleContainer.size() ;

}


int ReadInputSampleFile(const string & InputSampleList, unordered_map<string,vector<sampleContainer> > & SampleContainer){

  ifstream inputFile (InputSampleList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 

  vector<string> NameSample;
  vector<string> NameReducedSample;
  vector<int>    ColorSample;
  vector<float>  SampleCrossSection;
  vector<int>    NumEntriesBefore;
  vector<int>    isSignal;

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" " ) continue ;
    stringstream line(buffer);

    string  NameSampleTemp;
    string  NameReducedSampleTemp;
    string  ColorSampleTemp;
    string  SampleCrossSectionTemp;
    string  NumEntresBeforeTemp;
    string  isSignalTemp;

    line >> NameSampleTemp >> NameReducedSampleTemp >> ColorSampleTemp >> SampleCrossSectionTemp >> NumEntresBeforeTemp >> isSignalTemp;

    NameSample.push_back(NameSampleTemp);   
    NameReducedSample.push_back(NameReducedSampleTemp);
    ColorSample.push_back(stoi(ColorSampleTemp)); 
    SampleCrossSection.push_back(stof(SampleCrossSectionTemp)); 
    NumEntriesBefore.push_back(stoi(NumEntresBeforeTemp));  
    isSignal.push_back(stoi(isSignalTemp));
  }

  
  for(size_t iSample = 0; iSample < NameReducedSample.size(); iSample++){
    SampleContainer[NameReducedSample.at(iSample)].push_back(
           sampleContainer(NameSample.at(iSample),ColorSample.at(iSample),SampleCrossSection.at(iSample),NumEntriesBefore.at(iSample),isSignal.at(iSample)));
  }


  NameSample.clear();
  NameReducedSample.clear();
  ColorSample.clear();
  SampleCrossSection.clear();
  NumEntriesBefore.clear();
  isSignal.clear();

  return SampleContainer.size() ;

}

// Variable file
int ReadInputVariableFile( const string & InputVariableList , vector<variableContainer> & varContainer){


  ifstream inputFile (InputVariableList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    stringstream line(buffer);

    string  VariablesTemp;
    string  VariablesNbinTemp;
    string  VariablesMinValueTemp;
    string  VariablesMaxValueTemp;
    string  VariablesTitleTemp;
    
    line >> VariablesTemp >> VariablesNbinTemp >> VariablesMinValueTemp >> VariablesMaxValueTemp >> VariablesTitleTemp ;
    for(size_t ifound = 0 ; ifound < VariablesTitleTemp.size() ; ifound++) {
      if(VariablesTitleTemp.at(ifound)=='_' && VariablesTitleTemp.at(ifound+1)!='{') VariablesTitleTemp.at(ifound)=' '; 
    }
    
    variableContainer dummy (VariablesTemp,
                             stoi(VariablesNbinTemp),
                             stof(VariablesMinValueTemp),
                             stof(VariablesMaxValueTemp),
                             VariablesTitleTemp);

    varContainer.push_back(dummy);                                  
  }

  return varContainer.size() ;

}

int ReadInputVariableFile( const string & InputVariableList , vector<variableContainer2D> & varContainer2D){

  ifstream inputFile (InputVariableList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    stringstream line(buffer);

    string  VariablesTempX;
    string  VariablesNbinTempX;
    string  VariablesMinValueTempX;
    string  VariablesMaxValueTempX;
    string  VariablesTitleTempX;

    string  VariablesTempY;
    string  VariablesNbinTempY;
    string  VariablesMinValueTempY;
    string  VariablesMaxValueTempY;
    string  VariablesTitleTempY;
    
    line >> VariablesTempX >> VariablesNbinTempX >> VariablesMinValueTempX >> VariablesMaxValueTempX >> VariablesTitleTempX >> VariablesTempY >> VariablesNbinTempY >> VariablesMinValueTempY >> VariablesMaxValueTempY >> VariablesTitleTempY ;

    for(size_t ifound = 0 ; ifound < VariablesTitleTempX.size() ; ifound++) {
      if(VariablesTitleTempX.at(ifound)=='_' && VariablesTitleTempX.at(ifound+1)!='{') VariablesTitleTempX.at(ifound)=' '; 
    }

    for(size_t ifound = 0 ; ifound < VariablesTitleTempY.size() ; ifound++) {
      if(VariablesTitleTempY.at(ifound)=='_' && VariablesTitleTempY.at(ifound+1)!='{') VariablesTitleTempY.at(ifound)=' '; 
    }
    
    variableContainer2D dummy (VariablesTempX,
			       stoi(VariablesNbinTempX),
			       stof(VariablesMinValueTempX),
			       stof(VariablesMaxValueTempX),
			       VariablesTitleTempX,
			       VariablesTempY,
			       stoi(VariablesNbinTempY),
			       stof(VariablesMinValueTempY),
			       stof(VariablesMaxValueTempY),
			       VariablesTitleTempY);

    varContainer2D.push_back(dummy);                                  
  }

  return varContainer2D.size() ;

}


int ReadInputVariableFile( const string & InputVariableList , vector<string> & varContainer){


  ifstream inputFile (InputVariableList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    stringstream line(buffer);

    string  VariablesTemp;
    
    line >> VariablesTemp ;

    varContainer.push_back(VariablesTemp);                                  
  }

  return varContainer.size() ;

}



// dynamic binning
int ReadInputVariableFileDynamicBinning ( const string & InputVariableList , vector<variableContainerDynamic> & varContainer){


  ifstream inputFile (InputVariableList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    stringstream line(buffer);

    string  VariablesTemp;
    string  VariablesNbinTemp;
    string  VariablesBinningTemp;
    string  VariablesTitleTemp;
    
    line >> VariablesTemp >> VariablesNbinTemp >> VariablesBinningTemp >> VariablesTitleTemp ;
    for(size_t ifound = 0 ; ifound < VariablesTitleTemp.size() ; ifound++) {
      if(VariablesTitleTemp.at(ifound)=='_' && VariablesTitleTemp.at(ifound+1)!='{') VariablesTitleTemp.at(ifound)=' '; 
    }

    stringstream binning(VariablesBinningTemp);
    string segment;
    vector<float> seglist;

    while(getline(binning, segment, ',')){
      seglist.push_back(stof(segment));
    }
    
    variableContainerDynamic dummy (VariablesTemp,
				    stoi(VariablesNbinTemp),
				    seglist,
				    VariablesTitleTemp);

    varContainer.push_back(dummy);                                  
  }

  return varContainer.size() ;

}

int ReadInputVariableFileDynamicBinning( const string & InputVariableList , vector<variableContainerDynamic2D> & varContainer2D){


  ifstream inputFile (InputVariableList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    stringstream line(buffer);

    string  VariablesTempX;
    string  VariablesNbinTempX;
    string  VariablesBinningTempX;
    string  VariablesTitleTempX;

    string  VariablesTempY;
    string  VariablesNbinTempY;
    string  VariablesBinningTempY;
    string  VariablesTitleTempY;
    
    line >> VariablesTempX >> VariablesNbinTempX >> VariablesBinningTempX >> VariablesTitleTempX >> VariablesTempY >> VariablesNbinTempY >> VariablesBinningTempY >> VariablesTitleTempY ;

    for(size_t ifound = 0 ; ifound < VariablesTitleTempX.size() ; ifound++) {
      if(VariablesTitleTempX.at(ifound)=='_' && VariablesTitleTempX.at(ifound+1)!='{') VariablesTitleTempX.at(ifound)=' '; 
    }

    for(size_t ifound = 0 ; ifound < VariablesTitleTempY.size() ; ifound++) {
      if(VariablesTitleTempY.at(ifound)=='_' && VariablesTitleTempY.at(ifound+1)!='{') VariablesTitleTempY.at(ifound)=' '; 
    }

    stringstream binningX(VariablesBinningTempX);
    stringstream binningY(VariablesBinningTempY);
    string segment;
    vector<float> seglistX;
    vector<float> seglistY;

    while(getline(binningX, segment, ',')){
      seglistX.push_back(stof(segment));
    }

    while(getline(binningY, segment, ',')){
      seglistY.push_back(stof(segment));
    }
    
    variableContainerDynamic2D dummy (VariablesTempX,
				      stoi(VariablesNbinTempX),
				      seglistX,
				      VariablesTitleTempX,
				      VariablesTempY,
				      stoi(VariablesNbinTempY),
				      seglistY,
				      VariablesTitleTempY);

    varContainer2D.push_back(dummy);                                  
  }

  return varContainer2D.size() ;

}



// cut list
int ReadInputCutFile( const string & InputCutList , vector<cutContainer> & CutContainer){


  ifstream inputFile (InputCutList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;

    stringstream line(buffer);      

    string layerName, ptL1, ptL2, chargeSign, flavour, nLep, nextra, MET, ptJet1, ptJet2, DetaJJ, Mjj, DetaLL, MllMin, MllMax,  MllZVetoMin, MllZVetoMax, bTagCut, nBVeto, nSoftMu, jetPUID, polarization, etaMaxL, ptJJLLMet, dRJL, HTTrackjets, lZep;    

    line >> layerName >> ptL1 >> ptL2 >> etaMaxL >> chargeSign >> flavour >> nLep >> nextra >> MET >> ptJet1 >> ptJet2 >> DetaJJ >> Mjj >> DetaLL >> MllMin >> MllMax >>  MllZVetoMin >> MllZVetoMax >> bTagCut >> nBVeto >> nSoftMu >> jetPUID >> dRJL >> ptJJLLMet >> lZep >> HTTrackjets >> polarization;     

    cutContainer dummy(layerName,
                       make_pair(stof(ptL1),stof(ptL2)),
		       stof(etaMaxL),
                       stof(chargeSign),
                       stof(flavour),
                       stoi(nLep),
                       stoi(nextra),
                       stof(MET),
                       make_pair(stof(ptJet1),stof(ptJet2)),
                       stof(DetaJJ),
                       stof(Mjj),
                       stof(DetaLL),
                       make_pair(stof(MllMin),stof(MllMax)),
                       make_pair(stof(MllZVetoMin),stof(MllZVetoMax)),
                       stof(bTagCut), 
                       stoi(nBVeto), 
                       stoi(nSoftMu), 		       
                       stof(jetPUID),
		       stof(dRJL),
		       stof(ptJJLLMet),
		       stof(lZep),
		       stof(HTTrackjets),
                       stoi(polarization));

    
    CutContainer.push_back(dummy);

  }

  return CutContainer.size() ;

}

int ReadInputTrainingFile (const string & InputTrainingList, vector<trainingContainer> & trainContainer){


  ifstream inputFile (InputTrainingList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;

    stringstream line(buffer);      

    string fileName, varNameReduced, puMin, puMax, methodTemp;
    vector<string> methodName;

    line >> fileName >> varNameReduced >> puMin >> puMax >> methodTemp;

    TString line_temp (methodTemp);
    line_temp.ReplaceAll(fileName.c_str(),"");  
    line_temp.ReplaceAll(varNameReduced.c_str(),"");  
    line_temp.ReplaceAll(puMin.c_str(),"");  
    line_temp.ReplaceAll(puMax.c_str(),"");  
    line_temp.ReplaceAll(" ","");  
  
    string segment;
    stringstream vect(line_temp.Data());
    while(getline(vect, segment,':')){
      methodName.push_back(segment);
    }


    trainingContainer dummy(fileName,
			    varNameReduced,
			    make_pair(stoi(puMin),stoi(puMax)),
			    methodName
			    );

    
    trainContainer.push_back(dummy);

  }

  return trainContainer.size() ;


}
