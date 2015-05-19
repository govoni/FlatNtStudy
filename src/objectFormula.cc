#include "objectFormula.h"

void objectFormula::setFormula(){

  TString muonScaleUncString ;
  TString muonResolutionUncString ;

  TString electronScaleUncString ;
  TString electronResolutionUncString ;

  TString jetScaleUncString ;
  TString jetResolutionUncString ;
  

  if(configurationName == ""){

    muonScaleUncString      = Form("0.0");
    muonResolutionUncString = Form("0.0");

    electronScaleUncString      = Form("0.0");
    electronResolutionUncString = Form("0.0");

    jetScaleUncString      = Form("0.0");
    jetResolutionUncString = Form("0.0");

  }

  // Phase I tracking up to 2p5

  else if(configurationName == "PhaseI_Tracker2p5"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker2p5"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("(abs(eta) < 2.5)*(pt < 30 )*(0.03) +\
                                   (abs(eta) < 2.5)*(pt >= 30 && pt < 60)*(0.02) + \
                                   (abs(eta) < 2.5)*(pt >= 60 && pt < 100)*(0.015) +\
                                   (abs(eta) < 2.5)*(pt >= 100 )*(0.01) +\
                                   (abs(eta) >= 2.5)*(pt < 30 )*(0.04) +\
                                   (abs(eta) >= 2.5)*(pt >= 30 && pt < 60)*(0.025) +\
                                   (abs(eta) >= 2.5)*(pt >= 60 && pt < 100)*(0.02) +\
                                   (abs(eta) >= 2.5)*(pt >= 100 )*(0.015)");

    jetResolutionUncString = Form("0.05");

  }

  // Phase I 4p0 
  else if(configurationName == "PhaseI_Tracker4p0"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker4p0"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("(abs(eta) < 2.5)*(pt < 30 )*(0.03) +\
                                   (abs(eta) < 2.5)*(pt >= 30 && pt < 60)*(0.02) + \
                                   (abs(eta) < 2.5)*(pt >= 60 && pt < 100)*(0.015) +\
                                   (abs(eta) < 2.5)*(pt >= 100 )*(0.01) +\
                                   (abs(eta) >= 2.5)*(pt < 30 )*(0.03) +\
                                   (abs(eta) >= 2.5)*(pt >= 30 && pt < 60)*(0.02) +\
                                   (abs(eta) >= 2.5)*(pt >= 60 && pt < 100)*(0.015) +\
                                   (abs(eta) >= 2.5)*(pt >= 100 )*(0.01)");

    jetResolutionUncString = Form("0.05");

  }

  else if(configurationName == "PhaseI_Tracker2p5_aged"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker2p5_aged"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");


    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");

  }

  else if(configurationName == "PhaseI_Tracker4p0_aged"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker4p0_aged"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");


  }

  else if(configurationName == "PhaseII_Tracker2p5_shashlik"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker2p5_shashlik"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");

  }

  else if(configurationName == "PhaseII_Tracker4p0_shashlik"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker4p0_shashlik"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronResolutionUncString = Form("0.02");
    electronScaleUncString      = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");

  }

  else if(configurationName == "PhaseII_Tracker2p5_HGC"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker2p5_HGC"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");
  }

  else if(configurationName == "PhaseII_Tracker4p0_HGC"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker4p0_HGC"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");

  }
  else {

    cout<<"analysis with systematics : setup formula for dummy configuration"<<endl;

    muonScaleUncString      = Form("0.01");
    muonResolutionUncString = Form("0.01");

    electronScaleUncString      = Form("0.02");
    electronResolutionUncString = Form("0.02");

    jetScaleUncString      = Form("0.05");
    jetResolutionUncString = Form("0.05");

  }  

  muonScaleUncString.ReplaceAll("eta","x");
  muonScaleUncString.ReplaceAll("pt","y");
  muonScaleUncString.ReplaceAll("energy","z");
  muonScaleUncString.ReplaceAll("phi","t");

  muonResolutionUncString.ReplaceAll("eta","x");
  muonResolutionUncString.ReplaceAll("pt","y");
  muonResolutionUncString.ReplaceAll("energy","z");
  muonResolutionUncString.ReplaceAll("phi","t");

  electronScaleUncString.ReplaceAll("eta","x");
  electronScaleUncString.ReplaceAll("pt","y");
  electronScaleUncString.ReplaceAll("energy","z");
  electronScaleUncString.ReplaceAll("phi","t");

  electronResolutionUncString.ReplaceAll("eta","x");
  electronResolutionUncString.ReplaceAll("pt","y");
  electronResolutionUncString.ReplaceAll("energy","z");
  electronResolutionUncString.ReplaceAll("phi","t");

  jetScaleUncString.ReplaceAll("eta","x");
  jetScaleUncString.ReplaceAll("pt","y");
  jetScaleUncString.ReplaceAll("energy","z");
  jetScaleUncString.ReplaceAll("phi","t");

  jetResolutionUncString.ReplaceAll("eta","x");
  jetResolutionUncString.ReplaceAll("pt","y");
  jetResolutionUncString.ReplaceAll("energy","z");
  jetResolutionUncString.ReplaceAll("phi","t");

  muonScaleUnc      = new TFormula("muonScaleUnc",muonScaleUncString.Data());    
  muonResolutionUnc = new TFormula("muonResolutionUnc",muonResolutionUncString.Data());

  electronScaleUnc      = new TFormula("electronScaleUnc",electronScaleUncString.Data());    
  electronResolutionUnc = new TFormula("electronResolutionUnc",electronResolutionUncString.Data());

  jetScaleUnc        = new TFormula("jetScaleUnc",jetScaleUncString.Data());
  jetResolutionUnc   = new TFormula("jetResolutionUnc",jetResolutionUncString.Data());

  muonScaleUnc->Compile();    
  muonResolutionUnc->Compile();
  electronScaleUnc->Compile();    
  electronResolutionUnc->Compile();
  jetScaleUnc->Compile();   
  jetResolutionUnc->Compile();

  return;
}
 
float objectFormula::evaluateMuonScaleUnc(float pt, float eta){
  return muonScaleUnc->Eval(pt,eta);
}

float objectFormula::evaluateMuonResolutionUnc(float pt, float eta, float energy){
  return muonResolutionUnc->Eval(pt,eta,energy);
}

float objectFormula::evaluateElectronScaleUnc(float pt, float eta){
  return electronScaleUnc->Eval(pt,eta);
}

float objectFormula::evaluateElectronResolutionUnc(float pt, float eta, float energy){
  return electronResolutionUnc->Eval(pt,eta,energy);
}

float objectFormula::evaluateJetScaleUnc(float pt, float eta){
  return jetScaleUnc->Eval(pt,eta);
}

float objectFormula::evaluateJetResolutionUnc(float pt, float eta, float energy){
  return jetResolutionUnc->Eval(pt,eta);
}
