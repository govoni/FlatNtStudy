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
#include <unordered_map>
#include <memory>

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

 sampleContainer(string sampleName, string sampleNameReduced, int color, float xsec, int numBefore, int isSignal):
  sampleName(sampleName),
    sampleNameReduced(sampleNameReduced),
    color(color),
    xsec(xsec),
    numBefore(numBefore),
    isSignal(isSignal){};


  bool operator < (const sampleContainer & container2) const {
    if(sampleName < container2.sampleName) return true;
    else if(sampleName == container2.sampleName){
      if (sampleNameReduced < container2.sampleNameReduced) return true;
      else return false;
    }
    else return false;    
  }

  bool operator == (const sampleContainer & container2) const {
    if(sampleName == container2.sampleName and xsec == container2.xsec and isSignal == container2.isSignal and sampleNameReduced == container2.sampleNameReduced) return true;
    else return false;    
  }
  
  string sampleName ;
  string sampleNameReduced;
  int    color;
  float  xsec;
  int    numBefore;
  int    isSignal;

};

// ---------------
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
	      float dRlJ,
	      float ptJJLLMet,
	      float lZep,
	      float HTTrackjet,
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
      dRlJ(dRlJ),
      ptJJLLMet(ptJJLLMet),
      lZep(lZep),
      HTTrackjet(HTTrackjet),
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
  float  dRlJ;
  float  ptJJLLMet;
  float  lZep;
  float  HTTrackjet;
  int    polarization;

};

// ---------------

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

class variableContainerDynamic : public variableContainer {

 public :
  
  variableContainerDynamic(){};
  ~variableContainerDynamic(){};

 variableContainerDynamic(string variableName, int Nbin, vector<float> binning, string label){
   (*this).variableName = variableName ;
   (*this).Nbin         = Nbin;
   (*this).binning      = binning;

   if(not binning.empty()){
     min          = binning.at(0);
     max          = binning.back();
   }
   
   (*this).label = label;	
 }

 variableContainer getVariableContainer(){
   return variableContainer(variableName,Nbin,min,max,label);
 }

 vector<float> binning;
  
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


class variableContainerDynamic2D : public variableContainer2D {

 public :

  variableContainerDynamic2D(){};
  ~variableContainerDynamic2D(){};

 variableContainerDynamic2D(string variableNameX, int NbinX, vector<float> binningX, string labelX,
			    string variableNameY, int NbinY, vector<float> binningY, string labelY){
   (*this).variableNameX = variableNameX;
   (*this).NbinX = NbinX;
   (*this).binningX = binningX;
   if(not binningX.empty()){
     minX = binningX.front();
     maxX  = binningX.back();
   }

   (*this).labelX = labelX;
   (*this).variableNameY = variableNameY;
   (*this).NbinY = NbinY ;
   (*this).binningY = binningY;
   if(not binningY.empty()){
     minY = binningY.front();
     maxY = binningY.back();
   }

   (*this).labelY = labelY;
 } 

 variableContainer2D getVariableContainer2D(){
   return variableContainer2D(variableNameX,NbinX,minX,maxX,labelX,
			      variableNameY,NbinY,minY,maxY,labelY);
 }

 vector<float> binningX ;
 vector<float> binningY ;

};



// ---------------

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

// ---------------

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

 histoContainer(string cutName, variableContainerDynamic container):
  cutName(cutName),
    varName(container.variableName){
    histogram = new TH1F((cutName+"_"+varName).c_str(),"",container.Nbin,&container.binning[0]);
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






// ---------------

class fakeRateContainer {

 public :

  fakeRateContainer(){};

  ~fakeRateContainer();

  fakeRateContainer(TFile* inputFile);

  float getFakeRate (const int & , const int &, const float &, const float &);
  float getFakeRateUncertainty(const int&, const int &, const float&, const float&);
  void  getFakeRateInterpolation( TGraph2D* fakeRate, TH2F* fakeMap, TH2F* ptCentre);

  shared_ptr<TH2F> Denominator_bjet_to_mu ;
  shared_ptr<TH2F> Denominator_jet_to_mu ;

  shared_ptr<TH2F> Denominator_bjet_to_e ;
  shared_ptr<TH2F> Denominator_jet_to_e ;

  shared_ptr<TH2F> Numerator_bjet_to_mu ;
  shared_ptr<TH2F> Numerator_jet_to_mu ;

  shared_ptr<TH2F> Numerator_bjet_to_e ;
  shared_ptr<TH2F> Numerator_jet_to_e ;


  shared_ptr<TH2F>     mFakeRate_bjet ;
  shared_ptr<TH2F>     mFakeRate_jet ;

  shared_ptr<TGraph2D> muonFakeRate_bjet;
  shared_ptr<TGraph2D> muonFakeRate_jet;
 
  shared_ptr<TH2F>     eFakeRate_bjet ;
  shared_ptr<TH2F>     eFakeRate_jet ;

  shared_ptr<TGraph2D> electronFakeRate_jet;
  shared_ptr<TGraph2D> electronFakeRate_bjet;

  //bin center
  shared_ptr<TH2F> PtCentre_jet_to_mu;
  shared_ptr<TH2F> PtCentre_jet_to_e;

  shared_ptr<TH2F> PtCentre_bjet_to_mu;
  shared_ptr<TH2F> PtCentre_bjet_to_e;

};

// ---------------

class fakeMigrationContainer {

 public :
  
  fakeMigrationContainer(){};
  
  ~fakeMigrationContainer();

  fakeMigrationContainer(TFile* inputFile);

  float getMigration (const int & leptonPID, const int & jetflavour, const float & pt, const float & eta);

  shared_ptr<TH2F> Pt_migration_jet_to_mu ;
  shared_ptr<TH2F> Pt_migration_jet_to_e ;

  shared_ptr<TH1F> Pt_migration_centre_jet_to_mu ;
  shared_ptr<TH1F> Pt_migration_centre_jet_to_e ;

  shared_ptr<TProfile> profile_jet_to_mu ;
  shared_ptr<TProfile> profile_jet_to_e ;

  shared_ptr<TGraph> migration_jet_to_mu ;
  shared_ptr<TGraph> migration_jet_to_e ;

  shared_ptr<TH2F> Pt_migration_bjet_to_mu ;
  shared_ptr<TH2F> Pt_migration_bjet_to_e ;

  shared_ptr<TH1F> Pt_migration_centre_bjet_to_mu ;
  shared_ptr<TH1F> Pt_migration_centre_bjet_to_e ;

  shared_ptr<TProfile> profile_bjet_to_mu ;
  shared_ptr<TProfile> profile_bjet_to_e ;

  shared_ptr<TGraph> migration_bjet_to_mu ;
  shared_ptr<TGraph> migration_bjet_to_e ;

};

// functions

int ReadInputSampleFile   (const string & , map<string, vector<sampleContainer> > & );

int ReadInputSampleFile   (const string & , unordered_map<string, vector< sampleContainer> > & );

int ReadInputVariableFile (const string & , vector<variableContainer> & );

int ReadInputVariableFile (const string & , vector<variableContainer2D> & );

int ReadInputVariableFile (const string & , vector<string> & );

int ReadInputTrainingFile (const string & , vector<trainingContainer> & );

int ReadInputCutFile      (const string & , vector<cutContainer> &);

// dynamic bin
int ReadInputVariableFileDynamicBinning (const string & , vector<variableContainerDynamic> & );

int ReadInputVariableFileDynamicBinning (const string & , vector<variableContainerDynamic2D> & );

#endif
