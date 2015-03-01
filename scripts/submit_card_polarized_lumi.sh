python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/lumiExtrapolation/ --inputVariable mjj_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin -5 --rMax 6 --batchMode --queque 8nh 

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/lumiExtrapolation/ --inputVariable mjj_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 300 --rMin -5 --rMax 6 --batchMode --queque 1nh --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/lumiExtrapolation/ --inputVariable mjj_mll --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin -5 --rMax 6 --batchMode --queque 8nh --noGenerateCards
