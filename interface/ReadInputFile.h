#ifndef ReadInpinutFile_h
#define ReadInpinutFile_h

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

#include "TString.h"

using namespace std;

// sample container class                                                                                                                                             
class sampleContainer {
 public :
  sampleContainer(){};
  ~sampleContainer(){};

 sampleContainer(string sampleName, int color, double xsec, int numBefore, int isSignal):
  sampleName(sampleName),
    color(color),
    xsec(xsec),
    numBefore(numBefore),
    isSignal(isSignal){};

  string sampleName ;
  int    color;
  double xsec;
  int numBefore;
  int isSignal;

};

class cutContainer {

 public :
  cutContainer(){};
  ~cutContainer(){};

 cutContainer(string cutLayerName, 
              pair<double,double> ptL, 
              double  chargeSign, 
              double  flavour,
              int     nLep, 
              int     nextra,             
              double  MET, 
              pair<double,double> ptJet, 
              double DetaJJ, 
              double Mjj, 
              double DetaLL, 
              pair<double,double> Mll, 
              pair<double,double> MllZVeto, 
              double bTagVeto, double jetPUID ):
    cutLayerName(cutLayerName),
      ptL( ptL),
      chargeSign(chargeSign),
      flavour(flavour),
      nLep(nLep),
      nextra(nextra),
      MET(MET),
      ptJet(ptJet),
      DetaJJ(DetaJJ),
      Mjj(Mjj),
      DetaLL(DetaLL),
      Mll(Mll),
      MllZVeto(MllZVeto),
      bTagVeto(bTagVeto),
      jetPUID(jetPUID){};

  string cutLayerName ;
  pair<double,double> ptL;
  double  chargeSign;
  double  flavour;
  int     nLep;
  int     nextra;
  double  MET;
  pair<double,double> ptJet;  
  double  DetaJJ;
  double  Mjj;
  double  DetaLL;
  pair<double,double>  Mll;
  pair<double,double>  MllZVeto;
  double  bTagVeto;
  double  jetPUID;
};

class variableContainer {

 public :

  variableContainer(){};
  ~variableContainer(){};

 variableContainer(string variableName, int Nbin, double min, double max, string label):
  variableName(variableName),
    Nbin(Nbin),
    min(min),
    max(max),
    label(label){}; 

  string variableName ;
  int Nbin ;
  double min;
  double max;
  string label;

};

class trainingContainer {

 public: 

  trainingContainer(){};
  ~trainingContainer(){};

 trainingContainer(string fileName, string varNameReduced, pair<int,int> puBin, vector<string> methodName):
  fileName(fileName),
    varNameReduced(varNameReduced),
    puBin(puBin),
    methodName(methodName)
  {};

  string fileName;
  string varNameReduced;
  pair<int,int> puBin;
  vector<string> methodName;

};

int ReadInputSampleFile   (const string & , map<string, vector<sampleContainer> > & );

int ReadInputVariableFile (const string & , vector<variableContainer> & );

int ReadInputVariableFile (const string & , vector<string> & );

int ReadInputTrainingFile (const string & , vector<trainingContainer> & );

int ReadInputCutFile      (const string & , vector<cutContainer> &);

#endif
