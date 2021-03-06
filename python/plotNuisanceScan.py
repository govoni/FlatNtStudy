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
parser.add_option('--inputVariable',   action="store", type="string", dest="inputVariable",     default="R_mll")
parser.add_option('--outputPlotDIR',   action="store", type="string", dest="outputPlotDIR", default="")

## make plots
parser.add_option('--makeAsymptoticPlot',           action="store", type="int",    dest="makeAsymptoticLimitPlot",   default=0)
parser.add_option('--makeProfileLikelihoodPlot',    action="store", type="int",    dest="makeProfileLikelihoodPlot", default=0)
parser.add_option('--makeMaxLikelihoodFitPlot',     action="store", type="int",    dest="makeMaxLikelihoodFitPlot",  default=0)
parser.add_option('--makeUncertaintyPlot',          action="store", type="int",    dest="makeUncertaintyPlot",       default=0)

(options, args) = parser.parse_args()

######################################################################
#### get signal strencht from max likelihood fit from limit tree ####
######################################################################

def getSignalStrenght(ifile,muValue,muErrUpOneSigma,muErrDownOneSigma):

 f = ROOT.TFile(ifile);
 t = f.Get("limit");
 entries = t.GetEntries();

 yMu     = []; yMu1sUp = []; yMu1sDw = [];

 ievent1sDw = 0;
 ievent1sUp = 0;
 ievent = 0;

 for i in range(entries):

  t.GetEntry(i);

  t_quantileExpected = t.quantileExpected;
  t_limit = t.limit;

  if t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17: 
      yMu1sDw.append((ievent1sDw,t_limit));
      ievent1sDw = ievent1sDw +1

  elif t_quantileExpected == 0.5: 
      yMu.append((ievent,t_limit));
      ievent = ievent +1

  elif t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85: 
      yMu1sUp.append((ievent1sUp,t_limit));
      ievent1sUp = ievent1sUp +1

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

def makeAsymptoticLimitPlot(filelist,filelist_Sys,filelist_add,filelist_remove):

    print "dummy implementation"

##############################
#### Make SM PValue Plots ####  
##############################  

def makeProfileLikelihoodPlot(filelist,filelist_Sys,filelist_add,filelist_remove):

    print "dummy implementation"



####################################
#### Make Signal Strenght Plots ####  
####################################  

def makeMaxLikelihoodFitPlot(filelist,filelist_Sys,filelist_add,filelist_remove):

    print "dummy implementation"


##############################                                                                                                                                                
### make uncertaintyb plot ###                                                                                                                                               
##############################                                                                                                                                                 

def makeUncertaintyPlot(filelist,filelist_Sys,filelist_add,filelist_remove):

    ROOT.gStyle.SetPadBottomMargin(0.25)

    tex2 = TLatex(0.17,0.86,"CMS Delphes Simulation");
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


    ybins_mu_err        = 0;
    ybins_mu_err_noSys  = 0;

    if len(filelist_add) != len(filelist_remove):
        exit("add and remove list does not have the same number of files .. please check");

    ifile = 0;

    muValue           = ROOT.TH1F("muValue","",100,-10,10);
    muErrUpOneSigma   = ROOT.TH1F("muErrUpOneSigma","",100,-10,10);
    muErrDownOneSigma = ROOT.TH1F("muErrDownOneSigma","",100,-10,10);

    muValue.Sumw2();
    muErrUpOneSigma.Sumw2();
    muErrDownOneSigma.Sumw2();

    filelist_add.sort();
    filelist_remove.sort();

    labelList = [];

    histo_add      = ROOT.TH1F("ybins_mu_err_add","ybins_mu_err_add",len(filelist_add),0,len(filelist_add));
    histo_remove   = ROOT.TH1F("ybins_mu_err_remove","ybins_mu_err_remove",len(filelist_remove),0,len(filelist_remove));

    for file in filelist_add :
        
        muValue.Reset("ICES");
        muErrUpOneSigma.Reset("ICES");
        muErrDownOneSigma.Reset("ICES");

        getSignalStrenght(file,muValue, muErrUpOneSigma,muErrDownOneSigma)

        if muErrDownOneSigma.GetMean() == 0 :
            histo_add.SetBinContent(ifile+1,muErrUpOneSigma.GetMean());
        else:
            histo_add.SetBinContent(ifile+1,(muErrUpOneSigma.GetMean()+muErrDownOneSigma.GetMean())/2);

        ifile = ifile+1;

        labelList.append(file.split(".")[0].replace("higgsCombinefinal_","").replace(options.channel+"_","").replace(options.inputVariable+"_","").replace("add_","").replace("CMS_",""));

    ifile = 0 ;

    for file in filelist_remove :

        muValue.Reset("ICES");
        muErrUpOneSigma.Reset("ICES");
        muErrDownOneSigma.Reset("ICES");

        getSignalStrenght(file,muValue, muErrUpOneSigma,muErrDownOneSigma)

        if muErrDownOneSigma.GetMean() == 0 :
            histo_remove.SetBinContent(ifile+1,muErrUpOneSigma.GetMean());
        else:
            histo_remove.SetBinContent(ifile+1,(muErrUpOneSigma.GetMean()+muErrDownOneSigma.GetMean())/2);

        ifile = ifile+1;


    for file in filelist : ## take the central value
        
        muValue.Reset("ICES");
        muErrUpOneSigma.Reset("ICES");
        muErrDownOneSigma.Reset("ICES");

        getSignalStrenght(file,muValue, muErrUpOneSigma,muErrDownOneSigma)
     
        if muErrDownOneSigma.GetMean() == 0 :
            ybins_mu_err = muErrUpOneSigma.GetMean();
        else:
            ybins_mu_err = (muErrUpOneSigma.GetMean()+muErrDownOneSigma.GetMean())/2;

            
    mu_err = ROOT.TF1("mu_err","[0]",0,max(len(filelist_add),len(filelist_remove)));
    mu_err.SetParameter(0,ybins_mu_err);

    for file in filelist_noSys : ## take the central value
        
        muValue.Reset("ICES");
        muErrUpOneSigma.Reset("ICES");
        muErrDownOneSigma.Reset("ICES");

        getSignalStrenght(file,muValue, muErrUpOneSigma,muErrDownOneSigma)
     
        if muErrDownOneSigma.GetMean() == 0 :
            ybins_mu_err_noSys = muErrUpOneSigma.GetMean();
        else:
            ybins_mu_err_noSys = (muErrUpOneSigma.GetMean()+muErrDownOneSigma.GetMean())/2;

            
    mu_err_noSys = ROOT.TF1("mu_err_noSys","[0]",0,max(len(filelist_add),len(filelist_remove)));
    mu_err_noSys.SetParameter(0,ybins_mu_err_noSys);

    can = ROOT.TCanvas("can","can",850,650);
    frame = can.DrawFrame(0,ybins_mu_err_noSys*0.5,max(len(filelist_add),len(filelist_remove)),ybins_mu_err*1.3);
    frame.SetBins(len(filelist_add),0,len(filelist_add));

    for ibin in range(frame.GetNbinsX()):
        frame.GetXaxis().SetBinLabel(ibin+1,labelList[ibin]);

    for ibin in range(histo_add.GetNbinsX()):
        if histo_add.GetBinContent(ibin+1) > ybins_mu_err :
            histo_add.SetBinContent(ibin+1,ybins_mu_err);
        if histo_add.GetBinContent(ibin+1) < ybins_mu_err_noSys :
            histo_add.SetBinContent(ibin+1,ybins_mu_err_noSys);

    for ibin in range(histo_remove.GetNbinsX()):
        if histo_remove.GetBinContent(ibin+1) > ybins_mu_err :
            histo_remove.SetBinContent(ibin+1,ybins_mu_err);
        if histo_remove.GetBinContent(ibin+1) < ybins_mu_err_noSys :
            histo_remove.SetBinContent(ibin+1,ybins_mu_err_noSys);


    frame.GetYaxis().SetTitle("uncertainty on WW_{ewk} cross section");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);

    frame.GetXaxis().LabelsOption ("v")
    frame.GetXaxis().SetLabelSize(0.035);

    histo_add.SetMarkerColor(ROOT.kBlack);
    histo_add.SetMarkerSize(1.25);
    histo_add.SetMarkerStyle(20);
    histo_add.Draw("Psame");
    
    histo_remove.SetMarkerColor(ROOT.kBlue);
    histo_remove.SetMarkerSize(1.5);
    histo_remove.SetMarkerStyle(24);
    histo_remove.Draw("Psame");

    
    mu_err_noSys.Draw("lsame");

    mu_err.SetLineStyle(7);
    mu_err.Draw("lsame");

    tex2.Draw("same");
    tex3.Draw("same");

    can.RedrawAxis();

    can.SaveAs(options.outputPlotDIR+"/uncertainty_vs_Nuisance.png","png");
    can.SaveAs(options.outputPlotDIR+"/uncertainty_vs_Nuisance.pdf","pdf");



##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## fix the chdir
    os.chdir(options.fileDIR);

    ## create output plot directory
    os.system("mkdir -p "+options.outputPlotDIR);

    setStyle();

    ## create the file list    
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" | grep _add_  | grep -v stat_shape  > list_add.txt");
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" | grep _remove_ | grep -v stat_shape  > list_remove.txt");
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" | grep noSys  | grep -v stat_shape  > list_noSys.txt");
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" | grep -v noSys | grep -v _remove_ | grep -v _add_ | grep -v stat_shape   > list.txt");

    ## make the list of files
    file = open("list_add.txt",'r')
    filelist_add = [];
    for line in file :
        filelist_add.append(line.split(" ")[0].replace("\n",""));

    file = open("list_remove.txt",'r')
    filelist_remove = [];
    for line in file :
        filelist_remove.append(line.split(" ")[0].replace("\n",""));

    file = open("list.txt",'r')
    filelist = [];
    for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));

    file = open("list_noSys.txt",'r')
    filelist_noSys = [];
    for line in file :
        filelist_noSys.append(line.split(" ")[0].replace("\n",""));

    if len(filelist) != 1 or len(filelist_noSys) !=1 :
        exit("no file for limit no Sys or complete")

    if options.makeAsymptoticLimitPlot :
        makeAsymptoticLimitPlot(filelist,filelist_noSys,filelist_add,filelist_remove);
    elif options.makeProfileLikelihoodPlot :
        makeProfileLikelihoodPlot(filelist,filelist_noSys,filelist_add,filelist_remove);
    elif options.makeMaxLikelihoodFitPlot :
        makeMaxLikelihoodFitPlot(filelist,filelist_noSys,filelist_add,filelist_remove);
    elif options.makeUncertaintyPlot:
        makeUncertaintyPlot(filelist,filelist_noSys,filelist_add,filelist_remove);

    os.system("rm list.txt");    
    os.system("rm list_noSys.txt");    
    os.system("rm list_add.txt");    
    os.system("rm list_remove.txt");    
 
