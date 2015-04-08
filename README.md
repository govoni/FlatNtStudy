===========
FlatNtStudy
===========

1) Install:

    git clone git@github.com:govoni/FlatNtStudy.git ;
    cd FlatNtStudy ;

   Install with combine :
   
    cmsrel CMSSW_7_1_5
    cd CMSSW_7_1_5/src/
    cmsenv
    git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
    cd HiggsAnalysis/CombinedLimit
    git fetch origin
    git checkout v5.0.1
    cd ../../
    git clone https://github.com/SPRACE/CMS-CombineEFTAnalysis.git CombineEFT
    source CombineEFT/CommonTools/setup/patchToSource.sh
    git apply /afs/cern.ch/user/j/jlauwers/public/TP/EFT.diff
    scramv1 b clean; scramv1 b
    cd ../../
    git clone git@github.com:govoni/FlatNtStudy.git ;
    cd FlatNtStudy ;

2) Compile:

    source scripts/setup.sh;
    make -j ;
    make exe -j ;

3) Run :

    .exe file are in the bin directory
  

4) Code to make comparison between LHE files of Phantom and MG:

    test/DrawLHEMGvsPH.cpp
 
   this is how to run it:

    ./bin/DrawLHEMGvsPH.exe <cfg file>

   example cfg can be found in :

    cfg/PlottingLHE_MGvsPH/


5) Code to make comparison between Delphes Trees of Phantom and MG:

    test/DrawDelphesMGvsPH.cpp 
 
   this is how to run it:

    ./bin/DrawDelphesMGvsPH.exe <cfg file>

   example cfg can be found in :

    cfg/PlottingReco_MGvsPH/


6) Code for making plot for both WW (SS,OS) and WZ analysis

    test/DrawFlatTreePlots.cpp

   this is how to run it:
   
    ./bin(DrawFlatTreePlots.exe <cfg file>

  
   There are different cfg files located in the cfg/PlottingCfg directory. In particular there are 4 sub-directories with templates:

    cfg/PlottingCfg/WW_SS_DF/plotBasic_WW_SS_DF.cfg    --> example for WW SS DF analysis
    cfg/PlottingCfg/WW_SS_SF/plotBasic_WW_SS_SF_EE.cfg --> example for WW SS SF ee final state
    cfg/PlottingCfg/WW_SS_SF/plotBasic_WW_SS_SF_UU.cfg --> example for WW SS SF mumu final state

    cfg/PlottingCfg/WZ_DF/plotBasic_WZ_DF.cfg --> example for WZ DF analysis       
    cfg/PlottingCfg/WZ_SF/plotBasic_WZ_SF_EE.cfg --> example for WZ SF eee    analysis       
    cfg/PlottingCfg/WZ_SF/plotBasic_WZ_SF_UU.cfg --> example for WZ SF mumumu analysis       

   ##############   
   CFG parmeters:
   ##############

    InputBaseDirectory = base dir on eos or local where all the subdirectory with files are locate
    InputSampleList    = text file where the list of sample to be plotted are defined
    InputVariableList  = text file with the list of variable to be plotted
    InputCutList       = text file where the cut are defined
    TreeName           = name of the flat tree

    Lumi                = lumi in fb-1 to be considered
    matchingCone        = used to match jets with leptons
    minLeptonCleaningPt = min lepton pt when jets are cleaned from isolated leptons
    minLeptonCutPt      = is the minimum threshold for defining isolated and loose isolated leptons
    minJetCutPt         = jet pt cut for jet counting
    usePuppiAsDefault   = use puppi jet as default when cut are applied instead of reco jets
    leptonIsoCut        = tight isolation cut
    leptonIsoCutLoose   = loose isolation cut
   
    outputPlotDirectory = output dir name for plots, under output/ directory

   #################
   Sample List file:
   #################

   You need to create a text file specifing in each column:

    -> Sample name of the directory where the root file are found 
    -> Reduced name for the plot ... different files in different directories can be merged in one entry in the stack giving the same reduced name. Example,
   
      PHANTOM_WZ_EWK_DF_126_TK2p5_EU                              WZ_EWK_DF
      PHANTOM_WZ_EWK_DF_126_TK2p5_UE                              WZ_EWK_DF

     means that both eemunu and mumuenu are merged in one common histo 

    -> color to be used
    -> cross section for the sample in (pb)
    -> number entries before: in case some skim have been done .. in our case should be always fixed to 1 since we can read directly the entries of the chain of root files
    -> isSignal = declare if the sample should be considered as signal or background for plotting and significance reasons

   ###############
   Layer Cut file:
   ###############

   You need to create a text file speciing some values for cuts to be applied (the code that loops on the event and apply these cuts is in src/utils.cc, fillHisto):

    -> Layer name is an identifier for the cut level
    -> Pt(l1) : leading lepton pt
    -> Pt(l2) : trailing leptons pt (if more than one trailing applyin this threshold to everyone)
    -> NLep   : number of tight leptons to be required in the event
    -> nextra : number of max extra loose leptons to be accepted in the event
    -> charge1*charge2   : if 0 not apply the cut, if 1 same sign, if -1 opposite sign (in WZ set to zero) .. this option used all the tight leptons that pass the nLep cut
    -> flavour1*flavour2 : if 0 not apply the cut, if 1 require all tight leptons to have the same flavour, if -1 require at least one tight lepton to have a different flavour
    -> pt(j1) and pt(j2) cut on the two leadin jets
    -> DetaJJ and MJJ are VBF cuts to apply
    -> bTagVeto: requires that the jet should have bTagging less than the value set in the card
    -> JetPUID:  requires that the jet should have a PU jet id flag grater than the value set in the card
    -> Mll(<) and Mll(>) are cut on the invariant mass of the two leading tight leptons in the event ... lower and upper bound.
    -> M(ll)(Zveto,min) M(ll)(Zveto,max) : are used in the following way .. loop on all each possible lepton pairs over a minPt cut, require to have same
       flavour and opposite charge (isolate leptons from Z decays) and then:

       if nLep == 2 typically used in a WW analysis, require the invariant mass to be outside the specified range,
       otherwise if nLep == 3, require that the mass should lie in that range.

       if at least one lepton pair fire this condition, the event can be skipped or used.

   ###################
   Variable List file:
   ###################

   You need to create a text file in which you need to specify:

    -> Variable name to used as histo name
    -> number of bins
    -> min and max of the x-axis
    -> label to be printed on the x-axis

   Since the variables that are plotted are not branch inside the default flat trees (some selection and cleaning are performed),
   we cannot use TTreeFormula ... if you want to define a new variable you need to decide a name for it, insert a line in the file and then
   fill the variable inside src/utils.cc (fillHisto method).

   if you comment out some line of the file with a #, those variables are not filled and not plotted.
   
   ###############################
   The code DrawFlatTreePlots.cpp:
   ###############################

    -> read the config file parameter
    -> read the sample txt file
    -> read the cut txt file
    -> read the variable txt file
    -> loop on the sample and create a chain of file for each Reduced name (sample with the same reduced name are merged together)
    -> loop on the event of each sample
    -> loop on the cut list and apply them
    -> for each cut and for each event loop on the variable list and store the value in a histogram.
    -> cut efficiency are calculated for each cut entry and for each sample. A root file with histogram is created under output/<outputPlotDirectory> and can
       be used to check the efficiency on signal and background samples of the applied selections.



7) Code for train MVA methods for selection optimization

    test/TMVATrainingSelections.cpp

   this is how to run it:
   
    ./bin/TMVATrainingSelections.exe <cfg file>

  
   There are different cfg files located in the cfg/TrainingCfg directory. In particular there are 4 sub-directories with templates:

    cfg/TrainingCfg/WW_SS_DF/trainBasic_WW_SS_DF.cfg    --> example for WW SS DF analysis
    cfg/TrainingCfg/WW_SS_SF/trainBasic_WW_SS_SF_EE.cfg --> example for WW SS SF ee final state
    cfg/TrainingCfg/WW_SS_SF/trainBasic_WW_SS_SF_UU.cfg --> example for WW SS SF mumu final state

    cfg/TrainingCfg/WZ_DF/trainBasic_WZ_DF.cfg --> example for WZ DF analysis       
    cfg/TrainingCfg/WZ_SF/trainBasic_WZ_SF_EE.cfg --> example for WZ SF eee    analysis       
    cfg/TrainingCfg/WZ_SF/trainBasic_WZ_SF_UU.cfg --> example for WZ SF mumumu analysis       

   ##############
   CFG parmeters:
   ##############

    InputBaseDirectory = base dir on eos or local where all the subdirectory with files are locate
    InputSampleList    = text file where the list of sample to be used are defined and classified as signal and background
    InputVariableList  = text file with the list of variables to be considered in the training
    InputSpectatorList = text file with the list of spectator variables 
    InputCutList       = text file where the cut are defined
    TreeName           = name of the flat tree
    TrainMVAMethodName = list of MVA methods to be considered (comma as separator); examples are CutsSA, CutsGA, Likelihood, BDTG, MLP .. etc
    VariableTransformation = list of transformation to be applied on the input variables follwoing TMVA convention (I,N,D,P,G)
    Label              = used when the final directory with xml files and output root file are created

    Lumi                = lumi in fb-1 to be considered
    matchingCone        = used to match jets with leptons
    minLeptonCleaningPt = min lepton pt when jets are cleaned from isolated leptons
    minLeptonCutPt      = is the minimum threshold for defining isolated and loose isolated leptons
    minJetCutPt         = jet pt cut for jet counting
    usePuppiAsDefault   = use puppi jet as default when cut are applied instead of reco jets
    leptonIsoCut        = tight isolation cut
    leptonIsoCutLoose   = loose isolation cut

    eventWeight         = to be applied as event based weight, can be a branch of the input trees... if you want we can plugin different weight strings for S and B
    pileUpBin           = training can be performed in independent pileup bins in case
    TrainEachVariable   = each variable in the list are trained independelty .. this option is useful when you want to train cut based selection on a huge set 
                          of input variables to se which is the most discriminating one
   
    outputFilrDirectory = output dir name for xml and root TMVA output files, under output/ directory
    outputFileName      = root of the .root output file

    
   ####################################
   The code TMVATrainingSelections.cpp:
   ####################################

    -> read the config file parameter
    -> read the sample txt file
    -> read the cut txt file
    -> read the variable txt file
    -> make a chain with all the background samples with the right lumi weight, and chain with all the signal ones
    -> Loop on the defined pileup bins, and loop on the cut list --> for each cut run the same set of training, the cut is used to pre-select events used in the training
    -> if train each variable is true a loop on the input var is done, otherwise the variable list is set as input of TMVA factory.
    -> loop on the chain event for both signal and background samples, apply the cut specified in the cutContainer and int the cfg file. Fill a TNtupla with the
       input variable value for each event (also the weight value is used in case).
    -> Loop on the input TMVA methods and run the training + testing for each of them.
    -> Close the loops and exit

8) Code to plot MVA training results from the produced root file

    test/TMVAPlotSignificance.cpp

   this is how to run it:
   
    ./bin/TMVAPlotSignificance.exe <cfg file>

  
   There are different cfg files located in the cfg/TrainingCfg directory. In particular there are 4 sub-directories with templates:

    cfg/TrainingCfg/WW_SS_DF/plotTrainBasic_WW_SS_DF.cfg    --> example for WW SS DF analysis
    cfg/TrainingCfg/WW_SS_SF/plotTrainBasic_WW_SS_SF_EE.cfg --> example for WW SS SF ee final state
    cfg/TrainingCfg/WW_SS_SF/plotTrainBasic_WW_SS_SF_UU.cfg --> example for WW SS SF mumu final state

    cfg/TrainingCfg/WZ_DF/plotTrainBasic_WZ_DF.cfg --> example for WZ DF analysis       
    cfg/TrainingCfg/WZ_SF/plotTrainBasic_WZ_SF_EE.cfg --> example for WZ SF eee    analysis       
    cfg/TrainingCfg/WZ_SF/plotTrainBasic_WZ_SF_UU.cfg --> example for WZ SF mumumu analysis       

   ##############
   CFG parmeters:
   ##############

    InputTrainingFileList = text file with the list of all the training result to be considered and plotted by the code	
    plotType              = 0 to make ROC as eff_sig vs eff_bkg, 1 to make eff_sig vs fake rate
    lumi                  = luminosity to be considered in fb ^{-1}
    outputPlotDirectory   = output directory where to put plots .. will be created under output/ dir   

   ##############
   Training List:
   ##############

   you need to create a txt file with this define set of column:

    -> fileName : path of each .root file created by TMVA to be considered
    -> varNameReduced : reduced name of the variable associated to this file
    -> puMin : min PU
    -> puMax : max PU
    -> methodName: a list of string separeted by ':' that indicates all the method used in that root file .. if more than one
       
   ####################################
   The code TMVAPlotSignificance.cpp:
   ####################################

    -> read the config file parameter
    -> read the training list txt file
    -> make the roc considering all the methods in all the input files overlayed in the same canvas
    -> Loop on the input file
    -> For each method in each file print: MVA distribution if present, overtraining and probability plots
    -> Take the number of signal and background events used in the testing from the TTree inside the root file, taking into account weights
    -> plot the significance for that number of expected signal and background events


9) Code to make signal polarized plot on Madgraphonly files:

    test/DrawPolarizationPlots.cpp
 
   this is how to run it:

    ./bin/DrawPolarizationPlots.exe <cfg file>

   example cfg can be found in :

    cfg/PolarizationCfg/

   the only difference is how to define the polarization cut by the LayerCut file and apply a LHE filter on MG samples to select the chose final state


10) Code to create datacards 1D and 2D for the inclusive analysis (WW_EWK cross sectio measurement)
 
    test/MakeDatacards.cpp
 
   this is how to run it:

    ./bin/MakeDatacards.exe <cfg file>

   example cfg can be found in :

    cfg/DataCardCfg/InclusiveCards

   here the cfg structure:

    InputSampleList	-> list of signal and background directories
    InputVariableList1D -> create a card for each variable with a root file with the histograms
    InputVariableList2D -> create a card for each pair of variable (unrolling of the TH2 in a TH1)
    InputCutList 	-> cut to be applied
    errorType 		    -> 0 means put on all the histogram sqrt(N) as bin content error, 1 set the poissonian mode, 2 meanse use the sum w2 for the MC.
    finalStateString 	    -> use to define categories in order to combine cards
    outputDataCardDirectory -> directory where all the cards and root files are located.

11) Code to create datacards 1D and 2D for the polarized cross section analysis (WW_EWK cross sectio measurement)
 
    test/MakeDatacards.cpp
 
   this is how to run it:

    ./bin/MakeDatacards.exe <cfg file>

   example cfg can be found in :

    cfg/DataCardCfg/PolarizedCards

   here the cfg structure:

    InputSampleList	-> list of signal and background directories
    InputVariableList1D -> create a card for each variable with a root file with the histograms
    InputVariableList2D -> create a card for each pair of variable (unrolling of the TH2 in a TH1)
    InputCutList 	-> cut to be applied
    errorType 		    -> 0 means put on all the histogram sqrt(N) as bin content error, 1 set the poissonian mode, 2 meanse use the sum w2 for the MC.
    finalStateString 	    -> use to define categories in order to combine cards
    outputDataCardDirectory -> directory where all the cards and root files are located.


12) submit combine jobs python/runCombine.py :
    
   basic option in common for all the methods:

    datacardDIR --> place where the datacard are for all the categories together with root file with histograms
    outputDIR   --> output directory for the file created by combine (it will be created as sub folder of datacardDIR)
    channel     --> channel to be analyzed : UU, EE, DF or COMB (combination)
    inputVariableList --> list of variables used to create the cards, in cfg/DataCards*/
    is2Dcard    --> in case of run the analysis on 2D cards, by default 1D
    batchMode  --> submit in LSF

   methods:

    computeAsymptotic --> compute Asymptotic limits: available options, noSystematics --> if 1 compute limit with -S 0 (no systematics taken into account)

    computeProfileLikelihood --> compute significance : observed and expected. When the expected is computed, different option can take into account: outputTree 0 means one toy for each file, 1 means nToys stored in one single output file (toys are performed in order to have a better estimation of the expected sensitivity), injectSignal is the stranght wrt to the signal rate in the datacard to inject.


    makeLikelihoodScan --> use the MultDimFit to have a scan of the Likelihood, rMin,rMax are defined in the code up to now.

    generateOnly --> generate the dataset toy from the templates in the card: options.injectSignal used to define the signal to inject, nToys number of toys and outputTree 0 means one output file per toy, 1 means nToys in 1 file.

    maximumLikelihoodFit --> make max likelihood fit, injecting signal with injectSignal, rMin and rMax fixed in the code, nToys to perform toys, outpuTree to store the information
        

12) EFT calculation :
    
    convert the SM card 

    python python/convertDC2EFT.py -i Card1D/final_mll_EEmm.txt -o Card1D/config_mll_EEmm -p LS0:-15:15:LS0

    python buildWorkspace.py --config=config_mll_EEmm 
    text2workspace.py WWVBS_mll_LS0.txt -o VBS_SS_Combine.root -P CombineEFT.CommonTools.HigherDimensionalOperators:LS0
    
    combine VBS_SS_Combine.root -M MultiDimFit -P LS0 --floatOtherPOIs=0 --algo=grid --points=1000 --minimizerStrategy=2 -n test_VBS_LS0 -t -1 --expectSignal=1