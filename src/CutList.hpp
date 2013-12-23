/*
 * src/CutList.hpp
 *
 * Definition of a list of cuts to be applied to some data
 */


#pragma once

#include "Cut.hpp"

#include <vector>
#include <string>
#include <map>

class CutList {
public:
  CutList();
  ~CutList();
  
  // Add cuts (with a name) which will be evaluated upon a 'Run' command
  void AddCut(const std::string& name, Cut&);
  void AddCut(Cut&);

  // Given some string which acts as a logic statement, when such a track returns 'true', run the action function provided
  void AddAction(const std::string& logic_stmt, void (*action)(const Track&));

  // Evaluate the track and run the associated action
  int Run(const Track& x);

  size_t Size();

protected:
  std::vector<Cut*> _cuts;
  std::map<uint32_t, std::vector<void (*)(const Track&)> > _action_map;
};

