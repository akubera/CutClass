/*
 *  src/CutList.cpp
 *
 *  Implementation of a list of cuts to perform on some data - 
 *    and actions to apply on data matching specific cut patterns.
 */

#include "CutList.hpp"

#include <sstream>
#include <algorithm>

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
  _name_map[name] = &c;
  std::vector<Cut*> &vc = c._subcuts;

  std::cout << "Adding Cut " << name << std::endl;

  for(size_t i = 0; i < vc.size(); i++) {
    Cut* cptr = vc[i];
    _cuts.push_back(cptr);
    if (cptr->_name == "") {
      std::stringstream ss;
      ss << name << '.' << (i+1);
      _name_map[ss.str()] = cptr;
    } else {
      _name_map[cptr->_name] = cptr;
    }
  }
}

void
CutList::AddCut(Cut& c) {
  if (c.Name() == "") {
    throw "ERROR : Adding Cut without an identifying name";
  }

  _cuts.push_back(&c);
  _name_map[c.Name()] = &c;
  std::vector<Cut*> &vc = c._subcuts;
  for (size_t i = 0; i < vc.size(); i++) {
    Cut* cptr = vc[i];
    _cuts.push_back(cptr);
    if (cptr->_name == "") {
      std::stringstream ss;
      ss << c.Name() << '.' << i;
      _name_map[ss.str()] = cptr;
      std::cout << "Adding : " << &c << " " << ss.str() << " -> " << cptr << std::endl;
    } else {
      _name_map[cptr->_name] = cptr;
    }
  }
}

int
CutList::Run(const Track& x) {
  int res = 0;
  std::cout << "\n[CutList::Run] (" << _cuts.size() << ")\n";
  // Loop through each cut and test track
  for (unsigned int i = 0; i <  _cuts.size(); i++) {
    bool yes = _cuts[i]->Run(x);
    res |= (yes << i);
    std::cout << "Running " << i << " :: " << yes << " -> " <<  res << std::endl;
  }
  // res is now a bitcode representing the positions of the _cuts list which the track passed

  std::cout << "Looping through actions\n";
  // Loop through actions and run all which match the resulting pattern
  for ( std::map<uint32_t, std::vector<void (*)(const Track&)> >::iterator it = _action_map.begin();
        it != _action_map.end();
        it++)
        {
          bool works =  (it->first & res) == it->first;
          std::cout << it->first << " & " << res << " = " << (it->first & res) << " ?= " << it->first << "  -> " << works << std::endl;
          if (works) {
            // loop through the actions!
            for (std::vector<void (*)(const Track&)>::iterator action = it->second.begin();
                           action != it->second.end(); action++) {
                             (*action)(x);
                   }
          }
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

void 
CutList::AddAction(const std::string& logic_stmt, void (*action)(const Track&)) {
  // copy the 'logic statement' into a stringstream to 
  //   read in tokens separated by whitespace
  std::stringstream ss(logic_stmt);
  
  // the resulting bitmask to be "found" when a track is tested
  uint32_t action_mask {0};
  
  do {
    // read the next token into the iterator string 'it'
    std::string it;
    ss >> it;
    
    // find the position in the _cuts vector
    // std::vector<Cut*>::iterator found = std::find(_cuts.begin(), _cuts.end(), _name_map[it]);
    // if (found == _cuts.end()) {
    //   std::cerr << "ERROR : Cut identified by '" << it << "' was not found." << std::endl; 
    //   throw std::exception();
    // }
    // get the position of the vector
    // size_t position = found - _cuts.begin();

    size_t position = 0;
    for (; position < _cuts.size(); position++) {
      if (_cuts[position] == _name_map[it]) {
        std::cout << "Found " << it << " at position " << position << std::endl;
        break;
      } else {
        std::cout << "Not at position " << position << " ("<< _cuts[position] << " != "<< _name_map[it] <<")\n";
      }
    }

    
    // bitwise OR to flip the bit at that position
    action_mask |= (0x01 << position);
    printf(" token : %s -> %lu  : %d\n",it.c_str(), position, action_mask);
    // std::cout << " token : " << it << ' ' << _name_map[it]  << ' '<< (size_t)() << std::endl;
  } while (ss.good());

  _action_map[action_mask].push_back(action);
}
