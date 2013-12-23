
#pragma once

#include <iostream>

#include <cmath>

typedef struct track Track;

struct track {
  float px;
  float py;
  float E;
  int id;
};

typedef bool (*pt_test)(Track);

template <int N, int D>
bool pt_greater(Track t) {
  float pt = sqrt(t.px*t.px+t.py*t.py);
  return (1.0f * N / D < pt);
};

struct pt_greator {
  pt_greator(float f) : _bound(f) {};
  bool operator() (Track t) {
    return (_bound < sqrt(t.px*t.px+t.py*t.py));
  };

protected:
  float _bound;
    
};

typedef bool (*test_int_func)(int);
typedef bool (*test_float_func)(float);

// typedef pt_greater<7, 2> pt_lt_3_5;
