/*
 * src/Cut.cpp
 *
 *
 */

#include "Cut.hpp"

Cut::Cut(const std::string& name, test_func function) :
  _index(0), _name(name), _test(function)
{
  
}

Cut::Cut(test_func function) :
  _index(0), _name(""), _test(function)
{
  
}

Cut::Cut(const std::string& name) :
  _index(0), _name(name)
{
  
}
