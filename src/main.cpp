
#include <iostream>

#include "main.hpp"
#include "Cut.hpp"

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
  c0.AddCut(new Cut(new eta_greator(2.0)));// (new eta_greator(5.0)) ;//(new eta_greator(8.0));
  std::cout << c0.Size() << std::endl;

  // Cut pt_cut("pt", new pt_greator(3.0));

  // pt_cut.AddCut(new pt_greator(6.0));
  // std::cout << c0.Size() << std::endl;

  // std::cout << c0.Run(1.0f) << ' ' << c0.Run(9.0f) << std::endl;

  std::cout << "It works!" << std::endl;
  return 0;
}

Track Generate() {
  Track res;

  res.px = gsl_ran_gaussian(gRandomGenerator, 25*25);
  res.py = gsl_ran_gaussian(gRandomGenerator, 25*25);
  res.pz = gsl_ran_gaussian(gRandomGenerator, 25*25);
  res.m =  T*(1+log(1/(1-gsl_ran_flat(gRandomGenerator, 0.0, 1.0))));
  res.E = abs(gsl_ran_gaussian(gRandomGenerator, 25*25));
  res.id = 0;
  return res;
}