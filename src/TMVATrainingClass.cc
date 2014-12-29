#include "TMVATrainingClass.h"
#include "utils.h"

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
  SetLabel          (Label);
  SetOutputFile     (outputFilePath,outputFileName) ;
  SetTransformations(transformation);

  if(factory_!=0) delete factory_;

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
   

  SetTreeName       (TreeName) ;
  SetSignalTree     (signalTreeList) ;
  SetBackgroundTree (backgroundTreeList) ;
  SetLabel (Label);
  SetOutputFile (outputFilePath,outputFileName) ;
  SetTransformations(transformation);

  if(factory_!=0) delete factory_;

  factory_ = new TMVA::Factory (TreeName_+"_"+Label_,
				outputFile_,
				Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",
				transformation.c_str()));
}

// constructor giving tree
TMVATrainingClass::TMVATrainingClass(const vector<TChain*> & signalTreeList, 
				     const vector<TChain*> & backgroundTreeList,  
				     const string & TreeName,
				     const string & outputFilePath , 
				     const string & outputFileName, 
				     const string & Label,
				     const string & transformation){
   

  SetTreeName       (TreeName) ;
  SetSignalTree     (signalTreeList) ;
  SetBackgroundTree (backgroundTreeList) ;
  SetLabel (Label);
  SetOutputFile (outputFilePath,outputFileName) ;
  SetTransformations(transformation);

  if(factory_!=0) delete factory_;

  factory_ = new TMVA::Factory (TreeName_+"_"+Label_,
				outputFile_,
				Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",
				transformation.c_str()));
}

// Deconstructor
TMVATrainingClass::~TMVATrainingClass(){


  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for_each(backgroundTreeList_.begin(),backgroundTreeList_.end(), default_delete<TTree>());
  backgroundTreeList_.clear();

  for_each(signalTNtuplaForTraining_.begin(),signalTNtuplaForTraining_.end(), default_delete<TNtuple>());
  signalTNtuplaForTraining_.clear();

  for_each(backgroundTNtuplaForTraining_.begin(),backgroundTNtuplaForTraining_.end(), default_delete<TNtuple>());
  backgroundTNtuplaForTraining_.clear();

  mapTrainingVariables_.clear();
  mapSpectatorVariables_.clear();

  signalGlobalWeight_.clear();
  backgroundGlobalWeight_.clear();
  
  outputFileWeightName_.clear();

  if(outputFile_!=0) outputFile_->Close();
  if(factory_!=0)    factory_->Delete();
  if(reader_!=0)     delete reader_;
}


//Set Signal Tree Name
void TMVATrainingClass::SetSignalTree (const vector<TFile*> & signalFileList, 
				       const string & TreeName){

  if(TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;

  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(size_t iFile = 0 ; iFile < signalFileList.size() ; iFile ++){
   if(signalFileList.at(iFile)!=0) 
    signalTreeList_.push_back((TTree*) signalFileList.at(iFile)->Get(TreeName_.c_str()));
  }   

  return ;

}


void TMVATrainingClass::SetSignalTree (const vector<TTree*> & signalTreeList){

  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(unsigned int iTree = 0; iTree< signalTreeList.size(); iTree++){
    if(signalTreeList.at(iTree)->GetEntries()>0) signalTreeList_.push_back(signalTreeList.at(iTree)) ; 
  }
  return ;

}

void TMVATrainingClass::SetSignalTree (const vector<TChain*> & signalChainList){

  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(unsigned int iTree = 0; iTree< signalChainList.size(); iTree++){
    if(signalChainList.at(iTree)->GetEntries()>0) signalTreeList_.push_back(signalChainList.at(iTree)) ; 
  }
  return ;

}




// Set Background Tree Name
void TMVATrainingClass::SetBackgroundTree (const vector<TFile*> & backgroundFileList, 
					   const string & TreeName){

  if(TreeName!="") TreeName_ = TreeName ;
  else TreeName_ = "easyDelphes" ;
     
  for_each(backgroundTreeList_.begin(),backgroundTreeList_.end(), default_delete<TTree>());
  backgroundTreeList_.clear();
  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(size_t iFile = 0 ; iFile < backgroundFileList.size() ; iFile ++){
   if(backgroundFileList.at(iFile)!=0) 
    backgroundTreeList_.push_back((TTree*) backgroundFileList.at(iFile)->Get(TreeName_.c_str()));
  }

  return ;

}

void TMVATrainingClass::SetBackgroundTree (const vector<TTree*> & backgroundTreeList){

  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(unsigned int iTree = 0; iTree< backgroundTreeList.size(); iTree++){
    if(backgroundTreeList.at(iTree)->GetEntries()>0) backgroundTreeList_.push_back(backgroundTreeList.at(iTree)) ; 
  }
   return ;

}

void TMVATrainingClass::SetBackgroundTree (const vector<TChain*> & backgroundChainList){

  for_each(signalTreeList_.begin(),signalTreeList_.end(), default_delete<TTree>());
  signalTreeList_.clear();

  for(unsigned int iTree = 0; iTree< backgroundChainList.size(); iTree++){
    if(backgroundChainList.at(iTree)->GetEntries()>0) backgroundTreeList_.push_back(backgroundChainList.at(iTree)) ; 
  }
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
void TMVATrainingClass::SetOutputFile ( const string & outputFilePath , 
					const string & outputFileName ){
  
  if( !outputFilePath.empty() && !outputFileName.empty()) { 
 
   outputFilePath_ = outputFilePath; 
   outputFileName_ = outputFileName;

   replace(outputFileName_.begin(),outputFileName_.end(),'/','_');
   replace(outputFileName_.begin(),outputFileName_.end(),'[','_');
   replace(outputFileName_.begin(),outputFileName_.end(),']','_');
   replace(outputFileName_.begin(),outputFileName_.end(),'(','_');
   replace(outputFileName_.begin(),outputFileName_.end(),')','_');
   replace(outputFileName_.begin(),outputFileName_.end(),':','_');

   outputFileNameComplete_ = outputFilePath_+"/"+outputFileName_+"_"+Label_+".root" ;

   if(outputFile_!=0) delete outputFile_ ;

   outputFile_ = new TFile((outputFilePath_+"/"+outputFileName_+"_"+Label_+".root").c_str(),"RECREATE");
   
   outputFile_->cd();
 }

  return ;

}

void TMVATrainingClass::SetTransformations (const std::string & transformations){

  transformations_ = transformations;

  return ;

}



// AddTrainingVariables in the MVA
void TMVATrainingClass::AddTrainingVariables ( const vector<string> & mapTrainingVariables, 
					       const vector<string> & mapSpectatorVariables){

  SetTrainingVariables(mapTrainingVariables);
  SetSpectatorVariables(mapSpectatorVariables);

  for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
    cout<<" train " <<mapTrainingVariables_.at(iVar)<<endl;
    factory_->AddVariable(mapTrainingVariables_.at(iVar)+" := "+mapTrainingVariables_.at(iVar),'F');
  }

  for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
    cout<<" spectator " <<mapSpectatorVariables_.at(iVar)<<endl;
    factory_->AddSpectator(mapSpectatorVariables_.at(iVar),'F');
  }    
}

// AddTrainingVariables in the MVA
void TMVATrainingClass::AddTrainingVariables ( const string& mapTrainingVariables, 
					       const vector<string> & mapSpectatorVariables){

  
  mapTrainingVariables_.push_back(mapTrainingVariables);
  SetSpectatorVariables(mapSpectatorVariables);

  for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
    cout<<" train " <<mapTrainingVariables_.at(iVar)<<" type "<<endl;
    factory_->AddVariable(mapTrainingVariables_.at(iVar)+" := "+mapTrainingVariables_.at(iVar),'F');
  }

  for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
    cout<<" spectator " <<mapSpectatorVariables_.at(iVar)<<" type "<<endl;
    factory_->AddSpectator(mapSpectatorVariables_.at(iVar),'F');
  }    
}

// Set Training Variables 
void TMVATrainingClass::SetTrainingVariables  (const vector<string> & mapTrainingVariables){

   mapTrainingVariables_.clear();
   if(mapTrainingVariables.size()!=0) mapTrainingVariables_ = mapTrainingVariables;

   return ;

}

// Set Spectator Variables
void TMVATrainingClass::SetSpectatorVariables (const vector<string> & mapSpectatorVariables){

   mapSpectatorVariables_.clear();
   if(mapSpectatorVariables.size()!=0) mapSpectatorVariables_ = mapSpectatorVariables;

   return ;
}



// Prepare the training addinf the preselection cut and the train/testing composition
void TMVATrainingClass::AddPrepareTraining (const cutContainer & cutContainer,
					    string  weightStringSignal,
					    string  weightStringBackground,
					    const pair<int,int> & PileUpBinOfTraining,
					    const int & nTraining,
					    const int & nTesting, 
					    const string & splitMode,
					    const string & NormMode){

  // cut container for events
  cutEvent_ = cutContainer;
  npuRange_ = PileUpBinOfTraining;

  // declare the option for the training and set the weight expression for the signal and background samples --> weight expression can be a formula, not only a single branch 
  TString Option = Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=%s:NormMode=%s:!V",
                         nTraining,nTraining,nTesting,nTesting,splitMode.c_str(),NormMode.c_str());

  SetEventWeight (weightStringSignal,weightStringBackground); // set the event basis weight in the factoryy
  factory_->PrepareTrainingAndTestTree("","",Option.Data());  // set the options

  // create the varlist for the TNtupla --> variables to be used as input, spectator and weights
  string varListSignal ;
  for(size_t iVar = 0; iVar < mapTrainingVariables_.size() ; iVar++){ // loop on training variables
    if(iVar!=0) varListSignal += ":"+mapTrainingVariables_.at(iVar);
    else        varListSignal += mapTrainingVariables_.at(iVar);
  }

  for(size_t iVar = 0; iVar < mapSpectatorVariables_.size() ; iVar++){ // loop on spectator variables
    if(iVar!=0) varListSignal += ":"+mapSpectatorVariables_.at(iVar);
    else        varListSignal += mapSpectatorVariables_.at(iVar);
  }

  string varListBackground ;
  varListBackground = varListSignal ;

  // the re-weight can be different for signal and background, while the input and spectator must be the same
  replace(weightStringBackground.begin(),weightStringBackground.end(),'*',':');
  replace(weightStringBackground.begin(),weightStringBackground.end(),'=',':');
  replace(weightStringBackground.begin(),weightStringBackground.end(),'/',':');
  varListBackground += ":"+weightStringBackground;

  replace(weightStringSignal.begin(),weightStringSignal.end(),'*',':');
  replace(weightStringSignal.begin(),weightStringSignal.end(),'=',':');
  replace(weightStringSignal.begin(),weightStringSignal.end(),'/',':');
  varListSignal += ":"+weightStringSignal;  

  // create the TNtuple structure for both signal and background
  for(size_t iTree = 0; iTree < signalTreeList_.size() ; iTree++){
    signalTNtuplaForTraining_.push_back(new TNtuple(TreeName_.c_str(),"",varListSignal.c_str()));
  }

  for(size_t iTree = 0; iTree < backgroundTreeList_.size() ; iTree++){
    backgroundTNtuplaForTraining_.push_back(new TNtuple(TreeName_.c_str(),"",varListBackground.c_str()));
  }

  // make a list with the name of all the variables to be used --> split the weight expression
  vector<string> totalSignalVariableList;
  string segment;
  stringstream signal_temp(varListSignal); 
  while(getline(signal_temp, segment,':')){
      totalSignalVariableList.push_back(segment);
  } 

  vector<string> totalBackgroundVariableList;
  stringstream backg_temp(varListBackground); 
  while(getline(backg_temp, segment,':')){
      totalBackgroundVariableList.push_back(segment);
  } 

  
  // Now loop on tress and events, applying cuts
  vector<float> variableValue;
  map<string,TH1F*> vect;

  for(size_t iTree = 0; iTree  < signalTNtuplaForTraining_.size() ; iTree++){
    if(reader_!=0) delete reader_;
    reader_  = new readTree((TTree*)(signalTreeList_.at(iTree))); // create a reader of each tree
    for(int iEvent = 0; iEvent < signalTreeList_.at(iTree)->GetEntries(); iEvent++){ // Loop on each tree entries
      reader_->fChain->GetEntry(iEvent) ;
      variableValue.clear();  
      if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;
      // skip event with less than two leptons by default                    
      if(reader_->pt1 < 0          or reader_->pt2 < 0)          continue ; // skip the event --> only two reco leptons are good                                               
      if(reader_->jetpt1 < 0       or reader_->jetpt2 < 0)       continue ; // skip the event with less than two reco jet                                                      
      if(reader_->jetpt_puppi1 < 0 or reader_->jetpt_puppi2 < 0) continue ; // skip the event with less than two reco jet                                                      

      if(reader_->npu < npuRange_.first or reader_->npu >  npuRange_.second) continue; // cut on NPU range
          
      vect.clear();
   
      // apply analysis cut from utils.h 
      if(!passCutContainerSelection(reader_,
				    cutEvent_,
				    "",
				    usePuppiAsDefault_,
				    minPtLeptonCut_,
				    minPtLeptonCutCleaning_,
				    leptonIsoCut_,
				    leptonIsoLooseCut_,
				    matchingCone_,
				    minJetCutPt_,
				    vect)) continue;


      FillVariablesNtupla(variableValue,totalSignalVariableList); // fill the vector with variables value     
      signalTNtuplaForTraining_.at(iTree)->Fill(&variableValue[0]); // fill the ntupla for this event
    }    
  }
  
  // Now loop on tress and events, applying cuts
  for(size_t iTree = 0; iTree  < backgroundTNtuplaForTraining_.size() ; iTree++){
    if(reader_!=0) delete reader_;
    reader_  = new readTree((TTree*)(backgroundTreeList_.at(iTree)));
    for(int iEvent = 0; iEvent < backgroundTreeList_.at(iTree)->GetEntries(); iEvent++){ // loop on backg events

      reader_->fChain->GetEntry(iEvent) ;    
      variableValue.clear();

      if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;
      // skip event with less than two leptons by default                    
      if(reader_->pt1 < 0          or reader_->pt2 < 0)          continue ; // skip the event --> only two reco leptons are good                                               
      if(reader_->jetpt1 < 0       or reader_->jetpt2 < 0)       continue ; // skip the event with less than two reco jet                                                      
      if(reader_->jetpt_puppi1 < 0 or reader_->jetpt_puppi2 < 0) continue ; // skip the event with less than two reco jet                                                      

      if(reader_->npu < npuRange_.first or reader_->npu > npuRange_.second) continue;
        
      vect.clear();
      if(!passCutContainerSelection(reader_, // apply analysis cuts
				    cutEvent_,
				    "",
				    usePuppiAsDefault_,
				    minPtLeptonCut_,
				    minPtLeptonCutCleaning_,
				    leptonIsoCut_,
				    leptonIsoLooseCut_,
				    matchingCone_,
				    minJetCutPt_,
				    vect)) continue;


      FillVariablesNtupla(variableValue,totalBackgroundVariableList); // fill the vector with variables value     
      backgroundTNtuplaForTraining_.at(iTree)->Fill(&variableValue[0]); // fill the ntupla for this event
    }
  }
}



void TMVATrainingClass::SetBasicEventCutInfo ( const bool & usePuppiAsDefault,
					       const float & minPtLeptonCut,
					       const float & minPtLeptonCutCleaning,
					       const float & leptonIsoCut,
					       const float & leptonIsoLooseCut,
					       const float & matchingCone,
					       const float & minJetCutPt     
					       ){

  usePuppiAsDefault_ = usePuppiAsDefault;
  minPtLeptonCut_         = minPtLeptonCut;
  minPtLeptonCutCleaning_ = minPtLeptonCutCleaning;
  leptonIsoCut_      = leptonIsoCut;
  leptonIsoLooseCut_ = leptonIsoLooseCut;
  matchingCone_ = matchingCone;
  minJetCutPt_  = minJetCutPt;

}
    
// Book MVA Training Variables 
void TMVATrainingClass::BookMVATrees (const vector<float> & signalGlobalWeight, 
				      const vector<float> & backgroundGlobalWeight){
  
  SetGlobalSampleWeight(signalGlobalWeight,backgroundGlobalWeight);
 
  if(signalGlobalWeight.size() == signalTNtuplaForTraining_.size()){
    for(size_t iTree = 0; iTree < signalTNtuplaForTraining_.size(); iTree ++) 
      factory_->AddSignalTree (signalTNtuplaForTraining_.at(iTree),signalGlobalWeight.at(iTree)) ;
  }
  else{        
    for(size_t iTree = 0; iTree < signalTNtuplaForTraining_.size(); iTree ++) 
      factory_->AddSignalTree (signalTNtuplaForTraining_.at(iTree),1.0) ;
  }

  if(backgroundGlobalWeight.size() == backgroundTNtuplaForTraining_.size()){
    for(size_t iTree = 0; iTree < backgroundTNtuplaForTraining_.size(); iTree ++) 
      factory_->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree),backgroundGlobalWeight.at(iTree)) ;
  }
  else{        
    for(size_t iTree = 0; iTree < backgroundTNtuplaForTraining_.size(); iTree ++) 
      factory_->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree),1.0) ;
  }
  
}

// set global event weight
void TMVATrainingClass::SetGlobalSampleWeight (const vector<float> & signalGlobalWeight, const vector<float> & backgroundGlobalWeight){

  signalGlobalWeight_.clear();
  backgroundGlobalWeight_.clear();

  signalGlobalWeight_     = signalGlobalWeight ;
  backgroundGlobalWeight_ = backgroundGlobalWeight ;

  return ;

}


void TMVATrainingClass::SetEventWeight (const string & weightStringSignal, const string & weightStringBackground){

  factory_->SetSignalWeightExpression(weightStringSignal);
  factory_->SetBackgroundWeightExpression(weightStringBackground);

  return ;

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
  outputFileWeightName_["MLP_"+Label_] = outputFilePath_+"/TMVAWeight_MLP_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["MLP_"+Label_];

  TString Option = Form ("!H:!V:NCycles=%d:CalculateErrors:HiddenLayers=%s:NeuronType=%s:CreateMVAPdfs:TrainingMethod=%s:TestRate=%d"
			 ":ConvergenceTests=%d:UseRegulator%s",nCycles,HiddenLayers.c_str(),NeuronType.c_str(),TrainingMethod.c_str(),TestRate,
                         ConvergenceTests,transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kMLP, "MLP", Option.Data());
  
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
  outputFileWeightName_["TMlpANN_"+Label_] = outputFilePath_+"/TMVAWeight_TMlpANN_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["TMlpANN_"+Label_];

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:LearningMethod=%s:ValidationFraction=%f:CreateMVAPdfs%s",
			 nCycles,HiddenLayers.c_str(),TrainingMethod.c_str(),ValidationFraction,transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN",Option.Data());

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
  outputFileWeightName_["BDT_"+Label_] = outputFilePath_+"/TMVAWeight_BDT_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDT_"+Label_];
  
  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nEventsMin=200:nCuts=200%s",NTrees,BoostType.c_str(),AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT, "BDT", Option.Data());

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
  outputFileWeightName_["BDTG_"+Label_] = outputFilePath_+"/TMVAWeight_BDTG_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG_"+Label_];

  TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nEventsMin=200:nCuts=2000%s",NTrees,GradBaggingFraction,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT, "BDTG", Option.Data());
  
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
  outputFileWeightName_["BDTF_"+Label_] = outputFilePath_+"/TMVAWeight_BDTF_"+Label_;
  (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTF_"+Label_];

  TString Option = Form ("!H:!V:CreateMVAPdfs:UseFisherCuts:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:"
                         "PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.10:nCuts=20%s",NTrees,BoostType.c_str(),
                          AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str(),transformations_.c_str());

  factory_->BookMethod( TMVA::Types::kBDT,"BDTF", Option.Data());

  if(optimizeMethods) factory_->OptimizeAllMethods();
                                                                                                                                                             
  factory_->TrainAllMethods();

  factory_->TestAllMethods();

  factory_->EvaluateAllMethods();

  factory_->DeleteAllMethods();

  cout << "==> Wrote root file: " << outputFile_->GetName() << endl;
  cout << "==> TMVAClassification is done!" << endl;
}

void TMVATrainingClass::FillVariablesNtupla(vector<float> & variableValue, const vector<string> & variableList){


  TLorentzVector L_dilepton, L_met;

  // dump all the lepton in the event                                                                                                                                      
  vector<leptonContainer> LeptonsAll;
  fillRecoLeptonsArray (LeptonsAll, *reader_);
  // dump tight leptons                                                                                                                                                    
  vector<leptonContainer> leptonsIsoTight ;
  leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_, minPtLeptonCut_);
    
  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
  if(not usePuppiAsDefault_)
    L_met.SetPtEtaPhiM       (reader_->pfmet,0.,reader_->pfmetphi, 0.) ; 
  else
    L_met.SetPtEtaPhiM       (reader_->pfmet_puppi,0.,reader_->pfmetphi_puppi, 0.) ; 
    
  TLorentzVector L_dijet;
  float asimJ = 0, asimL = 0, Rvar = 0;

  vector<jetContainer> RecoJetsAll ;
  if(not usePuppiAsDefault_)
   fillRecoJetArray (RecoJetsAll, *reader_) ;
  else 
   fillPuppiJetArray (RecoJetsAll, *reader_) ;

  // take jets                                                                                                                                                             
  vector<jetContainer> RecoJets;
  RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt_, cutEvent_.bTagVeto, cutEvent_.jetPUID, minPtLeptonCutCleaning_, matchingCone_);

  if(RecoJets.size() > 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
  }
      
  // loop on the variable and find the values
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){
    if(variableList.at(iVar) == "detajj" and RecoJets.size() >= 2){
      variableValue.push_back(float(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))); 
    }
    else if(variableList.at(iVar) == "mjj" and RecoJets.size() >= 2){
      variableValue.push_back(float(L_dijet.M()));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LL"){
      variableValue.push_back(float(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)));
    }
    else if(variableList.at(iVar) == "mll"){
      variableValue.push_back(float(L_dilepton.M()));
    }
    else if(variableList.at(iVar) == "Asim_j"){
      variableValue.push_back(float(asimJ));
    }
    else if(variableList.at(iVar) == "Asim_l"){
      variableValue.push_back(float(asimL));
    }
    else if(variableList.at(iVar) == "R"){
      variableValue.push_back(float(Rvar));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LL"){
      variableValue.push_back(float(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LMet"){
      variableValue.push_back(float(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met)));
    }     
    else if(variableList.at(iVar) == "DeltaPhi_LLMet"){
      variableValue.push_back(float(L_dilepton.DeltaPhi(L_met)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_TLMet"){
      variableValue.push_back(float(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_met)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJ"   and RecoJets.size()>=2){
      variableValue.push_back(float(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJMet" and RecoJets.size()>=2){
      variableValue.push_back(float(L_dijet.DeltaPhi(L_met)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LJMet" and RecoJets.size()>=1){
      variableValue.push_back(float(RecoJets.at(0).jet4V_.DeltaPhi(L_met)));
    }
    else if(variableList.at(iVar) == "DeltaPhi_TJMet" and RecoJets.size() >=2){
      variableValue.push_back(float(RecoJets.at(1).jet4V_.DeltaPhi(L_met)));
    }
    else if(variableList.at(iVar) == "npu"){
      variableValue.push_back(float(reader_-> npu));
    }
    else if(variableList.at(iVar) == "ptj1" and RecoJets.size()>=1){
      variableValue.push_back(float(RecoJets.at(0).jet4V_.Pt()));
    }
    else if(variableList.at(iVar) == "ptj2" and RecoJets.size()>=2){
      variableValue.push_back(float(RecoJets.at(1).jet4V_.Pt()));
    }
    else if(variableList.at(iVar) == "ptl1"){
      variableValue.push_back(float(leptonsIsoTight.at(0).lepton4V_.Pt()));
    }
    else if(variableList.at(iVar) == "ptl2"){
      variableValue.push_back(float(leptonsIsoTight.at(1).lepton4V_.Pt()));
    }
    else if(variableList.at(iVar) == "met"){
      variableValue.push_back(float(L_met.Pt()));
    }
    //        else if(variableList.at(iVar) == "weight"){
    //	  variableValue.push_back(float(reader_->weight));
    //}
  }
}
