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
#include "TStyle.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TMath.h"

using namespace std ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// generic functions implemented in the cc
double deltaPhi (double phi1, double phi2) ;
bool   FillChain (TChain& chain, const string& inputFileList) ;
// give an histogram set the poisson error bars
void   setPoissonErrorsToHisto (TH1F * input) ;
// add the overflow bin as real bin to the histo
void   addOverFlow (TH1F * input) ;            
// take an histogram with bin errors
TH1F*  getHistoOfErrors (TH1F * input, int isLog) ; 
// create a stack from a histo
THStack* CreateStack (TH1F * histo) ; 


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// one set of histograms given a cut layer
// layer is alias for cut ... simple class with a string and a map of TH1
class layer { 
 public:
  layer (string name = "dummy") :
  m_layerName (name) {} ;
  string m_layerName ;
  unordered_map<string, TH1F *> m_histos ;
  unordered_map<string, TH2F *> m_2Dhistos ;
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
  int   m_totInitialEvents ;
  bool  m_isSignal ;
  int   m_color ;
  bool  m_readyForPlotting ;
  float m_weight ;
  unordered_map<string, layer> m_sampleContent ; // map of layer name and layer objects
  vector<string> m_layersSequence ;
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class plotter { // generic plotter class
 public:
  plotter (float lumi, string folderName = "plots") ;
  ~plotter () {} ;
  
  // adding methods
  void addSample        (string sampleName, float XS, int totInitialEvents, 
                         bool isSignal, int color) ;
  void addLayerToSample (string sampleName, string layerName) ;
  void addPlotToLayer   (string sampleName, string layerName, string plotName, 
                         int nBins, float xMin, float xMax, string labelName = "") ;
  void add2DPlotToLayer (string sampleName, string layerName, string plotName, 
                         int nBinsX, float xMinX, float xMaxX,
                         int nBinsY, float xMinY, float xMaxY,
                         string labelNameX = "", string labelNameY = "") ;
  // copy methods
  void copyLayerInSample   (string sampleName, string target, string origin) ;
  void copySampleStructure (string target, string origin, float newXS, 
                            int newTotInitialEvents, bool isSignal, int newColor) ;
  // print structure
  void printStructure () ;
  void printEventNumber (string layerName, string histoName) ;
  // init legend
  TLegend initLegend (int sampleNum) ;
  // fill histo
  void fillHisto     (string sampleName, string layerName, string histoName, 
                      float value, float weight) ;
  void fill2DHisto   (string sampleName, string layerName, string histoName, 
                      float valueX, float valueY, float weight) ;
  void setRootAspect () ;
  void prepareCanvas (float xmin, float xmax, float ymin, float ymax, 
                      string xaxisTitle, string yaxisTitle, bool hasPull) ;
  void prepareSampleForPlotting (string sampleName) ;
  void cleanFromLateX (TString & Name) ;
 
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
		    bool isNormalized = false, float scaleSignal = 1., int isLog = 0,
		    string folderName = "") ;
  void compareStoBFullLayer (string layerName, string folderTag = "") ;
  void compareStoB2D (string layerName, string histoName, 
            string xaxisTitle, string yaxisTitle, 
            bool isNormalized = false, float scaleSignal = 1., int isLog = 0, 
            string folderName = "") ;
  void compareStoBFullLayer2D (string layerName, string folderTag = "") ;

  // Plot relatve excess
  void plotRelativeExcess (string layerName, string histoName, string xaxisTitle, string yaxisTitle,
			   bool isNormalized = false, float scaleSignal = 1., int isLog = 0.,
			   string folderName = "") ;

  void plotRelativeExcessFullLayer (string layerName, string basefolder = ".") ;

  // scale all histos
  void scaleAllHistos (float scaleFactor) ;

  // scale all histos
  void normaliseAllHistos () ;
  void normaliseAll1DHistos () ;
  void normaliseAll2DHistos () ;

  // save all histos
  void saveAllHistos (string filename) ;

  // poissonian errors
  void setPoissonErrors () ;

  void resetAll (float lumi) ;

  template <class T>
  void DrawPlots (vector<T*> histo, TLegend leg, int sampleNum,
		  string xaxisTitle, string yaxisTitle, int isLog, string folderName, bool plotCanvas = true)
    {
      // FIXME add error bands here
      histo.at (0)->Draw () ;
      if(xaxisTitle == "")
	xaxisTitle = histo.at (0)->GetXaxis()->GetTitle();

      float xmin = histo.at (0)->GetXaxis ()->GetXmin () ;
      float xmax = histo.at (0)->GetXaxis ()->GetXmax () ;
      float ymin = histo.at (0)->GetMinimum () ;
      float ymax = histo.at (0)->GetMaximum () ;
      for (size_t i = 1 ; i < histo.size () ; ++i)
        {
          histo.at (i)->Draw () ;
          if (histo.at (i)->GetXaxis ()->GetXmin () < xmin) xmin = histo.at (i)->GetXaxis ()->GetXmin () ;
          if (histo.at (i)->GetXaxis ()->GetXmax () > xmax) xmax = histo.at (i)->GetXaxis ()->GetXmax () ;
          if (histo.at (i)->GetMinimum () < ymin) ymin = histo.at (i)->GetMinimum () ;
          if (histo.at (i)->GetMaximum () > ymax) ymax = histo.at (i)->GetMaximum () ;
        }
      if (isLog && ymin <= 0) ymin = 0.001 ;

      float height = 0.17 ;
      float linesNum = sampleNum / 3 + 1 * (sampleNum % 3) ;

      if (isLog)
        {
          float lymax = log10 (ymax) + height * (log10 (ymax) - log10 (ymin)) * linesNum ;
          ymax = pow (10, lymax) ;
        }
      else ymax += height * (ymax - ymin) * linesNum ;
      if (isLog) m_canvas.SetLogy () ;

      prepareCanvas (xmin, xmax, ymin, ymax, xaxisTitle, yaxisTitle, 0) ;
      string options = "same histo" ;
    
    
      for (size_t i = 0 ; i < histo.size () ; ++i) 
        {
          histo.at (i)->Draw (options.c_str ()) ;
        }
      leg.Draw () ;
      m_canvas.RedrawAxis () ;

      if(plotCanvas){
       string filename = folderName + histo.at (0)->GetName () ;
       TString Name (filename.c_str());     
       cleanFromLateX (Name) ;  
       if (histo.size () > 1) Name += "_compare" ;
       if (isLog) Name += "_log" ;
       Name += ".pdf" ;
       m_canvas.Print (Name, "pdf") ;
       Name.ReplaceAll(".pdf",".png");
       m_canvas.Print (Name, "png") ;
      }
      

      if (isLog) m_canvas.SetLogy (0) ;

      return;
    } // DrawPlots

  template <class T>
  void Draw2DPlots (vector<T*> histo, TLegend leg, int sampleNum,
		  string xaxisTitle, string yaxisTitle, int isLog, string folderName, bool plotCanvas = true)
    {
      if (isLog) m_canvas.SetLogz () ;
      float xmin = histo.at (0)->GetXaxis ()->GetXmin () ;
      float xmax = histo.at (0)->GetXaxis ()->GetXmax () ;
      float ymin = histo.at (0)->GetYaxis ()->GetXmin () ;
      float ymax = histo.at (0)->GetYaxis ()->GetXmax () ;
      if ("" == xaxisTitle) xaxisTitle = histo.at (0)->GetXaxis ()->GetTitle () ;
      if ("" == yaxisTitle) yaxisTitle = histo.at (0)->GetYaxis ()->GetTitle () ;

      prepareCanvas (xmin, xmax, ymin, ymax, xaxisTitle, yaxisTitle, 0) ;

      string options = "same cont2" ;
      for (size_t i = 0 ; i < histo.size () ; ++i) 
        {
          histo.at (i)->Draw (options.c_str ()) ;
        }
      leg.Draw () ;
      m_canvas.RedrawAxis () ;

      if (plotCanvas)
        {
          string filename = folderName + histo.at (0)->GetName () ;
          TString Name (filename.c_str ()) ;       
          cleanFromLateX (Name) ;  
          if (histo.size () > 1) Name += "_compare" ;
          if (isLog) Name += "_log" ;
          Name += ".pdf" ;
          m_canvas.Print (Name, "pdf") ;
          Name.ReplaceAll (".pdf",".png") ;
          m_canvas.Print (Name, "png") ;
        }

      if (isLog) m_canvas.SetLogz (0) ;

      return;
    } // Draw2DPlots



 private:

 string m_folderName ; 
 float m_lumi ;
 TCanvas m_canvas ;
 unordered_map<string, sample> m_samples ; //map of sample names and sample object
 unordered_map<string, string> m_titles ;
 vector<string> m_samplesSequence ;
} ;

#endif
