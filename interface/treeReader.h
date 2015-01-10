#ifndef treeReader_h
#define treeReader_h

#include <map>
#include <vector>
#include <string>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"
#include "TBranch.h"
#include "TBranchElement.h"
#include "TLeaf.h"

using namespace std;

class treeReader
{
  public:
  
    treeReader (TTree *, bool verbosity = false) ;
    ~treeReader () ;
  
    void GetEntry (int iEvent) {m_tree->GetEntry (iEvent) ; } ;
    int GetEntries () {return m_tree->GetEntries () ; } ;
    
    vector<ROOT::Math::XYZVector>*  Get3V    (const string &name);
    vector<ROOT::Math::XYZTVector>* Get4V    (const string &name);
    vector<double>*                 GetDouble(const string &name);
    vector<float>*                  GetFloat (const string &name);
    vector<int>*                    GetInt   (const string &name);
    vector<bool>*                   GetBool  (const string &name);
    vector<string>*                 GetString(const string &name);

    ROOT::Math::XYZVector*  get3V    (const string &name);
    ROOT::Math::XYZTVector* get4V    (const string &name);
    double*                 getDouble(const string &name);
    float*                  getFloat (const string &name);
    int*                    getInt   (const string &name);
    bool*                   getBool  (const string &name);
    string*                 getString(const string &name);
    
  private:

    map <string, vector<ROOT::Math::XYZVector> * >  m_3Vvectors ;
    map <string, vector<ROOT::Math::XYZTVector> * > m_4Vvectors ;
    map <string, vector<double> * >                 m_Dvectors ;
    map <string, vector<float> * >                  m_Fvectors ;
    map <string, vector<int> * >                    m_Ivectors ;
    map <string, vector<bool> * >                   m_Bvectors ;
    map <string, vector<string> * >                 m_Svectors ;
    
    map <string, ROOT::Math::XYZVector * >  k_3Vvectors ;
    map <string, ROOT::Math::XYZTVector * > k_4Vvectors ;
    map <string, double * >                 k_Dvectors ;
    map <string, float * >                  k_Fvectors ;
    map <string, int * >                    k_Ivectors ;
    map <string, bool * >                   k_Bvectors ;
    map <string, string * >                 k_Svectors ;

    
    TTree * m_tree ;
    bool m_verbosity ;

} ;

#endif

