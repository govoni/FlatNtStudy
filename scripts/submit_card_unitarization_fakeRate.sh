python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton --bruteForce 1

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_HvsNoH_Dynamic/Card2D/fakeRateExtrapolation/ --inputVariable R_mll --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys -1 --rMin -10 --rMax 10 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton
