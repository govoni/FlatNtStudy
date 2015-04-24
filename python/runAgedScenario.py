#! /usr/bin/env pythony
## example python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive/Card1D/lumiExtrapolation/ --inputVariable ptjj --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin 0.85 --rMax 1.15

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
from collections import defaultdict

############################################
#            Job steering                  #
############################################

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

##### other basci options for all the methods 
parser.add_option('--datacardDIR',      action="store", type="string",  dest="datacardDIR",     default="", help="direcotry where to find datacard")
parser.add_option('--inputVariable',    action="store", type="string",  dest="inputVariable",   default="", help="name of the input variable to be used")
parser.add_option('--rescaleFactor',    action="store", type=float,     dest="rescaleFactor",   default=1., help="rescale factor for aging effect")
parser.add_option('--outputCardDIR',    action="store", type="string",  dest="outputCardDIR",   default="", help="output directory")
parser.add_option('--datacardType',     action="store", type=int     ,  dest="datacardType",    default=0, help="0 = inclusive, 1 = polarized, 2 = noH-H")

(options, args) = parser.parse_args()

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    print "###### start extrapolate analysis ########";

    if options.datacardDIR == "":
     sys.exit("provide a datacard directory");

    os.chdir(options.datacardDIR);

    os.system("mkdir -p "+options.outputCardDIR);

    os.system("rm "+options.outputCardDIR+"/*.txt")
    os.system("rm "+options.outputCardDIR+"/*.root")

    ## make the card list
    os.system("ls | grep txt | grep -v COMB | grep _UUpp.txt | grep "+options.inputVariable+" >  list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _UUmm.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _EEpp.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _EEmm.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _EUpp.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _EUmm.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _UEpp.txt | grep "+options.inputVariable+" >> list.txt");
    os.system("ls | grep txt | grep -v COMB | grep _UEmm.txt | grep "+options.inputVariable+" >> list.txt");

    datacardFile = open("list.txt","r");
    datacardList = [];
    
    for iline in datacardFile :
        if iline.split(" ")[0]!="" and iline.split(" ")[0]!="#" and iline.split(" ")[0]!=" "  and iline.split(" ")[0]!="\n" and iline.split(" ")[0]!="\t":
           datacardList.append(iline.split(" ")[0].replace("\n",""));

    ## total number of datacards
    os.system("rm list.txt");

    createdCards = [];

    ## fake rate on datacard list
    for datacard in datacardList :

            fakeColumn   = [] ;
            observed = 0;            
            inputfile = open('%s'%(datacard),'r');

            ## create a new root file
            inputrootfile = ROOT.TFile(datacard.replace(".txt",".root"),"READ");
      
            outname = datacard ;
            print "create the new datacard ",options.outputCardDIR+"/"+outname;
            
            fileNew = open('%s/%s'%(options.outputCardDIR,outname), 'w');
            createdCards.append(outname);

            iproc = 0;

            for ifile in inputfile :

                if ifile.find(datacard.replace(".txt",".root"))!=-1 :
                    line = ifile.replace(datacard.replace(".txt",".root"),datacard.replace(".txt",".root"));
                    fileNew.write(line);
                    continue ;

                if ifile.split(" ")[0] != "rate" and ifile.split(" ")[0] != "process" :
                    fileNew.write(ifile);
                    continue;

                if ifile.split(" ")[0] == "process" and iproc ==0:
                    iproc = 1 ;
                    fileNew.write(ifile);
                    icol = 0;
                    for columns in ifile.split() :
                        if options.datacardType == 0 :
                            if columns != "WW_QCD" and not ROOT.TString(columns).Contains("WW_EWK") and not ROOT.TString(columns).Contains("HminusNoH") :
                                icol = icol+1;
                                continue;
                            else :
                                fakeColumn.append(icol);
                                icol = icol+1;
                                continue;
                        elif options.datacardType== 1 :
                            if columns != "WW_QCD" and not ROOT.TString(columns).Contains("_LL") :
                                icol = icol+1;
                                continue;
                            else :
                                fakeColumn.append(icol);
                                icol = icol+1;
                                continue;
                        elif options.datacardType== 2 :
                            if columns != "WW_QCD" and not ROOT.TString(columns).Contains("HminusNoH") :
                                icol = icol+1;
                                continue;
                            else :
                                fakeColumn.append(icol);
                                icol = icol+1;
                                continue;

                elif ifile.split(" ")[0] == "process" and iproc !=0:
                    fileNew.write(ifile);

                if ifile.split(" ")[0] == "rate" :
                     lineToWrite = "rate ";
                     icol = 0;
                     for columns in ifile.split() :
                         if columns == "rate" :
                            icol = icol+1;
                            continue ;
                         elif icol in fakeColumn :
                                lineToWrite += "  %f   "%(float(columns)*options.rescaleFactor);
                                icol = icol+1;
                                continue;
                         else :                                
                                lineToWrite += "  %f   "%(float(columns));
                                icol = icol+1;
                                continue;

                     fileNew.write(lineToWrite+"\n");
                     continue ;

            fileNew.close();
                
            ## copy root file
            outrootname = outname.replace(".txt",".root");
            outrootfile = ROOT.TFile("%s/%s"%(options.outputCardDIR,outrootname),"RECREATE");
              
            for key in inputrootfile.GetListOfKeys() :
                    if key.GetClassName().find("TH1") == -1 and key.GetClassName().find("TH2") == -1 :
                        continue ;
                    outrootfile.cd();                  
                    histo = inputrootfile.Get(key.GetName()).Clone("temp");
                    if options.datacardType == 0 :
                        if ROOT.TString(key.GetName()).Contains("WW_QCD") or ROOT.TString(key.GetName()).Contains("WW_EWK") or ROOT.TString(key.GetName()).Contains("HminusNoH"):       
                            histo.Scale(options.rescaleFactor);
                    elif options.datacardType == 1:
                        if ROOT.TString(key.GetName()).Contains("WW_QCD") or ROOT.TString(key.GetName()).Contains("_LL") :
                            histo.Scale(options.rescaleFactor);
                    elif options.datacardType == 2 :
                        if ROOT.TString(key.GetName()).Contains("WW_QCD") or ROOT.TString(key.GetName()).Contains("HminusNoH") :
                            histo.Scale(options.rescaleFactor);


                    histo.Write(key.GetName());

            outrootfile.Write();              
            outrootfile.Close();    
