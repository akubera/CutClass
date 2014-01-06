/*
 * src/CutList.hpp
 *
 * Definition of a list of cuts to be applied to some data
 */


#pragma once

#include "Cut.hpp"

#include <vector>
#include <string>
#include <functional>
#include <map>

template <typename T>
using cut_action = std::function<void(const T &)>;

template <typename T>
class CutList {
public:

public:
  CutList();
  ~CutList();
  
  // Add cuts (with a name) which will be evaluated upon a 'Run' command
  void AddCut(const std::string& name, Cut<T>&);
  void AddCut(Cut<T>&);

  // Given some string which acts as a logic statement, when such a track returns 'true', run the action function provided
  void AddAction(const std::string& logic_stmt, const cut_action<T> &action);

  // Evaluate the track and run the associated action
  int Run(const T& x);

  size_t Size();
  
  void Print() {
    for (  std::map<std::string, unsigned short>::iterator it = _name_map.begin();
          it != _name_map.end(); 
          it++)
    {
      std::cout << it->first << " -> " << it->second << std::endl;
    }
  }

protected:
  
  // the list of cuts
  std::vector<Cut<T>*> _cuts;

  // maps names to the index of the cut
  std::map<std::string, unsigned short> _name_map;
  
  // maps bitsequences (in form of 32-bit unsigned ints) to actions involving a track
  std::map<uint32_t, std::vector<cut_action<T>>> _action_map;

};

#include "CutList.tpp"
