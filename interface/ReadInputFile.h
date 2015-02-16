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
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TProfile.h"
#include "TGraph2D.h"
#include "TGraph.h"

using namespace std;

// sample container class                                                                                                                                             
class sampleContainer {
 public :
  sampleContainer(){};
  ~sampleContainer(){};

 sampleContainer(string sampleName, int color, float xsec, int numBefore, int isSignal):
  sampleName(sampleName),
    color(color),
    xsec(xsec),
    numBefore(numBefore),
    isSignal(isSignal){};


  bool operator < (const sampleContainer & container2) const {
    if(sampleName < container2.sampleName) return true;
    else return false;    
  }

  bool operator == (const sampleContainer & container2) const {
    if(sampleName == container2.sampleName and xsec == container2.xsec and isSignal == container2.isSignal) return true;
    else return false;    
  }
  
  string sampleName ;
  int    color;
  float xsec;
  int numBefore;
  int isSignal;

};

class cutContainer {

 public :
  cutContainer(){};
  ~cutContainer(){};

 cutContainer(string cutLayerName, 
              pair<float,float> ptL, 
	      float etaMaxL,
              float chargeSign, 
              float flavour,
              int   nLep, 
              int   nextra,             
              float MET, 
              pair<float,float> ptJet, 
              float DetaJJ, 
              float Mjj, 
              float DetaLL, 
              pair<float,float> Mll, 
              pair<float,float> MllZVeto, 
              float bTagCut, 
              int   nBVeto, 
	      int   nSoftMu,
	      float jetPUID,
              int   polarization = 99):
    cutLayerName(cutLayerName),
      ptL( ptL),
      etaMaxL(etaMaxL),
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
      bTagCut(bTagCut),
      nBVeto(nBVeto),
      nSoftMu(nSoftMu),
      jetPUID(jetPUID),
      polarization(polarization){};

  string cutLayerName ;
  pair<float,float> ptL;
  float  etaMaxL;
  float  chargeSign;
  float  flavour;
  int    nLep;
  int    nextra;
  float  MET;
  pair<float,float> ptJet;  
  float  DetaJJ;
  float  Mjj;
  float  DetaLL;
  pair<float,float>  Mll;
  pair<float,float>  MllZVeto;
  float  bTagCut;
  int    nBVeto;
  int    nSoftMu;
  float  jetPUID;
  int    polarization;

};

class variableContainer {

 public :

  variableContainer(){};
  ~variableContainer(){};

 variableContainer(string variableName, int Nbin, float min, float max, string label):
  variableName(variableName),
    Nbin(Nbin),
    min(min),
    max(max),
    label(label){}; 

  string variableName ;
  int Nbin ;
  float min;
  float max;
  string label;

};

class variableContainer2D {

 public :

  variableContainer2D(){};
  ~variableContainer2D(){};

 variableContainer2D(string variableNameX, int NbinX, float minX, float maxX, string labelX,
                     string variableNameY, int NbinY, float minY, float maxY, string labelY):
  variableNameX(variableNameX),
    NbinX(NbinX),
    minX(minX),
    maxX(maxX),
    labelX(labelX),
    variableNameY(variableNameY),
    NbinY(NbinY),
    minY(minY),
    maxY(maxY),
    labelY(labelY){}; 

  string variableNameX ;
  int NbinX ;
  float minX;
  float maxX;
  string labelX;

  string variableNameY ;
  int NbinY ;
  float minY;
  float maxY;
  string labelY;

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

// class in order to make plots                                                                                                                                               

class histoContainer {

 public :

  histoContainer(){};

  ~histoContainer(){};

 histoContainer(string cutName, variableContainer container):
  cutName(cutName),
    varName(container.variableName){
    histogram = new TH1F((cutName+"_"+varName).c_str(),"",container.Nbin,container.min,container.max);
    histogram->GetXaxis()->SetTitle(container.label.c_str());
    histogram->StatOverflows(1);
    histogram->Sumw2();
  }

  bool operator == (const histoContainer & plot2) const {
    if(plot2.cutName == cutName and plot2.varName == varName ) return true;
    else return false;
  }

  bool findCutByLabel ( const string & cutString){
    TString name = Form("%s",cutName.c_str());
    if(name.Contains(cutString.c_str())) return true;
    else return false;

  }

  string cutName;
  string varName;
  TH1F* histogram;

};

class fakeRateContainer {

 public :

  fakeRateContainer(){};

  ~fakeRateContainer();

  fakeRateContainer(const string & fileName);

  float getFakeRate (const int & PID, const float & pt, const float & eta);
  float getFakeRateUncertainty(const int&, const float&, const float&);

  TFile *inputFile ;

  TH2F* eeDenominator ;
  TH2F* meDenominator ;
  TH2F* eDenominator ;

  TH2F* eeNumerator ;
  TH2F* meNumerator ;
  TH2F* eNumerator ;

  TH2F* emDenominator ;
  TH2F* mmDenominator ;
  TH2F* mDenominator ;

  TH2F* emNumerator ;
  TH2F* mmNumerator ;
  TH2F* mNumerator ;

  TH2F* mFakeRate ;
  TGraph2D* muonFakeRate;
 
  TH2F* eFakeRate ;
  TGraph2D* electronFakeRate;


};

class fakeMigrationContainer {

 public :
  
  fakeMigrationContainer(){};
  
  ~fakeMigrationContainer();

  fakeMigrationContainer(const string & fileName);

  float getMigration (const int & PID, const float & pt, const float & eta);

  TFile* inputFile;

  TH2F* eeBarrel ;
  TH2F* meBarrel ;
  TH2F* eBarrel ;
  TProfile* eBarrelProfile ;
  TGraph* electronBarrel ;

  TH2F* eeEndcap ;
  TH2F* meEndcap ;
  TH2F* eEndcap ;
  TProfile* eEndcapProfile ;
  TGraph* electronEndcap ;

  TH2F* mmBarrel ;
  TH2F* emBarrel ;
  TH2F* mBarrel ;
  TProfile* mBarrelProfile ;
  TGraph *muonBarrel ;

  TH2F* mmEndcap ;
  TH2F* emEndcap ;
  TH2F* mEndcap ;
  TProfile* mEndcapProfile ;
  TGraph *muonEndcap ;


};

// functions

int ReadInputSampleFile   (const string & , map<string, vector<sampleContainer> > & );

int ReadInputVariableFile (const string & , vector<variableContainer> & );

int ReadInputVariableFile (const string & , vector<variableContainer2D> & );

int ReadInputVariableFile (const string & , vector<string> & );

int ReadInputTrainingFile (const string & , vector<trainingContainer> & );

int ReadInputCutFile      (const string & , vector<cutContainer> &);

#endif
