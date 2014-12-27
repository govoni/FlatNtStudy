#include "TMVATrainingClass.h"

// constructor giving files 
TMVATrainingClass::TMVATrainingClass(const vector<TFile*> & signalFileList, 
				   const vector<TFile*> & backgroundFileList, 
				   const string & TreeName,
				   const string & outputFilePath , 
				   const string & outputFileName, 
				   const string & Label, 
				   const string & transformation){

 
  SetSignalTree     (signalFileList,TreeName) ;
  SetBackgroundTree (backgroundFileList,TreeName) ;
  SetLabel (Label);
  SetOutputFile (outputFilePath , outputFileName) ;

  factory_ = new TMVA::Factory (TreeName_+"_"+Label_,
                                outputFile_, 
                                Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",
                                gROOT->IsBatch()?"!":"",transformation.c_str()));

}

// constructor giving tree
TMVATrainingClass::TMVATrainingClass(const vector<TTree*> & signalTreeList, 
				   const vector<TTree*> & backgroundTreeList,  
				   const string & TreeName,
                                   const string & outputFilePath , 
				   const string & outputFileName, 
				   const string & Label,
				   const string & transformation){
   

  SetTreeName (TreeName) ;
  SetSignalTree (signalTreeList) ;
  SetBackgroundTree (backgroundTreeList) ;
  SetLabel (Label);
  SetOutputFile (outputFilePath , outputFileName) ;

  factory_ = new TMVA::Factory (TreeName_+"_"+Label_,
				outputFile_,
				Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",
				transformation.c_str()));
}

// Deconstructor
TMVATrainingClass::~TMVATrainingClass(){

  for(size_t iTree = 0; iTree < signalTreeList_.size() ; iTree++) { 
   if(signalTreeList_.at(iTree)!=0)  delete signalTreeList_.at(iTree) ; 
  }

  for(size_t iTree = 0; iTree < backgroundTreeList_.size() ; iTree++) {
   if(backgroundTreeList_.at(iTree)!=0)  delete backgroundTreeList_.at(iTree) ;
  }

  for(size_t iTree = 0; iTree < signalTreeListForTraining_.size() ; iTree++) { 
   if(signalTreeListForTraining_.at(iTree)!=0)  delete signalTreeListForTraining_.at(iTree) ; 
  }

  for(size_t iTree = 0; iTree < backgroundTreeListForTraining_.size() ; iTree++) {
   if(backgroundTreeListForTraining_.at(iTree)!=0)  delete backgroundTreeListForTraining_.at(iTree) ;
  }

  if(outputFile_!=0) outputFile_->Close() ;
  if(factory_!=0)    factory_->Delete() ;

}

// AddTrainingVariables in the MVA
void TMVATrainingClass::AddTrainingVariables ( const vector<pair<string,string> > & mapTrainingVariables, 
                                              const vector<pair<string,string> > & mapSpectatorVariables){

  SetTrainingVariables(mapTrainingVariables);
  SetSpectatorVariables(mapSpectatorVariables);

  for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
    cout<<" train " <<mapTrainingVariables_.at(iVar).first<<" type "<<mapTrainingVariables_.at(iVar).second<<endl;
    factory_->AddVariable(mapTrainingVariables_.at(iVar).first+" := "+mapTrainingVariables_.at(iVar).first,*mapTrainingVariables_.at(iVar).second.c_str());
  }

  for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
    cout<<" spectator " <<mapSpectatorVariables_.at(iVar).first<<" type "<<mapSpectatorVariables_.at(iVar).second<<endl;
    factory_->AddSpectator(mapSpectatorVariables_.at(iVar).first,mapSpectatorVariables_.at(iVar).second.c_str());
  }    
}

// AddTrainingVariables in the MVA
void TMVATrainingClass::AddTrainingVariables ( const pair<string,string> & mapTrainingVariables, 
					      const vector<pair<string,string> > & mapSpectatorVariables){

  
  mapTrainingVariables_.push_back(mapTrainingVariables);
  SetSpectatorVariables(mapSpectatorVariables);

  for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
    cout<<" train " <<mapTrainingVariables_.at(iVar).first<<" type "<<mapTrainingVariables_.at(iVar).second<<endl;
    factory_->AddVariable(mapTrainingVariables_.at(iVar).first+" := "+mapTrainingVariables_.at(iVar).first,*mapTrainingVariables_.at(iVar).second.c_str());
  }

  for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
    cout<<" spectator " <<mapSpectatorVariables_.at(iVar).first<<" type "<<mapSpectatorVariables_.at(iVar).second<<endl;
    factory_->AddSpectator(mapSpectatorVariables_.at(iVar).first,mapSpectatorVariables_.at(iVar).second.c_str());
  }    
}



// Prepare the training addinf the preselection cut and the train/testing composition
void TMVATrainingClass::AddPrepareTraining (const cutContainer & cutContainer,
                                           const string & weightStringSignal,
					   const string & weightStringBackground,
					   const pair<int,int> & PileUpBinOfTraining,
					   const int & nTraining,
					   const int & nTesting, 
					   const string & splitMode,
					   const string & NormMode){


  // declare the option for the training and set the weight expression for the signal and background samples
  TString Option = Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=%s:NormMode=%s:!V",
                         nTraining,nTraining,nTesting,nTesting,splitMode.c_str(),NormMode.c_str());

  SetEventWeight (weightStringSignal,weightStringBackground);
  factory_->PrepareTrainingAndTestTree("","",Option.Data() );

}

// Book MVA Training Variables 
void TMVATrainingClass::BookMVATrees (const vector<double> & signalGlobalWeight, 
				     const vector<double> & backgroundGlobalWeight){
  
  SetGlobalSampleWeight(signalGlobalWeight,backgroundGlobalWeight);
 
  if(signalGlobalWeight.size() == signalTreeListForTraining_.size()){
    for(size_t iTree = 0; iTree < signalTreeListForTraining_.size(); iTree ++) 
      factory_->AddSignalTree (signalTreeListForTraining_.at(iTree),signalGlobalWeight.at(iTree)) ;
  }
  else{        
    for(size_t iTree = 0; iTree < signalTreeListForTraining_.size(); iTree ++) 
      factory_->AddSignalTree (signalTreeListForTraining_.at(iTree),1.0) ;
  }

  if(backgroundGlobalWeight.size() == backgroundTreeListForTraining_.size()){
    for(size_t iTree = 0; iTree < backgroundTreeListForTraining_.size(); iTree ++) 
      factory_->AddBackgroundTree (backgroundTreeListForTraining_.at(iTree),backgroundGlobalWeight.at(iTree)) ;
  }
  else{        
    for(size_t iTree = 0; iTree < backgroundTreeListForTraining_.size(); iTree ++) 
      factory_->AddBackgroundTree (backgroundTreeListForTraining_.at(iTree),1.0) ;
  }
  
}

// Train Rectangular Cuts
void TMVATrainingClass::BookandTrainRectangularCuts (const string & FitMethod, 
						     string variable){

  // create output directory if not there and change the variable name in case of a list
  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 
  replace(variable.begin(),variable.end(),'/', '_');
  replace(variable.begin(),variable.end(),'[', '_');
  replace(variable.begin(),variable.end(),']', '_');
  replace(variable.begin(),variable.end(),'(', '_');
  replace(variable.begin(),variable.end(),')', '_');
  replace(variable.begin(),variable.end(),':', '_');

  // Set Name of the Weight file for TMVA evaluating procedure
  outputFileWeightName_["Cuts"+FitMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_Cuts"+FitMethod+"_"+Label_+"_"+variable;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod+"_"+Label_];

  // book the method
  if(FitMethod!=""){ 

     TString Option = Form("!H:!V:FitMethod=%s:EffSel",FitMethod.c_str());
     TString Name   = Form("Cuts%s",FitMethod.c_str());

     if(TString(Name).Contains("CutsGA"))  
       Option = Option+":CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95";

     factory_->BookMethod( TMVA::Types::kCuts, Name.Data(),Option.Data());
  }

  else{
        TString Option = Form("!H:!V:FitMethod=MC:EffSel%s",transformations_.c_str());

        factory_->BookMethod( TMVA::Types::kCuts, "CutsMC"+FitMethod,Option.Data());

        Option = Form("!H:!V:FitMethod=GA::CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95%s",transformations_.c_str());

        factory_->BookMethod( TMVA::Types::kCuts, "CutsGA"+FitMethod,Option.Data());

        Option = Form("!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale%s",transformations_.c_str());

        factory_->BookMethod( TMVA::Types::kCuts, "CutsSA"+FitMethod,Option.Data());

  }

  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout<< "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout<< "==> TMVAClassification is done!" << endl;

}

// Train Likelihood
void TMVATrainingClass::BookandTrainLikelihood ( const string & LikelihoodType ){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure
  outputFileWeightName_[LikelihoodType+"_"+Label_] = outputFilePath_+"/TMVAWeight_"+LikelihoodType+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_[LikelihoodType+"_"+Label_];

  TString Option ;

  if( LikelihoodType == "LikelihoodKDE") { 
    Option = Form("LikelihoodKDE");
    factory_->BookMethod(TMVA::Types::kLikelihood, Option.Data(),"!H:!V:IgnoreNegWeightsInTraining:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:"
			                                         "KDEiter=Adaptive:CreateMVAPdfs:KDEFineFactor=0.3:KDEborder=None%s"+transformations_);
  }
  else if( LikelihoodType == "PDERS") { 
      Option = Form("%s",LikelihoodType.c_str());
      factory_->BookMethod(TMVA::Types::kPDERS, Option.Data(),
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:CreateMVAPdfs:DeltaFrac=4:GaussSigma=0.3:NormTree=T"+transformations_);
  }
  else if( LikelihoodType == "PDEFoam") { 
       Option = Form("%s",LikelihoodType.c_str());
       factory_->BookMethod(TMVA::Types::kPDEFoam, Option.Data(),"!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:SigBgSeparate=F:TailCut=0.001"
                                                                 ":VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T"+transformations_);
  }
  else if( LikelihoodType == "PDEFoamBoost") { 
      Option = Form("%s",LikelihoodType.c_str());
      factory_->BookMethod(TMVA::Types::kPDEFoam, Option.Data(),
                           "!H:!V:IgnoreNegWeightsInTraining:Boost_Num=30:CreateMVAPdfs:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4"
                           ":UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=300:nBin=20:Nmin=300:Kernel=None:Compress=T"+transformations_);
  }
  else{ Option = Form("%s",LikelihoodType.c_str());
        factory_->BookMethod( TMVA::Types::kLikelihood, Option.Data(),"!H:!V:!TransformOutput:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpol=Spline2"
			                                              ":NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50"+transformations_);
  }


  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train Fisher Discriminant
void TMVATrainingClass::BookandTrainFisherDiscriminant(){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                              

  outputFileWeightName_["Fisher"+Label_] = outputFilePath_+"/TMVAWeight_Fisher_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Fisher"+Label_];


  factory_->BookMethod( TMVA::Types::kFisher, "Fisher",
                        "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10:Fisher"+transformations_ );


  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train Linear Discriminant
void TMVATrainingClass::BookandTrainLinearDiscriminant(){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 
  // Set Name of the Weight file for TMVA evaluating procedure

  outputFileWeightName_["LD"+Label_] = outputFilePath_+"/TMVAWeight_LD_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["LD"+Label_];

  // Training Testing and Evaluating   
  outputFile_->cd();
  factory_->BookMethod( TMVA::Types::kLD, "LD", "H:!V:VarTransform=I,N,D,P:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10"+transformations_);

  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train MLP
void TMVATrainingClass::BookandTrainMLP(const int & nCycles, 
                                       const string & HiddenLayers, 
				       const string & NeuronType,
				       const string & TrainingMethod, 
				       const int & TestRate, 
				       const int & ConvergenceTests){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 
  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                               
  outputFileWeightName_["MLP_"+NeuronType+"_"+TrainingMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_MLP_"+NeuronType+"_"+TrainingMethod+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["MLP_"+NeuronType+"_"+TrainingMethod+"_"+Label_];

  TString Option = Form ("!H:!V:NCycles=%d:CalculateErrors:HiddenLayers=%s:NeuronType=%s:CreateMVAPdfs:TrainingMethod=%s:TestRate=%d"
			 ":ConvergenceTests=%d:UseRegulator%s",nCycles,HiddenLayers.c_str(),NeuronType.c_str(),TrainingMethod.c_str(),TestRate,
                         ConvergenceTests,transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kMLP, "MLP_"+NeuronType+"_"+TrainingMethod, Option.Data());
  
  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train Clemont Ferrand ANN
void TMVATrainingClass::BookandTrainCFMlpANN ( const int & nCycles, 
					      const string & HiddenLayers){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  outputFileWeightName_["CFMlpANN_"+Label_] = outputFilePath_+"/TMVAWeight_CFMlpANN_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["CFMlpANN_"+Label_];

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:CreateMVAPdfs%s",nCycles,HiddenLayers.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN",Option.Data());

  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}


// Train TMVA ANN
void TMVATrainingClass::BookandTrainTMlpANN  ( const int & nCycles, const string & HiddenLayers,  const string & TrainingMethod, const float & ValidationFraction){

  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                            
  outputFileWeightName_["TMlpANN_"+TrainingMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_TMlpANN_"+TrainingMethod+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["TMlpANN_"+TrainingMethod+"_"+Label_];

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:LearningMethod=%s:ValidationFraction=%f:CreateMVAPdfs%s",
			 nCycles,HiddenLayers.c_str(),TrainingMethod.c_str(),ValidationFraction,transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN_"+TrainingMethod,Option.Data());

  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train BDT
void TMVATrainingClass::BookandTrainBDT ( const int & NTrees, 
					 const bool & optimizeMethods, 
					 const string & BoostType, 
					 const float & AdaBoostBeta,
				         const string & PruneMethod, 
					 const int & PruneStrength, 
					 const int & MaxDepth, 
					 const string & SeparationType){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                          
  outputFileWeightName_["BDT_"+BoostType+"_"+PruneMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_BDT_"+BoostType+"_"+PruneMethod+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDT_"+BoostType+"_"+PruneMethod+"_"+Label_];
  
  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nEventsMin=200:nCuts=200%s",NTrees,BoostType.c_str(),AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT, "BDT_"+BoostType+"_"+PruneMethod, Option.Data());

  if(optimizeMethods) factory_->OptimizeAllMethods();                                                                                                                                                            
  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train BDTG
void TMVATrainingClass::BookandTrainBDTG ( const int & NTrees, 
					  const bool & optimizeMethods, 
					  const float & GradBaggingFraction, 
					  const string & PruneMethod,
                       			  const int & PruneStrength, 
					  const int & MaxDepth, 
					  const string & SeparationType){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                          
  outputFileWeightName_["BDTG_"+PruneMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_BDTG_"+PruneMethod+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG_"+PruneMethod+"_"+Label_];

  TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nEventsMin=200:nCuts=2000%s",NTrees,GradBaggingFraction,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT, "BDTG_"+PruneMethod, Option.Data());
  
  if(optimizeMethods) factory_->OptimizeAllMethods();                                                                                                                     
                                 
  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;

}

// Train BDTF
void TMVATrainingClass::BookandTrainBDTF ( const int & NTrees, 
					  const bool & optimizeMethods, 
					  const string & BoostType, 
					  const float & AdaBoostBeta, 
					  const string & PruneMethod,
                       			  const int & PruneStrength, 
					  const int & MaxDepth, 
					  const string & SeparationType){


  string command = " if [ ! -e "+outputFilePath_+" ] ; then mkdir "+outputFilePath_+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                 
  outputFileWeightName_["BDTF_"+PruneMethod+"_"+Label_] = outputFilePath_+"/TMVAWeight_BDTF_"+PruneMethod+"_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTF_"+PruneMethod+"_"+Label_];

  TString Option = Form ("!H:!V:CreateMVAPdfs:UseFisherCuts:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:"
                         "PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.10:nCuts=20%s",NTrees,BoostType.c_str(),
                          AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT,"BDTF_"+PruneMethod+"_", Option.Data());

  if(optimizeMethods) factory_->OptimizeAllMethods();
                                                                                                                                                             
  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;
}


//Set Signal Tree Name
void TMVATrainingClass::SetSignalTree (const vector<TFile*> & signalFileList, const string & TreeName){

  if(TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;

  for(size_t iFile = 0 ; iFile < signalFileList.size() ; iFile ++){
   if(signalFileList.at(iFile)!=0) 
    signalTreeList_.push_back((TTree*) signalFileList.at(iFile)->Get(TreeName_.c_str()));
  }   

  return ;

}


void TMVATrainingClass::SetSignalTree (const vector<TTree*> & signalTreeList){
  
  for(unsigned int iTree = 0; iTree< signalTreeList.size(); iTree++){
    if(signalTreeList.at(iTree)->GetEntries()>0) signalTreeList_.push_back(signalTreeList.at(iTree)) ; 
  }
  return ;

}

// Set Background Tree Name
void TMVATrainingClass::SetBackgroundTree (const vector<TFile*> & backgroundFileList, const string & TreeName){

  if(TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;
     
  for(size_t iFile = 0 ; iFile < backgroundFileList.size() ; iFile ++){
   if(backgroundFileList.at(iFile)!=0) 
    backgroundTreeList_.push_back((TTree*) backgroundFileList.at(iFile)->Get(TreeName_.c_str()));
  }

  return ;

}

void TMVATrainingClass::SetBackgroundTree (const vector<TTree*> & backgroundTreeList){

  for(unsigned int iTree = 0; iTree< backgroundTreeList.size(); iTree++){
    if(backgroundTreeList.at(iTree)->GetEntries()>0) backgroundTreeList_.push_back(backgroundTreeList.at(iTree)) ; 
  }
   return ;

}

// Set Training Variables 
void TMVATrainingClass::SetTrainingVariables  (const vector<pair<string,string> > & mapTrainingVariables){

   if(mapTrainingVariables.size()!=0) mapTrainingVariables_ = mapTrainingVariables;

   return ;

}

// Set Spectator Variables
void TMVATrainingClass::SetSpectatorVariables (const vector<pair<string,string> > & mapSpectatorVariables){

   if(mapSpectatorVariables.size()!=0) mapSpectatorVariables_ = mapSpectatorVariables;

   return ;
}

// Set label
void TMVATrainingClass::SetLabel (const string & Label ){

  Label_ = Label ;

  return ;

}

// Set Tree Name
void TMVATrainingClass::SetTreeName (const string & TreeName ){

  if(TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;

  return ;
}

// set output file
void TMVATrainingClass::SetOutputFile ( const string & outputFilePath , const string & outputFileName ){
  
  if( !outputFilePath.empty() && !outputFileName.empty()) { 
 
   outputFilePath_=outputFilePath; 
   outputFileName_=outputFileName;

   replace(outputFileName_.begin(),outputFileName_.end(),'/','_');
   replace(outputFileName_.begin(),outputFileName_.end(),'[','_');
   replace(outputFileName_.begin(),outputFileName_.end(),']','_');
   replace(outputFileName_.begin(),outputFileName_.end(),'(','_');
   replace(outputFileName_.begin(),outputFileName_.end(),')','_');
   replace(outputFileName_.begin(),outputFileName_.end(),':','_');

   outputFileNameComplete_ = outputFilePath_+"/"+outputFileName_+"_"+Label_+".root" ;

   outputFile_ = new TFile((outputFilePath_+"/"+outputFileName_+"_"+Label_+".root").c_str(),"RECREATE");
   
   outputFile_->cd();
 }

  return ;

}

// set global event weight
void TMVATrainingClass::SetGlobalSampleWeight (const vector<double> & signalGlobalWeight, const vector<double> & backgroundGlobalWeight){

  signalGlobalWeight_     = signalGlobalWeight ;
  backgroundGlobalWeight_ = backgroundGlobalWeight ;

  return ;

}


void TMVATrainingClass::SetEventWeight (const string & weightStringSignal, const string & weightStringBackground){

  factory_->SetSignalWeightExpression(weightStringSignal);
  factory_->SetBackgroundWeightExpression(weightStringBackground);

  return ;

}

void TMVATrainingClass::SetTransformations (const string & transformations){

  transformations_ = transformations;  

 return ;

}
