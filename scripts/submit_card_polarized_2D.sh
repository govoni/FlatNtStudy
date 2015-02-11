python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel UU --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel EE --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel DF --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel COMB --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel UU --nToys 1000  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel EE --nToys 1000  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel DF --nToys 1000  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel COMB --nToys 1000  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --noCombineCards

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel UU --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel EE --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel DF --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel COMB --nToys -1  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --noCombineCards

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel UU --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --nToys -1 

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel EE --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --nToys -1 

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel DF --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --nToys -1 

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card2D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel COMB --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables2D.txt --injectSignal 1 --is2Dcard --nToys -1  --noCombineCards
