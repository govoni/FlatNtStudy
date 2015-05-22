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

parser.add_option('--fileDIR',   action="store", type="string", dest="fileDIR", default="")
parser.add_option('--channel',   action="store", type="string", dest="channel",     default="UUpp")
parser.add_option('--outputPlotDIR',   action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariable',   action="store", type="string", dest="inputVariable", default="")


parser.add_option('--nbin',   action="store", type="int", dest="nbin", default=500)
parser.add_option('--qmin',   action="store", type="float", dest="qmin", default=-100)
parser.add_option('--qmax',   action="store", type="float", dest="qmax", default=100)


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

    tex3 = ROOT.TLatex(0.67,0.96,"14 TeV, 3000 fb^{-1}");
    tex3.SetNDC(1);
    tex3.SetTextAlign(11);
    tex3.SetTextFont(42);
    tex3.SetTextSize(0.04);
    tex3.SetLineWidth(2);

    ## fix the chdir                                                                                                                                                           
    os.chdir(options.fileDIR);

    ## create output plot directory                                                                                                                                             
    os.system("mkdir -p "+options.outputPlotDIR);

    ## create the file list                                                                                                                                                     
    os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

    ## make the list of files                                                                                                                                                   
    file = open("list.txt",'r')
    filelist = [];

    nullHypoHist = ROOT.TH1F("nullHypoHist","",options.nbin,options.qmin,options.qmax);
    alteHypoHist = ROOT.TH1F("alteHypoHist","",options.nbin,options.qmin,options.qmax);
    nullHypoHist.Sumw2();
    alteHypoHist.Sumw2();

    for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));


    for fileName in filelist :

        fileOut = ROOT.TFile(fileName,"READ");
        fileOut.cd();
        toyDir = fileOut.GetDirectory("toys");
        if toyDir == 0 :
           print "Error in file ",fileName,": directory /toys not found";
           continue;
        
        for key in toyDir.GetListOfKeys() :
            if ROOT.TString(key.GetClassName()).Contains("RooStats") and ROOT.TString(key.GetClassName()).Contains("HypoTestResult") :
              hypoTestToy = toyDir.Get(key.GetName());
              if hypoTestToy == 0:
                continue;

              bDistribution = hypoTestToy.GetNullDistribution();
              sDistribution = hypoTestToy.GetAltDistribution();
              
              bdist   = bDistribution.GetSamplingDistribution();
              bweight = bDistribution.GetSampleWeights();

              if len(bdist) != len(bweight):
                print "Error in null hypo distribution : weight and values have not the same size .. skip file"
                continue;

              for index in range(len(bdist)):
                nullHypoHist.Fill(-2*bdist[index],bweight[index]);

              sdist   = sDistribution.GetSamplingDistribution();
              sweight = sDistribution.GetSampleWeights();

              if len(sdist) != len(sweight):
                print "Error in alternative hypo distribution : weight and values have not the same size .. skip file"
                continue;

              for index in range(len(sdist)):
                alteHypoHist.Fill(-2*sdist[index],sweight[index]);
              
              data =  hypoTestToy.GetTestStatisticData();


    #################
    can = ROOT.TCanvas("can","can",600,600);
       
    legend = ROOT.TLegend(0.45,0.25,0.85,0.45);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetTextSize(0.031);

    nullHypoHist.Scale(1./nullHypoHist.Integral());
    alteHypoHist.Scale(1./alteHypoHist.Integral());

    frame = can.DrawFrame(nullHypoHist.GetXaxis().GetXmin(),0.,nullHypoHist.GetXaxis().GetXmax(),
                          max(nullHypoHist.GetMaximum(),alteHypoHist.GetMaximum())*1.5);

    
    frame.GetXaxis().SetTitle("-2 x Ln(L_{noH}/L_{H})");
    frame.GetXaxis().SetTitleSize(0.045);
    frame.GetXaxis().SetLabelSize(0.04);

    frame.GetYaxis().SetTitle("Probability Density");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);

    nullHypoHist.SetFillStyle(3001)
    nullHypoHist.SetFillColor(801);
    nullHypoHist.SetLineColor(ROOT.kRed);
    nullHypoHist.SetLineWidth(2);
    nullHypoHist.SetLineStyle(7);

    alteHypoHist.SetFillStyle(3001)
    alteHypoHist.SetFillColor(861);
    alteHypoHist.SetLineColor(ROOT.kBlue);
    alteHypoHist.SetLineWidth(2);

    nullHypoHist.Draw("hist same");
    alteHypoHist.Draw("hist same");
    
    legend.AddEntry(nullHypoHist,"WW_{ewk} with H","fl")
    legend.AddEntry(alteHypoHist,"WW_{ewk} no H","fl")
    legend.Draw("same");

    tex.Draw();
    tex2.Draw();

    os.system("mkdir -p "+options.outputPlotDIR);
    os.system("rm "+options.outputPlotDIR+"/*");

    can.RedrawAxis();

    can.SaveAs(options.outputPlotDIR+"/hyposeperation.png","png");
    can.SaveAs(options.outputPlotDIR+"/hyposeperation.pdf","pdf");
    can.SaveAs(options.outputPlotDIR+"/hyposeperation.root","root");
