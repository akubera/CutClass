/*
 *  src/CutList.cpp
 *
 *  Implementation of a list of cuts to perform on some data - 
 *    and actions to apply on data matching specific cut patterns.
 */

#include "CutList.hpp"



CutList::CutList() {
  
}


void 
CutList::AddCut(const std::string& name, Cut c) {
  _cuts.push_back(&c);
}

void
CutList::AddCut(Cut c) {
  if (c.Name() == "") {
    throw "ERROR : Adding Cut without an identifying name";
  }

  
}
  
int
CutList::Run(float x) {
  int res = 0;
  for (unsigned int i = 0; i <  _cuts.size(); i++) {
    res |= (_cuts[i]->Run(x) << i);
  }
  return res;
}
