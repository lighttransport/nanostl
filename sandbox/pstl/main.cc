#ifndef NANOSTL_PSTL
#error "NANOSTL_PSTL must be defined"
#endif

#include "nanoexecution.h"
#include "nanovector.h"
#include "nanoalgorithm.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  nanostl::vector<double> a;
  a.resize(13);
  nanostl::fill(nanostl::execution::par_unseq, a.begin(), a.end(), 42.0);

  for (int i = 0; i < a.size(); i++) {
    printf("a[%d] = %g\n", i, a[i]);
  }

  return 0;
}
