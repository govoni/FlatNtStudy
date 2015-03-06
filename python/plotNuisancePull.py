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
    normalizationHistoIn        = {};
    normalizationHistoError_In  = {};
    normalizationHisto_S        = {};
    normalizationHistoError_S   = {};
    normalizationHisto_B        = {};
    normalizationHistoError_B   = {};

    nuisanceHistoIn        = {};
    nuisanceHistoError_In  = {};
    nuisanceHisto_S        = {};
    nuisanceHistoError_S   = {};
    nuisanceHisto_B        = {};
    nuisanceHistoError_B   = {};


    ### make pull plot for normalization
    pullNormS = {};
    pullNormB = {};
    pullNuisS = {};
    pullNuisB = {};

    ## loop on the variable list
    ifile = 0 ;
    for file in filelist :

      ## sum number of files
      ifile = ifile +1 ;

      ## open a file
      tfile = ROOT.TFile.Open(file,"READ");

      #################
      ## PREFIT PART ##
      #################

      ## take the input normalizations pre-fit
      normalizationArgList = ROOT.RooArgList(tfile.Get("norm_prefit"));

      ## take the input nuisances
      nuisance_prefit = tfile.Get("nuisances_prefit_res");
      nuisanceArgList = nuisance_prefit.floatParsInit();

      for ilist in range(normalizationArgList.getSize()) :

        if ifile == 1 :
          normalizationHistoIn[normalizationArgList.at(ilist).GetName()] = []
          normalizationHistoError_In[normalizationArgList.at(ilist).GetName()] = ROOT.TH1F("normalizationHistoError_In_"+normalizationArgList.at(ilist).GetName(),"",1000,0,10000);

        variable = ROOT.RooRealVar(normalizationArgList.at(ilist));
        normalizationHistoIn[normalizationArgList.at(ilist).GetName()].append(variable.getVal());
        normalizationHistoError_In[normalizationArgList.at(ilist).GetName()].Fill(variable.getError());


      for ilist in range(nuisanceArgList.getSize()) :
        if ifile == 1:
          nuisanceHistoIn[nuisanceArgList.at(ilist).GetName()] = [];
          nuisanceHistoError_In[nuisanceArgList.at(ilist).GetName()] = ROOT.TH1F("nuisanceHistoError_In_"+nuisanceArgList.at(ilist).GetName(),"",1000,0,10000);

        variable = ROOT.RooRealVar(nuisanceArgList.at(ilist));
        nuisanceHistoIn[nuisanceArgList.at(ilist).GetName()].append(variable.getVal());
        nuisanceHistoError_In[nuisanceArgList.at(ilist).GetName()].Fill(variable.getError());

      
      #################
      ## POSTFIT PART #
      #################


      ## take the output nuisance B and S fit
      for itoy in range(options.nToys):
        
        normalizationArgList_S = ROOT.RooArgList(tfile.Get("norm_fit_s_%d"%itoy));

        for ilist in range(normalizationArgList_S.getSize()) :


          if ifile == 1 :
            normalizationHisto_S[normalizationArgList_S.at(ilist).GetName()] = []
            normalizationHistoError_S[normalizationArgList.at(ilist).GetName()] = ROOT.TH1F("normalizationHistoError_S_"+normalizationArgList.at(ilist).GetName()+"_"+str(itoy),"",1000,0,10000);
            pullNormS[normalizationArgList_S.at(ilist).GetName()] = [];

          variable = ROOT.RooRealVar(normalizationArgList_S.at(ilist));
          normalizationHisto_S[normalizationArgList_S.at(ilist).GetName()].append(variable.getVal());
          normalizationHistoError_S[normalizationArgList_S.at(ilist).GetName()].Fill(variable.getError());
        
          pullNormS[normalizationArgList_S.at(ilist).GetName()].append(variable.getVal()-ROOT.RooRealVar(normalizationArgList.at(ilist)).getVal());

          
        ## take the output normalization B and S fit
        normalizationArgList_B = ROOT.RooArgList(tfile.Get("norm_fit_b_%d"%itoy));

        for ilist in range(normalizationArgList_B.getSize()) :

          if ifile == 1 :
            normalizationHistoError_B[normalizationArgList.at(ilist).GetName()] = ROOT.TH1F("normalizationHistoError_B_"+normalizationArgList.at(ilist).GetName()+"_"+str(itoy),"",1000,0,10000);
            normalizationHisto_B[normalizationArgList_B.at(ilist).GetName()] = [];
            pullNormB[normalizationArgList_B.at(ilist).GetName()] = [];

          variable = ROOT.RooRealVar(normalizationArgList_B.at(ilist));
          normalizationHisto_B[normalizationArgList_B.at(ilist).GetName()].append(variable.getVal());
          normalizationHistoError_B[normalizationArgList_B.at(ilist).GetName()].Fill(variable.getError());
          pullNormB[normalizationArgList_B.at(ilist).GetName()].append(variable.getVal()-ROOT.RooRealVar(normalizationArgList.at(ilist)).getVal());



        ## take the output fitted nuisances
        result_S = tfile.Get("fit_s_%d"%itoy)
        nuisanceArgList_S = result_S.floatParsFinal();

        for ilist in range(nuisanceArgList_S.getSize()):
         if ifile ==  1 :
           nuisanceHisto_S[nuisanceArgList_S.at(ilist).GetName()] = [];
           nuisanceHistoError_S[nuisanceArgList_S.at(ilist).GetName()] = ROOT.TH1F("nuisanceHistoError_S_"+nuisanceArgList_S.at(ilist).GetName()+"_"+str(itoy),"",1000,0,10000);
           pullNuisS[nuisanceArgList_S.at(ilist).GetName()] = [];

         variable = ROOT.RooRealVar(nuisanceArgList_S.at(ilist));
         nuisanceHisto_S[nuisanceArgList_S.at(ilist).GetName()].append(variable.getVal());
         nuisanceHistoError_S[nuisanceArgList_S.at(ilist).GetName()].Fill(variable.getError());
         if nuisanceArgList_S.at(ilist).GetName() != "r" :
           pullNuisS[nuisanceArgList_S.at(ilist).GetName()].append(variable.getVal()-ROOT.RooRealVar(nuisanceArgList.at(ilist)).getVal());
         else:
           pullNuisS[nuisanceArgList_S.at(ilist).GetName()].append(variable.getVal()-options.expectSignal);

        ## take the output fitted nuisances
        result_B = tfile.Get("fit_s_%d"%itoy)
        nuisanceArgList_B = result_B.floatParsFinal();

        for ilist in range(nuisanceArgList_B.getSize()):
         if ifile ==  1 :
           nuisanceHisto_B[nuisanceArgList_B.at(ilist).GetName()] = [];
           nuisanceHistoError_B[nuisanceArgList_B.at(ilist).GetName()] = ROOT.TH1F("nuisanceHistoError_B_"+nuisanceArgList_B.at(ilist).GetName()+"_"+str(itoy),"",1000,0,10000);
           pullNuisB[nuisanceArgList_B.at(ilist).GetName()] = [];

         variable = ROOT.RooRealVar(nuisanceArgList_B.at(ilist));
         nuisanceHisto_B[nuisanceArgList_B.at(ilist).GetName()].append(variable.getVal());
         nuisanceHistoError_B[nuisanceArgList_B.at(ilist).GetName()].Fill(variable.getError());
         if nuisanceArgList_B.at(ilist).GetName() != "r" :
           pullNuisB[nuisanceArgList_B.at(ilist).GetName()].append(variable.getVal()-ROOT.RooRealVar(nuisanceArgList.at(ilist)).getVal());
         else:
           pullNuisB[nuisanceArgList_B.at(ilist).GetName()].append(variable.getVal()-options.expectSignal);



    ###########################
    ### MAKE the final PULL ###
    ###########################
 
    pullPlotNormSFit = ROOT.TH1F("pullPlotNormSFit","",len(pullNormS),0,len(pullNormS));
    pullValue = ROOT.TH1F("pullValue","",100,-5,5);
    ibin = 0;
    for key, value in pullNormS.iteritems() :
      pullPlotNormSFit.GetXaxis().SetBinLabel(ibin+1,key.split("/")[1])
      pullValue.Reset("ICES");
      for val in range(len(value)):
        pullValue.Fill(value[val]/normalizationHistoError_S[key].GetMean());

      pullPlotNormSFit.SetBinContent(ibin+1,pullValue.GetMean());
      pullPlotNormSFit.SetBinError(ibin+1,normalizationHistoError_S[key].GetMean()/normalizationHistoError_In[key].GetMean())
      ibin = ibin+1;

    pullPlotNormBFit = ROOT.TH1F("pullPlotNormBFit","",len(pullNormB),0,len(pullNormB));
    ibin = 0;
    for key, value in pullNormB.iteritems() :
      pullPlotNormBFit.GetXaxis().SetBinLabel(ibin+1,key.split("/")[1])
      pullValue.Reset("ICES");
      for val in range(len(value)):
        pullValue.Fill(value[val]/normalizationHistoError_B[key].GetMean());

      pullPlotNormBFit.SetBinContent(ibin+1,pullValue.GetMean());
      pullPlotNormBFit.SetBinError(ibin+1,normalizationHistoError_B[key].GetMean()/normalizationHistoError_In[key].GetMean());
      ibin = ibin+1;

    pullPlotNuisSFit = ROOT.TH1F("pullPlotNuisSFit","",len(pullNuisS),0,len(pullNuisS));
    ibin = 0;
    for key, value in pullNuisS.iteritems() :
      pullPlotNuisSFit.GetXaxis().SetBinLabel(ibin+1,key.split("/")[0])
      pullValue.Reset("ICES");
      for val in range(len(value)):
        pullValue.Fill(value[val]/nuisanceHistoError_S[key].GetMean());

      pullPlotNuisSFit.SetBinContent(ibin+1,pullValue.GetMean());
      if key != "r" :
        pullPlotNuisSFit.SetBinError(ibin+1,nuisanceHistoError_S[key].GetMean()/nuisanceHistoError_In[key].GetMean());        
      else :
        pullPlotNuisSFit.SetBinError(ibin+1,nuisanceHistoError_S[key].GetMean()/1.);        
      ibin = ibin+1;

    pullPlotNuisBFit = ROOT.TH1F("pullPlotNuisBFit","",len(pullNuisB),0,len(pullNuisB));
    ibin = 0;
    for key, value in pullNuisB.iteritems() :
      pullPlotNuisBFit.GetXaxis().SetBinLabel(ibin+1,key.split("/")[0])
      pullValue.Reset("ICES");
      for val in range(len(value)):
        pullValue.Fill(value[val]/nuisanceHistoError_B[key].GetMean());

      pullPlotNuisBFit.SetBinContent(ibin+1,pullValue.GetMean());
      if key != "r" :
        pullPlotNuisBFit.SetBinError(ibin+1,nuisanceHistoError_B[key].GetMean()/nuisanceHistoError_In[key].GetMean());
      else :
        pullPlotNuisBFit.SetBinError(ibin+1,nuisanceHistoError_B[key].GetMean()/1.);
      ibin = ibin+1;
 
    
    ############
    ### DRAW ###
    ############

    canSNorm = ROOT.TCanvas("canSNorm","canSNorm",1050,650);    

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

    pullPlotNormSFit.GetYaxis().SetTitle("Normalization Pull");
    pullPlotNormSFit.GetYaxis().SetTitleOffset(1.05);
    pullPlotNormSFit.GetYaxis().SetTitleSize(0.045);
    pullPlotNormSFit.GetYaxis().SetTitleFont(42);

    pullPlotNormSFit.GetXaxis().SetTitleSize(0.045);
    pullPlotNormSFit.GetXaxis().SetTitleFont(42);

    band1s = ROOT.TGraphErrors();
    band2s = ROOT.TGraphErrors();
    band1s.SetFillColor(ROOT.kGreen);
    band2s.SetFillColor(ROOT.kYellow);

    for ipoint in range(pullPlotNormSFit.GetNbinsX()+1):
      band1s.SetPoint(ipoint,ipoint,0);
      band1s.SetPointError(ipoint,0,1);

      band2s.SetPoint(ipoint,ipoint,0);
      band2s.SetPointError(ipoint,0,2);

    pullPlotNormSFit.GetYaxis().SetRangeUser(-3,3);
    pullPlotNormSFit.Draw("AXIS")
    band2s.Draw("E3same");
    band1s.Draw("E3same");

    oneSLine = ROOT.TF1("oneSLine","1",0,pullPlotNormSFit.GetNbinsX()+1);
    oneSLine.SetLineColor(ROOT.kBlack); oneSLine.SetLineWidth(2);
    oneSLine.Draw("same");
    twoSLine = ROOT.TF1("twoSLine","-1",0,pullPlotNormSFit.GetNbinsX()+1);
    twoSLine.SetLineColor(ROOT.kBlack); oneSLine.SetLineWidth(2);
    twoSLine.Draw("same");

    pullPlotNormSFit.SetLineColor(ROOT.kBlack);
    pullPlotNormSFit.SetLineWidth(2);
    pullPlotNormSFit.SetMarkerColor(ROOT.kBlack);
    pullPlotNormSFit.SetMarkerStyle(20);

    pullPlotNormSFit.SetFillColor(ROOT.kBlack);
    pullPlotNormSFit.SetFillStyle(3001);
    pullPlotNormSFit.Draw("PE2 same");

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();

    canSNorm.Update();
    canSNorm.RedrawAxis();
    canSNorm.RedrawAxis("g");
    canSNorm.Update();

    canSNorm.SaveAs("normS_%s_%s.png"%(options.inputVariable,options.channel),"png");
    canSNorm.SaveAs("normS_%s_%s.pdf"%(options.inputVariable,options.channel),"pdf");
 
    ########
    canSNuis = ROOT.TCanvas("canSNuis","canSNuis",1150,650);    

    pullPlotNuisSFit.GetYaxis().SetTitle("Nuisance Pull");
    pullPlotNuisSFit.GetYaxis().SetTitleOffset(1.05);
    pullPlotNuisSFit.GetYaxis().SetTitleSize(0.045);
    pullPlotNuisSFit.GetYaxis().SetTitleFont(42);

    pullPlotNuisSFit.GetXaxis().SetTitleSize(0.045);
    pullPlotNuisSFit.GetXaxis().SetTitleFont(42);

    for ipoint in range(pullPlotNuisSFit.GetNbinsX()+1):
      band1s.SetPoint(ipoint,ipoint,0);
      band1s.SetPointError(ipoint,0,1);

      band2s.SetPoint(ipoint,ipoint,0);
      band2s.SetPointError(ipoint,0,2);

    pullPlotNuisSFit.GetYaxis().SetRangeUser(-3,3);
    pullPlotNuisSFit.Draw("AXIS")
    band2s.Draw("E3same");
    band1s.Draw("E3same");

    oneSLine = ROOT.TF1("oneSLine","1",0,pullPlotNuisSFit.GetNbinsX()+1);
    oneSLine.SetLineColor(ROOT.kBlack); oneSLine.SetLineWidth(2);
    oneSLine.Draw("same");
    twoSLine = ROOT.TF1("twoSLine","-1",0,pullPlotNuisSFit.GetNbinsX()+1);
    twoSLine.SetLineColor(ROOT.kBlack); twoSLine.SetLineWidth(2);
    twoSLine.Draw("same");
    zeroSLine = ROOT.TF1("zeroSLine","0",0,pullPlotNuisSFit.GetNbinsX()+1);
    zeroSLine.SetLineColor(ROOT.kBlack); zeroSLine.SetLineWidth(2);  zeroSLine.SetLineStyle(7);
    zeroSLine.Draw("same");

    pullPlotNuisSFit.SetLineColor(ROOT.kBlack);
    pullPlotNuisSFit.SetLineWidth(2);
    pullPlotNuisSFit.SetMarkerColor(ROOT.kBlack);
    pullPlotNuisSFit.SetMarkerStyle(20);

    pullPlotNuisSFit.SetFillColor(ROOT.kBlack);
    pullPlotNuisSFit.SetFillStyle(3001);
    pullPlotNuisSFit.Draw("PE2 same");

    tex.Draw("same");
    tex2.Draw();
    tex3.Draw();

    canSNuis.Update();
    canSNuis.RedrawAxis();
    canSNuis.RedrawAxis("g");
    canSNuis.Update();

    canSNuis.SaveAs("nuisS_%s_%s.png"%(options.inputVariable,options.channel),"png");
    canSNuis.SaveAs("nuisS_%s_%s.pdf"%(options.inputVariable,options.channel),"pdf");
