#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>

// sample container class                                                                                                                                                                  
class sampleContainer {
 public :
  sampleContainer(){};
  ~sampleContainer(){};

 sampleContainer(std::string sampleName, int color, double xsec, int numBefore, int isSignal):
  sampleName(sampleName),
    color(color),
    xsec(xsec),
    numBefore(numBefore),
    isSignal(isSignal){};

  std::string sampleName ;
  int color;
  double xsec;
  int numBefore;
  int isSignal;

};

class cutContainer {

 public :
  cutContainer(){};
  ~cutContainer(){};

 cutContainer(std::string cutLayerName, 
              std::pair<double,double> ptL, 
              double  chargeSign, 
              double  flavour,
              int     nLep, 
              int     nextra,             
              double  MET, 
              std::pair<double,double> ptJet, 
              double DetaJJ, 
              double Mjj, 
              std::pair<double,double> Mll, 
              std::pair<double,double> MllZVeto, 
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
      Mll(Mll),
      MllZVeto(MllZVeto),
      bTagVeto(bTagVeto),
      jetPUID(jetPUID){};

  std::string cutLayerName ;
  std::pair<double,double> ptL;
  double  chargeSign;
  double  flavour;
  int     nLep;
  int     nextra;
  double  MET;
  std::pair<double,double> ptJet;  
  double  DetaJJ;
  double  Mjj;
  std::pair<double,double>  Mll;
  std::pair<double,double>  MllZVeto;
  double  bTagVeto;
  double  jetPUID;
};

class variableContainer {

 public :

  variableContainer(){};
  ~variableContainer(){};

  variableContainer(std::string variableName, int Nbin, double min, double max, std::string label):
   variableName(variableName),
    Nbin(Nbin),
    min(min),
    max(max),
    label(label){}; 

   std::string variableName ;
   int Nbin ;
   double min;
   double max;
   std::string label;

};


int ReadInputSampleFile   (const std::string & , std::map<std::string, std::vector<sampleContainer> > & );

int ReadInputVariableFile (const std::string & , std::vector<variableContainer> & );

int ReadInputCutFile      (const std::string & , std::vector<cutContainer> &);

