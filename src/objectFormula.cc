#include "objectFormula.h"

void objectFormula::setFormula(){
  
  if(configurationName == ""){

    muonScaleUnc      = new TFormula("muonScaleUnc","0.");    
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.");    
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.");

    jetScaleUnc        = new TFormula("jetScaleUnc","0.");
    jetResolutionUnc   = new TFormula("jetResolutionUnc","0.");

  }

  // Phase I tracking up to 2p5

  else if(configurationName == "PhaseI_Tracker2p5"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker2p5"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc",  "0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }

  // Phase I 4p0 
  else if(configurationName == "PhaseI_Tracker4p0"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker4p0"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

 
    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }

  else if(configurationName == "PhaseI_Tracker2p5_aged"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker2p5_aged"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");


    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }

  else if(configurationName == "PhaseI_Tracker4p0_aged"){

    cout<<"analysis with systematics : setup formula for PhaseI_Tracker4p0_aged"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");


  }

  else if(configurationName == "PhaseII_Tracker2p5_shashlik"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker2p5_shashlik"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }

  else if(configurationName == "PhaseII_Tracker4p0_shashlik"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker4p0_shashlik"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");
    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }

  else if(configurationName == "PhaseII_Tracker2p5_HGC"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker2p5_HGC"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");
  }

  else if(configurationName == "PhaseII_Tracker4p0_HGC"){

    cout<<"analysis with systematics : setup formula for PhaseII_Tracker4p0_HGC"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }
  else {

    cout<<"analysis with systematics : setup formula for dummy configuration"<<endl;

    muonScaleUnc      = new TFormula("muonScaleUnc","0.01");
    muonResolutionUnc = new TFormula("muonResolutionUnc","0.01");

    electronScaleUnc      = new TFormula("electronScaleUnc","0.02");
    electronResolutionUnc = new TFormula("electronResolutionUnc","0.02");

    jetScaleUnc      = new TFormula("jetScaleUnc","0.05");
    jetResolutionUnc = new TFormula("jetResolutionUnc","0.05");

  }  

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
