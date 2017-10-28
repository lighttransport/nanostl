#include "nanovector.h"
#include "nanoalgorithm.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

int main(int argc, char **argv)
{
  nanostl::vector<int> v;

  assert(v.empty());

  v.push_back(1);
  assert(!v.empty());

  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);


  assert(v.at(0) == 1);
  assert(v[1] == 2);
  assert(v[2] == 3);
  assert(v[3] == 4);
  assert(v[4] == 5);

  v.resize(3);
  assert(v.at(0) == 1);
  assert(v[1] == 2);
  assert(v[2] == 3);

  v.resize(6);
  assert(v.at(0) == 1);
  assert(v[1] == 2);
  assert(v[2] == 3);

  assert(nanostl::min(1, 2) == 1);
  assert(nanostl::max(1, 2) == 2);

  std::cout << "size    : " << v.size() << std::endl;
  std::cout << "capacity: " << v.capacity() << std::endl;

  return EXIT_SUCCESS;
}
