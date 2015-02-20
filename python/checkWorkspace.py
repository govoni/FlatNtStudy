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

from ROOT import gROOT, gStyle, gSystem, TLatex, TColor, gPad, TList, TString

from collections import defaultdict

############################################
#            Job steering                  #
############################################

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

## parse files
parser.add_option('--inputDIR',            action="store", type="string", dest="inputDIR",      default="",   help="input directory where files are located")
parser.add_option('--channel',             action="store", type="string", dest="channel",       default="UU", help="workspace channel to look at")
parser.add_option('--outputPlotDIR',       action="store", type="string", dest="outputPlotDIR", default="",   help="output directory where to put plots")
parser.add_option('--inputVariableList',   action="store", type="string", dest="inputVariableList", default="",   help="input variable list")
parser.add_option('--isCard2D',            action="store", type="int",    dest="isCard2D",          default=0,   help="if 2D or 1D card to be considered")

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
  gStyle.SetTitleYOffset(1.15); # => 1.15 if exponents                                                                                                                       

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

shapeSys = ["CMS_scale_l","CMS_res_l","CMS_scale_j","CMS_res_j","stat_shape"]

process  = ["fakeLepton","WZ_EWK","wrongCharge","WW_QCD","WZ_QCD","WW_EWK_SS_TT","WW_EWK_SS_TL","WW_EWK_SS_LL","WW_EWK_SS_126","noHminusH","Data"]

if __name__ == '__main__':
  
  ## variable list
  variableName = [];
  variableLabel = [];

  fileVar = open('%s'%(options.inputVariableList), 'r');

  if not options.isCard2D :

    for columns in ( raw.strip().split() for raw in fileVar ):
      if columns[0].find('#')!=-1 or columns[0] == ' ': continue;
      variableLabel.append(columns[4]);
      variableName.append(columns[0]);

  else:

    for columns in ( raw.strip().split() for raw in fileVar ):
      if columns[0].find('#')!=-1 or columns[0] == ' ': continue;

      if not columns[0] in variableName :
        variableName.append(columns[0]);

      if not columns[4] in variableLabel :
        variableLabel.append(columns[4]);


  ## fix the chdir
  os.chdir(options.inputDIR);

  ## create output plot directory
  os.system("mkdir -p "+options.outputPlotDIR);

  ## create the file list    
  os.system("ls | grep root | grep root | grep "+options.channel+"  > list.txt");

  setStyle();

  ## make the list of files
  file = open("list.txt",'r')
  filelist = [];

  for line in file :
    filelist.append(line.split(" ")[0].replace("\n",""));

  os.system("rm list.txt");    

  print filelist;

  can = ROOT.TCanvas("can","can",650,650);
  leg = ROOT.TLegend(0.65,0.82,0.82,0.92);

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

  
  ## loop on variables and files
  for var in range(len(variableName)) :
    for file in filelist :
      if file.find(variableName[var]+"_"+options.channel) != -1 :
        print file;        
        ## take the file open it and start to make comparison plots
        tfile = ROOT.TFile(file,"READ");
        tlist = tfile.GetListOfKeys();
        ## loop on file keys
        for key in range(tlist.GetEntries()):
          ## skip sys
          if TString(tlist.At(key).GetName()).Contains("Up") or TString(tlist.At(key).GetName()).Contains("Down"):
            continue;
          
          ## identify the process
          proc = "";
          for p in process :
            if TString(tlist.At(key).GetName()).Contains(p) :
              proc = p ;
              break;
          
          ## start to plot each systematics if found in the same canvas:
          for sys in shapeSys :
            ## make the same plot of the three variations
            if (tlist.FindObject("histo_"+proc+"_"+sys+"Up") and tlist.FindObject("histo_"+proc+"_"+sys+"Down")) or (tlist.FindObject("histo_"+proc+"_CMS_"+proc+"_"+sys+"Up") and tlist.FindObject("histo_"+proc+"_CMS_"+proc+"_"+sys+"Down")): 

              can.cd();

              sysUp = tfile.FindObjectAny("histo_"+proc+"_"+sys+"Up") ;
              sysDw = tfile.FindObjectAny("histo_"+proc+"_"+sys+"Down") ;
              
              if not sysUp :
                sysUp = tfile.FindObjectAny("histo_"+proc+"_CMS_"+proc+"_"+sys+"Up") ;
              if not sysDw :
                sysDw = tfile.FindObjectAny("histo_"+proc+"_CMS_"+proc+"_"+sys+"Down") ;

              hist  = tfile.FindObjectAny(tlist.At(key).GetName());

              hist.SetLineColor(ROOT.kBlack);
              hist.SetMarkerColor(ROOT.kBlack);
              hist.SetMarkerStyle(20);
              hist.SetFillColor(ROOT.kBlack);
              hist.SetFillStyle(0);
              hist.SetLineWidth(2);

              hist.GetXaxis().SetTitle(variableLabel[var]);
              hist.GetYaxis().SetTitle("Entries");
              hist.GetYaxis().SetTitleOffset(1.20);
              hist.GetYaxis().SetRangeUser(0,max(hist.GetMaximum(),sysUp.GetMaximum(),sysDw.GetMaximum())*1.2);

              hist.Draw("hist");

              sysUp.SetLineColor(ROOT.kBlue);
              sysUp.SetMarkerColor(ROOT.kBlue);
              sysUp.SetMarkerStyle(20);
              sysUp.SetFillStyle(0);
              sysUp.SetFillColor(ROOT.kBlue);
              sysUp.SetLineWidth(2);

              sysDw.SetLineColor(ROOT.kRed);
              sysDw.SetMarkerColor(ROOT.kRed);
              sysDw.SetMarkerStyle(20);
              sysDw.SetFillColor(ROOT.kRed);
              sysDw.SetFillStyle(0);
              sysDw.SetLineWidth(2);

              sysUp.Draw("hist same");
              sysDw.Draw("hist same");
              
              leg.Clear();
              leg.SetFillColor(0);
              leg.SetShadowColor(0);
              leg.SetTextFont(42);
              leg.SetTextSize(0.028);
              leg.SetBorderSize(0);
              leg.AddEntry(hist,"central value","L");
              leg.AddEntry(sysUp,"%s up"%(sys),"L");
              leg.AddEntry(sysDw,"%s down"%(sys),"L");

              leg.Draw("same");

              tex.Draw("same");
              tex2.Draw("same");
              tex3.Draw("same");

              can.SaveAs("%s/%s_%s_%s_%s.png"%(options.outputPlotDIR,proc,variableName[var],sys,options.channel),"png");
              can.SaveAs("%s/%s_%s_%s_%s.pdf"%(options.outputPlotDIR,proc,variableName[var],sys,options.channel),"pdf");
              
