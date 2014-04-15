
#pragma once

#include <iostream>

#include "Cut.hpp"
#include "CutList.hpp"

#include <cmath>
#include <random>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>

typedef struct track Track;

//extern gsl_rng *gRandomGenerator;
extern std::mt19937 gRandomGenerator;

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

  float pt() const {
    return sqrt(px*px + py*py);
  };

  void print() const {
    printf("px %f py %f pz %f m %f E %f\n",px,py,pz,m,E);
  }

  static std::normal_distribution<> rnd_gauss;
  static std::uniform_real_distribution<float> rnd_flat;

  static std::function<float()> get_gauss;
};

Track Generate();

typedef Cut<Track> TrackCut;
typedef CutList<Track> TrackCutlist;
