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

from cStringIO import StringIO

from subprocess import Popen

from ROOT import gROOT, gStyle, gSystem, TLatex, TGaxis, TPaveText, TH2D, TColor, gPad, TGraph2D, TLine,TGraph,TList

from collections import defaultdict


#####################################
###### definition of the style ######
#####################################

def setStyle ():

  gStyle.SetPadBorderMode (0)
  gStyle.SetFrameBorderMode (0)
  gStyle.SetPadBottomMargin (0.12)
  gStyle.SetPadLeftMargin (0.12)
  gStyle.SetCanvasColor (ROOT.kWhite)
  gStyle.SetCanvasDefH (600) #Height of canvas                                                                                                                               
  gStyle.SetCanvasDefW (600) #Width of canvas                                                                                                                                
  gStyle.SetCanvasDefX (0)   #POsition on screen
  gStyle.SetCanvasDefY (0)
  gStyle.SetPadTopMargin (0.05)
  gStyle.SetPadBottomMargin (0.12)#0.13)
  gStyle.SetPadLeftMargin (0.13)#0.16)
  gStyle.SetPadRightMargin (0.07)#0.02)                                                                                                                                     

  # For the Pad:                                                                                                                                                             
  gStyle.SetPadBorderMode (0)
  gStyle.SetPadColor (ROOT.kWhite)
  gStyle.SetPadGridX (ROOT.kFALSE)
  gStyle.SetPadGridY (ROOT.kFALSE)
  gStyle.SetGridColor (0)
  gStyle.SetGridStyle (3)
  gStyle.SetGridWidth (1)

  # For the frame:                                                                                                                                                           
  gStyle.SetFrameBorderMode (0)
  gStyle.SetFrameBorderSize (1)
  gStyle.SetFrameFillColor (0)
  gStyle.SetFrameFillStyle (0)
  gStyle.SetFrameLineColor (1)
  gStyle.SetFrameLineStyle (1)
  gStyle.SetFrameLineWidth (1)

  gStyle.SetAxisColor (1, "XYZ")
  gStyle.SetStripDecimals (ROOT.kTRUE)
  gStyle.SetTickLength (0.03, "XYZ")
  gStyle.SetNdivisions (505, "XYZ")
  gStyle.SetPadTickX (1)  # To get tick marks on the opposite side of the frame                                                                                              
  gStyle.SetPadTickY (1)
  gStyle.SetGridColor (0)
  gStyle.SetGridStyle (3)
  gStyle.SetGridWidth (1)

  gStyle.SetTitleColor (1, "XYZ")
  gStyle.SetTitleFont (42, "XYZ")
  gStyle.SetTitleSize (0.05, "XYZ")
  gStyle.SetTitleXOffset (1.15)#0.9)                                                                                                                                       
  gStyle.SetTitleYOffset (1.3) # => 1.15 if exponents                                                                                                                       

  gStyle.SetLabelColor (1, "XYZ")
  gStyle.SetLabelFont (42, "XYZ")
  gStyle.SetLabelOffset (0.007, "XYZ")
  gStyle.SetLabelSize (0.045, "XYZ")

  gStyle.SetPadBorderMode (0)
  gStyle.SetFrameBorderMode (0)
  gStyle.SetTitleTextColor (1)
  gStyle.SetTitleFillColor (10)
  gStyle.SetTitleFontSize (0.05)

  gStyle.SetOptStat (0)
  gStyle.SetOptTitle (0)
  gStyle.SetOptFit (0)

  NRGBs = 5
  NCont = 255
  stops = [ 0.00, 0.34, 0.61, 0.84, 1.00 ]
  red   = [ 0.00, 0.00, 0.87, 1.00, 0.51 ]
  green = [ 0.00, 0.81, 1.00, 0.20, 0.00 ]
  blue  = [ 0.51, 1.00, 0.12, 0.00, 0.00 ]
  stopsArray = array ('d', stops)
  redArray   = array ('d', red)
  greenArray = array ('d', green)
  blueArray  = array ('d', blue)
  TColor.CreateGradientColorTable (NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
  gStyle.SetNumberContours (NCont)

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    ## set the style
    setStyle ()

    tex = ROOT.TLatex (0.82,0.96,"14 TeV")
    tex.SetNDC (1)
    tex.SetTextAlign (11)
    tex.SetTextFont (42)
    tex.SetTextSize (0.04)
    tex.SetLineWidth (2)

    tex2 = TLatex (0.17,0.96,"CMS Delphes Simulation")
    tex2.SetNDC (1)
    tex2.SetTextAlign (11)
    tex2.SetTextFont (61)    
    tex2.SetLineWidth (2)
    tex2.SetTextSize (0.04)

    tex4 = TLatex (0.17,0.80,"WW WZ combination")
    tex4.SetNDC (1)
    tex4.SetTextAlign (11)
    tex4.SetTextFont (42)    
    tex4.SetLineWidth (2)
    tex4.SetTextSize (0.04)

    tex3 = ROOT.TLatex (0.67,0.96,"14 TeV, 3000 fb^{-1}")
    tex3.SetNDC (1)
    tex3.SetTextAlign (11)
    tex3.SetTextFont (42)
    tex3.SetTextSize (0.04)
    tex3.SetLineWidth (2)

    parser = argparse.ArgumentParser (description = 'plot LL, TL, TT and bkg')
    parser.add_argument ('-D', '--datacard' , default= 'NONE', help='1D datacard to be read')
    parser.add_option('-b', action='store_true', dest='noX', default=True, help='no X11 windows')
    args = parser.parse_args ()
    print 'reading', args.datacard
    f_datacard = ROOT.TFile (args.datacard)
    l0 = [elem.GetName () for elem in f_datacard.GetListOfKeys ()]
    l1 = [elem for elem in l0 if elem.find ('_scale') == -1]
    l2 = [elem for elem in l1 if elem.find ('Data') == -1]
    l3 = [elem for elem in l2 if elem.find ('_res') == -1]
    l4 = [elem for elem in l3 if elem.find ('_stat') == -1]
    l5 = [elem for elem in l4 if elem.find ('_shape') == -1]

    elem_TT = [elem for elem in l5 if elem.find ('_TT') != -1]
    elem_LT = [elem for elem in l5 if elem.find ('_LT') != -1]
    elem_LL = [elem for elem in l5 if elem.find ('_LL') != -1]

    l6 = [elem for elem in l5 if elem.find ('_TT') == -1]
    l7 = [elem for elem in l6 if elem.find ('_LT') == -1]
    l8 = [elem for elem in l7 if elem.find ('_LL') == -1]
    
    print 'backgrouds:   ', l8
    print 'TT component: ', elem_TT
    print 'LT component: ', elem_LT
    print 'LL component: ', elem_LL
    
    h_LL = f_datacard.Get (elem_LL[0])
    h_LT = f_datacard.Get (elem_LT[0])
    h_TT = f_datacard.Get (elem_TT[0])
    h_bk = f_datacard.Get (l8[0])
    for i in range (1, len (l8)) : 
        h_bk.Add (f_datacard.Get (l8[i]))
        print 'adding', l8[i]

    h_LL.Scale (1./h_LL.Integral ())    
    h_LT.Scale (1./h_LT.Integral ())    
    h_TT.Scale (1./h_TT.Integral ())    
    h_bk.Scale (1./h_bk.Integral ())    
    
    can = ROOT.TCanvas ("can","can",600,600)
    
    legend = ROOT.TLegend (0.4,0.68,0.56,0.9)
    legend.SetFillColor (0)
    legend.SetFillStyle (0)
    legend.SetBorderSize (0)
    legend.SetTextSize (0.040)

    h_LL.SetLineColor (1)
    h_LT.SetLineColor (2)
    h_TT.SetLineColor (9)
    h_bk.SetLineColor (16)

    h_LT.SetLineStyle (2)
    h_TT.SetLineStyle (5)

    h_bk.SetFillColor (16)

    histos = [h_bk, h_TT, h_LT, h_LL]
    maxi = 0. 
    mini = h_LL.GetMaximum ()
    for elem in histos :
      if elem.GetMaximum () > maxi : maxi = elem.GetMaximum ()
      if elem.GetMinimum () < mini : mini = elem.GetMinimum ()
    maxi = maxi * 1.1
    if mini > 0 : mini = mini * 0.9  

    frame = can.DrawFrame (h_LL.GetXaxis ().GetXmin (), mini, h_LL.GetXaxis ().GetXmax (), maxi)

    frame.GetXaxis ().SetTitle (h_LL.GetXaxis ().GetTitle ()) 
    frame.GetXaxis ().SetTitleSize (0.045)
    frame.GetXaxis ().SetLabelSize (0.04)

    frame.GetYaxis ().SetTitle (h_LL.GetYaxis ().GetTitle ())
    frame.GetYaxis ().SetTitleSize (0.045)
    frame.GetYaxis ().SetTitleOffset (1.17)
    frame.GetYaxis ().SetLabelSize (0.04)

    legend.AddEntry (h_LL, "LL","l")
    legend.AddEntry (h_LT, "LT","l")
    legend.AddEntry (h_TT, "TT","l")
    legend.AddEntry (h_bk, "bkg","fl")
    
    for elem in histos : 
        elem.SetLineWidth (3)
        elem.Draw ('histo same')
        
    tex.Draw ()
    tex2.Draw ()
    legend.Draw ()
    can.RedrawAxis ()

    can.SaveAs (args.datacard + '.png', 'png')

    
