#include "ReadInputFile.h"

using namespace std;

fakeRateContainer::~fakeRateContainer(){

  if(inputFile != 0)
    inputFile->Delete() ;

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

}

fakeRateContainer::fakeRateContainer(const string & fileName){

  inputFile = TFile::Open(fileName.c_str(),"READ");

  eeDenominator = (TH2F*) inputFile->Get("Denominator_W_to_e_jet_to_e");
  meDenominator = (TH2F*) inputFile->Get("Denominator_W_to_m_jet_to_e");
  eDenominator  = (TH2F*) eeDenominator->Clone("eDenominator");
  eDenominator->Add(meDenominator);

  eeNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_e_jet_to_e");
  meNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_m_jet_to_e");
  eNumerator  = (TH2F*) eeNumerator->Clone("eNumerator");
  eNumerator->Add(meNumerator);

  eFakeRate = (TH2F*) eNumerator->Clone("eFakeRate");
  eFakeRate->Divide(eDenominator);

  mmDenominator = (TH2F*) inputFile->Get("Denominator_W_to_m_jet_to_m");
  emDenominator = (TH2F*) inputFile->Get("Denominator_W_to_e_jet_to_m");
  mDenominator  = (TH2F*) mmDenominator->Clone("mDenominator");
  mDenominator->Add(emDenominator);

  mmNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_m_jet_to_m");
  emNumerator = (TH2F*) inputFile->Get("Jet_Numerator_W_to_e_jet_to_m");
  mNumerator  = (TH2F*) mmNumerator->Clone("mNumerator");
  mNumerator->Add(emNumerator);

  mFakeRate = (TH2F*) emNumerator->Clone("mFakeRate");
  mFakeRate->Divide(mDenominator);
}

float fakeRateContainer::getFakeRate (const int & PID, const float & pt, const float & eta){

  if(fabs(PID) == 11){ // electron case                                                                                                                                        
    return eFakeRate->GetBinContent(eFakeRate->FindBin(fabs(eta)),eFakeRate->FindBin(pt));
  }
  else if (fabs(PID) == 13){ // electron case                                                                                                                                  
    return mFakeRate->GetBinContent(mFakeRate->FindBin(fabs(eta)),mFakeRate->FindBin(pt));
  }
  else return 1 ;



}

fakeMigration::~fakeMigration(){

  if(inputFile !=0)
    inputFile->Delete();

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

fakeMigration::fakeMigration(const string & fileName){

  inputFile = TFile::Open(fileName.c_str(),"READ");

  eeBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_e");
  meBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_m_jet_to_e");
  eBarrel  = (TH2F*) eeBarrel->Clone("eBarrel");
  eBarrel->Add(meBarrel);
  eBarrelProfile = eBarrel->ProfileX("_pfX");

  eeEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_e");
  meEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_m_jet_to_e");
  eEndcap  = (TH2F*) eEndcap->Clone("eEndcap");
  eEndcap->Add(meEndcap);
  eEndcapProfile = eEndcap->ProfileX("_pfX");

  mmBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_m_jet_to_m");
  emBarrel = (TH2F*) inputFile->Get("Pt_migration_barrel_W_to_e_jet_to_m");
  mBarrel  = (TH2F*) mmBarrel->Clone("mBarrel");
  mBarrel->Add(emBarrel);
  mBarrelProfile = mBarrel->ProfileX("_pfX");

  mmEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_m_jet_to_m");
  emEndcap = (TH2F*) inputFile->Get("Pt_migration_endcap_W_to_e_jet_to_m");
  mEndcap  = (TH2F*) mmEndcap->Clone("mEndcap");
  mEndcap->Add(emEndcap);
  mEndcapProfile = mEndcap->ProfileX("_pfX");

}

float fakeMigration::getMigration (const int & PID, const float & pt, const float & eta){

  if(fabs(PID) == 11 and fabs(eta) < 1.5 ){ // electron case                                                                                                                   
    return eBarrelProfile->GetBinContent(eBarrelProfile->FindBin(pt));
  }
  if(fabs(PID) == 11 and fabs(eta) >= 1.5 ){ // electron case                                                                                                                  
    return eEndcapProfile->GetBinContent(eEndcapProfile->FindBin(pt));
  }

  if(fabs(PID) == 13 and fabs(eta) < 1.5 ){ // electron case                                                                                                                   
    return mBarrelProfile->GetBinContent(mBarrelProfile->FindBin(pt));
  }
  if(fabs(PID) == 13 and fabs(eta) >= 1.5 ){ // electron case                                                                                                                  
    return mEndcapProfile->GetBinContent(mEndcapProfile->FindBin(pt));
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
  vector<double>      SampleCrossSection;
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
                             stod(VariablesMinValueTemp),
                             stod(VariablesMaxValueTemp),
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
			       stod(VariablesMinValueTempX),
			       stod(VariablesMaxValueTempX),
			       VariablesTitleTempX,
			       VariablesTempY,
			       stoi(VariablesNbinTempY),
			       stod(VariablesMinValueTempY),
			       stod(VariablesMaxValueTempY),
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

    string layerName, ptL1, ptL2, chargeSign, flavour, nLep, nextra, MET, ptJet1, ptJet2, DetaJJ, Mjj, DetaLL, MllMin, MllMax,  MllZVetoMin, MllZVetoMax, bTagVeto, jetPUID, polarization;    

    line >> layerName >> ptL1 >> ptL2 >> chargeSign >> flavour >> nLep >> nextra >> MET >> ptJet1 >> ptJet2 >> DetaJJ >> Mjj >> DetaLL >> MllMin >> MllMax >>  MllZVetoMin >> MllZVetoMax >> bTagVeto >> jetPUID >> polarization;     

    cutContainer dummy(layerName,
                       make_pair(stod(ptL1),stod(ptL2)),
                       stod(chargeSign),
                       stod(flavour),
                       stoi(nLep),
                       stoi(nextra),
                       stod(MET),
                       make_pair(stod(ptJet1),stod(ptJet2)),
                       stod(DetaJJ),
                       stod(Mjj),
                       stod(DetaLL),
                       make_pair(stod(MllMin),stod(MllMax)),
                       make_pair(stod(MllZVetoMin),stod(MllZVetoMax)),
                       stod(bTagVeto), 
                       stod(jetPUID),
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
