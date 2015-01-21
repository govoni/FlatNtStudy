#include "TMVATrainingClass.h"
#include "utils.h"
#include "TSystem.h"

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

  factory_.push_back(new TMVA::Factory (TreeName_+"_"+Label_,
					outputFile_.back(), 
					Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",transformation.c_str())));
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

  factory_.push_back(new TMVA::Factory (TreeName_+"_"+Label_,
					outputFile_.back(),
					Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",transformation.c_str())));

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
  SetTransformations(transformation);  
  SetOutputFile (outputFilePath,outputFileName) ;
  
  factory_.push_back(new TMVA::Factory (TreeName_+"_"+Label_,
					outputFile_.back(),
					Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",transformation.c_str())));
  
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

  for(size_t iFile = 0; iFile < outputFile_.size(); iFile++)
    outputFile_.at(iFile)->Close();
  for_each(outputFile_.begin(),outputFile_.end(), default_delete<TFile>());
  outputFile_.clear();

  for_each(factory_.begin(),factory_.end(), default_delete<TMVA::Factory>());
  factory_.clear();

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

  for(size_t iFile = 0 ; iFile < backgroundFileList.size() ; iFile ++){
   if(backgroundFileList.at(iFile)!=0) 
    backgroundTreeList_.push_back((TTree*) backgroundFileList.at(iFile)->Get(TreeName_.c_str()));
  }

  return ;

}

void TMVATrainingClass::SetBackgroundTree (const vector<TTree*> & backgroundTreeList){

  for_each(backgroundTreeList_.begin(),backgroundTreeList_.end(), default_delete<TTree>());
  backgroundTreeList_.clear();

  for(unsigned int iTree = 0; iTree< backgroundTreeList.size(); iTree++){
    if(backgroundTreeList.at(iTree)->GetEntries()>0) backgroundTreeList_.push_back(backgroundTreeList.at(iTree)) ; 
  }
   return ;

}

void TMVATrainingClass::SetBackgroundTree (const vector<TChain*> & backgroundChainList){

  for_each(backgroundTreeList_.begin(),backgroundTreeList_.end(), default_delete<TTree>());
  backgroundTreeList_.clear();
  
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
 
    outputFilePath_.push_back(outputFilePath); 
    outputFileName_.push_back(outputFileName);

    replace(outputFileName_.back().begin(),outputFileName_.back().end(),'/','_');
    replace(outputFileName_.back().begin(),outputFileName_.back().end(),'[','_');
    replace(outputFileName_.back().begin(),outputFileName_.back().end(),']','_');
    replace(outputFileName_.back().begin(),outputFileName_.back().end(),'(','_');
    replace(outputFileName_.back().begin(),outputFileName_.back().end(),')','_');
    replace(outputFileName_.back().begin(),outputFileName_.back().end(),':','_');

   if(Label_!="") 
     outputFileNameComplete_.push_back(outputFilePath_.back()+"/"+outputFileName_.back()+"_"+Label_+".root");
   else
     outputFileNameComplete_.push_back(outputFilePath_.back()+"/"+outputFileName_.back()+".root");
 
   cout<<"TMVATrainingClass::SetOutputFile "<<outputFileNameComplete_.back()<<endl;

   outputFile_.push_back(new TFile((outputFileNameComplete_.back()).c_str(),"RECREATE"));   
   outputFile_.back()->cd();
 }

  return ;

}

void TMVATrainingClass::SetTransformations (const std::string & transformations){

  transformations_ = transformations;

  return ;

}



// AddTrainingVariables in the MVA
void TMVATrainingClass::AddTrainingVariables ( const vector<string> & mapTrainingVariables, 
					       const vector<string> & mapSpectatorVariables,
					       const bool & trainEachVarIndependently){

  SetTrainingVariables(mapTrainingVariables);
  SetSpectatorVariables(mapSpectatorVariables);

  trainEachVarIndependently_ = trainEachVarIndependently;

  if(trainEachVarIndependently_ == false){
    for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
      cout<<"TMVATrainingClass::AddTrainingVariables : train " <<mapTrainingVariables_.at(iVar)<<endl;
      factory_.back()->AddVariable(mapTrainingVariables_.at(iVar)+" := "+mapTrainingVariables_.at(iVar),'F');
    }

    for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
      cout<<"TMVATrainingClass::AddTrainingVariables : spectator " <<mapSpectatorVariables_.at(iVar)<<endl;
      factory_.back()->AddSpectator(mapSpectatorVariables_.at(iVar),'F');
    }    
  }
  else{

    factory_.clear();
    outputFile_.clear();
    for( size_t iVar = 0 ; iVar < mapTrainingVariables_.size() ; iVar ++ ){
      TString Name = Form("%s_%s_%s",TreeName_.c_str(),Label_.c_str(),mapTrainingVariables_.at(iVar).c_str());
      SetOutputFile     (outputFilePath_.back(),outputFileName_.at(0)+"_"+mapTrainingVariables_.at(iVar)) ;
      factory_.push_back(new TMVA::Factory (Name,
					    outputFile_.back(),
					    Form("!V:!Silent:%sColor:DrawProgressBar:AnalysisType=Classification%s",gROOT->IsBatch()?"!":"",transformations_.c_str())));
      cout<<"TMVATrainingClass::AddTrainingVariables : train " <<mapTrainingVariables_.at(iVar)<<" factory "<<factory_.back()->RootBaseDir()->GetName()<<endl;
      factory_.back()->AddVariable(mapTrainingVariables_.at(iVar)+" := "+mapTrainingVariables_.at(iVar),'F');
    }
    
    for(size_t iFact = 0; iFact < factory_.size(); iFact++){
      for( size_t iVar = 0 ; iVar < mapSpectatorVariables_.size() ; iVar ++ ){
	cout<<"TMVATrainingClass::AddTrainingVariables spectator " <<mapSpectatorVariables_.at(iVar)<<endl;
	factory_.at(iFact)->AddSpectator(mapSpectatorVariables_.at(iVar),'F');
      }
    }
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
  for(size_t ifact = 0; ifact < factory_.size(); ifact++){
    factory_.at(ifact)->PrepareTrainingAndTestTree("","",Option.Data());  // set the options
  }
  
  // create the varlist for the TNtupla --> variables to be used as input, spectator and weights
  vector<string> varListSignal ;
  vector<string> varListBackground ;

  if(trainEachVarIndependently_ == false){

    varListSignal.assign(1,"");

    for(size_t iVar = 0; iVar < mapTrainingVariables_.size() ; iVar++){ // loop on training variables
      if(iVar!=0) varListSignal.at(0) += ":"+mapTrainingVariables_.at(iVar);
      else        varListSignal.at(0) += mapTrainingVariables_.at(iVar);
    }
    varListSignal.at(0) += ":" ;

    for(size_t iVar = 0; iVar < mapSpectatorVariables_.size() ; iVar++){ // loop on spectator variables
      if(iVar!=0) varListSignal.at(0) += ":"+mapSpectatorVariables_.at(iVar);
      else        varListSignal.at(0) += mapSpectatorVariables_.at(iVar);
    }

    varListBackground = varListSignal ;

  
    // the re-weight can be different for signal and background, while the input and spectator must be the same
    replace(weightStringBackground.begin(),weightStringBackground.end(),'*',':');
    replace(weightStringBackground.begin(),weightStringBackground.end(),'=',':');
    replace(weightStringBackground.begin(),weightStringBackground.end(),'/',':');
    varListBackground.at(0) += ":"+weightStringBackground;

    replace(weightStringSignal.begin(),weightStringSignal.end(),'*',':');
    replace(weightStringSignal.begin(),weightStringSignal.end(),'=',':');
    replace(weightStringSignal.begin(),weightStringSignal.end(),'/',':');
    varListSignal.at(0) += ":"+weightStringSignal;  

  }
  else{

    varListSignal.assign(mapTrainingVariables_.size(),"");
    
    for(size_t iVar = 0; iVar < mapTrainingVariables_.size() ; iVar++){ // loop on training variables
      varListSignal.at(iVar) += mapTrainingVariables_.at(iVar)+":";
    }

    for(size_t iVar = 0; iVar < mapSpectatorVariables_.size() ; iVar++){ // loop on spectator variables
      for(size_t iVar2 = 0; iVar2 < mapTrainingVariables_.size() ; iVar2++){ // loop on training variables     
	varListSignal.at(iVar2) += mapSpectatorVariables_.at(iVar)+":";
      }
    }

    varListBackground = varListSignal ;

  
    // the re-weight can be different for signal and background, while the input and spectator must be the same
    replace(weightStringBackground.begin(),weightStringBackground.end(),'*',':');
    replace(weightStringBackground.begin(),weightStringBackground.end(),'=',':');
    replace(weightStringBackground.begin(),weightStringBackground.end(),'/',':');
    for(size_t iVar = 0; iVar < mapTrainingVariables_.size() ; iVar++){
      varListBackground.at(iVar) += weightStringBackground;
    }
    
    replace(weightStringSignal.begin(),weightStringSignal.end(),'*',':');
    replace(weightStringSignal.begin(),weightStringSignal.end(),'=',':');
    replace(weightStringSignal.begin(),weightStringSignal.end(),'/',':');
    for(size_t iVar = 0; iVar < mapTrainingVariables_.size() ; iVar++){
      varListSignal.at(iVar) += weightStringSignal;  
    }
  }


  
  // create the TNtuple structure for both signal and background
  for(size_t iTree = 0; iTree < signalTreeList_.size() ; iTree++){
    for(size_t iVar = 0; iVar < varListSignal.size(); iVar++){   
      signalTNtuplaForTraining_.push_back(new TNtuple(Form("%s_%d_%d",TreeName_.c_str(),int(iTree*varListSignal.size()),int(iVar)),"",varListSignal.at(iVar).c_str()));
    }
  }

  for(size_t iTree = 0; iTree < backgroundTreeList_.size() ; iTree++){
    for(size_t iVar = 0; iVar < varListBackground.size(); iVar++){   
      backgroundTNtuplaForTraining_.push_back(new TNtuple(Form("%s_%d_%d",TreeName_.c_str(),int(iTree*varListBackground.size()),int(iVar)),"",varListBackground.at(iVar).c_str()));
    }
  }

  
  // Now loop on tress and events, applying cuts
  vector<float> variableValue;
  vector<string> totalSignalVariableList;
  vector<string> totalBackgroundVariableList;
  map<string,TH1F*> vect;

  for(size_t iTree = 0; iTree  < signalTreeList_.size() ; iTree++){

    reader_  = new readTree((TTree*)(signalTreeList_.at(iTree))); // create a reader of each tree    
    
    for(int iEvent = 0; iEvent < signalTreeList_.at(iTree)->GetEntries(); iEvent++){ // Loop on each tree entries
      reader_->fChain->GetEntry(iEvent) ;

      // make a list with the name of all the variables to be used --> split the weight expression
      variableValue.clear();  
      totalSignalVariableList.clear();

      if (iEvent % 100000 == 0) cout << "TMVATrainingClass::AddPrepareTraining reading signal event " << iEvent << "\n" ;
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
				    leptonIsoCut_mu_,
				    leptonIsoCut_el_,
				    leptonIsoLooseCut_,
				    matchingCone_,
				    minJetCutPt_,
				    vect)) continue;

      for(size_t iVar = 0; iVar < varListSignal.size(); iVar++){
	totalSignalVariableList.clear();
	string segment;
	stringstream signal_temp(varListSignal.at(iVar)); 
	while(getline(signal_temp, segment,':')){
	  totalSignalVariableList.push_back(segment);
	} 
	variableValue.clear();
	FillVariablesNtupla(variableValue,totalSignalVariableList); // fill the vector with variables value     
        if(iTree == 0)	signalTNtuplaForTraining_.at(iTree+iVar)->Fill(&variableValue[0]); // fill the ntupla for this event      
        else signalTNtuplaForTraining_.at(iTree*varListSignal.size()+iVar)->Fill(&variableValue[0]); // fill the ntupla for this event      
      }
    }
  }
  
  // Now loop on tress and events, applying cuts
  for(size_t iTree = 0; iTree  < backgroundTreeList_.size() ; iTree++){
    reader_  = new readTree((TTree*)(backgroundTreeList_.at(iTree)));
    for(int iEvent = 0; iEvent < backgroundTreeList_.at(iTree)->GetEntries(); iEvent++){ // loop on backg events

      reader_->fChain->GetEntry(iEvent) ;    

      variableValue.clear();  
      totalBackgroundVariableList.clear();

      if (iEvent % 100000 == 0) cout << "TMVATrainingClass::AddPrepareTraining reading bkg event " << iEvent << "\n" ;
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
				    leptonIsoCut_mu_,
				    leptonIsoCut_el_,
				    leptonIsoLooseCut_,
				    matchingCone_,
				    minJetCutPt_,
				    vect)) continue;


      for(size_t iVar = 0; iVar < varListBackground.size(); iVar++){

	totalBackgroundVariableList.clear();
	string segment;
	stringstream signal_temp(varListBackground.at(iVar)); 
	while(getline(signal_temp, segment,':')){
	  totalBackgroundVariableList.push_back(segment);
	} 
	variableValue.clear();
	FillVariablesNtupla(variableValue,totalBackgroundVariableList); // fill the vector with variables value     
	backgroundTNtuplaForTraining_.at(iTree*varListBackground.size()+iVar)->Fill(&variableValue[0]); // fill the ntupla for this event       
      }
    }
  } 
  
}



void TMVATrainingClass::SetBasicEventCutInfo ( const bool & usePuppiAsDefault,
					       const float & minPtLeptonCut,
					       const float & minPtLeptonCutCleaning,
					       const float & leptonIsoCut_mu,
					       const float & leptonIsoCut_el,
					       const float & leptonIsoLooseCut,
					       const float & matchingCone,
					       const float & minJetCutPt     
					       ){

  usePuppiAsDefault_      = usePuppiAsDefault;
  minPtLeptonCut_         = minPtLeptonCut;
  minPtLeptonCutCleaning_ = minPtLeptonCutCleaning;
  leptonIsoCut_mu_        = leptonIsoCut_mu;
  leptonIsoCut_el_        = leptonIsoCut_el;
  leptonIsoLooseCut_      = leptonIsoLooseCut;
  matchingCone_ = matchingCone;
  minJetCutPt_  = minJetCutPt;

}
    
// Book MVA Training Variables 
void TMVATrainingClass::BookMVATrees (const vector<float> & signalGlobalWeight, 
				      const vector<float> & backgroundGlobalWeight){
  
  SetGlobalSampleWeight(signalGlobalWeight,backgroundGlobalWeight);

  if( trainEachVarIndependently_ == false){
    if(signalGlobalWeight.size() == signalTNtuplaForTraining_.size()){
      for(size_t iTree = 0; iTree < signalTNtuplaForTraining_.size(); iTree ++) {
	factory_.back()->RootBaseDir()->cd();
	factory_.back()->AddSignalTree (signalTNtuplaForTraining_.at(iTree),signalGlobalWeight.at(iTree)) ;
      }
    }
    else{        
      for(size_t iTree = 0; iTree < signalTNtuplaForTraining_.size(); iTree ++){ 
	factory_.back()->RootBaseDir()->cd();
	factory_.back()->AddSignalTree (signalTNtuplaForTraining_.at(iTree),1.0) ;
      }
    }

    if(backgroundGlobalWeight.size() == backgroundTNtuplaForTraining_.size()){
      for(size_t iTree = 0; iTree < backgroundTNtuplaForTraining_.size(); iTree ++) {
	factory_.back()->RootBaseDir()->cd();
	factory_.back()->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree),backgroundGlobalWeight.at(iTree)) ;
      }
    }
    else{        
      for(size_t iTree = 0; iTree < backgroundTNtuplaForTraining_.size(); iTree ++) {
	factory_.back()->RootBaseDir()->cd();
	factory_.back()->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree),1.0) ;
      }
    }
  }
  else{
    if(signalGlobalWeight.size() == signalTNtuplaForTraining_.size()/mapTrainingVariables_.size()){
      for(size_t iTree = 0; iTree < signalGlobalWeight.size(); iTree ++) {
        for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){
	  factory_.at(iVar)->RootBaseDir()->cd();
	  factory_.at(iVar)->AddSignalTree (signalTNtuplaForTraining_.at(iTree*mapTrainingVariables_.size()+iVar),signalGlobalWeight.at(iTree)) ;
	}
      }
    }
    else{        
      for(size_t iTree = 0; iTree < signalTNtuplaForTraining_.size(); iTree++) 
        for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){
	  factory_.at(iVar)->RootBaseDir()->cd();
	  factory_.at(iVar)->AddSignalTree (signalTNtuplaForTraining_.at(iTree*mapTrainingVariables_.size()+iVar),1.0) ;
	}
    }

   if(backgroundGlobalWeight.size() == backgroundTNtuplaForTraining_.size()/mapTrainingVariables_.size()){
      for(size_t iTree = 0; iTree < backgroundGlobalWeight.size(); iTree ++){ 
	for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){
	  factory_.at(iVar)->RootBaseDir()->cd();
	  factory_.at(iVar)->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree*mapTrainingVariables_.size()+iVar),backgroundGlobalWeight.at(iTree)) ;
	}
      }
    }
    else{        
      for(size_t iTree = 0; iTree < backgroundTNtuplaForTraining_.size(); iTree ++){ 
        for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){
	  factory_.at(iVar)->RootBaseDir()->cd();
	  factory_.at(iVar)->AddBackgroundTree (backgroundTNtuplaForTraining_.at(iTree*mapTrainingVariables_.size()+iVar),1.0) ;
	}
      }
    }
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

  for(size_t ifact = 0; ifact < factory_.size(); ifact++){
    factory_.at(ifact)->RootBaseDir()->cd();
    factory_.at(ifact)->SetSignalWeightExpression(weightStringSignal);
    factory_.at(ifact)->SetBackgroundWeightExpression(weightStringBackground);
  }

  return ;

}


// Train Rectangular Cuts
void TMVATrainingClass::BookandTrainRectangularCuts (const string & FitMethod){

  // create output directory if not there and change the variable name in case of a list
  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  if(trainEachVarIndependently_){

    for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){

      string variable = mapTrainingVariables_.at(iVar);
      replace(variable.begin(),variable.end(),'/', '_');
      replace(variable.begin(),variable.end(),'[', '_');
      replace(variable.begin(),variable.end(),']', '_');
      replace(variable.begin(),variable.end(),'(', '_');
      replace(variable.begin(),variable.end(),')', '_');
      replace(variable.begin(),variable.end(),':', '_');

      outputFile_.at(iVar)->cd();
      factory_.at(iVar)->RootBaseDir()->cd();

      // Set Name of the Weight file for TMVA evaluating procedure
      if(Label_ !=""){
	outputFileWeightName_["Cuts"+FitMethod+"_"+Label_] = outputFilePath_.back()+"/TMVAWeight_Cuts"+FitMethod+"_"+Label_+"_"+variable;
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod+"_"+Label_];
      }
      else {
	outputFileWeightName_["Cuts"+FitMethod] = outputFilePath_.back()+"/TMVAWeight_Cuts"+FitMethod+"_"+variable;
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod];
      }

      // book the method
      if(FitMethod!=""){ 

	TString Option = Form("!H:!V:FitMethod=%s:EffSel",FitMethod.c_str());
	TString Name   = Form("Cuts%s",FitMethod.c_str());

	if(TString(Name).Contains("CutsGA"))  
	  Option = Option+":CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95";

	factory_.at(iVar)->BookMethod( TMVA::Types::kCuts, Name.Data(),Option.Data());
      }

      else{

        TString Option = Form("!H:!V:FitMethod=MC:EffSel");

        factory_.at(iVar)->BookMethod( TMVA::Types::kCuts, "CutsMC"+FitMethod,Option.Data());

        Option = Form("!H:!V:FitMethod=GA::CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95");
	
        factory_.at(iVar)->BookMethod( TMVA::Types::kCuts, "CutsGA"+FitMethod,Option.Data());
	
        Option = Form("!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale");

        factory_.at(iVar)->BookMethod( TMVA::Types::kCuts, "CutsSA"+FitMethod,Option.Data());
      }    

      factory_.at(iVar)->TrainAllMethods();

      factory_.at(iVar)->TestAllMethods();

      factory_.at(iVar)->EvaluateAllMethods();
    
      factory_.at(iVar)->DeleteAllMethods();
    }
  }
  else{

    outputFile_.back()->cd();
    factory_.back()->RootBaseDir()->cd();

    // Set Name of the Weight file for TMVA evaluating procedure
    if(Label_ !=""){
      outputFileWeightName_["Cuts"+FitMethod+"_"+Label_] = outputFilePath_.back()+"/TMVAWeight_Cuts"+FitMethod+"_"+Label_;
      (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod+"_"+Label_];
    }
    else {
      outputFileWeightName_["Cuts"+FitMethod] = outputFilePath_.back()+"/TMVAWeight_Cuts"+FitMethod;
      (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Cuts"+FitMethod];
    }

    // book the method
    if(FitMethod!=""){ 

      TString Option = Form("!H:!V:FitMethod=%s:EffSel",FitMethod.c_str());
      TString Name   = Form("Cuts%s",FitMethod.c_str());
      
      if(TString(Name).Contains("CutsGA"))  
	Option = Option+":CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95";
      
      factory_.back()->BookMethod( TMVA::Types::kCuts, Name.Data(),Option.Data());
    }

    else{
      TString Option = Form("!H:!V:FitMethod=MC:EffSel");
      
      factory_.back()->BookMethod( TMVA::Types::kCuts, "CutsMC"+FitMethod,Option.Data());
      
      Option = Form("!H:!V:FitMethod=GA::CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95");
      
      factory_.back()->BookMethod( TMVA::Types::kCuts, "CutsGA"+FitMethod,Option.Data());
	
      Option = Form("!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale");

      factory_.back()->BookMethod( TMVA::Types::kCuts, "CutsSA"+FitMethod,Option.Data());
    }    

    factory_.back()->TrainAllMethods();
    
    factory_.back()->TestAllMethods();
    
    factory_.back()->EvaluateAllMethods();
    
    factory_.back()->DeleteAllMethods();
  }

  cout<< "==> TMVAClassification is done!" << endl;

}

// Train Likelihood
void TMVATrainingClass::BookandTrainLikelihood ( const string & LikelihoodType ){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if(Label_ !=""){
    outputFileWeightName_[LikelihoodType+"_"+Label_] = outputFilePath_.back()+"/TMVAWeight_"+LikelihoodType+"_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_[LikelihoodType+"_"+Label_];
  }
  else {
    outputFileWeightName_[LikelihoodType] = outputFilePath_.back()+"/TMVAWeight_"+LikelihoodType;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_[LikelihoodType];
  }

  TString Option ;

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  if( LikelihoodType == "LikelihoodKDE") { 
    Option = Form("LikelihoodKDE");
    factory_.back()->BookMethod(TMVA::Types::kLikelihood, Option.Data(),"!H:!V:IgnoreNegWeightsInTraining:!TransformOutput:PDFInterpol=KDE:KDEtype=Gauss:"
			                                         "KDEiter=Adaptive:CreateMVAPdfs:KDEFineFactor=0.3:KDEborder=None");
  }
  else if( LikelihoodType == "PDERS") { 
      Option = Form("%s",LikelihoodType.c_str());
      factory_.back()->BookMethod(TMVA::Types::kPDERS, Option.Data(),
                           "!H:!V:VolumeRangeMode=Adaptive:KernelEstimator=Gauss:CreateMVAPdfs:DeltaFrac=4:GaussSigma=0.3:NormTree=T");
  }
  else if( LikelihoodType == "PDEFoam") { 
       Option = Form("%s",LikelihoodType.c_str());
       factory_.back()->BookMethod(TMVA::Types::kPDEFoam, Option.Data(),"!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:SigBgSeparate=F:TailCut=0.001"
                                                                 ":VolFrac=0.0666:nActiveCells=500:nSampl=2000:nBin=5:Nmin=100:Kernel=None:Compress=T");
  }
  else if( LikelihoodType == "PDEFoamBoost") { 
      Option = Form("%s",LikelihoodType.c_str());
      factory_.back()->BookMethod(TMVA::Types::kPDEFoam, Option.Data(),
                           "!H:!V:IgnoreNegWeightsInTraining:Boost_Num=30:CreateMVAPdfs:Boost_Transform=linear:SigBgSeparate=F:MaxDepth=4"
                           ":UseYesNoCell=T:DTLogic=MisClassificationError:FillFoamWithOrigWeights=F:TailCut=0:nActiveCells=300:nBin=20:Nmin=300:Kernel=None:Compress=T");
  }
  else{ Option = Form("%s",LikelihoodType.c_str());
        factory_.back()->BookMethod( TMVA::Types::kLikelihood, Option.Data(),"!H:!V:!TransformOutput:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpol=Spline2"
			                                              ":NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50");
  }


  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

 cout << "==> TMVAClassification is done!" << endl;

}

// Train Fisher Discriminant
void TMVATrainingClass::BookandTrainFisherDiscriminant(){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                              
  if(Label_ !=""){
    outputFileWeightName_["Fisher_"+Label_] = outputFilePath_.back()+"/TMVAWeight_Fisher_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Fisher_"+Label_];
  }
  else{
    outputFileWeightName_["Fisher"] = outputFilePath_.back()+"/TMVAWeight_Fisher";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["Fisher"];
  }

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kFisher, "Fisher",
                        "!H:!V:CreateMVAPdfs:IgnoreNegWeightsInTraining:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10:Fisher");


  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

  cout << "==> TMVAClassification is done!" << endl;

}

// Train Linear Discriminant
void TMVATrainingClass::BookandTrainLinearDiscriminant(){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl;
 
  // Set Name of the Weight file for TMVA evaluating procedure
  if(Label_ !=""){
    outputFileWeightName_["LD_"+Label_] = outputFilePath_.back()+"/TMVAWeight_LD_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["LD_"+Label_];
  }
  else {
    outputFileWeightName_["LD"] = outputFilePath_.back()+"/TMVAWeight_LD";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["LD"];
  }

  // Training Testing and Evaluating   
  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kLD, "LD", "H:!V:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10");

  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

  cout << "==> TMVAClassification is done!" << endl;

}

// Train MLP
void TMVATrainingClass::BookandTrainMLP(const int & nCycles, 
					const string & HiddenLayers, 
					const string & NeuronType,
					const string & TrainingMethod, 
					const int & TestRate, 
					const int & ConvergenceTests){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if(Label_ != ""){
    outputFileWeightName_["MLP_"+Label_] = outputFilePath_.back()+"/TMVAWeight_MLP_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["MLP_"+Label_];
  }
  else{
    outputFileWeightName_["MLP"] = outputFilePath_.back()+"/TMVAWeight_MLP";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["MLP"];
  }

  TString Option = Form ("!H:!V:NCycles=%d:CalculateErrors:HiddenLayers=%s:NeuronType=%s:CreateMVAPdfs:TrainingMethod=%s:TestRate=%d"
			 ":ConvergenceTests=%d:UseRegulator",nCycles,HiddenLayers.c_str(),NeuronType.c_str(),TrainingMethod.c_str(),TestRate,
                         ConvergenceTests);

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kMLP, "MLP", Option.Data());
  
  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

  cout << "==> TMVAClassification is done!" << endl;

}

// Train Clemont Ferrand ANN
void TMVATrainingClass::BookandTrainCFMlpANN ( const int & nCycles, 
					       const string & HiddenLayers){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                            
  if(Label_ !=""){
    outputFileWeightName_["CFMlpANN_"+Label_] = outputFilePath_.back()+"/TMVAWeight_CFMlpANN_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["CFMlpANN_"+Label_];
  }
  else {
    outputFileWeightName_["CFMlpANN"] = outputFilePath_.back()+"/TMVAWeight_CFMlpANN";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["CFMlpANN"];
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:CreateMVAPdfs",nCycles,HiddenLayers.c_str());
 
  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN",Option.Data());

  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

  cout << "==> TMVAClassification is done!" << endl;

}


// Train TMVA ANN
void TMVATrainingClass::BookandTrainTMlpANN  ( const int & nCycles, 
					       const string & HiddenLayers,  
					       const string & TrainingMethod, 
					       const float & ValidationFraction){

  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                        
  if(Label_ != ""){         
    outputFileWeightName_["TMlpANN_"+Label_] = outputFilePath_.back()+"/TMVAWeight_TMlpANN_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["TMlpANN_"+Label_];
  }
  else {
    outputFileWeightName_["TMlpANN"] = outputFilePath_.back()+"/TMVAWeight_TMlpANN";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["TMlpANN"];
  }

  TString Option = Form ("!H:!V:NCycles=%d:HiddenLayers=%s:LearningMethod=%s:ValidationFraction=%f:CreateMVAPdfs",
			 nCycles,HiddenLayers.c_str(),TrainingMethod.c_str(),ValidationFraction);

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN",Option.Data());

  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

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


  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure
  if(Label_ != ""){                                                          
    outputFileWeightName_["BDT_"+Label_] = outputFilePath_.back()+"/TMVAWeight_BDT_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDT_"+Label_];
  }
  else {
    outputFileWeightName_["BDT"] = outputFilePath_.back()+"/TMVAWeight_BDT";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDT"];
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:MinNodeSize=2:nCuts=200",NTrees,BoostType.c_str(),AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str());

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kBDT, "BDT", Option.Data());

  if(optimizeMethods) factory_.back()->OptimizeAllMethods();                                                                                                                                                            
  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

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


  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 


  if(trainEachVarIndependently_){

    for(size_t iVar = 0; iVar < mapTrainingVariables_.size(); iVar++){

      string variable = mapTrainingVariables_.at(iVar);
      replace(variable.begin(),variable.end(),'/', '_');
      replace(variable.begin(),variable.end(),'[', '_');
      replace(variable.begin(),variable.end(),']', '_');
      replace(variable.begin(),variable.end(),'(', '_');
      replace(variable.begin(),variable.end(),')', '_');
      replace(variable.begin(),variable.end(),':', '_');

      outputFile_.at(iVar)->cd();
      factory_.at(iVar)->RootBaseDir()->cd();

      // Set Name of the Weight file for TMVA evaluating procedure                                                                                                          
      if(Label_ != ""){
	outputFileWeightName_["BDTG_"+Label_] = outputFilePath_.back()+"/TMVAWeight_BDTG_"+Label_+"_"+variable;
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG_"+Label_];
      }
      else {
	outputFileWeightName_["BDTG"] = outputFilePath_.back()+"/TMVAWeight_BDTG_"+variable;
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG"];
      }

      TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nCuts=2000:IgnoreNegWeightsInTraining",NTrees,GradBaggingFraction,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str());
      
      factory_.at(iVar)->BookMethod( TMVA::Types::kBDT, "BDTG", Option.Data());
      
      if(optimizeMethods) factory_.at(iVar)->OptimizeAllMethods();                                                                                                             
                                 
      factory_.at(iVar)->TrainAllMethods();
      
      factory_.at(iVar)->TestAllMethods();
  
      factory_.at(iVar)->EvaluateAllMethods();

    }
  }
  else{

      // Set Name of the Weight file for TMVA evaluating procedure                                                                                                          
      if(Label_ != ""){
	outputFileWeightName_["BDTG_"+Label_] = outputFilePath_.back()+"/TMVAWeight_BDTG_"+Label_;
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG_"+Label_];
      }
      else {
	outputFileWeightName_["BDTG"] = outputFilePath_.back()+"/TMVAWeight_BDTG";
	(TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTG"];
      }

      TString Option = Form ("CreateMVAPdfs:NTrees=%d:BoostType=Grad:!UseBaggedGrad:GradBaggingFraction=%f:PruneMethod=%s:PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.1:NNodesMax=100000:UseYesNoLeaf=F:nCuts=2000",NTrees,GradBaggingFraction,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str());

      outputFile_.back()->cd();
      factory_.back()->RootBaseDir()->cd();

      factory_.back()->BookMethod( TMVA::Types::kBDT, "BDTG", Option.Data());
      
      if(optimizeMethods) factory_.back()->OptimizeAllMethods();                                                                                                             
                                 
      factory_.back()->TrainAllMethods();
      
      factory_.back()->TestAllMethods();
  
      factory_.back()->EvaluateAllMethods();

  }
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



  if(trainEachVarIndependently_) {
    cerr<<" train each var independently available only for Cut and BDTG --> exit from this function"<<endl;
    return ;
  }

  string command = " if [ ! -e "+outputFilePath_.back()+" ] ; then mkdir "+outputFilePath_.back()+" ; fi";
  int result = system(command.c_str());
  if(result) cout<<"Directory created "<<outputFilePath_.back()<<endl; 

  // Set Name of the Weight file for TMVA evaluating procedure                                                                                                                 
  if(Label_ !=""){
    outputFileWeightName_["BDTF_"+Label_] = outputFilePath_.back()+"/TMVAWeight_BDTF_"+Label_;
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTF_"+Label_];
  }
  else{
    outputFileWeightName_["BDTF"] = outputFilePath_.back()+"/TMVAWeight_BDTF";
    (TMVA::gConfig().GetIONames()).fWeightFileDir = outputFileWeightName_["BDTF"];
  }

  TString Option = Form ("!H:!V:CreateMVAPdfs:UseFisherCuts:NTrees=%d:BoostType=%s:AdaBoostBeta=%f:PruneMethod=%s:"
                         "PruneStrength=%d:MaxDepth=%d:SeparationType=%s:Shrinkage=0.10:nCuts=20",NTrees,BoostType.c_str(),
                          AdaBoostBeta,PruneMethod.c_str(),PruneStrength,MaxDepth,SeparationType.c_str());

  outputFile_.back()->cd();
  factory_.back()->RootBaseDir()->cd();

  factory_.back()->BookMethod( TMVA::Types::kBDT,"BDTF", Option.Data());

  if(optimizeMethods) factory_.back()->OptimizeAllMethods();
                                                                                                                                                             
  factory_.back()->TrainAllMethods();

  factory_.back()->TestAllMethods();

  factory_.back()->EvaluateAllMethods();

  factory_.back()->DeleteAllMethods();

  cout << "==> TMVAClassification is done!" << endl;
}

void TMVATrainingClass::FillVariablesNtupla(vector<float> & variableValue, const vector<string> & variableList){

  TLorentzVector L_dilepton, L_met;

  // dump all the lepton in the event                                                                                                                                      
  vector<leptonContainer> LeptonsAll;
  fillRecoLeptonsArray (LeptonsAll, *reader_);
  // dump tight leptons                                                                                                                                                    
  vector<leptonContainer> leptonsIsoTight ;
  leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu_, leptonIsoCut_el_, minPtLeptonCut_);
    
  L_dilepton = leptonsIsoTight.at(0).lepton4V_ + leptonsIsoTight.at(1).lepton4V_ ;
  if(not usePuppiAsDefault_)
    L_met.SetPtEtaPhiM       (reader_->pfmet,0.,reader_->pfmetphi, 0.) ; 
  else
    L_met.SetPtEtaPhiM       (reader_->pfmet_puppi,0.,reader_->pfmetphi_puppi, 0.) ; 
    
  TLorentzVector L_dijet;
  float asimJ = 0, asimL = 0, Rvar = 0, aveEta = 0;

  vector<jetContainer> RecoJetsAll ;
  if(not usePuppiAsDefault_)
   fillRecoJetArray (RecoJetsAll, *reader_) ;
  else 
   fillPuppiJetArray (RecoJetsAll, *reader_) ;

  // take jets                                                                                                                                                             
  vector<jetContainer> RecoJets;
  RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt_, cutEvent_.bTagVeto, cutEvent_.jetPUID, minPtLeptonCutCleaning_, matchingCone_);

  asimL = (leptonsIsoTight.at(0).lepton4V_.Pt()-leptonsIsoTight.at(1).lepton4V_.Pt())/(leptonsIsoTight.at(0).lepton4V_.Pt()+leptonsIsoTight.at(1).lepton4V_.Pt());

  if(RecoJets.size() >= 2){
    L_dijet  = RecoJets.at(0).jet4V_ + RecoJets.at(1).jet4V_;
    asimJ    = (RecoJets.at(0).jet4V_.Pt()-RecoJets.at(1).jet4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()+RecoJets.at(1).jet4V_.Pt()) ;
    Rvar     = (leptonsIsoTight.at(0).lepton4V_.Pt()*leptonsIsoTight.at(1).lepton4V_.Pt())/(RecoJets.at(0).jet4V_.Pt()*RecoJets.at(1).jet4V_.Pt()) ;
    aveEta    = 0.5 * (RecoJets.at(0).jet4V_.Eta() + RecoJets.at(1).jet4V_.Eta());
  }


  // track jet info                                                                                                                                                          
  float TKJ_SumHT = 0.,TKJ_SumHT_IN = 0., TKJ_SumHT_OUT = 0. ;
  int   TKJ_num   = 0, TKJ_num_IN   = 0,  TKJ_num_OUT  = 0 ;

  if(RecoJets.size() >=2){

    float deltaRThreshold = 0.5 ;

    vector<jetContainer> trackJetsAll;
    fillTrackJetArray (trackJetsAll,*reader_) ;
    vector<jetContainer> trackJets ;
    trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning_, matchingCone_);
    float TJ_etaMin = RecoJets.at (0).jet4V_.Eta () ;
    float TJ_etaMax = RecoJets.at (1).jet4V_.Eta () ;
    float TJ_phiMin = RecoJets.at (0).jet4V_.Phi () ;
    float TJ_phiMax = RecoJets.at (1).jet4V_.Phi () ;
    if (TJ_etaMin > TJ_etaMax){
      swap (TJ_etaMin, TJ_etaMax) ;
      swap (TJ_phiMin, TJ_phiMax) ;
    }


    // loop over track jets                                                                                                                                                  
    for (size_t iJet = 0 ; iJet < trackJets.size () ; ++iJet){

      float iJetPhi = trackJets.at (iJet).jet4V_.Phi () ;
      float iJetEta = trackJets.at (iJet).jet4V_.Eta () ;
      float iJetPt  = trackJets.at (iJet).jet4V_.Pt () ;

      float dR2_Min = deltaPhi(TJ_phiMin, iJetPhi);
      dR2_Min *= dR2_Min ;
      dR2_Min += (iJetEta - TJ_etaMin) * (iJetEta - TJ_etaMin) ;
      float dR2_Max = deltaPhi (TJ_phiMax, iJetPhi) ;
      dR2_Max *= dR2_Max ;
      dR2_Max += (iJetEta - TJ_etaMax) * (iJetEta - TJ_etaMax) ;

      // veto the tag jets                                                                                                                                                  
      if (sqrt(dR2_Max) < deltaRThreshold || sqrt(dR2_Min) < deltaRThreshold) continue ;

      float iJetModPhi = iJetPhi ;
      float iJetZep    = (trackJets.at (iJet).jet4V_.Eta () - aveEta) /(TJ_etaMax - TJ_etaMin);
      if (iJetZep < -0.5)     iJetModPhi -= TJ_phiMin ;
      else if (iJetZep > 0.5) iJetModPhi -= TJ_phiMax ;

      ++TKJ_num ;
      TKJ_SumHT += iJetPt ;

      if (iJetEta > TJ_etaMin && iJetEta < TJ_etaMax){
	++TKJ_num_IN ;
	TKJ_SumHT_IN += iJetPt ;
      }

      else if (iJetEta < TJ_etaMin || iJetEta > TJ_etaMax){
	++TKJ_num_OUT ;
	TKJ_SumHT_OUT += iJetPt ;
      }
    }
  }
      
  // loop on the variable and find the values
  for(size_t iVar = 0; iVar < variableList.size(); iVar++){

    if(variableList.at(iVar) == "numTkjets" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_num));
    }

    else if(variableList.at(iVar) == "numTkjets_In" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_num_IN));
    }

    else if(variableList.at(iVar) == "numTkjets_Out" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_num_OUT));
    }

    else if(variableList.at(iVar) == "HTtkjets" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_SumHT));
    }

    else if(variableList.at(iVar) == "HTtkjets_In" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_SumHT_IN));
    }

    else if(variableList.at(iVar) == "HTtkjets_Out" and RecoJets.size() >= 2){
      variableValue.push_back(float(TKJ_SumHT_OUT));
    }

    else if(variableList.at(iVar) == "detajj" and RecoJets.size() >= 2){
      variableValue.push_back(float(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))); 
    }

    else if(variableList.at(iVar) == "detajj" and RecoJets.size() < 2){
      variableValue.push_back(float(-1)); 
    }
    else if(variableList.at(iVar) == "mjj" and RecoJets.size() >= 2){
      variableValue.push_back(float(L_dijet.M()));
    }
    else if(variableList.at(iVar) == "mjj" and RecoJets.size() < 2){
      variableValue.push_back(float(-1));
    }

    else if(variableList.at(iVar) == "etaj1etaj2" and RecoJets.size() >= 2){
      variableValue.push_back(float(RecoJets.at(0).jet4V_.Eta()*RecoJets.at(1).jet4V_.Eta()));
    }
    else if(variableList.at(iVar) == "etaj1etaj2" and RecoJets.size() < 2){
      variableValue.push_back(float(999));
    }

    else if(variableList.at(iVar) == "leadLepZep" and RecoJets.size() >= 2){
      variableValue.push_back(float((leptonsIsoTight.at(0).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))));
    }
    else if(variableList.at(iVar) == "leadLepZep" and RecoJets.size() < 2){
      variableValue.push_back(float(999));
    }
    else if(variableList.at(iVar) == "trailLepZep" and RecoJets.size() >= 2){
      variableValue.push_back(float((leptonsIsoTight.at(1).lepton4V_.Eta()-aveEta)/(fabs(RecoJets.at(0).jet4V_.Eta()-RecoJets.at(1).jet4V_.Eta()))));
    }
    else if(variableList.at(iVar) == "trailLepZep" and RecoJets.size() < 2){
      variableValue.push_back(float(999));
    }

    else if(variableList.at(iVar) == "DeltaPhi_LL"){
      variableValue.push_back(float(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_))));
    }
    else if(variableList.at(iVar) == "mll"){
      variableValue.push_back(float(L_dilepton.M()));
    }
    else if(variableList.at(iVar) == "mlljj" and RecoJets.size() >=2){
      variableValue.push_back(float((L_dilepton+L_dijet).M()));
    }
    else if(variableList.at(iVar) == "mlljj" and RecoJets.size()  < 2){
      variableValue.push_back(float(-1));
    }

    else if(variableList.at(iVar) == "mlljjmet" and RecoJets.size() >=2){
      variableValue.push_back(float((L_dilepton+L_dijet+L_met).M()));
    }
    else if(variableList.at(iVar) == "mlljjmet" and RecoJets.size()  < 2){
      variableValue.push_back(float(-1));
    }

    else if(variableList.at(iVar) == "mTH"){
      variableValue.push_back(float(sqrt(2*L_dilepton.Pt()*L_met.Pt()*(1-TMath::Cos(L_dilepton.DeltaPhi(L_met))))));
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
      variableValue.push_back(float(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(leptonsIsoTight.at(1).lepton4V_))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LMet"){
      variableValue.push_back(float(fabs(leptonsIsoTight.at(0).lepton4V_.DeltaPhi(L_met))));
    }     
    else if(variableList.at(iVar) == "DeltaPhi_LLMet"){
      variableValue.push_back(float(fabs(L_dilepton.DeltaPhi(L_met))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_TLMet"){
      variableValue.push_back(float(fabs(leptonsIsoTight.at(1).lepton4V_.DeltaPhi(L_met))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJ" and RecoJets.size()>=2){
      variableValue.push_back(float(fabs(RecoJets.at(0).jet4V_.DeltaPhi(RecoJets.at(1).jet4V_))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJ" and RecoJets.size()<2){
      variableValue.push_back(float(-999));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJMet" and RecoJets.size()>=2){
      variableValue.push_back(float(fabs(L_dijet.DeltaPhi(L_met))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_JJMet" and RecoJets.size()<2){
      variableValue.push_back(float(-999));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LJMet" and RecoJets.size()>=1){
      variableValue.push_back(float(fabs(RecoJets.at(0).jet4V_.DeltaPhi(L_met))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_LJMet" and RecoJets.size()<1){
      variableValue.push_back(float(-999));
    }
    else if(variableList.at(iVar) == "DeltaPhi_TJMet" and RecoJets.size() >=2){
      variableValue.push_back(float(fabs(RecoJets.at(1).jet4V_.DeltaPhi(L_met))));
    }
    else if(variableList.at(iVar) == "DeltaPhi_TJMet" and RecoJets.size() <2){
      variableValue.push_back(float(-999));
    }
    else if(variableList.at(iVar) == "npu"){
      variableValue.push_back(float(reader_-> npu));
    }
    else if(variableList.at(iVar) == "ptj1" and RecoJets.size()>=1){
      variableValue.push_back(float(RecoJets.at(0).jet4V_.Pt()));
    }
    else if(variableList.at(iVar) == "ptj1" and RecoJets.size()<1){
      variableValue.push_back(float(-999));
    }
    else if(variableList.at(iVar) == "ptj2" and RecoJets.size()>=2){
      variableValue.push_back(float(RecoJets.at(1).jet4V_.Pt()));
    }
    else if(variableList.at(iVar) == "ptj2" and RecoJets.size()<2){
      variableValue.push_back(float(-999));
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
    else if(variableList.at(iVar) == "etal1etal2"){
      variableValue.push_back(float(leptonsIsoTight.at(0).lepton4V_.Eta()*leptonsIsoTight.at(1).lepton4V_.Eta()));
    }
    else if(variableList.at(iVar) == "DeltaEta_LL"){
      variableValue.push_back(float(fabs(leptonsIsoTight.at(0).lepton4V_.Eta()-leptonsIsoTight.at(1).lepton4V_.Eta())));
    }

    //        else if(variableList.at(iVar) == "weight"){
    //	  variableValue.push_back(float(reader_->weight));
    //}
    else if(variableList.at(iVar) == "1"){
      variableValue.push_back(float(1));
    }
  }
}

void TMVATrainingClass::copyDir(TDirectory *source){
  //copy all objects and subdirs of directory source as a subdir of the current directory                                                                                       
  TDirectory *savdir = gDirectory;
  TDirectory *adir = savdir->mkdir(source->GetName());
  
 
  //loop on all entries of this directory                                                                                                                                       
  TKey *key;
  TIter nextkey(source->GetListOfKeys());  
  while ((key = (TKey*)nextkey())) {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    if (cl->InheritsFrom(TDirectory::Class())) {
      source->cd(key->GetName());
      TDirectory *subdir = gDirectory;
      if(adir!=0) adir->cd();
      copyDir(subdir);
      if(adir!=0) adir->cd();
    } else if (cl->InheritsFrom(TTree::Class())) {
      TTree *T = (TTree*)source->Get(key->GetName());
      adir->cd();
      TTree *newT = T->CloneTree(-1,"fast");
      newT->Write();
    } else {
      source->cd();
      TObject *obj = key->ReadObj();
      if(adir!=0) adir->cd();
      obj->Write();
      delete obj;
    }
  }
 
  if(adir!=0) adir->SaveSelf(kTRUE);
  savdir->cd();
}

void TMVATrainingClass::copyFile(TDirectory* f, TFile* outputFile) {
  //Copy all objects and subdirs of file fname as a subdir of the current directory                                                                                             
  TDirectory *target = outputFile;
  target->cd();
  copyDir(f);
  target->Close();
  outputFile->Close();  
}
