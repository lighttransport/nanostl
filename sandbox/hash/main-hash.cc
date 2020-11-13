#include "nanofunctional.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  auto h = nanostl::hash<int>{}(argc);
  auto f = nanostl::hash<float>{}(1.3f);
  auto d = nanostl::hash<double>{}(1.5);

  printf("argc = %llu\n", h);
  printf("f = %llu\n", f);
  printf("d = %llu\n", d);

  return 0;
}
