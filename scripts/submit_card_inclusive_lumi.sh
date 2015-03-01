python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_v2/Card1D/lumiExtrapolation/ --inputVariable ptjj --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin 0.85 --rMax 1.15 --batchMode --queque cmscaf1nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_v2/Card1D/lumiExtrapolation/ --inputVariable ptjj --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 500 --rMin 0.85 --rMax 1.15 --batchMode --queque cmscaf1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_v2/Card1D/lumiExtrapolation/ --inputVariable ptjj --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin 0.85 --rMax 1.15 --batchMode --queque cmscaf1nh --noGenerateCards
