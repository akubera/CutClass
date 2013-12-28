
#include <iostream>

#include "main.hpp"
#include "Cut.hpp"
#include "CutList.hpp"

#include <TFile.h>

// the GNU Scientific Library random number generator (global variable)
gsl_rng *gRandomGenerator;

// a "temperature" value for the Boltzmann mass distribution
static float T = 250;

TFile *file = new TFile("file.root", "RECREATE");

TH1F *h1 = new TH1F("h1", "Mass", 360, 0, 300);
TH1F *h2 = new TH1F("h2", "Pt", 360, 0, 50);

int
main()
{
  // setup random number generator
  gRandomGenerator = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set (gRandomGenerator, 0.0);

  // created a rapidity cut named "eta", which returns true if the passed track has a
  //  pseudo-rapidity greater than 0.1
  Cut c0("eta", new eta_greator(0.1));

  // add some other cuts acting on different ranges
  c0.AddCut("zab", new eta_greator(2.0))(new eta_greator(5.0))(new eta_greator(8.0));


  // Create a pt cut
  Cut pt_cut("pt", new pt_greator(3.0));

  // add another cut to the pt-cut group
  pt_cut.AddCut(new pt_greator(6.0));

  // add cuts to a cutlist
  CutList cuts;
  cuts.AddCut(c0);
  cuts.AddCut(pt_cut);


  cuts.AddAction("eta", add_to_histogram_eta_1);
  cuts.AddAction("pt zab", add_to_histogram_1);

  cuts.Print();

  // generate a random cut
  Track track = Generate();
  track.print();
  std::cout << "Testing Random : " << cuts.Run(track) << std::endl;
  for (int i = 0; i < 5e8; i++) {
    Track t = Generate();
//    std::cout << "Mass : " << t.m << std::endl;
//    cuts.Run(t);
  }
  // std::cout << c0.Run(1.0f) << ' ' << c0.Run(9.0f) << std::endl;

  std::cout << "It works!" << std::endl;
  h1->Write();
  h2->Write();

  file->Write();
  file->Close();
  delete file;
  return 0;
}


// Function to generate some particles (use Gaussian distributions)
Track Generate() {
  Track res;

  res.px = gsl_ran_gaussian(gRandomGenerator, 20);
  res.py = gsl_ran_gaussian(gRandomGenerator, 20);
  res.pz = gsl_ran_gaussian(gRandomGenerator, 4);
  double x = gsl_ran_flat(gRandomGenerator, 0.0, 1.0);
  // Boltzmann?
  res.m =  T*(1+log(1/(1-x)));
  res.E = sqrt(res.m * res.m + res.px * res.px +res.py*res.py + res.pz*res.pz);
  res.id = 0;

  h1->Fill(res.m);
  h2->Fill(res.pt());

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
