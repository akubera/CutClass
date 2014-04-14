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

  friend class CutList;

public:
  // Cut(const std::string& name, test_int_func);
  Cut(const std::string& name, test_func*);
  Cut(test_func*);
  // Cut(const std::string& name);

  virtual ~Cut();

  std::string Name() {return _name;};
  
  bool Run(const Track&);

  size_t Size();

  class CutInserter {
  public:
    CutInserter(Cut *c) : _parent(c) {};
    CutInserter(test_func *t_func) : _parent(new Cut(t_func)) {};
    virtual ~CutInserter() { };

    CutInserter& operator() (Cut *c);
    CutInserter& operator() (test_func *test);
    CutInserter& operator() (const std::string& name, test_func *test);

  protected:
    Cut* _parent;
  };

  CutInserter AddCut(Cut *c);
  CutInserter AddCut(test_func *t_func) {
    return AddCut(new Cut(t_func));
  };
  CutInserter AddCut(const std::string& str, test_func *t_func) {
    return AddCut(new Cut(str, t_func));
  };

protected:
  size_t _index;
  std::string _name;
  test_func* _test;

  std::vector<Cut*> _subcuts;

private:
  
  void AddCutVoid(Cut *c);

};

