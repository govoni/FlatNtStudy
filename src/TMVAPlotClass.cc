#include "TMVAPlotClass.h"

using namespace std;

// default contructor
TMVAPlotClass::TMVAPlotClass(){

  // ROC plots 
  cROC_        = NULL;
  frameROC_    = NULL;
  cROCLog_     = NULL;
  frameROCLog_ = NULL;
  legROC_      = NULL;
  bannerROC_   = NULL;

  // MVA output, correlation and Significance 
  cMVAs_                  = NULL ;
  cCorrelationSignal_     = NULL ;
  cCorrelationBackground_ = NULL ;
  cSignificance_          = NULL ;

  // method informations 
  color_index_  = 0;
  method_index_ = 0;
  mvas_index_   = 0;
  thisMethod_   = 0 ;

  // histograms
  histoSignal_     = NULL ;
  histoBackground_ = NULL ;
  effSignal_       = NULL ;
  effBackground_   = NULL ;

  signalType_     = false ;
  backgroundType_ = false ;

  inputMethodName_.clear();
  originalMethodName_.clear();
  
  (*this).DestroyCanvases();

}

TMVAPlotClass::~TMVAPlotClass(){

  if(cROC_    != NULL)   
    cROC_->Delete();
  if(frameROC_ != NULL)     
    frameROC_->Delete();
  if(cROCLog_    != NULL)  
    cROCLog_->Delete();
  if(frameROCLog_ != NULL)  
    frameROCLog_->Delete();
  if(legROC_  != NULL)     
    legROC_->Delete();
  if(bannerROC_ != NULL)       
    bannerROC_->Delete();

  if(cMVAs_ != NULL)                   
    cMVAs_->Delete();
  if(cCorrelationSignal_ != NULL)      
    cCorrelationSignal_->Delete();
  if(cCorrelationBackground_ != NULL)  
    cCorrelationBackground_->Delete();
  if(cSignificance_!= NULL)            
    cSignificance_->Delete();

  if(histoSignal_!=NULL)      
    histoSignal_->Delete();
  if(histoBackground_!=NULL)  
    histoBackground_->Delete();
  if(effSignal_!=NULL)        
    effSignal_->Delete();
  if(effBackground_!=NULL)    
    effBackground_->Delete();

  for_each(inputFiles_.begin(),inputFiles_.end(), default_delete<TFile>());
  inputFiles_.clear() ;

  for_each(fSignificanceBox_.begin(),fSignificanceBox_.end(), default_delete<significanceBox>());
  fSignificanceBox_.clear() ;

  for_each(canvasLabel_.begin(),canvasLabel_.end(), default_delete<TLatex>());
  canvasLabel_.clear() ;

  inputMethodName_.clear() ;
  originalMethodName_.clear();
    
}

// checks if file with name "fin" is already open, and if not opens one
void TMVAPlotClass::openFileInput( const string & fin ){

 TFile* file = gDirectory->GetFile(); // take the file from the current gDirectory, TFile heredit form TDirectory
 if (file == 0 || fin != file->GetName()) { // if empty or != from fin, close the file and read the new one
   if (file != 0) {
     gROOT->cd();
     file->Close();
   }

  cout << "--- Opening root file " << fin << " in read mode" << endl;

  file = TFile::Open( fin.c_str(), "READ" );
 }
 else file = gDirectory->GetFile(); // otherwise take the file from the directory
      
 inputFiles_.push_back(file); // fill the vector

 return ;

}

// open file from a list of file names eliminating the old content
void TMVAPlotClass::openFileInput( const vector<string> & fin ){

  
  for_each(inputFiles_.begin(),inputFiles_.end(), default_delete<TFile>());
  inputFiles_.clear() ;

  for(size_t iFile = 0 ; iFile < fin.size(); iFile++){ // loop on the input vector
    cout << "--- Opening root file " << fin.at(iFile) << " in read mode" << endl; // open the fie in read mode
    inputFiles_.push_back(TFile::Open( fin.at(iFile).c_str(), "READ"));
 }

 return ;

}

// Get the input file from the name
TFile* TMVAPlotClass::GetInputFile(const string & fin){

  for(size_t iFile = 0 ; iFile < inputFiles_.size(); iFile++){ // loop on the input file vector
    if(inputFiles_.at(iFile)->GetName() == fin)  // check if exsist
      return inputFiles_.at(iFile) ;
  }

  return 0 ;
}

// get input file vector and not a single file
vector<TFile*> TMVAPlotClass::GetInputFile(){ // return the full list
  return inputFiles_ ;
}


// destroy the full list of the exsisting canvas
void TMVAPlotClass::DestroyCanvases(){

  TList* canvasList = (TList*) gROOT->GetListOfCanvases(); // take the canvas list
  TListIter itCan(canvasList); // iterator on a TList
  TObject *oCan;
  while ( (oCan = itCan())) delete oCan;
  return;
}

// create canvas, frame and legend for ROC plot
void TMVAPlotClass::CreateCanvasandFrameROC(const int & plotType, 
					    const float & puMin, 
					    const float & puMax){
                               

  cROC_ = new TCanvas((string("cROC")+string(inputFiles_.at(0)->GetName())).c_str(),"cROC",180,52,550,550); // make the canvas and set style
  cROC_->cd();
  cROC_->SetTicks();
  cROC_->SetFillColor(0);
  cROC_->SetBorderMode(0);
  cROC_->SetBorderSize(2);  
  cROC_->SetTickx(1);
  cROC_->SetTicky(1);
  cROC_->SetRightMargin(0.05);
  cROC_->SetBottomMargin(0.12);
  cROC_->SetFrameBorderMode(0);


  frameROC_ = new TH2F((string("frameROC")+string(inputFiles_.at(0)->GetName())).c_str(),"",500,0,1,500,0,1); // make the frame for the ROC
  frameROC_->SetLineWidth(2);
  frameROC_->SetMarkerStyle(21);
  frameROC_->SetMarkerSize(0.3);
  frameROC_->GetXaxis()->SetNdivisions(405);
  frameROC_->GetYaxis()->SetNdivisions(405);
  frameROC_->GetXaxis()->SetTitle("#varepsilon_{sig}"); 
  frameROC_->GetXaxis()->SetLabelOffset(0.012);
  frameROC_->GetXaxis()->SetLabelSize(0.042);
  frameROC_->GetXaxis()->SetTitleSize(0.05);
  frameROC_->GetXaxis()->SetTitleOffset(1.05);
  if(plotType == 0) // plotType 0 means effsig vs eff backg
   frameROC_->GetYaxis()->SetTitle("#varepsilon_{bkg}");
  else // plotType 1 means effsign vs 1-eff backg
   frameROC_->GetYaxis()->SetTitle("1-#varepsilon_{bkg}");
  frameROC_->GetYaxis()->SetLabelOffset(0.012);
  frameROC_->GetYaxis()->SetLabelSize(0.042);
  frameROC_->GetYaxis()->SetTitleSize(0.05);
  frameROC_->GetYaxis()->SetTitleOffset(1.25);
  frameROC_->Draw("");

  banner4Plot(false,puMin,puMax,plotType); // make the banner for the ROC curve

  for_each(canvasLabel_.begin(),canvasLabel_.end(), default_delete<TLatex>());
  canvasLabel_.clear() ;

  canvasLabel_.push_back(new TLatex(0.95,0.92," 14 TeV"));
  canvasLabel_.back()->SetNDC();
  canvasLabel_.back()->SetTextAlign(31);
  canvasLabel_.back()->SetTextFont(42);
  canvasLabel_.back()->SetTextSize(0.04);
  canvasLabel_.back()->SetLineWidth(2);
  canvasLabel_.back()->Draw();
  canvasLabel_.push_back(new TLatex(0.13,0.92,"Delphes"));
  canvasLabel_.back()->SetNDC();
  canvasLabel_.back()->SetTextFont(61);
  canvasLabel_.back()->SetTextSize(0.04);
  canvasLabel_.back()->SetLineWidth(2);
  canvasLabel_.back()->Draw();
  canvasLabel_.push_back(new TLatex(0.2824,0.92,"Simulation Preliminary"));
  canvasLabel_.back()->SetNDC();
  canvasLabel_.back()->SetTextFont(52);
  canvasLabel_.back()->SetTextSize(0.0304);
  canvasLabel_.back()->SetLineWidth(2);
  canvasLabel_.back()->Draw();
  
  cROCLog_ = new TCanvas((string("cROCLog")+string(inputFiles_.at(0)->GetName())).c_str(),"cROC",180,52,550,550); //Log canvas
  cROCLog_->cd();
  cROCLog_->SetTicks();
  cROCLog_->SetFillColor(0);
  cROCLog_->SetBorderMode(0);
  cROCLog_->SetBorderSize(2);
  
  cROCLog_->SetTickx(1);
  cROCLog_->SetTicky(1);
  cROCLog_->SetRightMargin(0.05);
  cROCLog_->SetBottomMargin(0.12);
  cROCLog_->SetFrameBorderMode(0);

  frameROCLog_ = new TH2F((string("frameROCLog")+string(inputFiles_.at(0)->GetName())).c_str(),"",500,0.0001,1,500,0.0001,1); // log frame
  frameROCLog_->SetLineWidth(2);
  frameROCLog_->SetMarkerStyle(21);
  frameROCLog_->SetMarkerSize(0.3);
  frameROCLog_->GetXaxis()->SetNdivisions(405);
  frameROCLog_->GetYaxis()->SetNdivisions(405);
  frameROCLog_->GetXaxis()->SetTitle("#varepsilon_{sig}");
  frameROCLog_->GetXaxis()->SetLabelOffset(0.012);
  frameROCLog_->GetXaxis()->SetLabelSize(0.042);
  frameROCLog_->GetXaxis()->SetTitleSize(0.05);
  frameROCLog_->GetXaxis()->SetTitleOffset(1.05);
  frameROCLog_->GetYaxis()->SetTitle("#varepsilon_{bkg}");
  frameROCLog_->GetYaxis()->SetLabelOffset(0.012);
  frameROCLog_->GetYaxis()->SetLabelSize(0.042);
  frameROCLog_->GetYaxis()->SetTitleSize(0.05);
  frameROCLog_->GetYaxis()->SetTitleOffset(1.25);
  frameROCLog_->Draw("");

  banner4Plot(false,puMin,puMax,plotType);

  for(vector<TLatex*>::iterator itLabel = canvasLabel_.begin(); itLabel!=canvasLabel_.end(); itLabel++)  
    (*itLabel)->Draw();
 
  if(plotType == 0) // make the legend in the bottom part of the canvas
   legROC_ = new TLegend(0.17,0.43,0.56,0.87,NULL,"brNDC");
  else // make the legend in the upper part of the canvas
   legROC_ = new TLegend(0.17,0.15,0.55,0.66,NULL,"brNDC");

  legROC_->SetBorderSize(0);
  legROC_->SetTextSize(0.033);
  legROC_->SetTextFont(42);
  legROC_->SetLineColor(1);
  legROC_->SetLineStyle(1);
  legROC_->SetLineWidth(2);
  legROC_->SetFillColor(19);
  legROC_->SetFillStyle(0);
}

// Produce a banner for ROC plots 
void TMVAPlotClass::banner4Plot (const bool & isLabel, 
				 const float & puMin, 
				 const float & puMax, 
				 const int & plotType){

  if(plotType == 0)
   bannerROC_ = new TPaveText(.23,0.26,.30,.41,"NDC"); 
  else
   bannerROC_ = new TPaveText(.57,0.21,.64,.36,"NDC"); 

  TString PULegend ;
  PULegend.Form("%d < N_{PU} < %d ",int(puMin),int(puMax));
  bannerROC_->AddText(PULegend.Data());
  bannerROC_->AddText("|#eta| < 2.5");

  bannerROC_->SetBorderSize(0);
  bannerROC_->SetFillColor(0);
  bannerROC_->SetTextSize(0.027);
  bannerROC_->SetFillStyle(0);
  bannerROC_->SetLineColor(1);
  bannerROC_->SetLineStyle(1);
  bannerROC_->SetLineWidth(0);
  bannerROC_->SetMargin(0);
  bannerROC_->SetTextFont(42);
  bannerROC_->SetShadowColor(0);
  bannerROC_->Draw();
}

// used to create output file for canvas
void TMVAPlotClass::PrintImageROC(TDirectory* dir, 
				  const string & outputPlotDirectory){

  string fname = outputPlotDirectory + "ROCcurve";

  if (TString(dir->GetName()).Contains("multicut")){ // look if multicut is inside
    TString fprepend(dir->GetName());
    fprepend.ReplaceAll("multicutMVA_","");
    fname = outputPlotDirectory + fprepend + "_" + "ROCcurve";
  }

  if (cROC_ == NULL){
     cout << "*** Error in TMVAPlotClass::PrintImageROC: canvas is NULL" << endl; 
     return ;
  }
  
  // create directory if not existing
  TString f = fname;
  TString dirName = f.Remove(f.Last('/'),f.Length()-f.Last( '/' ));
  gSystem->mkdir( dirName );

  TString pdfName  = fname + ".pdf";
  TString pngName  = fname + ".png";
  TString rootName = fname + ".root";
  cROC_->cd();
  cROC_->Print(pdfName);
  cROC_->Print(pngName);
  cROC_->Print(rootName);

  TString pdfNameLog  = fname + "_Log.pdf";
  TString pngNameLog  = fname + "_Log.png";
  TString rootNameLog = fname + "_Log.root";
  cROCLog_->cd();
  cROCLog_->SetLogy();
  cROCLog_->Print(pdfNameLog);
  cROCLog_->Print(pngNameLog);
  cROCLog_->Print(rootNameLog);

  return ;
}

// Methods to print image given canvas and a output name
void TMVAPlotClass::PrintImage(TCanvas* c, 
			       const string & fname){

  TString pngName  = fname+".png";
  TString pdfName  = fname+".pdf";
  TString rootName = fname+".root";

  c->Print(pdfName);
  c->Print(pngName);
  c->Print(rootName);

  return ;

}

// function to normalize a histo also if it has weights
void TMVAPlotClass::NormalizeHist(TH1F* h) { 

 if (h==0) return;
 if (h->GetSumw2N() == 0) h->Sumw2(); // apply sumW2 before scaling the histo
 if (h->GetSumOfWeights()!=0) {
   float dx = (h->GetXaxis()->GetXmax()-h->GetXaxis()->GetXmin())/h->GetNbinsX(); // take the bin width
   h->Scale( 1.0/h->GetSumOfWeights()/dx ); // scale for the bin width --> entries if no weights are applied
 }
 return ;
}

void TMVAPlotClass::NormalizeHists( TH1F* sig, TH1F* bkg) {

 if (sig && sig->GetSumw2N() == 0) sig->Sumw2();
 if (bkg && bkg->GetSumw2N() == 0) bkg->Sumw2();
      
 if(sig->GetSumOfWeights()!=0) {
     float dx = (sig->GetXaxis()->GetXmax()-sig->GetXaxis()->GetXmin())/sig->GetNbinsX();
     sig->Scale( 1.0/sig->GetSumOfWeights()/dx );
 }

 if (bkg != 0 && bkg->GetSumOfWeights()!=0) {
     float dx = (bkg->GetXaxis()->GetXmax()-bkg->GetXaxis()->GetXmin())/bkg->GetNbinsX();
     bkg->Scale( 1.0/bkg->GetSumOfWeights()/dx);
 }

 return ;

}

// the following are tools to help handling different methods and titles
void TMVAPlotClass::GetMethodName( TString & name, 
				   TKey * mkey ) { // from a string and a key take only the method name
   if (mkey == 0) return;
   name = mkey->GetName();
   name.ReplaceAll("Method_","");
   return ;
}

void TMVAPlotClass::GetMethodTitle( TString & name, 
				    TKey * ikey ) { // from a string and a key take only the full name
   if (ikey == 0) return;
   name = ikey->GetName();
   return ;
}

void TMVAPlotClass::GetMethodName( TString & name, 
				   TDirectory * mdir ) {
   if (mdir == 0) return;
   name = mdir->GetName();
   name.ReplaceAll("Method_","");
   return ;
}

void TMVAPlotClass::GetMethodTitle( TString & name, 
				    TDirectory * idir ) {
   if (idir == 0) return;
   name = idir->GetName();
   return ;

}


// fill a TList with all the methods
int TMVAPlotClass::GetListOfMethods( TList & methods, 
				     TDirectory *dir){

 if (dir==0) dir = gDirectory;
 TIter mnext(dir->GetListOfKeys()); // take the list of keys in the directory
 TKey *mkey;
 methods.Clear();
 methods.SetOwner(kFALSE);
 int ni = 0;

 while ((mkey = (TKey*)mnext())) { // make sure, that we only look at TDirectory with name Method_<xxx>
   TString name = mkey->GetClassName();
   TClass *cl   = gROOT->GetClass(name);
   if (cl->InheritsFrom("TDirectory")) { // look at the different directories of the method in the TMVA output
     if (TString(mkey->GetName()).BeginsWith("Method_")) {
       methods.Add(mkey);
       ni++;
     }
   }
 }
 return ni;
}


// get a list of titles (i.e TDirectory) given a method dir
int TMVAPlotClass::GetListOfTitles( TDirectory *rfdir, 
				    TList & titles ){

 int ni = 0;
 if (rfdir==0) return 0;

 TList *keys = rfdir->GetListOfKeys(); // take the list of key of the directory

 if (keys==0) {
   cout << "+++ Directory '" << rfdir->GetName() << "' contains no keys" << endl;
   return 0;
 }

 TIter rfnext(keys);
 TKey *rfkey;
 titles.Clear();
 titles.SetOwner(kFALSE);

 while ((rfkey = (TKey*)rfnext())) { // make sure, that we only look at histograms
   TClass *cl = gROOT->GetClass(rfkey->GetClassName());
   if (cl->InheritsFrom("TDirectory")) {
     titles.Add(rfkey);
     ni++;
   }
 }

 return ni;
 
}


// get the list of all titles for a given method --> if the input dir is 0, gDirectory is used returns a list of keys
int TMVAPlotClass::GetListOfTitles( TString & methodName, 
				    TList & titles, 
				    TDirectory *dir){

  if (dir==0) dir = gDirectory;
  TDirectory* rfdir = (TDirectory*)dir->Get( methodName );
  if (rfdir==0) {
    cout << "+++ Could not locate directory '" << methodName << endl;
    return 0;
  }

  return (*this).GetListOfTitles(rfdir,titles);
}

// Next key iterator matching the className
TKey *TMVAPlotClass::NextKey(TIter & keyIter, 
			     string className) {

 TKey *key  = (TKey *) keyIter.Next();
 TKey *rkey = 0;
 bool loop  = (key!=0);

 while (loop) {
   TClass *cl = gROOT->GetClass(key->GetClassName());
   if (cl->InheritsFrom(className.c_str())) { 
     loop = kFALSE;                                             
     rkey = key;
   } 
   else {
     key = (TKey *)keyIter.Next();
     if (key==0) loop = kFALSE;
   }
 }
 return rkey;
}


// return the name of the directory which contains output MVA plots 
TDirectory *TMVAPlotClass::GetInputVariablesDir( TMVAPlotClass::TypeOfPlot type, 
						 TDirectory *dir){

 // get the InputVariables directory
 const TString directories[TMVAPlotClass::kNumOfMethods] = { "InputVariables_Id",
							     "InputVariables_Deco",
							     "InputVariables_PCA",
							     "InputVariables_Gauss_Deco" };
 if (dir==0) dir = gDirectory;

 // get top dir containing all hists of the variables
 dir = (TDirectory*)gDirectory->Get(directories[type]);
 if (dir==0) {
   cout << "+++ Could not locate input variable directory '" << directories[type] << endl;
   return 0;
 }
 
 return dir;

}


// Set and store the method name in a vector in order to be used in the legend 
void TMVAPlotClass::SetMethodName(const vector<string> & SetMethodName){

  inputMethodName_    = SetMethodName;
  originalMethodName_ = SetMethodName;

  for(size_t iMethod = 0 ; iMethod < inputMethodName_.size() ; iMethod++){
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll(":_:","#"));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll(":__:","_{"));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll(":___:","}"));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll("//"," "));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll("/../","("));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll("/::/",")"));
    inputMethodName_.at(iMethod) = string(TString(inputMethodName_.at(iMethod)).ReplaceAll("/:::/","="));
  }       

  for(size_t iMethod = 0 ; iMethod < originalMethodName_.size() ; iMethod++){
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll(":_:","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll(":__:","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll(":___:","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll("//","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll("/","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll("/../","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll("/::/","_"));
    originalMethodName_.at(iMethod) = string(TString(originalMethodName_.at(iMethod)).ReplaceAll("/:::/","_"));
  }       

  return ; 
}

void TMVAPlotClass::SetInputFiles (const vector<TFile*> & inputFiles){ 

  for_each(inputFiles_.begin(),inputFiles_.end(), default_delete<TFile>());
  inputFiles_.clear() ;
  inputFiles_ = inputFiles; 
}

// methods to read efficiency histograms for each method in order to do significance plot
void TMVAPlotClass::ReadHistograms (TFile* inputFile){

  fSignificanceBox_.clear();  

  // search for the right histograms in full list of keys                                                                                                                   
  TIter next(inputFile->GetListOfKeys());
  TKey *key(0);

  while( (key = (TKey*)next()) ) { // loop on the key of the file

    if(!TString(key->GetName()).BeginsWith("Method_")) continue; // skip method directories
    if(!gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory") ) continue;
    TDirectory* mDir = (TDirectory*)key->ReadObj(); //take the object
    
    TIter keyIt(mDir->GetListOfKeys());
    TKey *titkey;
    int maxLenTitle = 0 ;

    while((titkey = (TKey*)keyIt())) { // loop on the inner keys
      if(!gROOT->GetClass(titkey->GetClassName())->InheritsFrom("TDirectory") ) continue;
      TDirectory* titDir = (TDirectory *)titkey->ReadObj();      

      fSignificanceBox_.push_back(new significanceBox());      

      (*this).GetMethodName(fSignificanceBox_.back()->methodName_,key); // get method name 
      (*this).GetMethodTitle(fSignificanceBox_.back()->methodTitle_,titDir); // get method title

      if (fSignificanceBox_.back()->methodTitle_.Length() > maxLenTitle) 
	maxLenTitle = fSignificanceBox_.back()->methodTitle_.Length();

      TString hname = "MVA_"+fSignificanceBox_.back()->methodTitle_;

      cout << "--- Classifier: " << fSignificanceBox_.back()->methodTitle_ << endl;
      
      if(histoBackground_ == NULL ) 
	fSignificanceBox_.back()->Background_ = dynamic_cast<TH1*>(titDir->Get(hname+"_B"));
      else 
	fSignificanceBox_.back()->Background_ = histoBackground_ ;

      if(histoSignal_ == NULL)      
	fSignificanceBox_.back()->Signal_ = dynamic_cast<TH1*>(titDir->Get( hname + "_S" ));
      else 
	fSignificanceBox_.back()->Signal_ = histoSignal_ ;

      if(effBackground_ == NULL ) 
	fSignificanceBox_.back()->efficiencyBackground_ = dynamic_cast<TH1*>(titDir->Get( hname + "_effB" ));
      else 
	fSignificanceBox_.back()->efficiencyBackground_ = effBackground_ ;

      if(effSignal_ ==NULL)  
	fSignificanceBox_.back()->efficiencySignal_  = dynamic_cast<TH1*>(titDir->Get( hname + "_effS" ));
      else 
	fSignificanceBox_.back()->efficiencySignal_ = effSignal_ ;
     
      if (fSignificanceBox_.back()->efficiencyBackground_ == NULL || fSignificanceBox_.back()->efficiencySignal_ == NULL){ 
	delete fSignificanceBox_.back(); 
	continue; 
      }
    }
  }  
 return;
}

void TMVAPlotClass::SetFormula(const TString & f){
  fFormula_ = f;
}

// get a different string for the formula in order to be used in a TFormula object
TString TMVAPlotClass::GetFormula(){

  TString f = fFormula_;
  f.ReplaceAll("epsilonS","x");
  f.ReplaceAll("S","x");
  f.ReplaceAll("epsilonB","y");
  f.ReplaceAll("B","y"); 
  return f;
}

TString TMVAPlotClass::GetFormulaString(){
  return fFormula_;
}

// get the latex string for the formula
TString TMVAPlotClass::GetLatexFormula(){

  TString f = fFormula_;
  if (signalType_ == true) 
   f.ReplaceAll("S","#epsilon_{S}");
  if (backgroundType_ == true) 
   f.ReplaceAll("B","#epsilon_{B}");   

  f.ReplaceAll("(","{");
  f.ReplaceAll(")","}");
  f.ReplaceAll("sqrt","#sqrt");

  return f;
}

// set if use just signal or bkg efficiency or multiply them for the expected number of events before the training to get the real significance
void TMVAPlotClass::SetSignalType    (const bool & type) { 
  signalType_ = type; 
}

void TMVAPlotClass::SetBackgroundType(const bool & type) { 
  backgroundType_ = type; 
}


// methods for plot the efficiency (ROC) curve for a given inputFile
void TMVAPlotClass::makeROCsPlot (TDirectory* dir, 
				  const int & plotType,
				  const float & puMin, 
				  const float & puMax, 
				  const string & outputPlotDirectory){

  cout<<"TMVAPlotClass::makeROCsPlot --> start the method "<<endl;

  // Plot the ROC curve with a proper style from root file originated by TMVA                                                                                             
  if(cROC_ == NULL) 
    (*this).CreateCanvasandFrameROC(plotType,puMin,puMax); 
      
  TH1F *h = NULL; 

  for(size_t iFile = 0; iFile <  inputFiles_.size();  iFile++){ // loop on the input file list 
   inputFiles_.at(iFile)->cd();
   TList TrainingMethods;
   TList hists;
   int res = (*this).GetListOfMethods(TrainingMethods); // take the methods used for this training

   if(res == 0) 
     cout<<"TMVAPlotClass::makeROCsPlot --> no methods found "<<endl ;
   
   TIter next(&TrainingMethods);
   TKey *key = 0, *hkey = 0;
   cout<<"TMVAPlotClass::makeROCsPlot training file : "<<inputFiles_.at(iFile)->GetName()<<endl;
   
   // loop over all methods stored in the TList TrainingMethods                                                                                                              
   while ((key = (TKey*)next())) {
     TDirectory *myDir = (TDirectory*)key->ReadObj();
     TList Titles;
     cout<<"TMVAPlotClass::makeROCsPlot training methods : "<<myDir->GetName()<<endl;

     int nTitles = (*this).GetListOfTitles(myDir,Titles); // get the titles list for eack method                                                                         
     if(nTitles == 0) 
       cout<<"TMVAPlotClass::makeROCsPlot --> no titles found "<<endl ;     

     TIter nextTitle(&Titles);
     TKey *titkey = 0;
     TDirectory *titDir = 0;
     
     while ((titkey = (*this).NextKey(nextTitle,"TDirectory"))) {

       titDir = (TDirectory*)titkey->ReadObj(); // read each object and take again the method title for each element of the list
       TString methodTitle;
       cout<<"TMVAPlotClass::makeROCsPlot title method : "<<titDir->GetName()<<endl;       
       (*this).GetMethodTitle(methodTitle,titDir);
       TIter nextKey( titDir->GetListOfKeys() ); // loop and the list of keys       
       while ((hkey = (*this).NextKey(nextKey,"TH1"))) { // take only the TH1 object type
	 h = (TH1F*)hkey->ReadObj();
	 TString hname = h->GetName();    // only the one which are called rejBvsS
	 h -> SetName(Form("%s_%s",inputFiles_.at(iFile)->GetName(),h->GetName()));		 
	 if(plotType == 0){ // plotType 0 means eff sig vs eff bkg
	   if(hname.Contains("effBvsS") && hname.BeginsWith("MVA_") && not hname.Contains("effBvsSLocal")) {
	     if(size_t(color_index_) <= vec_color.size()){
	       h->SetLineWidth(vec_linewidth[color_index_]);
	       h->SetLineColor(vec_color[color_index_]);
	       h->SetLineStyle(vec_linestyle[color_index_]);
	       cROC_->cd();
	       h->Draw("csame");
	       cROCLog_->cd();
	       h->Draw("csame");
	       hists.Add(h);
	       color_index_ = color_index_+1;
	       legROC_->AddEntry(h,inputMethodName_.at(method_index_).c_str(),"l");
	       method_index_ ++ ;
	     }
	     else{
	       h->SetLineWidth(vec_linewidth[color_index_-vec_color.size()]);
	       h->SetLineColor(vec_color[color_index_-vec_color.size()]);
	       h->SetLineStyle(vec_linestyle[color_index_-vec_color.size()]);
	       cROC_->cd();
	       h->Draw("csame");
	       cROCLog_->cd();
	       h->Draw("csame");
	       hists.Add(h);
	       color_index_ = color_index_+1;
	       legROC_->AddEntry(h,inputMethodName_.at(method_index_).c_str(),"l");
	       method_index_ ++ ;
	     }	
	   }
	 }
	 else{
	   if (hname.Contains("rejBvsS") && hname.BeginsWith("MVA_")) {
	     if(size_t(color_index_) <= vec_color.size()){
	       h->SetLineWidth(vec_linewidth[color_index_]);
	       h->SetLineColor(vec_color[color_index_]);
	       h->SetLineStyle(vec_linestyle[color_index_]);
	       cROC_->cd();
	       h->Draw("csame");
	       cROCLog_->cd();
	       h->Draw("csame");
	       hists.Add(h);
	       color_index_ = color_index_+1;
	       legROC_->AddEntry(h,inputMethodName_.at(method_index_).c_str(),"l");
	       method_index_ ++ ;
	     }
	     else{
	       h->SetLineWidth(vec_linewidth[color_index_-vec_color.size()]);
	       h->SetLineColor(vec_color[color_index_-vec_color.size()]);
	       h->SetLineStyle(vec_linestyle[color_index_-vec_color.size()]);
	       cROC_->cd();
	       h->Draw("csame");
	       cROCLog_->cd();
	       h->Draw("csame");
	       hists.Add(h);
	       color_index_ = color_index_+1;
	       legROC_->AddEntry(h,inputMethodName_.at(method_index_).c_str(),"l");
	       method_index_ ++ ;
	     }	
	   }	 
	 }       
       }               
     }   
   }
   /// Loop on the different histos                                                                                                                                          
   while (hists.GetSize()) {
     TListIter hIt(&hists); // define an iterator                                                                                                                         
     TH1F* hist(0);
     Float_t largestInt=-1;
     TH1F* histWithLargestInt(0);
     while ((hist = (TH1F*)hIt())!=0) {
       Float_t integral = hist->Integral(1,hist->GetNbinsX());
       if (integral>largestInt) {
	 largestInt = integral;
	 histWithLargestInt = hist;
       }
     }
     if (histWithLargestInt == 0) {
       cout << "ERROR - unknown hist \"histWithLargestInt\" --> serious problem in ROOT file" << endl;
       break;
     }    
      
     // set legend names      
     hists.Remove(histWithLargestInt);
   }   
  }
   
  cROC_->cd();  
  legROC_->Draw("same");
  cROC_->Update();
  
  return;
}

// method in order to plot correlation matrix betwenn input variables
void TMVAPlotClass::makeCorrelationMatrixPlot(TFile* inputFile, 
					      const string & inputName, 
					      const string & outputPlotDirectory){
  
  string nameCorrelationSignal     = "CorrelationMatrixS";
  string nameCorrelationBackground = "CorrelationMatrixB";

  TList TrainingMethods;
  int res = (*this).GetListOfMethods(TrainingMethods);
  if(res == 0) cout<<" TMVAPlotClass::plotCorrelationMatrix --> no methods found "<<endl ;

  if(TString(TrainingMethods.At(0)->GetName()).Contains("Cuts")) return ; // single cut are skipped

  TH2F* hSignal     = (TH2F*) inputFile->Get(nameCorrelationSignal.c_str());     // get the object from the file
  TH2F* hBackground = (TH2F*) inputFile->Get(nameCorrelationBackground.c_str()); // get the object from the file

  hSignal->SetName    (string(Form("%s_%s",nameCorrelationSignal.c_str(),inputName.c_str())).c_str());
  hBackground->SetName(string(Form("%s_%s",nameCorrelationBackground.c_str(),inputName.c_str())).c_str());
  
  if(hSignal == 0 || hBackground == 0){ 
    cerr<<" Null Pointer for correlation Matrix --> exit without plot "<<endl;  
    return ; 
  }

  cCorrelationSignal_ = new TCanvas(string(Form("c%s_%s",nameCorrelationSignal.c_str(),inputName.c_str())).c_str(),Form("Correlation Matrix Signal"),180,52,550,550);
  float newMargin1 = 0.13;
  float newMargin2 = 0.15;
  float newMargin3 = 0.20;

  cCorrelationSignal_->SetGrid();
  cCorrelationSignal_->SetTicks();
  cCorrelationSignal_->SetLeftMargin(newMargin3);
  cCorrelationSignal_->SetBottomMargin(newMargin2);
  cCorrelationSignal_->SetRightMargin(newMargin1);
  cCorrelationSignal_->SetTopMargin(newMargin1);
  
  gStyle->SetPaintTextFormat("3g");

  hSignal->SetMarkerSize(1.5);
  hSignal->SetMarkerColor(0);
  hSignal->GetXaxis()->SetLabelSize(0.035);
  hSignal->GetYaxis()->SetLabelSize(0.035);
  hSignal->LabelsOption("d");
  hSignal->SetLabelOffset(0.011);// label offset on x axis                                                                                                                 
  
  hBackground->SetMarkerSize(1.5);
  hBackground->SetMarkerColor(0);
  hBackground->GetXaxis()->SetLabelSize(0.035);
  hBackground->GetYaxis()->SetLabelSize(0.035);
  hBackground->LabelsOption("d");
  hBackground->SetLabelOffset( 0.011 );// label offset on x axis                                                                                                               

  // Plot correlation between signal      
  cCorrelationSignal_->cd();
  hSignal->Draw("colz");                                                                                                                                                   

  for( int iBinX = 0 ; iBinX<hSignal->GetNbinsX() ; iBinX++){ // check the bin label name and in case change them
     TString LabelBinName = hSignal->GetXaxis()->GetBinLabel(iBinX+1);
     LabelBinName.ReplaceAll("[0]","");
     hSignal->GetXaxis()->SetBinLabel(iBinX+1,LabelBinName);
     LabelBinName = hSignal->GetYaxis()->GetBinLabel(iBinX+1);
     LabelBinName.ReplaceAll("[0]","");
     hSignal->GetYaxis()->SetBinLabel(iBinX+1,LabelBinName);
  }

  hSignal->Draw("textsame"); 

  // add comment                                                                                                                                                      
  TText* text = new TText( 0.53, 0.88, "Linear correlation coefficients in %" );
  text->SetNDC();
  text->SetTextSize( 0.026 );
  text->AppendPad();
  cCorrelationSignal_->Update();

  // add comment                                                                                                                                                            
  TText* text2 = new TText( 0.3, 0.92, "Correlation Matrix for Signal Events" );
  text2->SetNDC();
  text2->SetTextSize( 0.033 );
  text2->AppendPad();
  cCorrelationSignal_->Update();

  nameCorrelationSignal     = string(Form("%s_%s",nameCorrelationSignal.c_str(),inputName.c_str()));
  nameCorrelationBackground = string(Form("%s_%s",nameCorrelationBackground.c_str(),inputName.c_str()));

  (*this).PrintImage(cCorrelationSignal_,outputPlotDirectory+"/"+nameCorrelationSignal);
    
  // Background correlation
  cCorrelationBackground_ = new TCanvas(string(Form("c%s_%s",nameCorrelationBackground.c_str(),inputName.c_str())).c_str(),Form("Correlation Matrix Signal"),180,52,550,550);
  cCorrelationBackground_->SetGrid();
  cCorrelationBackground_->SetTicks();
  cCorrelationBackground_->SetLeftMargin(newMargin3);
  cCorrelationBackground_->SetBottomMargin(newMargin2);
  cCorrelationBackground_->SetRightMargin(newMargin1);
  cCorrelationBackground_->SetTopMargin(newMargin1);

  cCorrelationBackground_->cd();
  hBackground->Draw("colz");                                                                                                                             

  for( int iBinX = 0 ; iBinX<hBackground->GetNbinsX() ; iBinX++){
     TString LabelBinName = hBackground->GetXaxis()->GetBinLabel(iBinX+1);
     LabelBinName.ReplaceAll("[0]","");
     hBackground->GetXaxis()->SetBinLabel(iBinX+1,LabelBinName);
     LabelBinName = hBackground->GetYaxis()->GetBinLabel(iBinX+1);
     LabelBinName.ReplaceAll("[0]","");
     hBackground->GetYaxis()->SetBinLabel(iBinX+1,LabelBinName);
  }

  hBackground->Draw("textsame");          
  // add comment                                                                                                                                                            
  text->AppendPad();

  // add comment                                                                                                                                                            
  TText* text3 = new TText( 0.3, 0.92, "Correlation Matrix for Background Events" );
  text3->SetNDC();
  text3->SetTextSize( 0.033 );
  text3->AppendPad();

  cCorrelationBackground_->Update();
  (*this).PrintImage(cCorrelationBackground_,outputPlotDirectory+"/"+nameCorrelationBackground);

  if(hSignal!=0)     delete hSignal;
  if(hBackground!=0) delete hBackground;
  if(text!=0)        delete text;

  return ;

}


// plot MVA output, probability and overtraining 
void TMVAPlotClass::makeMVAsPlot(TFile* inputFile,  
				 HistType htype, 
				 const string & outputPlotDirectory){

  TIter next(inputFile->GetListOfKeys());
  TKey *key(0);

  while ((key = (TKey*)next())) {
    if (!TString(key->GetName()).BeginsWith("Method_")) continue;
    if (!gROOT->GetClass(key->GetClassName())->InheritsFrom("TDirectory")) continue;

    TString methodName;
    (*this).GetMethodName(methodName,key); // take the method name for each key inside the input file

    TDirectory* mDir = (TDirectory*)key->ReadObj(); // take the list of jey of the selected object

    TIter keyIt(mDir->GetListOfKeys());
    TKey *titkey;

    while ((titkey = (TKey*)keyIt())) { // loop on the second list of keys

      if (!gROOT->GetClass(titkey->GetClassName())->InheritsFrom("TDirectory")) continue; // if is not a TDirectory object skipe

      TDirectory *titDir = (TDirectory *)titkey->ReadObj();
      TString methodTitle;
      (*this).GetMethodTitle(methodTitle,titDir); // get the tutke of the method (another directory found)
      cout << "--- Found directory for method: " << methodName << "::" << methodTitle << endl;

      TString hname = "MVA_" + methodTitle;
      if      (htype == ProbaType  ) hname += "_Proba";
      else if (htype == RarityType ) hname += "_Rarity";

      // take output distribution for signal and baclground
      histoSignal_     = dynamic_cast<TH1*>(titDir->Get(hname+"_S" ));
      histoBackground_ = dynamic_cast<TH1*>(titDir->Get(hname+"_B"));

      if (histoSignal_==0 || histoBackground_==0) {
	if (htype == MVAType)
	  cout << ":\t mva distribution not available (this is normal for Cut classifier)" << endl;
	else if(htype == ProbaType)
	  cout << ":\t probability distribution not available" << endl;
	else if(htype == RarityType)
	  cout << ":\t rarity distribution not available" << endl;
	else if(htype == CompareType)
	  cout << ":\t overtraining check not available" << endl;
	else cout << endl;
	continue;
      }

      if((methodTitle).Contains("Likelihood"))
         methodTitle = Form("Likelhood");
      else if ((methodTitle).Contains("LD"))
         methodTitle = Form("Linear Discriminant");
      else if ((methodTitle).Contains("BDT") && !(methodTitle).Contains("BDTG") && !(methodTitle).Contains("BDTF"))
         methodTitle = Form("Boosted Decision Tree");
      else if ((methodTitle).Contains("MLP"))
         methodTitle = Form("Multi-Layer Perceptron (MLP)");
      else if ((methodTitle).Contains("PDEFoam")) 
         methodTitle = Form("PDEFoam");
      else if ((methodTitle).Contains("SVN")) 
         methodTitle = Form("Supported Vector Machine (SVM)");
      else if ((methodTitle).Contains("Fisher")) 
         methodTitle = Form("Fisher Discriminant");
        
      histoSignal_->SetTitle(   Form("Response for classifier: %s", methodTitle.Data()) );
      if(htype == ProbaType)
	histoSignal_->SetTitle( Form("Probability for classifier: %s", methodTitle.Data()) );
      else if (htype == RarityType)
        histoSignal_->SetTitle( Form("Rarity for classifier: %s", methodTitle.Data()) );
      else if (htype == CompareType)
	histoSignal_->SetTitle( Form("Overtraining check for classifier: %s", methodTitle.Data()) );                                                                        
      

      TString cCanvasTitle = ((htype == MVAType)     ? Form("Response %s",methodTitle.Data()) :
			      (htype == ProbaType)   ? Form("Probability %s",methodTitle.Data()) :
			      (htype == CompareType) ? Form("Comparison %s",methodTitle.Data()) :
			      Form("Rarity %s",methodTitle.Data()));

      cMVAs_ = new TCanvas( Form("cMVAs_%d",(*this).mvas_index_), cCanvasTitle,180,100,550,550);
      cMVAs_->cd();
      cMVAs_->SetTicks();
      cMVAs_->SetFillColor(0);
      cMVAs_->SetBorderMode(0);
      cMVAs_->SetBorderSize(2);
  
      cMVAs_->SetTickx(1);
      cMVAs_->SetTicky(1);
      cMVAs_->SetRightMargin(0.05);
      cMVAs_->SetBottomMargin(0.12);
      cMVAs_->SetFrameBorderMode(0);

      // set the histogram style                                                                                                                                            
      histoSignal_->SetFillColor(4);
      histoSignal_->SetLineColor(4);
      histoSignal_->SetLineWidth(2);
      histoSignal_->SetFillStyle(1001);

      if(htype == CompareType) histoSignal_->SetFillStyle(3001);
         
      histoBackground_->SetFillColor(2);
      histoBackground_->SetLineColor(2);
      histoBackground_->SetLineWidth(2);
      histoBackground_->SetFillStyle(3005);
    
      // normalise both signal and background                                                                                                                               
      (*this).NormalizeHists((TH1F*)histoSignal_,(TH1F*)histoBackground_);

      // frame limits                                                                                                                
      Float_t nrms = 10;
      cout << "--- Mean and RMS (S): " << histoSignal_->GetMean() << ", " << histoSignal_->GetRMS() << endl;
      cout << "--- Mean and RMS (B): " << histoBackground_->GetMean() << ", " << histoBackground_->GetRMS() << endl;

      Float_t xmin = TMath::Max(TMath::Min(histoSignal_->GetMean()-nrms*histoSignal_->GetRMS(),histoBackground_->GetMean()-nrms*histoBackground_->GetRMS()),histoSignal_->GetXaxis()->GetXmin());
      Float_t xmax = TMath::Min(TMath::Max(histoSignal_->GetMean()+nrms*histoSignal_->GetRMS(),histoBackground_->GetMean()+nrms*histoBackground_->GetRMS()),histoSignal_->GetXaxis()->GetXmax());
      Float_t ymin = 0;
      Float_t maxMult = (htype == CompareType) ? 1.3 : 1.2;
      Float_t ymax = TMath::Max(histoSignal_->GetMaximum(),histoBackground_->GetMaximum())*maxMult;

      // build a frame                                                                                                                                                        
      Int_t   nb = 500;
      TString hFrameName(TString("frame") + methodTitle);
      TObject *object = gROOT->FindObject(hFrameName);
      if(object) delete object;

      TH2F* frame = new TH2F( hFrameName, histoSignal_->GetTitle(),nb, xmin, xmax, nb, ymin, ymax );
      frame->GetXaxis()->SetTitle( methodTitle + ((htype == MVAType || htype == CompareType) ? " response" : "") );

      if      (htype == ProbaType  ) frame->GetXaxis()->SetTitle( "Signal probability" );
      else if (htype == RarityType ) frame->GetXaxis()->SetTitle( "Signal rarity" );

      frame->GetYaxis()->SetTitle("(1/N) dN/dx");

      frame->SetLineWidth(2);
      frame->SetMarkerStyle(21);
      frame->SetMarkerSize(0.3);
      frame->GetXaxis()->SetNdivisions(405);
      frame->GetYaxis()->SetNdivisions(405);
      frame->GetXaxis()->SetLabelOffset(0.012);
      frame->GetXaxis()->SetLabelSize(0.03);
      frame->GetXaxis()->SetTitleSize(0.035);
      frame->GetXaxis()->SetTitleOffset(1.05);
      frame->GetYaxis()->SetLabelOffset(0.012);
      frame->GetYaxis()->SetLabelSize(0.03);
      frame->GetYaxis()->SetTitleSize(0.035);
      frame->GetYaxis()->SetTitleOffset(1.10);
      frame->Draw("");
   
      // Draw legend                                                                                                                                                           
      TLegend *legend= new TLegend( cMVAs_->GetLeftMargin()+0.03, 1-cMVAs_->GetTopMargin()-0.15,
				    cMVAs_->GetLeftMargin()+(htype == CompareType ? 0.40 : 0.3)+0.15, 1-cMVAs_->GetTopMargin()-0.03);
      legend->SetFillStyle(0);
      legend->SetFillColor(0);
      legend->SetTextSize (0.042);
      if(htype == CompareType) legend->SetTextSize (0.032); 
      legend->SetBorderSize(0);
      legend->SetTextFont(42);
      legend->SetLineColor(1);
      legend->SetLineStyle(1);
      legend->SetLineWidth(2);

      legend->AddEntry(histoSignal_,TString("Signal")     + ((htype == CompareType) ? " (testing)" : ""), "F");
      legend->AddEntry(histoBackground_,TString("Background") + ((htype == CompareType) ? " (testing)" : ""), "F");

      legend->SetMargin( (htype == CompareType ? 0.2 : 0.3) );
      legend->Draw("same");

      histoSignal_->Draw("samehist");
      histoBackground_->Draw("samehist");

      TLegend *legend2= new TLegend( 1-cMVAs_->GetRightMargin()-0.4, 1-cMVAs_->GetTopMargin()-0.15,
	        		     1-cMVAs_->GetRightMargin()-0.02,1-cMVAs_->GetTopMargin()-0.03);

      if (htype == CompareType) {
	// if overtraining check, load additional histograms                                                                                                                
	TH1* sigOv = 0;
	TH1* bgdOv = 0;
	TString ovname = hname += "_Train";
	sigOv = dynamic_cast<TH1*>(titDir->Get( ovname + "_S" ));
	bgdOv = dynamic_cast<TH1*>(titDir->Get( ovname + "_B" ));

	// normalise both signal and background                                                                                                              
	if (sigOv == 0 || bgdOv == 0){
	  cout << "+++ overtraining check histograms do not exist" << endl; 
	  return;
	}
	(*this).NormalizeHists((TH1F*)sigOv,(TH1F*)bgdOv );

	Int_t col = histoSignal_->GetLineColor();
	sigOv->SetMarkerColor(col);
	sigOv->SetMarkerSize(0.7);
	sigOv->SetMarkerStyle(20);
	sigOv->SetLineWidth(1);
	sigOv->SetLineColor(col);
	sigOv->Draw("e1same");

	col = histoBackground_->GetLineColor();
	bgdOv->SetMarkerColor(col);
	bgdOv->SetMarkerSize(0.7);
	bgdOv->SetMarkerStyle(20);
	bgdOv->SetLineWidth(1);
	bgdOv->SetLineColor(col);
	bgdOv->Draw("e1same");

	ymax = TMath::Max( ymax, float(TMath::Max(sigOv->GetMaximum(),bgdOv->GetMaximum())*maxMult));
	frame->GetYaxis()->SetLimits(0,ymax);

        legend2->SetFillStyle(0);
        legend2->SetFillColor(0);
        legend2->SetTextSize (0.03);
        legend2->SetBorderSize(0);
        legend2->SetTextFont(42);
        legend2->SetLineColor(1);
        legend2->SetLineStyle(1);
        legend2->SetLineWidth(2);
 	legend2->AddEntry(sigOv,"Signal (training)","P");
 	legend2->AddEntry(bgdOv,"Background (training)","P");
 	legend2->SetMargin( 0.1 );
 	legend2->Draw("same");

	// perform K-S test                                                                                                                                                     
	cout << "--- Perform Kolmogorov-Smirnov tests" << endl;
	Float_t kolS = histoSignal_->KolmogorovTest(sigOv);
	Float_t kolB = histoBackground_->KolmogorovTest(bgdOv);
	cout << "--- Goodness of signal (background) consistency: " << kolS << " (" << kolB << ")" << endl;

	TString probatext = Form("Kolmogorov-Smirnov test:");
        TString probatext2 = Form("signal (background) probability = %5.3g (%5.3g)", kolS, kolB );
	TLatex* tt  = new TLatex(0.25,0.72,probatext);
	TLatex* tt2 = new TLatex(0.25,0.69,probatext2);
	tt->SetNDC(); 
        tt->SetTextSize(0.025); 
        tt->AppendPad();
	tt2->SetNDC(); 
        tt2->SetTextSize(0.025); 
        tt2->AppendPad();

	// chi2 test                                                                                                                                            
	cout << "--- Perform Chi2 tests" << endl;
	Float_t chi2S = histoSignal_->Chi2Test(sigOv,"WW CHI2/NDF" );
	Float_t chi2B = histoBackground_->Chi2Test(bgdOv,"WW CHI2/NDF" );
	cout << "--- Goodness of signal (background) consistency: " << chi2S << " (" << chi2B << ")" << endl;

	TString probatext3 = Form("#Chi^{2} test:");
        TString probatext4 = Form("signal (background) #Chi^{2}/ndf = %5.3g (%5.3g)", chi2S, chi2B );
	TLatex* tt3  = new TLatex(0.25,0.65,probatext3);
	TLatex* tt4  = new TLatex(0.25,0.62,probatext4);
	tt3->SetNDC(); 
        tt3->SetTextSize(0.025); 
        tt3->AppendPad();
	tt4->SetNDC(); 
        tt4->SetTextSize(0.025); 
        tt4->AppendPad();

      }

      // redraw axes                                                                                                                                                        
      frame->Draw("sameaxis");

      // text for overflows                                                                                                                                                
      Int_t    nbin = histoSignal_->GetNbinsX();
      Float_t dxu  = histoSignal_->GetBinWidth(0);
      Float_t dxo  = histoSignal_->GetBinWidth(nbin+1);
      TString uoflow = Form( "U/O-flow (S,B): (%.1f, %.1f)%% / (%.1f, %.1f)%%",
			     histoSignal_->GetBinContent(0)*dxu*100, histoBackground_->GetBinContent(0)*dxu*100,
			     histoSignal_->GetBinContent(nbin+1)*dxo*100, histoBackground_->GetBinContent(nbin+1)*dxo*100 );

      TText* t = new TText( 0.975, 0.115, uoflow );
      t->SetNDC();
      t->SetTextSize( 0.030 );
      t->SetTextAngle( 90 );
      t->AppendPad();

      for_each(canvasLabel_.begin(),canvasLabel_.end(), default_delete<TLatex>());
      canvasLabel_.clear() ;

      canvasLabel_.push_back(new TLatex(0.95,0.92," 14 TeV"));
      canvasLabel_.back()->SetNDC();
      canvasLabel_.back()->SetTextAlign(31);
      canvasLabel_.back()->SetTextFont(42);
      canvasLabel_.back()->SetTextSize(0.04);
      canvasLabel_.back()->SetLineWidth(2);
      canvasLabel_.back()->Draw();
      canvasLabel_.push_back(new TLatex(0.13,0.92,"Delphes"));
      canvasLabel_.back()->SetNDC();
      canvasLabel_.back()->SetTextFont(61);
      canvasLabel_.back()->SetTextSize(0.04);
      canvasLabel_.back()->SetLineWidth(2);
      canvasLabel_.back()->Draw();
      canvasLabel_.push_back(new TLatex(0.2824,0.92,"Simulation Preliminary"));
      canvasLabel_.back()->SetNDC();
      canvasLabel_.back()->SetTextFont(52);
      canvasLabel_.back()->SetTextSize(0.0304);
      canvasLabel_.back()->SetLineWidth(2);
      canvasLabel_.back()->Draw();
      cMVAs_->Update();

      methodTitle.ReplaceAll(" ","_");
      methodTitle.ReplaceAll("(","_");
      methodTitle.ReplaceAll(")","_");
      methodTitle.ReplaceAll("[","_");
      methodTitle.ReplaceAll("]","_");
      methodTitle.ReplaceAll("{","_");
      methodTitle.ReplaceAll("}","_");

      if      (htype == MVAType)     (*this).PrintImage(cMVAs_, string(Form("%s/mva_output_%s",outputPlotDirectory.c_str(),methodTitle.Data())));
      else if (htype == ProbaType)   (*this).PrintImage(cMVAs_, string(Form("%s/probability_%s",outputPlotDirectory.c_str(),methodTitle.Data())));
      else if (htype == CompareType) (*this).PrintImage(cMVAs_, string(Form("%s/overtraining_%s",outputPlotDirectory.c_str(),methodTitle.Data())));
      else                           (*this).PrintImage(cMVAs_, string(Form("%s/rarity_%s",outputPlotDirectory.c_str(),methodTitle.Data())));
    
      mvas_index_ = mvas_index_  +1 ;

      if(frame!=0)   delete frame;
      if(legend!=0)  delete legend;
      if(t!=0)       delete t;
      if(legend2!=0) delete legend2 ;
	       
    }

  }	       

  return ;

}



// final method to do significance plot
void TMVAPlotClass::makeSignificancePlot (TFile* inputFile, 
					  const string & inputName, 
					  SignificanceType stype, 
					  const float & numberSignalEvents, 
					  const float & numberBackgroundEvents,
					  const bool & UseSignalEfficiency, 
					  const bool & UseBackgroundEfficiency, 
					  const string & outputPlotDirectory){


  if(inputFile == 0) {cerr<<" empty file --> exit "<<endl; return ; }

  // read the histo storing the signal and background effciencies and distribution
  (*this).ReadHistograms(inputFile); 
  
  // set the formula for significance inside the available choices
  if     (stype == 0)  (*this).SetFormula("S/B"); 
  else if(stype == 1)  (*this).SetFormula("S/sqrt(B)");  
  else if(stype == 2)  (*this).SetFormula("S/sqrt(S+B)");  
  else if(stype == 3)  (*this).SetFormula("2*(sqrt(S+B)-sqrt(B))");  
  else { cerr<<" Not known formula --> exit "<<endl; return; }

  // set if efficiency have to be used alone in the significance or multiply them for the yields
  (*this).SetSignalType(UseSignalEfficiency);
  (*this).SetBackgroundType(UseBackgroundEfficiency);

  // TFormula for the significance
  TFormula significanceFormula("significanceFormula",(*this).GetFormula());
  
  vector<significanceBox*>::iterator itList =  (*this).fSignificanceBox_.begin();
  TString cname = "Classifier";
  int maxLenTitle = 0 ;

  if (cname.Length() >  maxLenTitle)  maxLenTitle = cname.Length();

  TString str = Form( "%*s   (  signal, backgr.)  Optimal-cut  %s      NSig      NBkg   EffSig   EffBkg", maxLenTitle, cname.Data(), GetFormulaString().Data() );
  cout << "--- " << setfill('=') << setw(str.Length()) << "" << setfill(' ') << endl;
  cout << "--- " << str << endl;
  cout << "--- " << setfill('-') << setw(str.Length()) << "" << setfill(' ') << endl;
  
  // loop on the histo list   
  for ( ;itList!=(*this).fSignificanceBox_.end(); ++itList) {
    if(signalType_)
      (*itList)->significance_ = new TH1F(Form("significance_%s_%s_stype%d",inputName.c_str(),(*itList)->methodTitle_.Data(),stype),"",(*itList)->efficiencySignal_->GetNbinsX(),(*itList)->efficiencySignal_->GetBinLowEdge(1),(*itList)->efficiencySignal_->GetBinLowEdge((*itList)->efficiencySignal_->GetNbinsX()+1));
    else
      (*itList)->significance_ = new TH1F(Form("significance_eff_%s_%s_stype%d",inputName.c_str(),(*itList)->methodTitle_.Data(),stype),"",(*itList)->efficiencySignal_->GetNbinsX(),(*itList)->efficiencySignal_->GetBinLowEdge(1),(*itList)->efficiencySignal_->GetBinLowEdge((*itList)->efficiencySignal_->GetNbinsX()+1));

    for (Int_t iBin = 1; iBin<=(*itList)->efficiencySignal_->GetNbinsX(); iBin++) {
      Float_t S = 0;
      if(!signalType_) 
	S = (*itList)->efficiencySignal_->GetBinContent(iBin)*numberSignalEvents;
      else 
	S = (*itList)->efficiencySignal_->GetBinContent(iBin) ;      

      Float_t B = 0;
      if(!backgroundType_) 
	B = (*itList)->efficiencyBackground_->GetBinContent(iBin)*numberBackgroundEvents;
      else 
	B = (*itList)->efficiencyBackground_->GetBinContent(iBin) ;

      Float_t significance = 0. ;
      Float_t sigErr = 0.;

      // evaluate the significance
      if     (stype == 0 && B != 0 )   
	significance = significanceFormula.Eval(S,B);
      else if(stype == 1 && B != 0 )   
	significance = significanceFormula.Eval(S,B);
      else if(stype == 2 && S+B != 0 ) 
	significance = significanceFormula.Eval(S,B);
      else 
	significance = significanceFormula.Eval(S,B);

      if ((*this).GetFormulaString() == "S/B")              
	sigErr = sqrt(S/(B*B)+S*S/(B*B*B));	
      else if ((*this).GetFormulaString() == "S/sqrt(B)")   
	sigErr = significance * sqrt( 1./S + 1./(2.*B));	
      else if ((*this).GetFormulaString() == "S/sqrt(S+B)") 
	sigErr = sqrt(S*(TMath::Power(1-0.5/sqrt(S+B),2))*1/(S+B)+B*S*S/(4*(S+B)));	
      else if ((*this).GetFormulaString() == "2*(sqrt(S+B)-sqrt(B))") 
	sigErr = sqrt(S*TMath::Power(1/sqrt(S+B),2)+B*TMath::Power(1/sqrt(S+B)-1/sqrt(B),2));	

      // set value and error                  
      (*itList)->significance_->SetBinContent(iBin,significance);
      (*itList)->significance_->SetBinError(iBin,sigErr);
    }
    
  }

  /// Plot the results  
  vector<significanceBox*>::iterator itSignificanceBox = fSignificanceBox_.begin();
  for ( ; itSignificanceBox!=(*this).fSignificanceBox_.end(); ++itSignificanceBox ){
  // create new canvas                                                                                                                                                        
   if(signalType_)
     cSignificance_ = new TCanvas( Form("cSignificance_%s_%s_type_%d",inputName.c_str(),(*itSignificanceBox)->methodTitle_.Data(),stype),Form("Efficiencies Classifier : %s",(*itSignificanceBox)->methodTitle_.Data()),180,52,550,550);
   else
     cSignificance_ = new TCanvas( Form("cSignificance_eff_%s_%s_type_%d",inputName.c_str(),(*itSignificanceBox)->methodTitle_.Data(),stype),Form("Efficiencies Classifier : %s",(*itSignificanceBox)->methodTitle_.Data()),180,52,550,550);

   cSignificance_->cd();
   cSignificance_->SetTickx(1);
   cSignificance_->SetTicky(0);
   cSignificance_->SetFillColor(0);
   cSignificance_->SetBorderMode(0);
   cSignificance_->SetBorderSize(2);
  
   cSignificance_->SetLeftMargin(0.1);
   cSignificance_->SetRightMargin(0.12);
   cSignificance_->SetBottomMargin(0.12);
   cSignificance_->SetFrameBorderMode(0);

   int bin  = 0 ;   
   for(int iBin = 0; iBin < (*itSignificanceBox)->efficiencyBackground_->GetNbinsX(); iBin++){
     if((*itSignificanceBox)->efficiencyBackground_->GetBinContent(iBin+1)>0.005) continue; // break when the efficiency on the bkg is less than 0.5% and save the bin
     else{ bin = iBin ; break; }
   }

   // Set X axis limit for significance plots
   (*itSignificanceBox)->efficiencySignal_->SetTitle("Efficiencies and Optimal Cut");
   if ((*itSignificanceBox)->methodTitle_.Contains("Cuts")) {
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "Signal Efficiency (#epsilon_{sig})" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser(0.1,1.); // no less than 10% of signal efficiency for rectangular cut plots 
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser(0.1,1.);
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("Likelihood")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "Likelihood Output" );
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("LD")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "Linear Discriminant Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("BDT") && !(*itSignificanceBox)->methodTitle_.Contains("BDTG")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "BDT Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("BDTG")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "Gradient BDT Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("MLP")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "MLP Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("PDEFoam")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "PDEFoam Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("SVM")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "SVM Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else if ((*itSignificanceBox)->methodTitle_.Contains("Fisher")){
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( "Fisher Output" );    
      (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
      (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }
   else{
       (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitle( (*itSignificanceBox)->methodTitle_ + " output" );
       (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
       (*itSignificanceBox)->significance_->GetXaxis()->SetRangeUser((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin));
   }

   // some plots style
   (*itSignificanceBox)->efficiencyBackground_->GetYaxis()->SetTitle("Efficiency");
   (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitleSize(0.035);
   (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetLabelSize(0.035);
   (*itSignificanceBox)->efficiencyBackground_->GetXaxis()->SetTitleOffset(1.05);
   (*itSignificanceBox)->efficiencyBackground_->GetYaxis()->SetTitleSize(0.035);
   (*itSignificanceBox)->efficiencyBackground_->GetYaxis()->SetLabelSize(0.035);
   (*itSignificanceBox)->efficiencyBackground_->GetYaxis()->SetTitleOffset(1.20);
   (*itSignificanceBox)->efficiencyBackground_->GetYaxis()->SetRangeUser(0.,(*itSignificanceBox)->efficiencyBackground_->GetMaximum()*1.3);
   (*itSignificanceBox)->efficiencyBackground_->SetLineColor(kRed);
   (*itSignificanceBox)->efficiencyBackground_->SetLineWidth(2);

   (*itSignificanceBox)->efficiencySignal_->SetLineColor(kBlue);
   (*itSignificanceBox)->efficiencySignal_->SetLineWidth(2);


   (*itSignificanceBox)->significance_->SetLineColor(kBlack);
   (*itSignificanceBox)->significance_->SetLineWidth(2);

   (*itSignificanceBox)->efficiencyBackground_->Draw("histl");
   (*itSignificanceBox)->efficiencySignal_->Draw("samehistl");   

    // fill the info for the maximum significance and error and location
    Int_t maxbin = (*itSignificanceBox)->significance_->GetMaximumBin();    
    (*itSignificanceBox)->maxSig_    = (*itSignificanceBox)->significance_->GetBinContent(maxbin) ;
    (*itSignificanceBox)->maxSigErr_ = (*itSignificanceBox)->significance_->GetBinError(maxbin) ;      
    (*itSignificanceBox)->maxBin_    = maxbin ;
    
    TString opt = Form( "%%%is:  (%%8.4g,%%8.4g)    %%9.4g   %%10.6g  %%8.7g  %%8.7g %%8.4g %%8.4g",maxLenTitle);     
    cout << "--- "<< Form( opt.Data(), (*itSignificanceBox)->methodTitle_.Data(), numberSignalEvents, numberBackgroundEvents, (*itSignificanceBox)->significance_->GetXaxis()->GetBinCenter(maxbin),(*itSignificanceBox)->maxSig_,(*itSignificanceBox)->efficiencySignal_->GetBinContent(maxbin)*numberSignalEvents, (*itSignificanceBox)->efficiencyBackground_->GetBinContent( maxbin )*numberBackgroundEvents,(*itSignificanceBox)->efficiencySignal_->GetBinContent(maxbin), (*itSignificanceBox)->efficiencyBackground_->GetBinContent(maxbin) ) <<endl;

    // scale the significance plot to one in order to be put in the same canvas with efficiencies
    (*itSignificanceBox)->significance_->Scale(1/(*itSignificanceBox)->significance_->GetMaximum());
    (*itSignificanceBox)->significance_->Draw("samehistl");

    
    // Draw legend                                                                                                                                                            
    TLegend *legend1= new TLegend( cSignificance_->GetLeftMargin()+0.05,1-cSignificance_->GetTopMargin()-0.17,cSignificance_->GetLeftMargin()+0.25,1-cSignificance_->GetTopMargin()-0.02);
    legend1->SetFillStyle(0);
    legend1->SetFillColor(0);
    legend1->SetTextFont(42);
    legend1->SetTextSize(0.032);
    legend1->SetBorderSize(0.);
    legend1->SetMargin(0.3);

    legend1->AddEntry((*itSignificanceBox)->efficiencySignal_,"Signal Efficiency","L");
    legend1->AddEntry((*itSignificanceBox)->efficiencyBackground_,"Background Efficiency","L");
    legend1->Draw("same");
   
    TLegend *legend2= new TLegend(cSignificance_->GetLeftMargin()+0.4,1-cSignificance_->GetTopMargin()-0.09,1-cSignificance_->GetRightMargin()-0.2,1-cSignificance_->GetTopMargin()-0.02);
    legend2->SetFillStyle(0);
    legend2->SetFillColor(0);
    legend2->SetBorderSize(0.);
    legend2->SetTextFont(42);
    legend2->SetTextSize(0.032);
    legend2->SetMargin(0.3);

    legend2->AddEntry((*itSignificanceBox)->significance_,Form("Significance %s",(*this).GetLatexFormula().Data()),"L");
    legend2->Draw("same");
    
    // line to indicate maximum efficiency                                                                                                                                 
    TLine* effline = 0 ;
    if ((*itSignificanceBox)->methodTitle_.Contains("Cuts")) effline = new TLine(0.1,1,1,1);
    else effline = new TLine((*itSignificanceBox)->efficiencyBackground_->GetBinLowEdge(1),1,(*itSignificanceBox)->efficiencyBackground_->GetBinCenter(bin)-(*itSignificanceBox)->efficiencyBackground_->GetBinWidth(bin),1);

    effline->SetLineWidth(3);
    effline->SetLineColor(6);
    effline->Draw("same");

    cSignificance_->Update();
    
    // print comments                                                                                                                                                         
    for_each(canvasLabel_.begin(),canvasLabel_.end(), default_delete<TLatex>());
    
    canvasLabel_.clear() ;
    
    canvasLabel_.push_back(new TLatex(0.95,0.92," 14 TeV"));
    
    canvasLabel_.back()->SetNDC();
    canvasLabel_.back()->SetTextAlign(31);
    canvasLabel_.back()->SetTextFont(42);
    canvasLabel_.back()->SetTextSize(0.04);
    canvasLabel_.back()->SetLineWidth(2);
    canvasLabel_.back()->Draw();
    canvasLabel_.push_back(new TLatex(0.13,0.92,"Delphes"));
    canvasLabel_.back()->SetNDC();
    canvasLabel_.back()->SetTextFont(61);
    canvasLabel_.back()->SetTextSize(0.04);
    canvasLabel_.back()->SetLineWidth(2);
    canvasLabel_.back()->Draw();
    canvasLabel_.push_back(new TLatex(0.2824,0.92,"Simulation Preliminary"));
    canvasLabel_.back()->SetNDC();
    canvasLabel_.back()->SetTextFont(52);
    canvasLabel_.back()->SetTextSize(0.0304);
    canvasLabel_.back()->SetLineWidth(2);
    canvasLabel_.back()->Draw();

    
    TString name = Form("For %.4g signal and %.4g background",numberSignalEvents,numberBackgroundEvents);
    TPaveText* line1 = new TPaveText(0.22,0.21,0.5,0.26,"NDC");
    line1->SetBorderSize(0);
    line1->SetFillColor(0);
    line1->SetFillStyle(0);
    line1->AddText(name.Data());
    line1->SetTextSize(0.027);
    line1->SetTextFont(42);
    //    line1->Draw("same");
  
    TPaveText* line2 = new TPaveText(0.22,0.15,0.5,0.2,"NDC");
    name = Form("max significance at %0.4g, cut at %0.4g",(*itSignificanceBox)->maxSig_,(*itSignificanceBox)->significance_->GetXaxis()->GetBinCenter((*itSignificanceBox)->maxBin_));
    line2->AddText(name);
    line2->SetBorderSize(0);
    line2->SetFillColor(0);
    line2->SetFillStyle(0);
    line2->SetTextSize(0.027);
    line2->SetTextFont(42);
    //    line2->Draw("same");
    
    TGaxis* rightAxis = new TGaxis(cSignificance_->GetUxmax(),cSignificance_->GetUymin(),cSignificance_->GetUxmax(),cSignificance_->GetUymax(),0,(*itSignificanceBox)->maxSig_*1.3,510,"+L");
    rightAxis->SetLineColor (kBlack);
    rightAxis->SetLabelColor(kBlack);
    rightAxis->SetTitleColor(kBlack);

    rightAxis->SetLabelSize(0.035);
    rightAxis->SetTitleSize(0.035);
    rightAxis->SetTitleOffset(1.25);
    rightAxis->SetTextFont(42);
    rightAxis->SetLabelFont(42);
    rightAxis->SetTitle("Significance");
    rightAxis->Draw();

    cSignificance_->Update();

    TString baseName ;
    if(UseSignalEfficiency && UseBackgroundEfficiency){
     if(thisMethod_ >= method_index_) continue ;
       baseName  = Form("mva_significance_eff_%s_%s",(*itSignificanceBox)->methodTitle_.Data(),inputName.c_str());

     if(stype == 0)      (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_B_file",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 1) (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_sqrtB",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 2) (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_sqrtSB",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 3) (*this).PrintImage(cSignificance_, string(Form("%s/%s_pval",outputPlotDirectory.c_str(),baseName.Data())));

     if ((*itSignificanceBox)->methodTitle_.Contains("Cuts")) thisMethod_++;

    }
    else if( !UseSignalEfficiency && !UseBackgroundEfficiency){
     if(thisMethod_ > method_index_) continue ;
       baseName  = Form("mva_significance_%s_%s",(*itSignificanceBox)->methodTitle_.Data(),inputName.c_str());

     if(stype == 0) (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_B",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 1) (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_sqrtB",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 2) (*this).PrintImage(cSignificance_, string(Form("%s/%s_S_over_sqrtSB",outputPlotDirectory.c_str(),baseName.Data())));
     else if(stype == 3) (*this).PrintImage(cSignificance_, string(Form("%s/%s_pval",outputPlotDirectory.c_str(),baseName.Data())));

     if ((*itSignificanceBox)->methodTitle_.Contains("Cuts")) thisMethod_++;

    }

    if(thisMethod_ == method_index_) thisMethod_ = 0;
   
    //   delete rightAxis ;
    delete cSignificance_;
    legend1->Delete() ;   
    legend2->Delete() ;
    line1->Delete() ;
    line2->Delete() ;
    effline->Delete() ;
  }
  return ;
  
}
