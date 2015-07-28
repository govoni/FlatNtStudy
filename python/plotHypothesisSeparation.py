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
parser.add_option('--fullCoverage', action='store_true', dest='fullCoverage', default=False, help='use full coverage')

parser.add_option('--fileDIR',         action="store", type="string", dest="fileDIR", default="")
parser.add_option('--channel',         action="store", type="string", dest="channel",     default="UUpp")
parser.add_option('--outputPlotDIR',   action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariable',   action="store", type="string", dest="inputVariable", default="")

 
parser.add_option('--nbin',    action="store", type="int", dest="nbin",       default=500)
parser.add_option('--qmin',    action="store", type="float", dest="qmin",     default=-100)
parser.add_option('--qmax',    action="store", type="float", dest="qmax",     default=100)
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

    ## make distribution plots
    nullHypoHist = ROOT.TH1F("nullHypoHist","",options.nbin*50,options.qmin,options.qmax);
    alteHypoHist = ROOT.TH1F("alteHypoHist","",options.nbin*50,options.qmin,options.qmax);
    expectedCLs  = ROOT.TH1F("expectedCLs","",options.nbin*50,0,1);   
    nullHypoHist.Sumw2();
    alteHypoHist.Sumw2();

    # store test statistics in array
    nullHypo_array    = [];
    alteHypo_array    = [];
    expectedCLs_array = [];

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
          expectedCLs_array.append(tree.limit);

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
                nullHypo_array.append(-2*bdist[index]*bweight[index]);
                nullHypoHist.Fill(-2*bdist[index],bweight[index]);

              sdist   = sDistribution.GetSamplingDistribution();
              sweight = sDistribution.GetSampleWeights();

              if len(sdist) != len(sweight):
                print "Error in alternative hypo distribution : weight and values have not the same size .. skip file"
                continue;

              for index in range(len(sdist)):
                alteHypo_array.append(-2*sdist[index]*sweight[index]);
                alteHypoHist.Fill(-2*sdist[index],sweight[index]);
              
              data =  hypoTestToy.GetTestStatisticData();


    ################# sorting vectors
    expectedCLs_array.sort();
    nullHypo_array.sort();
    alteHypo_array.sort();

    ##### store median and sigma band values
    medianH0 = nullHypo_array[int(len(nullHypo_array)/2)]
    qH068p   = nullHypo_array[int(math.floor(len(nullHypo_array) * 0.5*(1-0.683)+0.5))];
    qH095p   = nullHypo_array[int(math.floor(len(nullHypo_array) * 0.5*(1-0.954)+0.5))];
    qH099p   = nullHypo_array[int(math.floor(len(nullHypo_array) * 0.5*(1-0.997)+0.5))];
    qH068m   = nullHypo_array[min(int(math.floor(len(nullHypo_array) * 0.5*(1+0.683)+0.5)),len(nullHypo_array)-1)];
    qH095m   = nullHypo_array[min(int(math.floor(len(nullHypo_array) * 0.5*(1+0.954)+0.5)),len(nullHypo_array)-1)];
    qH099m   = nullHypo_array[min(int(math.floor(len(nullHypo_array) * 0.5*(1+0.997)+0.5)),len(nullHypo_array)-1)];


    medianH1 = alteHypo_array[int(len(alteHypo_array)/2)]
    qH168p   = alteHypo_array[int(math.floor(len(alteHypo_array) * 0.5*(1-0.683)+0.5))];
    qH195p   = alteHypo_array[int(math.floor(len(alteHypo_array) * 0.5*(1-0.954)+0.5))];
    qH199p   = alteHypo_array[int(math.floor(len(alteHypo_array) * 0.5*(1-0.997)+0.5))];
    qH168m   = alteHypo_array[min(int(math.floor(len(alteHypo_array) * 0.5*(1+0.683)+0.5)),len(alteHypo_array)-1)];
    qH195m   = alteHypo_array[min(int(math.floor(len(alteHypo_array) * 0.5*(1+0.954)+0.5)),len(alteHypo_array)-1)];
    qH199m   = alteHypo_array[min(int(math.floor(len(alteHypo_array) * 0.5*(1+0.997)+0.5)),len(alteHypo_array)-1)];


    ### print some  infor
    print "generated toy for           H0 ",len(nullHypo_array)," for H1 ",len(alteHypo_array);
    print "Mean of test statistics :   H0 ",nullHypoHist.GetMean()," H1 : ",alteHypoHist.GetMean()
    print "RMS of test statistics :    H0 ",nullHypoHist.GetRMS()," H1 : ",alteHypoHist.GetRMS()
    print "Median of test statistics : H0 ",medianH0," H1 : ",medianH1


    #### calculate the sigma separation
    stepSize = 0.05;
    coverage = 0.0;
    diff     = 1000.0;
    cut      = alteHypo_array[0]-stepSize;
    crosspoint = -99.0;
    startH0 = len(nullHypo_array)-1; 
    startH1 = 0;
    print "Starting to loop with cut at ",cut;

    if options.fullCoverage :

      while cut <= nullHypo_array[len(nullHypo_array)-1]+stepSize :
        cutH0 =-1.0;
        cutH1 =-1.0;
        
        for iH0 in range(startH0,-1,-1):
          if nullHypo_array[iH0] < cut:
            cutH0 = len(nullHypo_array)-iH0;
            break;

        for iH1 in range(startH1,len(alteHypo_array)):
          if alteHypo_array[iH1] > cut :
            cutH1=iH1;
            break;
      
        if cutH0 >= 0 and  cutH1 >=0:
          fracH0 = float((len(nullHypo_array)-cutH0))/len(nullHypo_array);
          fracH1 = float((len(alteHypo_array)-cutH1))/len(alteHypo_array);
          if math.fabs(fracH0-fracH1) < diff :            
            diff       = math.fabs(fracH0-fracH1);
            coverage   = math.fabs(fracH0+fracH1)/2.0;
            crosspoint = cut;
 
      cut = cut+stepSize;
      

      print "Finished loop on vector elements, min diff is ",diff,", looped until cut_fin=",cut;
      print "q value where H0 and H1 distributions have same area on opposite sides: ",crosspoint,"  Coverage=",coverage;
      separation = 2*ROOT.Math.normal_quantile_c(1.0-coverage, 1.0);
      print "Separation from tail prob: ",math.fabs(separation);
    
    ## calculate significance 
    integralH0 = nullHypoHist.Integral();
    integralH1 = alteHypoHist.Integral();

    tailH0    = nullHypoHist.Integral(1,nullHypoHist.FindBin(medianH1))/integralH0;
    tailH1    = alteHypoHist.Integral(alteHypoHist.FindBin(medianH0),alteHypoHist.GetNbinsX())/integralH1;
    sigExpH0  = ROOT.Math.normal_quantile_c(tailH0,1.0);
    sigExpH1  = ROOT.Math.normal_quantile_c(tailH1,1.0);
    print "Median point prob H0: ",tailH0,"  (",ROOT.Math.normal_quantile_c(tailH0,1.0), " sigma)";
    print "Median point prob H1: ",tailH1,"  (",ROOT.Math.normal_quantile_c(tailH1,1.0), " sigma)";
    print "Expected CLs ",expectedCLs.GetMean(),"  (",ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0)," sigma)";

    #diff     = 10.0;
    #coverage = 0.0;

    #for iBin in range(nullHypoHist.GetNbinsX()):

      #fracH0 = nullHypoHist.Integral(1,iBin)/integralH0;
      #fracH1 = alteHypoHist.Integral(iBin,alteHypoHist.GetNbinsX())/integralH1; 
      #if math.fabs(fracH0-fracH1) < diff :
      # diff     = math.fabs(fracH0-fracH1);
      # coverage = (fracH0+fracH1)/2.0;
    #sepH = 2*ROOT.Math.normal_quantile_c(1.0 - coverage, 1.0);
    #print "Separation from histograms = ",sepH," with coverage ",coverage, " with diff ",diff;



    ### plotting test statistics distribution
    can = ROOT.TCanvas("can","can",600,600);
       
    legend = ROOT.TLegend(0.25,0.75,0.85,0.85);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetTextSize(0.031);
    legend.SetNColumns(2);

    ## rebin histograms
    nullHypoHist.Rebin(50);
    alteHypoHist.Rebin(50);
    expectedCLs.Rebin(50);

    ## scale in order to produce probablities
    nullHypoHist.Scale(1./nullHypoHist.Integral());
    alteHypoHist.Scale(1./alteHypoHist.Integral());

    ## draw the framw with the right y-axis scale
    frame = can.DrawFrame(nullHypoHist.GetXaxis().GetXmin(),0.,nullHypoHist.GetXaxis().GetXmax(),
                          max(nullHypoHist.GetMaximum(),alteHypoHist.GetMaximum())*1.6);


    ## set x-axis label  
    if options.coupling == 0 :
      frame.GetXaxis().SetTitle("-2 x LQ(L_{H,c^{2}_{v}=0}/L_{H,c^{2}_{v}=1})");
    else :
      frame.GetXaxis().SetTitle("-2 x Ln(L_{H,c^{2}_{v}=%0.1f}/L_{H,c^{2}_{v}=0})"%options.coupling);
      
    ## frame style  
    frame.GetXaxis().SetTitleSize(0.040);
    frame.GetXaxis().SetLabelSize(0.030);

    frame.GetYaxis().SetTitle("Probability Density");
    frame.GetYaxis().SetTitleSize(0.040);
    frame.GetYaxis().SetTitleOffset(1.25);
    frame.GetYaxis().SetLabelSize(0.030);

    ## histo style
    nullHypoHist.SetFillStyle(3001)
    nullHypoHist.SetFillColor(801);
    nullHypoHist.SetLineColor(ROOT.kRed);
    nullHypoHist.SetLineWidth(2);
    nullHypoHist.SetLineStyle(7);

    alteHypoHist.SetFillStyle(3001)
    alteHypoHist.SetFillColor(861);
    alteHypoHist.SetLineColor(ROOT.kBlue);
    alteHypoHist.SetLineWidth(2);

    ## draw on the PAD
    nullHypoHist.Draw("hist same");
    alteHypoHist.Draw("hist same");
    
    ## legend
    legend.AddEntry(nullHypoHist,  "WW_{ewk} H, c_{v}=1","f")
    if options.coupling == 0:
      legend.AddEntry(alteHypoHist,"WW_{ewk} H, c_{v}=0","f")
    else:
      legend.AddEntry(alteHypoHist,"WW_{ewk} H, c_{v}=%0.1f"%(options.coupling),"f")
    legend.Draw("same");


    ## draw the median line for H0
    medianLineNull = ROOT.TLine(medianH0,0,medianH0,nullHypoHist.GetMaximum());
    medianLineNull.SetLineWidth(2);
    medianLineNull.SetLineStyle(7);
    medianLineNull.Draw("same")

    ## Draw Banner
    tex3.Draw();
    tex2.Draw();

    if options.fullCoverage :
      tex = ROOT.TLatex(0.25,0.7,"CLs^{exp} = %0.2f #rightarrow %0.2f #sigma"%(expectedCLs.GetMean(),math.fabs(separation)));
    else:
      tex = ROOT.TLatex(0.25,0.7,"CLs^{exp} = %0.2f #rightarrow %0.2f #sigma"%(expectedCLs.GetMean(),math.fabs(ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0))));
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
