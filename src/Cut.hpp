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

typedef struct {
  int id;
  union {
    test_int_func i_func;
    test_float_func f_func;
  };
} test_func;


/**
 * Cut
 *
 * Class which accepts a function pointer and returns a boolean 
 *
 */
class Cut {
public:
  // Cut(const std::string& name, test_int_func);
  Cut(const std::string& name, test_func);
  Cut(test_func);
  Cut(const std::string& name);

  virtual ~Cut();

  std::string Name() {return _name;};

  bool Run(float f) {
    return _test.f_func(f);
  }

  bool Run(int i) {
    return _test.i_func(i);
  }

  size_t Size();

  class CutInserter {
  public:
    CutInserter(Cut *c) : _parent(c) {};
    CutInserter& operator()(Cut *c);
    ~CutInserter() { std::cout << "~CutInserter\n"; };

  protected:
    Cut* _parent;
  };

  CutInserter AddCut(Cut *c);

protected:
  size_t _index;
  std::string _name;

  test_func _test;

  std::vector<Cut*> _subcuts;

private:
  
  void AddCutVoid(Cut *c);

};

