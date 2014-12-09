//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Nov 22 16:06:26 2014 by ROOT version 5.34/21
// from TTree easyDelphes/easyDelphes
// found on file: /Users/govoni/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root
//////////////////////////////////////////////////////////

#ifndef readTree_h
#define readTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class readTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         leptonLHEpt1;
   Float_t         leptonLHEeta1;
   Float_t         leptonLHEphi1;
   Float_t         leptonLHEpid1;
   Float_t         leptonLHEpt2;
   Float_t         leptonLHEeta2;
   Float_t         leptonLHEphi2;
   Float_t         leptonLHEpid2;
   Float_t         leptonLHEpt3;
   Float_t         leptonLHEeta3;
   Float_t         leptonLHEphi3;
   Float_t         leptonLHEpid3;
   Float_t         neutrinoLHEpt1;
   Float_t         neutrinoLHEeta1;
   Float_t         neutrinoLHEphi1;
   Float_t         neutrinoLHEpid1;
   Float_t         neutrinoLHEpt2;
   Float_t         neutrinoLHEeta2;
   Float_t         neutrinoLHEphi2;
   Float_t         neutrinoLHEpid2;
   Float_t         neutrinoLHEpt3;
   Float_t         neutrinoLHEeta3;
   Float_t         neutrinoLHEphi3;
   Float_t         neutrinoLHEpid3;
   Float_t         jetLHEPartonpt1;
   Float_t         jetLHEPartoneta1;
   Float_t         jetLHEPartonphi1;
   Float_t         jetLHEPartonpid1;
   Float_t         jetLHEPartonpt2;
   Float_t         jetLHEPartoneta2;
   Float_t         jetLHEPartonphi2;
   Float_t         jetLHEPartonpid2;
   Float_t         jetLHEPartonpt3;
   Float_t         jetLHEPartoneta3;
   Float_t         jetLHEPartonphi3;
   Float_t         jetLHEPartonpid3;
   Float_t         jetGenPartonpt1;
   Float_t         jetGenPartonpt2;
   Float_t         jetGenPartonpt3;
   Float_t         jetGenPartonpt4;
   Float_t         jetGenPartonpid1;
   Float_t         jetGenPartonpid2;
   Float_t         jetGenPartonpid3;
   Float_t         jetGenPartonpid4;
   Float_t         jetGenPartonphi1;
   Float_t         jetGenPartonphi2;
   Float_t         jetGenPartonphi3;
   Float_t         jetGenPartonphi4;
   Float_t         jetGenPartoneta1;
   Float_t         jetGenPartoneta2;
   Float_t         jetGenPartoneta3;
   Float_t         jetGenPartoneta4;
   Float_t         leptonGenpt1;
   Float_t         leptonGenpt2;
   Float_t         leptonGenpt3;
   Float_t         leptonGenpt4;
   Float_t         leptonGenpid1;
   Float_t         leptonGenpid2;
   Float_t         leptonGenpid3;
   Float_t         leptonGenpid4;
   Float_t         leptonGenphi1;
   Float_t         leptonGenphi2;
   Float_t         leptonGenphi3;
   Float_t         leptonGenphi4;
   Float_t         leptonGeneta1;
   Float_t         leptonGeneta2;
   Float_t         leptonGeneta3;
   Float_t         leptonGeneta4;
   Float_t         neutrinoGenpt1;
   Float_t         neutrinoGenpt2;
   Float_t         neutrinoGenpt3;
   Float_t         neutrinoGenpt4;
   Float_t         neutrinoGenpid1;
   Float_t         neutrinoGenpid2;
   Float_t         neutrinoGenpid3;
   Float_t         neutrinoGenpid4;
   Float_t         neutrinoGenphi1;
   Float_t         neutrinoGenphi2;
   Float_t         neutrinoGenphi3;
   Float_t         neutrinoGenphi4;
   Float_t         neutrinoGeneta1;
   Float_t         neutrinoGeneta2;
   Float_t         neutrinoGeneta3;
   Float_t         neutrinoGeneta4;
   Float_t         pt1;
   Float_t         pt2;
   Float_t         eta1;
   Float_t         eta2;
   Float_t         phi1;
   Float_t         phi2;
   Float_t         ch1;
   Float_t         ch2;
   Float_t         iso1;
   Float_t         iso2;
   Int_t           pdgid1;
   Int_t           pdgid2;
   Float_t         mll;
   Float_t         PTll;
   Float_t         dPhill;
   Float_t         dRll;
   Float_t         dEtall;
   Float_t         etall;
   Float_t         yll;
   Int_t           nextra;
   Int_t           sameflav;
   Int_t           channel;
   Float_t         jetGenpt1;
   Float_t         jetGenpt2;
   Float_t         jetGenpt3;
   Float_t         jetGenpt4;
   Float_t         jetGenphi1;
   Float_t         jetGenphi2;
   Float_t         jetGenphi3;
   Float_t         jetGenphi4;
   Float_t         jetGeneta1;
   Float_t         jetGeneta2;
   Float_t         jetGeneta3;
   Float_t         jetGeneta4;
   Int_t           njet;
   Int_t           nbjet;
   Int_t           hardbjpb;
   Int_t           softbjpb;
   Int_t           njetid;
   Float_t         jeteta1;
   Float_t         jeteta2;
   Float_t         jeteta3;
   Float_t         jeteta4;
   Float_t         jeteta5;
   Float_t         jeteta6;
   Float_t         jeteta7;
   Float_t         jeteta8;
   Float_t         jetphi1;
   Float_t         jetphi2;
   Float_t         jetphi3;
   Float_t         jetphi4;
   Float_t         jetphi5;
   Float_t         jetphi6;
   Float_t         jetphi7;
   Float_t         jetphi8;
   Float_t         jetpt1;
   Float_t         jetpt2;
   Float_t         jetpt3;
   Float_t         jetpt4;
   Float_t         jetpt5;
   Float_t         jetpt6;
   Float_t         jetpt7;
   Float_t         jetpt8;
   Float_t         jetmass1;
   Float_t         jetmass2;
   Float_t         jetmass3;
   Float_t         jetmass4;
   Float_t         jetmass5;
   Float_t         jetmass6;
   Float_t         jetmass7;
   Float_t         jetmass8;
   Float_t         mjj;
   Float_t         detajj;
   Int_t           njet_puppi;
   Int_t           nbjet_puppi;
   Int_t           hardbjpb_puppi;
   Int_t           softbjpb_puppi;
   Int_t           njetid_puppi;
   Float_t         jeteta1_puppi;
   Float_t         jeteta2_puppi;
   Float_t         jeteta3_puppi;
   Float_t         jeteta4_puppi;
   Float_t         jeteta5_puppi;
   Float_t         jeteta6_puppi;
   Float_t         jeteta7_puppi;
   Float_t         jeteta8_puppi;
   Float_t         jetphi1_puppi;
   Float_t         jetphi2_puppi;
   Float_t         jetphi3_puppi;
   Float_t         jetphi4_puppi;
   Float_t         jetphi5_puppi;
   Float_t         jetphi6_puppi;
   Float_t         jetphi7_puppi;
   Float_t         jetphi8_puppi;
   Float_t         jetpt1_puppi;
   Float_t         jetpt2_puppi;
   Float_t         jetpt3_puppi;
   Float_t         jetpt4_puppi;
   Float_t         jetpt5_puppi;
   Float_t         jetpt6_puppi;
   Float_t         jetpt7_puppi;
   Float_t         jetpt8_puppi;
   Float_t         jetmass1_puppi;
   Float_t         jetmass2_puppi;
   Float_t         jetmass3_puppi;
   Float_t         jetmass4_puppi;
   Float_t         jetmass5_puppi;
   Float_t         jetmass6_puppi;
   Float_t         jetmass7_puppi;
   Float_t         jetmass8_puppi;
   Float_t         mjj_puppi;
   Float_t         detajj_puppi;
   Float_t         pfmet;
   Float_t         pfmetphi;
   Float_t         metGenpt;
   Float_t         metGenphi;
   Float_t         pfmet_puppi;
   Float_t         pfmetphi_puppi;

   // List of branches
   TBranch        *b_leptonLHEpt1;   //!
   TBranch        *b_leptonLHEeta1;   //!
   TBranch        *b_leptonLHEphi1;   //!
   TBranch        *b_leptonLHEpid1;   //!
   TBranch        *b_leptonLHEpt2;   //!
   TBranch        *b_leptonLHEeta2;   //!
   TBranch        *b_leptonLHEphi2;   //!
   TBranch        *b_leptonLHEpid2;   //!
   TBranch        *b_leptonLHEpt3;   //!
   TBranch        *b_leptonLHEeta3;   //!
   TBranch        *b_leptonLHEphi3;   //!
   TBranch        *b_leptonLHEpid3;   //!
   TBranch        *b_neutrinoLHEpt1;   //!
   TBranch        *b_neutrinoLHEeta1;   //!
   TBranch        *b_neutrinoLHEphi1;   //!
   TBranch        *b_neutrinoLHEpid1;   //!
   TBranch        *b_neutrinoLHEpt2;   //!
   TBranch        *b_neutrinoLHEeta2;   //!
   TBranch        *b_neutrinoLHEphi2;   //!
   TBranch        *b_neutrinoLHEpid2;   //!
   TBranch        *b_neutrinoLHEpt3;   //!
   TBranch        *b_neutrinoLHEeta3;   //!
   TBranch        *b_neutrinoLHEphi3;   //!
   TBranch        *b_neutrinoLHEpid3;   //!
   TBranch        *b_jetLHEPartonpt1;   //!
   TBranch        *b_jetLHEPartoneta1;   //!
   TBranch        *b_jetLHEPartonphi1;   //!
   TBranch        *b_jetLHEPartonpid1;   //!
   TBranch        *b_jetLHEPartonpt2;   //!
   TBranch        *b_jetLHEPartoneta2;   //!
   TBranch        *b_jetLHEPartonphi2;   //!
   TBranch        *b_jetLHEPartonpid2;   //!
   TBranch        *b_jetLHEPartonpt3;   //!
   TBranch        *b_jetLHEPartoneta3;   //!
   TBranch        *b_jetLHEPartonphi3;   //!
   TBranch        *b_jetLHEPartonpid3;   //!
   TBranch        *b_jetGenPartonpt1;   //!
   TBranch        *b_jetGenPartonpt3;   //!
   TBranch        *b_jetGenPartonpt4;   //!
   TBranch        *b_jetGenPartonpid1;   //!
   TBranch        *b_jetGenPartonpid2;   //!
   TBranch        *b_jetGenPartonpid3;   //!
   TBranch        *b_jetGenPartonpid4;   //!
   TBranch        *b_jetGenPartonphi1;   //!
   TBranch        *b_jetGenPartonphi2;   //!
   TBranch        *b_jetGenPartonphi3;   //!
   TBranch        *b_jetGenPartonphi4;   //!
   TBranch        *b_jetGenPartoneta1;   //!
   TBranch        *b_jetGenPartoneta2;   //!
   TBranch        *b_jetGenPartoneta3;   //!
   TBranch        *b_jetGenPartoneta4;   //!
   TBranch        *b_leptonGenpt1;   //!
   TBranch        *b_leptonGenpt2;   //!
   TBranch        *b_leptonGenpt3;   //!
   TBranch        *b_leptonGenpt4;   //!
   TBranch        *b_leptonGenpid1;   //!
   TBranch        *b_leptonGenpid2;   //!
   TBranch        *b_leptonGenpid3;   //!
   TBranch        *b_leptonGenpid4;   //!
   TBranch        *b_leptonGenphi1;   //!
   TBranch        *b_leptonGenphi2;   //!
   TBranch        *b_leptonGenphi3;   //!
   TBranch        *b_leptonGenphi4;   //!
   TBranch        *b_leptonGeneta1;   //!
   TBranch        *b_leptonGeneta2;   //!
   TBranch        *b_leptonGeneta3;   //!
   TBranch        *b_leptonGeneta4;   //!
   TBranch        *b_neutrinoGenpt1;   //!
   TBranch        *b_neutrinoGenpt2;   //!
   TBranch        *b_neutrinoGenpt3;   //!
   TBranch        *b_neutrinoGenpt4;   //!
   TBranch        *b_neutrinoGenpid1;   //!
   TBranch        *b_neutrinoGenpid2;   //!
   TBranch        *b_neutrinoGenpid3;   //!
   TBranch        *b_neutrinoGenpid4;   //!
   TBranch        *b_neutrinoGenphi1;   //!
   TBranch        *b_neutrinoGenphi2;   //!
   TBranch        *b_neutrinoGenphi3;   //!
   TBranch        *b_neutrinoGenphi4;   //!
   TBranch        *b_neutrinoGeneta1;   //!
   TBranch        *b_neutrinoGeneta2;   //!
   TBranch        *b_neutrinoGeneta3;   //!
   TBranch        *b_neutrinoGeneta4;   //!
   TBranch        *b_pt1;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta1;   //!
   TBranch        *b_eta2;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_ch1;   //!
   TBranch        *b_ch2;   //!
   TBranch        *b_iso1;   //!
   TBranch        *b_iso2;   //!
   TBranch        *b_pdgid1;   //!
   TBranch        *b_pdgid2;   //!
   TBranch        *b_mll;   //!
   TBranch        *b_PTll;   //!
   TBranch        *b_dPhill;   //!
   TBranch        *b_dRll;   //!
   TBranch        *b_dEtall;   //!
   TBranch        *b_etall;   //!
   TBranch        *b_yll;   //!
   TBranch        *b_nextra;   //!
   TBranch        *b_sameflav;   //!
   TBranch        *b_channel;   //!
   TBranch        *b_jetGenpt1;   //!
   TBranch        *b_jetGenpt3;   //!
   TBranch        *b_jetGenpt4;   //!
   TBranch        *b_jetGenphi1;   //!
   TBranch        *b_jetGenphi2;   //!
   TBranch        *b_jetGenphi3;   //!
   TBranch        *b_jetGenphi4;   //!
   TBranch        *b_jetGeneta1;   //!
   TBranch        *b_jetGeneta2;   //!
   TBranch        *b_jetGeneta3;   //!
   TBranch        *b_jetGeneta4;   //!
   TBranch        *b_nbjet;   //!
   TBranch        *b_hardbjpb;   //!
   TBranch        *b_softbjpb;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_njetid;   //!
   TBranch        *b_jeteta1;   //!
   TBranch        *b_jeteta2;   //!
   TBranch        *b_jeteta3;   //!
   TBranch        *b_jeteta4;   //!
   TBranch        *b_jeteta5;   //!
   TBranch        *b_jeteta6;   //!
   TBranch        *b_jeteta7;   //!
   TBranch        *b_jeteta8;   //!
   TBranch        *b_jetphi1;   //!
   TBranch        *b_jetphi2;   //!
   TBranch        *b_jetphi3;   //!
   TBranch        *b_jetphi4;   //!
   TBranch        *b_jetphi5;   //!
   TBranch        *b_jetphi6;   //!
   TBranch        *b_jetphi7;   //!
   TBranch        *b_jetphi8;   //!
   TBranch        *b_jetpt1;   //!
   TBranch        *b_jetpt2;   //!
   TBranch        *b_jetpt3;   //!
   TBranch        *b_jetpt4;   //!
   TBranch        *b_jetpt5;   //!
   TBranch        *b_jetpt6;   //!
   TBranch        *b_jetpt7;   //!
   TBranch        *b_jetpt8;   //!
   TBranch        *b_jetmass1;   //!
   TBranch        *b_jetmass2;   //!
   TBranch        *b_jetmass3;   //!
   TBranch        *b_jetmass4;   //!
   TBranch        *b_jetmass5;   //!
   TBranch        *b_jetmass6;   //!
   TBranch        *b_jetmass7;   //!
   TBranch        *b_jetmass8;   //!
   TBranch        *b_mjj;   //!
   TBranch        *b_detajj;   //!
   TBranch        *b_pfmet;   //!
   TBranch        *b_pfmetphi;   //!
   TBranch        *b_metGenpt;   //!
   TBranch        *b_metGenphi;   //!
   TBranch        *b_pfmet_puppi;   //!
   TBranch        *b_pfmetphi_puppi;   //!

   readTree(TTree *tree=0);
   virtual ~readTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef readTree_cxx
readTree::readTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/Users/govoni/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/Users/govoni/data/TP/ntuples/2014-09-22/TP_SS_EWK_DF_126.root");
      }
      f->GetObject("easyDelphes",tree);

   }
   Init(tree);
}

readTree::~readTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t readTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t readTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void readTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("leptonLHEpt1", &leptonLHEpt1, &b_leptonLHEpt1);
   fChain->SetBranchAddress("leptonLHEeta1", &leptonLHEeta1, &b_leptonLHEeta1);
   fChain->SetBranchAddress("leptonLHEphi1", &leptonLHEphi1, &b_leptonLHEphi1);
   fChain->SetBranchAddress("leptonLHEpid1", &leptonLHEpid1, &b_leptonLHEpid1);
   fChain->SetBranchAddress("leptonLHEpt2", &leptonLHEpt2, &b_leptonLHEpt2);
   fChain->SetBranchAddress("leptonLHEeta2", &leptonLHEeta2, &b_leptonLHEeta2);
   fChain->SetBranchAddress("leptonLHEphi2", &leptonLHEphi2, &b_leptonLHEphi2);
   fChain->SetBranchAddress("leptonLHEpid2", &leptonLHEpid2, &b_leptonLHEpid2);
   fChain->SetBranchAddress("leptonLHEpt3", &leptonLHEpt3, &b_leptonLHEpt3);
   fChain->SetBranchAddress("leptonLHEeta3", &leptonLHEeta3, &b_leptonLHEeta3);
   fChain->SetBranchAddress("leptonLHEphi3", &leptonLHEphi3, &b_leptonLHEphi3);
   fChain->SetBranchAddress("leptonLHEpid3", &leptonLHEpid3, &b_leptonLHEpid3);
   fChain->SetBranchAddress("neutrinoLHEpt1", &neutrinoLHEpt1, &b_neutrinoLHEpt1);
   fChain->SetBranchAddress("neutrinoLHEeta1", &neutrinoLHEeta1, &b_neutrinoLHEeta1);
   fChain->SetBranchAddress("neutrinoLHEphi1", &neutrinoLHEphi1, &b_neutrinoLHEphi1);
   fChain->SetBranchAddress("neutrinoLHEpid1", &neutrinoLHEpid1, &b_neutrinoLHEpid1);
   fChain->SetBranchAddress("neutrinoLHEpt2", &neutrinoLHEpt2, &b_neutrinoLHEpt2);
   fChain->SetBranchAddress("neutrinoLHEeta2", &neutrinoLHEeta2, &b_neutrinoLHEeta2);
   fChain->SetBranchAddress("neutrinoLHEphi2", &neutrinoLHEphi2, &b_neutrinoLHEphi2);
   fChain->SetBranchAddress("neutrinoLHEpid2", &neutrinoLHEpid2, &b_neutrinoLHEpid2);
   fChain->SetBranchAddress("neutrinoLHEpt3", &neutrinoLHEpt3, &b_neutrinoLHEpt3);
   fChain->SetBranchAddress("neutrinoLHEeta3", &neutrinoLHEeta3, &b_neutrinoLHEeta3);
   fChain->SetBranchAddress("neutrinoLHEphi3", &neutrinoLHEphi3, &b_neutrinoLHEphi3);
   fChain->SetBranchAddress("neutrinoLHEpid3", &neutrinoLHEpid3, &b_neutrinoLHEpid3);
   fChain->SetBranchAddress("jetLHEPartonpt1", &jetLHEPartonpt1, &b_jetLHEPartonpt1);
   fChain->SetBranchAddress("jetLHEPartoneta1", &jetLHEPartoneta1, &b_jetLHEPartoneta1);
   fChain->SetBranchAddress("jetLHEPartonphi1", &jetLHEPartonphi1, &b_jetLHEPartonphi1);
   fChain->SetBranchAddress("jetLHEPartonpid1", &jetLHEPartonpid1, &b_jetLHEPartonpid1);
   fChain->SetBranchAddress("jetLHEPartonpt2", &jetLHEPartonpt2, &b_jetLHEPartonpt2);
   fChain->SetBranchAddress("jetLHEPartoneta2", &jetLHEPartoneta2, &b_jetLHEPartoneta2);
   fChain->SetBranchAddress("jetLHEPartonphi2", &jetLHEPartonphi2, &b_jetLHEPartonphi2);
   fChain->SetBranchAddress("jetLHEPartonpid2", &jetLHEPartonpid2, &b_jetLHEPartonpid2);
   fChain->SetBranchAddress("jetLHEPartonpt3", &jetLHEPartonpt3, &b_jetLHEPartonpt3);
   fChain->SetBranchAddress("jetLHEPartoneta3", &jetLHEPartoneta3, &b_jetLHEPartoneta3);
   fChain->SetBranchAddress("jetLHEPartonphi3", &jetLHEPartonphi3, &b_jetLHEPartonphi3);
   fChain->SetBranchAddress("jetLHEPartonpid3", &jetLHEPartonpid3, &b_jetLHEPartonpid3);
   fChain->SetBranchAddress("jetGenPartonpt1", &jetGenPartonpt1, &b_jetGenPartonpt1);
   fChain->SetBranchAddress("jetGenPartonpt2", &jetGenPartonpt2, &b_jetGenPartonpt1);
   fChain->SetBranchAddress("jetGenPartonpt3", &jetGenPartonpt3, &b_jetGenPartonpt3);
   fChain->SetBranchAddress("jetGenPartonpt4", &jetGenPartonpt4, &b_jetGenPartonpt4);
   fChain->SetBranchAddress("jetGenPartonpid1", &jetGenPartonpid1, &b_jetGenPartonpid1);
   fChain->SetBranchAddress("jetGenPartonpid2", &jetGenPartonpid2, &b_jetGenPartonpid2);
   fChain->SetBranchAddress("jetGenPartonpid3", &jetGenPartonpid3, &b_jetGenPartonpid3);
   fChain->SetBranchAddress("jetGenPartonpid4", &jetGenPartonpid4, &b_jetGenPartonpid4);
   fChain->SetBranchAddress("jetGenPartonphi1", &jetGenPartonphi1, &b_jetGenPartonphi1);
   fChain->SetBranchAddress("jetGenPartonphi2", &jetGenPartonphi2, &b_jetGenPartonphi2);
   fChain->SetBranchAddress("jetGenPartonphi3", &jetGenPartonphi3, &b_jetGenPartonphi3);
   fChain->SetBranchAddress("jetGenPartonphi4", &jetGenPartonphi4, &b_jetGenPartonphi4);
   fChain->SetBranchAddress("jetGenPartoneta1", &jetGenPartoneta1, &b_jetGenPartoneta1);
   fChain->SetBranchAddress("jetGenPartoneta2", &jetGenPartoneta2, &b_jetGenPartoneta2);
   fChain->SetBranchAddress("jetGenPartoneta3", &jetGenPartoneta3, &b_jetGenPartoneta3);
   fChain->SetBranchAddress("jetGenPartoneta4", &jetGenPartoneta4, &b_jetGenPartoneta4);
   fChain->SetBranchAddress("leptonGenpt1", &leptonGenpt1, &b_leptonGenpt1);
   fChain->SetBranchAddress("leptonGenpt2", &leptonGenpt2, &b_leptonGenpt2);
   fChain->SetBranchAddress("leptonGenpt3", &leptonGenpt3, &b_leptonGenpt3);
   fChain->SetBranchAddress("leptonGenpt4", &leptonGenpt4, &b_leptonGenpt4);
   fChain->SetBranchAddress("leptonGenpid1", &leptonGenpid1, &b_leptonGenpid1);
   fChain->SetBranchAddress("leptonGenpid2", &leptonGenpid2, &b_leptonGenpid2);
   fChain->SetBranchAddress("leptonGenpid3", &leptonGenpid3, &b_leptonGenpid3);
   fChain->SetBranchAddress("leptonGenpid4", &leptonGenpid4, &b_leptonGenpid4);
   fChain->SetBranchAddress("leptonGenphi1", &leptonGenphi1, &b_leptonGenphi1);
   fChain->SetBranchAddress("leptonGenphi2", &leptonGenphi2, &b_leptonGenphi2);
   fChain->SetBranchAddress("leptonGenphi3", &leptonGenphi3, &b_leptonGenphi3);
   fChain->SetBranchAddress("leptonGenphi4", &leptonGenphi4, &b_leptonGenphi4);
   fChain->SetBranchAddress("leptonGeneta1", &leptonGeneta1, &b_leptonGeneta1);
   fChain->SetBranchAddress("leptonGeneta2", &leptonGeneta2, &b_leptonGeneta2);
   fChain->SetBranchAddress("leptonGeneta3", &leptonGeneta3, &b_leptonGeneta3);
   fChain->SetBranchAddress("leptonGeneta4", &leptonGeneta4, &b_leptonGeneta4);
   fChain->SetBranchAddress("neutrinoGenpt1", &neutrinoGenpt1, &b_neutrinoGenpt1);
   fChain->SetBranchAddress("neutrinoGenpt2", &neutrinoGenpt2, &b_neutrinoGenpt2);
   fChain->SetBranchAddress("neutrinoGenpt3", &neutrinoGenpt3, &b_neutrinoGenpt3);
   fChain->SetBranchAddress("neutrinoGenpt4", &neutrinoGenpt4, &b_neutrinoGenpt4);
   fChain->SetBranchAddress("neutrinoGenpid1", &neutrinoGenpid1, &b_neutrinoGenpid1);
   fChain->SetBranchAddress("neutrinoGenpid2", &neutrinoGenpid2, &b_neutrinoGenpid2);
   fChain->SetBranchAddress("neutrinoGenpid3", &neutrinoGenpid3, &b_neutrinoGenpid3);
   fChain->SetBranchAddress("neutrinoGenpid4", &neutrinoGenpid4, &b_neutrinoGenpid4);
   fChain->SetBranchAddress("neutrinoGenphi1", &neutrinoGenphi1, &b_neutrinoGenphi1);
   fChain->SetBranchAddress("neutrinoGenphi2", &neutrinoGenphi2, &b_neutrinoGenphi2);
   fChain->SetBranchAddress("neutrinoGenphi3", &neutrinoGenphi3, &b_neutrinoGenphi3);
   fChain->SetBranchAddress("neutrinoGenphi4", &neutrinoGenphi4, &b_neutrinoGenphi4);
   fChain->SetBranchAddress("neutrinoGeneta1", &neutrinoGeneta1, &b_neutrinoGeneta1);
   fChain->SetBranchAddress("neutrinoGeneta2", &neutrinoGeneta2, &b_neutrinoGeneta2);
   fChain->SetBranchAddress("neutrinoGeneta3", &neutrinoGeneta3, &b_neutrinoGeneta3);
   fChain->SetBranchAddress("neutrinoGeneta4", &neutrinoGeneta4, &b_neutrinoGeneta4);
   fChain->SetBranchAddress("pt1", &pt1, &b_pt1);
   fChain->SetBranchAddress("pt2", &pt2, &b_pt);
   fChain->SetBranchAddress("eta1", &eta1, &b_eta1);
   fChain->SetBranchAddress("eta2", &eta2, &b_eta2);
   fChain->SetBranchAddress("phi1", &phi1, &b_phi1);
   fChain->SetBranchAddress("phi2", &phi2, &b_phi2);
   fChain->SetBranchAddress("ch1", &ch1, &b_ch1);
   fChain->SetBranchAddress("ch2", &ch2, &b_ch2);
   fChain->SetBranchAddress("iso1", &iso1, &b_iso1);
   fChain->SetBranchAddress("iso2", &iso2, &b_iso2);
   fChain->SetBranchAddress("pdgid1", &pdgid1, &b_pdgid1);
   fChain->SetBranchAddress("pdgid2", &pdgid2, &b_pdgid2);
   fChain->SetBranchAddress("mll", &mll, &b_mll);
   fChain->SetBranchAddress("PTll", &PTll, &b_PTll);
   fChain->SetBranchAddress("dPhill", &dPhill, &b_dPhill);
   fChain->SetBranchAddress("dRll", &dRll, &b_dRll);
   fChain->SetBranchAddress("dEtall", &dEtall, &b_dEtall);
   fChain->SetBranchAddress("etall", &etall, &b_etall);
   fChain->SetBranchAddress("yll", &yll, &b_yll);
   fChain->SetBranchAddress("nextra", &nextra, &b_nextra);
   fChain->SetBranchAddress("sameflav", &sameflav, &b_sameflav);
   fChain->SetBranchAddress("channel", &channel, &b_channel);
   fChain->SetBranchAddress("jetGenpt1", &jetGenpt1, &b_jetGenpt1);
   fChain->SetBranchAddress("jetGenpt2", &jetGenpt2, &b_jetGenpt1);
   fChain->SetBranchAddress("jetGenpt3", &jetGenpt3, &b_jetGenpt3);
   fChain->SetBranchAddress("jetGenpt4", &jetGenpt4, &b_jetGenpt4);
   fChain->SetBranchAddress("jetGenphi1", &jetGenphi1, &b_jetGenphi1);
   fChain->SetBranchAddress("jetGenphi2", &jetGenphi2, &b_jetGenphi2);
   fChain->SetBranchAddress("jetGenphi3", &jetGenphi3, &b_jetGenphi3);
   fChain->SetBranchAddress("jetGenphi4", &jetGenphi4, &b_jetGenphi4);
   fChain->SetBranchAddress("jetGeneta1", &jetGeneta1, &b_jetGeneta1);
   fChain->SetBranchAddress("jetGeneta2", &jetGeneta2, &b_jetGeneta2);
   fChain->SetBranchAddress("jetGeneta3", &jetGeneta3, &b_jetGeneta3);
   fChain->SetBranchAddress("jetGeneta4", &jetGeneta4, &b_jetGeneta4);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("nbjet", &nbjet, &b_nbjet);
   fChain->SetBranchAddress("hardbjpb", &hardbjpb, &b_hardbjpb);
   fChain->SetBranchAddress("softbjpb", &softbjpb, &b_softbjpb);
   fChain->SetBranchAddress("njetid", &njetid, &b_njetid);
   fChain->SetBranchAddress("jeteta1", &jeteta1, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2", &jeteta2, &b_jeteta2);
   fChain->SetBranchAddress("jeteta3", &jeteta3, &b_jeteta3);
   fChain->SetBranchAddress("jeteta4", &jeteta4, &b_jeteta4);
   fChain->SetBranchAddress("jeteta5", &jeteta5, &b_jeteta5);
   fChain->SetBranchAddress("jeteta6", &jeteta6, &b_jeteta6);
   fChain->SetBranchAddress("jeteta7", &jeteta7, &b_jeteta7);
   fChain->SetBranchAddress("jeteta8", &jeteta8, &b_jeteta8);
   fChain->SetBranchAddress("jetphi1", &jetphi1, &b_jetphi1);
   fChain->SetBranchAddress("jetphi2", &jetphi2, &b_jetphi2);
   fChain->SetBranchAddress("jetphi3", &jetphi3, &b_jetphi3);
   fChain->SetBranchAddress("jetphi4", &jetphi4, &b_jetphi4);
   fChain->SetBranchAddress("jetphi5", &jetphi5, &b_jetphi5);
   fChain->SetBranchAddress("jetphi6", &jetphi6, &b_jetphi6);
   fChain->SetBranchAddress("jetphi7", &jetphi7, &b_jetphi7);
   fChain->SetBranchAddress("jetphi8", &jetphi8, &b_jetphi8);
   fChain->SetBranchAddress("jetpt1", &jetpt1, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2", &jetpt2, &b_jetpt2);
   fChain->SetBranchAddress("jetpt3", &jetpt3, &b_jetpt3);
   fChain->SetBranchAddress("jetpt4", &jetpt4, &b_jetpt4);
   fChain->SetBranchAddress("jetpt5", &jetpt5, &b_jetpt5);
   fChain->SetBranchAddress("jetpt6", &jetpt6, &b_jetpt6);
   fChain->SetBranchAddress("jetpt7", &jetpt7, &b_jetpt7);
   fChain->SetBranchAddress("jetpt8", &jetpt8, &b_jetpt8);
   fChain->SetBranchAddress("jetmass1", &jetmass1, &b_jetmass1);
   fChain->SetBranchAddress("jetmass2", &jetmass2, &b_jetmass2);
   fChain->SetBranchAddress("jetmass3", &jetmass3, &b_jetmass3);
   fChain->SetBranchAddress("jetmass4", &jetmass4, &b_jetmass4);
   fChain->SetBranchAddress("jetmass5", &jetmass5, &b_jetmass5);
   fChain->SetBranchAddress("jetmass6", &jetmass6, &b_jetmass6);
   fChain->SetBranchAddress("jetmass7", &jetmass7, &b_jetmass7);
   fChain->SetBranchAddress("jetmass8", &jetmass8, &b_jetmass8);
   fChain->SetBranchAddress("mjj", &mjj, &b_mjj);
   fChain->SetBranchAddress("detajj", &detajj, &b_detajj);
   fChain->SetBranchAddress("njet_puppi", &njet_puppi, &b_njet);
   fChain->SetBranchAddress("nbjet_puppi", &nbjet_puppi, &b_nbjet);
   fChain->SetBranchAddress("hardbjpb_puppi", &hardbjpb_puppi, &b_hardbjpb);
   fChain->SetBranchAddress("softbjpb_puppi", &softbjpb_puppi, &b_softbjpb);
   fChain->SetBranchAddress("njetid_puppi", &njetid_puppi, &b_njetid);
   fChain->SetBranchAddress("jeteta1_puppi", &jeteta1_puppi, &b_jeteta1);
   fChain->SetBranchAddress("jeteta2_puppi", &jeteta2_puppi, &b_jeteta2);
   fChain->SetBranchAddress("jeteta3_puppi", &jeteta3_puppi, &b_jeteta3);
   fChain->SetBranchAddress("jeteta4_puppi", &jeteta4_puppi, &b_jeteta4);
   fChain->SetBranchAddress("jeteta5_puppi", &jeteta5_puppi, &b_jeteta5);
   fChain->SetBranchAddress("jeteta6_puppi", &jeteta6_puppi, &b_jeteta6);
   fChain->SetBranchAddress("jeteta7_puppi", &jeteta7_puppi, &b_jeteta7);
   fChain->SetBranchAddress("jeteta8_puppi", &jeteta8_puppi, &b_jeteta8);
   fChain->SetBranchAddress("jetphi1_puppi", &jetphi1_puppi, &b_jetphi1);
   fChain->SetBranchAddress("jetphi2_puppi", &jetphi2_puppi, &b_jetphi2);
   fChain->SetBranchAddress("jetphi3_puppi", &jetphi3_puppi, &b_jetphi3);
   fChain->SetBranchAddress("jetphi4_puppi", &jetphi4_puppi, &b_jetphi4);
   fChain->SetBranchAddress("jetphi5_puppi", &jetphi5_puppi, &b_jetphi5);
   fChain->SetBranchAddress("jetphi6_puppi", &jetphi6_puppi, &b_jetphi6);
   fChain->SetBranchAddress("jetphi7_puppi", &jetphi7_puppi, &b_jetphi7);
   fChain->SetBranchAddress("jetphi8_puppi", &jetphi8_puppi, &b_jetphi8);
   fChain->SetBranchAddress("jetpt1_puppi", &jetpt1_puppi, &b_jetpt1);
   fChain->SetBranchAddress("jetpt2_puppi", &jetpt2_puppi, &b_jetpt2);
   fChain->SetBranchAddress("jetpt3_puppi", &jetpt3_puppi, &b_jetpt3);
   fChain->SetBranchAddress("jetpt4_puppi", &jetpt4_puppi, &b_jetpt4);
   fChain->SetBranchAddress("jetpt5_puppi", &jetpt5_puppi, &b_jetpt5);
   fChain->SetBranchAddress("jetpt6_puppi", &jetpt6_puppi, &b_jetpt6);
   fChain->SetBranchAddress("jetpt7_puppi", &jetpt7_puppi, &b_jetpt7);
   fChain->SetBranchAddress("jetpt8_puppi", &jetpt8_puppi, &b_jetpt8);
   fChain->SetBranchAddress("jetmass1_puppi", &jetmass1_puppi, &b_jetmass1);
   fChain->SetBranchAddress("jetmass2_puppi", &jetmass2_puppi, &b_jetmass2);
   fChain->SetBranchAddress("jetmass3_puppi", &jetmass3_puppi, &b_jetmass3);
   fChain->SetBranchAddress("jetmass4_puppi", &jetmass4_puppi, &b_jetmass4);
   fChain->SetBranchAddress("jetmass5_puppi", &jetmass5_puppi, &b_jetmass5);
   fChain->SetBranchAddress("jetmass6_puppi", &jetmass6_puppi, &b_jetmass6);
   fChain->SetBranchAddress("jetmass7_puppi", &jetmass7_puppi, &b_jetmass7);
   fChain->SetBranchAddress("jetmass8_puppi", &jetmass8_puppi, &b_jetmass8);
   fChain->SetBranchAddress("mjj_puppi", &mjj_puppi, &b_mjj);
   fChain->SetBranchAddress("detajj_puppi", &detajj_puppi, &b_detajj);
   fChain->SetBranchAddress("pfmet", &pfmet, &b_pfmet);
   fChain->SetBranchAddress("pfmetphi", &pfmetphi, &b_pfmetphi);
   fChain->SetBranchAddress("metGenpt", &metGenpt, &b_metGenpt);
   fChain->SetBranchAddress("metGenphi", &metGenphi, &b_metGenphi);
   fChain->SetBranchAddress("pfmet_puppi", &pfmet_puppi, &b_pfmet_puppi);
   fChain->SetBranchAddress("pfmetphi_puppi", &pfmetphi_puppi, &b_pfmetphi_puppi);
   Notify();
}

Bool_t readTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void readTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t readTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef readTree_cxx
