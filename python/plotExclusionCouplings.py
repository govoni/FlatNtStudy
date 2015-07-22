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
  gStyle.SetNumberContours(NCont)

####### compute CLs ####
def  compueCLsFromTestStatistics(filelist,
                                 medianCLs,
                                 CLs068p,CLs095p,CLs068m,CLs095m):


   nullHypo          = ROOT.TH1F("nullHypo","",100000,-500,500);
   alteHypo          = ROOT.TH1F("alteHypo","",100000,-500,500);
   expectedCLs_mean  = ROOT.TH1F("expectedCLs_mean","",10000,0,1);

   nullHypo_array = [];
   alteHypo_array = [];

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
          expectedCLs_mean.Fill(tree.limit);

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
                nullHypo.Fill(-2*bdist[index]*bweight[index]);
                nullHypo_array.append(-2*bdist[index]*bweight[index]);
  
              sdist   = sDistribution.GetSamplingDistribution();
              sweight = sDistribution.GetSampleWeights();

              if len(sdist) != len(sweight):
                print "Error in alternative hypo distribution : weight and values have not the same size .. skip file"
                continue;

              for index in range(len(sdist)):
                alteHypo.Fill(-2*sdist[index]*sweight[index]);
                alteHypo_array.append(-2*sdist[index]*sweight[index]);


   nullHypo_array.sort();
   alteHypo_array.sort();

    ##### store median and sigma band values                                                                                                                                    
   medianH0 = nullHypo_array[int(len(nullHypo_array)/2)]
   qH068p   = nullHypo_array[int(math.floor(len(nullHypo_array) * 0.5*(1-0.683)+0.5))];
   qH095p   = nullHypo_array[int(math.floor(len(nullHypo_array) * 0.5*(1-0.954)+0.5))];
   qH068m   = nullHypo_array[min(int(math.floor(len(nullHypo_array) * 0.5*(1+0.683)+0.5)),len(nullHypo_array)-1)];
   qH095m   = nullHypo_array[min(int(math.floor(len(nullHypo_array) * 0.5*(1+0.954)+0.5)),len(nullHypo_array)-1)];

   medianH1 = alteHypo_array[int(len(alteHypo_array)/2)]
   qH168p   = alteHypo_array[int(math.floor(len(alteHypo_array) * 0.5*(1-0.683)+0.5))];
   qH195p   = alteHypo_array[int(math.floor(len(alteHypo_array) * 0.5*(1-0.954)+0.5))];
   qH168m   = alteHypo_array[min(int(math.floor(len(alteHypo_array) * 0.5*(1+0.683)+0.5)),len(alteHypo_array)-1)];
   qH195m   = alteHypo_array[min(int(math.floor(len(alteHypo_array) * 0.5*(1+0.954)+0.5)),len(alteHypo_array)-1)];

   medianCLs.append( (alteHypo.Integral(alteHypo.FindBin(medianH0),alteHypo.GetNbinsX())/alteHypo.Integral())/(nullHypo.Integral(1,nullHypo.FindBin(medianH0))/nullHypo.Integral()));
   CLs068p  .append((alteHypo.Integral(alteHypo.FindBin(qH068p),alteHypo.GetNbinsX())/alteHypo.Integral())/(nullHypo.Integral(1,nullHypo.FindBin(qH068p))/nullHypo.Integral()));
   CLs068m  .append((alteHypo.Integral(alteHypo.FindBin(qH068m),alteHypo.GetNbinsX())/alteHypo.Integral())/(nullHypo.Integral(1,nullHypo.FindBin(qH068m))/nullHypo.Integral()));
   CLs095p  .append((alteHypo.Integral(alteHypo.FindBin(qH095p),alteHypo.GetNbinsX())/alteHypo.Integral())/(nullHypo.Integral(1,nullHypo.FindBin(qH095p))/nullHypo.Integral()));
   CLs095m  .append((alteHypo.Integral(alteHypo.FindBin(qH095m),alteHypo.GetNbinsX())/alteHypo.Integral())/(nullHypo.Integral(1,nullHypo.FindBin(qH095m))/nullHypo.Integral()));
    


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

    ## input file for phaseI 50PU, aged and phaseII for polarized signficance vs lumi
    fileAsymptotic_noHmH = [];
    fileAsymptotic_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p9_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeAsymptotic/higgsCombinefinal_R_mll_COMB_3000.Asymptotic.mH100.1214179857.root","READ"));
    fileAsymptotic_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p8_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeAsymptotic/higgsCombinefinal_R_mll_COMB_3000.Asymptotic.mH100.220703311.root","READ"));
    fileAsymptotic_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p7_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeAsymptotic/higgsCombinefinal_R_mll_COMB_3000.Asymptotic.mH100.-1543848339.root","READ"));
    fileAsymptotic_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeAsymptotic/higgsCombinefinal_R_mll_COMB_3000.Asymptotic.mH100.-1635732130.root","READ"));

    xbins      = array('f',[0.1,0.2,0.3,1.0]); 
    xbins_env  = array('f',[0.1,0.2,0.3,1.0,1.0,0.3,0.2,0.1]); 

    yMu1s   = array('f',[]);
    yMu     = array('f',[]);
    yMu2s   = array('f',[]);

    for file in fileAsymptotic_noHmH : 
      tree = file.Get("limit");
      for i in range(tree.GetEntries()):
        tree.GetEntry(i);
        t_quantileExpected = tree.quantileExpected;
        if t_quantileExpected == -1.:
          continue ;
        elif t_quantileExpected >= 0.024 and t_quantileExpected <= 0.026:
          yMu2s.append(tree.limit);
        elif t_quantileExpected >= 0.15 and t_quantileExpected <= 0.17:
          yMu1s.append(tree.limit);
        elif t_quantileExpected == 0.5:
          yMu.append(tree.limit);
        else: continue;

    for i in range( len(fileAsymptotic_noHmH)-1, -1, -1 ):
      tree = fileAsymptotic_noHmH[i].Get("limit");
      for i in range(tree.GetEntries()):
        tree.GetEntry(i);
        t_quantileExpected = tree.quantileExpected;
        if t_quantileExpected >= 0.83 and t_quantileExpected <= 0.85:
          yMu1s.append(tree.limit);
        elif t_quantileExpected >= 0.974 and t_quantileExpected <= 0.976:
          yMu2s.append(tree.limit);
        else: continue;
      

    curGraph_exp = ROOT.TGraphAsymmErrors(len(xbins),xbins,yMu);
    curGraph_1s  = ROOT.TGraphAsymmErrors(len(xbins_env),xbins_env,yMu1s);
    curGraph_2s  = ROOT.TGraphAsymmErrors(len(xbins_env),xbins_env,yMu2s);

    can = ROOT.TCanvas("can","can",600,600);
    
    frame = can.DrawFrame(0.1,0,1,ROOT.TMath.MaxElement(curGraph_2s.GetN(),curGraph_2s.GetY())*1.2);

    frame.GetXaxis().SetTitle("#Delta c^{H}_{V}");
    frame.GetXaxis().SetTitleSize(0.045);
    frame.GetXaxis().SetLabelSize(0.04);

    frame.GetYaxis().SetTitle("95% CL on #Delta_{SM} / (H(#Delta c^{H}_{V})-SM)");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);
    
    curGraph_exp.SetMarkerStyle(20);
    curGraph_exp.SetLineWidth(3);
    curGraph_exp.SetLineStyle(ROOT.kDashed);
    curGraph_exp.SetMarkerSize(1.6);
    curGraph_exp.SetMarkerSize(1.3);
    curGraph_exp.SetMarkerColor(ROOT.kBlack);

    curGraph_1s.SetFillColor(ROOT.kGreen);
    curGraph_1s.SetFillStyle(1001);
    curGraph_1s.SetLineWidth(0);
    curGraph_1s.SetLineStyle(0);

    curGraph_2s.SetFillColor(ROOT.kYellow);
    curGraph_2s.SetFillStyle(1001);
    curGraph_2s.SetLineWidth(0);
    curGraph_2s.SetLineStyle(0);

    oneLine = ROOT.TF1("oneLine","1",0,1);
    oneLine.SetLineColor(ROOT.kRed);
    oneLine.SetLineWidth(3);

    frame.GetYaxis().SetNdivisions(505);
    can.SetGridx(1);
    can.SetGridy(1);

    curGraph_2s.Draw("F");
    curGraph_1s.Draw("Fsame");
    curGraph_exp.Draw("PLsame");

    leg = ROOT.TLegend(0.4,0.6,0.83,0.80);
    leg.SetFillColor(0);
    leg.SetFillStyle(0);
    leg.SetShadowColor(0);
    leg.SetTextFont(42);
    leg.SetTextSize(0.028);

    leg.AddEntry(curGraph_exp,"Asympt. CL_{S} Expected","L")
    leg.AddEntry(curGraph_1s, "Asympt. CL_{S} Expected #pm 1#sigma","F")
    leg.AddEntry(curGraph_2s, "Asympt. CL_{S} Expected #pm 2#sigma","F")

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    leg.Draw();
    oneLine.Draw("same");

    tex2.Draw();
    tex3.Draw();


    os.system("mkdir -p output/outputHiggsCoupling/");
    os.system("rm output/outputHiggsCoupling/*");

    can.SaveAs("output/outputHiggsCoupling/AsymptoticLimit_noHmH.png");
    can.SaveAs("output/outputHiggsCoupling/AsymptoticLimit_noHmH.pdf");


    ## input file for phaseI 50PU, aged and phaseII for polarized signficance vs lumi
    fileSignificance_noHmH = [];
    fileSignificance_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p9_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeProfileLikelihood/higgsCombinefinal_R_mll_COMB_3000.ProfileLikelihood.mH100.484786542.root","READ"));
    fileSignificance_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p8_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeProfileLikelihood/higgsCombinefinal_R_mll_COMB_3000.ProfileLikelihood.mH100.859027725.root","READ"));
    fileSignificance_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_0p7_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeProfileLikelihood/higgsCombinefinal_R_mll_COMB_3000.ProfileLikelihood.mH100.-2144031331.root ","READ"));
    fileSignificance_noHmH.append(ROOT.TFile("output/DataCards_WW_SS_HvsNoH_Dynamic_PhaseI/Card2D/lumiExtrapolation/computeProfileLikelihood/higgsCombinefinal_R_mll_COMB_3000.ProfileLikelihood.mH100.1923788443.root","READ"));

    xbins   = array('f',[0.1,0.2,0.3,1.0]); 
    yMu     = array('f',[]);

    for file in fileSignificance_noHmH : 
      tree = file.Get("limit");
      for i in range(tree.GetEntries()):
        tree.GetEntry(i);
        t_quantileExpected = tree.quantileExpected;
        if t_quantileExpected == -1.:
          yMu.append(tree.limit);
        else: continue;

    curGraph_exp = ROOT.TGraphAsymmErrors(len(xbins),xbins,yMu);

    can = ROOT.TCanvas("can","can",600,600);
    
    frame = can.DrawFrame(0.1,0.,1,ROOT.TMath.MaxElement(curGraph_exp.GetN(),curGraph_exp.GetY())*1.2);

    frame.GetXaxis().SetTitle("#Delta c^{H}_{V}");
    frame.GetXaxis().SetTitleSize(0.045);
    frame.GetXaxis().SetLabelSize(0.04);

    frame.GetYaxis().SetTitle("Discovery significance H(c^{H}_{V})-H");
    frame.GetYaxis().SetTitleSize(0.045);
    frame.GetYaxis().SetTitleOffset(1.17);
    frame.GetYaxis().SetLabelSize(0.04);
    
    curGraph_exp.SetMarkerStyle(20);
    curGraph_exp.SetLineWidth(3);
    curGraph_exp.SetLineStyle(ROOT.kDashed);
    curGraph_exp.SetMarkerSize(1.6);
    curGraph_exp.SetMarkerSize(1.3);
    curGraph_exp.SetMarkerColor(ROOT.kBlack);

    curGraph_exp.Draw("PL");

    can.Update();
    can.RedrawAxis();
    can.RedrawAxis("g");
    can.Update();

    tex2.Draw();
    tex3.Draw();
    can.SaveAs("output/outputHiggsCoupling/Significance_noHmH.png");
    can.SaveAs("output/outputHiggsCoupling/Significance_noHmH.pdf");
