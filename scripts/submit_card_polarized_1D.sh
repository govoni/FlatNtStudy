python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel UU --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel EE --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel DF --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --maximumLikelihoodFit --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeMaximumLikelihoodFit --batchMode --queque cmscaf1nd --channel COMB --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel UU --nToys 500  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel EE --nToys 500  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel DF --nToys 500  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeProfileLikelihood --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeProfileLikelihood --batchMode --queque cmscaf1nh --channel COMB --nToys 500  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1 --noCombineCards

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel UU --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel EE --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel DF --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1

python python/runCombine.py  --computeAsymptotic --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeAsymptotic --batchMode --queque cmscaf1nd --channel COMB --nToys 100  --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1 --noCombineCards

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel UU --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1  --nToys -1

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel EE --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1 --nToys -1

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel DF --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1  --nToys -1

python python/runCombine.py  --makeLikelihoodScan --datacardDIR output/DataCards_WW_SS_Polarized/Card1D/ --outputDIR computeLikelihoodScan --batchMode --queque cmscaf1nh --channel COMB --rMin -1 --rMax 3 --inputVariableList cfg/DataCardCfg/PolarizedCards/Variables1D.txt --injectSignal 1  --noCombineCards --nToys -1
