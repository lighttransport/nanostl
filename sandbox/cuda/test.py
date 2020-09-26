import os

import pycuda.autoinit
import pycuda.driver as drv
import numpy

from pycuda.compiler import SourceModule

# It looks pycuda only accepts absolute path
include_dirs = [os.path.join(os.getcwd(), "../../include")]

mod = SourceModule("""
#include <nanovector.h>
#include <nanomap.h>
#include <nanostring.h>
#include <nanolimits.h>
//#include <vector>

extern "C" {
__global__ void multiply_them(float *dest, float *a, float *b)
{
  float infval = nanostl::numeric_limits<float>::infinity();
  float minval = nanostl::numeric_limits<float>::min();
  float denorm_minval = nanostl::numeric_limits<float>::denorm_min();
  float maxval = nanostl::numeric_limits<float>::max();
  float qnanval = nanostl::numeric_limits<float>::quiet_NaN();
  float snanval = nanostl::numeric_limits<float>::signaling_NaN();

  double dinfval = nanostl::numeric_limits<double>::infinity();
  double dminval = nanostl::numeric_limits<double>::min();
  double ddenorm_minval = nanostl::numeric_limits<double>::denorm_min();
  double dmaxval = nanostl::numeric_limits<double>::max();
  double dqnanval = nanostl::numeric_limits<double>::quiet_NaN();
  double dsnanval = nanostl::numeric_limits<double>::signaling_NaN();

  nanostl::map<nanostl::string, int> m;

  nanostl::vector<float> ab;
  ab.push_back(1.0f);
  printf("bora = %f\\n", ab[0]);
  int *ptr = new int[10];
  const int i = threadIdx.x;
  dest[i] = a[i] * b[i];
}
} // extern "C"
"""
, no_extern_c=1, include_dirs=include_dirs)


multiply_them = mod.get_function("multiply_them")

a = numpy.random.randn(400).astype(numpy.float32)
b = numpy.random.randn(400).astype(numpy.float32)

dest = numpy.zeros_like(a)
multiply_them(
        drv.Out(dest), drv.In(a), drv.In(b),
        block=(400,1,1), grid=(1,1))

print(dest-a*b)
