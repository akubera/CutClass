/*
 *  src/CutList.cpp
 *
 *  Implementation of a list of cuts to perform on some data - 
 *    and actions to apply on data matching specific cut patterns.
 */
#include "CutList.hpp"

#include <sstream>
#include <algorithm>

#include <cstring>
#include <cstdlib>
#include <cassert>

template <typename T>
CutList<T>::CutList() {
  
}

template <typename T>
CutList<T>::~CutList() {
  for (auto cut : _cuts) {
//    delete cut;
  }
}

template <typename T>
void 
CutList<T>::AddCut(const std::string& name, Cut<T>& c) {
  // add the memory location of the cut to vector of cuts
  _cuts.push_back(&c);

  // map the name provided to index of the newly pushed back cut pointer
  _name_map[name] = _cuts.size()-1;

  // Do we really want to include the cut's original name, too?
  if ((c._name != "") && (c._name != name)) {
    std::cout << "Cut<T> " << _cuts.size() << " has a name : '" << c._name << "'" << std::endl;
    _name_map[c._name] = _cuts.size()-1;
  }

  // std::cout << "Adding Cut<T> " << name << std::endl;


  // iterate through the cut's subcuts
  for(size_t i = 0; i < c._subcuts.size(); i++) {
    // recursively call AddCut<T> to each subcut
    AddCut(name + std::to_string(i+1), *c._subcuts[i]);
  }
}

template <typename T>
void
CutList<T>::AddCut(Cut<T>& c) {
  if (c.Name() == "") {
    throw "ERROR : Adding Cut without an identifying name";
  }
  AddCut(c.Name(), c);
}

template <typename T>
int
CutList<T>::Run(const T& x) {
  int res = 0;
//  std::cout << "\n[CutList<T>::Run] (" << _cuts.size() << ")\n";
  // Loop through each cut and test track
  for (unsigned int i = 0; i <  _cuts.size(); i++) {
    bool yes = _cuts[i]->Run(x);
    res |= (yes << i);
  }
  // res is now a bitcode representing the positions of the _cuts list which the track passed

  // Loop through actions and run all which match the resulting pattern
  for (auto it : _action_map) {
    bool works =  (it.first & res) == it.first;
//          std::cout << it->first << " & " << res << " = " << (it->first & res) << " ?= " << it->first << "  -> " << works << std::endl;
    if (works) {
      // loop through the actions!
      for (auto action : it.second) {
        action(x);
      }
    }
  }

  return res;
}

template <typename T>
size_t 
CutList<T>::Size() {
  return std::accumulate(_cuts.begin(), _cuts.end(), 0, [](size_t s, Cut<T>* c){return s + c->Size();});
}

static char* uitoa(const unsigned int value, char * str, int base, size_t buff_size) {
    assert(buff_size > log(value)/log(base) + 1);
    // copy the value
    unsigned int num {value};

    for (int i = (int)buff_size-1; i >= 0; i--) {
        int ratio = (int)(num / (int)pow(base, i));
        str[buff_size - i-1] = ratio + '0';
        num %= (int)pow(base, i);
    }
    return str;
}

template <typename T>
void 
CutList<T>::AddAction(const std::string& logic_stmt, const cut_action<T> &action) {
  // copy the 'logic statement' into a stringstream to 
  //   read in tokens separated by whitespace
  std::stringstream ss(logic_stmt);
  
  // the resulting bitmask to be "found" when a track is tested
  uint32_t action_mask {0};
  std::cout << "Building condition '" << logic_stmt << "'" << std::endl;
  do {
    // read the next token into the iterator string 'it'
    std::string it;
    ss >> it;
    
    // find the position in the _cuts vector
    std::map<std::string, unsigned short>::iterator found = _name_map.find(it);
    
    // std::vector<Cut*>::iterator found = std::find(_cuts.begin(), _cuts.end(), _name_map[it]);
    if (found == _name_map.end()) {
      std::cerr << "ERROR : Cut<T> identified by '" << it << "' was not found." << std::endl; 
      throw std::exception();
    }
    // get the position of the vector
    // size_t position = found - _cuts.begin();
    size_t position = found->second;
    
    // bitwise OR to flip the bit at that position
    action_mask |= (0x01 << position);
    char action_buffer[33] = {0};
    uitoa(action_mask, action_buffer, 2, 8);
    printf(" token : %s -> %lu\tcurrent mask %s (%d)\n",it.c_str(), position, action_buffer, action_mask);
    // std::cout << " token : " << it << ' ' << _name_map[it]  << ' '<< (size_t)() << std::endl;
  } while (ss.good());

  _action_map[action_mask].push_back(action);
}
