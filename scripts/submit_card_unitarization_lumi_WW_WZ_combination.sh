
#python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/PhaseI/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh   --runCombinationOnly --runWWWZCombination

#python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/PhaseI/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards --runCombinationOnly --runWWWZCombination

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/PhaseII/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh   --runCombinationOnly --runWWWZCombination

python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/PhaseII/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards --runCombinationOnly --runWWWZCombination

#python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/Aged/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeAsymptotic --makeAsymptotic --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh   --runCombinationOnly --runWWWZCombination

#python python/runLumiExtrapolation.py --datacardDIR output/DataCards_Combination_WW_WZ_v2/Aged/HvsNoH/lumiExtrapolation/ --inputVariable R_mll --outputDIR computeProfileLikelihood --makeProfileLikelihood --injectSignal 1 --nToys -1 --rMin -100 --rMax 100 --batchMode --queque 1nh --noGenerateCards --runCombinationOnly --runWWWZCombination

