/*
test the plotter structure creation
-------------------

c++ -o test_04 `root-config --glibs --cflags` -lm test_04.cpp plotter.cc
./test_04

*/

#include <iostream>
#include <map>

#include "plotter.h"

using namespace std ;

int main (int argc, char ** argv)
{
  float lumi = 20 ;
  plotter analysisPlots (lumi) ;
  analysisPlots.addSample ("EWK_WW2j", 4.13649215685881443E-003, 100000, 1, 50) ; 
  analysisPlots.addLayerToSample ("EWK_WW2j", "total") ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "mjj", 200, 0., 2000.) ; 
  analysisPlots.addPlotToLayer ("EWK_WW2j", "total", "ptj1", 100, 0., 1000.) ; 
  analysisPlots.copyLayerInSample ("EWK_WW2j", "mjj", "total") ;
  analysisPlots.copySampleStructure ("QCD_WW2j", "EWK_WW2j", 1.06691296353271774E-003, 100000, 0, 8) ;
  analysisPlots.printStructure () ;
  return 0 ;
}  
