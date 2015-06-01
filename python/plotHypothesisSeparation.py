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

 
parser.add_option('--nbin',    action="store", type="int", dest="nbin", default=500)
parser.add_option('--qmin',    action="store", type="float", dest="qmin", default=-100)
parser.add_option('--qmax',    action="store", type="float", dest="qmax", default=100)
parser.add_option('--coupling',action="store", type="float", dest="coupling", default=0)


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

#####################
#### inverse erf ####
#####################
def ErfInverse(y) : 
  kMaxit = 50; 
  kEps = 1e-14; 
  kConst = 0.8862269254527579; #// sqrt(pi)/2.0 

  if ROOT.TMath.Abs(y) <= kEps:
    return kConst*y; 

  if(ROOT.TMath.Abs(y) < 1.0) :
    erfi = kConst*ROOT.TMath.Abs(y); 
    Y0 = ROOT.TMath.Erf(0.9*erfi); 
    derfi = 0.1*erfi; 
    for iter in range(kMaxit):
      Y1 = 1. - ROOT.TMath.Erfc(erfi); 
      DY1 = ROOT.TMath.Abs(y) - Y1; 
      if (ROOT.TMath.Abs(DY1) < kEps) :
        if (y < 0) :
          return -erfi; 
        else:
          return erfi;
      DY0 = Y1 - Y0; 
      derfi *= DY1/DY0; 
      Y0 = Y1; 
      erfi += derfi; 
      if ROOT.TMath.Abs(derfi/erfi) < kEps :
        if y < 0 : 
          return -erfi; 
        else: 
          return erfi;

  return 0; 


##################################                                                                                                                                              
########### Main Code ############                                                                                                                                              
##################################                                                                                                                                             

if __name__ == '__main__':

    ## set the style                                                                                                                                                           
    setStyle();


    tex2 = TLatex(0.17,0.875,"CMS Delphes Simulation");
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
    expectedCLs  = ROOT.TH1F("expectedCLs","",options.nbin,0,1);   
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

        ##take the cls as the average one
        tree = fileOut.Get("limit");
        for iEntry in range(tree.GetEntries()):
          tree.GetEntry(iEntry);
          if tree.quantileExpected != 0.5:
            continue;
          expectedCLs.Fill(tree.limit);

        ##take the list of keys
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
       
    legend = ROOT.TLegend(0.25,0.75,0.85,0.85);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetTextSize(0.031);
    legend.SetNColumns(2);

    median_array = array("f",[]);

    for ibin in range(nullHypoHist.GetNbinsX()):
      for itimes in range(int(nullHypoHist.GetBinContent(ibin+1))):
        median_array.append(nullHypoHist.GetBinCenter(ibin+1));

    if len(median_array)%2 == 0:
      median = median_array[len(median_array)/2]
    else :
      median = median_array[len(median_array)/2-1]




    nullHypoHist.Scale(1./nullHypoHist.Integral());
    alteHypoHist.Scale(1./alteHypoHist.Integral());

    frame = can.DrawFrame(nullHypoHist.GetXaxis().GetXmin(),0.,nullHypoHist.GetXaxis().GetXmax(),
                          max(nullHypoHist.GetMaximum(),alteHypoHist.GetMaximum())*1.6);


    if options.coupling == 0 :
      frame.GetXaxis().SetTitle("-2 x LQ(L_{H,c_{v}=0}/L_{H,c_{v}=1})");
    else :
      frame.GetXaxis().SetTitle("-2 x Ln(L_{H,c_{v}=%0.1f}/L_{H,c_{v}=0})"%options.coupling);
      
    frame.GetXaxis().SetTitleSize(0.040);
    frame.GetXaxis().SetLabelSize(0.030);

    frame.GetYaxis().SetTitle("Probability Density");
    frame.GetYaxis().SetTitleSize(0.040);
    frame.GetYaxis().SetTitleOffset(1.25);
    frame.GetYaxis().SetLabelSize(0.030);

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
    
    legend.AddEntry(nullHypoHist,  "WW_{ewk} H, c_{v}=1","f")
    if options.coupling == 0:
      legend.AddEntry(alteHypoHist,"WW_{ewk} H, c_{v}=0","f")
    else:
      legend.AddEntry(alteHypoHist,"WW_{ewk} H, c_{v}=%0.1f"%(options.coupling),"f")
    legend.Draw("same");


    medianLineNull = ROOT.TLine(median,0,median,nullHypoHist.GetMaximum());
    medianLineNull.SetLineWidth(2);
    medianLineNull.SetLineStyle(7);
    medianLineNull.Draw("same")

    tex3.Draw();
    tex2.Draw();

    tex = ROOT.TLatex(0.25,0.7,"CLs^{exp} = %0.3f"%(expectedCLs.GetMean()));
    tex.SetNDC(1);
    tex.SetTextAlign(11);
    tex.SetTextFont(42);
    tex.SetTextSize(0.04);
    tex.SetLineWidth(2);
    tex.Draw();

    os.system("mkdir -p "+options.outputPlotDIR);
    os.system("rm "+options.outputPlotDIR+"/*");

    can.RedrawAxis();

    can.SaveAs(options.outputPlotDIR+"/hyposeperation_c%0.1f.png"%(options.coupling),"png");
    can.SaveAs(options.outputPlotDIR+"/hyposeperation_c%0.1f.pdf"%(options.coupling),"pdf");
    can.SaveAs(options.outputPlotDIR+"/hyposeperation_c%0.1f.root"%(options.coupling),"root");
