#ifndef plotter_h
#define plotter_h
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TChain.h"
using namespace std ;
double deltaPhi (double phi1, double phi2);
bool FillChain(TChain& chain, const std::string& inputFileList);
// List of some methods implemented in the .cc
void setPoissonErrorsToHisto (TH1F * input); // give an histogram set the poisson error bars
void addOverFlow (TH1F * input) ; // add the overflow bin as real bin to the histo
TH1F* getHistoOfErrors (TH1F * input); // take an histogram with bin errors
THStack* CreateStack (TH1F * histo); // create a stack from a histo
// one set of histograms given a cut layer
class layer { // layer is alias for cut ... simple class with a string and a map of TH1
 public:
  layer (string name = "dummy") :
  m_layerName (name) {} ;
  string m_layerName ;
  unordered_map<std::string, TH1F *> m_histos ;
} ;
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// all the layers of histograms for a single sample
class sample {
 public:
  sample () :
  m_sampleName ("dummy"),
    m_XS (1),
    m_color (1)
      {}
  // inputs are : name, XS, lumi, initial events, if signal or not, color
  sample (string name, float XS, float lumi, int totInitialEvents, bool isSignal, int color) :
  m_sampleName (name),
    m_XS (XS),
    m_totInitialEvents (totInitialEvents),
    m_isSignal (isSignal),
    m_color (color),
    m_readyForPlotting (false),
    m_weight (XS * lumi / static_cast<float> (m_totInitialEvents)) // luminosity weight
      {}
  string m_sampleName ;
  float m_XS ;
  int m_totInitialEvents ;
  bool m_isSignal ;
  int m_color ;
  bool m_readyForPlotting ;
  float m_weight ;
  unordered_map<string, layer> m_sampleContent ; // map of layer name and layer objects
  vector<string> m_layersSequence ;
} ;
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
class plotter { // generic plotter class
 public:
  plotter (float lumi) ;
  ~plotter () {} ;
  // adding methods
  void addSample (string sampleName, float XS, int totInitialEvents, bool isSignal, int color) ;
  void addLayerToSample (string sampleName, string layerName) ;
  void addPlotToLayer (string sampleName, string layerName, string plotName, int nBins, float xMin, float xMax) ;
  // copy methods
  void copyLayerInSample (string sampleName, string target, string origin) ;
  void copySampleStructure (string target, string origin, float newXS, int newTotInitialEvents, bool isSignal, int newColor) ;
  // print structure
  void printStructure () ;
  // init legend
  TLegend initLegend (int sampleNum) ;
  // fill histo
  void fillHisto (string sampleName, string layerName, string histoName, float value, float weight) ;
  void setRootAspect () ;
  void prepareCanvas (float xmin, float xmax, float ymin, float ymax, string xaxisTitle, string yaxisTitle, bool hasPull) ;
  void prepareSampleForPlotting (string sampleName) ;
  // plotting methods
  void plotSingleSample (string sampleName, string layerName, string histoName,
			 string xaxisTitle, string yaxisTitle, int isLog = 0,
			 string folderName = "") ;
  void plotSingleLayer (string layerName, string histoName,
			string xaxisTitle, string yaxisTitle, int isLog = 0,
			string folderName = "") ;
  void plotFullLayer (string layerName) ;
  // compare S and B
  void compareStoB (string layerName, string histoName, string xaxisTitle, string yaxisTitle,
		    bool isNormalized = false, float scaleSignal = 1., int isLog = 0.,
		    string folderName = "") ;
  void compareStoBFullLayer (string layerName) ;
  // Plot relatve excess
  void plotRelativeExcess (string layerName, string histoName, string xaxisTitle, string yaxisTitle,
			   bool isNormalized = false, float scaleSignal = 1., int isLog = 0.,
			   string folderName = "") ;
  void plotRelativeExcessFullLayer (string layerName, string basefolder = ".") ;
  // scale all histos
  void scaleAllHistos (float scaleFactor) ;
  // poissonian errors
  void setPoissonErrors () ;
  void resetAll (float lumi) ;


template <class T>
  void DrawPlots (vector<T*> histo, TLegend leg, int sampleNum,
		  string xaxisTitle, string yaxisTitle, int isLog, string folderName){
  // FIXME add error bands here
  histo.at (0)->Draw () ;
  float xmin = histo.at (0)->GetXaxis ()->GetXmin () ;
  float xmax = histo.at (0)->GetXaxis ()->GetXmax () ;
  float ymin = histo.at (0)->GetMinimum () ;
  float ymax = histo.at (0)->GetMaximum () ;
  for (size_t i = 1 ; i < histo.size () ; ++i){
    histo.at (i)->Draw () ;
    if (histo.at (i)->GetXaxis ()->GetXmin () < xmin) xmin = histo.at (i)->GetXaxis ()->GetXmin () ;
    if (histo.at (i)->GetXaxis ()->GetXmax () > xmax) xmax = histo.at (i)->GetXaxis ()->GetXmax () ;
    if (histo.at (i)->GetMinimum () < ymin) ymin = histo.at (i)->GetMinimum () ;
    if (histo.at (i)->GetMaximum () > ymax) ymax = histo.at (i)->GetMaximum () ;
  }
  if (isLog && ymin <= 0) ymin = 0.001 ;
  float height = 0.17 ;
  float linesNum = sampleNum / 3 + 1 * (sampleNum % 3) ;
  if (isLog){
    float lymax = log10 (ymax) + height * (log10 (ymax) - log10 (ymin)) * linesNum ;
    ymax = pow (10, lymax) ;
  }
  else ymax += height * (ymax - ymin) * linesNum ;
  if (isLog) m_canvas.SetLogy () ;
  prepareCanvas (xmin, xmax, ymin, ymax, xaxisTitle, yaxisTitle, 0) ;
  string options = "same histo" ;
  for (size_t i = 0 ; i < histo.size () ; ++i)
    histo.at (i)->Draw (options.c_str ()) ;
  leg.Draw () ;
  m_canvas.RedrawAxis () ;
  string filename = folderName + histo.at (0)->GetName () ;
  if (histo.size () > 1) filename += "_compare" ;
  if (isLog) filename += "_log" ;
  filename += ".pdf" ;
  m_canvas.Print (filename.c_str (), "pdf") ;
  if (isLog) m_canvas.SetLogy (0) ;
  return;
 }
 private:
 float m_lumi ;
 TCanvas m_canvas ;
 unordered_map<string, sample> m_samples ; //map of sample names and sample object
 unordered_map<string, string> m_titles ;
 vector<string> m_samplesSequence ;
} ;
#endif