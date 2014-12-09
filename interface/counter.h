#ifndef counter_h
#define counter_h
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std ;
class counter {
 public:
  counter () {} ;
  ~counter () {} ;
  void addStage (string cut){ // put a map with cut string as key and a number of events after the cut
    if (m_counts.find (cut) != m_counts.end ()) return ;
    m_counts[cut] = 0. ;
    m_order.push_back (cut) ;
    return ;
  };
  float getCounts (string cut) { // get the number of events after the cut
    if (m_counts.find (cut) == m_counts.end ()) return -1. ;
    return m_counts[cut] ;
  };
  float getAbsEff (string cut) { // take the absolute efficiency respect to the zero position (ordered respct to the number of events
    if (m_counts.find (cut) == m_counts.end ()) return -1. ;
    return m_counts[cut] / m_counts[m_order.at (0)];
  };
  float getRelEff (string cut) { // find the relative with respct to the selection that is one position before in the map
    if (m_counts.find (cut) == m_counts.end ()) return -1. ;
    vector<string>::iterator pos = find (m_order.begin (), m_order.end (), cut) ;
    --pos ;
    return m_counts[cut] / m_counts[*pos];
  };
  void increase (string cut){ // increase by one unity
    if (m_counts.find (cut) == m_counts.end ()) {
      cout << "WARNING incrementing unexisting selection " << cut << endl ;
      return ;
    }
    ++m_counts[cut] ;
  }
  void printSummary () { // summary content of the map
    cout.setf (ios::fixed) ;
    cout << m_order.at (0) << "\t"
	 << setprecision (0) << getCounts (m_order.at (0)) << "\t"
	 << setprecision (2) << getAbsEff (m_order.at (0)) << "\t -- \n" ;
    for (size_t iStage = 1 ; iStage < m_order.size () ; ++iStage){
      cout << m_order.at (iStage) << "\t"
	   << setprecision (0) << getCounts (m_order.at (iStage)) << "\t"
	   << setprecision (2) << getAbsEff (m_order.at (iStage)) << "\t"
	   << setprecision (2) << getRelEff (m_order.at (iStage)) << "\n" ;
    }
    cout << setprecision (4) ;
  } ;
 private:
  map<string, float> m_counts ;
  vector<string> m_order ;
} ;
#endif
