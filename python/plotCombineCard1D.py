#! /usr/bin/env pythonexp
import os
import glob
import math
from array import array
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

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

## parse files
parser.add_option('--fileDIR',   action="store", type="string", dest="fileDIR", default="")
parser.add_option('--channel',   action="store", type="string", dest="channel",     default="UUpp")
parser.add_option('--outputPlotDIR',       action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariableList',   action="store", type="string", dest="inputVariableList", default="")

## make plots
parser.add_option('--makeAsymptoticPlot',           action="store", type="int",    dest="makeAsymptoticLimitPlot",   default=0)
parser.add_option('--makeProfileLikelihoodPlot',    action="store", type="int",    dest="makeProfileLikelihoodPlot", default=0)
parser.add_option('--makeLikelihoodScanPlot',       action="store", type="int",    dest="makeLikelihoodScanPlot",    default=0)
parser.add_option('--makeMaxLikelihoodFitPlot',     action="store", type="int",    dest="makeMaxLikelihoodFitPlot",  default=0)
parser.add_option('--makeUncertaintyPlot',          action="store", type="int",    dest="makeUncertaintyPlot",       default=0)

parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)

parser.add_option('--symmetricError',    action="store", type=int, dest="symmetricError", default=1)

(options, args) = parser.parse_args()


########################################
###### Make Asymptotic Limit Plot ######
########################################

def getAsymptoticLimit (file, limitExp, limit1sUp, limit1sDw, limit2sUp, limit2sDw):
        
 f = ROOT.TFile(file);
 t = f.Get("limit"); 

 yMu     = [];  yMu1sUp = [];  yMu1sDw = [];  yMu2sUp = [];  yMu2sDw = [];
 ievent2sDw = 0;  ievent1sDw = 0;  ievent1sUp = 0; ievent2sUp = 0; ievent = 0;

 for i in range(t.GetEntries()):
        
  t.GetEntry(i);
  t_quantileExpected = t.quantileExpected;
  t_limit = t.limit;

  if t_quantileExpected == -1.: 
      continue ;
  elif t_quantileExpected >= 0.024 and t_quantileExpected <= 0.026: 
      yMu2sDw.append((ievent2sDw,t.limit));
      ievent2sDw = ievent2sDw+1;
  elif t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17: 
      yMu1sDw.append((ievent1sDw,t.limit));
      ievent1sDw = ievent1sDw+1;
  elif t_quantileExpected == 0.5: 
      yMu.append((ievent,t.limit));
      ievent = ievent+1;
  elif t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85: 
      yMu1sUp.append((ievent1sUp,t.limit));
      ievent1sUp = ievent1sUp+1;    
  elif t_quantileExpected >= 0.974 and t_quantileExpected <= 0.976: 
      yMu2sUp.append((ievent2sUp,t.limit));
      ievent2sUp = ievent2sUp+1;
  else: print "Unknown quantile!"

 #### fill histos
 for mu in yMu :
     limitExp.Fill(mu[1])

 for mu in yMu1sUp :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             limit1sUp.Fill(mu[1]-yMu[i][1])

 for mu in yMu1sDw :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             limit1sDw.Fill(yMu[i][1]-mu[1])

 for mu in yMu2sUp :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             limit2sUp.Fill(mu[1]-yMu[i][1])

 for mu in yMu2sDw :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             limit2sDw.Fill(yMu[i][1]-mu[1])


 return ;

####################################################
### Get Limit value using expected quantile only ###
####################################################

def getExpectedQuantile(file, signifExp):

 f = ROOT.TFile(file);
 t = f.Get("limit");
 entries = t.GetEntries();

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;
  signifExp.Fill(t.limit);
  
 return ;

######################################################################
#### get signal strencht from max likelihood fit file for S+B fit ####
######################################################################

def getSignalStrenghtMLFit(file,muValue):

 f = ROOT.TFile(file);
 t = f.Get("tree_fit_sb");
 entries = t.GetEntries();

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;

  if t.fit_status != 0 : continue ;

  muValue.Fill(t.mu);
  
 return ;

######################################################################
#### get signal strencht from max likelihood fit from limit tree ####
######################################################################

def getSignalStrenght(ifile,muValue,muErrUpOneSigma,muErrUpTwoSigma,muErrDownOneSigma,muErrDownTwoSigma):

 f = ROOT.TFile(ifile);
 t = f.Get("limit");
 entries = t.GetEntries();

 yMu     = []; yMu1sUp = []; yMu1sDw = []; yMu2sUp = []; yMu2sDw = [];

 ievent2sDw = 0;
 ievent1sDw = 0;
 ievent1sUp = 0;
 ievent2sUp = 0;
 ievent = 0;

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;
  t_limit = t.limit;

  if t_quantileExpected >= 0.024 and t_quantileExpected <= 0.026 : 
      yMu2sDw.append((ievent2sDw,t_limit));
      ievent2sDw = ievent2sDw +1

  elif t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17: 
      yMu1sDw.append((ievent1sDw,t_limit));
      ievent1sDw = ievent1sDw +1

  elif t_quantileExpected == 0.5: 
      yMu.append((ievent,t_limit));
      ievent = ievent +1

  elif t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85: 
      yMu1sUp.append((ievent1sUp,t_limit));
      ievent1sUp = ievent1sUp +1

  elif t_quantileExpected >= 0.974 and t_quantileExpected <= 0.976: 
      yMu2sUp.append((ievent2sUp,t_limit));
      ievent2sUp = ievent2sUp +1

  elif t_quantileExpected == -1 :
      continue ;
  else: print "Unknown quantile!"


 ## fill histograms
 for mu in yMu :
     muValue.Fill(mu[1])

 for mu in yMu1sUp :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             if mu[1]-yMu[i][1] <  0. : continue ;
             muErrUpOneSigma.Fill(mu[1]-yMu[i][1])

 for mu in yMu1sDw :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             if yMu[i][1]-mu[1] <  0. : continue ;
             muErrDownOneSigma.Fill(yMu[i][1]-mu[1])

 for mu in yMu2sUp :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             if mu[1]-yMu[i][1] <  0. : continue ;
             muErrUpTwoSigma.Fill(mu[1]-yMu[i][1])

 for mu in yMu2sDw :
     for i, v in enumerate(yMu) :
         if v[0] == mu[0]:
             if yMu[i][1]-mu[1] < 0. : continue ;
             muErrDownTwoSigma.Fill(yMu[i][1]-mu[1])


 return ;

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
  gStyle.SetPadBottomMargin(0.15);#0.13);
  gStyle.SetPadLeftMargin(0.11);#0.16);
  gStyle.SetPadRightMargin(0.05);#0.02);                                                                                                                                     

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
  gStyle.SetOptFit(1)

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


##############################
#### Make SM Limits Plots ####  
##############################  

def makeAsymptoticLimitPlot(filelist,variableName,variableLabel):

    nPoints = len(variableName);
       
    ## array for TGraph Asym Errors
    xbins        = array('f', []); 
    xbins_err_up = array('f', []);
    xbins_err_dw = array('f', []);
    ybins_exp    = array('f', []); 
    ybins_1s_up  = array('f', []); 
    ybins_2s_up  = array('f', []);
    ybins_1s_dw  = array('f', []); 
    ybins_2s_dw  = array('f', []);

    ymax = array('f',[]);

    binName = [];     

    ## histograms for the limit values
    limitExp     = ROOT.TH1F("limitExp","",1000,-1,100);
    limitExp1sUp = ROOT.TH1F("limitExp1sUp","",1000,-1,100);
    limitExp2sUp = ROOT.TH1F("limitExp2sUp","",1000,-1,100);
    limitExp1sDw = ROOT.TH1F("limitExp1sDw","",1000,-1,100);
    limitExp2sDw = ROOT.TH1F("limitExp2sDw","",1000,-1,100);
    limitExp.Sumw2();
    limitExp1sUp.Sumw2();
    limitExp2sUp.Sumw2();
    limitExp1sDw.Sumw2();
    limitExp2sDw.Sumw2();

    for ivar in range(len(variableName)) :
        for ifile in range(len(filelist)):
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :

                binName.append(variableLabel[ivar]);

                limitExp.Reset("ICES");
                limitExp1sUp.Reset("ICES");
                limitExp2sUp.Reset("ICES");
                limitExp1sDw.Reset("ICES");
                limitExp2sDw.Reset("ICES");

                getAsymptoticLimit(filelist[ifile],limitExp,limitExp1sUp,limitExp1sDw,limitExp2sUp,limitExp2sDw);
                xbins.append(ivar+0.5);
                xbins_err_up.append(0.5);
                xbins_err_dw.append(0.5);

                ybins_exp.append(limitExp.GetMean());
                ybins_2s_dw.append(limitExp2sDw.GetMean());
                ybins_1s_dw.append(limitExp1sDw.GetMean());
                ybins_2s_up.append(limitExp2sUp.GetMean());
                ybins_1s_up.append(limitExp1sUp.GetMean());
                
                ymax.append(limitExp.GetMean()+limitExp2sUp.GetMean());
                break;


    ## make the plot and setting the style                               
    curGraph_exp = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp);
    curGraph_1s  = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_1s_dw,ybins_1s_up);
    curGraph_2s  = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_2s_dw,ybins_2s_up);

    curGraphMax  = ROOT.TGraph(nPoints,xbins,ymax);

    curGraph_exp.SetMarkerStyle(20);
    curGraph_exp.SetLineWidth(3);
    curGraph_exp.SetLineStyle(1);
    curGraph_exp.SetMarkerSize(1.6);
    curGraph_exp.SetMarkerSize(1.3);
    curGraph_exp.SetMarkerColor(ROOT.kBlack);

    curGraph_1s.SetFillColor(ROOT.kGreen);
    curGraph_1s.SetFillStyle(1001);
    curGraph_1s.SetLineStyle(ROOT.kDashed);
    curGraph_1s.SetLineWidth(3);

    curGraph_2s.SetFillColor(ROOT.kYellow);
    curGraph_2s.SetFillStyle(1001);
    curGraph_2s.SetLineStyle(ROOT.kDashed);
    curGraph_2s.SetLineWidth(3);
                               
    oneLine = ROOT.TF1("oneLine","1",0,nPoints);
    oneLine.SetLineColor(ROOT.kRed);
    oneLine.SetLineWidth(3);
    
    ## canvas
    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,0.0,len(binName),ROOT.TMath.MaxElement(curGraphMax.GetN(),curGraphMax.GetY())*1.2);
    hrl.SetBins(len(binName),0,len(binName));
    hrl.GetYaxis().SetTitle("#sigma_{95%} / #sigma_{SM}");
    hrl.GetYaxis().SetTitleOffset(1.15);
    hrl.GetYaxis().SetTitleSize(0.045);
    hrl.GetYaxis().SetTitleFont(42);

    hrl.GetXaxis().SetTitleSize(0.045);
    hrl.GetXaxis().SetTitleFont(42);
    
    for ibin in range(hrl.GetNbinsX()):
        hrl.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    hrl.GetYaxis().SetNdivisions(505);
    can.SetGridx(1);
    can.SetGridy(1);
                   
    curGraph_2s.Draw("2");
    curGraph_1s.Draw("2same");
    curGraph_exp.Draw("Psame");

    leg2 = ROOT.TLegend(0.53,0.71,0.86,0.92);
    leg2.SetFillColor(0);
    leg2.SetFillStyle(0);
    leg2.SetShadowColor(0);
    leg2.SetTextFont(42);
    leg2.SetTextSize(0.028);

    leg2.AddEntry(curGraph_exp,"Asympt. CL_{S} Expected","L")
    leg2.AddEntry(curGraph_1s, "Asympt. CL_{S} Expected #pm 1#sigma","LF")
    leg2.AddEntry(curGraph_2s, "Asympt. CL_{S} Expected #pm 2#sigma","LF")
                                       
    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    leg2.Draw();
    oneLine.Draw("same");

    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw();

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw();

    can.SaveAs("%s/AsymptoticLimit_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/AsymptoticLimit_%s.pdf"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/AsymptoticLimit_%s.root"%(options.outputPlotDIR,options.channel));

##############################
#### Make SM PValue Plots ####  
##############################  

def makeProfileLikelihoodPlot(filelist,variableName,variableLabel):

    nPoints = len(variableName);
    
    xbins_exp           = array('f', []);
    ybins_exp           = array('f', []);
    xbins_err           = array('f', []);
    ybins_err           = array('f', []);
    ybin_max            = array('f', []);
    binName = [];     

    ## histogram
    signifExp = ROOT.TH1F("signifExp","",10000,0,150);
    signifExp.Sumw2();

    for ivar in range(len(variableName)) :

        for ifile in range(len(filelist)):

            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :

                binName.append(variableLabel[ivar]);

                signifExp.Reset("ICES");

                xbins_exp.append(ivar+0.5); 
                xbins_err.append(0.); 

                getExpectedQuantile(filelist[ifile],signifExp);

                ybins_exp.append(signifExp.GetMean());
                ybins_err.append(signifExp.GetMeanError());                    
                ybin_max.append(signifExp.GetMean()+signifExp.GetRMS());
                break;


    gr_exp = ROOT.TGraphAsymmErrors(nPoints,xbins_exp,ybins_exp,xbins_err,xbins_err,ybins_err,ybins_err);
    gr_max = ROOT.TGraphAsymmErrors(nPoints,xbins_exp,ybin_max,xbins_err,xbins_err,ybins_err,ybins_err);                         
    gr_exp.SetLineColor(1); 
    gr_exp.SetMarkerColor(1); 
    gr_exp.SetMarkerStyle(20); 
    gr_exp.SetLineWidth(3);
    gr_exp.SetMarkerSize(1.6);
    gr_exp.SetLineStyle(8);

    oneSLine = ROOT.TF1("oneSLine","1",0,nPoints);
    oneSLine.SetLineColor(ROOT.kRed); oneSLine.SetLineWidth(2); oneSLine.SetLineStyle(2);
    twoSLine = ROOT.TF1("twoSLine","2",0,nPoints);
    twoSLine.SetLineColor(ROOT.kRed); twoSLine.SetLineWidth(2); twoSLine.SetLineStyle(2);
    threeSLine = ROOT.TF1("threeSLine","3",0,nPoints);
    threeSLine.SetLineColor(ROOT.kRed); threeSLine.SetLineWidth(2); threeSLine.SetLineStyle(2);
    fourSLine = ROOT.TF1("fourSLine","4",0,nPoints);
    fourSLine.SetLineColor(ROOT.kRed); fourSLine.SetLineWidth(2); fourSLine.SetLineStyle(2);
    fiveSLine = ROOT.TF1("fiveSLine","5",0,nPoints);
    fiveSLine.SetLineColor(ROOT.kRed); fiveSLine.SetLineWidth(2); fiveSLine.SetLineStyle(2);
    
    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,0,nPoints,ROOT.TMath.MaxElement(gr_exp.GetN(),gr_max.GetY())*1.2); 
    hrl.SetBins(len(binName),0,len(binName));
    hrl.GetYaxis().SetTitle("significance");
    hrl.GetYaxis().SetTitleOffset(0.95);

    for ibin in range(hrl.GetNbinsX()):
        hrl.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    can.SetGrid();

    gr_exp.Draw("P");

    oneSLine.Draw("same");
    twoSLine.Draw("same");
    threeSLine.Draw("same");
    fourSLine.Draw("same");
    fiveSLine.Draw("same");


    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw();

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw();

    can.SaveAs("%s/ProfileLikelihood_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s.pdf"%(options.outputPlotDIR,options.channel));



####################################
#### Make Signal Strenght Plots ####  
####################################  

def makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel):

    nPoints = len(variableName);

    xbins_mu         = array('f', []);
    xbins_mu_err_up  = array('f', []);
    xbins_mu_err_dn  = array('f', []);
    ybins_mu         = array('f', []);
    ybins_mu_err_up  = array('f', []);
    ybins_mu_err_dn  = array('f', []);
    ybins_mu_err_up_2s  = array('f', []);
    ybins_mu_err_dn_2s  = array('f', []);

    binName = [];

    muValue         = ROOT.TH1F("muValue","",1000,-100,100);
    muErrUpOneSigma = ROOT.TH1F("muErrUpOneSigma","",1000,-100,100);
    muErrUpTwoSigma = ROOT.TH1F("muErrUpTwoSigma","",1000,-100,100);
    muErrDownOneSigma = ROOT.TH1F("muErrDownOneSigma","",1000,-100,100);
    muErrDownTwoSigma = ROOT.TH1F("muErrDownTwoSigma","",1000,-100,100);

    muValue.Sumw2();
    muErrUpOneSigma.Sumw2();
    muErrUpTwoSigma.Sumw2();
    muErrDownOneSigma.Sumw2();
    muErrDownTwoSigma.Sumw2();

    for ivar in range(len(variableName)) :
        for ifile in range(len(filelist)):
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                
                muValue.Reset("ICES");
                muErrUpOneSigma.Reset("ICES");
                muErrUpTwoSigma.Reset("ICES");
                muErrDownOneSigma.Reset("ICES");
                muErrDownTwoSigma.Reset("ICES");

                getSignalStrenght(filelist[ifile],muValue, muErrUpOneSigma, muErrUpTwoSigma, muErrDownOneSigma, muErrDownTwoSigma)

                xbins_mu.append(ivar+0.5); 
                xbins_mu_err_up.append(0.5); 
                xbins_mu_err_dn.append(0.5); 
                ybins_mu.append(muValue.GetMean());

                if muErrUpOneSigma.GetMean() > muErrUpTwoSigma.GetMean() :
                     ybins_mu_err_up.append(muErrDownOneSigma.GetMean()/2);
                else :
                     ybins_mu_err_up.append(muErrUpOneSigma.GetMean());

                ybins_mu_err_up.append(muErrUpOneSigma.GetMean());
                ybins_mu_err_dn.append(muErrDownOneSigma.GetMean());
                ybins_mu_err_up_2s.append(muErrUpTwoSigma.GetMean());
                ybins_mu_err_dn_2s.append(muErrDownTwoSigma.GetMean());                

                    

                break;

    gr_mu_1s = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn,ybins_mu_err_up);
    gr_mu_1s.SetLineColor(1); gr_mu_1s.SetMarkerColor(1); gr_mu_1s.SetMarkerStyle(20); gr_mu_1s.SetLineWidth(5); gr_mu_1s.SetMarkerSize(1.6);

    gr_mu_2s = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn_2s,ybins_mu_err_up_2s);
    gr_mu_2s.SetLineColor(ROOT.kBlue); gr_mu_2s.SetMarkerColor(ROOT.kBlue); gr_mu_2s.SetMarkerStyle(20); gr_mu_2s.SetLineWidth(3); gr_mu_2s.SetMarkerSize(1.6);

    ban1s = TLatex(950,1.,("#mu SM injected"));
    ban1s.SetTextSize(0.028); ban1s.SetTextColor(1)

 
    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,1-ROOT.TMath.MaxElement(gr_mu_2s.GetN(),ybins_mu_err_dn_2s)*1.25,nPoints,1+ROOT.TMath.MaxElement(gr_mu_2s.GetN(),ybins_mu_err_up_2s)*1.25);
    hrl.SetBins(len(binName),0,nPoints);

    for ibin in range(hrl.GetNbinsX()):
        hrl.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    hrl.GetYaxis().SetTitle("signal strenght");
    hrl.GetYaxis().SetTitleOffset(0.95)

    can.SetGrid();
   
    gr_mu_2s.Draw("P");
    gr_mu_1s.Draw("Psame"); 


    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw();

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw();
   
    can.SaveAs("%s/maximumLikelihoodFit_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    can.SaveAs("%s/maximumLikelihoodFit_%s.png"%(options.outputPlotDIR,options.channel),"png");

##############################
### make uncertaintyb plot ###
##############################

def makeUncertaintyPlot(filelist,variableName,variableLabel):

    nPoints = len(variableName);

    xbins_mu         = array('f', []);
    ybins_mu_err_1s  = array('f', []);
    ybins_mu_err_2s  = array('f', []);

    binName = [];

    muValue         = ROOT.TH1F("muValue","",1000,-100,100);
    muErrUpOneSigma = ROOT.TH1F("muErrUpOneSigma","",1000,-100,100);
    muErrUpTwoSigma = ROOT.TH1F("muErrUpTwoSigma","",1000,-100,100);
    muErrDownOneSigma = ROOT.TH1F("muErrDownOneSigma","",1000,-100,100);
    muErrDownTwoSigma = ROOT.TH1F("muErrDownTwoSigma","",1000,-100,100);

    muValue.Sumw2();
    muErrUpOneSigma.Sumw2();
    muErrUpTwoSigma.Sumw2();
    muErrDownOneSigma.Sumw2();
    muErrDownTwoSigma.Sumw2();

    for ivar in range(len(variableName)) :
        for ifile in range(len(filelist)):
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                
                muValue.Reset("ICES");
                muErrUpOneSigma.Reset("ICES");
                muErrUpTwoSigma.Reset("ICES");
                muErrDownOneSigma.Reset("ICES");
                muErrDownTwoSigma.Reset("ICES");

                getSignalStrenght(filelist[ifile],muValue, muErrUpOneSigma, muErrUpTwoSigma, muErrDownOneSigma, muErrDownTwoSigma)

                xbins_mu.append(ivar+0.5); 

                if options.symmetricError == 1 :
                    ybins_mu_err_1s.append((muErrUpOneSigma.GetMean()+muErrDownOneSigma.GetMean())/2);
                    ybins_mu_err_2s.append((muErrUpTwoSigma.GetMean()+muErrDownTwoSigma.GetMean())/2);
                else:
                    ybins_mu_err_1s.append(muErrDownOneSigma.GetMean());
                    ybins_mu_err_2s.append(muErrDownTwoSigma.GetMean());

                break;

    gr_mu_1s = ROOT.TGraph(nPoints,xbins_mu,ybins_mu_err_1s);
    gr_mu_1s.SetLineColor(1); gr_mu_1s.SetMarkerColor(1); gr_mu_1s.SetMarkerStyle(20); gr_mu_1s.SetLineWidth(2); gr_mu_1s.SetMarkerSize(1.6);

    gr_mu_2s = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu_err_2s);
    gr_mu_2s.SetLineColor(ROOT.kBlue); gr_mu_2s.SetMarkerColor(ROOT.kBlue); gr_mu_2s.SetMarkerStyle(20); gr_mu_2s.SetLineWidth(2); gr_mu_2s.SetMarkerSize(1.6);

    ban1s = TLatex(950,1.,("#mu SM injected"));
    ban1s.SetTextSize(0.028); ban1s.SetTextColor(1)
 
    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,0.,nPoints,ROOT.TMath.MaxElement(gr_mu_2s.GetN(),gr_mu_2s.GetY())*1.25);
    hrl.SetBins(len(binName),0,nPoints);

    for ibin in range(hrl.GetNbinsX()):
        hrl.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    hrl.GetYaxis().SetTitle("signal strenght uncertainty");
    hrl.GetYaxis().SetTitleOffset(0.95)

    can.SetGrid();
   
    gr_mu_2s.Draw("PL");
    gr_mu_1s.Draw("PLsame"); 


    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw("same");

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw("same");

    leg2 = ROOT.TLegend(0.73,0.8,0.86,0.92);
    leg2.SetFillColor(0);
    leg2.SetFillStyle(0);
    leg2.SetShadowColor(0);
    leg2.SetTextFont(42);
    leg2.SetTextSize(0.028);

    leg2.AddEntry(gr_mu_2s,"2#sigma band","PL")
    leg2.AddEntry(gr_mu_1s,"1#sigma band","PL")

    leg2.Draw("same")

    can.SaveAs("%s/mu_uncertainty_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    can.SaveAs("%s/mu_uncertainty_%s.png"%(options.outputPlotDIR,options.channel),"png");


#################################
### make likelihood scan plot ###
#################################    

def makeLikelihoodScanPlot(filelist,variableName,variableLabel):

    nPoints = len(filelist);
    
    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw();

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw();
    
    binName = [];
    varName = [];


    for ivar in range(len(variableName)) :
        for ifile in range(len(filelist)):
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                varName.append(variableName[ivar]);

                f = ROOT.TFile(filelist[ifile]);
                t = f.Get("limit");

                mapMuDnll = ROOT.TH1F("mapMuDnll","",150,options.rMin,options.rMax);
                
                for ientry in range(t.GetEntries()):
                    t.GetEntry(ientry);   
                    mapMuDnll.Fill(t.r,2*t.deltaNLL); 

                gr_mu = ROOT.TGraph(mapMuDnll);
                gr_mu.SetMinimum(options.rMin*1.2);
                gr_mu.SetMaximum(options.rMax*0.9);
                gr_mu.GetXaxis().SetRangeUser(options.rMin*1.05,options.rMax*0.95);
                gr_mu.GetYaxis().SetRangeUser(0,gr_mu.GetMaximum()*0.9);

                gr_mu.SetLineWidth(2);
                gr_mu.SetLineColor(2);
                gr_mu.SetFillColor(2);
                gr_mu.SetMarkerStyle(20);

                can = ROOT.TCanvas("can_%s"%(varName[len(varName)-1]),"can_%s"%(varName[len(varName)-1]),700,650);
                gr_mu.GetYaxis().SetTitle("-2#Delta ln(L)");
                gr_mu.GetYaxis().SetTitleOffset(1.0);
                
                gr_mu.GetXaxis().SetTitle("signal strenght (%s)"%(binName[len(binName)-1]));
                can.SetGrid();
                gr_mu.Draw("AC");

                tex.Draw();
                tex2.Draw();
                tex3.Draw();

                can.SaveAs("%s/LikelihoodScan_%s_%s.pdf"%(options.outputPlotDIR,varName[len(varName)-1],options.channel),"pdf");
                can.SaveAs("%s/LikelihoodScan_%s_%s.png"%(options.outputPlotDIR,varName[len(varName)-1],options.channel),"png");
                break ;                           

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## make the list of variables
    variableName = [];
    variableLabel = [];

    fileVar = open('%s'%(options.inputVariableList), 'r');
    for columns in ( raw.strip().split() for raw in fileVar ):
        if columns[0].find('#')!=-1 or columns[0] == ' ': continue;
        if ROOT.TString(options.inputVariableList).Contains("Dynamic"):
            variableLabel.append(columns[3]);
            variableName.append(columns[0]);
        else:
            variableLabel.append(columns[4]);
            variableName.append(columns[0]);

    ## fix the chdir
    os.chdir(options.fileDIR);

    ## create output plot directory
    os.system("mkdir -p "+options.outputPlotDIR);

    ## create the file list    
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

    ## make the list of files
    file = open("list.txt",'r')
    filelist = [];

    for line in file :
     filelist.append(line.split(" ")[0].replace("\n",""));
    
    ## set the common style
    setStyle();

    if options.makeAsymptoticLimitPlot :
        makeAsymptoticLimitPlot(filelist,variableName,variableLabel);
    elif options.makeProfileLikelihoodPlot :
        makeProfileLikelihoodPlot(filelist,variableName,variableLabel);
    elif options.makeLikelihoodScanPlot :
        makeLikelihoodScanPlot(filelist,variableName,variableLabel);    
    elif options.makeMaxLikelihoodFitPlot :
        makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel);
    elif options.makeUncertaintyPlot:
        makeUncertaintyPlot(filelist,variableName,variableLabel);              
                  
    os.system("rm list.txt");    
