#include "plotter.h"

#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TColor.h"

using namespace std ;


// add the overflow bin to a histogram
void addOverFlow (TH1F * input) {
  // clone the histogram
  TH1F * dummy = new TH1F ("tempo",input->GetTitle(),input->GetNbinsX()+1,input->GetXaxis()->GetXmin(),input->GetXaxis()->GetXmax()+input->GetBinWidth(1)) ;
  dummy->Sumw2 () ;
  for (int iBin = 0 ; iBin <= input->GetNbinsX()+1 ; ++iBin) {
      dummy->SetBinContent(iBin, input->GetBinContent(iBin)) ;
      dummy->SetBinError(iBin,input->GetBinError(iBin)) ;
  }

  string name = input->GetName() ;  
  input->SetName ("trash") ;
  dummy->SetName (name.c_str()) ;
  swap (*input, *dummy) ;
  if(input !=0 and input != NULL) delete input;
  return ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// using this suggestion by CMS statistics commitee
void setPoissonErrorsToHisto (TH1F * input) {
  input->SetBinErrorOption(TH1::kPoisson);
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
TH1F *  getHistoOfErrors (TH1F * input) {
  string name = "err_" ;
  name += input->GetName() ;
  string title = "errors of " ;
  title += input->GetTitle() ;
  TH1F * dummy = new TH1F (
    name.c_str(),
    title.c_str(),
    input->GetNbinsX(),
    input->GetXaxis()->GetXmin (),
    input->GetXaxis()->GetXmax ()
  ) ;
  for (int iBin = 0 ; iBin <= input->GetNbinsX () + 1 ; ++iBin) 
      dummy->SetBinContent (iBin, input->GetBinError(iBin)) ;

  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
THStack * CreateStack (TH1F * histo){
  string name = histo->GetName () ;
  name += "_st" ;
  THStack * dummy = new THStack (name.c_str (), "") ;
  dummy->Add (histo) ;
  return dummy ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
plotter::plotter (float lumi) :
  m_lumi   (lumi) ,
  m_canvas ("can","can",180,52,550,550){
  setRootAspect () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add an empty structure to collect all the plots relative to a sample
void plotter::addSample (string sampleName, float XS, int totInitialEvents, bool isSignal, int color){ 
  // fill the sample map creating the object : name, XS, luminosity, processed events, isSignal or not, color
  m_samples[sampleName] = sample (sampleName, XS, m_lumi, totInitialEvents, isSignal, color) ; // not ordered by name
  m_samplesSequence.push_back (sampleName) ; // take trace of the original sequence
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// add a layer of plots (corresponding to a given cut) to a sample
void plotter::addLayerToSample (string sampleName, string layerName){
  // to each sample add a map with a name for the cut
  m_samples[sampleName].m_sampleContent[layerName] = layer (layerName) ; // add a layer to the sample
  m_samples[sampleName].m_layersSequence.push_back (layerName) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// add a plot to a layer of a given sample i.e. a variable to plot --> General strategy is loop on samples, loop on cuts and then loop on variables and add things
void plotter::addPlotToLayer (string sampleName, string layerName, 
                              string plotName, int nBins, float xMin, float xMax){
  string h_name = sampleName + "_" + layerName + "_" + plotName ; // make the name for the sample and cut
  TH1F * dummy = new TH1F (h_name.c_str (), h_name.c_str (), nBins, xMin, xMax) ; // build the histogram
  dummy->Sumw2 () ;
  m_samples[sampleName].m_sampleContent[layerName].m_histos[plotName] = dummy ; // store the histogram
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// duplicate a layer into a new one with a different name, within a single sample
void plotter::copyLayerInSample (string sampleName, string target, string origin){ // copy a layer cut in a different same

  addLayerToSample (sampleName, target) ;
  // loop over histos in the layer "origin" of sample "sampleName"
  for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[sampleName].m_sampleContent[origin].m_histos.begin () ;
       iHisto != m_samples[sampleName].m_sampleContent[origin].m_histos.end () ;
       ++iHisto){
      string h_name = sampleName + "_" + target + "_" + iHisto->first ;
      m_samples[sampleName].m_sampleContent[target].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
      m_samples[sampleName].m_sampleContent[target].m_histos[iHisto->first]->Reset () ;
  }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// duplicate a sample structure
void plotter::copySampleStructure (string target, string origin, float newXS, int newTotInitialEvents, bool isSignal, int newColor){

  addSample (target, newXS, newTotInitialEvents, isSignal, newColor) ; // add a new sample with the target name
  if (m_samples[origin].m_sampleContent.size () == 0) return ;

  string firstLayer = m_samples[origin].m_layersSequence.at(0); // take the first layer of the origin 
  addLayerToSample (target, firstLayer) ;
  
  for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[origin].m_sampleContent[firstLayer].m_histos.begin () ;
       iHisto != m_samples[origin].m_sampleContent[firstLayer].m_histos.end () ; ++iHisto){
      string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
      m_samples[target].m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
      m_samples[target].m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
  }
  
  for (size_t iLayer = 1 ; iLayer < m_samples[origin].m_layersSequence.size () ; ++iLayer)
      copyLayerInSample (target, m_samples[origin].m_layersSequence.at (iLayer), firstLayer) ;

  return ;    
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::printStructure() {

  // loop over samples
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ; iSample != m_samples.end () ; ++iSample){
      cout << iSample->first << "\t" 
           << iSample->second.m_sampleName << "\t"
           << iSample->second.m_XS << "\t"
           << iSample->second.m_totInitialEvents << "\t"
           << iSample->second.m_isSignal << "\t"
           << iSample->second.m_color << "\n" ;

      // loop over layers
      for (size_t iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer){
          string name = iSample->second.m_layersSequence.at (iLayer) ;
          
          cout << " + " << name << "\t" 
               << iSample->second.m_sampleContent[name].m_layerName << "\n" ;

          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto){
              cout << "    + " << iHisto->first << "\t"
                   << iHisto->second->GetName () << "\t"
                   << iHisto->second->GetEntries () << "\t"
                   << iHisto->second->GetNbinsX () << "\n" ;
            } // loop over histos
        } // loop over layers
    } // loop over samples
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::fillHisto (string sampleName, string layerName, string histoName, float value, float weight) {
  //PG NB assuming the histo is existing, not to slow down with cross-checks!
  m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName]->Fill (value, weight * m_samples[sampleName].m_weight) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::prepareSampleForPlotting (string sampleName){
  if (m_samples[sampleName].m_readyForPlotting) return ;

  // loop over layers
  for (size_t iLayer = 0 ; iLayer < m_samples[sampleName].m_layersSequence.size () ; ++iLayer){

      string name = m_samples[sampleName].m_layersSequence.at (iLayer) ;
      // loop over histos
      for (unordered_map<string, TH1F *>::iterator iHisto = m_samples[sampleName].m_sampleContent[name].m_histos.begin () ;
           iHisto != m_samples[sampleName].m_sampleContent[name].m_histos.end () ;++iHisto){
	addOverFlow (iHisto->second) ;  // add the overflow
        if (m_samples[sampleName].m_isSignal){ // if signal not filled
              iHisto->second->SetFillColor (0) ;
              iHisto->second->SetLineColor (m_samples[sampleName].m_color) ;
              iHisto->second->SetLineWidth (2) ;
	}
        else{ // if background filled           
              iHisto->second->SetFillColor (m_samples[sampleName].m_color) ;
              iHisto->second->SetLineColor (m_samples[sampleName].m_color) ;
	}
      } // loop over histos
  } // loop over layers
  m_samples[sampleName].m_readyForPlotting = true ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::prepareCanvas (float xmin, float xmax, float ymin, float ymax, string xaxisTitle, string yaxisTitle, bool hasPull){

  TH1F * bkg = m_canvas.DrawFrame(xmin, ymin, xmax, ymax) ;
  bkg->GetXaxis ()->SetTitle (xaxisTitle.c_str ()) ;
  bkg->GetYaxis ()->SetTitle (yaxisTitle.c_str ()) ;
  bkg->Draw () ;

  // prepare some banner style
  TLatex * tex = new TLatex(0.94,0.92," 13 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->Draw("same");
  TLatex * tex2 = new TLatex(0.14,0.92,"Delphes");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.04);
  tex2->SetLineWidth(2);
  tex2->Draw("same");
  TLatex * tex3 = new TLatex(0.286,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.035);
  tex3->SetLineWidth(2);
  tex3->Draw("same");
   
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::plotSingleSample (string sampleName, string layerName, string histoName,
                                string xaxisTitle, string yaxisTitle, int isLog, string folderName){ // plot just one sample, not stack

  prepareSampleForPlotting (sampleName) ; // prepare the sample

  TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ; // take the variable
  m_canvas.cd () ;

  TLegend leg = initLegend (1) ; // fill the legend
  leg.AddEntry (h_var, sampleName.c_str (), "fl") ;

  DrawPlots (vector<TH1F*>(1,h_var),leg,1,xaxisTitle,yaxisTitle,isLog,folderName) ; // Drawing function

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::plotSingleLayer (string layerName, string histoName, 
                               string xaxisTitle, string yaxisTitle, int isLog, string folderName){

  string name     = string ("st_") + layerName + "_" + histoName ;
  THStack * stack = new THStack (name.c_str (), "") ; // make a stack
  int nsamples    = m_samplesSequence.size () ; // number of samples

  TLegend leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){ // all the background goes in the stack
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add(h_var) ; // add to the stack
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ; // add to the legend
    }
  
  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){
      string sampleName = m_samplesSequence.at (iSample) ;
      if (m_samples[sampleName].m_isSignal == false) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add (h_var) ; // signal stack
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }

  DrawPlots (vector<THStack*> (1,stack),leg,m_samplesSequence.size(),xaxisTitle,yaxisTitle,isLog,folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::plotFullLayer (string layerName) {

  // FIXME needs to be finished --> make the output folder
  string outFolderName = "plots/stacks/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = m_samples.begin ()->second.m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto){
      // plot all the layer with and without log scale on the y-axis
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str (), "#sigma #times lumi", 1, outFolderName) ;
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str (), "#sigma #times lumi", 0, outFolderName) ;

    }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::compareStoB (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                           bool isNormalized, float scaleSignal, int isLog, string folderName){

  string    name      = string ("comp_bkg_") + layerName + "_" + histoName ;
  THStack  *bkg_stack = new THStack (name.c_str (), "") ;
  int nsamples        = m_samplesSequence.size () ;

  TLegend leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (size_t iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample) { // loop on the sample
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) continue ; // skip signal
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      bkg_stack->Add (h_var) ; // make bkg stack
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }
  
  name = string ("comp_sig_") + layerName + "_" + histoName ;
  THStack * sig_stack = new THStack (name.c_str (), "") ;

  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){
      string sampleName = m_samplesSequence.at (iSample) ;
      if (m_samples[sampleName].m_isSignal == false) continue ;
      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      sig_stack->Add (h_var) ; // namke signal stack
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
    }

  vector<THStack *> histos ;
  histos.push_back (bkg_stack) ;
  histos.push_back (sig_stack) ;
  
  DrawPlots (histos, leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ; // draw the plots

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::compareStoBFullLayer (string layerName) {

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
                                  string folderName){

  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  string name        = string ("st_SM_") + layerName + "_" + histoName ; // SM = QCD bkg + EWK 126 GeV H
  THStack * SM_stack = new THStack (name.c_str (), "") ;
  name               = string ("st_nH_") + layerName + "_" + histoName ;  // no H = QCD bkg + EWK noH
  THStack * nH_stack = new THStack (name.c_str (), "") ;

  int nsamples = m_samplesSequence.size () ;
  TLegend leg = initLegend (nsamples + 1) ;

  // add bkg to the stack and identify the position of the two signals in the sample
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  int sigSamplesNum = 0 ;
  vector<int> signalPos (2, 0) ;
  for (size_t iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample) {
      string sampleName = m_samplesSequence.at (iSample) ;
      prepareSampleForPlotting (sampleName) ;
      if (m_samples[sampleName].m_isSignal == true) {
          signalPos.at (sigSamplesNum) = iSample ;
          ++sigSamplesNum ;
          continue ;
      }

      TH1F * h_var = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;
      SM_stack->Add (h_var) ;
      nH_stack->Add (h_var) ;
      leg.AddEntry (h_var, sampleName.c_str (), "fl") ;
  }

  if (2 != sigSamplesNum){
      cout << "ERROR: found " << sigSamplesNum << " signal samples, exiting" << endl ;
      exit (1) ;
  }

  // add the first signal to the stack, to bild up the total SM expectations
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  string sampleName = m_samplesSequence.at (signalPos.at (0)) ;
  TH1F * h_sigSM    = m_samples[sampleName].m_sampleContent[layerName].m_histos[histoName] ;  
  SM_stack->Add(h_sigSM) ;
  leg.AddEntry(h_sigSM, sampleName.c_str (), "fl") ;

  // calculate the difference between noH and SM126
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  sampleName   = m_samplesSequence.at (signalPos.at (1)) ;
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
  THStack * delta_stack = CreateStack(diff) ;
  vector<THStack *> histos ;  // ensemble of objects to be drawn together
  histos.push_back (SM_stack) ;  
  histos.push_back (delta_stack) ;  
  
  // histogram to plot the error band of the SM bkg + 126 Higgs THstack 
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  name = "SM_histo_" ;
  name += h_sigSM->GetName () ;
  TH1F * SM_histo = (TH1F *) SM_stack->GetStack ()->Last ()->Clone (name.c_str ()) ; // take the last as the background expectation
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
  histos.push_back (CreateStack(nH_histo)) ;

  // do the drawing
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  DrawPlots (histos, leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  SM_histo->Draw ("E2same") ; // draw the error band on the SM THstack and then reprint the canvas 

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

  TLegend leg2        = initLegend (2) ;
  TH1F * h_tot_SM     = (TH1F *) SM_stack->GetStack ()->Last () ;
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
void plotter::plotRelativeExcessFullLayer (string layerName, string basefolder) {
  // FIXME needs to be finished
  string outFolderName = basefolder + "/plots/relExcess/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.m_sampleContent[layerName].m_histos.end () ; ++iHisto){
      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                          0, 1., 0, outFolderName) ;
      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), "#sigma #times lumi", 
                          0, 1., 1, outFolderName) ;
  }
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::scaleAllHistos (float scaleFactor){
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ; ++iSample) {
      // loop over layers
      for (size_t iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer){

          string name = iSample->second.m_layersSequence.at (iLayer) ;          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ; ++iHisto){
              iHisto->second->Scale (scaleFactor) ;
	  }// loop over histos
      }// loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::setPoissonErrors () {

  cout << "WARNING setting all the histograms to the poisson error\n" ;
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ; ++iSample){
    
      // loop over layers
    for (size_t iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer){        
          string name = iSample->second.m_layersSequence.at (iLayer) ;          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ;
               ++iHisto){
            
              setPoissonErrorsToHisto (iHisto->second) ;
	  }// loop over histos
    }// loop over layers
  }// loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::resetAll (float lumi){

  cout << "WARNING resetting all the stored quantities\n" ;
  cout << "luminosity set to " << lumi << "\n" ;
  m_lumi = lumi ;
  for (unordered_map<string, sample>::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ; ++iSample){
    
      // loop over layers
    for (size_t iLayer = 0 ; iLayer < iSample->second.m_layersSequence.size () ; ++iLayer){        
          string name = iSample->second.m_layersSequence.at (iLayer) ;          
          // loop over histos
          for (unordered_map<string, TH1F *>::iterator iHisto = iSample->second.m_sampleContent[name].m_histos.begin () ;
               iHisto != iSample->second.m_sampleContent[name].m_histos.end () ; ++iHisto){            
              iHisto->second->Reset () ;
	  } // loop over histos
    }// loop over layers
  }// loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
TLegend plotter::initLegend (int sampleNum){
  TLegend leg (0.2, 0.95 - 0.1 * sampleNum * 0.33, 0.87, 0.95) ;
  leg.SetNColumns (3) ;
  leg.SetLineStyle (0) ;
  leg.SetFillStyle (0) ;
  return leg ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::setRootAspect (){

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  // For the canvas:                                                                                                                                                           
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefX(0);   //POsition on screen                                                                                                                             
  gStyle->SetCanvasDefY(0);
  gStyle->SetCanvasColor (-1) ; 
  m_canvas.SetTickx(1);
  m_canvas.SetTicky(1);

  // For the Pad:               
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:                                                                                                                                                                    
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  // For the histo:                                                                                                                                                                       
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  gStyle->SetEndErrorSize(2);
  gStyle->SetMarkerStyle(20);

  //For the fit/function:                                                                                                                                                                  
  gStyle->SetOptFit(0);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

  //For the date:                                                                                                                                                                          
  gStyle->SetOptDate(0);

  // For the statistics box:                                                                                                                                                              
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");                                                                                                             
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.03);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.15);
  gStyle->SetStatW(0.2);

  // Margins:                                                                                                                                                                              
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.15);

  // For the Global title:                                                                                                                                                                 
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);

  // For the axis titles:                                                                                                                                                                  
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.05, "XYZ");
  gStyle->SetTitleXOffset(1.10);
  gStyle->SetTitleYOffset(1.30);
  gStyle->SetTitleOffset(1.30, "Z");

  // For the axis labels:                                                                                                                                                                  
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.005, "XYZ");
  gStyle->SetLabelSize(0.03, "XYZ");

  // For the axis:                                                                                                                                                                         
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame                                                                                                        
  gStyle->SetPadTickY(1);

  // Change for log plots:                                                                                                                                                                 
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);


}

template <class T>
void plotter::DrawPlots (vector<T*> histo, TLegend leg, int sampleNum,
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
  float height   = 0.17 ;
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

