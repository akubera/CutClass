
#pragma once

#include <iostream>

#include "Cut.hpp"
#include "CutList.hpp"

#include <cmath>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

typedef struct track Track;

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
};

Track Generate();

extern gsl_rng *gRandomGenerator;

typedef Cut<Track> TrackCut;
typedef CutList<Track> TrackCutlist;
