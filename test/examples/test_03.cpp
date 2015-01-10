/*
test the cfg file reader
-------------------

c++ -o test_03 `root-config --glibs --cflags` -lm test_03.cpp config.cc 
./test_03 test.cfg

test.cfg has this content: 

[first]
one 1
two 2

% this is a comment

[second]
one One
three  Three

[third]
name1
name2
name3

*/

#include <iostream>
#include <map>

#include "config.h"

using namespace std ;

int main (int argc, char ** argv)
{
  if (argc < 2) exit (1) ;
  
  config configFile (argv[1]) ;

  configFile.printContent () ;

  cout << " ---- ---- ----\n" ;
  map<string, string> outS = configFile.readStringValues ("second") ;
  for (map<string, string>::const_iterator iElement = outS.begin () ;
       iElement != outS.end () ; ++iElement)
    {
      cout << iElement->first << " = " << iElement->second << "\n" ;
    }   

  cout << " ---- ---- ----\n" ;
  map<string, float> outF = configFile.readFloatValues ("first") ;
  for (map<string, float>::const_iterator iElement = outF.begin () ;
       iElement != outF.end () ; ++iElement)
    {
      cout << iElement->first << " = " << iElement->second << "\n" ;
    }   

  cout << " ---- ---- ----\n" ;
  outS = configFile.readStringValues ("third") ;
  for (map<string, string>::const_iterator iElement = outS.begin () ;
       iElement != outS.end () ; ++iElement)
    {
      cout << iElement->first << " = " << iElement->second << "\n" ;
    }   

  configFile.readFloatValues ("unexisting") ;
  
  return 0 ;
}  
