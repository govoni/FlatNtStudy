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

    tex2 = TLatex(0.17,0.86,"CMS Delphes PhaseII Simulation");
    tex2.SetNDC(1);
    tex2.SetTextAlign(11);
    tex2.SetTextFont(61);    
    tex2.SetLineWidth(2);
    tex2.SetTextSize(0.04);

    tex3 = ROOT.TLatex(0.67,0.96,"14 TeV, 3000 fb^{-1}");
    tex3.SetNDC(1);
    tex3.SetTextAlign(11);
    tex3.SetTextFont(42);
    tex3.SetTextSize(0.04);
    tex3.SetLineWidth(2);

    significance      = ROOT.TH1F("significance","significance",6,0,6);
    significance_stat = ROOT.TH1F("significance_stat","significance_stat",6,0,6);

    ## input file for phaseI 50PU, aged and phaseII for polarized signficance vs lumi

    filePolarized_ptj30  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj30_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","READ");
    treePolarized_ptj30  = filePolarized_ptj30.Get("limit");

    for i in range(treePolarized_ptj30.GetEntries()):
      treePolarized_ptj30.GetEntry(i)
      significance.SetBinContent(1,treePolarized_ptj30.limit);

    significance.GetXaxis().SetBinLabel(1,"nominal");

    filePolarized_ptj40  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj40_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj40  = filePolarized_ptj40.Get("limit");

    for i in range(treePolarized_ptj40.GetEntries()):
      treePolarized_ptj40.GetEntry(i)
      significance.SetBinContent(2,treePolarized_ptj40.limit);

    significance.GetXaxis().SetBinLabel(2,"p_{T}(jet)>40GeV");

    filePolarized_ptj50  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj50_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj50  = filePolarized_ptj50.Get("limit");

    for i in range(treePolarized_ptj50.GetEntries()):
      treePolarized_ptj50.GetEntry(i)
      significance.SetBinContent(3,treePolarized_ptj50.limit);

    significance.GetXaxis().SetBinLabel(3,"p_{T}(jet)>50GeV");

    filePolarized_ptj60  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj60_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj60  = filePolarized_ptj60.Get("limit");
    for i in range(treePolarized_ptj60.GetEntries()):
      treePolarized_ptj60.GetEntry(i)
      significance.SetBinContent(4,treePolarized_ptj60.limit);

    significance.GetXaxis().SetBinLabel(4,"p_{T}(jet)>60GeV");

    filePolarized_ptl25  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptl25_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptl25  = filePolarized_ptl25.Get("limit");
    for i in range(treePolarized_ptl25.GetEntries()):
      treePolarized_ptl25.GetEntry(i)
      significance.SetBinContent(5,treePolarized_ptl25.limit);

    significance.GetXaxis().SetBinLabel(5,"p_{T}(lep)>25GeV");

    filePolarized_ptl30  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptl30_DeltaPhi_JJ_ptl1_COMB.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptl30  = filePolarized_ptl30.Get("limit");
    for i in range(treePolarized_ptl30.GetEntries()):
      treePolarized_ptl30.GetEntry(i)
      significance.SetBinContent(6,treePolarized_ptl30.limit);

    significance.GetXaxis().SetBinLabel(6,"p_{T}(lep)>30GeV");


    filePolarized_ptj30_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj30_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj30_stat  = filePolarized_ptj30_stat.Get("limit");
    for i in range(treePolarized_ptj30_stat.GetEntries()):
      treePolarized_ptj30_stat.GetEntry(i)
      significance_stat.SetBinContent(1,treePolarized_ptj30_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");

    filePolarized_ptj40_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj40_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj40_stat  = filePolarized_ptj40_stat.Get("limit");
    for i in range(treePolarized_ptj40_stat.GetEntries()):
      treePolarized_ptj40_stat.GetEntry(i)
      significance_stat.SetBinContent(2,treePolarized_ptj40_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");

    filePolarized_ptj50_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj50_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj50_stat  = filePolarized_ptj50_stat.Get("limit");
    for i in range(treePolarized_ptj50_stat.GetEntries()):
      treePolarized_ptj50_stat.GetEntry(i)
      significance_stat.SetBinContent(3,treePolarized_ptj50_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");

    filePolarized_ptj60_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptj60_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptj60_stat  = filePolarized_ptj60_stat.Get("limit");
    for i in range(treePolarized_ptj60_stat.GetEntries()):
      treePolarized_ptj60_stat.GetEntry(i)
      significance_stat.SetBinContent(4,treePolarized_ptj60_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");


    filePolarized_ptl25_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptl25_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptl25_stat  = filePolarized_ptl25_stat.Get("limit");
    for i in range(treePolarized_ptl25_stat.GetEntries()):
      treePolarized_ptl25_stat.GetEntry(i)
      significance_stat.SetBinContent(5,treePolarized_ptl25_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");

    filePolarized_ptl30_stat  = ROOT.TFile("output/DataCards_WW_SS_Polarized_Dynamic_Aged/performanceVsCut/higgsCombineptl30_DeltaPhi_JJ_ptl1_COMB_stat.ProfileLikelihood.mH120.root","OPEN");
    treePolarized_ptl30_stat  = filePolarized_ptl30_stat.Get("limit");
    for i in range(treePolarized_ptl30_stat.GetEntries()):
      treePolarized_ptl30_stat.GetEntry(i)
      significance_stat.SetBinContent(6,treePolarized_ptl30_stat.limit);

    significance_stat.GetXaxis().SetBinLabel(1,"nominal");


    can = ROOT.TCanvas("can","can",600,600);

    significance.GetYaxis().SetRangeUser(0.,5.);

    significance.GetXaxis().SetTitleSize(0.045);
    significance.GetXaxis().SetLabelSize(0.035);

    significance.GetYaxis().SetTitle("Expected significance (#sigma)");
    significance.GetYaxis().SetTitleSize(0.045);
    significance.GetYaxis().SetTitleOffset(1.17);
    significance.GetYaxis().SetLabelSize(0.04);
    

    significance.SetMarkerSize(1.25);
    significance.SetMarkerStyle(20);
    significance.SetLineStyle(1);
    significance.SetLineWidth(2);
    significance.SetLineColor(ROOT.kBlue);
    significance.SetMarkerColor(ROOT.kBlue);

    significance.Draw("pl");

    significance_stat.SetMarkerSize(1.25);
    significance_stat.SetMarkerStyle(20);
    significance_stat.SetLineWidth(2);
    significance_stat.SetLineColor(ROOT.kRed);
    significance_stat.SetMarkerColor(ROOT.kRed);
    significance_stat.Draw("plsame");

    legend = ROOT.TLegend(0.45,0.65,0.75,0.8)
    legend.AddEntry(significance,"with Systematics","pl")
    legend.AddEntry(significance_stat,"only Statistics","pl")
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.Draw("same");

    tex2.Draw();
    tex3.Draw();

    os.system("mkdir -p output/detectorThreshold/");
    os.system("rm output/detectorThreshold/*");

    can.RedrawAxis();

    can.SaveAs("output/detectorThreshold/LL_scattering_vsThreshold.png","png");
    can.SaveAs("output/detectorThreshold/LL_scattering_vsThreshold.pdf","pdf");
