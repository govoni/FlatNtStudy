#include "kinematicUtils.h"

void computeAnglesResonance(TLorentzVector p4H, TLorentzVector p4V1, TLorentzVector p4F1, TLorentzVector p4F2,
                            TLorentzVector p4V2, TLorentzVector p4F3, TLorentzVector p4F4,
                            double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1){
    
  //// costhetastar in the resonance or VV system decay rest frame
  TVector3 boostX = -(p4H.BoostVector());
  TLorentzVector p4V1inXFrame( p4V1 );
  TLorentzVector p4V2inXFrame( p4V2 );
  p4V1inXFrame.Boost( boostX );
  p4V2inXFrame.Boost( boostX );
  TVector3 theV1X_p3 = TVector3( p4V1inXFrame.X(), p4V1inXFrame.Y(), p4V1inXFrame.Z() );
  TVector3 theV2X_p3 = TVector3( p4V2inXFrame.X(), p4V2inXFrame.Y(), p4V2inXFrame.Z() );    
  costhetastar = theV1X_p3.CosTheta();
    
  //// --------------------------- costheta1
  TVector3 boostV1 = -(p4V1.BoostVector());
  TLorentzVector p4F1_BV1( p4F1 );
  TLorentzVector p4F2_BV1( p4F2 );
  TLorentzVector p4F3_BV1( p4F3 );
  TLorentzVector p4F4_BV1( p4F4 );
  p4F1_BV1.Boost( boostV1 );
  p4F2_BV1.Boost( boostV1 );
  p4F3_BV1.Boost( boostV1 );
  p4F4_BV1.Boost( boostV1 );
    
  TLorentzVector p4V2_BV1 = p4F3_BV1 + p4F4_BV1;
  //// costheta1
  costheta1 = -p4V2_BV1.Vect().Dot( p4F1_BV1.Vect() )/p4V2_BV1.Vect().Mag()/p4F1_BV1.Vect().Mag();
    
  //// --------------------------- costheta2
  TVector3 boostV2 = -(p4V2.BoostVector());
  TLorentzVector p4F1_BV2( p4F1 );
  TLorentzVector p4F2_BV2( p4F2 );
  TLorentzVector p4F3_BV2( p4F3 );
  TLorentzVector p4F4_BV2( p4F4 );
  p4F1_BV2.Boost( boostV2 );
  p4F2_BV2.Boost( boostV2 );
  p4F3_BV2.Boost( boostV2 );
  p4F4_BV2.Boost( boostV2 );
    
  TLorentzVector p4V1_BV2 = p4F2_BV2 + p4F3_BV2;
  //// costheta2
  costheta2 = -p4V1_BV2.Vect().Dot( p4F3_BV2.Vect() )/p4V1_BV2.Vect().Mag()/p4F3_BV2.Vect().Mag();
    
  //// --------------------------- Phi and Phi1
  TLorentzVector p4F1_BX( p4F1 );
  TLorentzVector p4F2_BX( p4F2 );
  TLorentzVector p4F3_BX( p4F3 );
  TLorentzVector p4F4_BX( p4F4 );
    
  p4F1_BX.Boost( boostX );
  p4F2_BX.Boost( boostX );
  p4F3_BX.Boost( boostX );
  p4F4_BX.Boost( boostX );
    
  TVector3 tmp1 = p4F1_BX.Vect().Cross( p4F3_BX.Vect() );
  TVector3 tmp2 = p4F2_BX.Vect().Cross( p4F4_BX.Vect() );    
    
  TVector3 normal1_BX( tmp1.X()/tmp1.Mag(), tmp1.Y()/tmp1.Mag(), tmp1.Z()/tmp1.Mag() ); 
  TVector3 normal2_BX( tmp2.X()/tmp2.Mag(), tmp2.Y()/tmp2.Mag(), tmp2.Z()/tmp2.Mag() ); 
    
  //// Phi
  TLorentzVector p4V1_BX = p4F1_BX + p4F2_BX;    
  double tmpSgnPhi = p4V1_BX.Vect().Dot( normal1_BX.Cross( normal2_BX) );
  double sgnPhi = tmpSgnPhi/fabs(tmpSgnPhi);
  Phi = sgnPhi * acos( -1.*normal1_BX.Dot( normal2_BX) );
    
    
  //////////////
    
  TVector3 beamAxis(0,0,1);
  TVector3 tmp3 = (p4F1_BX + p4F2_BX).Vect();
    
  TVector3 p3V1_BX( tmp3.X()/tmp3.Mag(), tmp3.Y()/tmp3.Mag(), tmp3.Z()/tmp3.Mag() );
  TVector3 tmp4 = beamAxis.Cross( p3V1_BX );
  TVector3 normalSC_BX( tmp4.X()/tmp4.Mag(), tmp4.Y()/tmp4.Mag(), tmp4.Z()/tmp4.Mag() );
    
  //// Phi1
  double tmpSgnPhi1 = p4V1_BX.Vect().Dot( normal1_BX.Cross( normalSC_BX) );
  double sgnPhi1 = tmpSgnPhi1/fabs(tmpSgnPhi1);    
  Phi1 = sgnPhi1 * acos( normal1_BX.Dot( normalSC_BX) );    
    
  //    std::cout << "extractAngles: " << std::endl;
  //    std::cout << "costhetastar = " << costhetastar << ", costheta1 = " << costheta1 << ", costheta2 = " << costheta2 << std::endl;
  //    std::cout << "Phi = " << Phi << ", Phi1 = " << Phi1 << std::endl;    
    
}



void computeAnglesVBF(TLorentzVector p4H,  TLorentzVector p4V1, TLorentzVector p4F1, TLorentzVector p4F2, TLorentzVector p4V2, 
		      TLorentzVector p4F3, TLorentzVector p4F4, TLorentzVector p4J1, TLorentzVector p4J2,
		      double& costheta1, double& costheta2, double& Phi, double& costhetastar, double& Phi1){
    
  //// costhetastar in the resonance or VV system decay rest frame
  TVector3 boostX = -((p4H+p4J1+p4J2).BoostVector());
  TLorentzVector p4V1inXFrame( p4V1 );
  TLorentzVector p4V2inXFrame( p4V2 );
  p4V1inXFrame.Boost( boostX );
  p4V2inXFrame.Boost( boostX );
  TVector3 theV1X_p3 = TVector3( p4V1inXFrame.X(), p4V1inXFrame.Y(), p4V1inXFrame.Z() );
  TVector3 theV2X_p3 = TVector3( p4V2inXFrame.X(), p4V2inXFrame.Y(), p4V2inXFrame.Z() );    
  costhetastar = theV1X_p3.CosTheta();
    
  //// --------------------------- costheta1
  TVector3 boostV1 = -((p4V1+p4J1+p4J2).BoostVector());
  TLorentzVector p4F1_BV1( p4F1 );
  TLorentzVector p4F2_BV1( p4F2 );
  TLorentzVector p4F3_BV1( p4F3 );
  TLorentzVector p4F4_BV1( p4F4 );
  p4F1_BV1.Boost( boostV1 );
  p4F2_BV1.Boost( boostV1 );
  p4F3_BV1.Boost( boostV1 );
  p4F4_BV1.Boost( boostV1 );
    
  TLorentzVector p4V2_BV1 = p4F3_BV1 + p4F4_BV1;
  //// costheta1
  costheta1 = -p4V2_BV1.Vect().Dot( p4F1_BV1.Vect() )/p4V2_BV1.Vect().Mag()/p4F1_BV1.Vect().Mag();
    
  //// --------------------------- costheta2
  TVector3 boostV2 = -((p4V2+p4J1+p4J2).BoostVector());
  TLorentzVector p4F1_BV2( p4F1 );
  TLorentzVector p4F2_BV2( p4F2 );
  TLorentzVector p4F3_BV2( p4F3 );
  TLorentzVector p4F4_BV2( p4F4 );
  p4F1_BV2.Boost( boostV2 );
  p4F2_BV2.Boost( boostV2 );
  p4F3_BV2.Boost( boostV2 );
  p4F4_BV2.Boost( boostV2 );
    
  TLorentzVector p4V1_BV2 = p4F2_BV2 + p4F3_BV2;
  //// costheta2
  costheta2 = -p4V1_BV2.Vect().Dot( p4F3_BV2.Vect() )/p4V1_BV2.Vect().Mag()/p4F3_BV2.Vect().Mag();
    
  //// --------------------------- Phi and Phi1
  TLorentzVector p4F1_BX( p4F1 );
  TLorentzVector p4F2_BX( p4F2 );
  TLorentzVector p4F3_BX( p4F3 );
  TLorentzVector p4F4_BX( p4F4 );
    
  p4F1_BX.Boost( boostX );
  p4F2_BX.Boost( boostX );
  p4F3_BX.Boost( boostX );
  p4F4_BX.Boost( boostX );
    
  TVector3 tmp1 = p4F1_BX.Vect().Cross( p4F3_BX.Vect() );
  TVector3 tmp2 = p4F2_BX.Vect().Cross( p4F4_BX.Vect() );    
    
  TVector3 normal1_BX( tmp1.X()/tmp1.Mag(), tmp1.Y()/tmp1.Mag(), tmp1.Z()/tmp1.Mag() ); 
  TVector3 normal2_BX( tmp2.X()/tmp2.Mag(), tmp2.Y()/tmp2.Mag(), tmp2.Z()/tmp2.Mag() ); 
    
  //// Phi
  TLorentzVector p4V1_BX = p4F1_BX + p4F2_BX;    
  double tmpSgnPhi = p4V1_BX.Vect().Dot( normal1_BX.Cross( normal2_BX) );
  double sgnPhi = tmpSgnPhi/fabs(tmpSgnPhi);
  Phi = sgnPhi * acos( -1.*normal1_BX.Dot( normal2_BX) );
    
    
  //////////////
    
  TVector3 beamAxis(0,0,1);
  TVector3 tmp3 = (p4F1_BX + p4F2_BX).Vect();
    
  TVector3 p3V1_BX( tmp3.X()/tmp3.Mag(), tmp3.Y()/tmp3.Mag(), tmp3.Z()/tmp3.Mag() );
  TVector3 tmp4 = beamAxis.Cross( p3V1_BX );
  TVector3 normalSC_BX( tmp4.X()/tmp4.Mag(), tmp4.Y()/tmp4.Mag(), tmp4.Z()/tmp4.Mag() );
    
  //// Phi1
  double tmpSgnPhi1 = p4V1_BX.Vect().Dot( normal1_BX.Cross( normalSC_BX) );
  double sgnPhi1 = tmpSgnPhi1/fabs(tmpSgnPhi1);    
  Phi1 = sgnPhi1 * acos( normal1_BX.Dot( normalSC_BX) );    
    
  //    std::cout << "extractAngles: " << std::endl;
  //    std::cout << "costhetastar = " << costhetastar << ", costheta1 = " << costheta1 << ", costheta2 = " << costheta2 << std::endl;
  //    std::cout << "Phi = " << Phi << ", Phi1 = " << Phi1 << std::endl;    
    
}

TLorentzVector getNeutrinoPz( TLorentzVector & lepton4V, TLorentzVector & met){

  TLorentzVector neutrino4V;

  double MW = 80.385;

  double leppt  = lepton4V.Pt();
  double lepphi = lepton4V.Phi();
  double lepeta = lepton4V.Eta();
  double lepenergy = lepton4V.E();

  double metpt  = met.Pt();
  double metphi = met.Phi();
  double metpx  = metpt*cos(metphi);
  double metpy  = metpt*sin(metphi);
  double pz  = 0;

  double pxl= leppt*cos(lepphi);
  double pyl= leppt*sin(lepphi);
  double pzl= leppt*sinh(lepeta);
  double El = lepenergy;
  double ml = lepton4V.M();

  double a = pow(MW,2) + pow(metpx+pxl,2) + pow(metpy+pyl,2) - metpx*metpx - metpy*metpy - El*El + pzl*pzl;
  double b = 2.*pzl;   
  double A = b*b -4.*El*El;
  double B = 2.*a*b;
  double C = a*a-4.*(metpx*metpx+metpy*metpy)*El*El;


  int type=2; // use the small abs real root

  double otherSol = 0.; 

  a = MW*MW - ml*ml + 2.0*pxl*metpx + 2.0*pyl*metpy;
  A = 4.0*(El*El - pzl*pzl);
  B = -4.0*a*pzl;
  C = 4.0*El*El*(metpx*metpx + metpy*metpy) - a*a;

  double newPtneutrino1 =0;
  double newPtneutrino2 =0;

  double tmproot = B*B - 4.0*A*C;

  bool isComplex;

  if (tmproot<0) {

    isComplex = true;
    pz = - B/(2*A); // take real part of complex roots
    otherSol = pz;

    // recalculate the neutrino pT
    // solve quadratic eq. discriminator = 0 for pT of nu
    double pnu = metpt;
    double Delta = (MW*MW - ml*ml);
    double alpha = (pxl*metpx/pnu + pyl*metpy/pnu);
    double AA = 4.*pzl*pzl - 4*El*El + 4*alpha*alpha;
    double BB = 4.*alpha*Delta;
    double CC = Delta*Delta;

    double tmpdisc = BB*BB - 4.0*AA*CC;
    double tmpsolpt1 = (-BB + TMath::Sqrt(tmpdisc))/(2.0*AA);
    double tmpsolpt2 = (-BB - TMath::Sqrt(tmpdisc))/(2.0*AA);

    //should rejetc negative root

    if ( tmpsolpt1 >0 ) { 
      newPtneutrino1 = tmpsolpt1; 
      newPtneutrino2 = tmpsolpt2;
    }
    else { 
      newPtneutrino1 = tmpsolpt2; 
      newPtneutrino2 = tmpsolpt1; 
    }

  }
  else {

    isComplex = false;
    double tmpsol1 = (-B + TMath::Sqrt(tmproot))/(2.0*A);
    double tmpsol2 = (-B - TMath::Sqrt(tmproot))/(2.0*A);

    if (type == 0 ) {
      // two real roots, pick the one closest to pz of muon
      if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol = tmpsol1;}
      else { pz = tmpsol1; otherSol = tmpsol2; }
      // if pz is > 300 pick the most central root
      if ( abs(pz) > 300. ) {
	if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol = tmpsol2; }
	else { pz = tmpsol2; otherSol = tmpsol1; }
      }
    }
    if (type == 1 ) {
      // two real roots, pick the one closest to pz of muon
      if (TMath::Abs(tmpsol2-pzl) < TMath::Abs(tmpsol1-pzl)) { pz = tmpsol2; otherSol = tmpsol1; }
      else {pz = tmpsol1; otherSol = tmpsol2; }
    }
    if (type == 2 ) {
      // pick the most central root.
      if (TMath::Abs(tmpsol1)<TMath::Abs(tmpsol2) ) { pz = tmpsol1; otherSol = tmpsol2; }
      else { pz = tmpsol2; otherSol = tmpsol1; }
    }
    if (type == 3 ) {
      // pick the largest value of the cosine
      TVector3 p3w, p3mu;
      p3w.SetXYZ(pxl+metpx, pyl+metpy, pzl+ tmpsol1);
      p3mu.SetXYZ(pxl, pyl, pzl );

      double sinthcm1 = 2.*(p3mu.Perp(p3w))/MW;
      p3w.SetXYZ(pxl+metpx, pyl+metpy, pzl+ tmpsol2);
      double sinthcm2 = 2.*(p3mu.Perp(p3w))/MW;

      double costhcm1 = TMath::Sqrt(1. - sinthcm1*sinthcm1);
      double costhcm2 = TMath::Sqrt(1. - sinthcm2*sinthcm2);

      if ( costhcm1 > costhcm2 ) { pz = tmpsol1; otherSol = tmpsol2; }
      else { pz = tmpsol2;otherSol = tmpsol1; }

    }//end of type3

  }//endl of if real root
 
  neutrino4V.SetPxPyPzE(metpx,metpy,pz,sqrt(metpx*metpx+metpy*metpy+pz*pz));

  return neutrino4V;
}
