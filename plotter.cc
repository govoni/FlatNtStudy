#include "plotter.h"

#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"

using namespace std ;


// add the overflow bin to a histogram
void addOverFlow (TH1F * input) 
{
  TH1F * dummy = new TH1F (
    "tempo",
    input->GetTitle (),
    input->GetNbinsX () + 1,
    input->GetXaxis ()->GetXmin (),
    input->GetXaxis ()->GetXmax () + input->GetBinWidth (1)
  ) ;
  dummy->Sumw2 () ;
  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) 
    {
      dummy->SetBinContent (iBin, input->GetBinContent (iBin)) ;
      dummy->SetBinError (iBin, input->GetBinError (iBin)) ;
    }
  string name = input->GetName () ;  
  input->SetName ("trash") ;
  dummy->SetName (name.c_str ()) ;
  swap (*input, *dummy) ;
  return ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


plotter::plotter (float lumi) :
  m_lumi (lumi) ,
  m_canvas ("can", "can", 500, 500)
{
// don't like it, need improvement
  setRootAspect () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add an empty structure to collect all the plots relative to a sample
void plotter::addSample (string sampleName, float XS, int totInitialEvents, bool isSignal, int color)
{
  m_samples[sampleName] = sample (sampleName, XS, m_lumi, totInitialEvents, isSignal, color) ;
  m_samplesSequence.push_back (sampleName) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add a layer of plots (corresponding to a given cut) to a sample
void plotter::addLayerToSample (string sampleName, string layerName)
{
  m_samples[sampleName].m_sampleContent[layerName] = layer (layerName) ;
  m_samples[sampleName].m_layersSequence.push_back (layerName) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add a plot to a layer of a given sample
void plotter::addPlotToLayer (string sampleName, string layerName, 
                              string plotName, int nBins, float xMin, float xMax)
{
  string h_name = sampleName + "_" + layerName + "_" + plotName ;
  TH1F * dummy = new TH1F (h_name.c_str (), h_name.c_str (), nBins, xMin, xMax) ;
  dummy->Sumw2 () ;
  m_samples[sampleName].m_sampleContent[layerName].m_histos[plotName] = dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// duplicate a layer into a new one with a different name, within a single sample
void plotter::copyLayerInSample (string sampleName, string target, string origin)
{
  addLayerToSample (sampleName, target) ;
  // loop over histos in the layer "origin" of sample "sampleName"
  for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[sampleName].m_sampleContent[origin].m_histos.begin () ;
       iHisto != m_samples[sampleName].m_sampleContent[origin].m_histos.end () ;
       ++iHisto)
    {
      string h_name = sampleName + "_" + target + "_" + iHisto->first ;
      m_samples[sampleName].m_sampleContent[target].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
      m_samples[sampleName].m_sampleContent[target].m_histos[iHisto->first]->Reset () ;
    }   
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// duplicate a sample structure
void plotter::copySampleStructure (string target, string origin, float newXS, int newTotInitialEvents, bool isSignal, int newColor)
{
  addSample (target, newXS, newTotInitialEvents, isSignal, newColor) ;
  if (m_samples[origin].m_sampleContent.size () == 0) return ;
  string firstLayer = m_samples[origin].m_layersSequence.at (0) ;
  addLayerToSample (target, firstLayer) ;
  
  for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[origin].m_sampleContent[firstLayer].m_histos.begin () ;
       iHisto != m_samples[origin].m_sampleContent[firstLayer].m_histos.end () ;
       ++iHisto)
    {
      string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
      m_samples[target].m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
      m_samples[target].m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
    }   
  
  for (int iLayer = 1 ; iLayer < m_samples[origin].m_layersSequence.size () ; ++iLayer)
    {
      copyLayerInSample (target, m_samples[origin].m_layersSequence.at (iLayer), firstLayer) ;
    }
  return ;    
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::printStructure ()
{
  // loop over samples
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample)
    {
      cout << iSample->first << "\t" 
           << iSample->second.m_sampleName << "\t"
           << iSample->second.m_XS << "\t"
           << iSample->second.m_totInitialEvents << "\t"
           << iSample->second.m_isSignal << "\t"
           << iSample->second.m_color << "\n" ;

      // loop over layers
      for (int iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer)
        {
          string name = iSample->second.m_layersSequence.at (iLayer) ;
          
          cout << " + " << name << "\t" 
               << iSample->second.m_sampleContent[name].m_layerName << "\n" ;
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto)
            {
              cout << "    + " << iHisto->first << "\t"
                   << iHisto->second->GetName () << "\t"
                   << iHisto->second->GetEntries () << "\t"
                   << iHisto->second->GetNbinsX () << "\n" ;
            } // loop over histos
        } // loop over layers
    } // loop over samples
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::fillHisto (string sampleName, string layerName, string histoName, float value, float weight) 
{
  //PG NB assuming the histo is existing, not to slow down with cross-checks!
  m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName]->Fill (
      value, weight * m_samples[sampleName].m_weight) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::prepareSampleForPlotting (string sampleName)
{
  if (m_samples[sampleName].m_readyForPlotting) return ;

  // loop over layers
  for (int iLayer = 0 ; iLayer < m_samples[sampleName].m_layersSequence.size () ; ++iLayer)
    {
      string name = m_samples[sampleName].m_layersSequence.at (iLayer) ;
      // loop over histos
      for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[sampleName].m_sampleContent[name].m_histos.begin () ;
           iHisto != m_samples[sampleName].m_sampleContent[name].m_histos.end () ;
           ++iHisto)
        {
          addOverFlow (iHisto->second) ; 
          if (m_samples[sampleName].m_isSignal)
            {
              iHisto->second->SetFillColor (0) ;
              iHisto->second->SetLineColor (m_samples[sampleName].m_color) ;
              iHisto->second->SetLineWidth (2) ;
            }
          else
            {  
              iHisto->second->SetFillColor (m_samples[sampleName].m_color) ;
              iHisto->second->SetLineColor (m_samples[sampleName].m_color) ;
            }
        } // loop over histos
    } // loop over layers
  m_samples[sampleName].m_readyForPlotting = true ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::prepareCanvas (float xmin, float xmax, float ymin, float ymax, string xaxisTitle, string yaxisTitle, bool hasPull)
{
  TH1F * bkg = m_canvas.DrawFrame (xmin, ymin, xmax, ymax) ;
  bkg->GetXaxis ()->SetTitle (xaxisTitle.c_str ()) ;
  bkg->GetYaxis ()->SetTitle (yaxisTitle.c_str ()) ;
  bkg->Draw () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotSingleSample (string sampleName, string layerName, string histoName,
                                string xaxisTitle, string yaxisTitle, int isLog)
{
  prepareSampleForPlotting (sampleName) ;
  TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
  m_canvas.cd () ;

  TLegend leg = initLegend (1) ;
  leg.AddEntry (h_var, sampleName.c_str (), "fl") ;

  DrawPlots (vector<TH1F *> (1, h_var), leg, 1, xaxisTitle, yaxisTitle, isLog) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotSingleLayer (string layerName, string histoName, string xaxisTitle, string yaxisTitle, int isLog)
{
  string name = string ("st_") + layerName + "_" + histoName ;
  THStack * stack = new THStack (name.c_str (), "") ;
  int nsamples = m_samplesSequence.size () ;

  TLegend leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }
  
  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      if (m_samples[sampleName].m_isSignal == false) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }

  DrawPlots (vector<THStack *> (1, stack), leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoB (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                           bool isNormalized, float scaleSignal, int isLog)
{
  // FIXME isNormalized needs to be implemented
  // FIXME scaleSignal needs to be implemented
  string name = string ("st_bkg_") + layerName + "_" + histoName ;
  THStack * bkg_stack = new THStack (name.c_str (), "") ;
  int nsamples = m_samplesSequence.size () ;

  TLegend leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (int iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      bkg_stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }
  
  name = string ("st_sig_") + layerName + "_" + histoName ;
  THStack * sig_stack = new THStack (name.c_str (), "") ;

  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      if (m_samples[sampleName].m_isSignal == false) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      sig_stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }

  vector<THStack *> histos ;
  histos.push_back (bkg_stack) ;
  histos.push_back (sig_stack) ;
  
  DrawPlots (histos, leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog) ;

  return ;


}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TLegend plotter::initLegend (int sampleNum)
{
  TLegend leg (0.2, 0.95 - 0.1 * sampleNum * 0.33, 0.87, 0.95) ;
  leg.SetNColumns (3) ;
  leg.SetLineStyle (0) ;
  leg.SetFillStyle (0) ;
  return leg ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::setRootAspect ()
{
  gStyle->SetCanvasColor (-1) ; 
  gStyle->SetPadColor (-1) ; 
  gStyle->SetFrameFillColor (-1) ; 
  gStyle->SetHistFillColor (-1) ; 
  gStyle->SetTitleFillColor (-1) ; 
  gStyle->SetFillColor (-1) ; 
//  gStyle->SetFillStyle (4000) ; 
  gStyle->SetStatStyle (0) ; 
  gStyle->SetTitleStyle (0) ; 
  gStyle->SetCanvasBorderSize (0) ; 
  gStyle->SetFrameBorderSize (0) ; 
  gStyle->SetLegendBorderSize (0) ; 
  gStyle->SetStatBorderSize (0) ; 
  gStyle->SetTitleBorderSize (0) ; 

  m_canvas.SetLeftMargin (0.17) ; 
  m_canvas.SetTopMargin (0.05) ; 
  gStyle->SetTitleYOffset (2) ;

  return ;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat (1111111) ;
  gStyle->SetOptFit (1111) ;
  gStyle->SetTitleBorderSize (0) ;
  gStyle->SetTitleX (0.08) ;
  gStyle->SetTitleY (0.97) ;
  gStyle->SetPalette (1,0) ;
  gStyle->SetLabelFont (42,"xyz") ;
  gStyle->SetTextFont (42) ;
  gStyle->SetStatFont (42) ;
  gStyle->SetTitleFont (42,"xyz") ;
  //gStyle->SetPadGridX (1) ;
  //gStyle->SetPadGridY (1) ;
  
  gStyle->SetTitleXOffset (1.25) ;
  gStyle->SetTitleYOffset (1.4) ;
  
  gStyle->SetTitleX (0.5) ;
  gStyle->SetTitleFont (42) ;
  gStyle->SetTitleAlign (23) ;
  
  gStyle->SetPadRightMargin  (0.15)  ;
  gStyle->SetPadLeftMargin   (0.25) ;
  gStyle->SetPadBottomMargin (0.2) ;
  gStyle->SetPadTopMargin    (0.1) ;
  
  gStyle->SetTitleSize (0.04, "xyz") ;
  gStyle->SetLabelSize (0.05, "xyz") ;
  
  gStyle->SetPadTickX (1) ;
  gStyle->SetPadTickY (1) ;
  
  gStyle->SetNdivisions (7, "xyz") ;
  
  gStyle->SetLineWidth (1) ;
  gStyle->SetFillStyle (0) ;
  gStyle->SetStatStyle (0) ;

}