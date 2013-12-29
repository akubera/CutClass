
#include <iostream>

#include "main.hpp"
#include "Cut.hpp"
#include "CutList.hpp"

#include <TFile.h>

// the GNU Scientific Library random number generator (global variable)
gsl_rng *gRandomGenerator;

// a "temperature" value for the Boltzmann mass distribution
static float T = 250;

// Global pointers to ROOT objects
TFile *file;
TH1F *h1, *h2, *px_h, *py_h, *pz_h, *pt_h, *eta_histogram;
TH1F *pt30_h, *pt40_h;

int
main()
{
  // setup random number generator
  gRandomGenerator = gsl_rng_alloc(gsl_rng_taus);
  gsl_rng_set (gRandomGenerator, 0.0);


  // Open TFile
  file = new TFile("file.root", "RECREATE");

  // Setup Histograms
  h1 = new TH1F("h1", "Mass", 360, 0, 300);
  h2 = new TH1F("h2", "Pt", 360, 0, 50);

  px_h = new TH1F("px_h", "Px", 360, -50, 50);
  py_h = new TH1F("py_h", "Py", 360, -50, 50);
  pz_h = new TH1F("pz_h", "Pz", 360, -50, 50);
  pt_h = new TH1F("pt_h", "Pt", 360, 0, 100);

  pt30_h = new TH1F("pt30_h", "Pt (Cut above 30)", 360, 0, 100);
  pt40_h = new TH1F("pt40_h", "Pt (Cut above 40)", 360, 0, 100);

  eta_histogram = new TH1F("eta_histogram", "#eta", 360, -1.0, 1.0);

  // created a rapidity cut named "eta", which returns true if the passed track has a
  //  pseudo-rapidity greater than 0.1
  Cut c0("eta", new eta_greator(0.1));

  // add some other cuts acting on different ranges
  c0.AddCut("zab", new eta_greator(2.0))(new eta_greator(5.0))(new eta_greator(8.0));


  // Create a pt cut
  Cut pt_cut("pt>30", new pt_greator(30.0));

  // add another cut to the pt-cut group
  Cut pt_40_cut("pt>40", new pt_greator(40.0));
//  pt_cut.AddCut("pt>40", new pt_greator(40.0));

  // add cuts to a cutlist
  CutList cuts;
  cuts.AddCut(c0);
  cuts.AddCut(pt_cut);
  cuts.AddCut(pt_40_cut);


  cuts.AddAction("eta", add_to_histogram_eta_1);
  cuts.AddAction("pt>30", add_to_histogram_pt_30);
  cuts.AddAction("pt>40", add_to_histogram_pt_40);

  cuts.Print();

  // generate a random cut
  Track track = Generate();
  track.print();
  std::cout << "Testing Random : " << cuts.Run(track) << std::endl;
  for (int i = 0; i < 5e6; i++) {
    Track t = Generate();
//    std::cout << "Mass : " << t.m << std::endl;
    cuts.Run(t);
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
  res.m =  T*log(1/(1-x));
  res.E = sqrt(res.m * res.m + res.px * res.px +res.py*res.py + res.pz*res.pz);
  res.id = 0;

  h1->Fill(res.m);

  px_h->Fill(res.px);
  py_h->Fill(res.py);
  pz_h->Fill(res.pz);
  pt_h->Fill(res.pt());

  return res;
}

void
add_to_histogram_1(const Track& track) {
//  std::cout << "Adding track with pt : " << track.pt() << std::endl;
}

void
add_to_histogram_eta_1(const Track& track) {
//  std::cout << "Track has eta > 0.1 : " << track.eta() << std::endl;
  eta_histogram->Fill(track.eta());
}

void
add_to_histogram_pt_30(const Track& track) {
  pt30_h->Fill(track.pt());
}

void
add_to_histogram_pt_40(const Track& track) {
  pt40_h->Fill(track.pt());
}
