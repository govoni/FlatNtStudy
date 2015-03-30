python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 0 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/lumiExtrapolation/ --inputVariable R_mjj --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards
