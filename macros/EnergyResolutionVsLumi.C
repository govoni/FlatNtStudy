#include "EvolutionECAL.h"
#include "Riostream.h"
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
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TGaxis.h"
#include "TPaveText.h"

#include <stdio.h>
#include <sstream>
#include <string> // this should be already included in <sstream>


struct DegradationAtEta{
  double eta;
  double instLumi;
  double totLumi;
  double muEM;
  double muHD;
  double ampDropTransparency;
  double ampDropPhotoDetector;
  double ampDropTotal;
  double noiseIncreaseADC;
  double resolutitonConstantTerm;
};

double calcEta(int ix, int iy){

  double eta= -log(tan(0.5*atan(sqrt((ix-50.5)*(ix-50.5)+(iy-50.5)*(iy-50.5))*2.98/328.)));
  eta = fabs(eta);
  
  return eta;
 
}


DegradationAtEta CalculateDegradation2(double eta, double totLumi, double instLumi, double temp=18.0, double shap=43.0)
{
  DegradationAtEta result;



  result.eta = eta;
  result.totLumi = totLumi;
  result.instLumi = instLumi;



  // Index of induced absorption due to EM damages in PWO4
  result.muEM = InducedAbsorptionEM(instLumi, eta);
  
  // Index of induced absorption due to hadron damages in PWO4
  result.muHD = InducedAbsorptionHadronic(totLumi, eta);

  // Average degradation of amplitude due to transparency change
  result.ampDropTransparency = DegradationMeanEM50GeV(result.muEM+result.muHD);

  // Average degradation of amplitude due to photo-detector aging
  result.ampDropPhotoDetector = AgingVPT(instLumi, totLumi, eta);

  result.ampDropTotal = result.ampDropTransparency*result.ampDropPhotoDetector;

  // Noise increase in ADC counts due to photo-detector and front-end
  result.noiseIncreaseADC = NoiseFactorFE2(totLumi, eta, temp, shap);

  // Resolution degradation due to LY non-uniformity caused by transparency loss
  result.resolutitonConstantTerm = ResolutionConstantTermEM50GeV(result.muEM+result.muHD);

  return result;
}
DegradationAtEta CalculateDegradation(double eta, double totLumi, double instLumi)
{
  DegradationAtEta result;



  result.eta = eta;
  result.totLumi = totLumi;
  result.instLumi = instLumi;

  std::cout <<"eta/totlum/luminst"<<eta<<" "<<totLumi<<" "<<instLumi<<std::endl;

  // Index of induced absorption due to EM damages in PWO4
  result.muEM = InducedAbsorptionEM(instLumi, eta);
  
  // Index of induced absorption due to hadron damages in PWO4
  result.muHD = InducedAbsorptionHadronic(totLumi, eta);

  // Average degradation of amplitude due to transparency change
  result.ampDropTransparency = DegradationMeanEM50GeV(result.muEM+result.muHD);

  // Average degradation of amplitude due to photo-detector aging
  result.ampDropPhotoDetector = AgingVPT(instLumi, totLumi, eta);

  result.ampDropTotal = result.ampDropTransparency*result.ampDropPhotoDetector;

  // Noise increase in ADC counts due to photo-detector and front-end
  result.noiseIncreaseADC = NoiseFactorFE(totLumi, eta);

  // Resolution degradation due to LY non-uniformity caused by transparency loss
  result.resolutitonConstantTerm = ResolutionConstantTermEM50GeV(result.muEM+result.muHD);

  return result;
}



double  Resolution(double eta, double totLumi, double instLumi, double ene)
{  


  // Initial parameters for ECAL resolution
  double S;
  double Nadc;
  double adc2GeV;
  double C;
  if(eta<1.497){
    S = 0.028;           // CMS note 2006/148 (EB test beam)
    Nadc = 1.1; 
    adc2GeV = 0.039;
    C = 0.003;
  }else{
    S = 0.052;          //  CMS DN 2009/002
    Nadc = 2.0;
    adc2GeV = 0.069;
    C = 0.004;
  }


  DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);

  // adjust resolution parameters
  S /= sqrt(d.ampDropTotal);
  Nadc *= d.noiseIncreaseADC;
  adc2GeV /= d.ampDropTotal;
  double N = Nadc*adc2GeV*3.0;   // 3x3 noise in GeV 
  C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);

  return sqrt(S*S/ene + N*N/ene/ene + C*C);

}


void EnergyResolutionVsLumi()
{
  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;

  double eta = 2.8;
  double factor = 1.0;
  factor = sin(2.0*atan(exp(-1.0*eta)));


  for(double ene=1.0; ene<1e+3; ene*=1.1){
    gr1->SetPoint(ngr1, ene*factor, Resolution(eta, 0.0, 0.0, ene) );
    ngr1++;
    gr2->SetPoint(ngr2, ene*factor, Resolution(eta, 500.0, 5+34, ene) );
    ngr2++;
    gr3->SetPoint(ngr3, ene*factor, Resolution(eta, 3000.0, 5+34, ene) );
    ngr3++;
  }

  gr1->SetMinimum(1e-3);
  gr1->SetMaximum(1.);
  gr1->SetLineColor(1);
  gr2->SetLineColor(4);
  gr3->SetLineColor(2);
  gr1->SetLineWidth(3);
  gr2->SetLineWidth(3);
  gr3->SetLineWidth(3);


  gr1->Draw("AL");
  gr2->Draw("L");
  gr3->Draw("L");

  TLegend *leg = new TLegend( 0.1, 0.7, 0.48, 0.9 );
  leg->AddEntry(gr1,"undamaged","l");
  leg->AddEntry(gr2,"5e+34,  300 fb^{-1}","l");
  leg->AddEntry(gr3,"5e+34, 3000 fb^{-1}","l");
  leg->Draw();

}




void EnergyResolutionVsEta()
{
  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;

  double ene0 = 50.0;
  for(double eta=0.0; eta<2.9; eta+=0.05){
    double factor = 1.0;
    factor = sin(2.0*atan(exp(-1.0*eta)));
    double ene = ene0/factor;
    gr1->SetPoint(ngr1, eta, Resolution(eta, 0.0, 0.0, ene) );
    ngr1++;
    gr2->SetPoint(ngr2, eta, Resolution(eta, 500.0, 5+34, ene) );
    ngr2++;
    gr3->SetPoint(ngr3, eta, Resolution(eta, 3000.0, 5+34, ene) );
    ngr3++;
  }

  gr1->SetMinimum(1e-3);
  gr1->SetMaximum(1.);
  gr1->SetLineColor(1);
  gr2->SetLineColor(4);
  gr3->SetLineColor(2);
  gr1->SetLineWidth(3);
  gr2->SetLineWidth(3);
  gr3->SetLineWidth(3);


  gr1->Draw("AL");
  gr2->Draw("L");
  gr3->Draw("L");

  TLegend *leg = new TLegend( 0.1, 0.7, 0.48, 0.9 );
  leg->AddEntry(gr1,"undamaged","l");
  leg->AddEntry(gr2,"5e+34,  300 fb^{-1}","l");
  leg->AddEntry(gr3,"5e+34, 3000 fb^{-1}","l");
  leg->Draw();

}





void Decomposition()
{

  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;
  TGraph *gr4 = new TGraph();
  int ngr4 = 0;

  double eta = 2.2;
  double instLumi = 5.0e+33;
  double totLumi = 23.;
  DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);


  // Initial parameters for ECAL resolution
  double S;
  double Nadc;
  double adc2GeV;
  double C;
  if(eta<1.497){
    S = 0.028;           // CMS note 2006/148 (EB test beam)
    Nadc = 1.1; 
    adc2GeV = 0.039;
    //  C = 0.003; // original value by Sasha
    C = 0.007; // EGM-11-001
  }else{
    S = 0.052;          //  CMS DN 2009/002
    Nadc = 2.0;
    adc2GeV = 0.069;
    //    C = 0.0038; // original value by Sasha
    C = 0.02;  // EGM-11-001
  }


  // adjust resolution parameters
  S /= sqrt(d.ampDropTotal);
  Nadc *= d.noiseIncreaseADC;
  adc2GeV /= d.ampDropTotal;
  double N = Nadc*adc2GeV*5.0;   // 5x5 noise in GeV 
  C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);


  // Printout 
  cout << " ------------------------------------------------------- " << endl;
  cout << "   eta              = " << d.eta << endl;
  cout << "   instLumi         = " << d.instLumi << endl;
  cout << "   totLumi          = " << d.totLumi << endl;
  cout << "   muEM             = " << d.muEM << endl;
  cout << "   muHD             = " << d.muHD << endl;
  cout << "   ampTransparency  = " << d.ampDropTransparency << endl;
  cout << "   ampPhotoDetector = " << d.ampDropPhotoDetector << endl;
  cout << "   ampDropTotal     = " << d.ampDropTotal << endl;
  cout << "   noiseIncreaseADC = " << d.noiseIncreaseADC << endl;
  cout << "   constantTerm     = " << d.resolutitonConstantTerm << endl;
  cout << " ------------------------------------------------------- " << endl;


  for(double ene=1.0; ene<1e+3; ene*=1.1){

    double res =  sqrt(S*S/ene + N*N/ene/ene + C*C);

    double factor = 1.0;
    factor = sin(2.0*atan(exp(-1.0*eta)));
  
    gr1->SetPoint(ngr1, ene*factor, sqrt(S*S/ene) );
    ngr1++;
    gr2->SetPoint(ngr2, ene*factor, sqrt(N*N/ene/ene) );
    ngr2++;
    gr3->SetPoint(ngr3, ene*factor, sqrt(C*C) );
    ngr3++;
    gr4->SetPoint(ngr4, ene*factor, res );
    ngr4++;

  }

  gr4->SetMinimum(1e-3);
  gr4->SetMaximum(1.);
  gr4->SetLineColor(1);
  gr1->SetLineColor(4);
  gr2->SetLineColor(3);
  gr3->SetLineColor(2);

  gr1->SetLineWidth(3);
  gr2->SetLineWidth(3);
  gr3->SetLineWidth(3);
  gr4->SetLineWidth(3);

  
  gr4->Draw("AL");
  gr1->Draw("L");
  gr2->Draw("L");
  gr3->Draw("L");

  TLegend *leg = new TLegend( 0.1, 0.7, 0.48, 0.9 );
  leg->AddEntry(gr1,"Stoch","l");
  leg->AddEntry(gr2,"Noise","l");
  leg->AddEntry(gr3,"Const","l");
  leg->AddEntry(gr4,"Total","l");
  leg->Draw();




  
}


void DecompositionEnergyVsIntLumi(double eta=2.0, double integratedLumi=1000.0, double temperature=18.0, double shaping_time=43.0)
{

  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;
  TGraph *gr4 = new TGraph();
  int ngr4 = 0;

  
  //  double eta = 2.0;
  double instLumi = 1.0e+34;
  bool ene_or_et = false; // true for energy - false for ET  

  double ene=60.;
  double factor = 1.0;
  factor = sin(2.0*atan(exp(-1.0*eta)));
  if(!ene_or_et)  ene=ene/factor;  // ET



  int nstep=10; 
  double deltaL=integratedLumi/nstep;
  double totLumi = 0.-deltaL;

  for (int ic=0; ic<nstep+1; ic++){

    totLumi=totLumi+deltaL;
    if(totLumi>300)instLumi = 5.0e+34; 
    DegradationAtEta d = CalculateDegradation2(eta, totLumi, instLumi, temperature, shaping_time);


  // Initial parameters for ECAL resolution
    double S;
    double Nadc;
    double adc2GeV;
    double C;
    if(eta<1.497){
      S = 0.028;           // CMS note 2006/148 (EB test beam)
      Nadc = 1.1; 
      adc2GeV = 0.039;
      //  C = 0.003; // original value by Sasha
      C = 0.01; // EGM-11-001
    }else{
      S = 0.052;          //  CMS DN 2009/002
      Nadc = 2.0;
      adc2GeV = 0.069;
      //    C = 0.0038; // original value by Sasha
      C = 0.02;  // EGM-11-001
    }
    
    
    // adjust resolution parameters
    S /= sqrt(d.ampDropTotal);
    Nadc *= d.noiseIncreaseADC;
    adc2GeV /= d.ampDropTotal;
    double NXt = 3; // tower of 3x3 or 5x5 crystals
    double N = Nadc*adc2GeV*NXt;   // noise in GeV in the tower 
    double C_unaged =C;
    C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);
    
    

    double res =  sqrt(S*S/ene + N*N/ene/ene + C*C); // sigma(E)/E 

    if(totLumi==integratedLumi){
    // Printout 
    cout << " ------------------------------------------------------- " << endl;
    cout << "   eta              = " << d.eta << endl;
    cout << "   instLumi         = " << d.instLumi << endl;
    cout << "   totLumi          = " << d.totLumi << endl;

    cout << "   sigma(E)/E = a/sqrt(E) (+) b/E (+) C " << endl;

    cout << "   stochastic term   a(GeV^-1/2) = " << S << endl;
    cout << "   noise term        b(GeV) = " << N << " (noise is calculated in a tower of " <<NXt<< "x"<<NXt<<" crystals)"<< endl;
    cout << "   constantTerm      C  = " << C << endl;
    cout << "(unaged stoch. term  a(GeV^-1/2) = " << S*sqrt(d.ampDropTotal) << ")"<< endl;
    cout << "(unaged noise term   b(GeV) = " << N/d.noiseIncreaseADC*d.ampDropTotal <<  ")"<< endl;
    cout << "(unaged constantTerm C  = " << C_unaged << ")"<< endl;
    
    cout << "  Energy="<<ene<< endl;
    cout << "  Energy res at E="<< ene<<" GeV is "<< res<< std::endl;
    cout << "  crystal transparency loss + photodector loss="<<d.ampDropTotal<<std::endl; 
    cout << " ------------------------------------------------------- " << endl;
    }


    gr1->SetPoint(ngr1,totLumi , sqrt(S*S/ene) );
    ngr1++;
    gr2->SetPoint(ngr2, totLumi, sqrt(N*N/ene/ene) );
    ngr2++;
    gr3->SetPoint(ngr3,totLumi , sqrt(C*C) );
    ngr3++;
    gr4->SetPoint(ngr4, totLumi, res );
    ngr4++;

  }

  gr4->SetMinimum(0.);
  gr4->SetMaximum(0.3);
  gr4->SetLineColor(1);
  gr1->SetLineColor(4);
  gr2->SetLineColor(3);
  gr3->SetLineColor(2);

  gr1->SetLineWidth(3);
  gr2->SetLineWidth(3);
  gr3->SetLineWidth(3);
  gr4->SetLineWidth(3);

  gr4->GetXaxis()->SetTitle("Integrated Luminosity (fb^{-1})");
  gr4->GetYaxis()->SetTitle("Energy resolution ");


  
  gr4->Draw("AL");
  gr1->Draw("L");
  gr2->Draw("L");
  gr3->Draw("L");

  TLegend *leg = new TLegend( 0.2, 0.7, 0.48, 0.9,NULL,"brNDC" );
  leg->SetBorderSize(1);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(0.04);
   leg->AddEntry(gr1,"Stoch","l");
  leg->AddEntry(gr2,"Noise","l");
  leg->AddEntry(gr3,"Const","l");
  leg->AddEntry(gr4,"Total","l");
  leg->Draw();


  TPaveText *pts = new TPaveText(0.35,0.88,0.8,0.94,"BRNDC");
  pts->SetName("apds");
  pts->SetBorderSize(0);
  pts->SetFillColor(0);
  pts->SetFillStyle(0);
  pts->SetTextAlign(12);
  pts->SetTextSize(0.038);
  TString aaa="#eta=";
  ostringstream myconvert;
  myconvert << fixed << setprecision(2) << eta; 
  string result = myconvert.str();
  aaa.Append(result);

  TString bbb=", ET=60GeV, E=";
  ostringstream myconvert1;
  myconvert1 << fixed << setprecision(1) << ene; 
  string result1 = myconvert1.str();
  bbb.Append(result1);
  aaa.Append(bbb);
  aaa.Append("GeV");
  TText *text7 = pts->AddText(aaa);
  pts->Draw();




  
}

void PlotWeight(double eta=1.0){

  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr4 = new TGraph();
  int ngr4 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;

  //  double eta = 2.0;
    double instLumi = 5.0e+34;
  double totLumi = 1000.0;
  // double instLumi =0.0;
  //double totLumi = 0.0;
  DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);

  double mu_ind=   d.muEM+d.muHD;



  // Printout 
  cout << " ------------------------------------------------------- " << endl;
  cout << "   eta              = " << d.eta << endl;
  cout << "   instLumi         = " << d.instLumi << endl;
  cout << "   totLumi          = " << d.totLumi << endl;
  cout << "   muEM             = " << d.muEM << endl;
  cout << "   muHD             = " << d.muHD << endl;
  cout << "   ampTransparency  = " << d.ampDropTransparency << endl;
  cout << "   ampPhotoDetector = " << d.ampDropPhotoDetector << endl;
  cout << "   ampDropTotal     = " << d.ampDropTotal << endl;
  cout << "   noiseIncreaseADC = " << d.noiseIncreaseADC << endl;
  cout << "   constantTerm     = " << d.resolutitonConstantTerm << endl;
  cout << " ------------------------------------------------------- " << endl;


  //  double xx=d.ampDropTotal;
  double xx=d.ampDropTransparency;
  gr2->SetPoint(ngr2, 0.35,xx );
  ngr2++;
  gr2->SetPoint(ngr2, 0.45,xx );
  ngr2++;
  double yy=d.ampDropTotal;
  gr3->SetPoint(ngr3, 0.35,yy );
  ngr3++;
  gr3->SetPoint(ngr3, 0.45,yy );
  ngr3++;

  //  double xt_length=0.22;

  for (int i=0; i<50; i++){
    double z=i*0.02;
    //    double zcrys=z*xt_length;
    //    double weightA= LightCollectionEfficiency(zcrys, 0.7);
    //double weightB= LightCollectionEfficiency(zcrys, mu_ind+0.7);
    double weight= LightCollectionEfficiencyWeighted(z, mu_ind);
    //    double weight2= weightB/weightA*yy/xx;
    double weight2= weight*yy/xx;
    gr1->SetPoint(ngr1, z,weight  );
    ngr1++;
    gr4->SetPoint(ngr4, z,weight2  );
    ngr4++;

  }

  gr1->SetLineColor(4);
  gr1->SetLineWidth(3);
  gr1->GetXaxis()->SetTitle("z/L_{xtal}");
  gr1->GetYaxis()->SetTitle("relative Light Output");
 
  gr1->Draw("AL");

  gr2->SetLineColor(kGreen);
  gr2->SetLineWidth(3);

  gr2->Draw("L");

  gr3->SetLineColor(kRed);
  gr3->SetLineWidth(3);

  gr3->Draw("L");


  gr4->SetLineColor(kRed);
  gr4->SetLineWidth(3);
  gr4->SetLineStyle(3);
  gr4->Draw("L");

}




void TowerPlot(){

  int IX[14648];
  int IY[14648];
  int IZ[14648];
  int TOWERID[14648];
  float TowerNoiseT[100000];
  
  for (int j=0; j<100000; j++){
    TowerNoiseT[j]=0;
    
  }


   FILE *fp = fopen("CMSSW.txt","r");
   char line[100];   
   // first jump 1 line
   fgets(line,100,fp);

   int ix,	iy,	iz,	hi,	dcc_id,	tower,	strip,	xtal,	tcc_id,	tt,	tt_strip,	tt_xtal,	DataSector,	CCUID	,Dee,	hv,	LV,	LMM,	towerid;

   int k=-1;
   while (fgets(line,100,fp)) {
     k++;

     sscanf(&line[0],"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&ix,&iy,&iz,&hi,&dcc_id,&tower,&strip,&xtal,&tcc_id,&tt,&tt_strip,&tt_xtal,&DataSector,&CCUID,&Dee,&hv,&LV,&LMM,&towerid);
     

     IX[k]=ix;
     IY[k]=iy;
     IZ[k]=iz;
     TOWERID[k]=towerid-939500000;

   }
   fclose(fp);

  double totLumi = 3000.;
  double instLumi = 1.0e+34;
  if(totLumi>300)instLumi = 5.0e+34; 

  for (int i=0; i<14648; i++){
     if(IZ[i]>0){
       double eta = calcEta(IX[i],IY[i]);


       DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);


       // Initial parameters for ECAL resolution
       double S;
       double Nadc;
       double adc2GeV;
       double C;
       if(eta<1.497){
	 S = 0.028;           // CMS note 2006/148 (EB test beam)
	 Nadc = 1.1; 
	 adc2GeV = 0.039;
	 //  C = 0.003; // original value by Sasha
	 C = 0.01; // EGM-11-001
       }else{
	 S = 0.052;          //  CMS DN 2009/002
	 Nadc = 2.0;
	 adc2GeV = 0.069;
	 //    C = 0.0038; // original value by Sasha
	 C = 0.02;  // EGM-11-001
       }
       
    
       // adjust resolution parameters
       S /= sqrt(d.ampDropTotal);
       Nadc *= d.noiseIncreaseADC;
       adc2GeV /= d.ampDropTotal;
       double N = Nadc*adc2GeV;   // 1xtal noise in GeV 
       C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);
       
    
       double ene=1.0; // this is our ET
       double factor = 1.0;
       factor = sin(2.0*atan(exp(-1.0*eta)));
       ene=ene/factor;  // this is the Energy for a given ET cut at a given eta 
       // this is the ET equivalent of the noise 

        double noiseT=N*factor;
	// double noiseT=N;
       


       //       double res =  sqrt(S*S/ene + N*N/ene/ene + C*C);
    
       TowerNoiseT[TOWERID[i]]=TowerNoiseT[TOWERID[i]]+ noiseT*noiseT;
       


     }
   }

  // facciamo le radici quadrate ... 
  for (int j=0; j<100000; j++){
    
    if(TowerNoiseT[j]>0) {
      TowerNoiseT[j]=sqrt(TowerNoiseT[j]);
      std::cout<<"noiseT="<<TowerNoiseT[j]<<std::endl;

    }
  }

  TH2F *h2_100 = new TH2F("h2_100","h2_100",100,0.0,100.0,100,0.,100.0);
  for (int i=0; i<14648; i++){
     if(IZ[i]>0){
       float x=IX[i];
       float y=IY[i];
       
       h2_100->Fill(x,y,3*TowerNoiseT[TOWERID[i]]);

     }

  }


  h2_100->GetXaxis()->SetTitle("ix");
  h2_100->GetYaxis()->SetTitle("iy");
  // h2_100->GetZaxis()->SetTitle("#sigma^{N}_{T}(GeV)");
  h2_100->Draw("zcol");



	
	TLatex *   tex = new TLatex(0.18,0.96,"CMS Simulation Preliminary   ECAL Endcaps");
		tex->SetNDC();
	tex->SetTextFont(42);
	tex->SetTextSize(0.04);
	tex->SetLineWidth(2);
	tex->Draw();
	
        TLatex *lff= new TLatex(0.87,0.96,"3#sigma^{N}_{T}(GeV)");;
		lff->SetNDC();
	lff->SetTextFont(42);
	lff->SetTextSize(0.04);
	lff->SetLineWidth(2);
	lff->Draw();

        TLatex *lff2= new TLatex(0.48,0.81,"#eta=2.0");;
		lff2->SetNDC();
	lff2->SetTextFont(42);
	lff2->SetTextSize(0.04);
	lff2->SetLineWidth(2);
	lff2->Draw();

        TLatex *lff3= new TLatex(0.48,0.71,"#eta=2.5");;
		lff3->SetNDC();
	lff3->SetTextFont(42);
	lff3->SetTextSize(0.04);
	lff3->SetLineWidth(2);
	lff3->Draw();
	
        TArc *a20=new TArc(51.0, 51.0, 17.8);  // eta = 2.0
        a20->SetLineColor(kBlack);
        a20->SetFillStyle(0);
        a20->Draw();

        TArc *a25=new TArc(51.0, 51.0, 30.);   // eta = 2.5
        a25->SetLineColor(kBlack);
        a25->SetFillStyle(0);
        a25->Draw();
 
}



void EnResBarrel(double totLumi, double eta, double temp, double shap)
{

  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;
  TGraph *gr3 = new TGraph();
  int ngr3 = 0;
  TGraph *gr4 = new TGraph();
  int ngr4 = 0;

  
 
  double instLumi = 5.0e+34;
  // double totLumi = 1000.;
  // double totLumi = 3000.;


    if(totLumi>300)instLumi = 5.0e+34; 
    DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);


  // Initial parameters for ECAL resolution
    double S;
    double Nadc;
    double adc2GeV;
    double C;
    if(eta<1.497){
      S = 0.028;           // CMS note 2006/148 (EB test beam)
      Nadc = 1.1; 
      adc2GeV = 0.039;
      //  C = 0.003; // original value by Sasha
      C = 0.01;  //0.01 EGM-11-001
    }else{
      S = 0.052;          //  CMS DN 2009/002
      Nadc = 2.0;
      adc2GeV = 0.069;
      //    C = 0.0038; // original value by Sasha
      C = 0.02;  // EGM-11-001
    }
    std::cout<< "noise increase="<<d.noiseIncreaseADC<<std::endl;
    
    // adjust resolution parameters
    S /= sqrt(d.ampDropTotal);
    Nadc *= d.noiseIncreaseADC;
    adc2GeV /= d.ampDropTotal;
    double N = Nadc*adc2GeV*5.0;   // 3x3 noise in GeV 
    C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);
    
    
    // Printout 
    /*    cout << " ------------------------------------------------------- " << endl;
    cout << "   eta              = " << d.eta << endl;
    cout << "   instLumi         = " << d.instLumi << endl;
    cout << "   totLumi          = " << d.totLumi << endl;
    cout << "   muEM             = " << d.muEM << endl;
    cout << "   muHD             = " << d.muHD << endl;
    cout << "   ampTransparency  = " << d.ampDropTransparency << endl;
    cout << "   ampPhotoDetector = " << d.ampDropPhotoDetector << endl;
    cout << "   ampDropTotal     = " << d.ampDropTotal << endl;
    cout << "   noiseIncreaseADC = " << d.noiseIncreaseADC << endl;
    cout << "   constantTerm     = " << d.resolutitonConstantTerm << endl;
    cout << " ------------------------------------------------------- " << endl;
    */
    
    double ene=1.;
    // double factor = 1.0;
    // factor = sin(2.0*atan(exp(-1.0*eta)));
    //  ene=ene/factor;  // ET

    double DeltaE=2.;
    for(int i=0; i<100; i++){

      ene=ene+DeltaE;

      //   cout << "Energy="<<ene<< endl;


      double res =  sqrt(S*S/ene + N*N/ene/ene + C*C);
    


      gr1->SetPoint(ngr1,ene , sqrt(S*S/ene) );
      ngr1++;
      gr2->SetPoint(ngr2,ene, sqrt(N*N/ene/ene) );
      ngr2++;
      gr3->SetPoint(ngr3,ene , sqrt(C*C) );
      ngr3++;
      gr4->SetPoint(ngr4,ene, res );
      ngr4++;
      
    }

    gr4->SetMinimum(0.);
    gr4->SetMaximum(0.05);
    gr4->SetLineColor(1);
    gr1->SetLineColor(4);
    gr2->SetLineColor(3);
    gr3->SetLineColor(2);
    
    gr1->SetLineWidth(3);
    gr2->SetLineWidth(3);
    gr3->SetLineWidth(3);
    gr4->SetLineWidth(3);
    
    
    gr4->Draw("AL");
    gr1->Draw("L");
    gr2->Draw("L");
    gr3->Draw("L");
    
    TLegend *leg = new TLegend( 0.5, 0.7, 0.7, 0.9, NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(1);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextSize(0.04);
    leg->AddEntry(gr1,"Stoch","l");
    leg->AddEntry(gr2,"Noise","l");
    leg->AddEntry(gr3,"Const","l");
    leg->AddEntry(gr4,"Total","l");
    leg->Draw();
    
     gr4->GetXaxis()->SetTitle("Energy (GeV)");
     gr4->GetYaxis()->SetTitle("#sigma (E)/E");

  
}
void EnResBarrelVsEta(double totLumi, double temp, double shap)
{

  TGraph *gr1 = new TGraph();
  int ngr1 = 0;
  TGraph *gr2 = new TGraph();
  int ngr2 = 0;

  
 
  double instLumi = 1.0e+34;
  // double totLumi = 1000.;
  // double totLumi = 3000.;


    if(totLumi>300)instLumi = 5.0e+34;

    int nstep=100;
    double delta_eta=1.497/nstep;
    double eta=0-delta_eta;

    for (int i=0; i<nstep; i++){
      eta=eta+delta_eta; 


      double ene=125/2.;
      double factor = 1.0;
      
            factor = sin(2.0*atan(exp(-1.0*eta)));
        ene=ene/factor;  // ET
      
      
         cout << "Energy="<<ene<< endl;
      


 
      DegradationAtEta d = CalculateDegradation(eta, totLumi, instLumi);


      // Initial parameters for ECAL resolution
      double S;
      double Nadc;
      double adc2GeV;
      double C;
      if(eta<1.497){
	S = 0.028;           // CMS note 2006/148 (EB test beam)
	Nadc = 1.1; 
	adc2GeV = 0.039;
	//  C = 0.003; // original value by Sasha
	C = 0.01; // theory //0.01 EGM-11-001
      }else{
	S = 0.052;          //  CMS DN 2009/002
	Nadc = 2.0;
	adc2GeV = 0.069;
	//    C = 0.0038; // original value by Sasha
	C = 0.02;  // EGM-11-001
      }
      std::cout<< "noise increase="<<d.noiseIncreaseADC<<std::endl;
      
      // adjust resolution parameters
      S /= sqrt(d.ampDropTotal);
      Nadc *= d.noiseIncreaseADC;
      adc2GeV /= d.ampDropTotal;
      double N = Nadc*adc2GeV*3.0;   // 3x3 or 5x5 noise in GeV 
      C = sqrt(C*C + d.resolutitonConstantTerm*d.resolutitonConstantTerm);
      
      
    
      
      double res =  sqrt(S*S/ene + N*N/ene/ene + C*C);
      
      
      
      gr1->SetPoint(ngr1,eta, res );
      ngr1++;

      // resolution with upgrade 

      
      DegradationAtEta d2 = CalculateDegradation2(eta, totLumi, instLumi, 18., 43.);

      if(eta<1.497){
	S = 0.028;           // CMS note 2006/148 (EB test beam)
	Nadc = 1.1; 
	adc2GeV = 0.039;
	//  C = 0.003; // original value by Sasha
	C = 0.01; // theory //0.01 EGM-11-001
      }else{
	S = 0.052;          //  CMS DN 2009/002
	Nadc = 2.0;
	adc2GeV = 0.069;
	//    C = 0.0038; // original value by Sasha
	C = 0.02;  // EGM-11-001
      }
      std::cout<< "noise increase="<<d2.noiseIncreaseADC<<std::endl;
      
      // adjust resolution parameters
      S /= sqrt(d2.ampDropTotal);
      Nadc *= d2.noiseIncreaseADC;
      adc2GeV /= d2.ampDropTotal;
      N = Nadc*adc2GeV*3.0;   // 3x3 or 5x5 noise in GeV 
      C = sqrt(C*C + d2.resolutitonConstantTerm*d2.resolutitonConstantTerm);
      
      
    
      
      res =  sqrt(S*S/ene + N*N/ene/ene + C*C);

      gr2->SetPoint(ngr2,eta, res );
      ngr2++;

      
    }

    gr1->SetMinimum(0.);
    gr1->SetMaximum(0.05);
    gr1->SetLineColor(1);
    gr2->SetLineColor(4);
    
    gr1->SetLineWidth(3);
    gr2->SetLineWidth(3);
    
    
    gr1->Draw("AL");
    gr2->Draw("L");

    
    TLegend *leg = new TLegend( 0.5, 0.7, 0.7, 0.9, NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(0);
   leg->SetLineStyle(1);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->SetTextSize(0.04);
    leg->AddEntry(gr1,"ageing, no upgrade","l");
    leg->AddEntry(gr2,"upgrade","l");

    leg->Draw();
    
     gr1->GetXaxis()->SetTitle("Energy (GeV)");
     gr1->GetYaxis()->SetTitle("#sigma (E)/E");

  
}
