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
parser.add_option('--outputPlotDIR',       action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariableList',   action="store", type="string", dest="inputVariableList", default="")

## make plots
parser.add_option('--makeAsymptoticPlot',           action="store", type="int",    dest="makeAsymptoticPlot",   default=0)
parser.add_option('--makeProfileLikelihoodPlot',    action="store", type="int",    dest="makeProfileLikelihoodPlot", default=0)
parser.add_option('--makeLikelihoodScanPlot',       action="store", type="int",    dest="makeLikelihoodScanPlot",    default=0)
parser.add_option('--makeMaxLikelihoodFitPlot',     action="store", type="int",    dest="makeMaxLikelihoodFitPlot",  default=0)

parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)

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

def getSignalStrenghtMLFit(file, muValue):

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
  gStyle.SetPadRightMargin(0.11);#0.02);                                                                                                                                     

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

    ## the matrix is always squared    
    nvar = len(variableName);

    histoExp   = ROOT.TH2F("histoExp","Asympt. CL_{S} Expected",nvar,0,nvar,nvar,0,nvar);
    histo1sUp  = ROOT.TH2F("histo1sUp","Asympt. CL_{S} Expected +1#sigma",nvar,0,nvar,nvar,0,nvar);
    histo2sUp  = ROOT.TH2F("histo2sUp","Asympt. CL_{S} Expected +2#sigma",nvar,0,nvar,nvar,0,nvar);
    histo1sDw  = ROOT.TH2F("histo1sDw","Asympt. CL_{S} Expected -1#sigma",nvar,0,nvar,nvar,0,nvar);
    histo2sDw  = ROOT.TH2F("histo2sDw","Asympt. CL_{S} Expected -2#sigma",nvar,0,nvar,nvar,0,nvar);


    for ivar in range(nvar) :

        histoExp.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histoExp.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        histo1sUp.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histo1sUp.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        histo2sUp.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histo2sUp.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        histo1sDw.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histo1sDw.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        histo2sDw.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histo2sDw.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);


    varUsed = [];

    limitExp     = ROOT.TH1F("limitExp","",1000,-1,10);
    limitExp1sUp = ROOT.TH1F("limitExp1sUp","",1000,-1,10);
    limitExp2sUp = ROOT.TH1F("limitExp2sUp","",1000,-1,10);
    limitExp1sDw = ROOT.TH1F("limitExp1sDw","",1000,-1,10);
    limitExp2sDw = ROOT.TH1F("limitExp2sDw","",1000,-1,10);
    limitExp.Sumw2();
    limitExp1sUp.Sumw2();
    limitExp2sUp.Sumw2();
    limitExp1sDw.Sumw2();
    limitExp2sDw.Sumw2();

    for ivarX in range(len(variableName)) :
        for ivarY in range(len(variableName)) :
            if ivarX == ivarY : 
                continue ;

            if not variableName[ivarX]+"_"+variableName[ivarY] in varUsed :

                varUsed.append(variableName[ivarX]+"_"+variableName[ivarY]);
                
                for ifile in range(len(filelist)):
 
                    if filelist[ifile].find(variableName[ivarX]+"_"+variableName[ivarY]+"_"+options.channel) != - 1 :

                        limitExp.Reset("ICES");
                        limitExp1sUp.Reset("ICES");
                        limitExp2sUp.Reset("ICES");
                        limitExp1sDw.Reset("ICES");
                        limitExp2sDw.Reset("ICES");

                        getAsymptoticLimit(filelist[ifile],limitExp,limitExp1sUp,limitExp1sDw,limitExp2sUp,limitExp2sDw);                                              

                        histoExp.SetBinContent(ivarX+1,ivarY+1,round(limitExp.GetMean(),3));                                     
                        histo1sUp.SetBinContent(ivarX+1,ivarY+1,round(limitExp1sUp.GetMean(),3));
                        histo2sUp.SetBinContent(ivarX+1,ivarY+1,round(limitExp2sUp.GetMean(),3));
                        histo1sDw.SetBinContent(ivarX+1,ivarY+1,round(limitExp1sDw.GetMean(),3));
                        histo2sDw.SetBinContent(ivarX+1,ivarY+1,round(limitExp2sDw.GetMean(),3));

                        histoExp.SetBinContent(ivarY+1,ivarX+1,round(limitExp.GetMean(),3));                                     
                        histo1sUp.SetBinContent(ivarY+1,ivarX+1,round(limitExp1sUp.GetMean(),3));
                        histo2sUp.SetBinContent(ivarY+1,ivarX+1,round(limitExp2sUp.GetMean(),3));
                        histo1sDw.SetBinContent(ivarY+1,ivarX+1,round(limitExp1sDw.GetMean(),3));
                        histo2sDw.SetBinContent(ivarY+1,ivarX+1,round(limitExp2sDw.GetMean(),3));

       
    histoExp.SetMarkerStyle(20);
    histoExp.SetLineWidth(3);
    histoExp.SetLineStyle(1);
    histoExp.SetMarkerColor(ROOT.kBlack);

    histo1sUp.SetMarkerStyle(20);
    histo1sUp.SetLineWidth(3);
    histo1sUp.SetLineStyle(1);
    histo1sUp.SetMarkerColor(ROOT.kBlack);

    histo2sUp.SetMarkerStyle(20);
    histo2sUp.SetLineWidth(3);
    histo2sUp.SetLineStyle(1);
    histo2sUp.SetMarkerColor(ROOT.kBlack);

    histo1sDw.SetMarkerStyle(20);
    histo1sDw.SetLineWidth(3);
    histo1sDw.SetLineStyle(1);
    histo1sDw.SetMarkerColor(ROOT.kBlack);

    histo2sDw.SetMarkerStyle(20);
    histo2sDw.SetLineWidth(3);
    histo2sDw.SetLineStyle(1);
    histo2sDw.SetMarkerColor(ROOT.kBlack);

                               
    ## canvas
    can = ROOT.TCanvas("can","can",650,650);

    can.SetGridx(1);
    can.SetGridy(1);
                   
    histoExp.Draw("colz TEXT");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    histoExp.GetZaxis().SetLabelSize(0.03);
    histoExp.GetZaxis().SetTitleSize(0.03);
    histoExp.GetZaxis().SetRangeUser(histoExp.GetMinimum()*0.75,histoExp.GetMaximum());
    histoExp.GetZaxis().SetTitleOffset(0.95);

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

    can.SaveAs("%s/AsymptoticLimit_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/AsymptoticLimit_%s.pdf"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/AsymptoticLimit_%s.root"%(options.outputPlotDIR,options.channel));
    
    histo1sUp.GetZaxis().SetLabelSize(0.03);
    histo1sUp.GetZaxis().SetTitleSize(0.03);
    histo1sUp.GetZaxis().SetRangeUser(histo1sUp.GetMinimum()*0.75,histo1sUp.GetMaximum());
    histo1sUp.GetZaxis().SetTitleOffset(0.95);

    histo1sUp.Draw("colz TEXT");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    tex.Draw();
    tex2.Draw();
    tex3.Draw();

    #can.SaveAs("%s/AsymptoticLimit_1sUp_%s.png"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_1sUp_%s.pdf"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_1sUp_%s.root"%(options.outputPlotDIR,options.channel));

    histo2sUp.GetZaxis().SetLabelSize(0.03);
    histo2sUp.GetZaxis().SetTitleSize(0.03);
    histo2sUp.GetZaxis().SetRangeUser(histo2sUp.GetMinimum()*0.75,histo2sUp.GetMaximum());
    histo2sUp.GetZaxis().SetTitleOffset(0.95);

    histo2sUp.Draw("colz TEXT");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    tex.Draw();
    tex2.Draw();
    tex3.Draw();

    #can.SaveAs("%s/AsymptoticLimit_2sUp_%s.png"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_2sUp_%s.pdf"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_2sUp_%s.root"%(options.outputPlotDIR,options.channel));

    histo1sDw.GetZaxis().SetLabelSize(0.03);
    histo1sDw.GetZaxis().SetTitleSize(0.03);
    histo1sDw.GetZaxis().SetRangeUser(histo1sDw.GetMinimum()*0.75,histo1sDw.GetMaximum());
    histo1sDw.GetZaxis().SetTitle("Asymptotic Limit -1#sigma");
    histo1sDw.GetZaxis().SetTitleOffset(0.95);

    histo1sDw.Draw("colz TEXT");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    tex.Draw();
    tex2.Draw();
    tex3.Draw();

    #can.SaveAs("%s/AsymptoticLimit_1sDw_%s.png"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_1sDw_%s.pdf"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_1sDw_%s.root"%(options.outputPlotDIR,options.channel));

    histo2sDw.GetZaxis().SetLabelSize(0.03);
    histo2sDw.GetZaxis().SetTitleSize(0.03);
    histo2sDw.GetZaxis().SetRangeUser(histo2sDw.GetMinimum()*0.75,histo2sDw.GetMaximum());
    histo2sDw.GetZaxis().SetTitle("Asymptotic Limit -2#sigma");
    histo2sDw.GetZaxis().SetTitleOffset(0.95);

    histo2sDw.Draw("colz TEXT");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    tex.Draw();
    tex2.Draw();
    tex3.Draw();

    #can.SaveAs("%s/AsymptoticLimit_2sDw_%s.png"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_2sDw_%s.pdf"%(options.outputPlotDIR,options.channel));
    #can.SaveAs("%s/AsymptoticLimit_2sDw_%s.root"%(options.outputPlotDIR,options.channel));
    

##############################
#### Make SM PValue Plots ####  
##############################  

def makeProfileLikelihoodPlot(filelist,variableName,variableLabel):

    ## the matrix is always squared                                                                                                                                             
    nvar = len(variableName);

    histoExp     = ROOT.TH2F("histoExp","",nvar,0,nvar,nvar,0,nvar);    
    histoExp_err = ROOT.TH2F("histoExp_err","",nvar,0,nvar,nvar,0,nvar);    

    for ivar in range(nvar) :

        histoExp.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histoExp.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        histoExp_err.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        histoExp_err.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);


    varUsed = [] ;

    signifExp = ROOT.TH1F("signifExp","",1000,0,150);

    for ivarX in range(len(variableName)) :
        for ivarY in range(len(variableName)) :
            if ivarX == ivarY :
                continue ;

            if not variableName[ivarX]+"_"+variableName[ivarY] in varUsed :

                varUsed.append(variableName[ivarX]+"_"+variableName[ivarY]);

                for ifile in range(len(filelist)):

                    if filelist[ifile].find(variableName[ivarX]+"_"+variableName[ivarY]+"_"+options.channel) != - 1 :
                        
                        signifExp.Reset("ICES");
                        getExpectedQuantile(filelist[ifile],signifExp);                

                        histoExp.SetBinContent(ivarX+1,ivarY+1,round(signifExp.GetMean(),2));
                        histoExp_err.SetBinContent(ivarX+1,ivarY+1,round(signifExp.GetRMS(),2));

                        histoExp.SetBinContent(ivarY+1,ivarX+1,round(signifExp.GetMean(),2));
                        histoExp_err.SetBinContent(ivarY+1,ivarX+1,round(signifExp.GetRMS(),2));

                         
    histoExp.GetZaxis().SetLabelSize(0.03);
    histoExp.GetZaxis().SetTitleSize(0.03);

    can = ROOT.TCanvas("can","can",650,650);

    can.SetGrid();

    histoExp.Draw("colz TEXT");

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


    histoExp_err.Draw("colz TEXT");

    histoExp_err.GetZaxis().SetLabelSize(0.03);
    histoExp_err.GetZaxis().SetTitleSize(0.03);

    tex.Draw();
    tex2.Draw();
    tex3.Draw();

    can.SaveAs("%s/ProfileLikelihood_err_%s.png"%(options.outputPlotDIR,options.channel));
    can.SaveAs("%s/ProfileLikelihood_err_%s.pdf"%(options.outputPlotDIR,options.channel));


####################################
#### Make Signal Strenght Plots ####  
####################################  

def makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel):


    ## the matrix is always squared                                                                                                                                             
    nvar = len(variableName);

    muValue         = ROOT.TH2F("muValue","",nvar,0,nvar,nvar,0,nvar);
    muErrUpOneSigma = ROOT.TH2F("muErrUpOneSigma","",nvar,0,nvar,nvar,0,nvar);
    muErrUpTwoSigma = ROOT.TH2F("muErrUpTwoSigma","",nvar,0,nvar,nvar,0,nvar);
    muErrDownOneSigma = ROOT.TH2F("muErrDownOneSigma","",nvar,0,nvar,nvar,0,nvar);
    muErrDownTwoSigma = ROOT.TH2F("muErrDownTwoSigma","",nvar,0,nvar,nvar,0,nvar);

    muValueTemp         = ROOT.TH1F("muValueTemp","",100,-10,10);
    muErrUpOneSigmaTemp = ROOT.TH1F("muErrUpOneSigmaTemp","",100,-10,10);
    muErrUpTwoSigmaTemp = ROOT.TH1F("muErrUpTwoSigmaTemp","",100,-10,10);
    muErrDownOneSigmaTemp = ROOT.TH1F("muErrDownOneSigmaTemp","",100,-10,10);
    muErrDownTwoSigmaTemp = ROOT.TH1F("muErrDownTwoSigmaTemp","",100,-10,10);

    for ivar in range(nvar):

        muValue.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        muValue.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        muErrUpOneSigma.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        muErrUpOneSigma.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        muErrUpTwoSigma.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        muErrUpTwoSigma.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        muErrDownOneSigma.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        muErrDownOneSigma.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);

        muErrDownTwoSigma.GetYaxis().SetBinLabel(ivar+1,variableLabel[ivar]);
        muErrDownTwoSigma.GetXaxis().SetBinLabel(ivar+1,variableLabel[ivar]);


    varUsed = [];

    for ivarX in range(len(variableName)) :
        for ivarY in range(len(variableName)) :
            if ivarX == ivarY :
                continue ;

            if not variableName[ivarX]+"_"+variableName[ivarY] in varUsed :

                varUsed.append(variableName[ivarX]+"_"+variableName[ivarY]);

                for ifile in range(len(filelist)):

                    if filelist[ifile].find(variableName[ivarX]+"_"+variableName[ivarY]+"_"+options.channel) != - 1 :

                        muValueTemp.Reset("ICES");
                        muErrUpOneSigmaTemp.Reset("ICES");
                        muErrUpTwoSigmaTemp.Reset("ICES");
                        muErrDownOneSigmaTemp.Reset("ICES");
                        muErrDownTwoSigmaTemp.Reset("ICES");

                        getSignalStrenght(filelist[ifile], muValueTemp, muErrUpOneSigmaTemp, muErrUpTwoSigmaTemp, muErrDownOneSigmaTemp, muErrDownTwoSigmaTemp)
                    
                        muValue.SetBinContent(ivarX+1,ivarY+1,round(muValueTemp.GetMean(),2));
                        muErrUpOneSigma.SetBinContent(ivarX+1,ivarY+1,round(muErrUpOneSigmaTemp.GetMean(),2));
                        muErrUpTwoSigma.SetBinContent(ivarX+1,ivarY+1,round(muErrUpTwoSigmaTemp.GetMean(),2));
                        muErrDownOneSigma.SetBinContent(ivarX+1,ivarY+1,round(muErrDownOneSigmaTemp.GetMean(),2));
                        muErrDownTwoSigma.SetBinContent(ivarX+1,ivarY+1,round(muErrDownTwoSigmaTemp.GetMean(),2));

                        muValue.SetBinContent(ivarY+1,ivarX+1,round(muValueTemp.GetMean(),2));
                        muErrUpOneSigma.SetBinContent(ivarY+1,ivarX+1,round(muErrUpOneSigmaTemp.GetMean(),2));
                        muErrUpTwoSigma.SetBinContent(ivarY+1,ivarX+1,round(muErrUpTwoSigmaTemp.GetMean(),2));
                        muErrDownOneSigma.SetBinContent(ivarY+1,ivarX+1,round(muErrDownOneSigmaTemp.GetMean(),2));
                        muErrDownTwoSigma.SetBinContent(ivarY+1,ivarX+1,round(muErrDownTwoSigmaTemp.GetMean(),2));


         
    can = ROOT.TCanvas("can","can",650,650);

    can.SetGrid();

    muValue.Draw("colz TEXT");   

    muValue.GetZaxis().SetLabelSize(0.03);
    muValue.GetZaxis().SetTitleSize(0.03);
    muValue.GetZaxis().SetRangeUser(0.8,1.2);

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
   
    can.SaveAs("%s/maximumLikelihoodFit_Mu_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    can.SaveAs("%s/maximumLikelihoodFit_Mu_%s.png"%(options.outputPlotDIR,options.channel),"png");


    muErrUpOneSigma.Draw("colz TEXT");   
 
    muErrUpOneSigma.GetZaxis().SetLabelSize(0.03);
    muErrUpOneSigma.GetZaxis().SetTitleSize(0.03);

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();

   
    can.SaveAs("%s/maximumLikelihoodFit_Mu_Up1s_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    can.SaveAs("%s/maximumLikelihoodFit_Mu_Up1s_%s.png"%(options.outputPlotDIR,options.channel),"png");

    muErrUpTwoSigma.GetZaxis().SetLabelSize(0.03);
    muErrUpTwoSigma.GetZaxis().SetTitleSize(0.03);

    muErrUpTwoSigma.Draw("colz TEXT");   

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();
   
    #can.SaveAs("%s/maximumLikelihoodFit_Mu_Up2s_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    #can.SaveAs("%s/maximumLikelihoodFit_Mu_Up2s_%s.png"%(options.outputPlotDIR,options.channel),"png");

    muErrDownOneSigma.GetZaxis().SetLabelSize(0.03);
    muErrDownOneSigma.GetZaxis().SetTitleSize(0.03);

    muErrDownOneSigma.Draw("colz TEXT");   

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();
   
    can.SaveAs("%s/maximumLikelihoodFit_Mu_Dw1s_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    can.SaveAs("%s/maximumLikelihoodFit_Mu_Dw1s_%s.png"%(options.outputPlotDIR,options.channel),"png");

    muErrDownTwoSigma.GetZaxis().SetLabelSize(0.03);
    muErrDownTwoSigma.GetZaxis().SetTitleSize(0.03);

    muErrDownTwoSigma.Draw("colz TEXT");   

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();

    #can.SaveAs("%s/maximumLikelihoodFit_Mu_Dw2s_%s.pdf"%(options.outputPlotDIR,options.channel),"pdf");
    #can.SaveAs("%s/maximumLikelihoodFit_Mu_Dw2s_%s.png"%(options.outputPlotDIR,options.channel),"png");


#################################
### make likelihood scan plot ###
#################################    

def makeLikelihoodScanPlot(filelist,variableName,variableLabel):

    ## the matrix is always squared                                                                                                                                             
    nvar = len(variableName);

    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);

    tex2 = ROOT.TLatex(0.173,0.957,"Delphes");
    tex2.SetNDC();
    tex2.SetTextFont(61);
    tex2.SetTextSize(0.04);
    tex2.SetLineWidth(2);

    tex3 = ROOT.TLatex(0.332,0.957,"Simulation Preliminary");
    tex3.SetNDC();
    tex3.SetTextFont(52);
    tex3.SetTextSize(0.035);
    tex3.SetLineWidth(2);

    varUsed = [];

    for ivarX in range(len(variableName)) :
        for ivarY in range(len(variableName)) :
            if ivarX == ivarY :
                continue ;

            if not variableName[ivarX]+"_"+variableName[ivarY] in varUsed :

                varUsed.append(variableName[ivarX]+"_"+variableName[ivarY]);

                for ifile in range(len(filelist)):

                    if filelist[ifile].find(variableName[ivarX]+"_"+variableName[ivarY]+"_"+options.channel) != - 1 :

                        f = ROOT.TFile(filelist[ifile]);
                        t = f.Get("limit");

                        mapMuDnll = ROOT.TH1F("mapMuDnll","",150,options.rMin,options.rMax);


                        for ientry in range(t.GetEntries()):
                            if ientry == 0 : continue ;   
                            t.GetEntry(ientry);   
                            mapMuDnll.Fill(t.r,2*t.deltaNLL);

                        gr_mu = ROOT.TGraph(mapMuDnll);
                        gr_mu.SetMinimum(options.rMin*1.2);
                        gr_mu.SetMaximum(options.rMax*0.9);
                        gr_mu.GetXaxis().SetRangeUser(options.rMin*1.05,options.rMax*0.95);
                        gr_mu.GetYaxis().SetRangeUser(0,gr_mu.GetMaximum()*0.9);

                        gr_mu.SetLineWidth(2);
                        gr_mu.SetLineColor(2);
                        gr_mu.SetMarkerStyle(20);

                        can = ROOT.TCanvas("can_%s_%s"%(variableName[ivarX],variableName[ivarY]),"can_%s_%s"%(variableLabel[ivarX],variableLabel[ivarY]),700,650);
                        gr_mu.GetYaxis().SetTitle("-2#Delta ln(L)");
                        gr_mu.GetYaxis().SetTitleOffset(1.0);

                        gr_mu.GetXaxis().SetTitle("signal strenght (%s)"%(variableLabel[ivarX]+variableLabel[ivarY]));
                        can.SetGrid();
                        gr_mu.Draw("AC");

                        tex.Draw();
                        tex2.Draw();
                        tex3.Draw();
                        
                        can.SaveAs("%s/LikelihoodScan_%s_%s.pdf"%(options.outputPlotDIR,variableName[ivarX]+"_"+variableName[ivarY],options.channel),"pdf");
                        can.SaveAs("%s/LikelihoodScan_%s_%s.png"%(options.outputPlotDIR,variableName[ivarX]+"_"+variableName[ivarY],options.channel),"png");
                                           


##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## make the list of variables
    variableName  = [];
    variableLabel = [];

    fileVar = open('%s'%(options.inputVariableList), 'r');
    for columns in ( raw.strip().split() for raw in fileVar ):
        if columns[0].find('#')!=-1 or columns[0] == ' ': continue;

        if not columns[0] in variableName :
            variableName.append(columns[0]);

        if not columns[4] in variableLabel :
            variableLabel.append(columns[4]);

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

    setStyle();
        
    if options.makeAsymptoticPlot :
        makeAsymptoticLimitPlot(filelist,variableName,variableLabel);
    elif options.makeProfileLikelihoodPlot :
        makeProfileLikelihoodPlot(filelist,variableName,variableLabel);
    elif options.makeLikelihoodScanPlot :
        makeLikelihoodScanPlot(filelist,variableName,variableLabel);    
    elif options.makeMaxLikelihoodFitPlot :
        makeMaxLikelihoodFitPlot(filelist,variableName,variableLabel);

    os.system("rm list.txt");    
