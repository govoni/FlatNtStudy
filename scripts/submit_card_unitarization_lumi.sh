python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_v2/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin -10 --rMax 10 --batchMode --queque 8nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_v2/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 300 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_v2/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin -10 --rMax 10 --batchMode --queque 8nh --noGenerateCards
