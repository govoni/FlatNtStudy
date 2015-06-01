#include "utils.h"
#include <memory>

//----------------------
template <typename T>
void loopOnEvents (plotter & analysisPlots, 
		   const string & sampleName,     
		   const int    & samplePosition,     
		   readTree* reader, 
		   vector<T>    & CutList,
		   vector<variableContainer> & VariableList,
		   const bool   & usePuppiAsDefault, 
		   const float  & minPtLeptonCut, 
		   const float  & minPtLeptonCutCleaning,
		   const float  & leptonIsoCut_mu,
		   const float  & leptonIsoCut_el, 
		   const float  & leptonIsoLooseCut,
		   const float  & matchingCone,   
		   const float  & minJetCutPt,   
		   map <string,TH1F*> & vect,
		   const string & finalStateString,
		   const string & scenarioString,
		   const string & fakeRateFile) {

  cout << "reading sample " << sampleName << "\n" ;
  cout << "analysis object sys set to "<<analysisPlots.getSystematics()<<endl;

  int maxevents = reader->fChain->GetEntries() ;

  // define the formulat for systematics study  --> should be provided by Egamma, Muon and JME POGs
  objectFormula scenarioFormula (scenarioString); 

  // define fake rate and migration matrix  
  shared_ptr<TFile> inputFile(new TFile(fakeRateFile.c_str(),"READ"));
  fakeRateContainer       fakeRate(inputFile.get()); 
  fakeMigrationContainer* fakeMigration = new fakeMigrationContainer(inputFile.get());

  // fake rate application --> check if the fake method has to be applied
  vector<sample> vecSample = analysisPlots.getSamples();
  bool applyFake = false;
  bool applyCharge = false;
  for( size_t iSample = 0; iSample < vecSample.size(); iSample++){
    if(vecSample.at(iSample).m_sampleName != sampleName) 
      continue ;
    if(vecSample.at(iSample).m_isSignal == -1)
      applyFake = true;
    if(vecSample.at(iSample).m_isSignal == -2)
      applyCharge = true ;
  }

  if(applyCharge and applyFake){
    cerr<<"problem in the setup, both fakes and charge misid has to be applied for this sample --> please check"<<endl;
    return ;
  }

  
  // loop over events ////////                                                                                                                                               
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){                                                                                                                      

    reader->fChain->GetEntry(iEvent) ;                                                                                                                                      
           
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;                                                                                                  

    // filter LHE level leptons for madgraph polarized events to speed up /////////
    if(not applyFake){
      if((TString(sampleName).Contains("Madgraph") and TString(sampleName).Contains("WW")) or TString(sampleName).Contains("WW_EWK") or TString(sampleName).Contains("WW_QCD")){
	if(TString(finalStateString) == "UU"){
	  if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
	    continue;
	}
	else if(TString(finalStateString) == "EE"){
	  if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) 
	    continue;
	}
	else if(TString(finalStateString) == "EU"){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 11) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 13) 
	    continue ;
	}
	else if(TString(finalStateString).Contains("UE")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 13) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 11) 
	    continue ;
	}
    
	// if an event pass the cut, fill the associated map                                                                                                                 
	TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2 ;

	L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
	L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
      
	L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
	L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

	if(L_lepton1.Pt() < minPtLeptonCut or L_lepton2.Pt() < minPtLeptonCut) 
	  continue;
      } 
    }   

    // loop on the cut list
    for(size_t iCut = 0; iCut < CutList.size() ; iCut++){ 

      TLorentzVector L_met, L_gen_met;
      TLorentzVector L_met_muonScaleUp, L_met_muonScaleDown, L_met_muonRes;
      TLorentzVector L_met_electronScaleUp, L_met_electronScaleDown, L_met_electronRes;
      TLorentzVector L_met_jetScaleUp, L_met_jetScaleDown, L_met_jetRes;

      if(not usePuppiAsDefault)
	L_met.SetPtEtaPhiM(reader->pfmet,0.,reader->pfmetphi, 0.) ;                                                                                                        
      else
	L_met.SetPtEtaPhiM(reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;                                                                                              

      L_gen_met.SetPtEtaPhiM(reader->metGenpt,0.,reader->metGenphi, 0.) ;                                                                                             

      //Lepton Sector ///////      
      vector<leptonContainer> LeptonsAll;
      vector<leptonContainer> muonsAll,muonsAllScaleUp, muonsAllScaleDown, muonsAllRes;
      vector<leptonContainer> electronsAll,electronsAllScaleUp, electronsAllScaleDown, electronsAllRes;
      fillRecoLeptonsArray (LeptonsAll, *reader);

      // dump tight leptons                                                                                                                                        
      vector<leptonContainer> leptonsIsoTight;
      vector<leptonContainer> muonsIsoTight,muonsIsoTightScaleUp, muonsIsoTightScaleDown, muonsIsoTightRes;
      vector<leptonContainer> electronsIsoTight,electronsIsoTightScaleUp, electronsIsoTightScaleDown, electronsIsoTightRes;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

      // take reco jets                                                                                                                                                   
      vector<jetContainer> RecoJetsAll, RecoJetsAllScaleUp, RecoJetsAllScaleDown, RecoJetsAllRes;
      if(not usePuppiAsDefault)
	fillRecoJetArray (RecoJetsAll, *reader) ;      
      else
	fillPuppiJetArray (RecoJetsAll, *reader) ;
      

      // take soft muons 
      vector<leptonContainer> softMuons;
      softMuons = dumpSoftMuons(LeptonsAll,RecoJetsAll,leptonIsoCut_mu,minPtLeptonCut,softMuonPt,matchingCone);

      // take jets                                                                                                                                                          
      vector<jetContainer> RecoJets, RecoJetsScaleUp, RecoJetsScaleDown, RecoJetsRes;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);

      // take gen jets                                                                                                                                                
      vector<jetContainer> GenJetsAll ;
      fillGenJetArray (GenJetsAll, *reader) ;

      // take gen jets                                                                                                                                                     
      vector<jetContainer> GenJets;
      GenJets  = dumpJets (GenJetsAll, leptonsIsoTight, 0., 999, -999, minPtLeptonCutCleaning, matchingCone);

      // take track jets
      vector<jetContainer> trackJetsAll;
      fillTrackJetArray (trackJetsAll,*reader) ;
            
      if(applyFake){ // the sample require the fake rate application
	
	makeFakeLeptonBackground( sampleName,        // name of the sample
				  samplePosition,     // position used in case of mutiple physics process with same sample name
				  finalStateString,  // final state string
				  analysisPlots,     // plotter object
				  reader,            // reader object
				  CutList.at(iCut),  // cut container object
				  VariableList,      // vector<variableContainer>
				  leptonsIsoTight,   // tight leptons as input
				  LeptonsAll,        // all leptons as input
				  softMuons,         // softMuons
				  RecoJets,          // reco jets 
				  GenJets,           // gen jets 
				  trackJetsAll,      // all track jets
				  L_met,             // missing energy
				  minJetCutPt,        // min jet pt
				  leptonIsoLooseCut,  // loose iso cut
				  minPtLeptonCutCleaning,  // cleaning lepton-jet
				  matchingCone,       // matching for cleaning
				  fakeRate,           // fake rate map
				  *fakeMigration,     // migration
				  vect);              // efficiency histograms
      }

      else if (applyCharge){

	makeFakeChargeBackground(sampleName,        // sampleName
				 samplePosition,     // samplePostion
				 finalStateString,  // indentify final state
				 analysisPlots,     // plotter object
				 reader,            // tree reader
				 CutList.at(iCut),  // cut container
				 VariableList,      // list of variables to plot
				 leptonsIsoTight,   // tight leptons
				 LeptonsAll,        // all leptons
				 softMuons,         // softMuons
				 RecoJets,          // reco jets 
				 GenJets,           // gen jets 
				 trackJetsAll,      // all track jets
				 L_met,             // missing energy
				 minJetCutPt,        // min jet pt
				 leptonIsoLooseCut,  // loose iso cut
				 minPtLeptonCutCleaning,  // cleaning lepton-jet
				 matchingCone,       // matching for cleaning
				 vect                // efficiency histos
				 );
      }

      else { // if not fake rate sample
      
	float dRThreshold     = 0.5 ;      
	float eventFakeWeight = 1;

	vector<jetContainer> trackJets ;
	trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
	trackJetEvent trackEvent;
	trackEvent = produceTrackJetEvent (trackJets,RecoJets);

	/// insert the systematics in the analysis
	if(analysisPlots.getSystematics() and not applyFake){

	  // lepton scale and resolution sector
	  for(size_t iLep = 0; iLep < LeptonsAll.size(); iLep++){
	    float scaleValueUnc = 0;
	    float resValueUnc = 0;

	    if(abs(LeptonsAll.at(iLep).flavour_) == 13){	     

	      muonsAll.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      muonsAllRes.push_back(LeptonsAll.at(iLep));
	      
	      scaleValueUnc = scenarioFormula.evaluateMuonScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateMuonResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      muonsAllScaleUp.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							  muonsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      muonsAllScaleDown.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
							    muonsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.E()*(1-scaleValueUnc));


	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);

	      muonsAllRes.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(resUnc),							  
						      muonsAll.back().lepton4V_.Py()*(resUnc),
						      muonsAll.back().lepton4V_.Pz()*(resUnc),
						      muonsAll.back().lepton4V_.E()*(resUnc));

	      L_met_muonScaleUp   = L_met - muonsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonScaleDown = L_met - muonsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonRes       = L_met - muonsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;


	    }

	    else if (abs(LeptonsAll.at(iLep).flavour_) == 11){

	      electronsAll.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      electronsAllRes.push_back(LeptonsAll.at(iLep));

	      scaleValueUnc = scenarioFormula.evaluateElectronScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateElectronResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      electronsAllScaleUp.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							      electronsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      electronsAllScaleDown.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
								electronsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);
	      
	      electronsAllRes.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(resUnc),							  
							  electronsAll.back().lepton4V_.Py()*(resUnc),
							  electronsAll.back().lepton4V_.Pz()*(resUnc),
							  electronsAll.back().lepton4V_.E()*(resUnc));

	      L_met_electronScaleUp   = L_met - electronsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronScaleDown = L_met - electronsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronRes       = L_met - electronsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;

	    }

	  }

	  muonsIsoTightScaleUp   = dumpLeptons (muonsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightScaleDown = dumpLeptons (muonsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightRes       = dumpLeptons (muonsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTight          = dumpLeptons (muonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  electronsIsoTightScaleUp   = dumpLeptons (electronsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightScaleDown = dumpLeptons (electronsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightRes       = dumpLeptons (electronsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTight          = dumpLeptons (electronsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  // jet scale and resolution sector
	  for(size_t iJet = 0; iJet < RecoJetsAll.size(); iJet++){

	    RecoJetsAllScaleUp.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllScaleDown.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllRes.push_back(RecoJetsAll.at(iJet));
	  
	    float jetScaleUnc = scenarioFormula.evaluateJetScaleUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta());
	    float jetResUnc   = scenarioFormula.evaluateJetResolutionUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta(),RecoJetsAll.at(iJet).jet4V_.E());

	    RecoJetsAllScaleUp.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(1+jetScaleUnc), 
							RecoJetsAll.at(iJet).jet4V_.Py()*(1+jetScaleUnc),
							RecoJetsAll.at(iJet).jet4V_.Pz()*(1+jetScaleUnc),
							RecoJetsAll.at(iJet).jet4V_.E()*(1+jetScaleUnc));

	    RecoJetsAllScaleDown.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(1-jetScaleUnc), 
							  RecoJetsAll.at(iJet).jet4V_.Py()*(1-jetScaleUnc),
							  RecoJetsAll.at(iJet).jet4V_.Pz()*(1-jetScaleUnc),
							  RecoJetsAll.at(iJet).jet4V_.E()*(1-jetScaleUnc));

	    float resUnc = 1 + gRandom->Gaus(0,jetResUnc);

	    RecoJetsAllRes.back().jet4V_.SetPxPyPzE(RecoJetsAll.at(iJet).jet4V_.Px()*(resUnc), 
						    RecoJetsAll.at(iJet).jet4V_.Py()*(resUnc),
						    RecoJetsAll.at(iJet).jet4V_.Pz()*(resUnc),
						    RecoJetsAll.at(iJet).jet4V_.E()*(resUnc));

	    L_met_jetScaleUp   = L_met - RecoJetsAllScaleUp.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetScaleDown = L_met - RecoJetsAllScaleDown.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetRes       = L_met - RecoJetsAllRes.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;

	  }

	  RecoJetsScaleUp   = dumpJets (RecoJetsAllScaleUp,leptonsIsoTight,minJetCutPt,999,CutList.at(iCut).jetPUID,minPtLeptonCutCleaning,matchingCone);
	  RecoJetsScaleDown = dumpJets (RecoJetsAllScaleDown, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning,matchingCone);
	  RecoJetsRes       = dumpJets (RecoJetsAllRes, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);

	}    

	if( passCutContainerSelection (CutList.at(iCut),
				       sampleName,
				       samplePosition,
				       reader,
				       LeptonsAll,
				       leptonsIsoTight,
				       softMuons,
				       RecoJets,
				       trackEvent,
				       L_met,
				       minPtLeptonCut,
				       leptonIsoLooseCut,
				       vect,
				       finalStateString,
				       eventFakeWeight)){
	    
	
	  if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){	
	    fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			leptonsIsoTight, softMuons, RecoJets, GenJets, trackEvent, L_met, "",eventFakeWeight);
	
	  }
	  
	  else if (typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	    fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			leptonsIsoTight, softMuons, RecoJets, GenJets, trackEvent, L_met, "",eventFakeWeight);
	
	  }
	}
	  
	/// if perform sys analysis      
	if(analysisPlots.getSystematics()){
	  
	  // analysis scaling muons Up
	  map<string,TH1F*> tempVect;
	  
	  vector<leptonContainer> leptonAll (muonsAllScaleUp);
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  vector<leptonContainer> leptonTight (muonsIsoTightScaleUp);
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					   sampleName,
					   samplePosition,
					   reader,
					   leptonAll,
					   leptonTight,
					   softMuons,
					   RecoJets,
					   trackEvent,
					   L_met_muonScaleUp,
					   minPtLeptonCut,
					   leptonIsoLooseCut,
					   tempVect,
					   finalStateString,
					   eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleUp, "muScaleUp",eventFakeWeight);
	    }
	  
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleUp, "muScaleUp",eventFakeWeight);
	    }
	  }

	  // analysis scaling muon down
	  leptonAll.clear();
	  leptonAll = muonsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  leptonTight.clear();
	  leptonTight = muonsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	      
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
              fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList,
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	      
            }
          }


	  // scale electron up
	  leptonAll.clear();
	  leptonAll = electronsAllScaleUp;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());

	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleUp;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	  
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleUp, "elScaleUp",eventFakeWeight);
	    }
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleUp, "elScaleUp",eventFakeWeight);
	    }
	  }
	  
	  // scale electron down
	  leptonAll.clear();
	  leptonAll = electronsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){	    
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleDown, "elScaleDown",eventFakeWeight);
	    }
	    	    
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight, softMuons, RecoJets, GenJets, trackEvent, L_met_electronScaleDown, "elScaleDown",eventFakeWeight);
	    }
	  }
	

	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = muonsAllRes;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());
	  leptonTight.clear();
	  leptonTight = muonsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){
	    
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonRes, "muRes",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_muonRes, "muRes",eventFakeWeight);
	    }
	  }


	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = electronsAllRes;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());
	  
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){
	    
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_electronRes, "elRes",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, sampleName, samplePosition, CutList.at(iCut).cutLayerName, VariableList, 
			  leptonTight,  softMuons, RecoJets, GenJets, trackEvent, L_met_electronRes, "elRes",eventFakeWeight);
	    }
	  }
	  
	  //// jets scale up
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleUp,
					 trackEvent,
					 L_met_jetScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){
	      
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, VariableList, 
			  leptonsIsoTight, softMuons, RecoJetsScaleUp, 
			  GenJets,trackEvent,
			  L_met_jetScaleUp, "jetScaleUp",eventFakeWeight);
	    }	    
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
		fillHistoWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName, VariableList, 
			    leptonsIsoTight, softMuons, RecoJetsScaleUp, 
			    GenJets,trackEvent,
			    L_met_jetScaleUp, "jetScaleUp",eventFakeWeight);
	    }
	  }

	  // analysis scaling jets down
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,					 
					 softMuons,
					 RecoJetsScaleDown,
					 trackEvent,
					 L_met_jetScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      
	      fillHistoWW(analysisPlots, sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList, 
			  leptonsIsoTight,softMuons,RecoJetsScaleDown, 
			  GenJets,trackEvent,
			  L_met_jetScaleDown, 
			  "jetScaleDown",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	  
	      fillHistoWZ(analysisPlots, sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList, 
			  leptonsIsoTight,softMuons,RecoJetsScaleDown, 
			  GenJets,trackEvent,
			  L_met_jetScaleDown, 
			  "jetScaleDown",eventFakeWeight);
	    }
	  }


	  // analysis smearing jets
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsRes,
					 trackEvent,
					 L_met_jetRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	  if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots,sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList, 
			  leptonsIsoTight,
			  softMuons,RecoJetsRes, 
			  GenJets, trackEvent,
			  L_met_jetRes,
			  "jetRes",eventFakeWeight);
	  }	
	  else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots,sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList, 
			  leptonsIsoTight,
			  softMuons,RecoJetsRes, 
			  GenJets, trackEvent,
			  L_met_jetRes,
			  "jetRes",eventFakeWeight);
	    }	
	  }
	}
      }
    }
  }
}


template <typename T>
void loopOnEvents (plotter & analysisPlots, 
		   const string & sampleName,     
		   const int    & samplePosition,     
		   readTree* reader, 
		   vector<T> & CutList,
		   vector<variableContainer> & VariableList,
		   vector<variableContainer2D> & VariableList2D,
		   const bool & usePuppiAsDefault, 
		   const float & minPtLeptonCut, 
		   const float & minPtLeptonCutCleaning,
		   const float & leptonIsoCut_mu,
		   const float & leptonIsoCut_el, 
		   const float & leptonIsoLooseCut,
		   const float & matchingCone,   
		   const float & minJetCutPt,   
		   map <string,TH1F*> & vect,
		   const string & finalStateString,
		   const string & scenarioString,
		   const string & fakeRateFile) {

  cout << "reading sample " << sampleName << "\n" ;
  cout<<"analysis object sys set to "<<analysisPlots.getSystematics()<<endl;

  int maxevents = reader->fChain->GetEntries() ;

  // define the formulat for systematics study  
  objectFormula scenarioFormula (scenarioString); 

  // define fake rate and migration matrix
  shared_ptr<TFile> inputFile(new TFile(fakeRateFile.c_str(),"READ"));
  fakeRateContainer       fakeRate(inputFile.get()); 
  fakeMigrationContainer* fakeMigration = new fakeMigrationContainer(inputFile.get());


  // fake rate application --> check if the fake method has to be applied
  vector<sample> vecSample = analysisPlots.getSamples();
  bool applyFake = false;
  bool applyCharge = false;
  for( size_t iSample = 0; iSample < vecSample.size(); iSample++){
    if(vecSample.at(iSample).m_sampleName != sampleName) 
      continue ;
    if(vecSample.at(iSample).m_isSignal == -1)
      applyFake = true;
    if(vecSample.at(iSample).m_isSignal == -2)
      applyCharge = true ;
  }

  if(applyCharge and applyFake){
    cerr<<"problem in the setup, both fakes and charge misid has to be applied for this sample --> please check"<<endl;
    return ;
  }
  
  // loop over events ////////                                                                                                                                               
  for (int iEvent = 0 ; iEvent < maxevents ; ++iEvent){                                                                                                                      

    reader->fChain->GetEntry(iEvent) ;                                                                                                                                      
           
    if (iEvent % 100000 == 0) cout << "reading event " << iEvent << "\n" ;                                                                                                  

    // filter LHE level leptons for madgraph polarized events /////////
    if(not applyFake){
      if((TString(sampleName).Contains("Madgraph") and TString(sampleName).Contains("WW")) or TString(sampleName).Contains("WW_EWK") or TString(sampleName).Contains("WW_QCD")){
	if(TString(finalStateString).Contains("UU")){
	  if(fabs(reader->leptonLHEpid1) != 13 or fabs(reader->leptonLHEpid2) != 13)
	    continue;
	}
	else if(TString(finalStateString).Contains("EE")){
	  if(fabs(reader->leptonLHEpid1) != 11 or fabs(reader->leptonLHEpid2) != 11) 
	    continue;
	}
	else if(TString(finalStateString).Contains("EU")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 11) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 13) 
	    continue ;
	}
	else if(TString(finalStateString).Contains("UE")){
	  if(fabs(reader->leptonLHEpid1) == fabs(reader->leptonLHEpid2)) 
	    continue ;
	  if(fabs(reader->leptonLHEpid1) != 13) 
	    continue;
	  if(fabs(reader->leptonLHEpid2) != 11) 
	    continue ;
	}
      }

      // if an event pass the cut, fill the associated map                                                                                                                 
      TLorentzVector L_lepton1, L_lepton2, L_parton1, L_parton2 ;

      L_lepton1.SetPtEtaPhiM(reader->leptonLHEpt1,reader->leptonLHEeta1,reader->leptonLHEphi1,reader->leptonLHEm1);
      L_lepton2.SetPtEtaPhiM(reader->leptonLHEpt2,reader->leptonLHEeta2,reader->leptonLHEphi2,reader->leptonLHEm2);
      
      L_parton1.SetPtEtaPhiM(reader->jetLHEPartonpt1,reader->jetLHEPartoneta1,reader->jetLHEPartonphi1,0.);
      L_parton2.SetPtEtaPhiM(reader->jetLHEPartonpt2,reader->jetLHEPartoneta2,reader->jetLHEPartonphi2,0.);

      if(L_lepton1.Pt() < minPtLeptonCut or L_lepton2.Pt() < minPtLeptonCut) 
	continue;
    }

    for(size_t iCut = 0; iCut < CutList.size() ; iCut++){ 

      TLorentzVector L_met, L_gen_met;

      TLorentzVector L_met_muonScaleUp, L_met_muonScaleDown, L_met_muonRes;
      TLorentzVector L_met_electronScaleUp, L_met_electronScaleDown, L_met_electronRes;
      TLorentzVector L_met_jetScaleUp, L_met_jetScaleDown, L_met_jetRes;

      if(not usePuppiAsDefault)
	L_met.SetPtEtaPhiM       (reader->pfmet,0.,reader->pfmetphi, 0.) ;                                                                                                  
      else
	L_met.SetPtEtaPhiM (reader->pfmet_puppi,0.,reader->pfmetphi_puppi, 0.) ;                                                                                        
      
      L_gen_met.SetPtEtaPhiM   (reader->metGenpt,0.,reader->metGenphi, 0.) ;                                                                                             

      //Lepton Sector ///////      
      vector<leptonContainer> LeptonsAll;
      vector<leptonContainer> muonsAll,muonsAllScaleUp, muonsAllScaleDown, muonsAllRes;
      vector<leptonContainer> electronsAll,electronsAllScaleUp, electronsAllScaleDown, electronsAllRes;
      fillRecoLeptonsArray (LeptonsAll, *reader);

      // dump tight leptons                                                                                                                                        
      vector<leptonContainer> leptonsIsoTight;
      vector<leptonContainer> muonsIsoTight,muonsIsoTightScaleUp, muonsIsoTightScaleDown, muonsIsoTightRes;
      vector<leptonContainer> electronsIsoTight,electronsIsoTightScaleUp, electronsIsoTightScaleDown, electronsIsoTightRes;
      leptonsIsoTight = dumpLeptons (LeptonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

      // take reco jets                                                                                                                                                   
      vector<jetContainer> RecoJetsAll, RecoJetsAllScaleUp, RecoJetsAllScaleDown, RecoJetsAllRes;
      if(not usePuppiAsDefault){
	fillRecoJetArray (RecoJetsAll, *reader) ;
      }
      else{
	fillPuppiJetArray (RecoJetsAll, *reader) ;
      }

      // dump soft muons
      vector<leptonContainer> softMuons;
      softMuons = dumpSoftMuons(LeptonsAll,RecoJetsAll,leptonIsoCut_mu,minPtLeptonCut,softMuonPt,matchingCone);

      // take jets                                                                                                                                                          
      vector<jetContainer> RecoJets, RecoJetsScaleUp, RecoJetsScaleDown, RecoJetsRes;
      RecoJets  = dumpJets (RecoJetsAll, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);

      // take gen jets                                                                                                                                                
      vector<jetContainer> GenJetsAll ;
      fillGenJetArray (GenJetsAll, *reader) ;

      // take gen jets                                                                                                                                                     
      vector<jetContainer> GenJets;
      GenJets  = dumpJets (GenJetsAll, leptonsIsoTight, 0., 999, -999, minPtLeptonCutCleaning, matchingCone);
      
      // take track jets
      vector<jetContainer> trackJetsAll;
      fillTrackJetArray (trackJetsAll,*reader) ;
      vector<jetContainer> trackJets ;
      float dRThreshold = 0.5 ;      

      float eventFakeWeight = 1.;

      if(applyFake){ // the sample require the fake rate application

	makeFakeLeptonBackground( sampleName,        // name of the sample
				  samplePosition,     
				  finalStateString,  // final state string
				  analysisPlots,     // plotter object
				  reader,            // reader object
				  CutList.at(iCut),  // cut container object
				  VariableList,      // vector<variableContainer>
				  VariableList2D,      // vector<variableContainer>
				  leptonsIsoTight,   // tight leptons as input
				  LeptonsAll,        // all leptons as input
				  softMuons,         // softMuons
				  RecoJets,          // reco jets 
				  GenJets,           // gen jets 
				  trackJetsAll,      // all track jets
				  L_met,             // missing energy
				  minJetCutPt,        // min jet pt
				  leptonIsoLooseCut,  // loose iso cut
				  minPtLeptonCutCleaning,  // cleaning lepton-jet
				  matchingCone,       // matching for cleaning
				  fakeRate,           // fake rate map
				  *fakeMigration,     // migration
				  vect);              // efficiency histograms
      }

      else if (applyCharge){

	makeFakeChargeBackground(sampleName,        // sampleName
				 samplePosition,     
				 finalStateString,  // indentify final state
				 analysisPlots,     // plotter object
				 reader,            // tree reader
				 CutList.at(iCut),  // cut container
				 VariableList,      // list of variables to plot
				 VariableList2D,      // list of variables to plot
				 leptonsIsoTight,   // tight leptons
				 LeptonsAll,        // all leptons
				 softMuons,         // softMuons
				 RecoJets,          // reco jets 
				 GenJets,           // gen jets 
				 trackJetsAll,      // all track jets
				 L_met,             // missing energy
				 minJetCutPt,        // min jet pt
				 leptonIsoLooseCut,  // loose iso cut
				 minPtLeptonCutCleaning,  // cleaning lepton-jet
				 matchingCone,       // matching for cleaning
				 vect                // efficiency histos
				 );

      }
      else { // if not fake rate sample
      
	trackJets = dumpTrackJets (trackJetsAll, leptonsIsoTight, 1., minPtLeptonCutCleaning, dRThreshold);
	trackJetEvent trackEvent = produceTrackJetEvent(trackJets,RecoJets);
	
	if(analysisPlots.getSystematics()){

	  // lepton scale and resolution sector
	  for(size_t iLep = 0; iLep < LeptonsAll.size(); iLep++){
	    float scaleValueUnc = 0;
	    float resValueUnc = 0;

	    if(abs(LeptonsAll.at(iLep).flavour_) == 13){

	      muonsAll.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      muonsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      muonsAllRes.push_back(LeptonsAll.at(iLep));
	      
	      scaleValueUnc = scenarioFormula.evaluateMuonScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateMuonResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      muonsAllScaleUp.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							  muonsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							  muonsAll.back().lepton4V_.E()*(1+scaleValueUnc));

	      muonsAllScaleDown.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
							    muonsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
							    muonsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);


	      muonsAllRes.back().lepton4V_.SetPxPyPzE(muonsAll.back().lepton4V_.Px()*(resUnc),							  
						      muonsAll.back().lepton4V_.Py()*(resUnc),
						      muonsAll.back().lepton4V_.Pz()*(resUnc),
						      muonsAll.back().lepton4V_.E()*(resUnc));
	      

	      L_met_muonScaleUp   = L_met - muonsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonScaleDown = L_met - muonsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_muonRes       = L_met - muonsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      
	    }
	    
	    else if (abs(LeptonsAll.at(iLep).flavour_) == 11){

	      electronsAll.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleUp.push_back(LeptonsAll.at(iLep));
	      electronsAllScaleDown.push_back(LeptonsAll.at(iLep));
	      electronsAllRes.push_back(LeptonsAll.at(iLep));

	      scaleValueUnc = scenarioFormula.evaluateElectronScaleUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta());
	      resValueUnc   = scenarioFormula.evaluateElectronResolutionUnc(LeptonsAll.at(iLep).lepton4V_.Pt(),LeptonsAll.at(iLep).lepton4V_.Eta(),LeptonsAll.at(iLep).lepton4V_.E());

	      electronsAllScaleUp.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1+scaleValueUnc),							  
							      electronsAll.back().lepton4V_.Py()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.Pz()*(1+scaleValueUnc),
							      electronsAll.back().lepton4V_.E()*(1+scaleValueUnc));


	      electronsAllScaleDown.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(1-scaleValueUnc),							  
								electronsAll.back().lepton4V_.Py()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.Pz()*(1-scaleValueUnc),
								electronsAll.back().lepton4V_.E()*(1-scaleValueUnc));

	      float resUnc   = 1 + gRandom->Gaus(0,resValueUnc);
	      
	      electronsAllRes.back().lepton4V_.SetPxPyPzE(electronsAll.back().lepton4V_.Px()*(resUnc),							  
							  electronsAll.back().lepton4V_.Py()*(resUnc),
							  electronsAll.back().lepton4V_.Pz()*(resUnc),
							  electronsAll.back().lepton4V_.E()*(resUnc));

	      L_met_electronScaleUp   = L_met - electronsAllScaleUp.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronScaleDown = L_met - electronsAllScaleDown.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;
	      L_met_electronRes       = L_met - electronsAllRes.back().lepton4V_ + LeptonsAll.at(iLep).lepton4V_;


	    }

	  }

	  muonsIsoTightScaleUp   = dumpLeptons (muonsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightScaleDown = dumpLeptons (muonsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  muonsIsoTightRes       = dumpLeptons (muonsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
      	  muonsIsoTight          = dumpLeptons (muonsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  electronsIsoTightScaleUp   = dumpLeptons (electronsAllScaleUp, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightScaleDown = dumpLeptons (electronsAllScaleDown, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTightRes       = dumpLeptons (electronsAllRes, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);
	  electronsIsoTight          = dumpLeptons (electronsAll, leptonIsoCut_mu, leptonIsoCut_el, minPtLeptonCut);

	  for(size_t iJet = 0; iJet < RecoJetsAll.size(); iJet++){

	    RecoJetsAllScaleUp.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllScaleDown.push_back(RecoJetsAll.at(iJet));
	    RecoJetsAllRes.push_back(RecoJetsAll.at(iJet));

	    float jetScaleUnc   = scenarioFormula.evaluateJetScaleUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta());
	    float jetResUnc  = scenarioFormula.evaluateJetResolutionUnc(RecoJetsAll.at(iJet).jet4V_.Pt(),RecoJetsAll.at(iJet).jet4V_.Eta(),RecoJetsAll.at(iJet).jet4V_.E());

	    RecoJetsAllScaleUp.back().jet4V_.SetPxPyPzE(RecoJetsAllScaleUp.back().jet4V_.Px()*(1+jetScaleUnc), 
							RecoJetsAllScaleUp.back().jet4V_.Py()*(1+jetScaleUnc),
							RecoJetsAllScaleUp.back().jet4V_.Pz()*(1+jetScaleUnc),
							RecoJetsAllScaleUp.back().jet4V_.E()*(1+jetScaleUnc));

	    RecoJetsAllScaleDown.back().jet4V_.SetPxPyPzE(RecoJetsAllScaleDown.back().jet4V_.Px()*(1-jetScaleUnc), 
							  RecoJetsAllScaleDown.back().jet4V_.Py()*(1-jetScaleUnc),
							  RecoJetsAllScaleDown.back().jet4V_.Pz()*(1-jetScaleUnc),
							  RecoJetsAllScaleDown.back().jet4V_.E()*(1-jetScaleUnc));

	    float resUnc = 1 + gRandom->Gaus(0,jetResUnc);

	    RecoJetsAllRes.back().jet4V_.SetPxPyPzE(RecoJetsAllRes.back().jet4V_.Px()*(resUnc), 
						    RecoJetsAllRes.back().jet4V_.Py()*(resUnc),
						    RecoJetsAllRes.back().jet4V_.Pz()*(resUnc),
						    RecoJetsAllRes.back().jet4V_.E()*(resUnc));

	    L_met_jetScaleUp   = L_met - RecoJetsAllScaleUp.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetScaleDown = L_met - RecoJetsAllScaleDown.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	    L_met_jetRes       = L_met - RecoJetsAllRes.back().jet4V_ + RecoJetsAll.at(iJet).jet4V_;
	  }

	  RecoJetsScaleUp  = dumpJets (RecoJetsAllScaleUp, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	  RecoJetsScaleDown  = dumpJets (RecoJetsAllScaleDown, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	  RecoJetsRes  = dumpJets (RecoJetsAllRes, leptonsIsoTight, minJetCutPt, 999, CutList.at(iCut).jetPUID, minPtLeptonCutCleaning, matchingCone);
	
	}     

	// analysis with nominal objects
	if( passCutContainerSelection (CutList.at(iCut),
				       sampleName,
				       samplePosition,     
				       reader,
				       LeptonsAll,
				       leptonsIsoTight,
				       softMuons,				       
				       RecoJets,
				       trackEvent,
				       L_met,
				       minPtLeptonCut,
				       leptonIsoLooseCut,
				       vect,
				       finalStateString,eventFakeWeight)){

	  if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	    fillHistoWW(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList, 
			leptonsIsoTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met,"",eventFakeWeight);
	  
	    fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList2D, 
			  leptonsIsoTight,softMuons,RecoJets, 
			  GenJets,trackEvent,
			  L_met,"",eventFakeWeight);
	  }
	
	  else if (typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){

	    fillHistoWZ(analysisPlots, sampleName, samplePosition,
			CutList.at(iCut).cutLayerName,VariableList, 
			leptonsIsoTight,softMuons,RecoJets, 
			GenJets,trackEvent,
			L_met,"",eventFakeWeight);
	    
	    fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName,VariableList2D, 
			  leptonsIsoTight,softMuons,RecoJets, 
			  GenJets,trackEvent,
			  L_met,"",eventFakeWeight);
	  }
	}

	/// if perform sys analysis
	if(analysisPlots.getSystematics()){

	  // analysis scaling leptons Up
	  map<string,TH1F*> tempVect;

	  vector<leptonContainer> leptonAll (muonsAllScaleUp);
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  vector<leptonContainer> leptonTight (muonsIsoTightScaleUp);
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonScaleUp, 
			  "muScaleUp",eventFakeWeight);
	    
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonScaleUp,"muScaleUp",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonScaleUp, 
			  "muScaleUp",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonScaleUp,"muScaleUp",eventFakeWeight);
	    }
	  }
	    
	  
	  // analysis scaling muon down
	  leptonAll.clear();
	  leptonAll = muonsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());

	  leptonTight.clear();
	  leptonTight = muonsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonScaleDown, 
			  "muScaleDown",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonScaleDown, 
			  "muScaleDown",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonScaleDown,"muScaleDown",eventFakeWeight);
	    }
	  }
	
	  // scale electron up
	  leptonAll.clear();
	  leptonAll = electronsAllScaleUp;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());

	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleUp;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronScaleUp, 
			  "elScaleUp",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronScaleUp,"elScaleUp",eventFakeWeight);
	    }	  
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronScaleUp, 
			  "elScaleUp",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronScaleUp,"elScaleUp",eventFakeWeight);
	    }
	  }
	  
	  // scale electron down
	  leptonAll.clear();
	  leptonAll = electronsAllScaleDown;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightScaleDown;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronScaleDown, 
			  "elScaleDown",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronScaleDown,"elScaleDown",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronScaleDown, 
			  "elScaleDown",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronScaleDown,"elScaleDown",eventFakeWeight);
	    }
	  }
	  	  
	  // analysis smearing leptons (extra smearing)
	  leptonAll.clear();
	  leptonAll = muonsAllRes;
	  leptonAll.insert(leptonAll.end(),electronsAll.begin(),electronsAll.end());
	  leptonTight.clear();
	  leptonTight = muonsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),electronsIsoTight.begin(),electronsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());
	  
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_muonRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonRes, 
			  "muRes",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonRes,"muRes",eventFakeWeight);
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	  
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_muonRes, 
			  "muRes",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_muonRes,"muRes",eventFakeWeight);
	    }
	  }

	  
	  leptonAll.clear();
	  leptonAll = electronsAllRes;
	  leptonAll.insert(leptonAll.end(),muonsAll.begin(),muonsAll.end());
	  leptonTight.clear();
	  leptonTight = electronsIsoTightRes;
	  leptonTight.insert(leptonTight.end(),muonsIsoTight.begin(),muonsIsoTight.end());

	  sort(leptonAll.rbegin(),leptonAll.rend());
	  sort(leptonTight.rbegin(),leptonTight.rend());

	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 leptonAll,
					 leptonTight,
					 softMuons,
					 RecoJets,
					 trackEvent,
					 L_met_electronRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      
	      fillHistoWW(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronRes, 
			  "elRes",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronRes,"elRes",eventFakeWeight);
	    }
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      
	      fillHistoWZ(analysisPlots, 
			  sampleName, samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonTight, 
			  softMuons,
			  RecoJets, 
			  GenJets,
			  trackEvent,
			  L_met_electronRes, 
			  "elRes",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonTight,softMuons,RecoJets, 
			    GenJets,trackEvent,
			    L_met_electronRes,"elRes",eventFakeWeight);
	    }
	  }
	  

	  //// jets
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleUp,
					 trackEvent,
					 L_met_jetScaleUp,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	      fillHistoWW(analysisPlots, 
			  sampleName, 
			  samplePosition,
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight,
			  softMuons,
			  RecoJetsScaleUp, 
			  GenJets,
			  trackEvent,
			  L_met_jetScaleUp, 
			  "jetScaleUp",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, 
			    samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsScaleUp, 
			    GenJets,trackEvent,
			    L_met_jetScaleUp,"jetScaleUp",eventFakeWeight);
	    }	  
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){	  
	      fillHistoWZ(analysisPlots, 
			  sampleName, 
			  samplePosition,     
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight, 
			  softMuons,
			  RecoJetsScaleUp, 
			  GenJets,
			  trackEvent,
			  L_met_jetScaleUp, 
			  "jetScaleUp",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsScaleUp, 
			    GenJets,trackEvent,
			    L_met_jetScaleUp,"jetScaleUp",eventFakeWeight);

	    }
	  }

	  // analysis scaling jets down
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsScaleDown,
					 trackEvent,
					 L_met_jetScaleDown,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){
	    

	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){	 
	      fillHistoWW(analysisPlots, 
			  sampleName, 
			  samplePosition,     
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight, 
			  softMuons,
			  RecoJetsScaleDown, 
			  GenJets,
			  trackEvent,
			  L_met_jetScaleDown, 
			  "jetScaleDown",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsScaleDown, 
			    GenJets,trackEvent,
			    L_met_jetScaleDown,"jetScaleDown",eventFakeWeight);

	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){	 
	      fillHistoWZ(analysisPlots, 
			  sampleName, 
			  samplePosition,     
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight, 
			  softMuons,
			  RecoJetsScaleDown, 
			  GenJets,
			  trackEvent,
			  L_met_jetScaleDown, 
			  "jetScaleDown",eventFakeWeight);
	    
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsScaleDown, 
			    GenJets,trackEvent,
			    L_met_jetScaleDown,"jetScaleDown",eventFakeWeight);
	  
	    }
	  }

	  
	  if( passCutContainerSelection (CutList.at(iCut),
					 sampleName,
					 samplePosition,     
					 reader,
					 LeptonsAll,
					 leptonsIsoTight,
					 softMuons,
					 RecoJetsRes,
					 trackEvent,
					 L_met_jetRes,
					 minPtLeptonCut,
					 leptonIsoLooseCut,
					 tempVect,
					 finalStateString,eventFakeWeight)){

	    // analysis smearing jets
	    if(typeid(CutList.at(iCut)) == typeid(cutContainerWW)){
	  
	      fillHistoWW(analysisPlots, 
			  sampleName, 
			  samplePosition,     
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight, 
			  softMuons,
			  RecoJetsRes, 
			  GenJets,
			  trackEvent,
			  L_met_jetRes,
			  "jetRes",eventFakeWeight);
	      
	      fillHisto2DWW(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsRes, 
			    GenJets,trackEvent,
			    L_met_jetRes,"jetRes",eventFakeWeight);
	      
	    }
	    else if(typeid(CutList.at(iCut)) == typeid(cutContainerWZ)){
	      fillHistoWZ(analysisPlots, 
			  sampleName, 
			  samplePosition,     
			  CutList.at(iCut).cutLayerName, 
			  VariableList, 
			  leptonsIsoTight, 
			  softMuons,
			  RecoJetsRes, 
			  GenJets,
			  trackEvent,
			  L_met_jetRes,
			  "jetRes",eventFakeWeight);
	      
	      fillHisto2DWZ(analysisPlots, sampleName, samplePosition,
			    CutList.at(iCut).cutLayerName,VariableList2D, 
			    leptonsIsoTight,
			    softMuons,
			    RecoJetsRes, 
			    GenJets,trackEvent,
			    L_met_jetRes,"jetRes",eventFakeWeight);
	  
	    }
	  }	
	}
      }
    }
  }
}



/// template instance
template void loopOnEvents<cutContainerWW> (plotter & analysisPlots,
					    const string & sampleName,
					    const int    & samplePosition,
					    readTree* reader,
					    vector<cutContainerWW>    & CutList,
					    vector<variableContainer> & VariableList,
					    const bool   & usePuppiAsDefault,
					    const float  & minPtLeptonCut,
					    const float  & minPtLeptonCutCleaning,
					    const float  & leptonIsoCut_mu,
					    const float  & leptonIsoCut_el,
					    const float  & leptonIsoLooseCut,
					    const float  & matchingCone,
					    const float  & minJetCutPt,
					    map <string,TH1F*> & vect,
					    const string & finalStateString,
					    const string & scenarioString,
					    const string & fakeRateFile);


template void loopOnEvents<cutContainerWW> (plotter & analysisPlots,
					    const string & sampleName,
					    const int    & samplePosition,
					    readTree* reader,
					    vector<cutContainerWW>    & CutList,
					    vector<variableContainer> & VariableList,
					    vector<variableContainer2D> & VariableList2D,
					    const bool   & usePuppiAsDefault,
					    const float  & minPtLeptonCut,
					    const float  & minPtLeptonCutCleaning,
					    const float  & leptonIsoCut_mu,
					    const float  & leptonIsoCut_el,
					    const float  & leptonIsoLooseCut,
					    const float  & matchingCone,
					    const float  & minJetCutPt,
					    map <string,TH1F*> & vect,
					    const string & finalStateString,
					    const string & scenarioString,
					    const string & fakeRateFile);

template void loopOnEvents<cutContainerWZ>(plotter & analysisPlots,
					   const string & sampleName,
					   const int    & samplePosition,
					   readTree* reader,
					   vector<cutContainerWZ>    & CutList,
					   vector<variableContainer> & VariableList,
					   const bool   & usePuppiAsDefault,
					   const float  & minPtLeptonCut,
					   const float  & minPtLeptonCutCleaning,
					   const float  & leptonIsoCut_mu,
					   const float  & leptonIsoCut_el,
					   const float  & leptonIsoLooseCut,
					   const float  & matchingCone,
					   const float  & minJetCutPt,
					   map <string,TH1F*> & vect,
					   const string & finalStateString,
					   const string & scenarioString,
					   const string & fakeRateFile);


template void loopOnEvents<cutContainerWZ>(plotter & analysisPlots,
					   const string & sampleName,
					   const int    & samplePosition,
					   readTree* reader,
					   vector<cutContainerWZ>    & CutList,
					   vector<variableContainer> & VariableList,
					   vector<variableContainer2D> & VariableList2D,
					   const bool   & usePuppiAsDefault,
					   const float  & minPtLeptonCut,
					   const float  & minPtLeptonCutCleaning,
					   const float  & leptonIsoCut_mu,
					   const float  & leptonIsoCut_el,
					   const float  & leptonIsoLooseCut,
					   const float  & matchingCone,
					   const float  & minJetCutPt,
					   map <string,TH1F*> & vect,
					   const string & finalStateString,
					   const string & scenarioString,
					   const string & fakeRateFile);

