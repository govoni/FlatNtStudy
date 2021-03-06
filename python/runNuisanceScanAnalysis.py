#!/usr/bin/python
import os
import sys
import subprocess
import operator
import ROOT
from commands import getstatusoutput
from operator import itemgetter
from optparse import OptionParser

############################################                                                                                                                                 
#            Job steering                  #                                                                                                                                   
############################################                                                                                                                                   
 

parser = OptionParser()

parser.add_option('-b', action='store_true', dest='noX', default=False, help='no X11 windows')

parser.add_option('--datacardDIR', action="store",      type="string", dest="datacardDIR",  default="")
parser.add_option('--outputDIR',   action="store",      type="string", dest="outputDIR",  default="")

parser.add_option('--submitJobs',  action="store_true", dest="submitJobs",  default=0)
parser.add_option('--queque',         action="store",      type="string",         dest="queque",      default="")


parser.add_option('--computeAsymptotic',        action='store_true', dest='computeAsymptotic',        default=False, help='run asymptotic limits')
parser.add_option('--computeProfileLikelihood', action='store_true', dest='computeProfileLikelihood', default=False, help='run profile likelihood')
parser.add_option('--maximumLikelihoodFit',     action='store_true', dest='maximumLikelihoodFit',     default=False, help='run max Likelihood fit inside combination tool')

parser.add_option('--channel',      action="store", type="string", dest="channel",      default="UUpp")
parser.add_option('--injectSignal',          action="store", type=float,    dest="injectSignal",  default=0., help='inject a singal in the toy generation')

parser.add_option('--rMin',          action="store", type=float, dest="rMin", default=0)
parser.add_option('--rMax',          action="store", type=float, dest="rMax", default=10)


(options, args) = parser.parse_args()

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
def runLimitCalc (filename,isNoSys=False) :

    if options.maximumLikelihoodFit :
        filenameBash   = "combineScript_"+filename.replace(".txt","")+"_MaxLikelihood.sh"
    elif options.computeProfileLikelihood:
        filenameBash   = "combineScript_"+filename.replace(".txt","")+"_ProfileLikelihood.sh"
    elif options.computeAsymptotic:
        filenameBash   = "combineScript_"+filename.replace(".txt","")+"_esymptotic.sh"
        
    outputname = filename.replace(".txt","");

    if options.maximumLikelihoodFit and not isNoSys:
        command = "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties --saveShapes --toysNoSystematics --saveToys -s -1 -n %s -m 100 -d %s  -t -1 --expectSignal=%d --robustFit=1 --skipBOnlyFit"%(options.rMin,options.rMax,outputname,filename,options.injectSignal);
    elif options.maximumLikelihoodFit and isNoSys:
        command = "combine -M MaxLikelihoodFit --minimizerAlgo Minuit2 --minimizerStrategy 1 --rMin %f --rMax %f --saveNormalizations --saveWithUncertainties --saveShapes --toysNoSystematics --saveToys -s -1 -n %s -m 100 -d %s  -t -1 --expectSignal=%d  --skipBOnlyFit"%(options.rMin,options.rMax,outputname,filename,options.injectSignal);
    elif options.computeAsymptotic :
        command = "combine -M Asymptotic --minimizerAlgo Minuit2 --minosAlgo stepping -n %s -m 100 -d %s -S 0 -s -1  --expectSignal=%d -t -1 --toysNoSystematics -H ProfileLikelihood  "%(outputname,filename,options.injectSignal);
    elif computeProfileLikelihood :
        command = "combine -M ProfileLikelihood --signif  -n %s -m 100 -d %s -t -1 --expectSignal=%d -s -1 --toysNoSystematics "%(outputname,filename,options.injectSignal);
    else : 
        return;

    f = open (filenameBash, 'w');
    f.write ("#/bash/sh \n");
    f.write ("cd "+os.getcwd()+"\n");
    f.write ("eval `scramv1 runtime -sh` \n");    
    f.write ("cd - \n");
    f.write ("cp  "+os.getcwd()+"/"+filename+" ./ \n");
    if options.channel != "COMB":
        f.write ("cp  "+os.getcwd()+"/*"+options.channel+"*root./ \n");
    else :
        f.write ("cp  "+os.getcwd()+"/*UUpp*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*UUmm*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*EEpp*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*EEmm*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*UEpp*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*UEmm*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*EUpp*root ./ \n");
        f.write ("cp  "+os.getcwd()+"/*EUmm*root ./ \n");
    f.write (command+"\n");
    f.write ("cp higgsCombine* "+os.getcwd()+"/"+options.outputDIR+"\n");
    f.write ("cp mlfit* "+os.getcwd()+"/"+options.outputDIR+"\n");
    f.close ();

    os.system("chmod +x "+filenameBash);

    if options.submitJobs :
        if options.queque!="" :
            os.system("bsub -q "+options.queque+" -o "+os.getcwd()+"/subJob"+filename.replace(".txt","")+".log -e "+os.getcwd()+"/subJob"+filename.replace(".txt","")+".err "+filenameBash);
        else:
            os.system("bsub -q 1nh -o "+os.getcwd()+"/subJob"+filename.replace(".txt","")+".log -e "+os.getcwd()+"/subJob"+filename.replace(".txt","")+".err "+filenameBash);


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
def clean (basename) :
    getstatusoutput('rm ./' + basename + '*')


# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
def lookAtSystematics (datacardname) :

# open the datacard file
# ---- ---- ---- ---- ---- ---- ---- ----

    print 'Opening original input datacard: ', datacardname 
    lines = open (datacardname, 'r').read().split ('\n')
    nametag = datacardname.split ('/')[-1].replace ('.txt', '')
    thepath = datacardname.replace (nametag + '.txt', '')

    print "nametag="+nametag
    print "thepath="+thepath

# separate header and systematics
# ---- ---- ---- ---- ---- ---- ---- ----
    
    ROOT.gStyle.SetGridStyle (1)
    ROOT.gStyle.SetGridColor (15)
    systime = 0
    header = []
    systematics = []
    for linea in lines:
        if '---' in linea : continue
        if systime == 0 :
            header.append (linea)
            if linea.split (' ')[0] == 'rate' :
                systime = 1
        else:
            systematics.append (linea)

    systematics = [elem for elem in systematics if len (elem.split ()) > 0]
# run the actual result with the original datacard
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    
    runLimitCalc (datacardname)

# remove, one at a time, one systematic, and run the limit 
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
    
    syslist = []
    syslist.append ('NOMINAL')

    for it in range (len (systematics)) :
        elements = systematics[it].split ()
        syslist.append (systematics[it].split ()[0])
        if len (elements) == 0 : continue

        filename = thepath + nametag + '_remove_' + systematics[it].split ()[0] + ".txt"
        f = open(filename, 'w')
        for linea in header: 
            if linea.split(" ")[0] == "kmax" :
                f.write("kmax * number of nuisance parameters \n");
            else :
                f.write (linea + '\n')
        for it1 in range (len (systematics)) :
            if (it1 == it) : continue
            if len (systematics[it1].split ()) == 0 : continue
            f.write (systematics[it1] + '\n')
        f.close ()
        
        runLimitCalc (filename)

# run the limit with no systematics
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    filename = thepath + nametag + '_noSys.txt'
    f = open(filename, 'w')
    for linea in header: 
        if linea.split(" ")[0] == "kmax" :
            f.write("kmax * number of nuisance parameters \n");
        else :
            f.write (linea + '\n')
    f.close ()
    runLimitCalc (filename,True)

# add, one at a time, only one systematic source, and run the limit
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

    for it in range (len (systematics)) :
        elements = systematics[it].split ()
        if len (elements) == 0 : continue

        filename = thepath + nametag + '_add_' + systematics[it].split ()[0] + ".txt"
        f = open(filename, 'w')
        for linea in header: 
            if linea.split(" ")[0] == "kmax" :
                f.write("kmax * number of nuisance parameters \n");
            else :
                f.write (linea + '\n')
        f.write (systematics[it] + '\n')
        f.close ()

        runLimitCalc (filename)
    
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

if __name__ == '__main__':

    if options.datacardDIR == "" : 
        print 'input datacard folder missing\n'
        exit (1)

    os.chdir(options.datacardDIR);

    os.system("mkdir -p "+options.outputDIR);
    os.system("rm -r "+options.outputDIR+"/*");
    os.system("rm -r combineScript*");
    os.system("rm -r subJob*");
    os.system("rm -r *_remove_*txt");
    os.system("rm -r *_add_*txt");
    os.system("rm -r *_noSys*txt");

    listOfDatacards = []
    for elem in getstatusoutput ("ls | grep txt | grep %s"%options.channel)[1].split ('\n'):
        listOfDatacards.append (str(elem))

    print listOfDatacards;

    for datacard in listOfDatacards :
        lookAtSystematics (datacard)



