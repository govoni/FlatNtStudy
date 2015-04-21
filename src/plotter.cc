#include "plotter.h"

using namespace std ;

// layer class  --------------------

vector<TH1F*> layer::getHistos(){

  vector<TH1F*> vectHisto ;
  unordered_map<string,TH1F*>::iterator itHisto = m_histos.begin();
  for( ; itHisto != m_histos.end(); itHisto++){
    vectHisto.push_back(itHisto->second);
  }
  return vectHisto;
 
}

vector<TH2F*> layer::getHistos2D(){

  vector<TH2F*> vectHisto2D ;
  unordered_map<string,TH2F*>::iterator itHisto = m_2Dhistos.begin();
  for( ; itHisto != m_2Dhistos.end(); itHisto++){
    vectHisto2D.push_back(itHisto->second);

  }
  return vectHisto2D;
}

// sample class  ---------------

vector<layer> sample::getLayer(){
  vector<layer> vectLayer ;
  unordered_map<string, layer>::iterator itLay = m_sampleContent.begin();
  for( ; itLay != m_sampleContent.end(); itLay++){
    vectLayer.push_back(itLay->second);
  }
  return vectLayer;
}

void mergeSample (sample & newSample, sample sample1, sample sample2){
    
  newSample.m_sampleName = sample1.m_sampleName ;
  newSample.m_isSignal   = sample1.m_isSignal;
  newSample.m_XS         = sample1.m_XS + sample2.m_XS;
  newSample.m_color      = sample1.m_color;
  newSample.m_readyForPlotting = sample1.m_readyForPlotting;
  newSample.m_totInitialEvents = sample1.m_totInitialEvents + sample2.m_totInitialEvents;
  newSample.m_weight           = sample1.m_weight*sample2.m_weight;
  newSample.m_layersSequence   = sample1.m_layersSequence;

  // clone the sample content of the first
  for(unordered_map<string, layer>::iterator itMap = sample1.m_sampleContent.begin();
      itMap != sample1.m_sampleContent.end();
      itMap++){
    newSample.m_sampleContent[itMap->first] = itMap->second;
  }

  // loop on this sample content
  for(unordered_map<string, layer>::iterator itMap  = newSample.m_sampleContent.begin(); 
      itMap != newSample.m_sampleContent.end();
      itMap++){

    unordered_map<string, layer>::iterator itMap2 = sample2.m_sampleContent.begin();
    for( ; itMap2 != sample2.m_sampleContent.end(); itMap2++){
      if(itMap->second.m_layerName != itMap2->second.m_layerName) continue;
      else break;
    }

    if(itMap2 == sample2.m_sampleContent.end()) continue ;
    
    // central histo
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos.begin();
	itHisto1 != itMap->second.m_histos.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos.begin();
      for( ; itHisto2 != itMap2->second.m_histos.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      if(itHisto2 == itMap2->second.m_histos.end()) continue;
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }

    // lept scale up

    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_muScaleUp.begin();
	itHisto1 != itMap->second.m_histos_muScaleUp.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_muScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_histos_muScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // mut scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_muScaleDown.begin();
	itHisto1 != itMap->second.m_histos_muScaleDown.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_muScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_histos_muScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }

    // mut scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_muRes.begin();
	itHisto1 != itMap->second.m_histos_muRes.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_muRes.begin();
      for( ; itHisto2 != itMap2->second.m_histos_muRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }



    // lept scale up

    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_elScaleUp.begin();
	itHisto1 != itMap->second.m_histos_elScaleUp.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_elScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_histos_elScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
	itHisto1->second->SetName(nameTemp);
    }
    
    // elt scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_elScaleDown.begin();
	itHisto1 != itMap->second.m_histos_elScaleDown.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_elScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_histos_elScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }

    // elt scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_elRes.begin();
	itHisto1 != itMap->second.m_histos_elRes.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_elRes.begin();
      for( ; itHisto2 != itMap2->second.m_histos_elRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }

    
    // jett scale up
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_jetScaleUp.begin();
	itHisto1 != itMap->second.m_histos_jetScaleUp.end();
	itHisto1++){

      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_jetScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_histos_jetScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // jett scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_jetScaleDown.begin();
	itHisto1 != itMap->second.m_histos_jetScaleDown.end();
	itHisto1++){
      
      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_jetScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_histos_jetScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }

    // jett scale dw
    for(unordered_map<string,TH1F*>::iterator itHisto1 = itMap->second.m_histos_jetRes.begin();
	itHisto1 != itMap->second.m_histos_jetRes.end();
	itHisto1++){
      
      unordered_map<string, TH1F*>::iterator itHisto2 = itMap2->second.m_histos_jetRes.begin();
      for( ; itHisto2 != itMap2->second.m_histos_jetRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	  else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    
    /// same thing for TH2

    // central histo
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos.begin();
	itHisto1 != itMap->second.m_2Dhistos.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
      }
    
    // mut scale up
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_muScaleUp.begin();
	itHisto1 != itMap->second.m_2Dhistos_muScaleUp.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_muScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_muScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
      }
    
    // mut scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_muScaleDown.begin();
	itHisto1 != itMap->second.m_2Dhistos_muScaleDown.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_muScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_muScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // mut scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_muRes.begin();
	itHisto1 != itMap->second.m_2Dhistos_muRes.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_muRes.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_muRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    


    // elt scale up
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_elScaleUp.begin();
	itHisto1 != itMap->second.m_2Dhistos_elScaleUp.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_elScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_elScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
      }
    
    // elt scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_elScaleDown.begin();
	itHisto1 != itMap->second.m_2Dhistos_elScaleDown.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_elScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_elScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // elt scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_elRes.begin();
	itHisto1 != itMap->second.m_2Dhistos_elRes.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_elRes.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_elRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }

      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    


    // jett scale up
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_jetScaleUp.begin();
	itHisto1 != itMap->second.m_2Dhistos_jetScaleUp.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_jetScaleUp.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_jetScaleUp.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	  else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // jett scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_jetScaleDown.begin();
	itHisto1 != itMap->second.m_2Dhistos_jetScaleDown.end();
	itHisto1++){
      
      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_jetScaleDown.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_jetScaleDown.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
    // jett scale dw
    for(unordered_map<string,TH2F*>::iterator itHisto1 = itMap->second.m_2Dhistos_jetRes.begin();
	itHisto1 != itMap->second.m_2Dhistos_jetRes.end();
	itHisto1++){

      unordered_map<string, TH2F*>::iterator itHisto2 = itMap2->second.m_2Dhistos_jetRes.begin();
      for( ; itHisto2 != itMap2->second.m_2Dhistos_jetRes.end(); itHisto2++){
	if(itHisto2->first != itHisto1->first) continue;
	else break;
      }
      
      itHisto1->second->Add(itHisto2->second);
      TString nameTemp = Form("%s",itHisto1->second->GetName());
      nameTemp.ReplaceAll("_0","");
      itHisto1->second->SetName(nameTemp);
    }
    
  }    
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
plotter::plotter (float  lumi, 
		  string folderName,
		  bool   includeSystematics,
		  string phase,
		  int    nPU) :
  m_folderName (folderName) ,
  m_lumi (lumi) ,
  m_includeSystematics(includeSystematics),
  m_phase(phase),
  m_nPU(nPU),
  m_canvas ("can", "can", 500, 500){
  setRootAspect () ;
}



// add an empty structure to collect all the plots relative to a sample
void plotter::addSample (string sampleName, float XS, int totInitialEvents, int isSignal, int color){

  m_samples[sampleName].push_back(sample(sampleName, XS, m_lumi, totInitialEvents, isSignal, color));
  if( find(m_samplesSequence.begin(),m_samplesSequence.end(),sampleName) == m_samplesSequence.end()){
    m_samplesSequence.push_back (sampleName) ;
  }
}


// add a layer of plots (corresponding to a given cut) to a sample
void plotter::addLayerToSample (string sampleName, string layerName){

  vector<sample>::iterator itSample = m_samples[sampleName].begin();
  for( ; itSample != m_samples[sampleName].end(); itSample++){
    itSample->m_sampleContent[layerName] = layer (layerName) ;
    itSample->m_layersSequence.push_back (layerName) ;
  }

}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add a plot to a layer of a given sample
void plotter::addPlotToLayer (string sampleName,   // name of the sample
			      string layerName,   // name of the layer
                              string plotName,    // name of the plot
			      int nBins, float xMin, float xMax, string labelName, // histo definition
			      bool sumW2){

  if (labelName == "") 
    labelName = plotName ;

  vector<sample>::iterator itSample = m_samples[sampleName].begin();
  int iSample = 0;
  for( ; itSample != m_samples[sampleName].end() ; itSample++){

    string h_name = sampleName + "_" + layerName + "_" + plotName + "_" + to_string(iSample) ;  
    iSample++;

    TString labelTemp = Form("%s",labelName.c_str());
    labelTemp.ReplaceAll("/_"," ");
    labelTemp.ReplaceAll("/"," ");
    labelName = labelTemp;

    TH1F * dummy = new TH1F (h_name.c_str (), h_name.c_str (), nBins, xMin, xMax) ;
    dummy->GetXaxis()->SetTitle(labelName.c_str()); 
    if(sumW2)
      dummy->Sumw2 () ;

    itSample->m_sampleContent[layerName].m_histos[plotName] = dummy ;

    if(m_includeSystematics){      

      itSample->m_sampleContent[layerName].m_histos_muScaleUp[plotName]   = (TH1F*) dummy->Clone((plotName+"_muScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_muScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"_muScaleDown").c_str()) ;
      
      itSample->m_sampleContent[layerName].m_histos_elScaleUp[plotName]   = (TH1F*) dummy->Clone((plotName+"_elScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_elScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"elScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_jetScaleUp[plotName]   = (TH1F*) dummy->Clone((plotName+"_jetScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_jetScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"_jetScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_muRes[plotName] = (TH1F*) dummy->Clone((plotName+"_muRes").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_elRes[plotName] = (TH1F*) dummy->Clone((plotName+"_elRes").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_jetRes[plotName] = (TH1F*) dummy->Clone((plotName+"_jetRes").c_str()) ;
    }
  }
}



// add a plot to a layer of a given sample
void plotter::addPlotToLayer (string sampleName,   // name of the sample
			      string layerName,   // name of the layer
                              string plotName,    // name of the plot
			      int nBins, vector<float> binningX, string labelName, // histo definition
			      bool sumW2){

  if (labelName == "") 
    labelName = plotName ;

  vector<sample>::iterator itSample = m_samples[sampleName].begin();
  int iSample = 0;
  for( ; itSample != m_samples[sampleName].end() ; itSample++){
    string h_name = sampleName + "_" + layerName + "_" + plotName + "_" + to_string(iSample) ;  
    iSample++;

    TH1F * dummy = new TH1F (h_name.c_str (), h_name.c_str (), nBins, &binningX[0]) ;
    TString labelTemp = Form("%s",labelName.c_str());
    labelTemp.ReplaceAll("/_"," ");
    labelTemp.ReplaceAll("/"," ");
    labelName = labelTemp;
    dummy->GetXaxis()->SetTitle(labelName.c_str()); 
    if(sumW2)
      dummy->Sumw2 () ;

    itSample->m_sampleContent[layerName].m_histos[plotName] = dummy ;

    if(m_includeSystematics){

      itSample->m_sampleContent[layerName].m_histos_muScaleUp[plotName] = (TH1F*) dummy->Clone((plotName+"_muScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_muScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"_muScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_elScaleUp[plotName] = (TH1F*) dummy->Clone((plotName+"_elScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_elScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"_elScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_jetScaleUp[plotName] = (TH1F*) dummy->Clone((plotName+"_jetScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_jetScaleDown[plotName] = (TH1F*) dummy->Clone((plotName+"_jetScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_muRes[plotName] = (TH1F*) dummy->Clone((plotName+"_muRes").c_str()) ;
      itSample->m_sampleContent[layerName].m_histos_elRes[plotName] = (TH1F*) dummy->Clone((plotName+"_elRes").c_str()) ;

      itSample->m_sampleContent[layerName].m_histos_jetRes[plotName] = (TH1F*) dummy->Clone((plotName+"_jetRes").c_str()) ;
    }
  }
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// add a plot to a layer of a given sample
void plotter::add2DPlotToLayer (string sampleName, 
				string layerName, 
				string plotName, 
				int nBinsX, float xMinX, float xMaxX,
				int nBinsY, float xMinY, float xMaxY,
				string labelNameX, string labelNameY,
                                bool sumW2){


  

  if (labelNameX == "") labelNameX = plotName + "_x" ;
  if (labelNameY == "") labelNameY = plotName + "_y" ;

  vector<sample>::iterator itSample = m_samples[sampleName].begin();
  int iSample = 0;

  for( ; itSample != m_samples[sampleName].end() ; itSample++){
    string h_name = sampleName + "_" + layerName + "_" + plotName + "_" + to_string(iSample);
    iSample++;
    TH2F * dummy = new TH2F (h_name.c_str (), h_name.c_str (), 
                           nBinsX, xMinX, xMaxX,
                           nBinsY, xMinY, xMaxY) ;

    TString labelTempX = Form("%s",labelNameX.c_str());
    labelTempX.ReplaceAll("/_"," ");
    labelTempX.ReplaceAll("/"," ");
    labelNameX = labelTempX;
    TString labelTempY = Form("%s",labelNameY.c_str());
    labelTempY.ReplaceAll("/_"," ");
    labelTempY.ReplaceAll("/"," ");
    labelNameY = labelTempY;

    dummy->GetXaxis ()->SetTitle (labelNameX.c_str ()) ; 
    dummy->GetYaxis ()->SetTitle (labelNameY.c_str ()) ; 

    if(sumW2)
      dummy->Sumw2 () ;
    
    itSample->m_sampleContent[layerName].m_2Dhistos[plotName] = dummy ;

    if(m_includeSystematics){

      itSample->m_sampleContent[layerName].m_2Dhistos_muScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_muScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_muScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_muScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_elScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_elScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_elScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_elScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_jetScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_jetScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_jetScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_jetScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_muRes[plotName] = (TH2F*) dummy->Clone((plotName+"_muRes").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_elRes[plotName] = (TH2F*) dummy->Clone((plotName+"_elRes").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_jetRes[plotName] = (TH2F*) dummy->Clone((plotName+"_jetRes").c_str()) ;
      
    }
  }
}


// add a plot to a layer of a given sample
void plotter::add2DPlotToLayer (string sampleName, 
				string layerName, 
				string plotName, 
				int nBinsX, vector<float> binningX,
				int nBinsY, vector<float> binningY,
				string labelNameX, string labelNameY,
                                bool sumW2){


  

  if (labelNameX == "") labelNameX = plotName + "_x" ;
  if (labelNameY == "") labelNameY = plotName + "_y" ;

  vector<sample>::iterator itSample = m_samples[sampleName].begin();
  int iSample = 0;

  for( ; itSample != m_samples[sampleName].end() ; itSample++){
    string h_name = sampleName + "_" + layerName + "_" + plotName + "_" + to_string(iSample);
    iSample++;
    TH2F * dummy = new TH2F (h_name.c_str (), h_name.c_str (), 
                           nBinsX, &binningX[0],
                           nBinsY, &binningY[0]) ;

    TString labelTempX = Form("%s",labelNameX.c_str());
    labelTempX.ReplaceAll("/_"," ");
    labelTempX.ReplaceAll("/"," ");
    labelNameX = labelTempX;
    TString labelTempY = Form("%s",labelNameY.c_str());
    labelTempY.ReplaceAll("/_"," ");
    labelTempY.ReplaceAll("/"," ");
    labelNameY = labelTempY;

    dummy->GetXaxis ()->SetTitle (labelNameX.c_str ()) ; 
    dummy->GetYaxis ()->SetTitle (labelNameY.c_str ()) ; 

    if(sumW2)
      dummy->Sumw2 () ;
    
    itSample->m_sampleContent[layerName].m_2Dhistos[plotName] = dummy ;

    if(m_includeSystematics){

      itSample->m_sampleContent[layerName].m_2Dhistos_muScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_muScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_muScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_muScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_elScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_elScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_elScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_elScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_jetScaleUp[plotName] = (TH2F*) dummy->Clone((plotName+"_jetScaleUp").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_jetScaleDown[plotName] = (TH2F*) dummy->Clone((plotName+"_jetScaleDown").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_muRes[plotName] = (TH2F*) dummy->Clone((plotName+"_muRes").c_str()) ;
      itSample->m_sampleContent[layerName].m_2Dhistos_elRes[plotName] = (TH2F*) dummy->Clone((plotName+"_elRes").c_str()) ;

      itSample->m_sampleContent[layerName].m_2Dhistos_jetRes[plotName] = (TH2F*) dummy->Clone((plotName+"_jetRes").c_str()) ;
      
    }
  }
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


// duplicate a layer into a new one with a different name, within a single sample
void plotter::copyLayerInSample (string sampleName, string target, string origin){

  addLayerToSample (sampleName, target) ; // add a layer to the new target sample

  // loop over histos in the layer "origin" of sample "sampleName"
  int iSample = 0;

  for(vector<sample>::iterator itSample = m_samples[sampleName].begin(); itSample != m_samples[sampleName].end(); itSample++){    

    for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos.begin () ;
	 itHisto != itSample->m_sampleContent[origin].m_histos.end () ;
	 ++itHisto){
    
      string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
      itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
      itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
      itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
    }   

    for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos.begin () ;
	 itHisto != itSample->m_sampleContent[origin].m_2Dhistos.end () ;
	 ++itHisto){
    
      string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
      itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
      itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
      itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
    }


    // if systematics are included

    if(m_includeSystematics){

      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_muScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_muScaleUp.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   
    
      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_muScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_muScaleUp.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }

      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_muScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_muScaleDown.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   

      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_muScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_muScaleDown.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }



      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_elScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_elScaleUp.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   
    
      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_elScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_elScaleUp.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }

      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_elScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_elScaleDown.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   

      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_elScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_elScaleDown.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }
  
  
      // loop over histos in the layer "origin" of sample "sampleName"
      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_jetScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_jetScaleUp.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   
    
      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_jetScaleUp.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_jetScaleUp.end () ;
	   ++itHisto){
	
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }

      // loop over histos in the layer "origin" of sample "sampleName"
      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_jetScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_jetScaleDown.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   

      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_jetScaleDown.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_jetScaleDown.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }

      // resolution
      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_elRes.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_elRes.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   
    
      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_elRes.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_elRes.end () ;
	   ++itHisto){
    
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }
      
      for (unordered_map<string, TH1F *>::iterator itHisto = itSample->m_sampleContent[origin].m_histos_jetRes.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_histos_jetRes.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first] = (TH1F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_histos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }   
    
      for (unordered_map<string, TH2F *>::iterator itHisto = itSample->m_sampleContent[origin].m_2Dhistos_jetRes.begin () ;
	   itHisto != itSample->m_sampleContent[origin].m_2Dhistos_jetRes.end () ;
	   ++itHisto){
      
	string h_name = sampleName + "_" + target + "_" + itHisto->first + "_" + to_string(iSample) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first] = (TH2F *) itHisto->second->Clone (h_name.c_str ()) ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->Reset () ;
	itSample->m_sampleContent[target].m_2Dhistos[itHisto->first]->SetTitle (h_name.c_str ()) ;
      }
    }
    iSample ++;
  }
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// duplicate a sample structure
void plotter::copySampleStructure (string target, string origin, float newXS, int newTotInitialEvents, int isSignal, int newColor){

  addSample (target, newXS, newTotInitialEvents, isSignal, newColor) ; // add a new sample

  vector<sample>::iterator itSampleOrig   = m_samples[origin].begin();
  vector<sample>::iterator itSampleTarget = m_samples[target].begin();

  string firstLayer = itSampleOrig->m_layersSequence.at (0) ;
  addLayerToSample (target, firstLayer) ; // add the first layer
 
  for( ; itSampleOrig != m_samples[origin].end() and itSampleTarget != m_samples[target].end();
       itSampleOrig++ , itSampleTarget++){

    if (itSampleOrig->m_sampleContent.size () == 0) return ;

    for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos.begin () ;
	 iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos.end () ;
	 ++iHisto){
    
      string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
      itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
      itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
    }
  
    for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos.begin () ;
	 iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos.end () ;
	 ++iHisto){
    
      string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
      itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
      itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
    }

    if(m_includeSystematics){

      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_muScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_muScaleUp.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muScaleUp.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }
      
      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_muScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_muScaleDown.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muScaleDown.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }



      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_elScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_elScaleUp.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elScaleUp.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }
      
      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_elScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_elScaleDown.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elScaleDown.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }

      //////
      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_jetScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_jetScaleUp.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetScaleUp.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetScaleUp.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }

      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_jetScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_jetScaleDown.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetScaleDown.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetScaleDown.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }

      ///
      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_muRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_muRes.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_muRes.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }


      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_elRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_elRes.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_elRes.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }

      for (unordered_map<string, TH1F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_histos_jetRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_histos_jetRes.end () ;
	   ++iHisto){
      
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first] = (TH1F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_histos[iHisto->first]->Reset () ;
      }
  
      for (unordered_map<string, TH2F *>::iterator iHisto = itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetRes.begin () ;
	   iHisto != itSampleOrig->m_sampleContent[firstLayer].m_2Dhistos_jetRes.end () ;
	   ++iHisto){
    
	string h_name = target + "_" + firstLayer + "_" + iHisto->first ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first] = (TH2F *) iHisto->second->Clone (h_name.c_str ()) ;
	itSampleTarget->m_sampleContent[firstLayer].m_2Dhistos[iHisto->first]->Reset () ;
      }
    }

  }
  
  for (unsigned int iLayer = 1 ; iLayer < itSampleOrig->m_layersSequence.size () ; ++iLayer){
    copyLayerInSample (target, itSampleOrig->m_layersSequence.at (iLayer), firstLayer) ;
  }

  return ;
    
}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::printStructure () {

  // loop over samples
  for (unordered_map<string, vector<sample> >::iterator itSample = m_samples.begin () ;
       itSample != m_samples.end () ;
       ++itSample){

    cout << itSample->first << "\t" ;
	
    for( vector<sample>::iterator itSubSample = itSample->second.begin();
	 itSubSample != itSample->second.end();
	 itSubSample++){

      cout << itSubSample->m_sampleName << "\t"
	   << itSubSample->m_XS << "\t"
	   << itSubSample->m_totInitialEvents << "\t"
	   << itSubSample->m_isSignal << "\t"
	   << itSubSample->m_color << "\n" ;

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
        
	string name = itSubSample->m_layersSequence.at (iLayer) ;
      
	cout << " + " << name << "\t" 
	     << itSubSample->m_sampleContent[name].m_layerName << "\n" ;
      // loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
             
	  cout << "    + " << iHisto->first << "\t"
	       << iHisto->second->GetName () << "\t"
	       << iHisto->second->Integral () << "\t"
	       << iHisto->second->GetNbinsX () << "\n" ;
	} // loop over histos	  
       
	// loop over 2D histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
             
	  cout << "    + " << iHisto->first << "\t"
	       << iHisto->second->GetName () << "\t"
	       << iHisto->second->Integral () << "\n" ;
	} // loop over histos	  
      } // loop over layers
    } // loop over sub samples
  } // loop over sample
}

void plotter::printEventNumber(string layerName, string histoName){

  // loop over samples
  cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
  cout<<"Event number for : Layer Name "<<layerName<<" Histo Name "<<histoName<<endl;

  for (unordered_map<string, vector<sample> >::iterator itSample = m_samples.begin () ;
       itSample != m_samples.end () ;
       ++itSample){
   
    cout << itSample->first << "\t" ;    
         
    // loop over layers
    for (unsigned int iLayer = 0 ; iLayer < itSample->second.at(0).m_layersSequence.size () ; ++iLayer){
      
      string name = itSample->second.at(0).m_layersSequence.at (iLayer) ;
      if(name != layerName) continue ;

      float integralVal1D = 0.;
      float integralVal2D = 0.;

      for(vector<sample>::iterator itSubSample = itSample->second.begin();
	  itSubSample != itSample->second.end();
	  itSubSample++){


	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
	  
	  if(histoName != iHisto->first) continue ;
	  integralVal1D += iHisto->second->Integral (0,iHisto->second->GetNbinsX()+1);
	}

	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  
	  if(histoName != iHisto->first) continue ;
	  integralVal2D += iHisto->second->Integral (0,iHisto->second->GetNbinsX()+1,0,iHisto->second->GetNbinsY()+1);
	}
      }
      
      if(integralVal1D!=0.)
	cout << setprecision(5) <<" events : integral "<< integralVal1D << "\n";
      if(integralVal2D!=0.)
	cout << setprecision(5) <<" events : integral "<< integralVal2D << "\n";

    }
  }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::fillHisto (string sampleName,int samplePosition, string layerName, string histoName, 
                         float value, float weight, string systematicName) {

  //PG NB assuming the histo is existing, not to slow down with cross-checks!
  if(systematicName !="" and not m_includeSystematics) return ;

  if(systematicName =="" or systematicName == "nominal" or systematicName == "central")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_muScaleUp[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_elScaleUp[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_muScaleDown[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_elScaleDown[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_jetScaleUp[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_jetScaleDown[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_muRes[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_elRes[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_histos_jetRes[histoName]->Fill (value, weight * m_samples[sampleName].at(samplePosition).m_weight) ;


  return ;

}

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::fill2DHisto (string sampleName, int samplePosition, string layerName, string histoName, 
                           float valueX,float valueY, float weight, string systematicName) {

  if(systematicName !="" and not m_includeSystematics) return ;


    
  if(systematicName =="" or systematicName == "nominal" or systematicName == "central")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_muScaleUp[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_muScaleDown[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_elScaleUp[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_elScaleDown[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetScaleUp")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_jetScaleUp[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetScaleDown")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_jetScaleDown[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "muRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_muRes[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "elRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_elRes[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;
  else if(systematicName == "jetRes")
    m_samples[sampleName].at(samplePosition).m_sampleContent[layerName].m_2Dhistos_jetRes[histoName]->Fill (valueX, valueY, weight * m_samples[sampleName].at(samplePosition).m_weight) ;


  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::prepareSampleForPlotting (string sampleName){

  for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
      itSample != m_samples[sampleName].end();
      itSample++){
    
    if (itSample->m_readyForPlotting) return ;
    
    // loop over layers
    for (unsigned int iLayer = 0 ; iLayer < itSample->m_layersSequence.size () ; ++iLayer) {

      string name = itSample->m_layersSequence.at (iLayer) ;

      // loop over histos
      for (unordered_map<string, TH1F *>::iterator iHisto = itSample->m_sampleContent[name].m_histos.begin () ;
           iHisto != itSample->m_sampleContent[name].m_histos.end () ;
           ++iHisto){

	addOverFlow (iHisto->second) ; 
	if (itSample->m_isSignal == 1){
	  iHisto->second->SetFillColor (0) ;
	  iHisto->second->SetLineColor (itSample->m_color) ;
	  iHisto->second->SetLineWidth (2) ;
	}
	else{  
	  iHisto->second->SetFillColor (itSample->m_color) ;
	  iHisto->second->SetLineColor (itSample->m_color) ;
	}
      } // loop over histos
      
      // loop over 2D histos
      for (unordered_map<string, TH2F *>::iterator iHisto = itSample->m_sampleContent[name].m_2Dhistos.begin () ;
           iHisto != itSample->m_sampleContent[name].m_2Dhistos.end () ;
           ++iHisto){

	if (itSample->m_isSignal == 1 ){
	  //              iHisto->second->SetFillColor (0) ;
	  iHisto->second->SetLineColor (itSample->m_color) ;
	  //              iHisto->second->SetLineWidth (2) ;
	}
	else{  
	  //              iHisto->second->SetFillColor (itSample->m_color) ;
	  iHisto->second->SetLineColor (itSample->m_color) ;
	}
      } // loop over 2D histos      
    } // loop over layers

    itSample->m_readyForPlotting = true ;
  }// loop over sub samples


}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
void plotter::cleanFromLateX (TString & Name) {

  Name.ReplaceAll("#","");
  Name.ReplaceAll("{","");
  Name.ReplaceAll("}","");
  Name.ReplaceAll("[","");
  Name.ReplaceAll("]","");
  Name.ReplaceAll("^","");
  Name.ReplaceAll("__","_");
  Name.ReplaceAll("..",".");
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::prepareCanvas (float xmin, float xmax, float ymin, float ymax, 
                             string xaxisTitle, string yaxisTitle, bool hasPull){

  TH1F * bkg = m_canvas.DrawFrame (xmin, ymin, xmax, ymax) ;
  bkg->GetXaxis ()->SetTitle (xaxisTitle.c_str ()) ;
  bkg->GetXaxis ()->SetTitleSize(0.045);
  bkg->GetXaxis ()->SetLabelSize(0.04);

  bkg->GetYaxis ()->SetTitle (yaxisTitle.c_str ()) ;
  bkg->GetYaxis ()->SetTitleSize(0.05);
  bkg->GetYaxis ()->SetTitleOffset(1.55);
  bkg->GetYaxis ()->SetLabelSize(0.04);

  bkg->Draw () ;

  TString banner = TString::Format("14 TeV, %d fb^{-1}, PU = %d",int(m_lumi)/1000,m_nPU);

  TLatex * tex;
  if(m_nPU >=100)
    tex = new TLatex(0.45,0.96,banner.Data());
  else
    tex = new TLatex(0.47,0.96,banner.Data());

  tex->SetNDC(1);
  tex->SetTextAlign(11);
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->Draw();

  TLatex* tex2 ;

  if(TString(m_phase).Contains("II")){
    TString banner2 = TString::Format("CMS Phase II Delphes Simulation");
    tex2 = new TLatex(0.21,0.89,banner2.Data());
  }
  else if (TString(m_phase).Contains("Aged")){
    TString banner2 = TString::Format("CMS Phase I Aged Delphes Simulation");
    tex2 = new TLatex(0.21,0.89,banner2.Data());
  }
  else {
    TString banner2 = TString::Format("CMS Phase I DelphesSimulation");
    tex2 = new TLatex(0.21,0.89,banner2.Data());

  }

  tex2->SetNDC(1);
  tex2->SetTextFont(61);
  tex2->SetTextAlign(11); 
  tex2->SetTextSize(0.04);
  tex2->SetLineWidth(2);
  tex2->Draw();

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotSingleSample (string sampleName, string layerName, string histoName,
                                string xaxisTitle, string yaxisTitle, int isLog, string folderName){

  prepareSampleForPlotting (sampleName) ; // prepare histo object

  TString xaxisTemp = TString::Format("%s",xaxisTitle.c_str());
  xaxisTemp.ReplaceAll("/_"," ");
  xaxisTemp.ReplaceAll("/"," ");
  xaxisTitle = xaxisTemp;

  int iSample = 0;

  vector<TH1F*> histoToPlot   ;
  vector<TH2F*> histoToPlot2D ;
  TLegend* leg = 0 ;
  TLegend* leg2D = 0 ;

  for( vector<sample>::iterator itSample = m_samples[sampleName].begin();
       itSample != m_samples[sampleName].end();
       itSample++){

    unordered_map<string, TH1F *>::const_iterator got1D =
      itSample->m_sampleContent[layerName].m_histos.find (histoName) ; // take an iterator on 1D histo for that sample and that layer

    // 1D histo found, plotting
    if ( got1D != itSample->m_sampleContent[layerName].m_histos.end ()){
      
      TH1F * h_var = got1D->second ;
      m_canvas.cd () ;

      if(iSample == 0){
	leg = initLegend (1) ; // crate the legend
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	    name.ReplaceAll("_","_{");
	    name += "}";
	}
	leg->AddEntry (h_var,name.Data(), "fl") ;
      }

      histoToPlot.push_back(h_var);
    }

    unordered_map<string, TH2F *>::const_iterator got2D =
      itSample->m_sampleContent[layerName].m_2Dhistos.find (histoName) ;

    // 1D histo found, plotting
    if ( got2D != itSample->m_sampleContent[layerName].m_2Dhistos.end ()){

      TH2F * h_var = got2D->second ;
      m_canvas.cd () ;

      if(iSample == 0){
	leg2D = initLegend (1) ;
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");
	  name += "}";
	}
	leg2D->AddEntry (h_var, name.Data(), "fl") ;
      }
      
      histoToPlot2D.push_back(h_var);
    }
    iSample++ ;
  }


  DrawPlots (histoToPlot, *leg, 1, xaxisTitle, yaxisTitle, isLog, folderName) ; // draw the histo
  Draw2DPlots (histoToPlot2D, *leg2D, 1, xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// plot a single histogram in a layer
void plotter::plotSingleLayer (string layerName, string histoName, 
                               string xaxisTitle, string yaxisTitle, int isLog, string folderName){ // plot all the samples in a stack for a layer

  TString xaxisTemp = TString::Format("%s",xaxisTitle.c_str());
  xaxisTemp.ReplaceAll("/_"," ");
  xaxisTemp.ReplaceAll("/"," ");
  xaxisTitle = xaxisTemp;

  string name = string ("st_") + layerName + "_" + histoName ;

  THStack * stack = new THStack (name.c_str (), "") ;

  int nsamples = m_samplesSequence.size () ;

  TLegend* leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){

    string sampleName = m_samplesSequence.at (iSample) ;
    prepareSampleForPlotting (sampleName) ;
    int iSub = 0;
    for( vector<sample>::iterator itSample = m_samples[sampleName].begin();
	 itSample != m_samples[sampleName].end();
	 itSample++){
      
      if (itSample->m_isSignal == 1) continue ;
      TH1F * h_var = itSample->m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add (h_var) ;
      if(iSub == 0){
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");			
	  name += "}";
	}
	leg->AddEntry (h_var, name.Data(), "fl") ;
      }
      iSub++;
    }
  }  
  
  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){

    string sampleName = m_samplesSequence.at (iSample) ;

    int iSub = 0;

    for( vector<sample>::iterator itSample = m_samples[sampleName].begin();
	 itSample != m_samples[sampleName].end();
	 itSample++){
      

      if (itSample->m_isSignal != 1) continue ;
      TH1F * h_var = itSample->m_sampleContent[layerName].m_histos[histoName] ;
      stack->Add (h_var) ;
      if(iSub == 0){
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");
	  name += "}";
	}
	leg->AddEntry (h_var, name.Data(), "fl") ;
      }
      iSub++;
    }	
  }

  DrawPlots (vector<THStack *> (1, stack), *leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;
  
  return ;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// plot all the histograms of a layer
void plotter::plotFullLayer (string layerName) { // plot all the layers
 
  // FIXME needs to be finished
  string outFolderName = m_folderName + "/stacks/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;
  
  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto){

    if(string(iHisto->second->GetXaxis()->GetTitle()) != ""){

      string yaxis ;
      if(TString(iHisto->second->GetXaxis()->GetTitle()).Contains("GeV"))
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1))+" GeV";
      else
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1));

      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->second->GetXaxis()->GetTitle(), yaxis, 1, outFolderName) ;
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->second->GetXaxis()->GetTitle(),yaxis, 0, outFolderName) ;
    }
    else {

      string yaxis ;
      if(TString(iHisto->second->GetXaxis()->GetTitle()).Contains("GeV"))
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1))+" GeV";
      else
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1));

      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str(), yaxis, 1, outFolderName) ;
      plotSingleLayer (layerName, iHisto->first.c_str (), 
                       iHisto->first.c_str(), yaxis, 0, outFolderName) ;

    }
  }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoB (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                           bool isNormalized, float ScaleSignal, int isLog, string folderName){

  // FIXME isNormalized needs to be implemented
  // FIXME ScaleSignal needs to be implemented

  TString xaxisTemp = TString::Format("%s",xaxisTitle.c_str());
  xaxisTemp.ReplaceAll("/_"," ");
  xaxisTemp.ReplaceAll("/"," ");
  xaxisTitle = xaxisTemp;

  string name = string ("comp_bkg_") + layerName + "_" + histoName ;

  THStack * bkg_stack = new THStack (name.c_str (), "") ;

  int nsamples = m_samplesSequence.size () ;

  TLegend* leg = initLegend (nsamples) ;

  // add bkg to the stack
  for (unsigned int iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample){
    
    string sampleName = m_samplesSequence.at (iSample) ;
    prepareSampleForPlotting (sampleName) ;
    int iSub = 0;
    for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
	itSample!= m_samples[sampleName].end();
	itSample++){

      if (itSample->m_isSignal == 1) continue ;
      TH1F * h_var = itSample->m_sampleContent[layerName].m_histos[histoName] ;
      bkg_stack->Add (h_var) ;
      if(iSub == 0){
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");
	  name += "}";
	}
	leg->AddEntry (h_var, name.Data(), "fl") ;
      }
      iSub++;
    }
  }
  
  name = string ("comp_sig_") + layerName + "_" + histoName ;
  THStack * sig_stack = new THStack (name.c_str (), "") ;

  // add signal to the stack
  for (int iSample = 0 ; iSample < nsamples ; ++iSample){
    
    int iSub = 0;
    string sampleName = m_samplesSequence.at (iSample) ;

    for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
	itSample!= m_samples[sampleName].end();
	itSample++){
      if (itSample->m_isSignal != 1) continue ;
      TH1F * h_var = itSample->m_sampleContent[layerName].m_histos[histoName] ;
      sig_stack->Add (h_var) ;
      if(iSub == 0){
	TString name = Form("%s",sampleName.c_str ());
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");
	  name += "}";
	}
	leg->AddEntry (h_var, name.Data(), "fl") ;
      }
      iSub++;
    }
  }

  vector<THStack *> histos ;
  histos.push_back (bkg_stack) ;
  histos.push_back (sig_stack) ;
  
  DrawPlots (histos, *leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoBFullLayer (string layerName, string folderTag){

  // FIXME needs to be finished
  string outFolderName = m_folderName + "/compareStoB" + folderTag + "/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto){

      string yaxis ;
      if(TString(iHisto->second->GetXaxis()->GetTitle()).Contains("GeV"))
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1))+" GeV";
      else
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1));


      compareStoB (layerName, iHisto->first.c_str (), iHisto->first.c_str (), yaxis, 
		   0, 1., 0, outFolderName) ;
      compareStoB (layerName, iHisto->first.c_str (), iHisto->first.c_str (), yaxis, 
		   0, 1., 1, outFolderName) ;
    }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::compareStoB2D (string layerName, string histoName, 
                             string xaxisTitle, string yaxisTitle, 
                             bool isNormalized, float scaleSignal, int isLog, 
                             string folderName){

  // FIXME isNormalized needs to be implemented
  // FIXME scaleSignal needs to be implemented
  TString xaxisTemp = TString::Format("%s",xaxisTitle.c_str());
  xaxisTemp.ReplaceAll("/_"," ");
  xaxisTemp.ReplaceAll("/"," ");
  xaxisTitle = xaxisTemp;

  string bkgName = string ("comp_bkg_") + layerName + "_" + histoName ;
  string sigName = string ("comp_sig_") + layerName + "_" + histoName ;
  TH2F * bkg_stack = 0 ;
  TH2F * sig_stack = 0 ;
  
  unsigned int nsamples = m_samplesSequence.size () ;

  // loop over samples
  for (unsigned int iSample = 0 ; iSample < nsamples ; ++iSample){

    string sampleName = m_samplesSequence.at (iSample) ;
    
    prepareSampleForPlotting (sampleName) ;

    int iSub = 0;
    for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
	itSample!= m_samples[sampleName].end();
	itSample++){


      TH2F * h_var = itSample->m_sampleContent[layerName].m_2Dhistos[histoName] ;
      if (itSample->m_isSignal == 1){
      if (sig_stack == 0)  
	sig_stack = (TH2F *) h_var->Clone (sigName.c_str ()) ;
      else                 
	sig_stack->Add (h_var) ;
      }
      else  {
	if (bkg_stack == 0)  
	  bkg_stack = (TH2F *) h_var->Clone (bkgName.c_str ()) ;
	else                 
	  bkg_stack->Add (h_var) ;
      }
      iSub++;
    }
  } // loop over samples

  TLegend* leg = initLegend (nsamples) ;
  leg->AddEntry (sig_stack, "sig", "fl") ;
  leg->AddEntry (bkg_stack, "bkg", "fl") ;
  
  vector<TH2F *> histos ;
  histos.push_back (sig_stack) ;
  histos.push_back (bkg_stack) ;
  
  Draw2DPlots (histos, *leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName) ;

  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::compareStoBFullLayer2D (string layerName, string folderTag){

  // FIXME needs to be finished

  string outFolderName = m_folderName + "/compareStoB" + folderTag + "/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH2F *>::iterator iHisto = 
         m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_2Dhistos.begin () ;
       iHisto != m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_2Dhistos.end () ; 
       ++iHisto){

    compareStoB2D (layerName, iHisto->first.c_str (), "", "", 
                   0, 1., 0, outFolderName) ;
    compareStoB2D (layerName, iHisto->first.c_str (), "", "", 
                   0, 1., 1, outFolderName) ;
  }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



void plotter::plotRelativeExcess (string layerName, string histoName, string xaxisTitle, string yaxisTitle, 
                                  bool isNormalized, float scaleSignal, int isLog,
                                  string folderName){

  
  // prepare the first stacks
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  TString xaxisTemp = TString::Format("%s",xaxisTitle.c_str());
  xaxisTemp.ReplaceAll("/_"," ");
  xaxisTemp.ReplaceAll("/"," ");
  xaxisTitle = xaxisTemp;

  string name = string ("st_SM_") + layerName + "_" + histoName ; // SM = QCD bkg + EWK 126 GeV H
  THStack * SM_stack = new THStack (name.c_str (), "") ;
  name = string ("st_nH_") + layerName + "_" + histoName ;  // no H = QCD bkg + EWK noH
  THStack * nH_stack = new THStack (name.c_str (), "") ;  
  int nsamples = m_samplesSequence.size () ;

  TLegend* leg = initLegend (nsamples + 1) ;

  // add bkg to the stack and identify the position of the two signals in the sample
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  vector<int> signalPosH126 ;
  vector<int> signalPosNoH  ;
  int sigSamplesNumH126 = 0;
  int sigSamplesNumNoH = 0;

  for (unsigned int iSample = 0 ; iSample < m_samplesSequence.size () ; ++iSample){

    string sampleName = m_samplesSequence.at (iSample) ;
    prepareSampleForPlotting (sampleName) ;
    
    int iSub = 0;
    for(vector<sample>::iterator itSubSample = m_samples[sampleName].begin();
	itSubSample!=m_samples[sampleName].end();
	itSubSample++){
       
      
      if (itSubSample->m_isSignal == 1 and TString(sampleName).Contains("126")) {
	signalPosH126.push_back(iSample);
	sigSamplesNumH126++;
	continue ;
      }

      else if (itSubSample->m_isSignal == 1 and TString(sampleName).Contains("noH")) {
	signalPosNoH.push_back(iSample);
	sigSamplesNumNoH++;
	continue ;
      }
      
      TH1F * h_var = itSubSample->m_sampleContent[layerName].m_histos[histoName] ;

      if(string(h_var->GetXaxis()->GetTitle())!="")
	xaxisTitle = h_var->GetXaxis()->GetTitle();

      SM_stack->Add (h_var) ;
      nH_stack->Add (h_var) ;
      if(iSub == 0){
	TString name = Form("%s",sampleName.c_str ());      
	if(name.Contains("_")){
	  name.ReplaceAll("_","_{");
	  name += "}";
	}
	leg->AddEntry (h_var, name.Data(), "fl") ;  
      }
      iSub++;
    }
    
  }
  
  if (sigSamplesNumH126 != sigSamplesNumNoH){
    cout << "ERROR: found " << sigSamplesNumH126 << " signal samples for H126 and "<<sigSamplesNumNoH<<" for noH exiting" << endl ;
    exit (1) ;
  }
  
  // add the first signal to the stack, to bild up the total SM expectations
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  string sampleName = m_samplesSequence.at (signalPosH126.at (0)) ;

  int iSub = 0;
  TH1F* h_sigSM = 0;

  for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
      itSample!= m_samples[sampleName].end();
      itSample++){

    if(iSub == 0)
      h_sigSM = (TH1F*) itSample->m_sampleContent[layerName].m_histos[histoName]->Clone("h_sigSM") ;  
    else 
      h_sigSM->Add(itSample->m_sampleContent[layerName].m_histos[histoName]);

    iSub++;
  }

  SM_stack->Add (h_sigSM) ;

  // calculate the difference between noH and SM126
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  sampleName = m_samplesSequence.at (signalPosNoH.at (0)) ;
  iSub = 0;
  TH1F * h_noH = 0 ;
  for(vector<sample>::iterator itSample = m_samples[sampleName].begin();
      itSample!= m_samples[sampleName].end();
      itSample++){

    if(iSub == 0)
      h_noH = (TH1F*) itSample->m_sampleContent[layerName].m_histos[histoName]->Clone("h_noH") ;      
    else
      h_noH->Add(itSample->m_sampleContent[layerName].m_histos[histoName]);  

    if(iSub == 0){
      name = "diff_" ;
      name += h_noH->GetName () ;
    }

    iSub++;
  }

  nH_stack->Add (h_noH) ;

  
  // put the difference in a THStack and compare it to the expected SM measurement
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  TH1F * diff = (TH1F *) h_noH->Clone (name.c_str ()) ;
  
  diff->Add (h_sigSM, -1.) ;
  diff->SetLineColor (2) ;
  diff->SetLineWidth (2) ;
  diff->SetFillStyle (0) ;

  name = diff->GetName () ;
  name += "_st" ;
  THStack * delta_stack = stackMe (diff) ;
  vector<THStack *> histos ;  // ensemble of objects to be drawn together
  histos.push_back (SM_stack) ;  
  histos.push_back (delta_stack) ;  
  
  // histogram to plot the error band of the SM bkg + 126 Higgs THstack 
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  name = "SM_histo_" ;
  name += h_sigSM->GetName () ;
  TH1F * SM_histo = (TH1F *) SM_stack->GetStack ()->Last ()->Clone (name.c_str ()) ;
  SM_histo->SetFillColor (1) ;
  SM_histo->SetLineColor (4) ;
  SM_histo->SetFillStyle (3004) ;

  // get the histogram of the total shape, of SM bkg + noH hypothesis
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  
  name = "nH_histo_" ;
  name += h_noH->GetName () ;
  TH1F * nH_histo = (TH1F *) nH_stack->GetStack ()->Last ()->Clone (name.c_str ()) ;
  nH_histo->SetLineColor (6) ;
  nH_histo->SetLineWidth (2) ;
  nH_histo->SetLineStyle (2) ;
  nH_histo->SetFillStyle (0) ;
  histos.push_back (stackMe (nH_histo)) ;

  leg->AddEntry (SM_histo, "WW_{EWK} (H_{126})", "fl") ;
  leg->AddEntry (nH_histo, "WW_{EWK} (noH_{126})", "fl") ;
  leg->AddEntry (diff,     "WW_{EWK} (noH_{126}-H_{126})", "fl") ;
  
  // do the drawing
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  DrawPlots (histos, *leg, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName, false) ;
  SM_histo->Draw ("E2same") ; // draw the error band on the SM THstack

  if (isLog) m_canvas.SetLogy (1) ;
  m_canvas.RedrawAxis () ;    
  leg->Draw () ;

  string filename = folderName + histos.at (0)->GetName () ;
  TString Name (filename.c_str());
  Name.ReplaceAll("#","");
  Name.ReplaceAll("{","");
  Name.ReplaceAll("}","");
  Name.ReplaceAll("[","");
  Name.ReplaceAll("]","");
  Name.ReplaceAll("^","");
  Name.ReplaceAll("__","_");
  Name.ReplaceAll("..",".");
  if (histos.size () > 1) Name += "_compare" ;
  if (isLog) Name += "_log" ;
  Name += ".pdf" ;
  m_canvas.Print (Name, "pdf") ;
  Name.ReplaceAll("pdf","png");
  m_canvas.Print (Name, "png") ;
  Name.ReplaceAll("png","root");
  m_canvas.Print (Name, "root") ;
  

  if (isLog) m_canvas.SetLogy (0) ;

  
  // direct comparison of the uncertainty on the SM to the difference between H and noH
  // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

  TLegend* leg2 = initLegend (2) ;
  TH1F * h_tot_SM     = (TH1F *) SM_stack->GetStack ()->Last () ;  
  TH1F * h_tot_SM_err = getHistoOfErrors (h_tot_SM,isLog) ;
  TH1F * h_significance  = (TH1F *) diff->Clone(("h_significance_"+string(h_noH->GetName())).c_str()) ;  

  for(int iBin = 0; iBin < h_significance->GetNbinsX()+1; iBin++){
    if(h_tot_SM_err->GetBinContent(iBin+1) == 0) h_significance->SetBinContent(iBin+1,0.);
    else h_significance->SetBinContent(iBin+1,diff->GetBinContent(iBin+1)/ h_tot_SM_err->GetBinContent(iBin+1));
  }

  h_tot_SM_err->SetLineColor (1) ;
  h_tot_SM_err->SetFillColor (1) ;
  h_tot_SM_err->SetFillStyle (3001) ;

  h_significance->SetLineColor (4) ;
  h_significance->SetLineWidth (2) ;
  
  leg2->AddEntry (h_tot_SM_err,   "SM fluct", "fl") ;
  leg2->AddEntry (diff,           "noH - 126", "fl") ;
  leg2->AddEntry (h_significance, "(noH - 126)/#sigma_{SM} ", "l") ;

  vector<TH1F *> histos2 ;
  histos2.push_back (h_tot_SM_err) ;
  histos2.push_back (diff) ;
  histos2.push_back (h_significance) ;
  DrawPlots (histos2, *leg2, m_samplesSequence.size (), xaxisTitle, yaxisTitle, isLog, folderName,false) ;

  filename = folderName +"err_st_" + string(histos.at (0)->GetName ());
  Name = TString::Format("%s",filename.c_str());
  Name.ReplaceAll("#","");
  Name.ReplaceAll("{","");
  Name.ReplaceAll("}","");
  Name.ReplaceAll("[","");
  Name.ReplaceAll("]","");
  Name.ReplaceAll("^","");
  Name.ReplaceAll("__","_");
  Name.ReplaceAll("..",".");
  if (isLog) Name += "_log" ;
  Name += ".pdf" ;
  m_canvas.Print (Name, "pdf") ;
  Name.ReplaceAll("pdf","png");
  m_canvas.Print (Name, "png") ;
  Name.ReplaceAll("png","root");
  m_canvas.Print (Name, "root") ;
  
  return ;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::plotRelativeExcessFullLayer (string layerName, string basefolder){

  // FIXME needs to be finished
  string outFolderName = m_folderName + "/" + basefolder + "/relExcess/" + layerName + "/";
  system (Form ("mkdir -p %s", outFolderName.c_str ())) ;

  // loop over variables and call plot single layer
  for (unordered_map<string, TH1F *>::iterator iHisto = 
         m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.begin () ;
       iHisto != m_samples.begin ()->second.at(0).m_sampleContent[layerName].m_histos.end () ; 
       ++iHisto){

      string yaxis ;
      if(TString(iHisto->second->GetXaxis()->GetTitle()).Contains("GeV"))
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1))+" GeV";
      else
	yaxis = "Events / "+TString::Format("%.2f",iHisto->second->GetBinWidth(1));


      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), yaxis, 
			  0, 1., 0, outFolderName) ;
      plotRelativeExcess (layerName, iHisto->first.c_str (), iHisto->first.c_str (), yaxis, 
			  0, 1., 1, outFolderName) ;
  }   
  
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::scaleAllHistos (float scaleFactor){

  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){

    for(vector<sample>::iterator iSubSample = iSample->second.begin ()  ;
	iSubSample != iSample->second.end();
	iSubSample++){

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < iSubSample->m_layersSequence.size () ; ++iLayer){
      
	string name = iSubSample->m_layersSequence.at (iLayer) ;
      
	// loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != iSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
	  iHisto->second->Scale (scaleFactor) ;
	}

	for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != iSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  iHisto->second->Scale (scaleFactor) ;       
	} // loop over histos

	if(m_includeSystematics){
	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }


	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }
	  

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_jetScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_jetScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }



	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }

	  for (unordered_map<string, TH1F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;
	  }
	
	  for (unordered_map<string, TH2F *>::iterator iHisto = iSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != iSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (scaleFactor) ;       
	  }
	}      
      }
    } // loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::saveAllHistos (string filename){

  TFile outfile ((m_folderName + "/" + filename).c_str (), "recreate") ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){

    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){
	 

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
	
	string name = itSubSample->m_layersSequence.at (iLayer) ;
        
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){

	  iHisto->second->Write () ;
	} // loop over histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  iHisto->second->Write () ;
	} // loop over histos

        if(m_includeSystematics){

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos



	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos


	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleUp.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos

          /////////////////////////////////////////////////

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos


	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto){
	    
	    iHisto->second->Write () ;
	  } // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto){
	    iHisto->second->Write () ;
	  } // loop over histos
	}
      } // loop over layers
    }
  } // loop over samples
  outfile.Close () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

void plotter::normaliseAll1DHistos (){

  cout << "WARNING normalising all the 1D histograms" << endl ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){


    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
	string name = itSubSample->m_layersSequence.at (iLayer) ;
	
	// loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
	  iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	} // loop over histos

	
	if(m_includeSystematics){

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos
	}
      }
    } // loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::normaliseAll2DHistos (){

  cout << "WARNING normalising all the 2D histograms" << endl ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){

    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){

	string name = itSubSample->m_layersSequence.at (iLayer) ;
          
	// loop over histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	} // loop over histos

	if(m_includeSystematics){

	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto){
	    iHisto->second->Scale (1. / iHisto->second->Integral ()) ;
	  } // loop over histos
	  
	}
      }
    } // loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::normaliseAllHistos (){

  normaliseAll1DHistos () ;
  normaliseAll2DHistos () ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::setPoissonErrors (){

  cout << "WARNING setting all the histograms to the poisson error\n" ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){
    
    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
	string name = itSubSample->m_layersSequence.at (iLayer) ;
          
	// loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
	  setPoissonErrorsToHisto (iHisto->second) ;
	} // loop over histos
	
	// loop over histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  setPoissonErrorsTo2DHisto (iHisto->second) ;
	} // loop over histos
 
	if(m_includeSystematics){

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos




	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto){
	    setPoissonErrorsToHisto (iHisto->second) ;
	  } // loop over histos
  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto){
	    setPoissonErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos
	}
      }
    } // loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::setAsymmetricErrors (){

  cout << "WARNING setting all the histograms to the poisson error\n" ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){

    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){

    
      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
	string name = itSubSample->m_layersSequence.at (iLayer) ;
	
	// loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto){
	  setAsymmetricErrorsToHisto (iHisto->second) ;
	} // loop over histos
  
	// loop over histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto){
	  setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	} // loop over histos
	
	if(m_includeSystematics){
	
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos


	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos


	  ///////////

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsToHisto (iHisto->second) ;
	  } // loop over histos
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto){
	    setAsymmetricErrorsTo2DHisto (iHisto->second) ;
	  } // loop over histos	  
	}
      }
    } // loop over layers
  } // loop over samples
  return ;
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::resetAll (float lumi){

  cout << "WARNING resetting all the stored quantities\n" ;
  cout << "luminosity set to " << lumi << "\n" ;
  m_lumi = lumi ;
  for (unordered_map<string, vector<sample> >::iterator iSample = m_samples.begin () ;
       iSample != m_samples.end () ;
       ++iSample){

    for( vector<sample>::iterator itSubSample = iSample->second.begin();
	 itSubSample != iSample->second.end();
	 itSubSample++){

      // loop over layers
      for (unsigned int iLayer = 0 ; iLayer < itSubSample->m_layersSequence.size () ; ++iLayer){
	string name = itSubSample->m_layersSequence.at (iLayer) ;
	
	// loop over histos
	for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_histos.end () ;
	     ++iHisto)
	    iHisto->second->Reset () ;

	// loop over histos
	for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos.begin () ;
	     iHisto != itSubSample->m_sampleContent[name].m_2Dhistos.end () ;
	     ++iHisto)
	  iHisto->second->Reset () ;

	
	if(m_includeSystematics){

          
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleUp.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleUp.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;

	
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;



	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleUp.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	  
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleUp.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;

	
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	  
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleUp.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;

	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	
	// loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetScaleDown.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_muRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_muRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_muRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;


	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_elRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	
	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_elRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_elRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;

          
	  // loop over histos
	  for (unordered_map<string, TH1F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_histos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_histos_jetRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;

	  // loop over histos
	  for (unordered_map<string, TH2F *>::iterator iHisto = itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.begin () ;
	       iHisto != itSubSample->m_sampleContent[name].m_2Dhistos_jetRes.end () ;
	       ++iHisto)
	    iHisto->second->Reset () ;
	}	
      }
    } // loop over layers
  } // loop over samples
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

TLegend* plotter::initLegend (int sampleNum){

  TLegend* leg = new TLegend (0.2, 0.9 - 0.1 * sampleNum * 0.45, 0.87, 0.86) ;
  leg->SetNColumns (2) ;
  leg->SetLineStyle (0) ;
  leg->SetFillStyle (0) ;
  return leg ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void plotter::setRootAspect (){

  gStyle->SetCanvasColor (-1) ; 
  gStyle->SetPadColor (-1) ; 
  gStyle->SetFrameFillColor (-1) ; 
  gStyle->SetHistFillColor (-1) ; 
  gStyle->SetTitleFillColor (-1) ; 
  gStyle->SetFillColor (-1) ; 
  gStyle->SetStatStyle (0) ; 
  gStyle->SetTitleStyle (0) ; 
  gStyle->SetCanvasBorderSize (0) ; 
  gStyle->SetFrameBorderSize (0) ; 
  gStyle->SetLegendBorderSize (0) ; 
  gStyle->SetStatBorderSize (0) ; 
  gStyle->SetTitleBorderSize (0) ; 

  m_canvas.SetLeftMargin (0.17) ; 
  m_canvas.SetTopMargin (0.05) ; 
  gStyle->SetTitleYOffset (2) ;

  return ;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat (1111111) ;
  gStyle->SetOptFit (1111) ;
  gStyle->SetTitleBorderSize (0) ;
  gStyle->SetTitleX (0.08) ;
  gStyle->SetTitleY (0.97) ;
  gStyle->SetPalette (1,0) ;
  gStyle->SetLabelFont (42,"xyz") ;
  gStyle->SetTextFont (42) ;
  gStyle->SetStatFont (42) ;
  gStyle->SetTitleFont (42,"xyz") ;
  
  gStyle->SetTitleXOffset (1.25) ;
  gStyle->SetTitleYOffset (1.4) ;
  
  gStyle->SetTitleX (0.5) ;
  gStyle->SetTitleFont (42) ;
  gStyle->SetTitleAlign (23) ;
  
  gStyle->SetPadRightMargin  (0.15)  ;
  gStyle->SetPadLeftMargin   (0.25) ;
  gStyle->SetPadBottomMargin (0.22) ;
  gStyle->SetPadTopMargin    (0.1) ;
  
  gStyle->SetTitleSize (0.04, "xyz") ;
  gStyle->SetLabelSize (0.05, "xyz") ;
  
  gStyle->SetPadTickX (1) ;
  gStyle->SetPadTickY (1) ;
  
  gStyle->SetNdivisions (7, "xyz") ;
  
  gStyle->SetLineWidth (1) ;
  gStyle->SetFillStyle (0) ;
  gStyle->SetStatStyle (0) ;

}
