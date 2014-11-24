#ifndef plotter_h
#define plotter_h

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <unordered_map>

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"

/*
lista casuale delle cose da implementare"
 - stack
 - segnale o fondo
 - cfg file
    - colori per ogni sample
    - lista dei sample
    - fare o non fare blinding
 - make folders
 - pesi
 - tanti tagli = tanti plot
 - sezioni d'urto
 - luminosita'
 - incertezze sistematiche
 - come si calcola l'errore statistico
 - toy experiments
 - blinding
 - legenda e colore dei sample
 - voglio poter aggiungere variabili anche non da cfg file
 - voglio plottare gli overflow (e underflow?)
 - fare giusto sumw2


*/

using namespace std ;


// // the first index (external vector) is the cuts level
// // the second index (internal vector) is the variable index
// typedef vector<vector<TH1F *> > sample_t ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// one set of histograms given a cut layer
struct layer
{
  layer (string name = "dummy") :
    m_layerName (name) {} ; 
  string m_layerName ;
  unordered_map<string, TH1F *> m_histos ;
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// all the layers of histograms for a single sample
struct sample
{
  sample () :
    m_sampleName ("dummy"),
    m_XS (1), 
    m_color (1)
    {}

  sample (string name, float XS, float lumi, int totInitialEvents, bool isSignal, int color) :
    m_sampleName (name),
    m_XS (XS), 
    m_totInitialEvents (totInitialEvents),
    m_isSignal (isSignal),
    m_color (color),
    m_readyForPlotting (false),
    m_weight (XS * lumi / static_cast<float> (m_totInitialEvents)) 
    {}

  string m_sampleName ;
  float m_XS ;
  int m_totInitialEvents ;
  bool m_isSignal ;
  int m_color ;
  bool m_readyForPlotting ;
  float m_weight ;
  unordered_map<string, layer> m_sampleContent ;
  vector<string> m_layersSequence ;
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add the overflow bin to a histogram
void addOverFlow (TH1F * input) ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class plotter
{
public:

  plotter (float lumi) ;
  
  ~plotter () {} ;
  
  void addSample (string sampleName, float XS, int totInitialEvents, bool isSignal, int color) ;
  void addLayerToSample (string sampleName, string layerName) ;
  void addPlotToLayer (string sampleName, string layerName, 
                       string plotName, int nBins, float xMin, float xMax) ;
  void copyLayerInSample (string sampleName, string target, string origin) ;
  void copySampleStructure (string target, string origin, float newXS, int newTotInitialEvents, bool isSignal, int newColor) ;
  void printStructure () ; 
  void plotSingleSample (string sampleName, string layerName, string histoName) ;

  void fillHisto (string sampleName, string layerName, string histoName, float value, float weight) ;

  void setRootAspect () ;
  void prepareCanvas (float xmin, float xmax, float ymin, float ymax, string xaxisTitle, string yaxisTitle, bool hasPull) ;
  void prepareSampleForPlotting (string sampleName) ;  
  void plotSingleSample (string sampleName, string layerName, string histoName, string xaxisTitle, string yaxisTitle, int isLog = 0) ;
  void plotSingleLayer (string layerName, string histoName, string xaxisTitle, string yaxisTitle, int isLog = 0) ;

  template <class T>
  void DrawPlot (T * histo, TLegend leg, int sampleNum, string xaxisTitle, string yaxisTitle, int isLog)
    {
      histo->Draw () ;
      float xmin = histo->GetXaxis ()->GetXmin () ;
      float xmax = histo->GetXaxis ()->GetXmax () ;
      float ymin = histo->GetMinimum () ;
      float ymax = histo->GetMaximum () ;
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
      histo->Draw (options.c_str ()) ;
      leg.Draw () ;
      m_canvas.RedrawAxis () ;    

      string filename = histo->GetName () ;
      if (isLog) filename += "_log" ;
      filename += ".pdf" ;
      m_canvas.Print (filename.c_str (), "pdf") ; 
      if (isLog) m_canvas.SetLogy (0) ;
    
      return ;
    }
  
private:

  float m_lumi ;
  TCanvas m_canvas ;
  unordered_map<string, sample> m_samples ;
  unordered_map<string, string> m_titles ;
  vector<string> m_samplesSequence ;
  

} ;



#endif