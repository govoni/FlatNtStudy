python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable DeltaPhi_JJ_ptl1 --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton
