python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/fakeRateExtrapolation/ --inputVariable mjj_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys 100 --rMin -1 --rMax 3 --batchMode --queque 8nh --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/fakeRateExtrapolation/ --inputVariable mjj_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys 300 --rMin -1 --rMax 3 --batchMode --queque 1nh --noGenerateCards  --fakeType fakeLepton

python python/runFakeRateExtrapolation.py --datacardDIR output/DataCards_WW_SS_Polarized_v2/Card2D/fakeRateExtrapolation/ --inputVariable mjj_mll --outputDIR computeMaxLikelihoodFit --makeMaxLikelihoodFit --injectSignal 1 --nToys 100 --rMin -1 --rMax 3 --batchMode --queque 8nh --noGenerateCards  --fakeType fakeLepton
