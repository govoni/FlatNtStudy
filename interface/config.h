#ifndef config_h
#define config_h
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
using namespace std ;
class record {
 public:
  record(){};
  ~record(){};
  record (string name) :
  m_recordName (name) {} ;
  std::string m_recordName ;
  std::map<std::string,std::string> m_recordContent ;
} ;
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
class config{ // class to read a config file for the plotting tool
 public:
  config (string filename) :
  m_filename (filename){
    readfile () ;
  } ;
  ~config () {} ;
  void printContent () ;
  map<string, string> readStringValues (string recordName) ; // read strings
  map<string, float> readFloatValues (string recordName) ; // read floats
 private:
  void readfile () ;
  string m_filename ;
  vector<record> m_content ;
} ;
#endif
