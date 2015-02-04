#! /usr/bin/env python
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
parser.add_option('--channel',   action="store", type="string", dest="channel",     default="UU")
parser.add_option('--outputPlotDIR',       action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariableList',   action="store", type="string", dest="inputVariableList", default="")

parser.add_option('--blindObservedValues', action="store", type="int", dest="blindObservedValues", default=0)

## make plots
parser.add_option('--makeAsymptoticPlot',           action="store", type="int",    dest="makeAsymptoticLimitPlot",   default=0)
parser.add_option('--makeProfileLikelihoodPlot',    action="store", type="int",    dest="makeProfileLikelihoodPlot", default=0)
parser.add_option('--makeLikelihoodScanPlot',       action="store", type="int",    dest="makeLikelihoodScanPlot",    default=0)
parser.add_option('--makeMaxLikelihoodFitPlot',     action="store", type="int",    dest="makeMaxLikelihoodFitPlot",  default=0)

(options, args) = parser.parse_args()

########################################
###### Make Asymptotic Limit Plot ######
########################################

def getAsymptoticLimit (file):
        
 f = ROOT.TFile(file);
 t = f.Get("limit"); 

 lims = [0,0,0,0,0,0];

 limit_entries = 0;  
    
 for i in range(t.GetEntries()):
        
  t.GetEntry(i);
  t_quantileExpected = t.quantileExpected;
  t_limit = t.limit;
        
  #print "limit: ", t_limit, ", quantileExpected: ",t_quantileExpected;        
  if t_quantileExpected == -1.: 
      lims[0] += t_limit; 
  elif t_quantileExpected >= 0.024 and t_quantileExpected <= 0.026: 
      lims[1] += t_limit;
  elif t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17: 
      lims[2] += t_limit;            
  elif t_quantileExpected == 0.5: 
      lims[3] += t_limit; 
      limit_entries += 1 ;            
  elif t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85: 
      lims[4] += t_limit;
  elif t_quantileExpected >= 0.974 and t_quantileExpected <= 0.976: 
      lims[5] += t_limit;
  else: print "Unknown quantile!"

 if limit_entries != 0 :

     lims[0] = lims[0]/limit_entries ;
     lims[1] = lims[1]/limit_entries ;
     lims[2] = lims[2]/limit_entries ;
     lims[3] = lims[3]/limit_entries ;
     lims[4] = lims[4]/limit_entries ;
     lims[5] = lims[5]/limit_entries ;

    
 return lims;

####################################################
### Get Limit value using expected quantile only ###
####################################################

def getExpectedQuantile(file):

 f = ROOT.TFile(file);
 t = f.Get("limit");
 entries = t.GetEntries();

 histo_val = ROOT.TH1F("histo_val","",1000,0,1000);
 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;
  histo_val.Fill(t.limit);
  
 return histo_val.GetMean(), histo_val.GetRMS();

######################################################################
#### get signal strencht from max likelihood fit file for S+B fit ####
######################################################################

def getSignalStrenghtMLFit(file):

 f = ROOT.TFile(file);
 t = f.Get("tree_fit_sb");
 entries = t.GetEntries();

 muValue = [];

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;

  if t.fit_status != 0 : continue ;

  muValue.append(t.mu);
  
 return muValue;

######################################################################
#### get signal strencht from max likelihood fit from limit tree ####
######################################################################

def getSignalStrenght(ifile,muValue,muErrUpOneSigma,muErrUpTwoSigma,muErrDownOneSigma,muErrDownTwoSigma):

 f = ROOT.TFile(ifile);
 t = f.Get("limit");
 entries = t.GetEntries();

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;
  t_limit = t.limit;

  if t_quantileExpected >= 0.024 and t_quantileExpected <= 0.026: 
      muErrDownTwoSigma.Fill(t_limit);
  elif t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17: 
      muErrDownOneSigma.Fill(t_limit);
  elif t_quantileExpected == 0.5: 
      muValue.Fill(t_limit);
  elif t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85: 
      muErrUpOneSigma.Fill(t_limit);
  elif t_quantileExpected >= 0.974 and t_quantileExpected <= 0.976: 
      muErrUpTwoSigma.Fill(t_limit);
  elif t_quantileExpected == -1 :
      continue ;
  else: print "Unknown quantile!"

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

    nPoints = len(filelist);
    
    xbins        = array('f', []); 
    xbins_err_up = array('f', []);
    xbins_err_dw = array('f', []);
    ybins_exp    = array('f', []); 
    ybins_1s_up  = array('f', []); 
    ybins_2s_up  = array('f', []);
    ybins_1s_dw  = array('f', []); 
    ybins_2s_dw  = array('f', []);

    setStyle();

    binName = [];     

    for ifile in range(len(filelist)):
        curAsymLimits = getAsymptoticLimit(filelist[ifile]);
        xbins.append(ifile+0.5);
        xbins_err_up.append(0.5);
        xbins_err_dw.append(0.5);

	ybins_exp.append( curAsymLimits[3] );
        ybins_2s_dw.append(ROOT.TMath.Abs(curAsymLimits[1]-curAsymLimits[3]));
	ybins_1s_dw.append(ROOT.TMath.Abs(curAsymLimits[2]-curAsymLimits[3]));
        ybins_2s_up.append(ROOT.TMath.Abs(curAsymLimits[5]-curAsymLimits[3]));
	ybins_1s_up.append(ROOT.TMath.Abs(curAsymLimits[4]-curAsymLimits[3]));
        for ivar in range(len(variableName)) :
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                break;


    ## make the plot and setting the style                               
    curGraph_exp = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp);
    curGraph_1s  = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_1s_dw,ybins_1s_up);
    curGraph_2s  = ROOT.TGraphAsymmErrors(nPoints,xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_2s_dw,ybins_2s_up);


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
    can_SM = ROOT.TCanvas("can_SM","can_SM",1050,650);
    hrl_SM = can_SM.DrawFrame(0,0.0,nPoints,ROOT.TMath.MaxElement(curGraph_2s.GetN(),curGraph_2s.GetY())*2);
    hrl_SM.SetBins(len(binName),0,len(binName)-1);
    gStyle.SetPadLeftMargin(0.08);
    hrl_SM.GetYaxis().SetTitle("#mu = #sigma_{95%} / #sigma_{SM}");
    hrl_SM.GetYaxis().SetTitleOffset(1.15);
    hrl_SM.GetYaxis().SetTitleSize(0.045);
    hrl_SM.GetYaxis().SetTitleFont(42);

    hrl_SM.GetXaxis().SetTitleSize(0.045);
    hrl_SM.GetXaxis().SetTitleFont(42);

    for ibin in range(hrl_SM.GetNbinsX()-1):
        hrl_SM.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    hrl_SM.GetYaxis().SetNdivisions(505);
    can_SM.SetGridx(1);
    can_SM.SetGridy(1);
                   
    curGraph_2s.Draw("2");
    curGraph_1s.Draw("2same");
    curGraph_exp.Draw("Psame");

    leg2 = ROOT.TLegend(0.53,0.71,0.86,0.92);
    leg2.SetFillColor(0);
    leg2.SetShadowColor(0);
    leg2.SetTextFont(42);
    leg2.SetTextSize(0.028);

    leg2.AddEntry(curGraph_exp,"Asympt. CL_{S} Expected","L")
    leg2.AddEntry(curGraph_1s, "Asympt. CL_{S} Expected #pm 1#sigma","LF")
    leg2.AddEntry(curGraph_2s, "Asympt. CL_{S} Expected #pm 2#sigma","LF")

                                       
    can_SM.Update();
    can_SM.RedrawAxis();
    can_SM.RedrawAxis("g");
    can_SM.Update();

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

    can_SM.SaveAs("%s/AsymptoticLimit_%s.png"%(options.outputPlotDIR,options.channel));
    can_SM.SaveAs("%s/AsymptoticLimit_%s.pdf"%(options.outputPlotDIR,options.channel));
    can_SM.SaveAs("%s/AsymptoticLimit_%s.root"%(options.outputPlotDIR,options.channel));

##############################
#### Make SM PValue Plots ####  
##############################  

def makeProfileLikelihoodPlot(filelist,variableName,variableLabel):

    nPoints = len(filelist);
    
    xbins_exp           = array('f', []);
    ybins_exp           = array('f', []);
    xbins_err           = array('f', []);
    ybins_err           = array('f', []);

    binName = [];     

    setStyle();

    for ifile in range(len(filelist)):

        xbins_exp.append(ifile+0.5); 
        xbins_err.append(0.5); 
        yval, yval_err =  getExpectedQuantile(filelist[ifile]);
        ybins_exp.append(yval);
        ybins_err.append(yval_err);

        for ivar in range(len(variableName)) :
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                break;


    gr_exp = ROOT.TGraphAsymmErrors(nPoints,xbins_exp,ybins_exp,xbins_err,xbins_err,ybins_err,ybins_err);
                         
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
    
    ban1s = TLatex(950,2,("1 #sigma"));
    ban1s.SetTextSize(0.028); ban1s.SetTextColor(1)
    ban2s = TLatex(950,2,("2 #sigma"));
    ban2s.SetTextSize(0.028); ban1s.SetTextColor(1)
    ban3s = TLatex(950,3,("3 #sigma"));
    ban3s.SetTextSize(0.028); ban2s.SetTextColor(1)
    ban4s = TLatex(950,4,("4 #sigma"));
    ban4s.SetTextSize(0.028); ban3s.SetTextColor(1);
    ban5s = TLatex(950,5,("5 #sigma"));
    ban5s.SetTextSize(0.028); ban4s.SetTextColor(1)

    #leg2 = ROOT.TLegend(0.25,0.2,0.6,0.35);
    #leg2.SetFillStyle(0);
    #leg2.SetBorderSize(1);
    #leg2.SetTextFont(42);
    #leg2.SetTextSize(0.028);
    
    #leg2.AddEntry( gr_exp, "exp signif "+options.channel, "pl" );

    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,0,nPoints,ROOT.TMath.MaxElement(gr_exp.GetN(),gr_exp.GetY())*1.2); 
    hrl.SetBins(len(binName),0,len(binName)-1);
    hrl.GetYaxis().SetTitle("significance");
    hrl.GetYaxis().SetTitleOffset(0.95);

    for ibin in range(hrl.GetNbinsX()-1):
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

    #leg2.Draw();

    ban1s.Draw();
    ban2s.Draw();
    ban3s.Draw();
    ban4s.Draw();

    can.SaveAs("%s/ProfileLikelihood_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s.pdf"%(options.outputPlotDIR,options.channel));



####################################
#### Make Signal Strenght Plots ####  
####################################  

def makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel):

    nPoints = len(filelist);

    xbins_mu         = array('f', []);
    xbins_mu_err_up  = array('f', []);
    xbins_mu_err_dn  = array('f', []);
    ybins_mu         = array('f', []);
    ybins_mu_err_up  = array('f', []);
    ybins_mu_err_dn  = array('f', []);
    ybins_mu_err_up_2s  = array('f', []);
    ybins_mu_err_dn_2s  = array('f', []);
    ybins_mu_ratio   = array('f', []);

    binName = [];

    setStyle();

    muValue         = ROOT.TH1F("muValue","",100,-10,10);
    muErrUpOneSigma = ROOT.TH1F("muErrUpOneSigma","",100,-10,10);
    muErrUpTwoSigma = ROOT.TH1F("muErrUpTwoSigma","",100,-10,10);
    muErrDownOneSigma = ROOT.TH1F("muErrDownOneSigma","",100,-10,10);
    muErrDownTwoSigma = ROOT.TH1F("muErrDownTwoSigma","",100,-10,10);
        
    for ifile in range(len(filelist)):

        getSignalStrenght(filelist[ifile],muValue, muErrUpOneSigma, muErrUpTwoSigma, muErrDownOneSigma, muErrDownTwoSigma)
     
        xbins_mu.append(ifile+0.5); 
        xbins_mu_err_up.append(0.5); 
        xbins_mu_err_dn.append(0.5); 

        ybins_mu.append(muValue.GetMean());
        ybins_mu_err_up.append(ROOT.TMath.Abs(muErrUpOneSigma.GetMean()-muValue.GetMean()));
        ybins_mu_err_dn.append(ROOT.TMath.Abs(muErrDownOneSigma.GetMean()-muValue.GetMean()));
        ybins_mu_err_up_2s.append(ROOT.TMath.Abs(muErrUpTwoSigma.GetMean()-muValue.GetMean()));
        ybins_mu_err_dn_2s.append(ROOT.TMath.Abs(muErrDownTwoSigma.GetMean()-muValue.GetMean()));

        if muValue.GetMean() >= 0 : 
            ybins_mu_ratio.append(muValue.GetMean()/muErrDownOneSigma.GetMean());
        else:        
            ybins_mu_ratio.append(muValue.GetMean()/muErrDownOneSigma.GetMean());
            
        for ivar in range(len(variableName)) :
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                break;

    gr_mu_ratio = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu_ratio);
    gr_mu_ratio.SetLineColor(1); gr_mu_ratio.SetMarkerColor(1); gr_mu_ratio.SetMarkerStyle(20); gr_mu_ratio.SetLineWidth(3);gr_mu_ratio.SetMarkerSize(1.6);


    gr_mu_1s = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn,ybins_mu_err_up);
    gr_mu_1s.SetLineColor(1); gr_mu_1s.SetMarkerColor(1); gr_mu_1s.SetMarkerStyle(20); gr_mu_1s.SetLineWidth(5); gr_mu_1s.SetMarkerSize(1.6);

    gr_mu_2s = ROOT.TGraphAsymmErrors(nPoints,xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn_2s,ybins_mu_err_up_2s);
    gr_mu_2s.SetLineColor(ROOT.kBlue); gr_mu_2s.SetMarkerColor(ROOT.kBlue); gr_mu_2s.SetMarkerStyle(20); gr_mu_2s.SetLineWidth(3); gr_mu_2s.SetMarkerSize(1.6);

    ban1s = TLatex(950,1.,("#mu SM injected"));
    ban1s.SetTextSize(0.028); ban1s.SetTextColor(1)

    #leg2 = ROOT.TLegend(0.25,0.2,0.6,0.35);
    #leg2.SetFillStyle(0);
    #leg2.SetBorderSize(1);
    #leg2.SetTextFont(42);
    #leg2.SetTextSize(0.028);
    
    #leg2.AddEntry( gr_mu_1s, "signal strenght %s "%options.channel, "pl" );
 
    can = ROOT.TCanvas("can","can",1050,650);
    hrl = can.DrawFrame(0,-5,nPoints,5);
    hrl.SetBins(len(binName),0,nPoints);

    for ibin in range(hrl.GetNbinsX()-1):
        hrl.GetXaxis().SetBinLabel(ibin+1,binName[ibin]);

    hrl.GetYaxis().SetTitle("signal strenght");
    hrl.GetYaxis().SetTitleOffset(0.95)

    can.SetGrid();
   
    #leg2.Draw();

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


#################################
### make likelihood scan plot ###
#################################    

def makeLikelihoodScanPlot(filelist,variableName,variableLabel):

    nPoints = len(filelist);
    
    setStyle();

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

    for ifile in range(len(filelist)):

        for ivar in range(len(variableName)) :
            if filelist[ifile].find(variableName[ivar]+"_"+options.channel) != -1 :
                binName.append(variableLabel[ivar]);
                varName.append(variableName[ivar]);
                break;

        f = ROOT.TFile(filelist[ifile]);
        t = f.Get("limit");
        
        xbins_mu = array('f',[]); 
        ybins_mu = array('f',[]); 

        for ientry in range(t.GetEntries()):
            if ientry == 0 : continue ;   
            t.GetEntry(ientry);   
            xbins_mu.append(t.r); 
            ybins_mu.append(2*t.deltaNLL); 

        gr_mu = ROOT.TGraph(t.GetEntries()-1,xbins_mu,ybins_mu);
        gr_mu.Sort();
                                    
        gr_mu.SetLineWidth(2);
        gr_mu.SetLineColor(2);
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
    
    if options.makeAsymptoticLimitPlot :
        makeAsymptoticLimitPlot(filelist,variableName,variableLabel);
    elif options.makeProfileLikelihoodPlot :
        makeProfileLikelihoodPlot(filelist,variableName,variableLabel);
    elif options.makeLikelihoodScanPlot :
        makeLikelihoodScanPlot(filelist,variableName,variableLabel);    
    elif options.makeMaxLikelihoodFitPlot :
        makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel);

    os.system("rm list.txt");    
