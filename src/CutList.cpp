/*
 *  src/CutList.cpp
 *
 *  Implementation of a list of cuts to perform on some data - 
 *    and actions to apply on data matching specific cut patterns.
 */

#include "CutList.hpp"


CutList::CutList() {
  
}

CutList::~CutList() {
  for (size_t i = 0; i < _cuts.size(); i++) {
    // delete _cuts[i];
  }
}

void 
CutList::AddCut(const std::string& name, Cut& c) {
  _cuts.push_back(&c);
}

void
CutList::AddCut(Cut& c) {
  if (c.Name() == "") {
    throw "ERROR : Adding Cut without an identifying name";
  }
  _cuts.push_back(&c);  
}
  
int
CutList::Run(const Track& x) {
  int res = 0;
  std::cout << "\n[CutList::Run] ("<<_cuts.size()<<")\n";
  for (unsigned int i = 0; i <  _cuts.size(); i++) {
    std::cout << "\t" << _cuts[i]->Run(x) << "\n";
    res |= (_cuts[i]->Run(x) << i);
  }
  return res;
}

// Function used to accumulate the counts of all the cuts
static size_t _cut_count(size_t s, Cut *c) {
  return s + c->Size();
}

size_t 
CutList::Size() {
  return std::accumulate(_cuts.begin(), _cuts.end(), 0, _cut_count);
  // return std::accumulate(_cuts.begin(), _cuts.end(), 0, [](size_t s, Cut* c){return s + c->Size();});
}
