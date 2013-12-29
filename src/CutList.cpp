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
  // add the memory location of the cut to vector of cuts
  _cuts.push_back(&c);

  // map the name provided to index of the newly pushed back cut pointer
  _name_map[name] = _cuts.size()-1;

  // Do we really want to include the cut's original name, too?
  if ((c._name != "") && (c._name != name)) {
    std::cout << "Cut has a name : '" << c._name << "'" << std::endl;
    _name_map[c._name] = _cuts.size()-1;
  }

  // std::cout << "Adding Cut " << name << std::endl;


  // iterate through the cut's subcuts
  for(size_t i = 0; i < c._subcuts.size(); i++) {
    std::stringstream ss;
    ss << name << '.' << (i+1);
    // recursively call AddCut to each subcut
    AddCut(ss.str(), *c._subcuts[i]);
  }
}

void
CutList::AddCut(Cut& c) {
  if (c.Name() == "") {
    throw "ERROR : Adding Cut without an identifying name";
  }
  AddCut(c.Name(), c);
}

int
CutList::Run(const Track& x) {
  int res = 0;
//  std::cout << "\n[CutList::Run] (" << _cuts.size() << ")\n";
  // Loop through each cut and test track
  for (unsigned int i = 0; i <  _cuts.size(); i++) {
    bool yes = _cuts[i]->Run(x);
    res |= (yes << i);
  }
  // res is now a bitcode representing the positions of the _cuts list which the track passed

  // Loop through actions and run all which match the resulting pattern
  for ( std::map<uint32_t, std::vector<void (*)(const Track&)> >::iterator it = _action_map.begin();
        it != _action_map.end();
        it++)
        {
          bool works =  (it->first & res) == it->first;
//          std::cout << it->first << " & " << res << " = " << (it->first & res) << " ?= " << it->first << "  -> " << works << std::endl;
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
    std::map<std::string, unsigned short>::iterator found = _name_map.find(it);

    // std::vector<Cut*>::iterator found = std::find(_cuts.begin(), _cuts.end(), _name_map[it]);
    if (found == _name_map.end()) {
      std::cerr << "ERROR : Cut identified by '" << it << "' was not found." << std::endl; 
      throw std::exception();
    }
    // get the position of the vector
    // size_t position = found - _cuts.begin();
    size_t position = found->second;

    // bitwise OR to flip the bit at that position
    action_mask |= (0x01 << position);
    printf(" token : %s -> %lu  : %d\n",it.c_str(), position, action_mask);
    // std::cout << " token : " << it << ' ' << _name_map[it]  << ' '<< (size_t)() << std::endl;
  } while (ss.good());

  _action_map[action_mask].push_back(action);
}

