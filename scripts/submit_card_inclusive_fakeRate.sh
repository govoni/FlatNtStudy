python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mjj --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 0 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mjj --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Inclusive_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mjj --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton
