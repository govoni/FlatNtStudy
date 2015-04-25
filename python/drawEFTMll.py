#! /usr/bin/env python                                                                                                                                                          
import os
import glob
import math
from array import array
import numpy as numpy
import sys
import time
import subprocess
import ROOT

from optparse import OptionParser
from subprocess import Popen

from ROOT import gROOT, gStyle, gSystem, TLatex, TGaxis, TPaveText, TH2D, TColor, gPad, TGraph2D, TLine,TGraph,TList

from collections import defaultdict

############################################                                                                                                                                   
#            Job steering                  #                                                                                                                                    
############################################                                                                                                                                   

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=True, help='no X11 windows')

(options, args) = parser.parse_args()

#####################################                                                                                                                                           
###### definition of the style ######                                                                                                                                           
#####################################                                                                                                                                            

def setStyle():

  gStyle.SetPadBorderMode(0);
  gStyle.SetFrameBorderMode(0);
  gStyle.SetPadBottomMargin(0.12);
  gStyle.SetPadLeftMargin(0.12);
  gStyle.SetCanvasColor(ROOT.kWhite);
  gStyle.SetCanvasDefH(600); #Height of canvas                                                                                                                                  
  gStyle.SetCanvasDefW(600); #Width of canvas                                                                                                                                   
  gStyle.SetCanvasDefX(0);   #POsition on screen                                                                                                                                
  gStyle.SetCanvasDefY(0);
  gStyle.SetPadTopMargin(0.06);
  gStyle.SetPadBottomMargin(0.12);#0.13);                                                                                                                                       
  gStyle.SetPadLeftMargin(0.13);#0.16);                                                                                                                                         
  gStyle.SetPadRightMargin(0.07);#0.02);                                                                                                                                         

  # For the Pad:                                                                                                                                                                
  gStyle.SetPadBorderMode(0);
  gStyle.SetPadColor(ROOT.kWhite);
  gStyle.SetPadGridX(ROOT.kFALSE);
  gStyle.SetPadGridY(ROOT.kFALSE);
  gStyle.SetGridColor(0);
  gStyle.SetGridStyle(3);
  gStyle.SetGridWidth(1);

  # For the frame:                                                                                                                                                              
  gStyle.SetFrameBorderMode(0);
  gStyle.SetFrameBorderSize(1);
  gStyle.SetFrameFillColor(0);
  gStyle.SetFrameFillStyle(0);
  gStyle.SetFrameLineColor(1);
  gStyle.SetFrameLineStyle(1);
  gStyle.SetFrameLineWidth(1);

  gStyle.SetAxisColor(1, "XYZ");
  gStyle.SetStripDecimals(ROOT.kTRUE);
  gStyle.SetTickLength(0.03, "XYZ");
  gStyle.SetNdivisions(505, "XYZ");
  gStyle.SetPadTickX(1);  # To get tick marks on the opposite side of the frame                                                                                                
  gStyle.SetPadTickY(1);
  gStyle.SetGridColor(0);
  gStyle.SetGridStyle(3);
  gStyle.SetGridWidth(1);

  gStyle.SetTitleColor(1, "XYZ");
  gStyle.SetTitleFont(42, "XYZ");
  gStyle.SetTitleSize(0.05, "XYZ");
  gStyle.SetTitleXOffset(1.15);#0.9);                                                                                                                                            
  gStyle.SetTitleYOffset(1.3); # => 1.15 if exponents                                                                                                                           
  gStyle.SetLabelColor(1, "XYZ");
  gStyle.SetLabelFont(42, "XYZ");
  gStyle.SetLabelOffset(0.007, "XYZ");
  gStyle.SetLabelSize(0.045, "XYZ");

  gStyle.SetPadBorderMode(0);
  gStyle.SetFrameBorderMode(0);
  gStyle.SetTitleTextColor(1);
  gStyle.SetTitleFillColor(10);

  gStyle.SetOptStat(0);
  gStyle.SetOptTitle(0)
  gStyle.SetOptFit(0)

  NRGBs = 5
  NCont = 255
  stops = [ 0.00, 0.34, 0.61, 0.84, 1.00 ]
  red   = [ 0.00, 0.00, 0.87, 1.00, 0.51 ]
  green = [ 0.00, 0.81, 1.00, 0.20, 0.00 ]
  blue  = [ 0.51, 1.00, 0.12, 0.00, 0.00 ]
  stopsArray = array('d', stops)
  redArray   = array('d', red)
  greenArray = array('d', green)
  blueArray  = array('d', blue)
  TColor.CreateGradientColorTable(NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
  gStyle.SetNumberContours(NCont)


##################################                                                                                                                                              
########### Main Code ############                                                                                                                                              
##################################                                                                                                                                             

if __name__ == '__main__':

  os.system("mkdir -p output/EFTplots/");
  os.system("rm output/EFTplots/*");

    ## set the style                                                                                                                                                            
  setStyle();
    
  tex2 = TLatex(0.17,0.86,"CMS Phase II Delphes Simulation");
  tex2.SetNDC(1);
  tex2.SetTextAlign(11);
  tex2.SetTextFont(61);
  tex2.SetLineWidth(2);
  tex2.SetTextSize(0.04);

  tex3 = ROOT.TLatex(0.53,0.95,"14 TeV, 3000 fb^{-1}, 140PU");
  tex3.SetNDC(1);
  tex3.SetTextAlign(11);
  tex3.SetTextFont(42);
  tex3.SetTextSize(0.04);
  tex3.SetLineWidth(2);
  
  ROOT.TH1.SetDefaultSumw2();
  SMcards = "/afs/cern.ch/user/j/jlauwers/public/TP/Cards/PhaseII_140PU_All/";
  EFTFit  = "/afs/cern.ch/user/j/jlauwers/public/TP/EFTfits/PhaseII_140PU_New/";
  
  fileSM  = ROOT.TFile(SMcards + "final_mll_All.root", "READ");
  hSignal = ROOT.TH1F();
  hSignal = fileSM.Get("histo_WW_EWK_SS_126");
  hBackg  = ROOT.TH1F();
  hBackg  = fileSM.Get("histo_WZ_QCD");
  hBackg.Add(fileSM.Get("histo_wrongCharge") );
  hBackg.Add(fileSM.Get("histo_WW_QCD") );
  hBackg.Add(fileSM.Get("histo_WZ_EWK") );
  hBackg.Add(fileSM.Get("histo_fakeLepton") );

  bins = hSignal.GetNbinsX();
  Noperators = 9;
  opName = ["S0","S1","M0","M1","M6","M7","T0","T1","T2"];
  opVal  = [1.1,3.5,0.75,1.03,1.50,1.3,0.07,0.03,0.11];
  opVal2 = [2.2,7.0,1.50,2.06,3.00,2.5,0.14,0.06,0.22];

  binning = array('f',[40,250,400,600,700,900,1200,1500]);
  hBackg_2  = ROOT.TH1F("hBackg_2","hBackg_2",bins,binning);
  hSignal_2 = ROOT.TH1F("hSignal_2","hSignal_2",bins,binning);

  for iBin in range(bins):
    hBackg_2.SetBinContent(iBin+1,  hBackg.GetBinContent(iBin+1));
    hBackg_2.SetBinError(iBin+1,    hBackg.GetBinError(iBin+1));
    hSignal_2.SetBinContent(iBin+1, hSignal.GetBinContent(iBin+1));
    hSignal_2.SetBinError(iBin+1,   hSignal.GetBinError(iBin+1));

  hSM_total = hBackg_2.Clone("hBackg_2");
  hSM_total.Add(hSignal_2);

  fileOut = ROOT.TFile("tmp.root","RECREATE");

  for  iOp in range(Noperators):

        fitFile = ROOT.TFile( EFTFit + "signal_WWVBS_mll_L"+opName[iOp]+".root", "READ");  
        hEFT   = hSignal_2.Clone("Sig"+opName[iOp]);

        hEFT_2 = hSignal_2.Clone("Sig"+opName[iOp]+"_2");
        
        for iBin in range(bins) :
            func = ROOT.TF1();
            func = fitFile.Get("bin_function_%d"%(iBin));
            
            hEFT.SetBinContent(iBin+1, hSignal_2.GetBinContent(iBin+1) * (func.Eval(opVal[iOp])) );
            hEFT.SetBinError  (iBin+1, hSignal_2.GetBinError(iBin+1) * (func.Eval(opVal[iOp])) );

            hEFT_2.SetBinContent(iBin+1, hSignal_2.GetBinContent(iBin+1) * (func.Eval(opVal2[iOp])) );
            hEFT_2.SetBinError  (iBin+1, hSignal_2.GetBinError(iBin+1) * (func.Eval(opVal2[iOp])) );
        
        hEFT.Add(hBackg_2);
        hEFT_2.Add(hBackg_2);
        
        can = ROOT.TCanvas("mll_"+opName[iOp],"mll_"+opName[iOp],600,600);

        hSM_total.GetXaxis().SetRangeUser(600.,hSM_total.GetXaxis().GetXmax());
        hEFT.GetXaxis().SetRangeUser(600.,hSM_total.GetXaxis().GetXmax());
        hEFT_2.GetXaxis().SetRangeUser(600.,hSM_total.GetXaxis().GetXmax());

        frame = can.DrawFrame(600.,0.5,hSM_total.GetXaxis().GetXmax(),max(hSM_total.GetMaximum(),max(hEFT.GetMaximum(),hEFT_2.GetMaximum()))*10);

        legend = ROOT.TLegend(0.52,0.58,0.70,0.75); 

        frame.GetXaxis().SetTitle("m_{ll} (GeV)");
        frame.GetYaxis().SetTitle("Events / bin");

        legend.SetTextSize(0.045);
        legend.SetTextFont(42);
        legend.SetFillColor(0);
        legend.SetFillStyle(0);
        legend.SetBorderSize(0);

        hSM_total.SetLineColor(1);
        hSM_total.SetLineWidth(2);
        hSM_total.GetXaxis().SetNdivisions(205);
        hSM_total.Draw("histsame");
        legend.AddEntry(hSM_total," Standard Model","l");
        
        hEFT.SetLineWidth(2);
        hEFT.SetLineColor(4);
        hEFT.SetLineStyle(9);
        hEFT.SetMarkerStyle(20);
        hEFT.SetMarkerColor(4);
        hEFT.SetMarkerSize(1.2);        
        hEFT.Draw("plsame");
        
        hEFT_2.SetLineWidth(2);
        hEFT_2.SetLineColor(2);
        hEFT_2.SetLineStyle(7);
        hEFT_2.SetMarkerStyle(20);
        hEFT_2.SetMarkerColor(2);
        hEFT_2.SetMarkerSize(1.2);        
        hEFT_2.Draw("plsame");

        opNameLegend = opName[iOp].replace("0","_{0}").replace("1","_{1}").replace("2","_{2}").replace("3","_{3}").replace("4","_{4}").replace("5","_{5}").replace("6","_{6}").replace("7","_{7}");

        legend.AddEntry(hEFT," %s = %1.2f TeV^{-4}"%(opNameLegend,opVal[iOp]),"pl");
        legend.AddEntry(hEFT_2," %s = %1.2f TeV^{-4}"%(opNameLegend,opVal2[iOp]),"pl");
        legend.Draw();
        can.SetLogy();
        
        tex2.Draw();
        tex3.Draw();
        
        can.RedrawAxis();
        can.Print("output/EFTplots/"+can.GetName()+".png","png");
        can.Print("output/EFTplots/"+can.GetName()+".pdf","pdf");
        can.Print("output/EFTplots/"+can.GetName()+".root","root");
        can.Print("output/EFTplots/"+can.GetName()+".C","C");
        
        
