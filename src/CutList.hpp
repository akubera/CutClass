/*
 * src/CutList.hpp
 *
 * Definition of a list of cuts to be applied to some data
 */


#pragma once

#include "Cut.hpp"

#include <vector>
#include <string>


class CutList {
public:
  CutList();
  
  void AddCut(const std::string& name, Cut);
  void AddCut(Cut);
  
  int Run(float);

protected:
  std::vector<Cut*> _cuts;

};
