python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic_PhaseI/Card2D//lumiExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic_PhaseI/Card2D//lumiExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh  --noGenerateCards

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic_PhaseI/Card2D//lumiExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards

