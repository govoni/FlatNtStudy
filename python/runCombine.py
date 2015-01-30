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
from ROOT import gROOT, gStyle, gSystem
from collections import defaultdict

############################################
#            Job steering                  #
############################################

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

#### basic allowed methods
parser.add_option('--computeAsymptotic',        action='store_true', dest='computeAsymptotic',        default=False, help='run asymptotic limits')
parser.add_option('--computeProfileLikelihood', action='store_true', dest='computeProfileLikelihood', default=False, help='run profile likelihood')
parser.add_option('--maximumLikelihoodFit',     action='store_true', dest='maximumLikelihoodFit',     default=False, help='run max Likelihood fit inside combination tool')
parser.add_option('--generateOnly',             action='store_true', dest='generateOnly',       default=False, help='only generation with combiner')
parser.add_option('--makeLikelihoodScan',       action='store_true', dest='makeLikelihoodScan', default=False, help='run the likelihood scan')

##### submit jobs to condor, lxbatch and hercules 
parser.add_option('--batchMode',      action='store_true', dest='batchMode',      default=False, help='to run jobs on condor fnal')
parser.add_option('--lxbatchCern',    action='store_true', dest='lxbatchCern',    default=False, help='run jobs on lxbatch system at cern, default is condor fnal')
parser.add_option('--queque',         action="store",      type="string",         dest="queque",      default="")

##### other basci options for all the methods 
parser.add_option('--datacardDIR',  action="store", type="string", dest="datacardDIR",  default="")
parser.add_option('--outputDIR',    action="store", type="string", dest="outputDIR",    default="")
parser.add_option('--channel',      action="store", type="string", dest="channel",      default="UU")

###### options for Bias test in the combination tool
parser.add_option('--nToys',                 action="store", type="int",    dest="nToys",                 default=0,  help="number of toys to generate")
parser.add_option('--inputGeneratedDataset', action="store", type="string", dest="inputGeneratedDataset", default="", help="parse a dataset from generateOnly")
parser.add_option('--outputTree',            action="store", type="int",    dest="outputTree",            default=0,  help="one toy per file or more toy in a file")
parser.add_option('--injectSignal',          action="store", type=float,    dest="injectSignal",  default=0., help='inject a singal in the toy generation')
parser.add_option('--noSystematics',         action="store", type=int,      dest="noSystematics", default=0,  help='avoid systematics')

###### input set of variables : 
parser.add_option('--inputVariableList', action="store", type="string", dest="inputVariableList", default="list of variables to be considered")
parser.add_option('--is2Dcard',          action="store_true", dest="is2Dcard", default=False)


(options, args) = parser.parse_args()


##########################################
###### Submit batch job for combine ######
##########################################

def submitBatchJobCombine( command, fn):
        
    currentDir = os.getcwd();
    
    # create a dummy bash/csh
    outScript = open(fn+".sh","w");

    outScript.write('#!/bin/bash \n');
    outScript.write('cd '+currentDir+'\n');
    outScript.write('eval `scram runtime -sh`'+'\n');
    outScript.write(command+'\n');
    
    if options.generateOnly:
        outScript.write("mv higgsCombine* "+options.inputGeneratedDataset+'\n');
    else :
        outScript.write("mv higgsCombine* "+options.outputDIR+'\n');
        outScript.write("mv mlfit* "+options.outputDIR+'\n');
    
    outScript.write("rm rootstats* "+'\n');
    outScript.close();

    os.system("chmod 777 "+currentDir+"/"+fn+".sh");

    if options.queque!="" :
        os.system("bsub -q "+options.queque+" -cwd "+os.getcwd()+" "+fn+".sh");
    else: 
        os.system("bsub -q 8nh -cwd "+os.getcwd()+" "+fn+".sh");


##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    print "###### start combine analysis ########";


    if options.datacardDIR == "":
     sys.exit("provide a datacard directory");

    print "mkdir -p "+options.outputDIR;
    os.system("mkdir -p "+options.datacardDIR+"/"+options.outputDIR);

    
    if options.injectSignal == 0 : 
        rMin = 0 ; 
        rMax = 10 ;
    else: 
        rMin = 0 ; 
        rMax = 10*options.injectSignal;


    ### start  analysis --> loop on the file and make a list of variable
    variable = [];
    datacardList = [];

    file = open('%s'%(options.inputVariableList), 'r');
    
    if not options.is2Dcard:

        for line in file:
            if line.strip('\n') == '': continue;
            if line[0] == '#' or line[0] == ' ': continue;
            variable.append(line.split(" ")[0]);

        print "##### list of variables ", variable ;
        print "##### number of variables ", len(variable) ;
        
        for var in variable :
            os.system("ls "+options.datacardDIR+" | grep _"+var+"_ | grep txt > list.txt"); # make a list of datacards            
            file = open("list.txt", 'r');                
            combineCommand = "combineCards.py ";

            if options.channel == "COMB" :
                iline = 0;
                lineString = "";
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel         
                    if iline == 0 :                       
                        combineCommand += " "+line.split(" ")[0];
                        iline = iline+1;
                        lineString = str(line.split(" ")[0]);
                        lineString = lineString.replace("_UU","");
                        lineString = lineString.replace("_EE","");
                        lineString = lineString.replace("_DF","");                        
                    else :
                        temp = line.split(" ")[0];
                        temp = temp.replace("_UU","");
                        temp = temp.replace("_EE","");
                        temp = temp.replace("_DF","");                        
                        if not temp == lineString :
                            continue ;
                        combineCommand += " "+line.split(" ")[0];
                       
                combineCommand = combineCommand.replace("\n","");
                combinedCard = combineCommand.split(" ")[len(combineCommand.split(" "))-1];
                combinedCard = combinedCard.replace("_UU","_COMB")
                combinedCard = combinedCard.replace("_EE","_COMB")
                combinedCard = combinedCard.replace("_DF","_COMB")
                combineCommand += " > "+ combinedCard;
                datacardList.append(combinedCard);
                print "combine cards: ",combineCommand;
                os.system(combineCommand);
                
            else:
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel
                    if line.split(" ")[0].find("_"+options.channel) !=-1:
                        if not line.split(" ")[0] in  datacardList :
                            datacard = line.split(" ")[0];
                            datacard = datacard.replace("\n","");
                            datacardList.append(datacard);

        print "###### datacards to be analyzed ",len(datacardList);        
                        
    else: ## 2D variable 

        iline = 0;
        for columns in ( raw.strip().split() for raw in file ):
            if columns[0].find('#')!=-1 or columns[0] == ' ': continue;               
            variable.append(columns[0]+"_"+columns[5]);

        print "##### list of variables ", variable ;
        print "##### number of variables ", len(variable) ;
        
        for var in variable :
            os.system("ls "+options.datacardDIR+" | grep _"+var+"_ | grep txt > list.txt"); # make a list of datacards            
            file = open("list.txt", 'r');                
            combineCommand = "combineCards.py ";

            if options.channel == "COMB" :
                iline = 0;
                lineString = "";
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel         
                    if iline == 0 :                       
                        combineCommand += " "+line.split(" ")[0];
                        iline = iline+1;
                        lineString = str(line.split(" ")[0]);
                        lineString = lineString.replace("_UU","");
                        lineString = lineString.replace("_EE","");
                        lineString = lineString.replace("_DF","");                        
                    else :
                        temp = line.split(" ")[0];
                        temp = temp.replace("_UU","");
                        temp = temp.replace("_EE","");
                        temp = temp.replace("_DF","");                        
                        if not temp == lineString :
                            continue ;
                        combineCommand += " "+line.split(" ")[0];
                       
                combineCommand = combineCommand.replace("\n","");
                combinedCard = combineCommand.split(" ")[len(combineCommand.split(" "))-1];
                combinedCard = combinedCard.replace("_UU","_COMB")
                combinedCard = combinedCard.replace("_EE","_COMB")
                combinedCard = combinedCard.replace("_DF","_COMB")
                combineCommand += " > "+ combinedCard;
                datacardList.append(combinedCard);
                print "combine cards: ",combineCommand;
                os.system(combineCommand);
                
            else:
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel
                    if line.split(" ")[0].find("_"+options.channel) !=-1:
                        if not line.split(" ")[0] in  datacardList :
                            datacard = line.split(" ")[0];
                            datacard = datacard.replace("\n","");
                            datacardList.append(datacard);


        print "###### datacards to be analyzed ",len(datacardList);        


    os.system("rm list.txt")

    print "chdir ",options.datacardDIR;
    os.chdir(options.datacardDIR);

    for card in datacardList :

       if  options.channel == "UU" and card.find("_UU") == -1 :
           continue ;
       if  options.channel == "EE" and card.find("_EE") == -1 :
           continue ;
       if  options.channel == "DF" and card.find("_DF") == -1 :
           continue ;
       if  options.channel == "COMB" and card.find("_COMB") == -1 :
           continue ;

       print "card to analyze",card

       ## generate only options
       outname = card.replace(".txt","");

       if options.generateOnly:

           print "######################################"
           print "####### Generate Only ################"
           print "######################################"

           os.system("mkdir -p "+options.inputGeneratedDataset); # make the output directory

           if options.outputTree == 0 : ## make a job for each single toy
               for iToy in range(options.nToys):                              
                   runCmmd =  "combine -M GenerateOnly --saveToys -s -1 -n %s  -m 100 -d %s -v 2 -t 1 --expectSignal=%d "%(outname,card,options.injectSignal);
                   print "runCmmd ",runCmmd;
                   if options.batchMode:
                       fn = "combineScript_%s_iToy_%d"%(outname,iToy);
                       submitBatchJobCombine(runCmmd,fn,options.inputGeneratedDataset);
                   else: 
                       os.system(runCmmd);
                       os.system("mv higgsCombine* "+options.inputGeneratedDataset);   
               continue ;

           else:
               runCmmd =  "combine -M GenerateOnly --saveToys -s -1 -n %s -m 100 -d %s -v 2 -t %d --expectSignal=%d "%(outname,card,options.nToys,options.injectSignal);
               print "runCmmd ",runCmmd;
               if options.batchMode:
                   fn = "combineScript_%s_nToys_%d"%(outname,options.nToys);
                   submitBatchJobCombine( runCmmd,fn,options.inputGeneratedDataset);
               else: 
                   os.system(runCmmd);
                   os.system("mv higgsCombine* "+options.inputGeneratedDataset);   
               continue ;

       elif options.maximumLikelihoodFit == 1:
                        

           print"######################################";
           print"####### Maximum Likelihood Fits ######";
           print"######################################";   
    
          ################################################# 
          #### just one fit with the defined datacards ####
          #################################################
                        
           if options.nToys == 0 : 
               runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %d --rMax %d --saveNormalizations --saveWithUncertainties  -n %s -m 100 -d  %s -v 2  --robustFit=1 --do95=1 -s -1"%(rMin,rMax,outname,card);
               print "runCmmd ",runCmmd;
               if options.batchMode:
                   fn = "combineScript_%s"%(outname);
                   submitBatchJobCombine(runCmmd,fn);
               else:   
                   os.system(runCmmd);
                   os.system("mv higgsCombine* "+options.outputDIR);   
                   os.system("mv mlfit* "+options.outputDIR);   

               continue;

           ######################################################## 
           #### run many toys and fits with the same datacards  ###
           ########################################################

           elif options.nToys != 0  :
               if options.outputTree == 0:  
                   for iToy in range(options.nToys):
                       runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %d --rMax %d --saveNormalizations --saveToys --saveWithUncertainties --toysNoSystematics -s -1 -n  %s -m 100 -d %s -v 2 -t 1 --expectSignal=%d --robustFit=1 --do95=1"%(rMin,rMax,outname,card,options.injectSignal);
                       print "runCmmd ",runCmmd;
                       if options.batchMode:
                           fn = "combineScript_%s_iToy_%d"%(outname,iToy);
                           submitBatchJobCombine( runCmmd, fn, );
                       else: 
                           os.system(runCmmd);
                           os.system("mv higgsCombine* "+options.outputDIR);   
                           os.system("mv mlfit* "+options.outputDIR);   
                       continue ;
               else:
                   runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %d --rMax %d --saveNormalizations --saveWithUncertainties  --toysNoSystematics --saveToys -s -1 -n %s -m 100 -d %s -v 2 -t %d --expectSignal=%d --robustFit=1 --do95=1"%(rMin,rMax,outname,card,options.nToys,options.injectSignal);
                   print "runCmmd ",runCmmd;
                   if options.batchMode:
                       fn = "combineScript_%s_nToys_%d"%(outname,options.nToys);
                       submitBatchJobCombine(runCmmd,fn);
                   else: 
                       os.system(runCmmd);
                       os.system("mv higgsCombine* "+options.outputDIR);   
                       os.system("mv mlfit* "+options.outputDIR);   

                   continue ;

                      
       elif options.computeAsymptotic == 1 :

           print "################################";
           print "#### Asymptotic Limit part  ####";
           print "################################";

           if options.noSystematics == 1 :
               runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s -v 2 -S 0 -s -1"%(outname,card);
               print "runCmmd ",runCmmd ;
               if options.batchMode:
                   fn = "combineScript_%s"%(outname);
                   submitBatchJobCombine(runCmmd,fn);
               else: 
                   os.system(runCmmd);
                   os.system("mv higgsCombine* "+options.outputDIR);   
               continue;

           elif options.noSystematics == 0 :
               if options.nToys == 0: 
                   runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s -v 2 -s -1"%(outname,card);                            
                   print "runCmmd ",runCmmd;
                   
                   if options.batchMode:
                       fn = "combineScript_%s"%(outname);
                       submitBatchJobCombine(runCmmd,fn);
                   else: 
                       os.system(runCmmd);
                       os.system("mv higgsCombine* "+options.outputDIR);   
                   
                   continue;
                           
                      
       elif options.computeProfileLikelihood == 1 : 

           print "##################################################"
           print "###### run the observed and expected pvalue  #####"
           print "##################################################" 
           
           runCmmd = "combine -M ProfileLikelihood --signif --pvalue -n %s -m 100 -d %s -v 2"%(outname,card);
           print "runCmmd ",runCmmd;
           
           if options.batchMode:
               fn = "combineScript_ProfileLikelihood_obs_%s"%(outname);
               submitBatchJobCombine(runCmmd,fn);
           else:
               os.system(runCmmd);
               os.system("mv higgsCombine* "+options.outputDIR);   
               os.system("mv higgsCombine* "+options.outputDIR);   

           if options.outputTree == 0:
               for iToy in range(options.nToys):
                   runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -v 2 --toysFreq -t 1 --expectSignal=%d -s -1 "%(outname,card,options.injectSignal);
                   print "runCmmd ",runCmmd;
                          
                   if options.batchMode:
                       fn = "combineScript_ProfileLikelihood_exp_%s_iToy_%d"%(outname,iToy);
                       submitBatchJobCombine(runCmmd, fn);
                   else:
                       os.system(runCmmd);
                       os.system("mv higgsCombine* "+options.outputDIR);   
           else:

               runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -v 2 --toysFreq -t %d --expectSignal=%d -s -1 "%(outname,card,options.nToys,options.injectSignal);
               print "runCmmd ",runCmmd;
                          
               if options.batchMode:
                   fn = "combineScript_ProfileLikelihood_exp_%s_iToy_%d"%(outname,iToy);
                   submitBatchJobCombine(runCmmd, fn);
               else:
                   os.system(runCmmd);
                   os.system("mv higgsCombine* "+options.outputDIR);   

       elif options.makeLikelihoodScan == 1:
                        
           runCmmd = "combine -M MultiDimFit -n %s -m 100 -d %s --algo=grid --points=150 --setPhysicsModelParameterRanges r=%d,%d -s -1 "%(outname,card,rMin,rMax);
           print "runCmmd ",runCmmd;
           if options.batchMode:
               fn = "combineScript_LikelihoodScan_%s"%(outname);
               submitBatchJobCombine(runCmmd, fn);
           else:
               os.system(runCmmd);
               os.system("mv higgsCombine* "+options.outputDIR);   
                                      
