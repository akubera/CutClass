
#include <iostream>

#include "main.hpp"
#include "Cut.hpp"
#include "CutList.hpp"

// the GNU Scientific Library random number generator (global variable)
gsl_rng *gRandomGenerator;

// a "temperature" value for the Boltzmann mass distribution
static float T = 250;

unsigned int pt_1_count = 0,
        pt_2_count = 0,
        pt_3_count = 0,
        pt_4_count = 0,
        pt_6_count = 0;


int
main()
{
  // setup random number generator
  gRandomGenerator = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set (gRandomGenerator, 0.0);

  // Create a pt cut
  Cut pt_cut("pt>3.0", new pt_greator(3.0));

  // add some more cuts to the pt-cut group
  //  (Cut::AddCut returns an inserter with operator() which
  //   continues to insert if given a name + function pair)
  pt_cut.AddCut("pt>4.0", new pt_greator(4.0))
               ("pt>6.0", new pt_greator(6.0))
               ("pt>2.0", new pt_greator(2.0))
               ("pt>1.0", new pt_greator(1.0));

  // create a cutlist
  CutList cuts;
  cuts.AddCut(pt_cut);

  cuts.AddAction("pt>3.0", action_pt_3_0);
  cuts.AddAction("pt>4.0", action_pt_4_0);
  cuts.AddAction("pt>1.0", action_pt_1_0);
  cuts.AddAction("pt>2.0", action_pt_2_0);
  cuts.AddAction("pt>6.0", action_pt_6_0);

  cuts.AddAction("pt>4.0 pt>2.0", action_pt_4_AND_2);

  
//  cuts.Print();

  // generate a random cut
  Track track = Generate();
  track.print();
  std::cout << "Testing Random : " << cuts.Run(track) << std::endl;
  for (int i = 0; i < 50; i++) {
    Track t = Generate();
//    std::cout << "Mass : " << t.m << std::endl;
    cuts.Run(t);
  }
  // std::cout << c0.Run(1.0f) << ' ' << c0.Run(9.0f) << std::endl;
  puts("");
  
std::cout << "Pt > 1.0 Count : " << pt_1_count << std::endl
          << "Pt > 2.0 Count : " << pt_2_count << std::endl
          << "Pt > 3.0 Count : " << pt_3_count << std::endl
          << "Pt > 4.0 Count : " << pt_4_count << std::endl
          << "Pt > 6.0 Count : " << pt_6_count << std::endl;



  std::cout << "It works!" << std::endl;
  return 0;
}


// Function to generate some particles (use Gaussian distributions)
Track Generate() {
  Track res;

  res.px = gsl_ran_gaussian(gRandomGenerator, 2);
  res.py = gsl_ran_gaussian(gRandomGenerator, 2);
  res.pz = gsl_ran_gaussian(gRandomGenerator, 4);
  double x = gsl_ran_flat(gRandomGenerator, 0.0, 1.0);
  // Boltzmann?
  res.m =  T*log(1/(1-x));
  res.E = sqrt(res.m * res.m + res.px * res.px +res.py*res.py + res.pz*res.pz);
  res.id = 0;
  return res;
}

void
add_to_histogram_1(const Track& track) {
  std::cout << "Adding track with pt : " << track.pt() << std::endl;
}

void
add_to_histogram_eta_1(const Track& track) {
  std::cout << "Track has eta > 0.1 : " << track.eta() << std::endl;
}

void action_pt_1_0(const Track& ) {
    pt_1_count++;
}

void action_pt_2_0(const Track& ) {
    pt_2_count++;
}

void action_pt_3_0(const Track& ) {
    pt_3_count++;
}

void action_pt_4_0(const Track& ) {
    pt_4_count++;
}

void action_pt_4_AND_2(const Track& ) {
    // do nothing!
}

void action_pt_6_0(const Track& ) {
    pt_6_count++;
}
