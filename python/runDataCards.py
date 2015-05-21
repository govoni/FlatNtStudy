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

##### submit jobs to condor, lxbatch and hercules 
parser.add_option('--submit',      action='store_true', dest='submit',      default=False, help='to run jobs on condor fnal')
parser.add_option('--cleanJobDir', action='store_true', dest='cleanJobDir', default=False, help='to run jobs on condor fnal')
parser.add_option('--queque',      action="store",      type="string",      dest="queque",      default="")

##### other basci options for all the methods 
parser.add_option('--configfile',   action="store", type="string", dest="configfile",   default="")
parser.add_option('--executable',   action="store", type="string", dest="executable",   default="")
parser.add_option('--jobDir',       action="store", type="string", dest="jobDir",       default="")
parser.add_option('--jobName',      action="store", type="string", dest="jobName",      default="")

(options, args) = parser.parse_args()

##################################
########### Main Code ############
##################################    

if __name__ == '__main__':

    print "###### start make datacard submission analysis ########";
    currentDir = os.getcwd();

    print "mkdir -p "+options.jobDir;
    os.system("mkdir -p "+options.jobDir);

    if options.cleanJobDir :
        os.system("rm "+options.jobDir+"/*");

    jobScriptName = options.jobName+".sh";
    jobLog        = options.jobName+".log";
    jobErr        = options.jobName+".err";

    # create a dummy bash/csh
    outScript = open(options.jobDir+"/"+jobScriptName,"w");

    outScript.write('#!/bin/bash \n');
    outScript.write('cd '+currentDir+" \n");
    outScript.write('eval `scram runtime -sh`'+'\n');
    outScript.write('source scripts/setup.sh '+'\n');    
    command = "./"+options.executable+" "+options.configfile+" \n";
    outScript.write(command+'\n');
    outScript.close();

    os.system("chmod 777 "+currentDir+"/"+options.jobDir+"/"+jobScriptName);

    if options.queque!="" :
        print "bsub -q "+options.queque+" -o "+currentDir+"/"+options.jobDir+"/"+jobLog+" -e "+currentDir+"/"+options.jobDir+"/"+jobErr+" "+currentDir+"/"+options.jobDir+"/"+jobScriptName
        if options.submit :
            os.system("bsub -q "+options.queque+" -o "+currentDir+"/"+options.jobDir+"/"+jobLog+" -e "+currentDir+"/"+options.jobDir+"/"+jobErr+" "+currentDir+"/"+options.jobDir+"/"+jobScriptName);
    else: 
        print "bsub -q 1nd  -o "+currentDir+"/"+options.jobDir+"/"+jobLog+" -e "+currentDir+"/"+options.jobDir+"/"+jobErr+" "+currentDir+"/"+options.jobDir+"/"+jobScriptName
        if options.submit :
            os.system("bsub -q 1nd -o "+currentDir+"/"+options.jobDir+"/"+jobLog+" -e "+currentDir+"/"+options.jobDir+"/"+jobErr+" "+currentDir+"/"+options.jobDir+"/"+jobScriptName);
        
