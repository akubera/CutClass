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

typedef void (*cut_action)(const Track& );

class CutList {
public:
  CutList();
  ~CutList();

  // Add cuts (with a name) which will be evaluated upon a 'Run' command
  void AddCut(const std::string& name, Cut&);
  void AddCut(Cut&);

  // Given some string which acts as a logic statement, when such a track returns 'true', run the action function provided
  void AddAction(const std::string& logic_stmt, cut_action);

  // Evaluate the track and run the associated action
  int Run(const Track& x);

  size_t Size();

  void Print() {
    for (  std::map<std::string, Cut*>::iterator it = _name_map.begin();
          it != _name_map.end();
          it++)
    {
      std::cout << it->first << " -> " << (void*)it->second << std::endl;
    }
  }

protected:

  // the list of cuts
  std::vector<Cut*> _cuts;

  // maps names of cuts to cuts
  std::map<std::string, Cut*> _name_map;

  // maps bitsequences (in form of 32-bit unsigned ints) to actions involving a track
  std::map<uint32_t, std::vector<void (*)(const Track&)> > _action_map;

};

