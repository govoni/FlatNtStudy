/*
compare for a single variable the expected stats uncertainty to the difference between H and noH
-------------------

c++ -o test_09 `root-config --glibs --cflags` -lm test_09.cpp readTree.cc plotter.cc 
./test_09

*/

#include <iostream>
#include <map>

#include "TChain.h"
#include "TLorentzVector.h"

#include "readTree.h"
#include "plotter.h"

using namespace std ;


double 
deltaPhi (double phi1, double phi2)
{
  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillHistos (plotter & analysisPlots, readTree & reader, const string sampleName)
{
  cout << "reading sample " << sampleName << "\n" ;
  int maxevents = reader.fChain->GetEntries () ;
//  maxevents = 10 ;
  // loop over events
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent)
    {
      reader.GetEntry (iEvent) ;
      if (iEvent % 10000 == 0) cout << "reading event " << iEvent << "\n" ; 

      TLorentzVector L_leadi_lepton ;
      L_leadi_lepton.SetPtEtaPhiM (reader.pt1, reader.eta1, reader.phi1, 0.) ;

      TLorentzVector L_trail_lepton ;
      L_trail_lepton.SetPtEtaPhiM (reader.pt2, reader.eta2, reader.phi2, 0.) ;

      TLorentzVector L_dilepton = L_leadi_lepton + L_trail_lepton ;

      TLorentzVector L_met ;
      L_met.SetPtEtaPhiM (reader.pfmet, 0., reader.pfmetphi, 0.) ;

      TLorentzVector L_leadi_jet ;
      L_leadi_jet.SetPtEtaPhiM (reader.jetpt1, reader.jeteta1, reader.jetphi1, 0.) ;

      TLorentzVector L_trail_jet ;
      L_trail_jet.SetPtEtaPhiM (reader.jetpt2, reader.jeteta2, reader.jetphi2, 0.) ;

      TLorentzVector L_dijet = L_leadi_jet + L_trail_jet ;

//      float asimJ = (L_leadi_jet.Pt () - L_trail_jet.Pt ()) / (L_leadi_jet.Pt () + L_trail_jet.Pt ()) ;
//      float asimL = (L_leadi_lepton.Pt () - L_trail_lepton.Pt ()) / (L_leadi_lepton.Pt () + L_trail_lepton.Pt ()) ;
//      float Rvar = L_leadi_lepton.Pt () * L_trail_lepton.Pt () / (L_leadi_jet.Pt () * L_trail_jet.Pt ()) ;

      analysisPlots.fillHisto (sampleName, "total", "dPhiLL", fabs (L_leadi_lepton.DeltaPhi (L_trail_lepton)), 1.) ;

      // pT Ws per distinguere segnale da fondo QCD

    } // loop over events
  analysisPlots.setPoissonErrors () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  
  float XS_EWK_WW2j_126 = 4.13649215685881443E-003/*pb*/ ;
  float XS_EWK_WW2j_noH = 4.49200073018412010E-003/*pb*/ ;
  float XS_QCD_WW2j_126 = 1.06691296353271774E-003/*pb*/ ;
  
  // print a table with the expected number of events
  // ---- ---- ---- ---- ---- ---- ----
  
  float baseLumi = 20. ; // fb^(-1)
  baseLumi *= 1000. ;    // transform into pb^(-1)
  cout << "lumi" << "\t" 
       << "EWK126" << "\t"
       << "EWKnoH" << "\t"
       << "dif" << "\t"
       << "QCD" << "\t"
       << "dif/QCD" << "\t"
       << "dif/rad(QCD)" << "\n" ;
  cout << "(/fb)" << "\t" 
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << "(evt)" << "\t"
       << " -- " << "\n" ;
  for (int i = 0 ; i < 8 ; ++i)
    {
      cout << (baseLumi / 1000.) << "\t" 
           << int (baseLumi * XS_EWK_WW2j_126) << "\t"
           << int (baseLumi * XS_EWK_WW2j_noH) << "\t"
           << int (baseLumi * XS_EWK_WW2j_noH - baseLumi * XS_EWK_WW2j_126) << "\t"
           << setprecision (2) << (XS_EWK_WW2j_noH - XS_EWK_WW2j_126) / (XS_QCD_WW2j_126) << "\t"
           << setprecision (2) << (baseLumi * XS_EWK_WW2j_noH - baseLumi * XS_EWK_WW2j_126) / sqrt (baseLumi * XS_QCD_WW2j_126) << "\t"
           << int (baseLumi * XS_QCD_WW2j_126) << "\n" ;
      baseLumi *= 2 ;
    }
  
  // make distributions with several luminosities
  // ---- ---- ---- ---- ---- ---- ----
 
  float lumi = 20 ; // fb^(-1)
  lumi *= 1000. ;   // transform into pb^(-1)
  plotter analysisPlots (lumi) ;
   
  // EWK 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_126 = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_126->Add ("eos/cms/store/group/dpg_ecal/alca_ecalcalib/ecalMIBI/rgerosa/TP_ANALYSIS/REDUCED_TREE/JOBS_DUMPER/PHANTOM_SS_EWK_DF_126_2p5/outDumper_0.root") ;
  int totEvents_EWK_WW2j_126 = sample_EWK_WW2j_126->GetEntries () ;

  readTree reader_EWK_WW2j_126 (sample_EWK_WW2j_126) ;

  analysisPlots.addSample ("EWK_WW2j_126", XS_EWK_WW2j_126, totEvents_EWK_WW2j_126, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j_126", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j_126", "total", "dPhiLL",    25, 0., 3.14) ; 

  fillHistos (analysisPlots, reader_EWK_WW2j_126, "EWK_WW2j_126") ;

  // EWK noH sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_EWK_WW2j_noH = new TChain ("easyDelphes") ;
  sample_EWK_WW2j_noH->Add ("eos/cms/store/group/dpg_ecal/alca_ecalcalib/ecalMIBI/rgerosa/TP_ANALYSIS/REDUCED_TREE/JOBS_DUMPER/PHANTOM_SS_EWK_DF_noH_2p5/outDumper_0.root") ;
  int totEvents_EWK_WW2j_noH = sample_EWK_WW2j_noH->GetEntries () ;

  readTree reader_EWK_WW2j_noH (sample_EWK_WW2j_noH) ;

  analysisPlots.copySampleStructure ("EWK_WW2j_noH", "EWK_WW2j_126", XS_EWK_WW2j_noH, totEvents_EWK_WW2j_noH, 1, 38) ;

  fillHistos (analysisPlots, reader_EWK_WW2j_noH, "EWK_WW2j_noH") ;

  // QCD 126 sample 
  // ---- ---- ---- ---- ---- ---- ----

  TChain * sample_QCD_WW2j_126 = new TChain ("easyDelphes") ;
  sample_QCD_WW2j_126->Add ("eos/cms/store/group/dpg_ecal/alca_ecalcalib/ecalMIBI/rgerosa/TP_ANALYSIS/REDUCED_TREE/JOBS_DUMPER/PHANTOM_SS_QCD_DF_126_2p5/outDumper_0.root") ;
  int totEvents_QCD_WW2j_126 = sample_QCD_WW2j_126->GetEntries () ;

  readTree reader_QCD_WW2j_126 (sample_QCD_WW2j_126) ;

  analysisPlots.copySampleStructure ("QCD_WW2j_126", "EWK_WW2j_126", XS_QCD_WW2j_126, totEvents_QCD_WW2j_126, 0, 8) ;

  fillHistos (analysisPlots, reader_QCD_WW2j_126, "QCD_WW2j_126") ;

  // plotting
  // ---- ---- ---- ---- ---- ---- ----

  int intlumi = lumi / 1000. ;
  for (int i = 0 ; i < 8 ; ++i)
    {
      analysisPlots.scaleAllHistos (2.) ;
      analysisPlots.setPoissonErrors () ;
      string baseFolder = "lumi_" ;
      baseFolder += to_string (intlumi) ;
      analysisPlots.plotRelativeExcessFullLayer ("total", baseFolder) ;
      intlumi *= 2. ;
    }

  return 0 ;
}  
