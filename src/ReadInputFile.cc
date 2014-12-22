#include "ReadInputFile.h"

using namespace std;

int ReadInputSampleFile(const std::string & InputSampleList, std::map<std::string,std::vector<sampleContainer> > & SampleContainer){


  std::ifstream inputFile (InputSampleList.c_str());
  std::string buffer;

  if(inputFile.fail()) return -1; 

  std::vector<std::string> NameSample;
  std::vector<std::string> NameReducedSample;
  std::vector<int>         ColorSample;
  std::vector<double>      SampleCrossSection;
  std::vector<int>         NumEntriesBefore;
  std::vector<int>         isSignal;

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" " ) continue ;
    std::stringstream line(buffer);

    std::string  NameSampleTemp;
    std::string  NameReducedSampleTemp;
    std::string  ColorSampleTemp;
    std::string  SampleCrossSectionTemp;
    std::string  NumEntresBeforeTemp;
    std::string  isSignalTemp;

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


  return SampleContainer.size() ;

}


int ReadInputVariableFile( const std::string & InputVariableList , std::vector<variableContainer> & varContainer){


  std::ifstream inputFile (InputVariableList.c_str());
  std::string buffer;

  if(inputFile.fail()) return -1; 
  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;
    std::stringstream line(buffer);

    std::string  VariablesTemp;
    std::string  VariablesNbinTemp;
    std::string  VariablesMinValueTemp;
    std::string  VariablesMaxValueTemp;
    std::string  VariablesTitleTemp;
    
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

int ReadInputCutFile( const std::string & InputCutList , std::vector<cutContainer> & CutContainer){


  std::ifstream inputFile (InputCutList.c_str());
  std::string buffer;

  if(inputFile.fail()) return -1; 

  while(!inputFile.eof()){
  
    getline(inputFile,buffer);

    if(buffer.empty() || !buffer.find("#") || buffer==" ") continue ;

    std::stringstream line(buffer);      

    std::string layerName, ptL1, ptL2, chargeSign, flavour, nLep, nextra, MET, ptJet1, ptJet2, DetaJJ, Mjj, MllMin, MllMax,  MllZVetoMin, MllZVetoMax, bTagVeto, jetPUID;    

    line >> layerName >> ptL1 >> ptL2 >> chargeSign >> flavour >> nLep >> nextra >> MET >> ptJet1 >> ptJet2 >> DetaJJ >> Mjj >> MllMin >> MllMax >>  MllZVetoMin >> MllZVetoMax >> bTagVeto >> jetPUID;     

    cutContainer dummy(layerName,
                       std::make_pair(stod(ptL1),stod(ptL2)),
                       stod(chargeSign),
                       stod(flavour),
                       stoi(nLep),
                       stoi(nextra),
                       stod(MET),
                       std::make_pair(stod(ptJet1),stod(ptJet2)),
                       stod(DetaJJ),
                       stod(Mjj),
                       std::make_pair(stod(MllMin),stod(MllMax)),
                       std::make_pair(stod(MllZVetoMin),stod(MllZVetoMax)),
                       stod(bTagVeto), 
                       stod(jetPUID));

    
    CutContainer.push_back(dummy);

  }

  return CutContainer.size() ;

}
