
#include <iostream>
#include <functional>
#include <random>

#include "main.hpp"
#include "testfunc.hpp"

// the GNU Scientific Library random number generator (global variable)
std::mt19937 gRandomGenerator; //  = new std::mt19937;
std::function<float()> track::get_gauss = std::bind(track::rnd_gauss, gRandomGenerator);

// a "temperature" value for the Boltzmann mass distribution
static float T = 250;

std::normal_distribution<> Track::rnd_gauss;
std::uniform_real_distribution<float> Track::rnd_flat(0, 1);


//template class Cut<Track>;


unsigned int pt_1_count = 0,
        pt_2_count = 0,
        pt_3_count = 0,
        pt_4_count = 0,
        pt_6_count = 0;

// generates a function which tests the pt of a particle against provided float
std::function<bool(const Track&)>
generate_pt_test(float f) {
    auto res = [f] (const Track &t) {return t.pt() > f;};
    return res;
}

int
main()
{
  // setup random number generator
//  gRandomGenerator = gsl_rng_alloc(gsl_rng_taus);
//  gsl_rng_set (gRandomGenerator, 0.0);
//  gRandomGenerator = new std::mt19937();


  // created a rapidity cut named "eta", which returns true if the passed track has a
  //  pseudo-rapidity greater than 0.1
  TrackCut c0("eta", eta_greator(0.1));

  // add some other cuts acting on different ranges
  c0.AddCut("zab>2", eta_greator(2.0))(eta_greator(5.0))(eta_greator(8.0));

  // Create a pt cut
//  TrackCut pt_cut("pt>3", pt_greator(3.0));

  // Create a pt cut
  TrackCut pt_cut("pt>3.0", generate_pt_test(3.0));

  // add some more cuts to the pt-cut group
  //  (Cut::AddCut returns an inserter with operator() which
  //   continues to insert if given a name + function pair)
  pt_cut.AddCut("pt>4.0", generate_pt_test(4.0))
               ("pt>6.0", generate_pt_test(6.0))
               ("pt>2.0", generate_pt_test(2.0))
               ("pt>1.0", generate_pt_test(1.0));

  // create a cutlist
  TrackCutlist cuts;
  cuts.AddCut(pt_cut);

//  cuts.AddAction("eta", add_to_histogram_eta_1);
//  cuts.AddAction("pt>3 zab>2", add_to_histogram_1);
//  cuts.AddAction("pt>3 eta", add_to_histogram_4);

//  cuts.AddAction("pt>3.0", action_pt_3_0);
//  cuts.AddAction("pt>4.0", action_pt_4_0);
//  cuts.AddAction("pt>1.0", action_pt_1_0);
//  cuts.AddAction("pt>2.0", action_pt_2_0);
//  cuts.AddAction("pt>6.0", action_pt_6_0);

  cuts.AddAction("pt>4.0 pt>2.0", [](const Track& ){ });

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

  res.px = Track::get_gauss();
  res.py = Track::get_gauss();
  res.pz = Track::get_gauss();
  double x = Track::rnd_flat(gRandomGenerator);
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
