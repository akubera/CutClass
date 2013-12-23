
#pragma once

#include <iostream>


#include <cmath>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>



typedef class track Track;

struct track {
  float px;
  float py;
  float pz;
  float E;
  float m;
  int id;
  
  float eta() const {
    return 1.0/2.0 * log((E+pz)/(E-pz));
  };
  void print() const {
    printf("px %f py %f pz %f m %f E %f\n",px,py,pz,m,E);
  }
};

typedef bool (*pt_test)(Track);

template <int N, int D>
bool pt_greater(Track t) {
  float pt = sqrt(t.px*t.px+t.py*t.py);
  return (1.0f * N / D < pt);
};

// typedef class test_func TestFunc;

class test_func {
public:
  test_func(float f) : _bound(f) {};
  virtual bool operator() (const Track& t) = 0;
protected:
  float _bound;
};

class pt_greator : public test_func {
public:
  pt_greator(float f) : test_func(f) {};
  bool operator() (const Track& t) {
    return (_bound < sqrt(t.px*t.px+t.py*t.py));
  };
};


class eta_greator : public test_func {
public:
  eta_greator(float f) : test_func(f) {};
  bool operator() (const Track& t) {
    return (_bound < t.eta());
  };
};


class eta_less : public test_func {
public:
  eta_less(float f) : test_func(f) {};
  bool operator() (const Track& t) {
    return (_bound > t.eta());
  };
};


Track Generate();


extern gsl_rng *gRandomGenerator;

// typedef bool (*test_int_func)(int);
// typedef bool (*test_float_func)(float);
// 
// typedef class {
//   int id;
//   union {
//     test_int_func i_func;
//     test_float_func f_func;
//   };
// } test_func;



