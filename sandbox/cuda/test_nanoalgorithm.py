import os

import pycuda.autoinit
import pycuda.driver as drv
import numpy

from pycuda.compiler import SourceModule

# It looks pycuda only accepts absolute path
include_dirs = [os.path.join(os.getcwd(), "../../include")]

mod = SourceModule(
    """
#include <nanovector.h>
#include <nanoalgorithm.h>

extern "C" {
__global__ void test_algorithm(float *dest, float *a, float *b)
{

  printf("max: %f, min: %f\\n",double(nanostl::max(*a, *b)),
                              double(nanostl::min(*a, *b)));
  nanostl::vector<float> v;
  v.push_back(*a);
  v.push_back(*b);

  printf("*max_element: %f, *min_element: %f\\n",
    double(*nanostl::max_element(v.begin(), v.end())),
    double(*nanostl::min_element(v.begin(), v.end())));

}
} // extern "C"
""",
    no_extern_c=1,
    include_dirs=include_dirs,
)


test_algorithm = mod.get_function("test_algorithm")

a = numpy.random.randn(4).astype(numpy.float32)
b = numpy.random.randn(4).astype(numpy.float32)

dest = numpy.zeros_like(a)
test_algorithm(drv.Out(dest), drv.In(a), drv.In(b), block=(4, 1, 1), grid=(1, 1))
