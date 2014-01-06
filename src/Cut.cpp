/*
 * src/Cut.cpp
 *
 *
 */

#include "Cut.hpp"

template <typename T> 
Cut<T>::Cut(std::string const& name, std::function<bool (T const&)> const& function) :
  _index(0), _name(name), _test(function)
{
  
}

template <typename T> 
Cut<T>::Cut(const test_func<T>& function) :
  _index(0), _name(""), _test(function)
{
  
}

template <typename T> 
Cut<T>::~Cut() {
  for (size_t i = 0; i < _subcuts.size(); i++) {
    delete _subcuts[i];
  }
}

template <typename T> 
size_t
Cut<T>::Size() {
  // Do not use lambda functions if we are not using c++11, instead use the external _cut_count function
  return std::accumulate(_subcuts.begin(), _subcuts.end(), 1, [](size_t s,Cut* c){return s + c->Size();});
}

template <typename T>
typename Cut<T>::CutInserter
Cut<T>::AddCut(Cut<T> *c) {
  AddCutVoid(c);
  return Cut<T>::CutInserter(this);
}

template <typename T>
typename Cut<T>::CutInserter& 
Cut<T>::CutInserter::operator() (Cut<T> *c) {
  _parent->AddCutVoid(c);
  return *this;
}

template <typename T>
typename Cut<T>::CutInserter& 
Cut<T>::CutInserter::operator() (const test_func<T>& test) {
  return (*this)(new Cut<T>(test));
}

template <typename T>
typename Cut<T>::CutInserter&
Cut<T>::CutInserter::operator() (const std::string& name, const test_func<T>& test) {
    return (*this)(new Cut<T>(name, test));
}

template <typename T>
void
Cut<T>::AddCutVoid(Cut *c) {
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

template <typename T>
bool
Cut<T>::Run(const T& t) {
  // run the stored test function with the track provided
  return _test(t);
}
