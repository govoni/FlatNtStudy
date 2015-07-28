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
parser.add_option('--channel',         action="store", type="string", dest="channel",     default="UUpp")
parser.add_option('--outputPlotDIR',   action="store", type="string", dest="outputPlotDIR", default="")
parser.add_option('--inputVariable',   action="store", type="string", dest="inputVariable", default="")

(options, args) = parser.parse_args()


inputDirectoryPhaseI  = ['output/DataCards_WW_SS_HypothesisSeparation_0p9_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p8_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p7_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p6_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p5_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_noH_Dynamic_PhaseI/Card2D/datacardSeparation/computeHypoSeparation/']

inputDirectoryAged    = ['output/DataCards_WW_SS_HypothesisSeparation_0p9_Dynamic_Aged/AgedCards/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p8_Dynamic_Aged/AgedCards/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p7_Dynamic_Aged/AgedCards/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p6_Dynamic_Aged/AgedCards/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p5_Dynamic_Aged/AgedCards/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_noH_Dynamic_Aged/AgedCards/computeHypoSeparation/']

inputDirectoryPhaseII = ['output/DataCards_WW_SS_HypothesisSeparation_0p9_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p8_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p7_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p6_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_0p5_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/',
                         'output/DataCards_WW_SS_HypothesisSeparation_noH_Dynamic_PhaseII/Card2D/datacardSeparation/computeHypoSeparation/']
 
qmin     = [-20,-30,-50,-50,-50,-200]
qmax     = [20,30,50,50,50,200]
nbin     = [75,100,150,150,150,200]
coupling = [0.1,0.2,0.3,0.4,0.5,1]

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

#######################################                                                                                                                                     
###### build the test statistics ######                                                                                                                                         
#######################################                                                                                                                                     

def calculatedTestStatisticsDistributions(filelist, 
                                          nullHypoHist, alteHypoHist, expectedCLs,
                                          nullHypo_array, alteHypo_array, expectedCLs_array):
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

#################################################                                                                                                                              
###### plot single separation distribution ######                                                                                                                              
#################################################                                                                                                                            

def plotHypotesisDistributions(nullHypoHist,alteHypoHist,expectedCLs, coupling, postfix):

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
  if coupling == 1 :
    frame.GetXaxis().SetTitle("-2 x LQ(L_{H,c^{2}_{v}=0}/L_{H,c^{2}_{v}=1})");
  else :
    frame.GetXaxis().SetTitle("-2 x Ln(L_{H,c^{2}_{v}=%0.1f}/L_{H,c^{2}_{v}=0})"%coupling);
      
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
  legend.AddEntry(nullHypoHist,  "WW_{ewk} H, c^{2}_{v}=1","f")
  if coupling == 1:
    legend.AddEntry(alteHypoHist,"WW_{ewk} H, c^{2}_{v}=0","f")
  else:
    legend.AddEntry(alteHypoHist,"WW_{ewk} H, c^{2}_{v}=%0.1f"%(coupling),"f")
    legend.Draw("same");


  ## draw the median line for H0
  medianLineNull = ROOT.TLine(medianH0,0,medianH0,nullHypoHist.GetMaximum());
  medianLineNull.SetLineWidth(2);
  medianLineNull.SetLineStyle(7);
  medianLineNull.Draw("same")

  ## Draw Banner
  tex3.Draw();
  tex2.Draw();

  tex = ROOT.TLatex(0.25,0.7,"CLs^{exp} = %0.2f #rightarrow %0.2f #sigma"%(expectedCLs.GetMean(),math.fabs(ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0))));
  tex.SetNDC(1);
  tex.SetTextAlign(11);
  tex.SetTextFont(42);
  tex.SetTextSize(0.04);
  tex.SetLineWidth(2);
  tex.Draw();

  can.RedrawAxis();

  can.SaveAs(options.outputPlotDIR+"/hyposeperation_c%0.1f_%s.png"%(coupling,postfix),"png");
  can.SaveAs(options.outputPlotDIR+"/hyposeperation_c%0.1f_%s.pdf"%(coupling,postfix),"pdf");


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

    ## create output plot directory                                                                                                                                           
    os.system("mkdir -p "+options.outputPlotDIR);
    os.system("rm -r  "+options.outputPlotDIR+"/*");

    pwd = os.getcwd()


    ### final plots
    phaseI_CLs    = ROOT.TGraphAsymmErrors(len(coupling))
    phaseII_CLs   = ROOT.TGraphAsymmErrors(len(coupling))
    phaseAged_CLs = ROOT.TGraphAsymmErrors(len(coupling))

    phaseI_Sepa    = ROOT.TGraphAsymmErrors(len(coupling))
    phaseII_Sepa   = ROOT.TGraphAsymmErrors(len(coupling))
    phaseAged_Sepa = ROOT.TGraphAsymmErrors(len(coupling))


    #################
    ## PhaseI plot ##
    #################
    
    ## start with phaseI
    for idir in range(len(inputDirectoryPhaseI)):

      ## fix the chdir
      os.chdir(pwd);
      os.chdir(inputDirectoryPhaseI[idir]);

      ## create the file list                                                                                                                                            
      os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

      ## make the list of files                                                                                                                                              
      file = open("list.txt",'r')
      filelist = [];

      ## make distribution plots
      nullHypoHist = ROOT.TH1F("nullHypoHist_%f_PhaseI"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      alteHypoHist = ROOT.TH1F("alteHypoHist_%f_PhaseI"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      expectedCLs  = ROOT.TH1F("expectedCLst_%f_PhaseI"%coupling[idir],"",nbin[idir]*50,0,1);   
      nullHypoHist.Sumw2();
      alteHypoHist.Sumw2();

      # store test statistics in array
      nullHypo_array    = [];
      alteHypo_array    = [];
      expectedCLs_array = [];

      for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));


      calculatedTestStatisticsDistributions(filelist,
                                            nullHypoHist, alteHypoHist, expectedCLs,
                                            nullHypo_array,alteHypo_array,expectedCLs_array);

              
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


      ### plotting test statistics distribution
      os.chdir(pwd);

      phaseI_CLs.SetPoint(idir,coupling[idir],expectedCLs.GetMean())

      separation = ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0);

      if numpy.isnan(separation) or numpy.isinf(separation):
        phaseI_Sepa.SetPoint(idir,coupling[idir],12.)
      else:
        phaseI_Sepa.SetPoint(idir,coupling[idir],ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0))


      plotHypotesisDistributions(nullHypoHist,alteHypoHist,expectedCLs, coupling[idir],"PhaseI");


    #####################
    ## Phase Aged plot ##
    #####################

    ## start with phaseI
    for idir in range(len(inputDirectoryAged)):

      ## fix the chdir
      os.chdir(pwd);
      os.chdir(inputDirectoryAged[idir]);

      ## create the file list                                                                                                                                            
      os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

      ## make the list of files                                                                                                                                              
      file = open("list.txt",'r')
      filelist = [];

      ## make distribution plots
      nullHypoHist = ROOT.TH1F("nullHypoHist_%f_Aged"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      alteHypoHist = ROOT.TH1F("alteHypoHist_%f_Aged"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      expectedCLs  = ROOT.TH1F("expectedCLst_%f_Aged"%coupling[idir],"",nbin[idir]*50,0,1);   
      nullHypoHist.Sumw2();
      alteHypoHist.Sumw2();

      # store test statistics in array
      nullHypo_array    = [];
      alteHypo_array    = [];
      expectedCLs_array = [];

      for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));


      calculatedTestStatisticsDistributions(filelist,
                                            nullHypoHist, alteHypoHist, expectedCLs,
                                            nullHypo_array,alteHypo_array,expectedCLs_array);

              
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

      phaseAged_CLs.SetPoint(idir,coupling[idir],expectedCLs.GetMean())

      separation = ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0);

      if numpy.isnan(separation) or numpy.isinf(separation):
        phaseAged_Sepa.SetPoint(idir,coupling[idir],10.13);
      else:
        phaseAged_Sepa.SetPoint(idir,coupling[idir],ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0))

      ### plotting test statistics distribution
      os.chdir(pwd);
      plotHypotesisDistributions(nullHypoHist,alteHypoHist,expectedCLs, coupling[idir],"Aged");


    ########################
    ## Phase PhaseII plot ##
    ########################
    '''
    ## start with phaseI
    for idir in range(len(inputDirectoryPhaseII)):

      ## fix the chdir
      os.chdir(pwd);
      os.chdir(inputDirectoryPhaseII[idir]);

      ## create the file list                                                                                                                                            
      os.system("ls | grep root | grep higgsCombine | grep "+options.channel+" > list.txt");

      ## make the list of files                                                                                                                                              
      file = open("list.txt",'r')
      filelist = [];

      ## make distribution plots
      nullHypoHist = ROOT.TH1F("nullHypoHist_%f_PhaseII"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      alteHypoHist = ROOT.TH1F("alteHypoHist_%f_PhaseII"%coupling[idir],"",nbin[idir]*50,qmin[idir],qmax[idir]);
      expectedCLs  = ROOT.TH1F("expectedCLst_%f_PhaseII"%coupling[idir],"",nbin[idir]*50,0,1);   
      nullHypoHist.Sumw2();
      alteHypoHist.Sumw2();

      # store test statistics in array
      nullHypo_array    = [];
      alteHypo_array    = [];
      expectedCLs_array = [];

      for line in file :
        filelist.append(line.split(" ")[0].replace("\n",""));

      calculatedTestStatisticsDistributions(filelist,
                                            nullHypoHist, alteHypoHist, expectedCLs,
                                            nullHypo_array,alteHypo_array,expectedCLs_array);

              
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

      phaseII_CLs.SetPoint(idir,coupling[idir],expectedCLs.GetMean())
      separation = ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0);
      if numpy.isnan(separation) or numpy.isinf(separation):
        phaseII_Sepa.SetPoint(idir,coupling[idir],12.55);
      else:
        phaseII_Sepa.SetPoint(idir,coupling[idir],ROOT.Math.normal_quantile_c(expectedCLs.GetMean()/2,1.0))

      ### plotting test statistics distribution
      os.chdir(pwd);
      plotHypotesisDistributions(nullHypoHist,alteHypoHist,expectedCLs, coupling[idir],"PhaseII");


    '''
    ##### make the final plot
    can = ROOT.TCanvas("can","can",600,600);

    legend = ROOT.TLegend(0.45,0.55,0.85,0.75);
    legend.SetFillColor(0);
    legend.SetFillStyle(0);
    legend.SetBorderSize(0);
    legend.SetTextSize(0.031);

    frame = can.DrawFrame(phaseI_CLs.GetXaxis().GetXmin(),0.,phaseI_CLs.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(phaseI_CLs.GetN(),phaseI_CLs.GetY()),
                          max(ROOT.TMath.MaxElement(phaseII_CLs.GetN(),phaseII_CLs.GetY()),
                              ROOT.TMath.MaxElement(phaseAged_CLs.GetN(),phaseAged_CLs.GetY())))*1.2);

      
    frame.SetNdivisions(510);

    frame.GetXaxis().SetTitle("#Deltac^{2}_{H}");
    frame.GetXaxis().SetTitleSize(0.045);
    frame.GetXaxis().SetLabelSize(0.04);

    frame.GetYaxis().SetTitle("Expected CLs");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);

    phaseI_CLs.SetMarkerSize(1.4); 
    phaseI_CLs.SetMarkerStyle(20); 
    phaseI_CLs.SetLineWidth(2);
    phaseI_CLs.Draw("pl");
    '''
    phaseII_CLs.SetMarkerSize(1.1);
    phaseII_CLs.SetLineStyle(1);
    phaseII_CLs.SetLineColor(ROOT.kRed);
    phaseII_CLs.SetMarkerColor(ROOT.kRed);
    phaseII_CLs.Draw("plsame"); 
    '''
    phaseAged_CLs.SetMarkerSize(1.4);
    phaseAged_CLs.SetMarkerStyle(20);
    phaseAged_CLs.SetLineStyle(1);
    phaseAged_CLs.SetLineWidth(2);
    phaseAged_CLs.SetLineColor(ROOT.kBlue);
    phaseAged_CLs.SetMarkerColor(ROOT.kBlue);
    phaseAged_CLs.Draw("plsame");

    tex2.Draw();
    tex3.Draw();

    legend.AddEntry(phaseI_CLs,    "Phase I 50 PU","pl");
    legend.AddEntry(phaseAged_CLs, "Phase I aged 140 PU","pl");
#    legend.AddEntry(phaseII_CLs,   "Phase II 140 PU","pl");
    legend.Draw();

    can.RedrawAxis();

    can.SaveAs(options.outputPlotDIR+"/hyposeperation_CLs.png","png");
    can.SaveAs(options.outputPlotDIR+"/hyposeperation_CLs.pdf","pdf");



    ##### make the final plot
    can2 = ROOT.TCanvas("can2","can2",600,600);

    legend2 = ROOT.TLegend(0.18,0.6,0.58,0.80);
    legend2.SetFillColor(0);
    legend2.SetFillStyle(0);
    legend2.SetBorderSize(0);
    legend2.SetTextSize(0.031);

    frame2 = can2.DrawFrame(phaseI_Sepa.GetXaxis().GetXmin(),0.,phaseI_Sepa.GetXaxis().GetXmax(),
                          max(ROOT.TMath.MaxElement(phaseI_Sepa.GetN(),phaseI_Sepa.GetY()),
                          max(ROOT.TMath.MaxElement(phaseII_Sepa.GetN(),phaseII_Sepa.GetY()),
                              ROOT.TMath.MaxElement(phaseAged_Sepa.GetN(),phaseAged_Sepa.GetY())))*1.2);

    frame2.SetNdivisions(510);

    frame2.GetXaxis().SetTitle("#Deltac^{2}_{H}");
    frame2.GetXaxis().SetTitleSize(0.045);
    frame2.GetXaxis().SetLabelSize(0.04);

    frame2.GetYaxis().SetTitle("Expected significance (#sigma)");
    frame2.GetYaxis().SetTitleSize(0.045);
    frame2.GetYaxis().SetTitleOffset(1.17);
    frame2.GetYaxis().SetLabelSize(0.04);

    phaseI_Sepa.SetMarkerSize(1.4);
    phaseI_Sepa.SetMarkerStyle(20);
    phaseI_Sepa.SetLineStyle(1);
    phaseI_Sepa.SetLineWidth(2);
    phaseI_Sepa.Draw("pl");
    '''
    phaseII_Sepa.SetMarkerSize(1.1);
    phaseII_Sepa.SetLineStyle(1);
    phaseII_Sepa.SetLineColor(ROOT.kRed);
    phaseII_Sepa.SetMarkerColor(ROOT.kRed);
    phaseII_Sepa.Draw("plsame");
    '''
    phaseAged_Sepa.SetMarkerStyle(20);
    phaseAged_Sepa.SetMarkerSize(1.4);
    phaseAged_Sepa.SetLineStyle(1);
    phaseAged_Sepa.SetLineWidth(2);
    phaseAged_Sepa.SetLineColor(ROOT.kBlue);
    phaseAged_Sepa.SetMarkerColor(ROOT.kBlue);
    phaseAged_Sepa.Draw("plsame");

    tex2.Draw();
    tex3.Draw();

    legend2.AddEntry(phaseI_Sepa,    "Phase I 50 PU","pl");
    legend2.AddEntry(phaseAged_Sepa, "Phase I aged 140 PU","pl");
#    legend2.AddEntry(phaseII_Sepa,   "Phase II 140 PU","pl");
    legend2.Draw();

    can2.RedrawAxis();

    can2.SaveAs(options.outputPlotDIR+"/hyposeperation_Sepa.png","png");
    can2.SaveAs(options.outputPlotDIR+"/hyposeperation_Sepa.pdf","pdf");



