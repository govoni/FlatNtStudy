python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin 0 --rMax 2 --batchMode --queque cmscaf1nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 500 --rMin 0 --rMax 2 --batchMode --queque cmscaf1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin 0 --rMax 2 --batchMode --queque cmscaf1nh --noGenerateCards
