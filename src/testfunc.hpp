/*
 * File:   testfunc.hpp
 * Author: andrewkubera
 *
 * Created on April 14, 2014, 8:27 PM
 */

#ifndef TESTFUNC_HPP
#define	TESTFUNC_HPP

#include "main.hpp"

class TestFunc {
public:
  TestFunc(float f) : _bound(f) {};
  virtual bool operator() (const Track& t) = 0;
protected:
  float _bound;
};

class pt_greator : public TestFunc {
public:
  pt_greator(float f) : TestFunc(f) {};
  bool operator() (const Track& t) {
    return (_bound < sqrt(t.px*t.px+t.py*t.py));
  };
};


class eta_greator : public TestFunc {
public:
  eta_greator(float f) : TestFunc(f) {};
  bool operator() (const Track& t) {
    return (_bound < t.eta());
  };
};


class eta_less : public TestFunc {
public:
  eta_less(float f) : TestFunc(f) {};
  bool operator() (const Track& t) {
    return (_bound > t.eta());
  };
};




#endif	/* TESTFUNC_HPP */

