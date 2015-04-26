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
  gStyle.SetPadTopMargin(0.05);
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
  gStyle.SetTitleFontSize(0.05);

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

    ## set the style
    setStyle();

    tex = ROOT.TLatex(0.82,0.96,"14 TeV");
    tex.SetNDC(1);
    tex.SetTextAlign(11);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);

    tex2 = TLatex(0.17,0.86,"CMS Delphes Simulation");
    tex2.SetNDC(1);
    tex2.SetTextAlign(11);
    tex2.SetTextFont(61);    
    tex2.SetLineWidth(2);
    tex2.SetTextSize(0.04);

    tex4 = TLatex(0.17,0.80,"WW WZ combination");
    tex4.SetNDC(1);
    tex4.SetTextAlign(11);
    tex4.SetTextFont(42);    
    tex4.SetLineWidth(2);
    tex4.SetTextSize(0.04);

    tex3 = ROOT.TLatex(0.67,0.96,"14 TeV, 3000 fb^{-1}");
    tex3.SetNDC(1);
    tex3.SetTextAlign(11);
    tex3.SetTextFont(42);
    tex3.SetTextSize(0.04);
    tex3.SetLineWidth(2);

    ## input file for phaseI 50PU, aged and phaseII for polarized signficance vs lumi
    filePolarizedPhaseI_vsLumi  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseI/Polarized/lumiExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsLumi.root","OPEN");

    significancePhaseI_vsLumi = ROOT.TGraphAsymmErrors();
    significancePhaseI_vsLumi = filePolarizedPhaseI_vsLumi.Get("signficance_graph");

    filePolarizedPhaseII_vsLumi = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseII/Polarized/lumiExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsLumi.root","OPEN");

    significancePhaseII_vsLumi = ROOT.TGraphAsymmErrors();
    significancePhaseII_vsLumi = filePolarizedPhaseII_vsLumi.Get("signficance_graph");

    filePolarizedAged_vsLumi    = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/Aged/Polarized/lumiExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsLumi.root","OPEN");
    significanceAged_vsLumi = ROOT.TGraphAsymmErrors();
    significanceAged_vsLumi = filePolarizedAged_vsLumi.Get("signficance_graph");

    can = ROOT.TCanvas("can","can",600,600);
    
    legend = ROOT.TLegend(0.45,0.25,0.85,0.45);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetTextSize(0.031);


    frame = can.DrawFrame(significancePhaseI_vsLumi.GetXaxis().GetXmin(),0.,significancePhaseI_vsLumi.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(significancePhaseI_vsLumi.GetN(),significancePhaseI_vsLumi.GetY()),
                          max(ROOT.TMath.MaxElement(significancePhaseII_vsLumi.GetN(),significancePhaseII_vsLumi.GetY()),
                              ROOT.TMath.MaxElement(significanceAged_vsLumi.GetN(),significanceAged_vsLumi.GetY())))*1.2);

    frame.GetXaxis().SetTitle("Luminosity (fb^{-1})");
    frame.GetXaxis().SetTitleSize(0.045);
    frame.GetXaxis().SetLabelSize(0.04);

    frame.GetYaxis().SetTitle("Expected significance (#sigma)");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);
    

    significancePhaseI_vsLumi.SetMarkerSize(1.1);
    significancePhaseI_vsLumi.SetLineStyle(1);
    significancePhaseI_vsLumi.Draw("pl");

    significancePhaseII_vsLumi.SetMarkerSize(1.1);
    significancePhaseII_vsLumi.SetLineStyle(1);
    significancePhaseII_vsLumi.SetLineColor(ROOT.kRed);
    significancePhaseII_vsLumi.SetMarkerColor(ROOT.kRed);
    significancePhaseII_vsLumi.Draw("plsame");

    significanceAged_vsLumi.SetMarkerSize(1.1);
    significanceAged_vsLumi.SetLineStyle(1);
    significanceAged_vsLumi.SetLineColor(ROOT.kBlue);
    significanceAged_vsLumi.SetMarkerColor(ROOT.kBlue);
    significanceAged_vsLumi.Draw("plsame");

    tex.Draw();
    tex2.Draw();
    tex4.Draw();

    legend.AddEntry(significancePhaseI_vsLumi,"Phase I 50 PU","pl");
    legend.AddEntry(significanceAged_vsLumi,  "Phase I aged 140 PU","pl");
    legend.AddEntry(significancePhaseII_vsLumi,"Phase II 140 PU","pl");
    legend.Draw();

    os.system("mkdir -p output/Combination/");
    os.system("rm output/Combination/*");

    can.RedrawAxis();

    can.SaveAs("output/Combination/LL_scattering_vsLumi.png","png");
    can.SaveAs("output/Combination/LL_scattering_vsLumi.pdf","pdf");
    can.SaveAs("output/Combination/LL_scattering_vsLumi.root","root");
    can.SaveAs("output/Combination/LL_scattering_vsLumi.C","C");

    ## input file for phaseI 50PU, aged and phaseII for polarized signficance vs fake
    filePolarizedPhaseI_vsFake  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseI/Polarized/fakeRateExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsFake.root","OPEN");

    significancePhaseI_vsFake = ROOT.TGraphAsymmErrors();
    significancePhaseI_vsFake = filePolarizedPhaseI_vsFake.Get("signficance_graph");

    filePolarizedPhaseII_vsFake  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseII/Polarized/fakeRateExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsFake.root","OPEN");

    significancePhaseII_vsFake = ROOT.TGraphAsymmErrors();
    significancePhaseII_vsFake = filePolarizedPhaseII_vsFake.Get("signficance_graph");

    filePolarizedAged_vsFake  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/Aged/Polarized/fakeRateExtrapolation/computeProfileLikelihood/plotProfileLikelihood/ProfileLikelihood_COMB_vsFake.root","OPEN");
    significanceAged_vsFake = ROOT.TGraphAsymmErrors();
    significanceAged_vsFake = filePolarizedAged_vsFake.Get("signficance_graph");

    legend.Clear();

    can2   = ROOT.TCanvas("can2","can2",600,600);

    frame2 = can2.DrawFrame(significancePhaseI_vsFake.GetXaxis().GetXmin(),
                          min(ROOT.TMath.MinElement(significancePhaseI_vsFake.GetN(),significancePhaseI_vsFake.GetY()),
                          min(ROOT.TMath.MinElement(significancePhaseII_vsFake.GetN(),significancePhaseII_vsFake.GetY()),
                              ROOT.TMath.MinElement(significanceAged_vsFake.GetN(),significanceAged_vsFake.GetY())))*0.7,                            
                            significancePhaseI_vsFake.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(significancePhaseI_vsFake.GetN(),significancePhaseI_vsFake.GetY()),
                          max(ROOT.TMath.MaxElement(significancePhaseII_vsFake.GetN(),significancePhaseII_vsFake.GetY()),
                              ROOT.TMath.MaxElement(significanceAged_vsFake.GetN(),significanceAged_vsFake.GetY())))*1.2);

    frame2.GetXaxis().SetTitle("fake rate scale factor");
    frame2.GetXaxis().SetTitleSize(0.045);
    frame2.GetXaxis().SetLabelSize(0.04);

    frame2.GetYaxis().SetTitle("Expected significance (#sigma)");
    frame2.GetYaxis().SetTitleSize(0.045);
    frame2.GetYaxis().SetTitleOffset(1.17);
    frame2.GetYaxis().SetLabelSize(0.04);


    significancePhaseI_vsFake.SetMarkerSize(1.1);
    significancePhaseI_vsFake.SetLineStyle(1);
    significancePhaseI_vsFake.Draw("pl");
    significancePhaseII_vsFake.SetMarkerSize(1.1);
    significancePhaseII_vsFake.SetLineStyle(1);
    significancePhaseII_vsFake.SetLineColor(ROOT.kRed);
    significancePhaseII_vsFake.SetMarkerColor(ROOT.kRed);
    significancePhaseII_vsFake.Draw("plsame");
    significanceAged_vsFake.SetMarkerSize(1.1);
    significanceAged_vsFake.SetLineStyle(1);
    significanceAged_vsFake.SetLineColor(ROOT.kBlue);
    significanceAged_vsFake.SetMarkerColor(ROOT.kBlue);
    significanceAged_vsFake.Draw("plsame");

    tex3.Draw();
    tex2.Draw();
    tex4.Draw();

    legend2 = ROOT.TLegend(0.45,0.55,0.85,0.75);
    legend2.SetFillColor(0);
    legend2.SetFillStyle(0);
    legend2.SetBorderSize(0);
    legend2.SetTextSize(0.031);

    legend2.AddEntry(significancePhaseI_vsFake,  "Phase I 50 PU","pl");
    legend2.AddEntry(significanceAged_vsFake,    "Phase I aged 140 PU","pl");
    legend2.AddEntry(significancePhaseII_vsFake, "Phase II 140 PU","pl");
    legend2.Draw();

    can2.RedrawAxis();
    
    can2.SaveAs("output/Combination/LL_scattering_vsFake.png","png");
    can2.SaveAs("output/Combination/LL_scattering_vsFake.pdf","pdf");
    can2.SaveAs("output/Combination/LL_scattering_vsFake.root","root");
    can2.SaveAs("output/Combination/LL_scattering_vsFake.C","C");

    ## input file for phaseI 50PU, aged and phaseII for noH-H signficance vs lumi
    fileHvsNoHPhaseI_vsLumi  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseI/HvsNoH/lumiExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");

    limitPhaseI_vsLumi = ROOT.TGraphAsymmErrors();
    limitPhaseI_vsLumi = fileHvsNoHPhaseI_vsLumi.Get("limit_exp");

    fileHvsNoHPhaseII_vsLumi = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseII/HvsNoH/lumiExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");

    limitPhaseII_vsLumi = ROOT.TGraphAsymmErrors();
    limitPhaseII_vsLumi = fileHvsNoHPhaseII_vsLumi.Get("limit_exp");

    fileHvsNoHAged_vsLumi   = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/Aged/HvsNoH/lumiExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");
    limitAged_vsLumi = ROOT.TGraphAsymmErrors();
    limitAged_vsLumi = fileHvsNoHAged_vsLumi.Get("limit_exp");


    can3 = ROOT.TCanvas("can3","can3",600,600);

    legend3 = ROOT.TLegend(0.45,0.45,0.85,0.65);
    legend3.SetFillColor(0);
    legend3.SetFillStyle(0);
    legend3.SetBorderSize(0);
    legend3.SetTextSize(0.031);


    frame3 = can3.DrawFrame(limitPhaseI_vsLumi.GetXaxis().GetXmin(),
                          min(ROOT.TMath.MinElement(limitPhaseI_vsLumi.GetN(),limitPhaseI_vsLumi.GetY()),
                          min(ROOT.TMath.MinElement(limitPhaseII_vsLumi.GetN(),limitPhaseII_vsLumi.GetY()),
                              ROOT.TMath.MinElement(limitAged_vsLumi.GetN(),limitAged_vsLumi.GetY())))*0.7,
                            limitPhaseI_vsLumi.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(limitPhaseI_vsLumi.GetN(),limitPhaseI_vsLumi.GetY()),
                          max(ROOT.TMath.MaxElement(limitPhaseII_vsLumi.GetN(),limitPhaseII_vsLumi.GetY()),
                              ROOT.TMath.MaxElement(limitAged_vsLumi.GetN(),limitAged_vsLumi.GetY())))*1.25);

    frame3.GetXaxis().SetTitle("Luminosity (fb^{-1})");
    frame3.GetXaxis().SetTitleSize(0.045);
    frame3.GetXaxis().SetLabelSize(0.04);

    frame3.GetYaxis().SetTitle("exclusion limit 95% C.L.");
    frame3.GetYaxis().SetTitleSize(0.045);
    frame3.GetYaxis().SetTitleOffset(1.20);
    frame3.GetYaxis().SetLabelSize(0.04);
    

    limitPhaseI_vsLumi.SetMarkerSize(1.1);
    limitPhaseI_vsLumi.SetLineStyle(1);
    limitPhaseI_vsLumi.Draw("pl");

    limitPhaseII_vsLumi.SetMarkerSize(1.1);
    limitPhaseII_vsLumi.SetLineStyle(1);
    limitPhaseII_vsLumi.SetLineColor(ROOT.kRed);
    limitPhaseII_vsLumi.SetMarkerColor(ROOT.kRed);
    limitPhaseII_vsLumi.Draw("plsame");

    limitAged_vsLumi.SetMarkerSize(1.1);
    limitAged_vsLumi.SetLineStyle(1);
    limitAged_vsLumi.SetLineColor(ROOT.kBlue);
    limitAged_vsLumi.SetMarkerColor(ROOT.kBlue);
    limitAged_vsLumi.Draw("plsame");

    tex.Draw();
    tex2.Draw();
    tex4.Draw();

    legend3.AddEntry(limitPhaseI_vsLumi, "Phase I 50 PU","pl");
    legend3.AddEntry(limitAged_vsLumi,   "Phase I aged 140 PU","pl");
    legend3.AddEntry(limitPhaseII_vsLumi,"Phase II 140 PU","pl");
    legend3.Draw();

    can3.SetLogy();
    can3.RedrawAxis();

    can3.SaveAs("output/Combination/HvsNoH_vsLumi.png","png");
    can3.SaveAs("output/Combination/HvsNoH_vsLumi.pdf","pdf");
    can3.SaveAs("output/Combination/HvsNoH_vsLumi.root","root");
    can3.SaveAs("output/Combination/HvsNoH_vsLumi.C","C");


    ## input file for phaseII 140PU, aged and phaseII for noH-H signficance vs lumi
    fileHvsNoHPhaseI_vsFake  = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseI/HvsNoH/fakeRateExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");

    limitPhaseI_vsFake = ROOT.TGraphAsymmErrors();
    limitPhaseI_vsFake = fileHvsNoHPhaseI_vsFake.Get("limit_exp");

    fileHvsNoHPhaseII_vsFake = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/PhaseII/HvsNoH/fakeRateExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");

    limitPhaseII_vsFake = ROOT.TGraphAsymmErrors();
    limitPhaseII_vsFake = fileHvsNoHPhaseII_vsFake.Get("limit_exp");

    fileHvsNoHAged_vsFake   = ROOT.TFile("output/DataCards_Combination_WW_WZ_v2/Aged/HvsNoH/fakeRateExtrapolation/computeAsymptotic/plotAsymptotic/Asymptotic_COMB.root","OPEN");
    limitAged_vsFake = ROOT.TGraphAsymmErrors();
    limitAged_vsFake = fileHvsNoHAged_vsFake.Get("limit_exp");


    can4 = ROOT.TCanvas("can4","can4",600,600);

    legend4 = ROOT.TLegend(0.45,0.22,0.85,0.42);
    legend4.SetFillColor(0);
    legend4.SetFillStyle(0);
    legend4.SetBorderSize(0);
    legend4.SetTextSize(0.031);


    frame4 = can4.DrawFrame(limitPhaseI_vsFake.GetXaxis().GetXmin(),
                          min(ROOT.TMath.MinElement(limitPhaseI_vsFake.GetN(),limitPhaseI_vsFake.GetY()),
                          min(ROOT.TMath.MinElement(limitPhaseII_vsFake.GetN(),limitPhaseII_vsFake.GetY()),
                              ROOT.TMath.MinElement(limitAged_vsFake.GetN(),limitAged_vsFake.GetY())))*0.4,                            
                            limitPhaseI_vsFake.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(limitPhaseI_vsFake.GetN(),limitPhaseI_vsFake.GetY()),
                          max(ROOT.TMath.MaxElement(limitPhaseII_vsFake.GetN(),limitPhaseII_vsFake.GetY()),
                              ROOT.TMath.MaxElement(limitAged_vsFake.GetN(),limitAged_vsFake.GetY())))*1.15);

    frame4.GetXaxis().SetTitle("fake rate scale factor");
    frame4.GetXaxis().SetTitleSize(0.045);
    frame4.GetXaxis().SetLabelSize(0.04);

    frame4.GetYaxis().SetTitle("exclusion limit 95% C.L.");
    frame4.GetYaxis().SetTitleSize(0.045);
    frame4.GetYaxis().SetTitleOffset(1.20);
    frame4.GetYaxis().SetLabelSize(0.04);
    

    limitPhaseI_vsFake.SetMarkerSize(1.1);
    limitPhaseI_vsFake.SetLineStyle(1);
    limitPhaseI_vsFake.Draw("pl");

    limitPhaseII_vsFake.SetMarkerSize(1.1);
    limitPhaseII_vsFake.SetLineStyle(1);
    limitPhaseII_vsFake.SetLineColor(ROOT.kRed);
    limitPhaseII_vsFake.SetMarkerColor(ROOT.kRed);
    limitPhaseII_vsFake.Draw("plsame");

    limitAged_vsFake.SetMarkerSize(1.1);
    limitAged_vsFake.SetLineStyle(1);
    limitAged_vsFake.SetLineColor(ROOT.kBlue);
    limitAged_vsFake.SetMarkerColor(ROOT.kBlue);
    limitAged_vsFake.Draw("plsame");

    tex3.Draw();
    tex2.Draw();
    tex4.Draw();

    legend4.AddEntry(limitPhaseI_vsFake, "Phase I 50 PU","pl");
    legend4.AddEntry(limitAged_vsFake,   "Phase I aged 140 PU","pl");
    legend4.AddEntry(limitPhaseII_vsFake,"Phase II 140 PU","pl");
    legend4.Draw();

    can4.RedrawAxis();
    can4.SaveAs("output/Combination/HvsNoH_vsFake.png","png");
    can4.SaveAs("output/Combination/HvsNoH_vsFake.pdf","pdf");
    can4.SaveAs("output/Combination/HvsNoH_vsFake.root","root");
    can4.SaveAs("output/Combination/HvsNoH_vsFake.C","C");


    
