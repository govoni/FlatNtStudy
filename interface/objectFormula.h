#ifndef objectFormula_h
#define objectFormula_h

#include <iostream>
#include <string>

#include "TString.h"
#include "TFormula.h"

using namespace std;

class objectFormula {

 public :

  objectFormula(){};

  ~objectFormula(){
    if(!muonScaleUnc) muonScaleUnc->Delete();
    if(!muonResolutionUnc) muonResolutionUnc->Delete();
    if(!electronScaleUnc) electronScaleUnc->Delete();
    if(!electronResolutionUnc) electronResolutionUnc->Delete();
    if(!jetScaleUnc) jetScaleUnc->Delete();
    if(!jetResolutionUnc) jetResolutionUnc->Delete();
  };

 objectFormula(const string & configurationName): // given a string it tries to setup values defined in the cc file
  configurationName(configurationName){
    setFormula();
  };

  void setFormula();

  float evaluateMuonScaleUnc(float pt, float eta);
  float evaluateMuonResolutionUnc(float pt, float eta, float energy);
  float evaluateElectronScaleUnc(float pt, float eta);
  float evaluateElectronResolutionUnc(float pt, float eta, float energy);
  float evaluateJetScaleUnc(float pt, float eta);
  float evaluateJetResolutionUnc(float pt, float eta, float energy);

 private :

  string configurationName;

  TFormula* muonScaleUnc ;
  TFormula* muonResolutionUnc ;
  TFormula* electronScaleUnc ;
  TFormula* electronResolutionUnc ;
  TFormula* jetScaleUnc ;
  TFormula* jetResolutionUnc ;

};


#endif
