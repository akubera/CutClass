/*
 * src/CutList.hpp
 *
 *
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
