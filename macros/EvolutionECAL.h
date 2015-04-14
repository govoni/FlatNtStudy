#include "TMath.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2.h"
#include "TArc.h"
#include "TLatex.h"
 
#include <stdio.h>

double LightCollectionEfficiency(double, double);
double LightCollectionEfficiencyWeighted(double, double);
double LightCollectionEfficiencyWeightedOld(double, double);
double DamageProfileEta(double);
double DamageProfileEtaAPD(double);
double InducedAbsorptionHadronic(double, double);
double DoseLongitudinalProfile(double);
double EquilibriumFractionColorCentersEM(double*, double*);
double InducedAbsorptionEM(double, double);
double DegradationMeanEM50GeV(double);
double DegradationNonLinearityEM50GeV(double, double);
double ResolutionConstantTermEM50GeV(double);
double ChargeVPTCathode(double, double, double);
double AgingVPT(double, double, double); 
double NoiseFactorFE(double, double); 
double NoiseFactorFE2(double,double, double, double); 
Double_t noise(Double_t , Double_t , Double_t , Double_t );
Double_t ball(Double_t tau) ;
Double_t predictionI(int ETABIN, Double_t temp, Double_t lumi_tot) ;
void ReadLumi();

float lumis[8030];
float tmax = 1000.;
static const int nbmax = 1000;
float TLow = 8;

  Double_t tau_new0  = 20; // 20 ns 
  Double_t tau_new17 = 20; // 20 ns 
  Double_t tau_std   = 43; // standard shaping time


//_____________________________________________________
double LightCollectionEfficiency(double z, double mu)
{
  double f = 0;
  if(z<=0.) return f;
  if(z>=0.22) return f;
  
  double e0 =  6.91563e-02;
  double e1 =  1.64406e+00;
  double e2 =  6.42509e-01;
  double E =  e0/(1+exp(e1*(log10(mu)-e2)));

  double d0 =  3.85334e-01;
  double d1 = -1.04647e-02;
  double D = d0*exp(d1*mu);

  double c0 =  3.77629e-01;
  double c1 = -3.23755e-01;
  double c2 =  1.50247e+00;
  double c3 =  3.03278e-01;
  double C =  -1 + c0*exp(c1*mu)*(1+c2*exp(c3*mu));

  double b0 = -3.33575e-01;
  double b1 =  4.44856e-01;
  double b2 =  1.91766e+00;
  double b3 =  2.69423e+00;
  double b4 =  1.06905e+00;
  double B =  (1/mu)*(b0 + b1*log10(mu) + b2*pow(log10(mu),2) 
		      + b3*pow(log10(mu),3) + b4*pow(log10(mu),4));

  double a0 = 7.18248e-02; 
  double a1 = 1.89016e+00;
  double a2 = 2.15651e-02;
  double a3 = 2.30786e-02;
  double A =  exp(B*mu*0.015)*(a0/(exp(a1*(log10(mu)+a2))+1)+a3);

  double R = 0.01*D*( 4/(0.222+E)/(0.222+E) - 1/((0.22-z)*(z+E)) );
  f =  A * exp(-B*mu*(0.22-z)) * (1+C*exp(R));
  
  return f;
}



//_____________________________________________________
//
// This function is for CMSSW FullSim "slope_LY"
// It returns weight<=1 for light collection from relative distance "z"
//   0 < z < 1
//   z = 0 at the face of a crystal
//   z = 1 at the photo-detector
//   weight = 1 for undamaged crystal at any z
//
double LightCollectionEfficiencyWeightedOld(double z, double mu_ind)
{
  if(z<=0) return 0;
  if(z>=1) return 0;
  if(mu_ind<0) return 1;


  double mu = mu_ind + 0.1; 
  double lmu = log10(mu);
  
  double e0 =  6.91563e-02;
  double e1 =  1.64406e+00;
  double e2 =  6.42509e-01;
  double E =  e0/(1+exp(e1*(lmu-e2)));

  double d0 =  3.85334e-01;
  double d1 = -1.04647e-02;
  double D = d0*exp(d1*mu);

  double c0 =  3.77629e-01;
  double c1 = -3.23755e-01;
  double c2 =  1.50247e+00;
  double c3 =  3.03278e-01;
  double C =  -1 + c0*exp(c1*mu)*(1+c2*exp(c3*mu));

  double b0 = -3.33575e-01;
  double b1 =  4.44856e-01;
  double b2 =  1.91766e+00;
  double b3 =  2.69423e+00;
  double b4 =  1.06905e+00;
  double B =  (1/mu)*(b0 + b1*lmu + b2*pow(lmu,2) 
		         + b3*pow(lmu,3) + b4*pow(lmu,4));

  double a0 = 7.18248e-02; 
  double a1 = 1.89016e+00;
  double a2 = 2.15651e-02;
  double a3 = 2.30786e-02;
  double A =  exp(B*mu*0.015)*(a0/(exp(a1*(lmu+a2))+1)+a3);

  double R = 0.01*D*( 4/(0.222+E)/(0.222+E) - 1/((0.22*0.22)*(1.-z)*(z+E/0.22)) );

  // for undamaged crystal, mu0 = 0.1
  double A0 =  0.0845209;
  double B0 = -4.85951;
  double C0 = -0.0681855;
  double D0 =  0.384931;
  double E0 =  0.0648029;
  double R0 = 0.01*D0*( 4/(0.222+E0)/(0.222+E0) - 1/((0.22*0.22)*(1.-z)*(z+E0/0.22)) );
  
  
  double f =  A/A0 * exp(-(B*mu-B0*0.1)*0.22*(1.-z)) * (1+C*exp(R))/(1+C0*exp(R0));
  
  return f;
}

//_____________________________________________________
//
// This function is for CMSSW FullSim "slope_LY"
// It returns weight<=1 for light collection from relative distance "z"
//   0 < z < 1
//   z = 0 at the face of a crystal
//   z = 1 at the photo-detector
//   weight = 1 for undamaged crystal at any z
//
double LightCollectionEfficiencyWeighted(double z, double mu_ind)
{
  if(z<=0) return 0;
  if(z>=1) return 0;
  if(mu_ind<0) return 1;

  double mu = mu_ind + 0.7; 
  double lmu = log10(mu);
  
  double e0 =  6.91563e-02;
  double e1 =  1.64406e+00;
  double e2 =  6.42509e-01;
  double E =  e0/(1+exp(e1*(lmu-e2)));

  double d0 =  3.85334e-01;
  double d1 = -1.04647e-02;
  double D = d0*exp(d1*mu);

  double c0 =  3.77629e-01;
  double c1 = -3.23755e-01;
  double c2 =  1.50247e+00;
  double c3 =  3.03278e-01;
  double C =  -1 + c0*exp(c1*mu)*(1+c2*exp(c3*mu));

  double b0 = -3.33575e-01;
  double b1 =  4.44856e-01;
  double b2 =  1.91766e+00;
  double b3 =  2.69423e+00;
  double b4 =  1.06905e+00;
  double B =  (1/mu)*(b0 + b1*lmu + b2*pow(lmu,2) 
		         + b3*pow(lmu,3) + b4*pow(lmu,4));

  double a0 = 7.18248e-02; 
  double a1 = 1.89016e+00;
  double a2 = 2.15651e-02;
  double a3 = 2.30786e-02;
  double A =  exp(B*mu*0.015)*(a0/(exp(a1*(lmu+a2))+1)+a3);

  double R = 0.01*D*( 4/(0.222+E)/(0.222+E) - 1/((0.22*0.22)*(1.-z)*(z+E/0.22)) );

  // for undamaged crystal, mu0 = 0.7
  double A0 =  0.0631452;
  double B0 = -0.52267;
  double C0 = -0.139646;
  double D0 =  0.382522;
  double E0 =  0.054473;
  double R0 = 0.01*D0*( 4/(0.222+E0)/(0.222+E0) - 1/((0.22*0.22)*(1.-z)*(z+E0/0.22)) );
  
  
  double f =  A/A0 * exp(-(B*mu-B0*0.7)*0.22*(1.-z)) * (1+C*exp(R))/(1+C0*exp(R0));
  
  return f;
}

//_________________________________________________________
double DamageProfileEta(double eta)
{
  double x = fabs(eta);
  if(x<1.497){
    return exp( -4.11065 + 0.258478*x );
  }else{
    return exp( -13.5112 + 7.913860*x - 0.998649*x*x );
  }
}


//_________________________________________________________
double DamageProfileEtaAPD(double eta)
{
  double x = fabs(eta);
  if(x<1.497){
    double et=x/1.48*34.0;
    double etaprof=( 9.54827 + et*0.0379222 + 
		     et*et*(-0.00257047) + 
		     et*et*et*0.00073546 + 
		     et*et*et*et*(-1.49683e-05)
		     )/9.54827;
    return etaprof;
  }else{
    return 1.0;
  }
}


//_________________________________________________________
double InducedAbsorptionHadronic(double lumi, double eta)
{
  double fluence = DamageProfileEta(eta) * 2.7e+13/500.0 * lumi;
  double mu = 2.08E-13 * pow( fluence, 1.0049);
  return mu;
}


//_________________________________________________________
double DoseLongitudinalProfile(double z)
{
  double alpha = 4.72877e+00;
  double beta  = 5.91296e-01;
  double amp1  = 6.24495e+02;
  double amp2  = 1.84367e-01;
  double offset   = 2.00705e+01;
  if (z>=0.0 && z<=22.0) {
    double term1 = (amp1 / TMath::Gamma(alpha)) * pow((beta*z),(alpha-1)) * exp (-beta*z);
    double term2 = amp2*(z-11.0)*(z-11.0) + offset;
    return (term1 + term2)/150.44;
  } else {       
   return 0;
  }
}


//_________________________________________________________
double EquilibriumFractionColorCentersEM(double *x, double *par)
{
  double instantLumi = par[0];
  double eta = par[1];
  double rate =  DoseLongitudinalProfile(x[0])*5.0*DamageProfileEta(eta)*instantLumi/1e+34;
  if(rate<=0.0) rate=0.0;
  double alpha = par[2];
  return rate/(alpha + rate);
}


 

//____________________________________________________________
double InducedAbsorptionEM(double lumi, double eta)
{
  double mu_max  = 2.0;
  double alpha1  = 3.41488e+00;
  TF1 *ftmp1 = new TF1("ftmp1", EquilibriumFractionColorCentersEM, 0.0, 22.0, 3);
  ftmp1->SetParameters(lumi, eta, alpha1);
  double muEM = mu_max*ftmp1->Integral(0.0, 22.0)/22.0; 
  return muEM;
}


//_____________________________________________________________
double DegradationMeanEM50GeV(double mu)
{
  double retval = 1.0;
  double x = mu;
  if( x<1e-4   ) return retval;
  if( x>=200.0 ) x=200.0;  // parameterization is not valid for large mu

  double par[11] = {  1.00000e+01,
		     -4.41441e-01, 7.08607e-02, -3.75572e-01, -3.60410e-01, 1.30130e-01,
		     -4.72350e-01, 3.36315e-01, -1.19872e-01,  1.99574e-02,-1.22910e-03  };


  double alpha = par[0];

  double f1 = par[1]*x + par[2]*x*x;  
  double u = log(x);
  double f2 = par[10];
  for(int i=9; i>=3; i--) f2 = par[i] + f2*u;

  retval = f1/(1.0+exp(alpha*u)) + f2/(1.0+exp(-alpha*u));
  retval = exp(retval);
  return retval;
  

}




//_____________________________________________________________
double DegradationNonLinearityEM50GeV(double mu, double ene)
{
  if(ene<=1e-3) return 0.0;

  double x = mu;
  if( mu<=0.06  ) x=0.06;
  if( mu>=150.0 ) x=150.0;

  double par[9] = { 5.17712e-03, 1.97597e-02, 3.36596e-02, 2.84505e-02, 1.38480e-02,
		    1.11498e-02, 7.73634e-03, -1.30767e-03, -2.20628e-03 };

  double u = log10(x);
  double slope = par[8];
  for(int i=7; i>=0; i--) slope = par[i] + slope*u;

  double retval = 1.0 + slope*log10(ene/50.0);
  if(retval<=0.0) retval = 0.0;
  return retval;

}


//_____________________________________________________________
double ResolutionConstantTermEM50GeV(double mu)
{

  double x = mu;
  if( mu<=0.01  ) x=0.01;
  if( mu>=200.0 ) x=200.0;

  double par[10] = { -6.21503e+00,  1.59759e+00, -4.75221e-02, -3.90299e-02,  3.97269e-03,
		      2.29574e-03, -1.05280e-04, -9.60963e-05, -1.29594e-06,  1.70850e-06  };

  double u = log(x);
  double f = par[9];
  for(int i=8; i>=0; i--) f = par[i] + f*u;
  return exp(f);

}


//__________________________________________________________
double ChargeVPTCathode(double instLumi, double eta, double integralLumi)
{
  double charge = 0.0;
  double tmpLumi = 0.0;
  double stepLumi = 1.0;
  double muEM = InducedAbsorptionEM(instLumi, eta);
  while(tmpLumi<integralLumi)
    {
      tmpLumi += stepLumi;  
      double muHD = InducedAbsorptionHadronic(tmpLumi, eta);
      double SS0 = DegradationMeanEM50GeV(muEM+muHD);
      charge += SS0*0.26e-3*DamageProfileEta(eta)*stepLumi;
    }
  return charge;
}





//_________________________________________________________
double AgingVPT(double instLumi, double integralLumi, double eta) 
{
  if(fabs(eta)<1.497) return 1.0;
  double Q = ChargeVPTCathode(instLumi, eta, integralLumi);
  double result = 0.772+0.228*(3.94304e-01*exp(-Q/5.99232e-04)+(1-3.94304e-01)*exp(-Q/1.58243e-02));
  return result;
}



//_________________________________________________________
double NoiseFactorFE(double lumi, double eta)
{
  double  x = fabs(eta);
  if(x<1.497){
    return sqrt( 1.0 + 0.495*0.03512*lumi*DamageProfileEtaAPD(eta));
  }else{
    return 1.0;
  } 
} 

double NoiseFactorFE2(double lumi, double eta, double Temperature, double shaping)
{
  double  x = fabs(eta);
  if(x<1.497){

    int HVEtaBin=x/(1.497/17.0);

    Double_t cur =predictionI(HVEtaBin,Temperature,lumi);

    Double_t noisex =noise(cur,1.1979,0.4555,shaping);

    Double_t noise_ratio= noisex/noise(0.,1.1979,0.4555,43.0);


    std::cout << cur<< " "<< noisex << " " <<noise_ratio<< std::endl; 
      
      return noise_ratio;

  }else{
    return 1.0;
  } 
} 





Double_t noise(Double_t x, Double_t p1, Double_t p2, Double_t tau_new) {
  // 
  // x is the current in microAmpere
  // noise is in ADC counts 
  // Marc's formula with Botjo&FC measurements
  // with shaping time dependence (tau_new)
  
  Double_t tau=43.0; // default shaping time
  Double_t a=p1*tau/tau_new;
  Double_t b=p2/tau *tau_new;
  Double_t ballistic_deficit=ball(tau_new)/ball(tau);


  Double_t y= TMath::Sqrt((a)+TMath::Abs(b*x))/ballistic_deficit ;
  
  return y;
}

Double_t ball(Double_t tau) {
  // 
  // ballistic effect fitted from Julie's formula
  Double_t p[5]={0.1309, 0.0868,-0.003873,8.19E-05, -6.445E-07}; 

  Double_t y=p[0]+p[1]*tau+p[2]*tau*tau+p[3]*tau*tau*tau+p[4]*tau*tau*tau*tau; 

  return y;
}

Double_t predictionI(int ETABIN, Double_t temp, Double_t lumi_tot) {

  // ETABIN da 0 a 17

  std::cout<<" input="<< ETABIN<<" " <<temp<<" " <<lumi_tot<<std::endl;

  ReadLumi();

  Double_t tstand=18+273;
  Double_t top=temp+273;
  Double_t En=0.95;
  Double_t KB=8.617E-5; 

  Double_t par[3]={1.0,21.0,0.5};

    if(temp!=18){
      // recovery time changes with T
      par[1]=par[1]*TMath::Exp(En/(KB*top)-En/(KB*tstand));
      // std::cout<< par[1]<<std::endl;
    }

  Double_t result=0;
  


    
  Double_t alpha=(10.0E-11)*2.*0.0006*0.25*50.*1.1; // in microAmpere (metto 1.1 per considerare anche Isurf)
  Double_t dPhidL=2.36E11/6.3; // MARS at eta=0 at 14 TeV
  Double_t et=ETABIN*2;
  Double_t etaprof=(9.54827+et*0.0379222+et*et*(-0.00257047)+et*et*et*0.00073546+et*et*et*et*(-1.49683e-05))/9.54827;
  dPhidL=dPhidL*etaprof;


  Double_t dPhidL8TeV=1.0;
  alpha=alpha*dPhidL;

  Double_t I0=0;
  Double_t I2=0;
  

  for (int i=1; i<8030; i++){
    // std::cout<<"lumiloop"<<lumis[i]<<std::endl;

    if(lumis[i]<= lumi_tot) {

      if(i>365) dPhidL8TeV=1.02;
      if(i>365*4) dPhidL8TeV=1.0/0.8;
      
      // temporary damage recovers with time par[1]
      Double_t deltaI0=alpha*TMath::Abs(par[0])*(lumis[i]-lumis[i-1])*dPhidL8TeV;
      Double_t rI0=-I0*(1.)/TMath::Abs(par[1]);
      I0=I0+deltaI0+rI0;
      
      // permanent damage does not recover 
      Double_t deltaI2=alpha*TMath::Abs(par[2])*(lumis[i]-lumis[i-1])*dPhidL8TeV;
      I2=I2+deltaI2;
    } 

  }

  result=(I0+I2); // current per channel 

  std::cout<<"resultI="<<result<<std::endl;

  if(temp!=18){
    //  calculates the overall current at lower temperature 
    Double_t Eg=1.2; 
    
    float factor=TMath::Power(top,1.5)*TMath::Exp(-Eg/(2*KB*top))/
      (TMath::Power(tstand,1.5)*TMath::Exp(-Eg/(2*KB*tstand)));
    result=result*factor;
    std::cout <<"factor="<<factor<<std::endl;
    
  }
  


  return result; 

}


void ReadLumi(){

  float rate=0;

  for (int k=0; k<8030; k++){
    lumis[k]=0.;
    if(k<=365) rate=(5./36.)/7;  // 2011
    if(k>365 && k<=365*2) rate=(25./36.)/7; // 2012 
    if(k>365*2 && k<=365*4) rate=0.;  // LS1 2013 e 14
    if(k>365*4 && k<=365*7) rate=(50./36.)/7.; // 2015-16-17
    if(k>365*7 && k<=365*9) rate=0.;       // LS2
    if(k>365*9 && k<=365*12) rate=(50./36.)/7.; // 2021-22-23
    if(k>365*12 && k<=365*14) rate=0.;  // LS3
    if(k>365*14 && k<=365*17) rate=(445./36.)/7.;
    if(k>365*17 && k<=365*19) rate=0.; // LS4 
    if(k>365*19 && k<=365*22) rate=(445./36.)/7.;

    if(rate==0&& k>0) {
      lumis[k]=lumis[k-1];
    }else {

      int i=k%365;
      if(i==0 && k==0){
	lumis[k]=0;
      } else  {
	float byes=1.0;
	int iw=i/7;
	if(iw==0 || iw==7 || iw==14 || iw==21 || iw==28 || iw==35 || iw>=42 ) byes=0;
	lumis[k]=lumis[k-1]+rate*byes;
      }
      if(i==364)  std::cout<<lumis[k]<<std::endl;

    }

  }

}
