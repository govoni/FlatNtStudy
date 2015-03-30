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
parser.add_option('--channel',   action="store", type="string", dest="channel",     default="UUpp")
parser.add_option('--outputPlotDIR',   action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariable',   action="store", type="string", dest="inputVariable", default="")

## make plots
parser.add_option('--makeAsymptoticPlot',           action="store", type="int",    dest="makeAsymptoticLimitPlot",   default=0)
parser.add_option('--makeProfileLikelihoodPlot',    action="store", type="int",    dest="makeProfileLikelihoodPlot", default=0)
parser.add_option('--makeMaxLikelihoodFitPlot',     action="store", type="int",    dest="makeMaxLikelihoodFitPlot",  default=0)

(options, args) = parser.parse_args()

fakeRateScaleFactor     = [0.2,0.5,0.7,0.8,0.9,1.1,1.2,1.5,2,2.5,3.0,4.0,5.0];

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


##############################
#### Make SM Limits Plots ####  
##############################  

def makeAsymptoticLimitPlot(filelist):

    xbins        = array('f', []); 
    xbins_err_up = array('f', []); 
    xbins_err_dw = array('f', []); 
    ybins_exp    = array('f', []); 
    ybins_1s_up  = array('f', []); 
    ybins_2s_up  = array('f', []);
    ybins_1s_dw  = array('f', []); 
    ybins_2s_dw  = array('f', []);

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


    for fake in fakeRateScaleFactor :
        for ifile in range(len(filelist)):
            if filelist[ifile].find("_%d"%(fake*10)) != -1 :

                limitExp.Reset("ICES");
                limitExp1sUp.Reset("ICES");
                limitExp2sUp.Reset("ICES");
                limitExp1sDw.Reset("ICES");
                limitExp2sDw.Reset("ICES");

                getAsymptoticLimit(filelist[ifile],limitExp,limitExp1sUp,limitExp1sDw,limitExp2sUp,limitExp2sDw);
     
                xbins.append(fake);
                if fake < 0.5 :
                    xbins_err_up.append(0.05);
                    xbins_err_dw.append(0.05);
                else :
                    xbins_err_up.append(0.01);
                    xbins_err_dw.append(0.01);

                ybins_exp.append(limitExp.GetMean());
                ybins_2s_dw.append(limitExp2sDw.GetMean());
                ybins_1s_dw.append(limitExp1sDw.GetMean());
                ybins_2s_up.append(limitExp2sUp.GetMean());
                ybins_1s_up.append(limitExp1sUp.GetMean());

                break;




    ## make the plot and setting the style                               
    curGraph_exp = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins,ybins_exp);
    curGraph_1s  = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_1s_dw,ybins_1s_up);
    curGraph_2s  = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins,ybins_exp,xbins_err_dw,xbins_err_up,ybins_2s_dw,ybins_2s_up);


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
                               
    oneLine = ROOT.TF1("oneLine","1",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]);
    oneLine.SetLineColor(ROOT.kRed);
    oneLine.SetLineWidth(3);
    
    ## canvas
    can_SM = ROOT.TCanvas("can_SM","can_SM",800,650);

    curGraph_2s.GetYaxis().SetTitle("#mu = #sigma_{95%} / #sigma_{SM}");
    curGraph_2s.GetYaxis().SetTitleOffset(1.1);
    curGraph_2s.GetYaxis().SetTitleSize(0.042);
    curGraph_2s.GetYaxis().SetTitleFont(42);

    curGraph_2s.GetXaxis().SetTitleSize(0.045);
    curGraph_2s.GetXaxis().SetTitleFont(42);
    curGraph_2s.GetXaxis().SetTitle("fake rate scale factor");

    curGraph_2s.GetYaxis().SetNdivisions(505);
                   
    curGraph_2s.Draw("a2");
    curGraph_1s.Draw("2same");
    curGraph_exp.Draw("Psame");

    can_SM.SetGridx(1);
    can_SM.SetGridy(1);

    leg2 = ROOT.TLegend(0.53,0.71,0.91,0.92);
    leg2.SetFillColor(0);
    leg2.SetShadowColor(0);
    leg2.SetTextFont(42);
    leg2.SetTextSize(0.025);

    leg2.AddEntry(curGraph_exp,"Asympt. CL_{S} Expected","L")
    leg2.AddEntry(curGraph_1s, "Asympt. CL_{S} Expected #pm 1#sigma","LF")
    leg2.AddEntry(curGraph_2s, "Asympt. CL_{S} Expected #pm 2#sigma","LF")


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

    can_SM.SetLogy();

    can_SM.SaveAs("%s/AsymptoticLimit_%s_log.png"%(options.outputPlotDIR,options.channel));
    can_SM.SaveAs("%s/AsymptoticLimit_%s_log.pdf"%(options.outputPlotDIR,options.channel));
    can_SM.SaveAs("%s/AsymptoticLimit_%s_log.root"%(options.outputPlotDIR,options.channel));


##############################
#### Make SM PValue Plots ####  
##############################  

def makeProfileLikelihoodPlot(filelist):

    xbins_exp           = array('f', []);
    ybins_exp           = array('f', []);
    xbins_err           = array('f', []);
    ybins_err           = array('f', []);

    ## histogram
    signifExp = ROOT.TH1F("signifExp","",10000,0,150);
    signifExp.Sumw2();

    for fake in fakeRateScaleFactor  :
        for ifile in range(len(filelist)):
            if filelist[ifile].find("_%d"%(fake*10)) != -1 :

                xbins_exp.append(fake); 
                xbins_err.append(0.); 

                signifExp.Reset("ICES");
                
                getExpectedQuantile(filelist[ifile],signifExp);
                ybins_exp.append(signifExp.GetMean());
                ybins_err.append(signifExp.GetMeanError());
                break;


    gr_exp = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins_exp,ybins_exp,xbins_err,xbins_err,ybins_err,ybins_err);
                         
    gr_exp.SetLineColor(1); 
    gr_exp.SetMarkerColor(1); 
    gr_exp.SetMarkerStyle(20); 
    gr_exp.SetLineWidth(3);
    gr_exp.SetMarkerSize(1.6);
    gr_exp.SetLineStyle(8);

    oneSLine = ROOT.TF1("oneSLine","1",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]+1);
    oneSLine.SetLineColor(ROOT.kRed); oneSLine.SetLineWidth(2); oneSLine.SetLineStyle(2);
    twoSLine = ROOT.TF1("twoSLine","2",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]+1);
    twoSLine.SetLineColor(ROOT.kRed); twoSLine.SetLineWidth(2); twoSLine.SetLineStyle(2);
    threeSLine = ROOT.TF1("threeSLine","3",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]+1);
    threeSLine.SetLineColor(ROOT.kRed); threeSLine.SetLineWidth(2); threeSLine.SetLineStyle(2);
    fourSLine = ROOT.TF1("fourSLine","4",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]+1);
    fourSLine.SetLineColor(ROOT.kRed); fourSLine.SetLineWidth(2); fourSLine.SetLineStyle(2);
    fiveSLine = ROOT.TF1("fiveSLine","5",0,fakeRateScaleFactor[len(fakeRateScaleFactor)-1]+1);
    fiveSLine.SetLineColor(ROOT.kRed); fiveSLine.SetLineWidth(2); fiveSLine.SetLineStyle(2);
    
    can = ROOT.TCanvas("can","can",800,650);

    gr_exp.GetYaxis().SetTitleOffset(0.95);
    gr_exp.GetYaxis().SetTitle("significance (#sigma)");

    gr_exp.GetXaxis().SetTitle("fake rate scale factor");

    can.SetGrid();

    gr_exp.Draw("aP");

    oneSLine.Draw("same");
    twoSLine.Draw("same");
    threeSLine.Draw("same");
    fourSLine.Draw("same");
    fiveSLine.Draw("same");

    tex = ROOT.TLatex(0.91,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

    tex2 = ROOT.TLatex(0.143,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);
    tex2.Draw();

    tex3 = ROOT.TLatex(0.30,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);
    tex3.Draw();

    can.SaveAs("%s/ProfileLikelihood_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s.pdf"%(options.outputPlotDIR,options.channel));

    can.SetLogy();

    can.SaveAs("%s/ProfileLikelihood_%s_log.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s_log.pdf"%(options.outputPlotDIR,options.channel));

    can.SetLogy(0);

    evolution = ROOT.TF1 ("evolution", "[0]/TMath::Sqrt([2]*[2] +[1]*[1]*x)", 0, 30) ;
    evolution.SetParameter (0, gr_exp.GetMaximum()) ;
    evolution.SetParameter (1, 0.5) ;
    evolution.SetParameter (2, 0.5) ;
    gr_exp.Fit ("evolution","RMEQEX0") ;

    can.cd();
    can.SetGrid();

    evolution.SetLineColor(ROOT.kRed);
    evolution.SetLineWidth(2);

    evolution.Draw();
    evolution.GetYaxis().SetTitleOffset(0.95);
    evolution.GetYaxis().SetTitle("significance (#sigma)");

    evolution.GetXaxis().SetTitle("fake rate scale factor");
    gr_exp.Draw("P");
    evolution.Draw("same");

    can.SaveAs("%s/ProfileLikelihood_%s_vsFake.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s_vsFake.pdf"%(options.outputPlotDIR,options.channel));

    can.SetLogy();

    can.SaveAs("%s/ProfileLikelihood_%s_vsFake_log.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_%s_vsFake_log.pdf"%(options.outputPlotDIR,options.channel));



####################################
#### Make Signal Strenght Plots ####  
####################################  

def makeMaxLikelihoodFitPlot(filelist):

    xbins_mu         = array('f', []);
    xbins_mu_err_up  = array('f', []);
    xbins_mu_err_dn  = array('f', []);
    ybins_mu         = array('f', []);
    ybins_mu_err_up  = array('f', []);
    ybins_mu_err_dn  = array('f', []);
    ybins_mu_err_up_2s  = array('f', []);
    ybins_mu_err_dn_2s  = array('f', []);

    muValue         = ROOT.TH1F("muValue","",100,-10,10);
    muErrUpOneSigma = ROOT.TH1F("muErrUpOneSigma","",100,-10,10);
    muErrUpTwoSigma = ROOT.TH1F("muErrUpTwoSigma","",100,-10,10);
    muErrDownOneSigma = ROOT.TH1F("muErrDownOneSigma","",100,-10,10);
    muErrDownTwoSigma = ROOT.TH1F("muErrDownTwoSigma","",100,-10,10);

    muValue.Sumw2();
    muErrUpOneSigma.Sumw2();
    muErrUpTwoSigma.Sumw2();
    muErrDownOneSigma.Sumw2();
    muErrDownTwoSigma.Sumw2();

        
    for fake in fakeRateScaleFactor :
        for ifile in range(len(filelist)):
            if filelist[ifile].find("_%d"%(fake*10)) != -1 :

                muValue.Reset("ICES");
                muErrUpOneSigma.Reset("ICES");
                muErrUpTwoSigma.Reset("ICES");
                muErrDownOneSigma.Reset("ICES");
                muErrDownTwoSigma.Reset("ICES");

                getSignalStrenght(filelist[ifile],muValue, muErrUpOneSigma, muErrUpTwoSigma, muErrDownOneSigma, muErrDownTwoSigma)
     
                xbins_mu.append(fake); 
                xbins_mu_err_up.append(0); 
                xbins_mu_err_dn.append(0); 

                ybins_mu.append(muValue.GetMean());
                ybins_mu_err_up.append(muErrUpOneSigma.GetMean());
                ybins_mu_err_dn.append(muErrDownOneSigma.GetMean());
                ybins_mu_err_up_2s.append(muErrUpTwoSigma.GetMean());
                ybins_mu_err_dn_2s.append(muErrDownTwoSigma.GetMean());                


                break;

            

    gr_mu_1s = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn,ybins_mu_err_up);
    gr_mu_1s.SetLineColor(1); gr_mu_1s.SetMarkerColor(1); gr_mu_1s.SetMarkerStyle(20); gr_mu_1s.SetLineWidth(5); gr_mu_1s.SetMarkerSize(1.6);

    gr_mu_2s = ROOT.TGraphAsymmErrors(len(fakeRateScaleFactor),xbins_mu,ybins_mu,xbins_mu_err_dn,xbins_mu_err_up,ybins_mu_err_dn_2s,ybins_mu_err_up_2s);
    gr_mu_2s.SetLineColor(ROOT.kBlue); gr_mu_2s.SetMarkerColor(ROOT.kBlue); gr_mu_2s.SetMarkerStyle(20); gr_mu_2s.SetLineWidth(3); gr_mu_2s.SetMarkerSize(1.6);

    ban1s = TLatex(950,1.,("#mu SM injected"));
    ban1s.SetTextSize(0.028); ban1s.SetTextColor(1)

 
    can = ROOT.TCanvas("can","can",800,650);

    gr_mu_2s.GetYaxis().SetTitle("signal strenght");
    gr_mu_2s.GetYaxis().SetTitleOffset(0.95)

    gr_mu_2s.GetXaxis().SetTitle("fake scale factor");

    can.SetGrid();
   
    gr_mu_2s.Draw("aP");
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

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## fix the chdir
    os.chdir(options.fileDIR);

    ## create output plot directory
    os.system("mkdir -p "+options.outputPlotDIR);

    ## create the file list    
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

    ## make the list of files
    file = open("list.txt",'r')
    filelist = [];

    setStyle();


    for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));

    if options.makeAsymptoticLimitPlot :
        makeAsymptoticLimitPlot(filelist);
    elif options.makeProfileLikelihoodPlot :
        makeProfileLikelihoodPlot(filelist);
    elif options.makeMaxLikelihoodFitPlot :
        makeMaxLikelihoodFitPlot(filelist);

    os.system("rm list.txt");    
 
