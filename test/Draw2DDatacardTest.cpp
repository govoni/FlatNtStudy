#include <iostream>

#include "plotter.h"
#include "utils.h"

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TString.h"
#include "TCanvas.h"

#include "RooPlot.h"
#include "RooHist.h"
#include "RooCurve.h"

using namespace std ;
//using namespace roofit ;



int main (int argc, char ** argv) 
{

  // binning, to be given from outside
  int nBinsX = 10 ;
  float Xmin = 0. ;
  float Xmax = 10. ;

  int nBinsY = 15 ;
  float Ymin = 0. ;
  float Ymax = 600. ;

 
  // maximum likelihood fit output from combine
  TFile f ("mlfitvbfcuts_R_mll_DF.root") ;
  RooPlot * SB_fit = (RooPlot *) f.Get ("vbfcuts_DF_fit_s") ;
  RooHist * events = (RooHist *) SB_fit->getObject (0) ;
  RooCurve * RC_sig = (RooCurve *) SB_fit->getObject (1) ;
  RooCurve * RC_bkg = (RooCurve *) SB_fit->getObject (2) ;
  RooCurve * RC_tot = (RooCurve *) SB_fit->getObject (3) ;

  TH2F * roll_tot = roll<RooCurve> (RC_tot, nBinsX, Xmin, Xmax, nBinsY, Ymin, Ymax, events) ;
  TH2F * roll_evt = roll<RooHist>  (events, nBinsX, Xmin, Xmax, nBinsY, Ymin, Ymax, events) ;

  TCanvas c1 ;
  c1.SetLogz () ;

  roll_tot->SetStats (0) ;
  roll_tot->Draw ("colz") ;
  c1.Print ("fitted.png", "png") ;

  roll_evt->SetStats (0) ;
  roll_evt->Draw ("colz") ;
  c1.Print ("evenp[ts.png", "png") ;

  TString name = roll_evt->GetName () ;
  name += "_diff" ;
  TH2F * roll_diff = (TH2F *) roll_evt->Clone (name) ;
  roll_diff->SetTitle (name) ;
  roll_diff->Add (roll_tot, -1) ;
  
  c1.SetLogz (0) ;
  roll_diff->SetStats (0) ;
  roll_diff->Draw ("colz") ;
  c1.Print ("diff.png", "png") ;

//  c1.SetLogz () ;
  roll_evt->Draw ("E") ;
  roll_tot->Draw ("surf same") ;
  c1.Print ("3Dcompare.png", "png") ;

  TFile outfile ("output.root","recreate") ;
  roll_tot->Write () ;
  roll_evt->Write () ;
  roll_diff->Write () ;
  outfile.Close () ;

  return 0 ;

  

  return 0 ;
}