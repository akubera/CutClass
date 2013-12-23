/*
 * src/Cut.hpp
 *
 *
 */

#pragma once

#include <vector>
#include <string>
#include <numeric>

#include <iostream>
#include <cstdlib>

#include "main.hpp"

// typedef bool (*test_func)(Track);

/**
 * Cut
 *
 * Class which accepts a function pointer and returns a boolean 
 *
 */
class Cut {
public:
  // Cut(const std::string& name, test_int_func);
  Cut(const std::string& name, test_func*);
  Cut(test_func*);
  Cut(const std::string& name);

  virtual ~Cut();

  std::string Name() {return _name;};

  // bool Run(float f) {
  //   return _test.f_func(f);
  // }
  // 
  // bool Run(int i) {
  //   return _test.i_func(i);
  // }
  
  bool Run(const Track&);

  size_t Size();

  class CutInserter {
  public:
    CutInserter(Cut *c) : _parent(c) {};
    CutInserter(test_func *t_func) : _parent(new Cut(t_func)) {};
    ~CutInserter() { std::cout << "~CutInserter\n"; };

    CutInserter& operator() (Cut *c);

  protected:
    Cut* _parent;
  };

  CutInserter AddCut(Cut *c);
  CutInserter AddCut(test_func *t_func) {
    return AddCut(new Cut(t_func));
  };

protected:
  size_t _index;
  std::string _name;
  test_func* _test;

  std::vector<Cut*> _subcuts;

private:
  
  void AddCutVoid(Cut *c);

};

