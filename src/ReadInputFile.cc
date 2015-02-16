#include "ReadInputFile.h"

using namespace std;

#define upperPtBound 3000
#define upperEtaBound 5.5

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

}

fakeRateContainer::fakeRateContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

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

  // trick for a correct TGraph2D interpolation
  float* ebinningX = new float[eFakeRate->GetNbinsX()+2];
  for(int iBinX = 1; iBinX <= eFakeRate->GetNbinsX()+1; iBinX++){
    if(iBinX == 1)
      ebinningX[iBinX-1] = eFakeRate->GetXaxis()->GetBinLowEdge(iBinX)-eFakeRate->GetXaxis()->GetBinWidth(iBinX);
    else if(iBinX == eFakeRate->GetNbinsX())
      ebinningX[iBinX-1] = eFakeRate->GetXaxis()->GetBinLowEdge(iBinX)+eFakeRate->GetXaxis()->GetBinWidth(iBinX);
    else 
      ebinningX[iBinX-1] = eFakeRate->GetXaxis()->GetBinLowEdge(iBinX);
  }
  ebinningX[eFakeRate->GetNbinsX()+1] = upperEtaBound;

  float* ebinningY = new float[eFakeRate->GetNbinsY()+2];
  for(int iBinY = 1; iBinY <= eFakeRate->GetNbinsY()+1; iBinY++){
    if(iBinY == 1)
      ebinningY[iBinY-1] = eFakeRate->GetYaxis()->GetBinLowEdge(iBinY)-eFakeRate->GetYaxis()->GetBinWidth(iBinY);
    else if(iBinY == eFakeRate->GetNbinsY())
      ebinningY[iBinY-1] = eFakeRate->GetYaxis()->GetBinLowEdge(iBinY)+eFakeRate->GetYaxis()->GetBinWidth(iBinY);
    else 
      ebinningY[iBinY-1] = eFakeRate->GetYaxis()->GetBinLowEdge(iBinY);
  }

  ebinningY[eFakeRate->GetNbinsY()+1] = upperPtBound;

  TH2F* eFakeRateTemp = new TH2F("eFakeRateTemp","",eFakeRate->GetNbinsX()+1,ebinningX,eFakeRate->GetNbinsY()+1,ebinningY);

  for(int iBinX = 0; iBinX < eFakeRateTemp->GetNbinsX(); iBinX++){    
    for(int iBinY = 0; iBinY < eFakeRateTemp->GetNbinsY(); iBinY++){
      eFakeRateTemp->SetBinContent(iBinX+1,iBinY+1,eFakeRate->GetBinContent(iBinX+1,iBinY+1));
    }
  }

  for(int iBinX = 0; iBinX < eFakeRateTemp->GetNbinsX(); iBinX++){    
    eFakeRateTemp->SetBinContent(iBinX+1,eFakeRateTemp->GetNbinsY(),eFakeRateTemp->GetBinContent(iBinX+1,eFakeRateTemp->GetNbinsY()-1));
  }				 

  for(int iBinY = 0; iBinY < eFakeRateTemp->GetNbinsY(); iBinY++){    
    eFakeRateTemp->SetBinContent(eFakeRateTemp->GetNbinsX(),iBinY+1,eFakeRateTemp->GetBinContent(eFakeRateTemp->GetNbinsX()-1,iBinY+1));
  }				 

  
  electronFakeRate = new TGraph2D(eFakeRateTemp);
  
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

  // trick for a correct TGraph2D interpolation
  float* mbinningX = new float[mFakeRate->GetNbinsX()+2];
  for(int iBinX = 1; iBinX <= mFakeRate->GetNbinsX()+1; iBinX++){
    if(iBinX == 1)
      mbinningX[iBinX-1] = mFakeRate->GetXaxis()->GetBinLowEdge(iBinX)-mFakeRate->GetXaxis()->GetBinWidth(iBinX);
    else if(iBinX == mFakeRate->GetNbinsX())
      mbinningX[iBinX-1] = mFakeRate->GetXaxis()->GetBinLowEdge(iBinX)+mFakeRate->GetXaxis()->GetBinWidth(iBinX);
    else 
      mbinningX[iBinX-1] = mFakeRate->GetXaxis()->GetBinLowEdge(iBinX);
  }

  mbinningX[mFakeRate->GetNbinsX()+1] = upperEtaBound;

  float* mbinningY = new float[mFakeRate->GetNbinsY()+2];
  for(int iBinY = 1; iBinY <= mFakeRate->GetNbinsY()+1; iBinY++){
    if(iBinY == 1)
      mbinningY[iBinY-1] = mFakeRate->GetYaxis()->GetBinLowEdge(iBinY)-mFakeRate->GetYaxis()->GetBinWidth(iBinY);
    else if(iBinY == mFakeRate->GetNbinsY())
      mbinningY[iBinY-1] = mFakeRate->GetYaxis()->GetBinLowEdge(iBinY)+mFakeRate->GetYaxis()->GetBinWidth(iBinY);
    else 
      mbinningY[iBinY-1] = mFakeRate->GetYaxis()->GetBinLowEdge(iBinY);
  }

  mbinningY[mFakeRate->GetNbinsY()+1] = upperPtBound;

  TH2F* mFakeRateTemp = new TH2F("mFakeRateTemp","",mFakeRate->GetNbinsX()+1,mbinningX,mFakeRate->GetNbinsY()+1,mbinningY);

  for(int iBinX = 0; iBinX < mFakeRateTemp->GetNbinsX(); iBinX++){    
    for(int iBinY = 0; iBinY < mFakeRateTemp->GetNbinsY(); iBinY++){
      mFakeRateTemp->SetBinContent(iBinX+1,iBinY+1,mFakeRate->GetBinContent(iBinX+1,iBinY+1));
    }
  }

  for(int iBinX = 0; iBinX < mFakeRateTemp->GetNbinsX(); iBinX++){    
    mFakeRateTemp->SetBinContent(iBinX+1,mFakeRateTemp->GetNbinsY(),mFakeRateTemp->GetBinContent(iBinX+1,mFakeRateTemp->GetNbinsY()-1));
  }				 

  for(int iBinY = 0; iBinY < mFakeRateTemp->GetNbinsY(); iBinY++){    
    mFakeRateTemp->SetBinContent(mFakeRateTemp->GetNbinsX(),iBinY+1,mFakeRateTemp->GetBinContent(mFakeRateTemp->GetNbinsX()-1,iBinY+1));
  }				 

  muonFakeRate = new TGraph2D(mFakeRateTemp);

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

}

fakeMigrationContainer::fakeMigrationContainer(const string & fileName){

  if (fileName == "")
    inputFile = TFile::Open("input/FakeRate.root","READ");
  else
    inputFile = TFile::Open(fileName.c_str(),"READ");

  eeBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_e");
  meBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_mu_jet_to_e");
  eBarrel  = (TH2F*) eeBarrel->Clone("eBarrel");
  eBarrel->Add(meBarrel);
  eBarrelProfile = eBarrel->ProfileX("_fX");
  electronBarrel = new TGraph(eBarrelProfile);

  eeEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_e");
  meEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_mu_jet_to_e");
  eEndcap  = (TH2F*) eeEndcap->Clone("eEndcap");
  eEndcap->Add(meEndcap);
  eEndcapProfile = eEndcap->ProfileX("_fX");
  electronEndcap = new TGraph(eEndcapProfile);

  mmBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_mu_jet_to_mu");
  emBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_mu");
  mBarrel  = (TH2F*) mmBarrel->Clone("mBarrel");
  mBarrel->Add(emBarrel);
  mBarrelProfile = mBarrel->ProfileX("_fX");
  muonBarrel = new TGraph(mBarrelProfile);

  mmEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_mu_jet_to_mu");
  emEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_mu");
  mEndcap  = (TH2F*) mmEndcap->Clone("mEndcap");
  mEndcap->Add(emEndcap);
  mEndcapProfile = mEndcap->ProfileX("_fX");
  muonEndcap = new TGraph(mEndcapProfile);


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

    string layerName, ptL1, ptL2, chargeSign, flavour, nLep, nextra, MET, ptJet1, ptJet2, DetaJJ, Mjj, DetaLL, MllMin, MllMax,  MllZVetoMin, MllZVetoMax, bTagCut, nBVeto, nSoftMu, jetPUID, polarization, etaMaxL;    

    line >> layerName >> ptL1 >> ptL2 >> etaMaxL >> chargeSign >> flavour >> nLep >> nextra >> MET >> ptJet1 >> ptJet2 >> DetaJJ >> Mjj >> DetaLL >> MllMin >> MllMax >>  MllZVetoMin >> MllZVetoMax >> bTagCut >> nBVeto >> nSoftMu >> jetPUID >> polarization;     

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
