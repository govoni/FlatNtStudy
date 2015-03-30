python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -30 --rMax 30 --batchMode --queque 1nh

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -30 --rMax 30 --batchMode --queque 1nh  --noGenerateCards 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -30 --rMax 30 --batchMode --queque 1nh --noGenerateCards --bruteForce 1

