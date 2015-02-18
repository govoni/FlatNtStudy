#include "ReadInputFile.h"

using namespace std;

#define upperPtBound  1000
#define upperEtaBound 3

fakeRateContainer::~fakeRateContainer(){

  if(eeDenominator != 0)
    eeDenominator->Delete() ;
  if(meDenominator != 0)
    meDenominator->Delete() ;
  if(eDenominator != 0)
    eDenominator->Delete() ;

  if(eeNumerator != 0)
    eeNumerator->Delete() ;
  if(meNumerator != 0)
    meNumerator->Delete() ;
  if(eNumerator != 0)
    eNumerator->Delete() ;

  if(mmDenominator != 0)
    mmDenominator->Delete() ;
  if(emDenominator != 0)
    emDenominator->Delete() ;
  if(mDenominator != 0)
    mDenominator->Delete() ;

  if(mmNumerator != 0)
    mmNumerator->Delete() ;
  if(emNumerator != 0)
    emNumerator->Delete() ;
  if(mNumerator != 0)
    mNumerator->Delete() ;

  if(mFakeRate!=0)
    mFakeRate->Delete() ;
  if(eFakeRate!=0)
    eFakeRate->Delete() ;

  if(muonFakeRate!=0)
    muonFakeRate->Delete() ;
  if(electronFakeRate!=0)
    electronFakeRate->Delete() ;

  if(mmPtCentre!=0)
    mmPtCentre->Delete();
  if(mePtCentre!=0)
    mePtCentre->Delete();
  if(emPtCentre!=0)
    emPtCentre->Delete();
  if(eePtCentre!=0)
    eePtCentre->Delete();

  if(mPtCentre!=0)
    mPtCentre->Delete();
  if(ePtCentre!=0)
    ePtCentre->Delete();

}

fakeRateContainer::fakeRateContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

  cout<<"start to build the fake rate container"<<endl;

  // electron case

  eeDenominator = (TH2F*) inputFile->Get("Denominator_W_to_e_jet_to_e");
  meDenominator = (TH2F*) inputFile->Get("Denominator_W_to_mu_jet_to_e");
  eDenominator  = (TH2F*) eeDenominator->Clone("eDenominator");
  eDenominator->Add(meDenominator);

  eeNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_e_jet_to_e");
  meNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_mu_jet_to_e");
  eNumerator  = (TH2F*) eeNumerator->Clone("eNumerator");
  eNumerator->Add(meNumerator);

  eFakeRate = (TH2F*) eNumerator->Clone("eFakeRate");
  eFakeRate->Divide(eDenominator);
  eFakeRate->Smooth();
  
  // center bin histo
  eePtCentre  = (TH2F*) inputFile->Get("Pt_centre_W_to_e_jet_to_e");
  mePtCentre  = (TH2F*) inputFile->Get("Pt_centre_W_to_mu_jet_to_e");

  ePtCentre  = (TH2F*) eePtCentre->Clone("ePtCentre");
  ePtCentre->Reset("ICES");

  for(int iBinX = 0; iBinX < eePtCentre->GetNbinsX()+1; iBinX++){  
    for(int iBinY = 0; iBinY < eePtCentre->GetNbinsY()+1; iBinY++){  
      ePtCentre->SetBinContent(iBinX,iBinY,(eePtCentre->GetBinContent(iBinX,iBinY)+mePtCentre->GetBinContent(iBinX,iBinY))/2);
    }
  }
  
  electronFakeRate = new TGraph2D();

  int nPoint = 0;

  for(int iBinX = 0; iBinX <= ePtCentre->GetNbinsX()+1; iBinX++){
    for(int iBinY = 0; iBinY <= ePtCentre->GetNbinsY()+1; iBinY++){

      float etaPoint = 0;
      float ptPoint  = 0;
      float valuePoint = 0;

      nPoint++;

      if(iBinX == 0)
	etaPoint = -0.1 ;
      if(iBinX == ePtCentre->GetNbinsX()+1)
	etaPoint = upperEtaBound ;
      else
	etaPoint = eFakeRate->GetXaxis()->GetBinCenter(iBinX);

      if(iBinY == 0)
	ptPoint = 0 ;
      if(iBinY == ePtCentre->GetNbinsY()+1)
	ptPoint = upperPtBound ;
      else{
	if(iBinX == 0)
	  ptPoint = ePtCentre->GetBinContent(iBinX+1,iBinY);
	else if(iBinX == ePtCentre->GetNbinsX()+1)
	  ptPoint = ePtCentre->GetBinContent(iBinX-1,iBinY);
	else
	  ptPoint = ePtCentre->GetBinContent(iBinX,iBinY);
      }

     
      if(iBinX == 0 and iBinY == 0){
	valuePoint = eFakeRate->GetBinContent(iBinX+1,iBinY+1);
      }
      else if(iBinY == ePtCentre->GetNbinsY()+1 and iBinX == ePtCentre->GetNbinsX()+1 ){
	valuePoint = eFakeRate->GetBinContent(iBinX-1,iBinY-1);
      }
      else if(iBinX == 0 and iBinY == ePtCentre->GetNbinsY()+1){
	valuePoint = eFakeRate->GetBinContent(iBinX+1,iBinY-1);
      }
     else if(iBinY == 0 and iBinX == ePtCentre->GetNbinsX()+1){
	valuePoint = eFakeRate->GetBinContent(iBinX-1,iBinY+1);
      }

      else if(iBinX == 0){
	valuePoint = eFakeRate->GetBinContent(iBinX+1,iBinY);
      }
      else if(iBinY == 0){
	valuePoint = eFakeRate->GetBinContent(iBinX,iBinY+1);
      }
      else if(iBinX == ePtCentre->GetNbinsX()+1 ){
	valuePoint = eFakeRate->GetBinContent(iBinX-1,iBinY);
      }
      else if(iBinY == ePtCentre->GetNbinsY()+1){
	valuePoint = eFakeRate->GetBinContent(iBinX,iBinY-1);
      }
      
      else {
	valuePoint = eFakeRate->GetBinContent(iBinX,iBinY);
      }

      electronFakeRate->SetPoint(nPoint,etaPoint,ptPoint,valuePoint);

    }
  }

  
  // muon sector
  
  mmDenominator = (TH2F*) inputFile->Get("Denominator_W_to_mu_jet_to_mu");
  emDenominator = (TH2F*) inputFile->Get("Denominator_W_to_e_jet_to_mu");
  mDenominator  = (TH2F*) mmDenominator->Clone("mDenominator");
  mDenominator->Add(emDenominator);

  mmNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_mu_jet_to_mu");
  emNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_e_jet_to_mu");
  mNumerator  = (TH2F*) mmNumerator->Clone("mNumerator");
  mNumerator->Add(emNumerator);
  
  mFakeRate = (TH2F*) mNumerator->Clone("mFakeRate");
  mFakeRate->Divide(mDenominator);
  mFakeRate->Smooth();

  // take the pt center for each bin
  emPtCentre  = (TH2F*) inputFile->Get("Pt_centre_W_to_e_jet_to_mu");
  mmPtCentre  = (TH2F*) inputFile->Get("Pt_centre_W_to_mu_jet_to_mu");

  mPtCentre   = (TH2F*) mmPtCentre->Clone("mPtCentre");
  mPtCentre->Reset("ICES");
  
  for(int iBinX = 0; iBinX < mmPtCentre->GetNbinsX()+1; iBinX++){  
    for(int iBinY = 0; iBinY < mmPtCentre->GetNbinsY()+1; iBinY++){  
      mPtCentre->SetBinContent(iBinX,iBinY,(mmPtCentre->GetBinContent(iBinX,iBinY)+emPtCentre->GetBinContent(iBinX,iBinY))/2);
    }
  }


  muonFakeRate = new TGraph2D();

  nPoint = 0;

  for(int iBinX = 0; iBinX <= mPtCentre->GetNbinsX()+1; iBinX++){
    for(int iBinY = 0; iBinY <= mPtCentre->GetNbinsY()+1; iBinY++){

      float etaPoint = 0;
      float ptPoint  = 0;
      float valuePoint = 0;

      nPoint++;

      if(iBinX == 0)
	etaPoint = -0.1 ;
      if(iBinX == mPtCentre->GetNbinsX()+1)
	etaPoint = upperEtaBound ;
      else
	etaPoint = mFakeRate->GetXaxis()->GetBinCenter(iBinX);

      if(iBinY == 0)
	ptPoint = 0 ;
      if(iBinY == mPtCentre->GetNbinsY()+1)
	ptPoint = upperPtBound ;
      else{
	if(iBinX == 0)
	  ptPoint = mPtCentre->GetBinContent(iBinX+1,iBinY);
	else if(iBinX == mPtCentre->GetNbinsX()+1)
	  ptPoint = mPtCentre->GetBinContent(iBinX-1,iBinY);
	else
	  ptPoint = mPtCentre->GetBinContent(iBinX,iBinY);
      }

      if(iBinX == 0 and iBinY == 0){
	valuePoint = mFakeRate->GetBinContent(iBinX+1,iBinY+1);
      }
      else if(iBinY == mPtCentre->GetNbinsY()+1 and iBinX == mPtCentre->GetNbinsX()+1 ){
	valuePoint = mFakeRate->GetBinContent(iBinX-1,iBinY-1);
      }
      else if(iBinX == 0 and iBinY == mPtCentre->GetNbinsY()+1){
	valuePoint = mFakeRate->GetBinContent(iBinX+1,iBinY-1);
      }
     else if(iBinY == 0 and iBinX == mPtCentre->GetNbinsX()+1){
	valuePoint = mFakeRate->GetBinContent(iBinX-1,iBinY+1);
      }

      else if(iBinX == 0){
	valuePoint = mFakeRate->GetBinContent(iBinX+1,iBinY);
      }
      else if(iBinY == 0){
	valuePoint = mFakeRate->GetBinContent(iBinX,iBinY+1);
      }
      else if(iBinX == mPtCentre->GetNbinsX()+1 ){
	valuePoint = mFakeRate->GetBinContent(iBinX-1,iBinY);
      }
      else if(iBinY == mPtCentre->GetNbinsY()+1){
	valuePoint = mFakeRate->GetBinContent(iBinX,iBinY-1);
      }
      
      else {
	valuePoint = mFakeRate->GetBinContent(iBinX,iBinY);
      }

      muonFakeRate->SetPoint(nPoint,etaPoint,ptPoint,valuePoint);

    }
  }
  
}

float fakeRateContainer::getFakeRate (const int & PID, const float & pt, const float & eta){

  if(fabs(PID) == 11){ // electron case                                                                                                                                        
    return electronFakeRate->Interpolate(fabs(eta),pt);
  }
  else if (fabs(PID) == 13){ // electron case                                                                                                                                  
    return muonFakeRate->Interpolate(fabs(eta),pt);
  }
  else return 1 ;

}

float fakeRateContainer::getFakeRateUncertainty (const int & PID, const float & pt, const float & eta){

  if(fabs(PID) == 11){ // electron case                                                                                                                                        
    return mFakeRate->GetBinError(eFakeRate->GetXaxis()->FindBin(fabs(eta)),eFakeRate->GetYaxis()->FindBin(pt));
  }
  else if (fabs(PID) == 13){ // electron case                                                                                                                                  
    return mFakeRate->GetBinError(mFakeRate->GetXaxis()->FindBin(fabs(eta)),mFakeRate->GetYaxis()->FindBin(pt));
  }
  else return 1 ;

}

fakeMigrationContainer::~fakeMigrationContainer(){

  if(eeBarrel !=0)
    eeBarrel->Delete();
  if(meBarrel !=0)
    meBarrel->Delete();
  if(eBarrel !=0)
    eBarrel->Delete();
  if(eBarrelProfile !=0)
    eBarrelProfile->Delete();

  if(eeEndcap !=0)
    eeEndcap->Delete();
  if(meEndcap !=0)
    meEndcap->Delete();
  if(eEndcap !=0)
    eEndcap->Delete();
  if(eEndcapProfile !=0)
    eEndcapProfile->Delete();

  if(mmBarrel !=0)
    mmBarrel->Delete();
  if(emBarrel !=0)
    emBarrel->Delete();
  if(mBarrel !=0)
    mBarrel->Delete();
  if(mBarrelProfile !=0)
    mBarrelProfile->Delete();

  if(mmEndcap !=0)
    mmEndcap->Delete();
  if(emEndcap !=0)
    emEndcap->Delete();
  if(mEndcap !=0)
    mEndcap->Delete();
  if(mEndcapProfile !=0)
    mEndcapProfile->Delete();

  if(mePtMigrationCentre!=0)
    mePtMigrationCentre->Delete();
  if(mmPtMigrationCentre!=0)
    mmPtMigrationCentre->Delete();
  if(emPtMigrationCentre!=0)
    emPtMigrationCentre->Delete();
  if(eePtMigrationCentre!=0)
    eePtMigrationCentre->Delete();

  if(ePtMigrationCentre!=0)
    ePtMigrationCentre->Delete();
  if(mPtMigrationCentre!=0)
    mPtMigrationCentre->Delete();

}

fakeMigrationContainer::fakeMigrationContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

  cout<<"start to build the fake rate migration container"<<endl;

  eeBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_e");
  meBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_mu_jet_to_e");
  eBarrel  = (TH2F*) eeBarrel->Clone("eBarrel");
  eBarrel->Add(meBarrel);
  eBarrel->Smooth();
  eBarrelProfile = eBarrel->ProfileX("_fX");

  eeEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_e");
  meEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_mu_jet_to_e");
  eEndcap  = (TH2F*) eeEndcap->Clone("eEndcap");
  eEndcap->Add(meEndcap);
  eEndcap->Smooth();
  eEndcapProfile = eEndcap->ProfileX("_fX");

  mePtMigrationCentre = (TH1F*) inputFile->Get("Pt_migration_centre_W_to_mu_jet_to_e");
  eePtMigrationCentre = (TH1F*) inputFile->Get("Pt_migration_centre_W_to_e_jet_to_e");

  ePtMigrationCentre   = (TH1F*) eePtMigrationCentre->Clone("ePtMigrationCentre");
  ePtMigrationCentre->Reset("ICES");

  for(int iBinX = 0; iBinX <= eePtMigrationCentre->GetNbinsX()+1; iBinX++){  
      ePtMigrationCentre->SetBinContent(iBinX,(eePtMigrationCentre->GetBinContent(iBinX)+mePtMigrationCentre->GetBinContent(iBinX))/2);    
  }

  electronBarrel = new TGraph();
  electronEndcap = new TGraph();

  for(int iBin = 0; iBin < eBarrelProfile->GetNbinsX(); iBin++){
    electronBarrel->SetPoint(iBin+1,ePtMigrationCentre->GetBinContent(iBin+1),eBarrelProfile->GetBinContent(iBin+1));
  }

  for(int iBin = 0; iBin < eEndcapProfile->GetNbinsX(); iBin++){
    electronEndcap->SetPoint(iBin+1,ePtMigrationCentre->GetBinContent(iBin+1),eEndcapProfile->GetBinContent(iBin+1));
  }

  mmBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_mu_jet_to_mu");
  emBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_mu");
  mBarrel  = (TH2F*) mmBarrel->Clone("mBarrel");
  mBarrel->Add(emBarrel);
  mBarrel->Smooth();
  mBarrelProfile = mBarrel->ProfileX("_fX");

  mmEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_mu_jet_to_mu");
  emEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_mu");
  mEndcap  = (TH2F*) mmEndcap->Clone("mEndcap");
  mEndcap->Add(emEndcap);
  mEndcap->Smooth();
  mEndcapProfile = mEndcap->ProfileX("_fX");


  mmPtMigrationCentre = (TH1F*) inputFile->Get("Pt_migration_centre_W_to_mu_jet_to_mu");
  emPtMigrationCentre = (TH1F*) inputFile->Get("Pt_migration_centre_W_to_e_jet_to_mu");

  mPtMigrationCentre  = (TH1F*) mmPtMigrationCentre->Clone("muPtMigrationCentre");
  mPtMigrationCentre->Reset("ICES");

  for(int iBinX = 0; iBinX < mmPtMigrationCentre->GetNbinsX()+1; iBinX++){  
    mPtMigrationCentre->SetBinContent(iBinX,(mmPtMigrationCentre->GetBinContent(iBinX)+emPtMigrationCentre->GetBinContent(iBinX))/2);
  }

  muonBarrel = new TGraph();
  muonEndcap = new TGraph();

  for(int iBin = 0; iBin < mBarrelProfile->GetNbinsX(); iBin++){
    muonBarrel->SetPoint(iBin+1,mPtMigrationCentre->GetBinContent(iBin+1),mBarrelProfile->GetBinContent(iBin+1));
  }

  for(int iBin = 0; iBin < mEndcapProfile->GetNbinsX(); iBin++){
    muonEndcap->SetPoint(iBin+1,mPtMigrationCentre->GetBinContent(iBin+1),mEndcapProfile->GetBinContent(iBin+1));
  }
}

float fakeMigrationContainer::getMigration (const int & PID, const float & pt, const float & eta){

  if(fabs(PID) == 11 and fabs(eta) < 1.5 ){ // electron case                                                                                                                   
    return electronBarrel->Eval(pt);
  }
  if(fabs(PID) == 11 and fabs(eta) >= 1.5 ){ // electron case                                                                                                                  
    return electronBarrel->Eval(pt);
  }

  if(fabs(PID) == 13 and fabs(eta) < 1.5 ){ // electron case                                                                                                                   
    return muonBarrel->Eval(pt);
  }
  if(fabs(PID) == 13 and fabs(eta) >= 1.5 ){ // electron case                                                                                                                  
    return muonEndcap->Eval(pt);
  }
  else return 1;

}

int ReadInputSampleFile(const string & InputSampleList, map<string,vector<sampleContainer> > & SampleContainer){


  ifstream inputFile (InputSampleList.c_str());
  string buffer;

  if(inputFile.fail()) return -1; 

  vector<string> NameSample;
  vector<string> NameReducedSample;
  vector<int>         ColorSample;
  vector<float>       SampleCrossSection;
  vector<int>         NumEntriesBefore;
  vector<int>         isSignal;

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
