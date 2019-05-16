#include "nanovalarray.h"

#include <cstdio>
#include <cstdlib>
//#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <valarray>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#define TEST_NO_MAIN
#include "acutest.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// For testing.
#include <limits>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

template <typename T>
static bool float_equals(T x, T y) {
  if (nanostl::fabs(x - y) < nanostl::numeric_limits<T>::epsilon()) {
    return true;
  }

  return false;
}

template <typename T>
static bool float_equals_by_eps(T x, T y, T eps) {
  if (nanostl::fabs(x - y) < eps) {
    return true;
  }

  return false;
}

// Ulps based equality check
// TODO(LTE): Consider nan, inf case.
// Based on this blog post: https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
template <typename T>
static bool float_equals_by_ulps(T x, T y, int max_ulp_diffs) {
  nanostl::IEEE754Float flt_x;
  nanostl::IEEE754Float flt_y;

  flt_x.f = x;
  flt_y.f = y;

  if (flt_x.bits.sign != flt_y.bits.sign) {
    // Check if +0/-0
    if ((flt_x.bits.exponent == 0) &&
        (flt_y.bits.exponent == 0) &&
        (flt_x.bits.mantissa == 0) &&
        (flt_y.bits.mantissa == 0)) {
      return true;
    }

    return false;
  }

  int diff = int(flt_x.ui) - int(flt_y.ui);

  // abs
  diff = (diff < 0) ? -diff : diff;

  if (diff <= max_ulp_diffs) {
    return true;
  }
  std::cout << "x: " << x << ", y: " << y << ", diff_ulps = " << diff << std::endl;

  return false;
}

extern "C" void test_valarray(void) {
  nanostl::valarray<int> v;

  TEST_CHECK(v.empty() == true);

  v.resize(1);
  v[0] = 2;

  nanostl::valarray<int> y;
  y.swap(v);

  TEST_CHECK(y[0] == 2);
  TEST_CHECK(v.size() == 0);


  {
    nanostl::valarray<float> inp(4);
    inp[0] = 0.1f;
    inp[1] = 1.0f;
    inp[2] = 2.2f;
    inp[3] = 3.11f;

    std::valarray<float> ref(4);
    ref[0] = 0.1f;
    ref[1] = 1.0f;
    ref[2] = 2.2f;
    ref[3] = 3.11f;

    nanostl::valarray<float> z = nanostl::sin(inp);
    std::valarray<float> z_ref = std::sin(ref);

    TEST_CHECK(float_equals_by_ulps(z[0], z_ref[0], 3));
    TEST_CHECK(float_equals_by_ulps(z[1], z_ref[1], 1));
    TEST_CHECK(float_equals_by_ulps(z[2], z_ref[2], 1));
    TEST_CHECK(float_equals_by_ulps(z[3], z_ref[3], 9));
  }
}
