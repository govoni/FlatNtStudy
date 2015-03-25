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
parser.add_option('--noCombineCards',    action='store_true', dest='noCombineCards', default=False, help='not combine again the cards')
parser.add_option('--queque',         action="store",      type="string",         dest="queque",      default="")

##### other basci options for all the methods 
parser.add_option('--datacardDIR',  action="store", type="string", dest="datacardDIR",  default="")
parser.add_option('--outputDIR',    action="store", type="string", dest="outputDIR",    default="")
parser.add_option('--channel',      action="store", type="string", dest="channel",      default="UUpp")

###### options for Bias test in the combination tool
parser.add_option('--nToys',                 action="store",     type="int",    dest="nToys",                 default=0,  help="number of toys to generate")
parser.add_option('--submitSingleJobs',      action="store_true",               dest="submitSingleJobs",      default=False,  help="one job one toy")

parser.add_option('--inputGeneratedDataset', action="store", type="string", dest="inputGeneratedDataset", default="", help="parse a dataset from generateOnly")
parser.add_option('--injectSignal',          action="store", type=float,    dest="injectSignal",  default=0., help='inject a singal in the toy generation')
parser.add_option('--noSystematics',         action="store", type=int,      dest="noSystematics", default=0,  help='avoid systematics')

###### input set of variables : 
parser.add_option('--inputVariableList', action="store", type="string", dest="inputVariableList", default="list of variables to be considered")
parser.add_option('--is2Dcard',          action="store_true", dest="is2Dcard", default=False)
parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)


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
    if options.channel !="COMB" :
        outScript.write('cp '+currentDir+"/"+fileNames+'* ./ \n');
    else :
        outScript.write('cp '+currentDir+"/"+fileNames+'* ./ \n');
        nametemp = fileNames.replace("COMB","");
        outScript.write('cp '+currentDir+"/"+nametemp+'*UUpp* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*UUmm* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*EEpp* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*EEmm* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*UEpp* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*UEmm* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*EUpp* ./ \n');
        outScript.write('cp '+currentDir+"/"+nametemp+'*EUmm* ./ \n');

    outScript.write(command+'\n');
    
    if options.generateOnly:
        outScript.write("mv higgsCombine* "+options.inputGeneratedDataset+'\n');
    else :
        outScript.write("mv higgsCombine*"+options.channel+"*  "+currentDir+"/"+options.outputDIR+'\n');
        outScript.write("mv mlfit*"+options.channel+"*  "+currentDir+"/"+options.outputDIR+'\n');
    
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

    print "###### start combine analysis ########";

    pwd = os.getcwd();    

    if options.datacardDIR == "":
     sys.exit("provide a datacard directory");

    print "mkdir -p "+options.datacardDIR+"/"+options.outputDIR;
    os.system("mkdir -p "+options.datacardDIR+"/"+options.outputDIR);

    os.system("eval `scramv1 runtime -sh`"); ## to set the right root for combine
    
    rMin = options.rMin ; 
    rMax = options.rMax ;

    ### start  analysis --> loop on the file and make a list of variable
    variable = [];
    datacardList = [];

    file = open('%s'%(options.inputVariableList), 'r');

    if not options.is2Dcard:

        for line in file:
            if line.strip('\n') == '': continue;
            if line[0] == '#' or line[0] == ' ': continue;
            linestring = line.split(" ")[0];
            linestring = linestring.replace("\n","");
            linestring = linestring.replace("\t","");
            variable.append(linestring);

        variable.sort();
        print "##### list of variables ", variable ;
        print "##### number of variables ", len(variable) ;
        os.chdir(options.datacardDIR);

        for var in variable :
            if options.channel == "COMB" :
                os.system("ls  | grep _"+var+"_UUpp | grep txt >  list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UUmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EEpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EEmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UEpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UEmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EUpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EUmm | grep txt >> list.txt"); # make a list of datacards            
            else:
                os.system("ls  | grep _"+var+"_"+options.channel+" | grep txt > list.txt"); # make a list of datacards            
                
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
                        lineString = lineString.replace("_UUpp","");
                        lineString = lineString.replace("_UUmm","");
                        lineString = lineString.replace("_EEpp","");
                        lineString = lineString.replace("_EEmm","");
                        lineString = lineString.replace("_EUpp","");                        
                        lineString = lineString.replace("_EUmm","");                        
                        lineString = lineString.replace("_UEpp","");                        
                        lineString = lineString.replace("_UEmm","");                        
                    else :
                        temp = line.split(" ")[0];
                        temp = temp.replace("_UUpp","");
                        temp = temp.replace("_UUpp","");
                        temp = temp.replace("_EEpp","");
                        temp = temp.replace("_EEmm","");
                        temp = temp.replace("_EUpp","");                        
                        temp = temp.replace("_EUmm","");                        
                        temp = temp.replace("_UEpp","");                        
                        temp = temp.replace("_UEmm","");                        
                        if not temp == lineString :
                            continue ;
                        combineCommand += " "+line.split(" ")[0];

                combineCommand = combineCommand.replace("\n","");
                combinedCard = combineCommand.split(" ")[len(combineCommand.split(" "))-1];
                combinedCard = combinedCard.replace("_UUpp","_COMB")
                combinedCard = combinedCard.replace("_UUmm","_COMB")
                combinedCard = combinedCard.replace("_EEpp","_COMB")
                combinedCard = combinedCard.replace("_EEmm","_COMB")
                combinedCard = combinedCard.replace("_EUpp","_COMB")
                combinedCard = combinedCard.replace("_EUmm","_COMB")
                combinedCard = combinedCard.replace("_UEpp","_COMB")
                combinedCard = combinedCard.replace("_UEmm","_COMB")
                combineCommand += " > " +combinedCard;
                datacardList.append(combinedCard);
                if not options.noCombineCards :
                    print "combine cards: ",combineCommand;
                    os.system(combineCommand);
                
            else:
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel
                    if line.split(" ")[0].find("_"+options.channel) !=-1:
                        if not line.split(" ")[0] in  datacardList :
                            datacard = line.split(" ")[0];
                            datacard = datacard.replace("\n","");
                            datacardList.append(datacard);

        datacardList.sort();
        print datacardList                    
        print "###### datacards to be analyzed ",len(datacardList);        
                        
    else: ## 2D variable 

        iline = 0;
        for columns in ( raw.strip().split() for raw in file ):
            if columns[0].find('#')!=-1 or columns[0] == ' ': continue;               
            if ROOT.TString(options.inputVariableList).Contains("Dynamic"):
                linestring = columns[0]+"_"+columns[4];
            else:
                linestring = columns[0]+"_"+columns[5];

            linestring = linestring.replace("\n","");
            linestring = linestring.replace("\t","");
            variable.append(linestring);

        variable.sort();
        print "##### list of variables ", variable ;
        print "##### number of variables ", len(variable) ;

        os.chdir(options.datacardDIR);        

        for var in variable :
            if options.channel == "COMB" :
                os.system("ls  | grep _"+var+"_UUpp | grep txt >  list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UUmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EEpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EEmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EUpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_EUmm | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UEpp | grep txt >> list.txt"); # make a list of datacards            
                os.system("ls  | grep _"+var+"_UEmm | grep txt >> list.txt"); # make a list of datacards            
            else:
                os.system("ls  | grep _"+var+"_"+options.channel+" | grep txt > list.txt"); # make a list of datacards            

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
                        lineString = lineString.replace("_UUpp","");
                        lineString = lineString.replace("_UUmm","");
                        lineString = lineString.replace("_EEpp","");
                        lineString = lineString.replace("_EEmm","");
                        lineString = lineString.replace("_EUpp","");                        
                        lineString = lineString.replace("_EUmm","");                        
                        lineString = lineString.replace("_UEpp","");                        
                        lineString = lineString.replace("_UEmm","");                        
                    else :
                        temp = line.split(" ")[0];
                        temp = temp.replace("_UUpp","");
                        temp = temp.replace("_UUmm","");
                        temp = temp.replace("_EEpp","");
                        temp = temp.replace("_EEmm","");
                        temp = temp.replace("_EUpp","");                        
                        temp = temp.replace("_EUmm","");                        
                        temp = temp.replace("_UEpp","");                        
                        temp = temp.replace("_UEmm","");                        
                        if not temp == lineString :
                            continue ;
                        combineCommand += " "+line.split(" ")[0];
                       
                combineCommand = combineCommand.replace("\n","");
                combinedCard = combineCommand.split(" ")[len(combineCommand.split(" "))-1];
                combinedCard = combinedCard.replace("_UUpp","_COMB")
                combinedCard = combinedCard.replace("_UUmm","_COMB")
                combinedCard = combinedCard.replace("_EEpp","_COMB")
                combinedCard = combinedCard.replace("_EEmm","_COMB")
                combinedCard = combinedCard.replace("_EUpp","_COMB")
                combinedCard = combinedCard.replace("_EUmm","_COMB")
                combinedCard = combinedCard.replace("_UEpp","_COMB")
                combinedCard = combinedCard.replace("_UEmm","_COMB")
                combineCommand += " > "+ combinedCard;
                datacardList.append(combinedCard);
                if not options.noCombineCards :
                    print "combine cards: ",combineCommand;
                    os.system(combineCommand);
                
            else:
                for line in file : ## loop on these variables, take into account that there should be only 1 card for each variables given a channel
                    if line.split(" ")[0].find("_"+options.channel) !=-1:
                        if not line.split(" ")[0] in  datacardList :
                            datacard = line.split(" ")[0];
                            datacard = datacard.replace("\n","");
                            datacardList.append(datacard);


        datacardList.sort();
        print datacardList ;
        print "###### datacards to be analyzed ",len(datacardList);        


    os.system("rm list.txt")


    for card in datacardList :

       if  options.channel == "UUpp" and card.find("_UUpp") == -1 :
           continue ;
       if  options.channel == "UUmm" and card.find("_UUmm") == -1 :
           continue ;
       if  options.channel == "EEpp" and card.find("_EEpp") == -1 :
           continue ;
       if  options.channel == "EEmm" and card.find("_EEmm") == -1 :
           continue ;
       if  options.channel == "UEpp" and card.find("_UEpp") == -1 :
           continue ;
       if  options.channel == "UEmm" and card.find("_UEmm") == -1 :
           continue ;
       if  options.channel == "EUpp" and card.find("_EUpp") == -1 :
           continue ;
       if  options.channel == "EUmm" and card.find("_EUmm") == -1 :
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

           runCmmd =  "combine -M GenerateOnly --saveToys -s -1 -n %s -m 100 -d %s -t %d --expectSignal=%d --toysNoSystematics "%(outname,card,options.nToys,options.injectSignal);
           print "runCmmd ",runCmmd;
           if options.batchMode:
               fn = "combineScript_generateOnly_%s_nToys_%d"%(outname,options.nToys);
               submitBatchJobCombine( runCmmd,fn,outname);
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
               runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties  -n %s -m 100 -d  %s  --robustFit=1 --saveShapes --do95=1 -s -1 --toysNoSystematics --skipBOnlyFit"%(rMin,rMax,outname,card,rMin,rMax);
               print "runCmmd ",runCmmd;
               if options.batchMode:
                   fn = "combineScript_MaxLikelihoodFit_%s"%(outname);
                   submitBatchJobCombine(runCmmd,fn,outname);
               else:   
                   os.system(runCmmd);
                   os.system("mv higgsCombine*"+options.channel+"*MaxLikelihood* "+options.outputDIR);   
                   os.system("mv mlfit*"+options.channel+"*"+options.outputDIR);   

               continue;

           ######################################################## 
           #### run many toys and fits with the same datacards  ###
           ########################################################

           elif options.nToys != 0  :

               if not options.submitSingleJobs :

                   runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties  --toysNoSystematics --saveShapes --saveToys -s -1 -n %s -m 100 -d %s  -t %d --expectSignal=%d --robustFit=1 --do95=1 --skipBOnlyFit"%(rMin,rMax,outname,card,options.nToys,options.injectSignal);
                   print "runCmmd ",runCmmd;
                   if options.batchMode:
                       fn = "combineScript_MaxLikelihoodFit_%s_nToys_%d"%(outname,options.nToys);
                       submitBatchJobCombine(runCmmd,fn,outname);
                   else: 
                       os.system(runCmmd);
                       os.system("mv higgsCombine*"+options.channel+"*MaxLikelihood* "+options.outputDIR);   
                       os.system("mv mlfit*"+options.channel+"*"+options.outputDIR);   

                   continue ;

               else:

                   for itoy in range(options.nToys):
                       outname += "itoy_%d"%(itoy);
                       runCmmd =  "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties  --saveShapes -toysNoSystematics --saveToys -s -1 -n %s -m 100 -d %s  -t 1 --expectSignal=%d --robustFit=1 --do95=1 --skipBOnlyFit"%(rMin,rMax,outname,card,options.injectSignal);
                       print "runCmmd ",runCmmd;
                       if options.batchMode:
                           fn = "combineScript_MaxLikelihoodFit_%s"%(outname);
                           submitBatchJobCombine(runCmmd,fn,outname);
                       else: 
                           os.system(runCmmd);
                           os.system("mv higgsCombine*"+options.channel+"*MaxLikelihood* "+options.outputDIR);   
                           os.system("mv mlfit*"+options.channel+"*"+options.outputDIR);   

                       continue ;

                      
       elif options.computeAsymptotic == 1 :

           print "################################";
           print "#### Asymptotic Limit part  ####";
           print "################################";

           if options.noSystematics == 1 :
               runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s -S 0 -s -1  -t %d --toysNoSystematics -H ProfileLikelihood "%(outname,card,options.injectSignal,options.nToys);
               print "runCmmd ",runCmmd ;
               if options.batchMode:
                   fn = "combineScript_Asymptotic_%s"%(outname);
                   submitBatchJobCombine(runCmmd,fn,outname);
               else: 
                   os.system(runCmmd);
                   os.system("mv higgsCombine*"+options.channel+"*Asymptotic* "+options.outputDIR);   
               continue;

           elif options.noSystematics == 0 :

               runCmmd = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s -s -1 -t %d --toysNoSystematics -H ProfileLikelihood"%(outname,card,options.injectSignal,options.nToys);                            
               print "runCmmd ",runCmmd;
                   
               if options.batchMode:
                   fn = "combineScript_Asymptotic_%s"%(outname);
                   submitBatchJobCombine(runCmmd,fn,outname);
               else: 
                   os.system(runCmmd);
                   os.system("mv higgsCombine*"+options.channel+"*Asymptotic* "+options.outputDIR);   
               continue;
                           
                      
       elif options.computeProfileLikelihood == 1 : 

           print "##################################################"
           print "###### run the observed and expected pvalue  #####"
           print "##################################################" 
           

           runCmmd = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -t %d --expectSignal=%d -s -1 --toysNoSystematics"%(outname,card,options.nToys,options.injectSignal);
           print "runCmmd ",runCmmd;
                          
           if options.batchMode:
               fn = "combineScript_ProfileLikelihood_exp_%s_iToy_%d"%(outname,options.nToys);
               submitBatchJobCombine(runCmmd,fn,outname);
           else:
               os.system(runCmmd);
               os.system("mv higgsCombine*"+options.channel+"*Profile* "+options.outputDIR);   

       elif options.makeLikelihoodScan == 1:
                        
           runCmmd = "combine -M MultiDimFit -n %s -m 100 -d %s --algo=grid --points=150 --setPhysicsModelParameterRanges r=%f,%f -s -1 --expectSignal=%d --toysNoSystematics -t %d"%(outname,card,rMin,rMax,options.injectSignal,options.nToys);
           print "runCmmd ",runCmmd;
           if options.batchMode:
               fn = "combineScript_LikelihoodScan_%s"%(outname);
               submitBatchJobCombine(runCmmd,fn,outname);
           else:
               os.system(runCmmd);
               os.system("mv higgsCombine*"+options.channel+"*MultiDimFit* "+options.outputDIR);   
                                      

