#include "utils.h"
#include <iostream>
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


bool closeToLeptons (float eta, float phi, vector<TLorentzVector> & TL_leptons, float R)
{
  for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
    {
      if ((eta - TL_leptons.at (iLep).Eta ()) * (eta - TL_leptons.at (iLep).Eta ()) +
          deltaPhi (phi, TL_leptons.at (iLep).Phi ()) * deltaPhi (phi, TL_leptons.at (iLep).Phi ()) < R * R) 
          return true ;
    }
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


bool closeToLeptons (float eta, float phi, readTree & reader, float R)
{
  if ((eta - reader.eta1) * (eta - reader.eta1) +
      deltaPhi (phi, reader.phi1) * deltaPhi (phi, reader.phi1) < R * R) return true ;
  if ((eta - reader.eta2) * (eta - reader.eta2) +
      deltaPhi (phi, reader.phi2) * deltaPhi (phi, reader.phi2) < R * R) return true ;
  return false ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillRecoJetArray (float * pt, float * eta, float * phi, 
                       float * mass, float * ID, float * btag, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.jetpt1 ;
    eta[i]  = reader.jeteta1 ;
    phi[i]  = reader.jetphi1 ;
    mass[i] = reader.jetmass1 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo1 ;

    pt[++i] = reader.jetpt2 ;
    eta[i]  = reader.jeteta2 ;
    phi[i]  = reader.jetphi2 ;
    mass[i] = reader.jetmass2 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo2 ;

    pt[++i] = reader.jetpt3 ;
    eta[i]  = reader.jeteta3 ;
    phi[i]  = reader.jetphi3 ;
    mass[i] = reader.jetmass3 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo3 ;

    pt[++i] = reader.jetpt4 ;
    eta[i]  = reader.jeteta4 ;
    phi[i]  = reader.jetphi4 ;
    mass[i] = reader.jetmass4 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo4 ;

    pt[++i] = reader.jetpt5 ;
    eta[i]  = reader.jeteta5 ;
    phi[i]  = reader.jetphi5 ;
    mass[i] = reader.jetmass5 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo5 ;
    
    pt[++i] = reader.jetpt6 ;
    eta[i]  = reader.jeteta6 ;
    phi[i]  = reader.jetphi6 ;
    mass[i] = reader.jetmass6 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo6 ;
    
    pt[++i] = reader.jetpt7 ;
    eta[i]  = reader.jeteta7 ;
    phi[i]  = reader.jetphi7 ;
    mass[i] = reader.jetmass7 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo7 ;

    pt[++i] = reader.jetpt8 ;
    eta[i]  = reader.jeteta8 ;
    phi[i]  = reader.jetphi8 ;
    mass[i] = reader.jetmass8 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo8 ;
    
    return ;
  }

// 
void fillPuppiJetArray (float * pt, float * eta, float * phi, 
                       float * mass, float * ID, float * btag, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.jetpt_puppi1 ;
    eta[i]  = reader.jeteta_puppi1 ;
    phi[i]  = reader.jetphi_puppi1 ;
    mass[i] = reader.jetmass_puppi1 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi1 ;

    pt[++i] = reader.jetpt_puppi2 ;
    eta[i]  = reader.jeteta_puppi2 ;
    phi[i]  = reader.jetphi_puppi2 ;
    mass[i] = reader.jetmass_puppi2 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi2 ;

    pt[++i] = reader.jetpt_puppi3 ;
    eta[i]  = reader.jeteta_puppi3 ;
    phi[i]  = reader.jetphi_puppi3 ;
    mass[i] = reader.jetmass_puppi3 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi3 ;

    pt[++i] = reader.jetpt_puppi4 ;
    eta[i]  = reader.jeteta_puppi4 ;
    phi[i]  = reader.jetphi_puppi4 ;
    mass[i] = reader.jetmass_puppi4 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi4 ;

    pt[++i] = reader.jetpt_puppi5 ;
    eta[i]  = reader.jeteta_puppi5 ;
    phi[i]  = reader.jetphi_puppi5 ;
    mass[i] = reader.jetmass_puppi5 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi5 ;
    
    pt[++i] = reader.jetpt_puppi6 ;
    eta[i]  = reader.jeteta_puppi6 ;
    phi[i]  = reader.jetphi_puppi6 ;
    mass[i] = reader.jetmass_puppi6 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi6 ;
    
    pt[++i] = reader.jetpt_puppi7 ;
    eta[i]  = reader.jeteta_puppi7 ;
    phi[i]  = reader.jetphi_puppi7 ;
    mass[i] = reader.jetmass_puppi7 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi7 ;

    pt[++i] = reader.jetpt_puppi8 ;
    eta[i]  = reader.jeteta_puppi8 ;
    phi[i]  = reader.jetphi_puppi8 ;
    mass[i] = reader.jetmass_puppi8 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo_puppi8 ;
    
    return ;
  }

// 
void fillGenJetArray (float * pt, float * eta, float * phi, 
                       float * mass, float * ID, float * btag, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.jetGenpt1 ;
    eta[i]  = reader.jetGeneta1 ;
    phi[i]  = reader.jetGenphi1 ;
    mass[i] = reader.jetGenm1 ;
    ID[i]   = -1. ;
    btag[i] = -1 ;

    pt[++i] = reader.jetGenpt2 ;
    eta[i]  = reader.jetGeneta2 ;
    phi[i]  = reader.jetGenphi2 ;
    mass[i] = reader.jetGenm2 ;
    ID[i]   = -1. ;
    btag[i] = -1 ;

    pt[++i] = reader.jetGenpt3 ;
    eta[i]  = reader.jetGeneta3 ;
    phi[i]  = reader.jetGenphi3 ;
    mass[i] = reader.jetGenm3 ;
    ID[i]   = -1. ;
    btag[i] = -1 ;

    pt[++i] = reader.jetGenpt4 ;
    eta[i]  = reader.jetGeneta4 ;
    phi[i]  = reader.jetGenphi4 ;
    mass[i] = reader.jetGenm4 ;
    ID[i]   = -1. ;
    btag[i] = -1 ;
    
    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


// isolation applied here
void fillRecoLeptonsArray (float * pt, float * eta, float * phi, float * iso, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.pt1 ;
    eta[i]  = reader.eta1 ;
    phi[i]  = reader.phi1 ;
    iso[i]  = reader.isoRhoCorr1 ;
    pt[++i] = reader.pt2 ;
    eta[i]  = reader.eta2 ;
    phi[i]  = reader.phi2 ;
    iso[i]  = reader.isoRhoCorr2 ;
    pt[++i] = reader.pt3 ;
    eta[i]  = reader.eta3 ;
    phi[i]  = reader.phi3 ;
    iso[i]  = reader.isoRhoCorr3 ;
    pt[++i] = reader.pt4 ;
    eta[i]  = reader.eta4 ;
    phi[i]  = reader.phi4 ;
    iso[i]  = reader.isoRhoCorr4 ;
    return ;
  }

void fillRecoLeptonsArray (float * pt, float * eta, float * phi, float * iso, float* charge, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.pt1 ;
    eta[i]  = reader.eta1 ;
    phi[i]  = reader.phi1 ;
    iso[i]  = reader.isoRhoCorr1 ;
    charge[i]  = reader.ch1 ;
    pt[++i] = reader.pt2 ;
    eta[i]  = reader.eta2 ;
    phi[i]  = reader.phi2 ;
    iso[i]  = reader.isoRhoCorr2 ;
    charge[i]  = reader.ch2 ;
    pt[++i] = reader.pt3 ;
    eta[i]  = reader.eta3 ;
    phi[i]  = reader.phi3 ;
    iso[i]  = reader.isoRhoCorr3 ;
    charge[i]  = reader.ch3 ;
    pt[++i] = reader.pt4 ;
    eta[i]  = reader.eta4 ;
    phi[i]  = reader.phi4 ;
    iso[i]  = reader.isoRhoCorr4 ;
    charge[i]  = reader.ch4 ;
    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void dumpLeptons (vector<TLorentzVector> & TL_leptons, 
                  float * pt, float * eta, float * phi, float * iso, float isocut, float minptcut)
  {
    for (int i = 0 ; i < 4 ; ++i)
      {
        if (iso[i] > isocut) continue ;
        if (pt[i] < minptcut) continue ;
        TLorentzVector L_lepton ;
        L_lepton.SetPtEtaPhiM (pt[i], eta[i], phi[i], 0.) ;
        TL_leptons.push_back (L_lepton) ;
      }
    return ;
  }

void dumpLeptons (vector<TLorentzVector> & TL_leptons, 
                  float * pt, float * eta, float * phi, float * iso, float* charge, float isocut, float minptcut)
  {
    for (int i = 0 ; i < 4 ; ++i)
      {
        if (iso[i] > isocut)  { charge[i] = -99; continue ;}
        if (pt[i] < minptcut) { charge[i] = -99; continue ;}
        TLorentzVector L_lepton ;
        L_lepton.SetPtEtaPhiM (pt[i], eta[i], phi[i], 0.) ;
        TL_leptons.push_back (L_lepton) ;
      }
    return ;
  }


void dumpLeptons(std::vector<leptonContainer> & TL_leptons, float * pt, float * eta, float * phi, float * iso, float isocut, float minptcut){
    for (int i = 0 ; i < 4 ; ++i)
      {
        if (iso[i] > isocut)  continue ;
        if (pt[i] < minptcut) continue ;
        TLorentzVector L_lepton ;
        L_lepton.SetPtEtaPhiM (pt[i], eta[i], phi[i], 0.) ;
        leptonContainer lepton(L_lepton,iso[i]);
        TL_leptons.push_back (lepton) ;
      }
    return ;
}


void dumpLeptons(std::vector<leptonContainer> & TL_leptons, float * pt, float * eta, float * phi, float * iso, float* charge, float isocut, float minptcut){
    for (int i = 0 ; i < 4 ; ++i)
      {
        if (iso[i] > isocut)  continue ;
        if (pt[i] < minptcut) continue ;
        TLorentzVector L_lepton ;
        L_lepton.SetPtEtaPhiM (pt[i], eta[i], phi[i], 0.) ;
        leptonContainer lepton(L_lepton,charge[i],iso[i]);
        TL_leptons.push_back (lepton) ;
      }
    return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void dumpJets (std::vector<TLorentzVector> & TL_jets, std::vector<TLorentzVector> & TL_leptons, 
               float * pt, float * eta, float * phi, float * mass, float * btag, 
               float minptcut, float btagcut, float lepminptcut, float deltaR, int Njets)
  {
    for (int iJet = 0 ; iJet < Njets ; ++iJet)
      {
        if (pt[iJet] < minptcut) continue ;
        if (btag[iJet] > btagcut) continue ;
        TLorentzVector L_jet ;
        L_jet.SetPtEtaPhiM (pt[iJet], eta[iJet], phi[iJet], mass[iJet]) ;
        bool discard = false ;
        for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
          {
            if (TL_leptons.at (iLep).Pt () < lepminptcut) continue ;
            if (TL_leptons.at (iLep).DeltaR (L_jet) < deltaR) 
              {
                discard = true ;
                break ;
              }
          }
        if (discard) continue ;
        TL_jets.push_back (L_jet) ;        
      }
    return ;
  }
               

void dumpJets (std::vector<TLorentzVector> & TL_jets, std::vector<leptonContainer> & TL_leptons, 
               float * pt, float * eta, float * phi, float * mass, float * btag, 
               float minptcut, float btagcut, float lepminptcut, float deltaR, int Njets)
  {
    for (int iJet = 0 ; iJet < Njets ; ++iJet)
      {
        if (pt[iJet] < minptcut) continue ;
        if (btag[iJet] > btagcut) continue ;
        TLorentzVector L_jet ;
        L_jet.SetPtEtaPhiM (pt[iJet], eta[iJet], phi[iJet], mass[iJet]) ;
        bool discard = false ;
        for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
          {
            if (TL_leptons.at (iLep).lepton4V_.Pt () < lepminptcut) continue ;
            if (TL_leptons.at (iLep).lepton4V_.DeltaR (L_jet) < deltaR) 
              {
                discard = true ;
                break ;
              }
          }
        if (discard) continue ;
        TL_jets.push_back (L_jet) ;        
      }
    return ;
  }
               



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillTrackJetArray (float * pt, float * eta, float * phi, float * mass, readTree & reader)
  {
    int i = -1 ;
    pt[++i] = reader.jetTrackpt1 ;
    eta[i]  = reader.jetTracketa1 ;
    phi[i]  = reader.jetTrackphi1 ;
    mass[i] = reader.jetTrackm1 ;
    
    pt[++i] = reader.jetTrackpt2 ;
    eta[i]  = reader.jetTracketa2 ;
    phi[i]  = reader.jetTrackphi2 ;
    mass[i] = reader.jetTrackm2 ;
    
    pt[++i] = reader.jetTrackpt3 ;
    eta[i]  = reader.jetTracketa3 ;
    phi[i]  = reader.jetTrackphi3 ;
    mass[i] = reader.jetTrackm3 ;
    
    pt[++i] = reader.jetTrackpt4 ;
    eta[i]  = reader.jetTracketa4 ;
    phi[i]  = reader.jetTrackphi4 ;
    mass[i] = reader.jetTrackm4 ;
    
    pt[++i] = reader.jetTrackpt5 ;
    eta[i]  = reader.jetTracketa5 ;
    phi[i]  = reader.jetTrackphi5 ;
    mass[i] = reader.jetTrackm5 ;
    
    pt[++i] = reader.jetTrackpt6 ;
    eta[i]  = reader.jetTracketa6 ;
    phi[i]  = reader.jetTrackphi6 ;
    mass[i] = reader.jetTrackm6 ;
    
    pt[++i] = reader.jetTrackpt7 ;
    eta[i]  = reader.jetTracketa7 ;
    phi[i]  = reader.jetTrackphi7 ;
    mass[i] = reader.jetTrackm7 ;
    
    pt[++i] = reader.jetTrackpt8 ;
    eta[i]  = reader.jetTracketa8 ;
    phi[i]  = reader.jetTrackphi8 ;
    mass[i] = reader.jetTrackm8 ;
    
    return ;
  }
               

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void dumpTrackJets (std::vector<TLorentzVector> & TL_jets, std::vector<TLorentzVector> & TL_leptons, 
                    float * pt, float * eta, float * phi, float * mass, 
                    float minptcut, float lepminptcut, float deltaR)
  {
    for (int iJet = 0 ; iJet < 8 ; ++iJet)
      {
        if (pt[iJet] < minptcut) continue ;
        TLorentzVector L_jet ;
        L_jet.SetPtEtaPhiM (pt[iJet], eta[iJet], phi[iJet], mass[iJet]) ;
        bool discard = false ;
        for (unsigned int iLep = 0 ; iLep < TL_leptons.size () ; ++iLep)
          {
            if (TL_leptons.at (iLep).Pt () < lepminptcut) continue ;
            if (TL_leptons.at (iLep).DeltaR (L_jet) < deltaR) 
              {
                discard = true ;
                break ;
              }
          }
        if (discard) continue ;
        TL_jets.push_back (L_jet) ;        
      }
    return ;
  }
