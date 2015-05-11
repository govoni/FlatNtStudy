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
import argparse

from optparse import OptionParser

from cStringIO import StringIO

from subprocess import Popen

from ROOT import gROOT, gStyle, gSystem, TLatex, TGaxis, TPaveText, TH2D, TColor, gPad, TGraph2D, TLine,TGraph,TList

from collections import defaultdict

parser = OptionParser()
parser.add_option('-b', action='store_true', dest='noX', default=True, help='no X11 windows')
parser.add_option('-O', '--datacardOld' , default= 'NONE', help='1D datacard to be read')
parser.add_option('-N', '--datacardNew' , default= 'NONE', help='1D datacard to be read')

(options, args) = parser.parse_args()


def readDC (f_datacard, tag, numCol) :
    print 'reading', f_datacard.GetName()
    histos = {}
    for elem in f_datacard.GetListOfKeys () :
        dummy = f_datacard.Get (elem.GetName ())
        dummy.SetName (elem.GetName () + tag)
        dummy.SetLineColor (int (numCol))
        histos[elem.GetName ()] = dummy
    return histos


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




##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## set the style
    setStyle();
    f_datacardOld = ROOT.TFile (options.datacardOld)
    histoOld = readDC (f_datacardOld, 'Old', '2')
    f_datacardNew = ROOT.TFile (options.datacardNew)
    histoNew = readDC (f_datacardNew, 'New', '8')

    os.system("mkdir -p outputPlot")
    
    can = ROOT.TCanvas ("can","can",600,600)
    print histoOld
    for key, value in histoOld.iteritems() :
        if histoOld[key].GetMaximum () > histoNew[key].GetMaximum() :            
            histoOld[key].Draw ('hist')
        else :    
            histoNew[key].Draw ('')
        histoOld[key].Draw ('hist same')
        histoNew[key].Draw ('same')
        can.SaveAs ('outputPlot/compareDC_' + key + '.png','png')

    
