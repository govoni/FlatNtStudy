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
from collections import defaultdict

############################################
#            Job steering                  #
############################################

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

##### other basci options for all the methods 
parser.add_option('--datacardDIR',   action="store", type="string", dest="datacardDIR",    default="")
parser.add_option('--inputVariable', action="store", type="string", dest="inputVariable",  default="")
parser.add_option('--inputLumi',     action="store", type="float",  dest="inputLumi",      default=3000)
parser.add_option('--outputDIR',     action="store", type="string",  dest="outputDIR",     default="")


parser.add_option('--makeAsymptotic',  action="store_true", dest="makeAsymptotic",        default=0)
parser.add_option('--makeProfileLikelihood',  action="store_true", dest="makeProfileLikelihood", default=0)
parser.add_option('--makeMaxLikelihoodFit',   action="store_true", dest="makeMaxLikelihoodFit",  default=0)

parser.add_option('--injectSignal',          action="store", type=float,    dest="injectSignal",  default=0., help='inject a singal in the toy generation')
parser.add_option('--nToys',                 action="store", type="int",    dest="nToys",                 default=0,  help="number of toys to generate")

parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)


luminosity = [20,50,100,250,500,1000,2000,3000];

(options, args) = parser.parse_args()


##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    print "###### start extrapolate analysis ########";

    if options.datacardDIR == "":
     sys.exit("provide a datacard directory");

    os.chdir(options.datacardDIR);

    for lumi in luminosity :
        os.system("rm *_%d.txt"%(lumi))
        os.system("rm *_%d.root"%(lumi))
    
    ## make the card list
    os.system("ls | grep txt | grep -v COMB | grep "+options.inputVariable+" > list.txt");

    datacardFile = open("list.txt","r");
    datacardList = [];

    for iline in datacardFile :
        if iline.split(" ")[0]!="" and iline.split(" ")[0]!="#" and iline.split(" ")[0]!=" "  and iline.split(" ")[0]!="\n" and iline.split(" ")[0]!="\t":
           datacardList.append(iline.split(" ")[0].replace("\n",""));

    ## total number of datacards
    os.system("rm list.txt");

    createdCards = [];

    ## lumi on datacard list
    for datacard in datacardList :

        observed = 0;

        ## loop on lumi values        
        for lumi in luminosity :

            inputfile = open('%s'%(datacard),'r');

            ## create a new root file
            inputrootfile = ROOT.TFile(datacard.replace(".txt",".root"),"READ");
      

            outname = datacard ;
            outname = datacard.replace(".txt","_%d.txt"%(lumi));
            print "create the new datacard ",outname;
              
            fileNew = open('%s'%(outname), 'w');
            createdCards.append(outname);

            for ifile in inputfile :

                if ifile.find(datacard.replace(".txt",".root"))!=-1 :
                    line = ifile.replace(datacard.replace(".txt",".root"),datacard.replace(".txt","_%d.root"%(lumi)));
                    fileNew.write(line);
                    continue ;

                if ifile.split(" ")[0] != "rate" and ifile.split(" ")[0] != "observation" :
                    fileNew.write(ifile);
                    continue;

                if ifile.split(" ")[0] == "observation" :
                    lineToWrite = "observation ";
                    for columns in ifile.split() :
                        if columns == "observation" :
                            continue ;
                        lineToWrite += "  %f  "%(float(columns)*lumi/options.inputLumi);
                        observed = int(float(columns)*lumi/options.inputLumi);
                    fileNew.write(lineToWrite+"\n");
                    continue ;


                if ifile.split(" ")[0] == "rate" :
                    lineToWrite = "rate ";
                    for columns in ifile.split() :
                        if columns == "rate" : 
                            continue ;
                        lineToWrite += "  %f   "%(float(columns)*lumi/options.inputLumi)                        
                    fileNew.write(lineToWrite+"\n");
                    continue ;

            fileNew.close();
                
            ## copy root file
            outrootname = outname.replace(".txt",".root");
            outrootfile = ROOT.TFile("%s"%(outrootname),"RECREATE");
              
            for key in inputrootfile.GetListOfKeys() :
                if key.GetClassName().find("TH1") == -1 and key.GetClassName().find("TH2") == -1 :
                    continue ;
                outrootfile.cd();                  
                histo = inputrootfile.Get(key.GetName()).Clone("temp");

                histo.Scale(1./(options.inputLumi/lumi));
                    
                histo.Write(key.GetName());

            outrootfile.Write();              
            outrootfile.Close();

    ## merge the two datacard set                 
    if options.outputDIR == "" :
        sys.exit("cannot run combine --> outDir to be provided");
    else :
        os.system("mkdir -p "+options.outputDIR);

    ## combine the cards
    combinedCards = [];

    for lumi in luminosity :
        for datacard in createdCards :
            if datacard.find("_%d"%(lumi)) != -1 :
                if datacard.find("_DF") != -1 :
                    combinedCards.append(datacard.replace("_DF","_COMB"));
                if datacard.find("_UU") != -1 :
                    combinedCards.append(datacard.replace("_UU","_COMB"));
                if datacard.find("_EE") != -1 :
                    combinedCards.append(datacard.replace("_EE","_COMB"));
                break ;

    for card in combinedCards :
      print "combineCards.py "+card.replace("_COMB","_EE")+" "+card.replace("_COMB","_UU")+" "+card.replace("_COMB","_DF")+" > "+card;  
      os.system("combineCards.py "+card.replace("_COMB","_EE")+" "+card.replace("_COMB","_UU")+" "+card.replace("_COMB","_DF")+" > "+card);        

    totalCards = createdCards + combinedCards

    for card in totalCards :
        
        outname = card.replace(".txt","");

        if options.makeAsymptotic :
            runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s  -s -1 --expectSignal=%d -t %d --toysNoSystematics"%(outname,card,options.injectSignal,options.nToys);
            os.system(runCmmd);
            os.system("mv higgsCombine* "+options.outputDIR);
            os.system("rm roostat*");
            continue ;

        if options.makeProfileLikelihood :
            runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -t %d --expectSignal=%d -s -1 --toysNoSystematics"%(outname,card,options.nToys,options.injectSignal);     
            os.system(runCmmd);
            os.system("mv higgsCombine* "+options.outputDIR);
            os.system("rm roostat* ");
            continue ;

        if options.makeMaxLielihoodFit :
            runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %d --rMax %d --saveNormalizations --saveWithUncertainties  -n %s -m 100 -d  %s  --robustFit=1 --do95=1 -s -1 -t %d --expectSignal %d --toysNoSystematics"%(options.rMin,options.rMax,outname,card,options.nToys,options.injectSignal);
            os.system(runCmmd);
            os.system("mv higgsCombine* "+options.outputDIR);
            os.system("mv mlfit* "+options.outputDIR);
            os.system("rm roostat* ");
            continue ;
