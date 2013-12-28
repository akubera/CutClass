/*
 * src/Cut.cpp
 *
 *
 */

#include "Cut.hpp"

Cut::Cut(const std::string& name, test_func* function) :
  _index(0), _name(name), _test(function)
{

}

Cut::Cut(test_func* function) :
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


// Function used to accumulate the counts of all the cuts
static size_t _cut_count(size_t s, Cut *c) {
  return s + c->Size();
}

size_t
Cut::Size() {
  // accumulate, beginning with 1
  return std::accumulate(_subcuts.begin(), _subcuts.end(), 1, _cut_count);

  // Do not use lambda functions if we are not using c++11, instead use the external _cut_count function
//    return std::accumulate(_subcuts.begin(), _subcuts.end(), 1, [](size_t s,Cut* c){return s + c->Size();});
}

Cut::CutInserter
Cut::AddCut(Cut *c) {
  AddCutVoid(c);
  return Cut::CutInserter(this);
}

Cut::CutInserter& 
Cut::CutInserter::operator() (Cut *c) {
  _parent->AddCutVoid(c);
  return *this;
}

Cut::CutInserter&
Cut::CutInserter::operator() (test_func *test) {
  return (*this)(new Cut(test));
}


void
Cut::AddCutVoid(Cut *c) {
  // std::cout << "Adding Cut with _bound : " << c->_test->_bound << std::endl;
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

bool
Cut::Run(const Track& t) {
  // run the stored test function with the track provided
  return (*_test)(t);
}
