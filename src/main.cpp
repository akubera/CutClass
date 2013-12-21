
#include <iostream>

#include "Cut.hpp"

bool atest(float x) {
  return x < 2.0;
}


int main()
{
  test_func tes = {0};
  tes.f_func = atest;
  
  Cut c0("c0", tes);
  
  std::cout << c0.Run(1.0f) << ' ' << c0.Run(9.0f) << std::endl;
  
  std::cout << "It works!" << std::endl;
  return 0;
}

