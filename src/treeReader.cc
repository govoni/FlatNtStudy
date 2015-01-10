#include "treeReader.h"

using namespace std ;

treeReader::treeReader (TTree * tree, bool verbosity) :
m_tree (tree),
m_verbosity (verbosity)
{

 TObjArray * br_list = m_tree->GetListOfBranches () ;
 TIter br_it (br_list) ;

 TBranch * iBranch ; 
 TBranchElement* bre ;
 TLeaf* Leaf;

 while ((iBranch = (TBranch *) br_it.Next ()))  {
  bre = (TBranchElement*) iBranch ;
  string bname = bre->GetClassName () ;      
  string branchTitle = iBranch->GetTitle();
  if(branchTitle.find("bsmReweight")!= string::npos) continue ;
  if(bname!=""){

   if (bname.find ("Event") != string::npos) continue ;
  
   if (!bname.find ("vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >")){
    if (m_verbosity)
     cout << "4V | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    m_4Vvectors[bre->GetName ()] = new vector<ROOT::Math::XYZTVector> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_4Vvectors[bre->GetName ()]) ;   
   }

   else if (!bname.find ("vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >") ){
    if (m_verbosity)
     cout << "3V | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    m_3Vvectors[bre->GetName ()] = new vector<ROOT::Math::XYZVector> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_3Vvectors[bre->GetName ()]) ;    
   }

   else if (!bname.find ("vector<bool>") ){
    if (m_verbosity)
     cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ; 
    m_Bvectors[bre->GetName ()] = new vector<bool> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_Bvectors[bre->GetName ()]) ; 
   }

   else if (!bname.find ("vector<int>") ){
    if (m_verbosity)
     cout << "IV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    m_Ivectors[bre->GetName ()] = new vector<int> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_Ivectors[bre->GetName ()]) ;    
   }

   else if (!bname.find ("vector<string>") ){
    if (m_verbosity)
     cout << "SV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    m_Svectors[bre->GetName ()] = new vector<string> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_Svectors[bre->GetName ()]) ; 
   }

   else if (!bname.find ("vector<float>") ){
    if (m_verbosity)
     cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ; 
    m_Fvectors[bre->GetName ()] = new vector<float> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_Fvectors[bre->GetName ()]) ; 
   }

   else if (!bname.find ("vector<double>") ){
    if (m_verbosity)
     cout << "DV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    m_Dvectors[bre->GetName ()] = new vector<double> ;
    m_tree->SetBranchAddress (bre->GetName (), &m_Dvectors[bre->GetName ()]) ;
   }

   else if (!bname.find ("ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > ")){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName() << "\n" ;

    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_4Vvectors[bre->GetName ()] = new  ROOT::Math::XYZTVector [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_4Vvectors[bre->GetName ()]) ;}
    else{ k_4Vvectors[bre->GetName ()] = new  ROOT::Math::XYZTVector ;m_tree->SetBranchAddress (bre->GetName (),k_4Vvectors[bre->GetName ()]) ;}
  
   }


    else if (!bname.find ("ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>* ")){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_3Vvectors[bre->GetName ()] = new  ROOT::Math::XYZVector [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_3Vvectors[bre->GetName ()]) ;}
    else{ k_3Vvectors[bre->GetName ()] = new  ROOT::Math::XYZVector ;m_tree->SetBranchAddress (bre->GetName (),k_3Vvectors[bre->GetName ()]) ;}

   }


   else if (!bname.find ("bool")  || !bname.find ("Bool_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Bvectors[bre->GetName ()] = new bool [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Bvectors[bre->GetName ()]) ;}
    else{ k_Bvectors[bre->GetName ()] = new bool ;     m_tree->SetBranchAddress (bre->GetName (),k_Bvectors[bre->GetName ()]) ;}
    }
  

   else if (!bname.find ("int")  || !bname.find ("Int_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Ivectors[bre->GetName ()] = new int [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Ivectors[bre->GetName ()]) ;}
    else{ k_Ivectors[bre->GetName ()] = new int ;     m_tree->SetBranchAddress (bre->GetName (),k_Ivectors[bre->GetName ()]) ;}
    }
  

  else if (!bname.find ("string")  || !bname.find ("TString") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Svectors[bre->GetName ()] = new string [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Svectors[bre->GetName ()]) ;}
    else{ k_Svectors[bre->GetName ()] = new string ;     m_tree->SetBranchAddress (bre->GetName (),k_Svectors[bre->GetName ()]) ;}
   
  }
  

  else if (!bname.find ("float")  || !bname.find ("Float_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;

    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }
   
    if(vectorx!=0 && vectory==0){k_Fvectors[bre->GetName ()] = new float [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Fvectors[bre->GetName ()]) ; }
    else if(vectorx==0 && vectory==0){ k_Fvectors[bre->GetName ()] = new float ;     m_tree->SetBranchAddress (bre->GetName (),k_Fvectors[bre->GetName ()]) ; }
 
   }
 
   else if (!bname.find ("double")  || !bname.find ("Double_t")  ){
      if (m_verbosity)
	cout << "DV | setting " << bre->GetName () << " for type : " << bre->GetClassName () << "\n" ;
    
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }
    if(vectorx!=0 && vectory==0){k_Dvectors[bre->GetName ()] = new double [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Dvectors[bre->GetName ()]) ;}
    else{ k_Dvectors[bre->GetName ()] = new double ;  m_tree->SetBranchAddress (bre->GetName (),k_Dvectors[bre->GetName ()]) ;}
   }
    
  }
 
  else{

    Leaf = iBranch->GetLeaf(iBranch->GetName());
    if(Leaf==0) continue;
    string bname = Leaf->GetTypeName();  

    if (!bname.find ("Event") ) continue ;
  
    else if (!bname.find ("vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >") ){
      if (m_verbosity)
	cout << "4V | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_4Vvectors[bre->GetName ()] = new vector<ROOT::Math::XYZTVector> ;
      m_tree->SetBranchAddress (bre->GetName (), &m_4Vvectors[bre->GetName ()]) ;      
    }

    else if (!bname.find ("vector<ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag> >") ){
      if (m_verbosity)
	cout << "3V | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_3Vvectors[bre->GetName ()] = new vector<ROOT::Math::XYZVector> ;
      m_tree->SetBranchAddress (bre->GetName (), &m_3Vvectors[bre->GetName ()]) ;      
    }

    else if (!bname.find ("vector<bool>") ){
      if (m_verbosity)
	cout << "IV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_Bvectors[bre->GetName ()] = new vector<bool>  ;
      m_tree->SetBranchAddress (bre->GetName (), &m_Bvectors[bre->GetName ()]) ;      
    }

    else if (!bname.find ("vector<int>") ){
      if (m_verbosity)
	cout << "IV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_Ivectors[bre->GetName ()] = new vector<int>  ;
      m_tree->SetBranchAddress (bre->GetName (), &m_Ivectors[bre->GetName ()]) ;      
    }

    else if (!bname.find ("vector<string>") ){
      if (m_verbosity)
	cout << "SV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_Svectors[bre->GetName ()] = new vector<string> ;
      m_tree->SetBranchAddress (bre->GetName (), &m_Svectors[bre->GetName ()]) ;     
    }

    else if (bname.find ("vector<float>") != string::npos){
      if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_Fvectors[bre->GetName ()] = new vector<float> ;
      m_tree->SetBranchAddress (bre->GetName (),&m_Fvectors[bre->GetName ()]) ;
    }

    else if (!bname.find ("vector<double>") ){
      if (m_verbosity)
	cout << "DV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
      m_Dvectors[bre->GetName ()] = new vector<double> ;
      m_tree->SetBranchAddress (bre->GetName (), &m_Dvectors[bre->GetName ()]) ;
    }

    else if (!bname.find ("ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >* ")){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;

    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_4Vvectors[bre->GetName ()] = new  ROOT::Math::XYZTVector [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_4Vvectors[bre->GetName ()]) ;}
    else{ k_4Vvectors[bre->GetName ()] = new  ROOT::Math::XYZTVector ;m_tree->SetBranchAddress (bre->GetName (),k_4Vvectors[bre->GetName ()]) ;}
  
    }


    else if (!bname.find ("ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,ROOT::Math::DefaultCoordinateSystemTag>* ")){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_3Vvectors[bre->GetName ()] = new  ROOT::Math::XYZVector [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_3Vvectors[bre->GetName ()]) ;}
    else{ k_3Vvectors[bre->GetName ()] = new  ROOT::Math::XYZVector ;m_tree->SetBranchAddress (bre->GetName (),k_3Vvectors[bre->GetName ()]) ;}
    }

   else if (!bname.find ("bool")  || !bname.find ("Bool_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Bvectors[bre->GetName ()] = new bool [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Bvectors[bre->GetName ()]) ;}
    else{ k_Bvectors[bre->GetName ()] = new bool ;     m_tree->SetBranchAddress (bre->GetName (),k_Bvectors[bre->GetName ()]) ;}
    }
  
  

   else if (!bname.find ("int")  || !bname.find ("Int_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Ivectors[bre->GetName ()] = new int [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Ivectors[bre->GetName ()]) ;}
    else{ k_Ivectors[bre->GetName ()] = new int ;     m_tree->SetBranchAddress (bre->GetName (),k_Ivectors[bre->GetName ()]) ;}
    }
  

  else if (!bname.find ("string")  || !bname.find ("TString") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
   
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }

    if(vectorx!=0 && vectory==0){k_Svectors[bre->GetName ()] = new string [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Svectors[bre->GetName ()]) ;}
    else{ k_Svectors[bre->GetName ()] = new string ;     m_tree->SetBranchAddress (bre->GetName (),k_Svectors[bre->GetName ()]) ;}
   
  }
  

  else if (!bname.find ("float")  || !bname.find ("Float_t") ){
    if (m_verbosity)
	cout << "FV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;

    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }
   
    if(vectorx!=0 && vectory==0){k_Fvectors[bre->GetName ()] = new float [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Fvectors[bre->GetName ()]) ; }
    else if(vectorx==0 && vectory==0){ k_Fvectors[bre->GetName ()] = new float ;     m_tree->SetBranchAddress (bre->GetName (),k_Fvectors[bre->GetName ()]) ; }
 
   }
 
   else if (!bname.find ("double")  || !bname.find ("Double_t")  ){
      if (m_verbosity)
	cout << "DV | setting " << bre->GetName () << " for type : " << Leaf->GetTypeName () << "\n" ;
    
    int foundFirst = branchTitle.find("/");
    int vectorx = 0 ;
    int vectory = 0 ;

    if(branchTitle.at(foundFirst-1)==']'){

      size_t foundUp = branchTitle.find_first_of("[");
      size_t foundDw = branchTitle.find_first_of("]");

      string npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);
      vectorx = atoi(npos.c_str());

      branchTitle.replace(foundUp,foundDw-foundUp+1,"");
      foundUp = branchTitle.find_first_of("[");
      foundDw = branchTitle.find_first_of("]");
      npos = branchTitle.substr(foundUp+1,foundDw-foundUp-1);

      vectory = atoi(npos.c_str());

    }
    if(vectorx!=0 && vectory==0){k_Dvectors[bre->GetName ()] = new double [vectorx]; m_tree->SetBranchAddress (bre->GetName (),k_Dvectors[bre->GetName ()]) ;}
    else{ k_Dvectors[bre->GetName ()] = new double ;  m_tree->SetBranchAddress (bre->GetName (),k_Dvectors[bre->GetName ()]) ;}
   }
    
  }

 } //PG loop over branches
 
 cout << " --> " << (m_3Vvectors.size () + m_4Vvectors.size () + m_Fvectors.size () + m_Dvectors.size () + m_Ivectors.size ()) +
                         (k_3Vvectors.size () + k_4Vvectors.size () + k_Fvectors.size () + k_Dvectors.size () + k_Ivectors.size ()) << " branches read\n" ;
 
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


treeReader::~treeReader () {

  for (map <string, vector<double> * >::const_iterator iMap = m_Dvectors.begin () ; iMap != m_Dvectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;
   
 
  for (map <string, vector<float> * >::const_iterator iMap = m_Fvectors.begin () ; iMap != m_Fvectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;
 
  for (map <string, vector<int> * >::const_iterator iMap = m_Ivectors.begin () ; iMap != m_Ivectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;

  for (map <string, vector<string> * >::const_iterator iMap = m_Svectors.begin () ; iMap != m_Svectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;
 
  for (map <string, vector<ROOT::Math::XYZTVector> * >::const_iterator iMap = m_4Vvectors.begin () ; iMap != m_4Vvectors.end () ;  ++iMap)
      if(iMap->second) delete iMap->second ;
 
  for (map <string, vector<ROOT::Math::XYZVector> * >::const_iterator iMap = m_3Vvectors.begin () ; iMap != m_3Vvectors.end (); ++iMap)
      if(iMap->second) delete iMap->second ;

  for (map <string, double * >::const_iterator iMap = k_Dvectors.begin () ; iMap != k_Dvectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;
 
  for (map <string, float * >::const_iterator iMap = k_Fvectors.begin () ; iMap != k_Fvectors.end () ; ++iMap)
      if(iMap->second)  delete iMap->second ;
 
  for (map <string, int * >::const_iterator iMap = k_Ivectors.begin () ; iMap != k_Ivectors.end () ; ++iMap)
      if(iMap->second) delete iMap->second ;

  for (map <string, string * >::const_iterator iMap = k_Svectors.begin () ; iMap != k_Svectors.end () ; ++iMap)
      if(iMap->second)   delete iMap->second ;
 
  for (map <string, ROOT::Math::XYZTVector * >::const_iterator iMap = k_4Vvectors.begin () ; iMap != k_4Vvectors.end () ;  ++iMap)
      if(iMap->second)  delete iMap->second ;
 
  for (map <string, ROOT::Math::XYZVector * >::const_iterator iMap = k_3Vvectors.begin () ; iMap != k_3Vvectors.end (); ++iMap)
      if(iMap->second)  delete iMap->second ;

} 


vector<ROOT::Math::XYZVector>* treeReader::Get3V(const string &name){
  map<string,vector<ROOT::Math::XYZVector> * >::const_iterator    it_3V  = m_3Vvectors.find(name);
  if (it_3V  != m_3Vvectors.end()  ) return m_3Vvectors[name];
  else return new vector<ROOT::Math::XYZVector>;
}

ROOT::Math::XYZVector* treeReader::get3V(const string &name){
  map<string,ROOT::Math::XYZVector * >::const_iterator    it_3V  = k_3Vvectors.find(name);
  if (it_3V  != k_3Vvectors.end()  ) return k_3Vvectors[name];
  else return new ROOT::Math::XYZVector;
}

vector<ROOT::Math::XYZTVector>* treeReader::Get4V(const string &name){
  map<string,vector<ROOT::Math::XYZTVector> * >::const_iterator   it_4V  = m_4Vvectors.find(name);
  if (it_4V  != m_4Vvectors.end()  ) return m_4Vvectors[name];
  else return new vector<ROOT::Math::XYZTVector>;
}

ROOT::Math::XYZTVector* treeReader::get4V(const string &name){
  map<string,ROOT::Math::XYZTVector * >::const_iterator   it_4V  = k_4Vvectors.find(name);
  if (it_4V  != k_4Vvectors.end()  ) return k_4Vvectors[name];
  else return new ROOT::Math::XYZTVector;
}

vector<double>* treeReader::GetDouble(const string &name){
  map<string,vector<double> * >::const_iterator  it_D  = m_Dvectors.find(name);
  if (it_D  != m_Dvectors.end()  ) return m_Dvectors[name];
  else return new vector<double>;
}

double* treeReader::getDouble(const string &name){
  map<string,double * >::const_iterator   it_D  = k_Dvectors.find(name);
  if (it_D  != k_Dvectors.end()  ) return k_Dvectors[name];
  else return new double;
}

vector<float>* treeReader::GetFloat(const string &name){
  map<string,vector<float> * >::const_iterator it_F  = m_Fvectors.find(name);
  if (it_F  != m_Fvectors.end()  ) return m_Fvectors[name];
  else return new vector<float>;
}

float* treeReader::getFloat(const string &name){
  map<string,float * >::const_iterator  it_F  = k_Fvectors.find(name);
  if (it_F  != k_Fvectors.end()  ) return k_Fvectors[name];
  else return new float;
}

vector<int>* treeReader::GetInt(const string &name){
  map<string,vector<int> * >::const_iterator it_I  = m_Ivectors.find(name);
  if (it_I  != m_Ivectors.end()  ) return m_Ivectors[name];
  else return new vector<int>;
}

int* treeReader::getInt(const string &name){
  map<string,int * >::const_iterator  it_I  = k_Ivectors.find(name);
  if (it_I  != k_Ivectors.end()  ) return k_Ivectors[name];
  else return new int;
}


vector<bool>* treeReader::GetBool(const string &name){
  map<string,vector<bool> * >::const_iterator it_I  = m_Bvectors.find(name);
  if (it_I  != m_Bvectors.end()  ) return m_Bvectors[name];
  else return new vector<bool>;
}

bool* treeReader::getBool(const string &name){
  map<string,bool * >::const_iterator  it_I  = k_Bvectors.find(name);
  if (it_I  != k_Bvectors.end()  ) return k_Bvectors[name];
  else return new bool;
}

vector<string>* treeReader::GetString(const string &name){
  map<string,vector<string> * >::const_iterator it_S  = m_Svectors.find(name);
  if (it_S  != m_Svectors.end()  ) return m_Svectors[name];
  else return new vector<string>;
}

string* treeReader::getString(const string &name){
  map<string,string * >::const_iterator  it_S  = k_Svectors.find(name);
  if (it_S  != k_Svectors.end()  ) return k_Svectors[name];
  else return new string;
}
