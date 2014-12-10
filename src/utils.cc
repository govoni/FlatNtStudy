#include "utils.h"

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
    int i = 0 ;
    pt[++i] = reader.jetTrackpt1 ;
    eta[i]  = reader.jetTracketa1 ;
    phi[i]  = reader.jetTrackphi1 ;
    mass[i] = reader.jetmass1 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo1 ;

    pt[++i] = reader.jetTrackpt2 ;
    eta[i]  = reader.jetTracketa2 ;
    phi[i]  = reader.jetTrackphi2 ;
    mass[i] = reader.jetmass2 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo2 ;

    pt[++i] = reader.jetTrackpt3 ;
    eta[i]  = reader.jetTracketa3 ;
    phi[i]  = reader.jetTrackphi3 ;
    mass[i] = reader.jetmass3 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo3 ;

    pt[++i] = reader.jetTrackpt4 ;
    eta[i]  = reader.jetTracketa4 ;
    phi[i]  = reader.jetTrackphi4 ;
    mass[i] = reader.jetmass4 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo4 ;

    pt[++i] = reader.jetTrackpt5 ;
    eta[i]  = reader.jetTracketa5 ;
    phi[i]  = reader.jetTrackphi5 ;
    mass[i] = reader.jetmass5 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo5 ;
    
    pt[++i] = reader.jetTrackpt6 ;
    eta[i]  = reader.jetTracketa6 ;
    phi[i]  = reader.jetTrackphi6 ;
    mass[i] = reader.jetmass6 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo6 ;
    
    pt[++i] = reader.jetTrackpt7 ;
    eta[i]  = reader.jetTracketa7 ;
    phi[i]  = reader.jetTrackphi7 ;
    mass[i] = reader.jetmass7 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo7 ;

    pt[++i] = reader.jetTrackpt8 ;
    eta[i]  = reader.jetTracketa8 ;
    phi[i]  = reader.jetTrackphi8 ;
    mass[i] = reader.jetmass8 ;
    ID[i]   = -1. ;
    btag[i] = reader.jetBTagAlgo8 ;
    
    return ;
  }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


// isolation applied here
void fillRecoLeptonsArray (float * pt, float * eta, float * phi, float * iso, readTree & reader)
  {
    int i = 0 ;
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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void dumpJets (std::vector<TLorentzVector> & TL_jets, std::vector<TLorentzVector> & TL_leptons, 
               float * pt, float * eta, float * phi, float * mass, float * btag, 
               float minptcut, float btagcut, float lepminptcut, float deltaR)
  {
    for (int iJet = 0 ; iJet < 8 ; ++iJet)
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
               

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- -


void fillTrackJetArray (float * pt, float * eta, float * phi, float * mass, readTree & reader)
  {
    int i = 0 ;
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