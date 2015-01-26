#include "ReadInputFile.h"

using namespace std;

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
