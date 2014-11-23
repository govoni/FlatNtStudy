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

/* example

[field]
var1 = value1
var2 = value2
var3 = value3

non accetto sbagli, devo sapere di che tipo sto leggendo il value


*/

using namespace std ;

struct record
{
  record (string name) : m_recordName (name) {} ;
  string                        m_recordName ;
  map<string, string> m_recordContent ;
} ;


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


class config
{
public:

  config (string filename) :
    m_filename (filename)
    {
      readfile () ;
    } ;
  
  ~config () {} ;

  void printContent () ;

  map<string, string> readStringValues (string recordName) ;
  map<string, float>       readFloatValues (string recordName) ;
  
private:

  void readfile () ;
  string         m_filename ;
  vector<record> m_content ;


} ;



#endif