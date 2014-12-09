#include "config.h"

using namespace std ;

void config::readfile (){

  ifstream infile (m_filename) ;

  string line ;
  while (getline (infile, line)){
      istringstream iss (line) ;
      string dummy ;
      iss >> dummy ;
      if (dummy.size () < 2) continue ;
      size_t found = dummy.find ("%") ;
      if (found != string::npos) continue ;
      found = dummy.find ("[") ;
      if (found != string::npos) // this is the beginning of a record
      {
          dummy = dummy.substr (1, dummy.size () - 2) ;
          m_content.push_back (record (dummy)) ;
     }
     else // read the full line of inputs
     {
          string value = "default" ;
          iss >> value ;
          m_content.back ().m_recordContent[dummy] = value ;
     }
  }
}


// ---- ---- ---- ---- ---- ---- ---- ----
void config::printContent (){
  for (size_t iRecord = 0 ; iRecord < m_content.size () ; ++iRecord){
      cout << "[" << m_content.at (iRecord).m_recordName << "]\n" ;
      for (map<string, string>::const_iterator iElement = m_content.at (iRecord).m_recordContent.begin () ;
           iElement != m_content.at (iRecord).m_recordContent.end () ; ++iElement){
          cout << iElement->first << " = " << iElement->second << "\n" ;
        }   
    } 
}


// ---- ---- ---- ---- ---- ---- ---- ----

map<string, string> config::readStringValues (string recordName){
  for (size_t iRecord = 0 ; iRecord < m_content.size () ; ++iRecord){
      if (m_content.at(iRecord).m_recordName == recordName) return m_content.at (iRecord).m_recordContent ;
  }
  cout << "WARNING field " << recordName << " does not exist" << endl ;
  exit (1) ;
}


// ---- ---- ---- ---- ---- ---- ---- ----


map<string, float> config::readFloatValues (string recordName){
  for (size_t iRecord = 0 ; iRecord < m_content.size () ; ++iRecord){
      if (m_content.at (iRecord).m_recordName == recordName) {
          map<string, float> newMap ;
          for (map<string, string>::const_iterator iElement = m_content.at (iRecord).m_recordContent.begin () ;
               iElement != m_content.at (iRecord).m_recordContent.end () ; ++iElement){            
              newMap[iElement->first] = atof (iElement->second.c_str ()) ;
	  }
          return newMap ;  
        }
  }
  cout << "ERROR field " << recordName << " does not exist, exiting execution" << endl ;
  exit (1) ;
}
