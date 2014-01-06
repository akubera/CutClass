/*
 * src/Cut.hpp
 *
 *
 */

#pragma once

#include <vector>
#include <string>
#include <numeric>
#include <functional>

#include <iostream>
#include <cstdlib>

template<typename T>
using test_func = std::function<bool(T const &)> const&;

template <typename _T>
class CutList;

/**
 * Cut
 *
 * Class which accepts a function pointer and returns a boolean 
 *
 */
template <typename T>
class Cut {

  friend class CutList<T>;

public:
  // Cut(const std::string& name, test_int_func);
//  Cut(const std::string& name, test_func<T>);
  Cut(const std::string& name, std::function<bool (T const&)> const&);
  Cut(const test_func<T>&);
  // Cut(const std::string& name);

  virtual ~Cut();

  std::string Name() {return _name;};
  
  bool Run(const T&);

  size_t Size();

  class CutInserter {
  public:
    CutInserter(Cut<T> *c) : _parent(c) {};
    CutInserter(const test_func<T>& t_func) : _parent(new Cut<T>(t_func)) {};
    virtual ~CutInserter() { };

    CutInserter& operator() (Cut<T> *c);
    CutInserter& operator() (const test_func<T>& test);
    CutInserter& operator() (const std::string& name, const test_func<T>& test);

  protected:
    Cut<T>* _parent;
  };

  CutInserter AddCut(Cut<T> *c);
  CutInserter AddCut(test_func<T> t_func) {
    return AddCut(new Cut<T>(t_func));
  };
  CutInserter AddCut(const std::string& str, const test_func<T>& t_func) {
    return AddCut(new Cut<T>(str, t_func));
  };

protected:
  size_t _index;
  std::string _name;
  test_func<T> _test;

  std::vector<Cut*> _subcuts;

private:
  
  void AddCutVoid(Cut *c);

};

