#! /usr/bin/env python
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
parser.add_option('--datacardDIR',   action="store", type="string", dest="datacardDIR",    default="")
parser.add_option('--inputVariable', action="store", type="string", dest="inputVariable",  default="")
parser.add_option('--inputLumi',     action="store", type="float",  dest="inputLumi",      default=3000)
parser.add_option('--outputDIR',     action="store", type="string",  dest="outputDIR",     default="")

parser.add_option('--batchMode',      action='store_true', dest='batchMode',      default=False, help='to run jobs on condor fnal')
parser.add_option('--queque',         action="store",      type="string",         dest="queque",      default="")

parser.add_option('--noGenerateCards',    action='store_true', dest='noGenerateCards', default=False, help='not generate again the cards')

parser.add_option('--makeAsymptotic',  action="store_true", dest="makeAsymptotic",        default=0)
parser.add_option('--makeProfileLikelihood',  action="store_true", dest="makeProfileLikelihood", default=0)
parser.add_option('--makeMaxLikelihoodFit',   action="store_true", dest="makeMaxLikelihoodFit",  default=0)

parser.add_option('--injectSignal',          action="store", type=float,    dest="injectSignal",  default=0., help='inject a singal in the toy generation')
parser.add_option('--nToys',                 action="store", type="int",    dest="nToys",         default=0,  help="number of toys to generate")
parser.add_option('--bruteForce',            action="store", type="int",    dest="bruteForce",    default=0,  help="use brute force for profile Likelihood")

parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)

luminosity     = [20,50,100,150,250,500,750,1200,1800,2400,3000];

(options, args) = parser.parse_args()

##########################################                                                                                                                                      
###### Submit batch job for combine ######                                                                                                                                      
##########################################                                                                                                                                     

def submitBatchJobCombine(command, fn, fileNames):

    currentDir = os.getcwd();

    # create a dummy bash/csh                                                                                                                                                   
    outScript = open(fn+".sh","w");

    outScript.write('#!/bin/bash \n');
    outScript.write('cd '+currentDir+'\n');
    outScript.write('eval `scram runtime -sh`'+'\n');
    outScript.write('cd - \n');
    if fileNames.find("COMB") == -1 :
        outScript.write('cp '+currentDir+"/"+fileNames+'* ./ \n');
    else :
        outScript.write('cp '+currentDir+"/"+fileNames+'* ./ \n');
        nametemp = fileNames.replace("COMB","UUpp");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","UUmm");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","EEpp");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","EEmm");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","EUpp");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","EUmm");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","UEpp");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');
        nametemp = fileNames.replace("COMB","UEmm");
        outScript.write('cp '+currentDir+"/"+nametemp+'* ./ \n');

    outScript.write(command+'\n');

    outScript.write("cp higgsCombine*"+fileNames+"* "+currentDir+"/"+options.outputDIR+'\n');
    outScript.write("cp mlfit*"+fileNames+"* "+currentDir+"/"+options.outputDIR+'\n');
    
    outScript.write("rm rootstats* "+'\n');
    outScript.close();

    os.system("chmod 777 "+currentDir+"/"+fn+".sh");

    if options.queque!="" :
        os.system("bsub -q "+options.queque+" -o "+currentDir+"/subJob"+fileNames+".log -e "+currentDir+"/subJob"+fileNames+".err "+fn+".sh");
    else:
        os.system("bsub -q 1nh -o "+currentDir+"/subJob"+fileNames+".log -e "+currentDir+"/subJob"+fileNames+".err "+fn+".sh");
        

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    print "###### start extrapolate analysis ########";

    if options.datacardDIR == "":
     sys.exit("provide a datacard directory");

    os.chdir(options.datacardDIR);

    if not options.noGenerateCards :
        for lumi in luminosity :
            os.system("rm *_%d.txt"%(lumi))
            os.system("rm *_%d.root"%(lumi))
    
    ## make the card list
    os.system("ls | grep txt | grep -v COMB | grep _UUpp.txt | grep "+options.inputVariable+" > list.txt");
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

    ## lumi on datacard list
    if not options.noGenerateCards :
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

                    histo.Scale(1./(float(options.inputLumi)/float(lumi)));
                    histo.Write(key.GetName());

                outrootfile.Write();              
                outrootfile.Close();    
    else:
        for datacard in datacardList :
            for lumi in luminosity :
                outname = datacard ;
                outname = datacard.replace(".txt","_%d.txt"%(lumi));
                createdCards.append(outname);


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
                if datacard.find("_UEpp") != -1 :
                    combinedCards.append(datacard.replace("_UEpp","_COMB"));
                if datacard.find("_UEmm") != -1 :
                    combinedCards.append(datacard.replace("_UEmm","_COMB"));
                if datacard.find("_EUpp") != -1 :
                    combinedCards.append(datacard.replace("_EUpp","_COMB"));
                if datacard.find("_EUmm") != -1 :
                    combinedCards.append(datacard.replace("_EUmm","_COMB"));
                if datacard.find("_EEpp") != -1 :
                    combinedCards.append(datacard.replace("_EEpp","_COMB"));
                if datacard.find("_EEmm") != -1 :
                    combinedCards.append(datacard.replace("_EEmm","_COMB"));
                if datacard.find("_UUpp") != -1 :
                    combinedCards.append(datacard.replace("_UUpp","_COMB"));
                if datacard.find("_UUmm") != -1 :
                    combinedCards.append(datacard.replace("_UUmm","_COMB"));

                break ;

    if not options.noGenerateCards :
        for card in combinedCards :
            print "combineCards.py "+card.replace("_COMB","_EEpp")+" "+card.replace("_COMB","_UUpp")+" "+card.replace("_COMB","_EUpp")+" "+card.replace("_COMB","_UEpp")+" "+card.replace("_COMB","_EEmm")+" "+card.replace("_COMB","_UUmm")+" "+card.replace("_COMB","_EUmm")+" "+card.replace("_COMB","_UEmm")+" > "+card;  
            os.system("combineCards.py "+card.replace("_COMB","_EEpp")+" "+card.replace("_COMB","_UUpp")+" "+card.replace("_COMB","_EUpp")+" "+card.replace("_COMB","_UEpp")+" "+card.replace("_COMB","_EEmm")+" "+card.replace("_COMB","_UUmm")+" "+card.replace("_COMB","_EUmm")+" "+card.replace("_COMB","_UEmm")+" > "+card);        

    totalCards = createdCards + combinedCards

    for card in totalCards :
        
        outname = card.replace(".txt","");

        if options.makeAsymptotic :
            runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s  -s -1 --expectSignal=%d -t %d --toysNoSystematics"%(outname,card,options.injectSignal,options.nToys);
            print runCmmd ;
            if options.batchMode:
                fn = "combineScript_Asymptotic_%s"%(outname);
                submitBatchJobCombine(runCmmd,fn,outname);
            else :    
                os.system(runCmmd);
                os.system("mv higgsCombine* "+options.outputDIR);
                os.system("rm roostat*");
            continue ;

        if options.makeProfileLikelihood :
            if options.bruteForce == 0 :
                runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -t %d --expectSignal=%d -s -1 --toysNoSystematics"%(outname,card,options.nToys,options.injectSignal);     
            else:
                 runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -t %d --expectSignal=%d -s -1 --toysNoSystematics --bruteForce"%(outname,card,options.nToys,options.injectSignal);     
   
            print "runCmmd ",runCmmd;
            if options.batchMode:
                fn = "combineScript_ProfileLikelihood_exp_%s_iToy_%d"%(outname,options.nToys);
                submitBatchJobCombine(runCmmd,fn,outname);
            else:
                os.system(runCmmd);
                os.system("mv higgsCombine* "+options.outputDIR);
                os.system("rm roostat* ");
            continue ;

        if options.makeMaxLikelihoodFit :
            runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties  -n %s -m 100 -d  %s  --robustFit=1 --do95=1 -s -1 -t %d --expectSignal %d --toysNoSystematics --skipBOnlyFit"%(options.rMin,options.rMax,outname,card,options.nToys,options.injectSignal);
            print runCmmd ;
            if options.batchMode:
                fn = "combineScript_MaxLikelihoodFit_%s_nToys_%d"%(outname,options.nToys);
                submitBatchJobCombine(runCmmd,fn,outname);
            else:
                os.system(runCmmd);
                os.system("mv higgsCombine* "+options.outputDIR);
                os.system("mv mlfit* "+options.outputDIR);
                os.system("rm roostat* ");
            continue ;

