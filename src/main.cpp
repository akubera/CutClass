
#include <iostream>

#include "main.hpp"
#include "Cut.hpp"
#include "CutList.hpp"

gsl_rng *gRandomGenerator;

// bool atest(float x) {
//   return x < 2.0;
// }
static float T = 220.0;

int main()
{
  // setup random number generator
  gRandomGenerator = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set (gRandomGenerator, 0.0);
  // test_func tes = {0};
  // tes.f_func = atest;

  Cut c0("eta", new eta_greator(1.0));

  std::cout << "RUNNING\n";

  c0.AddCut(new eta_greator(2.0))(new eta_greator(5.0))(new eta_greator(8.0));
  std::cout << "c0.Size : " << c0.Size() << std::endl;

  Cut pt_cut("pt", new pt_greator(3.0));

  pt_cut.AddCut(new pt_greator(6.0));
  std::cout << "pt_cut.Size : " << pt_cut.Size() << std::endl;


  CutList cuts;
  cuts.AddCut(c0);
  cuts.AddCut(pt_cut);
  Track track = Generate();
  track.print();
  std::cout << "Testing Random : " << cuts.Run(track) << std::endl;
  // std::cout << c0.Run(1.0f) << ' ' << c0.Run(9.0f) << std::endl;

  std::cout << "It works!" << std::endl;
  return 0;
}

Track Generate() {
  Track res;

  res.px = gsl_ran_gaussian(gRandomGenerator, 25*15);
  res.py = gsl_ran_gaussian(gRandomGenerator, 25*15);
  res.pz = gsl_ran_gaussian(gRandomGenerator, 25*15);
  double x = gsl_ran_flat(gRandomGenerator, 0.0, 1.0);
  // Boltzmann
  res.m =  T*(1+log(1/(1-x)));
  res.E = sqrt(res.m * res.m + res.px * res.px +res.py*res.py + res.pz*res.pz);
  res.id = 0;
  return res;
}
