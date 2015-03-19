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

#python ../../../python/plotPostFitDistributions.py --fileDIR ./  --channel EEmm --outputPlotDIR plotPostFitDistribution --inputVariable detajj --nToys 1 --expectSignal 1 -b

############################################
#            Job steering                  #
############################################

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

## parse files
parser.add_option('--fileDIR',             action="store", type="string", dest="fileDIR", default="")
parser.add_option('--channel',             action="store", type="string", dest="channel",     default="UU")
parser.add_option('--outputPlotDIR',       action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariable',       action="store", type="string", dest="inputVariable", default="")
parser.add_option('--nToys',               action="store", type=int,      dest="nToys", default=0)
parser.add_option('--expectSignal',        action="store", type=int,      dest="expectSignal", default=1)

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

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## fix the chdir
    os.chdir(options.fileDIR);

    ## create output plot directory
    os.system("mkdir -p "+options.outputPlotDIR);

    ## create the file list    
    os.system("ls | grep root | grep mlfit | grep "+options.channel+" | grep "+options.inputVariable+" > list.txt");

    setStyle();

    ## make the list of files
    file = open("list.txt",'r')
    filelist = [];

    for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));

    os.system("rm list.txt");    

    
    ## normalization histograms for more than one file
    prefitHistogram        = {};
    postfitHistogram       = {};

    ## loop on the variable list
    for file in filelist :

      ## open a file
      tfile = ROOT.TFile.Open(file,"READ");

      #################
      ## PREFIT PART ##
      #################

      ## take the input normalizations pre-fit
      tfile.cd("shapes_prefit");
      
      listOfKeys = ROOT.gDirectory.GetListOfKeys();

      channelName = "";

      for key in listOfKeys :
        if ROOT.TString(key.GetName()).Contains(options.channel) :
          ROOT.gDirectory.cd(key.GetName());
          channelName = key.GetName();
          listOfHistograms = ROOT.gDirectory.GetListOfKeys();
          for keyHisto in listOfHistograms :
            if ROOT.TString(keyHisto.GetName()).Contains("total"):
              continue;
            histo = ROOT.gDirectory.FindObjectAny(keyHisto.GetName());
            prefitHistogram[keyHisto.GetName()] = histo;

      for toy in range(options.nToys) :
        
        tfile2 = ROOT.TFile.Open(file,"READ");
        tfile2.cd("shapes_fit_s_%d/%s"%(toy,channelName));
        listOfKeys = ROOT.gDirectory.GetListOfKeys();
        for keyHisto in listOfKeys :
            if ROOT.TString(keyHisto.GetName()).Contains("total"):
                continue;
            histo = ROOT.gDirectory.FindObjectAny(keyHisto.GetName());
            if(histo != 0) :
             postfitHistogram[keyHisto.GetName()+"_"+str(toy)] = histo;
            else:
              continue;
        

    ## make a plot for each toy
    tex = ROOT.TLatex(0.892,0.957," 14 TeV");
    tex.SetNDC();
    tex.SetTextAlign(31);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw("same");

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

    can = ROOT.TCanvas("can","can",650,650);
              
    for toy in range(options.nToys) :
      for key, value in prefitHistogram.iteritems() :

        leg = ROOT.TLegend(0.73,0.71,0.86,0.92);
        leg.SetFillColor(0);
        leg.SetShadowColor(0);
        leg.SetTextFont(42);
        leg.SetTextSize(0.028);
        leg.SetBorderSize(0);
        leg.SetFillStyle(0);

        value.SetLineWidth(2);
        value.SetLineColor(ROOT.kBlue);
        value.GetXaxis().SetTitle(options.inputVariable);
        value.GetYaxis().SetTitle("entries");
        value.GetYaxis().SetTitleOffset(1.25);

        leg.AddEntry(value,"prefit","l");
        
        post = postfitHistogram[key+"_"+str(toy)];
        post.SetLineWidth(2);
        post.SetLineColor(ROOT.kRed);        
        post.SetMarkerColor(ROOT.kBlack);        

        prefitHistogram[key].GetYaxis().SetRangeUser(0,ROOT.TMath.Max(prefitHistogram[key].GetMaximum(),post.GetMaximum())*1.3);

        prefitHistogram[key].Draw("hist");

        tex.Draw("same");
        tex2.Draw("same");
        tex3.Draw("same");


        post.Draw("EPsame");

        leg.AddEntry(post,"postfit","pl");
        leg.Draw("same")

        can.SaveAs("%s/plot_%s.png"%(options.outputPlotDIR,key+"_"+options.channel+"_"+str(toy)),"png");
        can.SaveAs("%s/plot_%s.pdf"%(options.outputPlotDIR,key+"_"+options.channel+"_"+str(toy)),"png");

        
