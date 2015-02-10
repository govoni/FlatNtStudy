python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/lumiExtrapolation/ --inputVariable mll_ptJJ --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin -1 --rMax 3 --batchMode --queque cmscaf1nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/lumiExtrapolation/ --inputVariable mll_ptJJ --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 100 --rMin -1 --rMax 3 --batchMode --queque cmscaf1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/lumiExtrapolation/ --inputVariable mll_ptJJ --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin -1 --rMax 3 --batchMode --queque cmscaf1nh --noGenerateCards
