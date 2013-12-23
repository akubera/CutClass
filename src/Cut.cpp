/*
 * src/Cut.cpp
 *
 *
 */

#include "Cut.hpp"

Cut::Cut(const std::string& name, test_func function) :
  _index(0), _name(name), _test(function)
{
  
}

Cut::Cut(test_func function) :
  _index(0), _name(""), _test(function)
{
  
}

Cut::Cut(const std::string& name) :
  _index(0), _name(name)
{
  
}

Cut::~Cut() {
  for (size_t i = 0; i < _subcuts.size(); i++) {
    delete _subcuts[i];
  }
}

size_t
Cut::Size()  {
    return std::accumulate(_subcuts.begin(), _subcuts.end(), 1, [](size_t s,Cut* c){return s + c->Size();});
}

Cut::CutInserter
Cut::AddCut(Cut *c) {
  AddCutVoid(c);
  return Cut::CutInserter(this);
}


Cut::CutInserter& 
Cut::CutInserter::operator()(Cut *c) {
  _parent->AddCutVoid(c);
  return *this;
}

void
Cut::AddCutVoid(Cut *c) {
  std::cout << "Stuff : " << (void*)c << std::endl;
  if (this == c) {
    std::cerr << "Attempting to add a cut to itself - not a good idea. Aborting." << std::endl;
    exit(EXIT_FAILURE);
  }
  if (nullptr == c) {
    std::cerr << "Attempting to add a null pointer to a cut. Aborting." << std::endl;
    exit(EXIT_FAILURE);
  }
  _subcuts.push_back(c);  
}

