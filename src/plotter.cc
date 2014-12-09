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
//  dummy->Sumw2 () ;
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


void setPoissonErrorsToHisto (TH1F * input) 
{
  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) 
    {
      input->SetBinError (iBin, sqrt (input->GetBinContent (iBin))) ;
    }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


TH1F *  getHistoOfErrors (TH1F * input)
{
  string name = "err_" ;
  name += input->GetName () ;
  string title = "errors of " ;
  title += input->GetTitle () ;
  TH1F * dummy = new TH1F (
    name.c_str (),
    title.c_str (),
    input->GetNbinsX (),
    input->GetXaxis ()->GetXmin (),
    input->GetXaxis ()->GetXmax ()
  ) ;
  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) 
    {
      dummy->SetBinContent (iBin, input->GetBinError (iBin)) ;
    }
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


THStack * stackMe (TH1F * histo)
{
  string name = histo->GetName () ;
  name += "_st" ;
  THStack * dummy = new THStack (name.c_str (), "") ;
  dummy->Add (histo) ;
  return dummy ;
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
//  dummy->Sumw2 () ;
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
  
  for (unsigned int iLayer = 1 ; iLayer < m_samples[origin].m_layersSequence.size () ; ++iLayer)
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
      for (unsigned int iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer)
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
  for (unsigned int iLayer = 0 ; iLayer < m_samples[sampleName].m_layersSequence.size () ; ++iLayer)
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
                                string xaxisTitle, string yaxisTitle, int isLog, string folderName)
{
  prepareSampleForPlotting (sampleName) ;
  TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
  m_canvas.cd () ;

  TLegend leg = initLegend (1) ;
  leg.AddEntry (h_var, sampleName.c_str (), "fl") ;

  DrawPlots (vector<TH1F *> (1, h_var), leg, 1, xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotSingleLayer (string layerName, string histoName, 
                               string xaxisTitle, string yaxisTitle, int isLog, string folderName)
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

  DrawPlots (vector<THStack *> (1, stack), leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotFullLayer (string layerName)
{
  // FIXME needs to be finished
  string outFolderName = "plots/stacks/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto)
    {
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str (), "#sigma #times lumi", 1, outFolderName) ;
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str (), "#sigma #times lumi", 0, outFolderName) ;
    }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoB (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                           bool isNormalized, float scaleSignal, int isLog, string folderName)
{
  // FIXME isNormalized needs to be implemented
  // FIXME scaleSignal needs to be implemented
  string name = string ("comp_bkg_") + layerName + "_" + histoName ;
  THStack * bkg_stack = new THStack (name.c_str (), "") ;
  int nsamples = m_samplesSequence.size () ;

  TLegend leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (unsigned int iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      bkg_stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }
  
  name = string ("comp_sig_") + layerName + "_" + histoName ;
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
  
  DrawPlots (histos, leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoBFullLayer (string layerName)
{
  // FIXME needs to be finished
  string outFolderName = "plots/compareStoB/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto)
    {
      compareStoB (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                   0, 1., 0, outFolderName) ;
      compareStoB (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                   0, 1., 1, outFolderName) ;
    }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

/*
this is in case of two alternative signal samples.
The one with the smallest XS (and the SM one) is the first, the one with the largest XS is the second.
This is tailored to the VBS WW same-sign analysis.
*/
void plotter::plotRelativeExcess (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                                  bool isNormalized, float scaleSignal, int isLog,
                                  string folderName)
{
  // FIXME isNormalized needs to be implemented
  // FIXME scaleSignal needs to be implemented
  
  // prepare the first stacks
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  string name = string ("st_SM_") + layerName + "_" + histoName ; // SM = QCD bkg + EWK 126 GeV H
  THStack * SM_stack = new THStack (name.c_str (), "") ;
  name = string ("st_nH_") + layerName + "_" + histoName ;  // no H = QCD bkg + EWK noH
  THStack * nH_stack = new THStack (name.c_str (), "") ;
  int nsamples = m_samplesSequence.size () ;

  TLegend leg = initLegend (nsamples + 1) ;

  // add bkg to the stack and identify the position of the two signals in the sample
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  int sigSamplesNum = 0 ;
  vector<int> signalPos (2, 0) ;
  for (unsigned int iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample)
    {
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) 
        {
          signalPos.at (sigSamplesNum) = iSample ;
          ++sigSamplesNum ;
          continue ;
        }
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      SM_stack->Add (h_var) ;
      nH_stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }
  if (2 != sigSamplesNum)
    {
      cout << "ERROR: found " << sigSamplesNum << " signal samples, exiting" << endl ;
      exit (1) ;
    }

  // add the first signal to the stack, to bild up the total SM expectations
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  string sampleName = m_samplesSequence.at (signalPos.at (0)) ;
  TH1F * h_sigSM = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;  
  SM_stack->Add (h_sigSM) ;
  leg.AddEntry (h_sigSM, sampleName.c_str (), "fl") ;

  // calculate the difference between noH and SM126
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  sampleName = m_samplesSequence.at (signalPos.at (1)) ;
  TH1F * h_noH = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;  
  nH_stack->Add (h_noH) ;
  name = "diff_" ;
  name += h_noH->GetName () ;
  TH1F * diff = (TH1F *) h_noH->Clone (name.c_str ()) ;
  diff->Add (h_sigSM, -1.) ;
  diff->SetLineColor (4) ;
  diff->SetLineWidth (2) ;
  diff->SetFillStyle (0) ;
  leg.AddEntry (diff, "noH - 126", "fl") ;

  // put the difference in a THStack and compare it to the expected SM measurement
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  name = diff->GetName () ;
  name += "_st" ;
  THStack * delta_stack = stackMe (diff) ;
  vector<THStack *> histos ;  // ensemble of objects to be drawn together
  histos.push_back (SM_stack) ;  
  histos.push_back (delta_stack) ;  
  
  // histogram to plot the error band of the SM bkg + 126 Higgs THstack 
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  name = "SM_histo_" ;
  name += h_sigSM->GetName () ;
  TH1F * SM_histo = (TH1F *) SM_stack->GetStack ()->Last ()->Clone (name.c_str ()) ;
  SM_histo->SetLineColor (1) ;
  SM_histo->SetFillColor (1) ;
  SM_histo->SetFillStyle (3001) ;

  // get the histogram of the total shape, of SM bkg + noH hypothesis
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  name = "nH_histo_" ;
  name += h_noH->GetName () ;
  TH1F * nH_histo = (TH1F *) nH_stack->GetStack ()->Last ()->Clone (name.c_str ()) ;
  nH_histo->SetLineColor (6) ;
  nH_histo->SetLineWidth (2) ;
  nH_histo->SetLineStyle (2) ;
  nH_histo->SetFillStyle (0) ;
  leg.AddEntry (nH_histo, "bkg + noH", "fl") ;
  histos.push_back (stackMe (nH_histo)) ;

  // do the drawing
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  DrawPlots (histos, leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;
  SM_histo->Draw ("E2same") ; // draw the error band on the SM THstack
                              // and then reprint the canvas 
  if (isLog) m_canvas.SetLogy (1) ;
  m_canvas.RedrawAxis () ;    
  leg.Draw () ;
  string filename = folderName + histos.at (0)->GetName () ;
  if (histos.size () > 1) filename += "_compare" ; 
  if (isLog) filename += "_log" ;
  filename += ".pdf" ;
  m_canvas.Print (filename.c_str (), "pdf") ; 
  if (isLog) m_canvas.SetLogy (0) ;

  // direct comparison of the uncertainty on the SM to the difference between H and noH
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  TLegend leg2 = initLegend (2) ;
  TH1F * h_tot_SM = (TH1F *) SM_stack->GetStack ()->Last () ;
  TH1F * h_tot_SM_err = getHistoOfErrors (h_tot_SM) ;
  h_tot_SM_err->SetLineColor (1) ;
  h_tot_SM_err->SetFillColor (1) ;
  h_tot_SM_err->SetFillStyle (3001) ;
  leg2.AddEntry (h_tot_SM_err, "SM fluct", "fl") ;
  leg2.AddEntry (diff, "noH - 126", "fl") ;

  vector<TH1F *> histos2 ;
  histos2.push_back (h_tot_SM_err) ;
  histos2.push_back (diff) ;
  DrawPlots (histos2, leg2, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotRelativeExcessFullLayer (string layerName, string basefolder)
{
  // FIXME needs to be finished
  string outFolderName = basefolder + "/plots/relExcess/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto)
    {
      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                          0, 1., 0, outFolderName) ;
      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                          0, 1., 1, outFolderName) ;
    }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::scaleAllHistos (float scaleFactor)
{
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample)
    {
      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer)
        {
          string name = iSample->second.m_layersSequence.at (iLayer) ;
          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto)
            {
              iHisto->second->Scale (scaleFactor) ;
            } // loop over histos
        } // loop over layers
    } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::setPoissonErrors ()
{
  cout << "WARNING setting all the histograms to the poisson error\n" ;
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample)
    {
      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer)
        {
          string name = iSample->second.m_layersSequence.at (iLayer) ;
          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto)
            {
              setPoissonErrorsToHisto (iHisto->second) ;
            } // loop over histos
        } // loop over layers
    } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::resetAll (float lumi)
{
  cout << "WARNING resetting all the stored quantities\n" ;
  cout << "luminosity set to " << lumi << "\n" ;
  m_lumi = lumi ;
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample)
    {
      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer)
        {
          string name = iSample->second.m_layersSequence.at (iLayer) ;
          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto)
            {
              iHisto->second->Reset () ;
            } // loop over histos
        } // loop over layers
    } // loop over samples
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