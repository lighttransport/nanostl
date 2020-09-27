#ifdef _MSC_VER
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include "nanoalgorithm.h"
#include "nanolimits.h"
#include "nanomap.h"
#include "nanomath.h"
#include "nanosstream.h"
#include "nanostring.h"
#include "nanoutility.h"
#include "nanovector.h"
#include "nanovalarray.h"

#include <cstdio>
#include <cstdlib>
//#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <valarray>

#include "nanoiterator.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

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

static void test_vector(void) {
  nanostl::vector<int> v;

  TEST_CHECK(v.empty() == true);

  v.push_back(1);
  TEST_CHECK(v.empty() == false);

  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);

  TEST_CHECK(v.at(0) == 1);
  TEST_CHECK(v[1] == 2);
  TEST_CHECK(v[2] == 3);
  TEST_CHECK(v[3] == 4);
  TEST_CHECK(v[4] == 5);

  v.resize(3);
  TEST_CHECK(v.at(0) == 1);
  TEST_CHECK(v[1] == 2);
  TEST_CHECK(v[2] == 3);

  v.resize(6);
  TEST_CHECK(v.at(0) == 1);
  TEST_CHECK(v[1] == 2);
  TEST_CHECK(v[2] == 3);

  v.erase(v.begin());
  TEST_CHECK(v.at(0) == 2);

  nanostl::vector<int> y;
  y.swap(v);

  TEST_CHECK(y.at(0) == 2);
  TEST_CHECK(v.size() == 0);
}

#if 0
static void test_valarray(void) {
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
#endif

static void test_iterator(void) {
  nanostl::vector<float> arr;
  arr.push_back(0.3f);
  arr.push_back(1.3f);
  arr.push_back(0.2f);
  arr.push_back(2.2f);

  std::vector<float> arr1;
  arr1.push_back(0.3f);
  arr1.push_back(1.3f);
  arr1.push_back(0.2f);
  arr1.push_back(2.2f);

  TEST_CHECK(nanostl::distance(arr.end(), arr.begin()) == std::distance(arr1.end(), arr1.begin()));

}

static void test_algorithm(void) {
  TEST_CHECK(nanostl::min(1, 2) == 1);
  TEST_CHECK(nanostl::max(1, 2) == 2);

  {
    nanostl::vector<float> arr;
    arr.push_back(0.3f);
    arr.push_back(1.3f);
    arr.push_back(0.2f);

    nanostl::vector<float>::iterator ret = nanostl::max_element(arr.begin(), arr.end());
    TEST_CHECK(nanostl::distance(arr.begin(), ret) == 1);
  }
}

static void test_string(void) {
  nanostl::string s("a");

  TEST_CHECK(s.empty() == false);

  TEST_CHECK(s[0] == 'a');
  TEST_CHECK(s.at(0) == 'a');
}

static void test_map(void) {
  nanostl::map<nanostl::string, int> m;

  TEST_CHECK(m.empty());

  m["a"] = 1;
  m["b"] = 2;

  TEST_CHECK(m["a"] == 1);
  TEST_CHECK(m["b"] == 2);
}

static void test_limits(void) {
  TEST_CHECK(nanostl::numeric_limits<char>::min() ==
             std::numeric_limits<char>::min());
  TEST_CHECK(nanostl::numeric_limits<char>::max() ==
             std::numeric_limits<char>::max());

  TEST_CHECK(nanostl::numeric_limits<short>::min() ==
             std::numeric_limits<short>::min());
  TEST_CHECK(nanostl::numeric_limits<short>::max() ==
             std::numeric_limits<short>::max());

  TEST_CHECK(nanostl::numeric_limits<int>::min() ==
             std::numeric_limits<int>::min());
  TEST_CHECK(nanostl::numeric_limits<int>::max() ==
             std::numeric_limits<int>::max());

  TEST_CHECK(float_equals(double(nanostl::numeric_limits<float>::min()),
                          double(std::numeric_limits<float>::min())));
  TEST_CHECK(float_equals(double(nanostl::numeric_limits<float>::max()),
                          double(std::numeric_limits<float>::max())));

  TEST_CHECK(float_equals(double(nanostl::numeric_limits<float>::denorm_min()),
                          double(std::numeric_limits<float>::denorm_min())));

  TEST_CHECK(float_equals(nanostl::numeric_limits<double>::min(),
                          std::numeric_limits<double>::min()));
  TEST_CHECK(float_equals(nanostl::numeric_limits<double>::max(),
                          std::numeric_limits<double>::max()));

  TEST_CHECK(float_equals(nanostl::numeric_limits<double>::denorm_min(),
                          std::numeric_limits<double>::denorm_min()));
}

static void test_math_func1(void) {
  TEST_CHECK(float_equals(nanostl::ceil(0.0f), std::ceil(0.0f)));

  TEST_CHECK(float_equals(nanostl::ceil(1.0f / 2.0f), std::ceil(1.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(2.0f / 2.0f), std::ceil(2.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(3.0f / 2.0f), std::ceil(3.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(4.0f / 2.0f), std::ceil(4.0f / 2.0f)));

  TEST_CHECK(float_equals(nanostl::ceil(0.0f / 2.0f), std::ceil(0.0f / 2.0f)));

  TEST_CHECK(
      float_equals(nanostl::ceil(-1.0f / 2.0f), std::ceil(-1.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::ceil(-2.0f / 2.0f), std::ceil(-2.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::ceil(-3.0f / 2.0f), std::ceil(-3.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::ceil(-4.0f / 2.0f), std::ceil(-4.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::ceil(-5.0f / 2.0f), std::ceil(-5.0f / 2.0f)));

  TEST_CHECK(
      float_equals(nanostl::floor(0.0f / 2.0f), std::floor(0.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(1.0f / 2.0f), std::floor(1.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(2.0f / 2.0f), std::floor(2.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(3.0f / 2.0f), std::floor(3.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(4.0f / 2.0f), std::floor(4.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(5.0f / 2.0f), std::floor(5.0f / 2.0f)));

  TEST_CHECK(
      float_equals(nanostl::floor(-0.0f / 2.0f), std::floor(-0.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(-1.0f / 2.0f), std::floor(-1.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(-2.0f / 2.0f), std::floor(-2.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(-3.0f / 2.0f), std::floor(-3.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(-4.0f / 2.0f), std::floor(-4.0f / 2.0f)));
  TEST_CHECK(
      float_equals(nanostl::floor(-5.0f / 2.0f), std::floor(-5.0f / 2.0f)));

  TEST_CHECK(nanostl::isfinite(std::numeric_limits<float>::max()) ==
             std::isfinite(std::numeric_limits<float>::max()));
  TEST_CHECK(nanostl::isfinite(std::numeric_limits<float>::infinity()) ==
             std::isfinite(std::numeric_limits<float>::infinity()));

  TEST_CHECK(nanostl::isnan(0.0f) == false);
  TEST_CHECK(nanostl::isnan(1.0f) == false);
  TEST_CHECK(nanostl::isnan(std::numeric_limits<float>::quiet_NaN()) == true);

  const float pos_inf_f = std::numeric_limits<float>::infinity();
  const double pos_inf_d = std::numeric_limits<double>::infinity();
  const float nan_f = std::numeric_limits<float>::quiet_NaN();
  const double nan_d = std::numeric_limits<double>::quiet_NaN();
  const float denormal_f = nanostl::numeric_limits<float>::min() / 2.0f;
  const double denormal_d = nanostl::numeric_limits<double>::min() / 2.0;

  TEST_CHECK(nanostl::isfinite(nan_f) == false);
  TEST_CHECK(nanostl::isfinite(pos_inf_f) == false);
  TEST_CHECK(nanostl::isfinite(0.0f) == true);
  TEST_CHECK(nanostl::isfinite(denormal_f) == true);

  TEST_CHECK(nanostl::isfinite(nan_d) == false);
  TEST_CHECK(nanostl::isfinite(pos_inf_d) == false);
  TEST_CHECK(nanostl::isfinite(0.0) == true);
  TEST_CHECK(nanostl::isfinite(denormal_d) == true);

  TEST_CHECK(nanostl::isinf(nan_f) == false);
  TEST_CHECK(nanostl::isinf(1.0f) == false);
  TEST_CHECK(nanostl::isinf(pos_inf_f) == true);

  TEST_CHECK(nanostl::isinf(nan_d) == false);
  TEST_CHECK(nanostl::isinf(1.0) == false);
  TEST_CHECK(nanostl::isinf(pos_inf_d) == true);

  TEST_CHECK(nanostl::isnan(nan_f) == true);
  TEST_CHECK(nanostl::isnan(pos_inf_f) == false);
  TEST_CHECK(nanostl::isnan(denormal_f) == false);
  TEST_CHECK(nanostl::isnan(0.0f) == false);
  TEST_CHECK(nanostl::isnan(pos_inf_f - pos_inf_f) == true);

  TEST_CHECK(nanostl::isnan(nan_d) == true);
  TEST_CHECK(nanostl::isnan(pos_inf_d) == false);
  TEST_CHECK(nanostl::isnan(denormal_d) == false);
  TEST_CHECK(nanostl::isnan(0.0) == false);
  TEST_CHECK(nanostl::isnan(pos_inf_d - pos_inf_d) == true);

  TEST_CHECK(std::isnormal(nan_f) == false);
  TEST_CHECK(std::isnormal(pos_inf_f) == false);
  TEST_CHECK(std::isnormal(0.0f) == false);
  TEST_CHECK(std::isnormal(1.0f) == true);
  TEST_CHECK(std::isnormal(nanostl::numeric_limits<float>::min() / 2.0f) ==
             false);

  TEST_CHECK(std::isnormal(nan_d) == false);
  TEST_CHECK(std::isnormal(pos_inf_d) == false);
  TEST_CHECK(std::isnormal(0.0) == false);
  TEST_CHECK(std::isnormal(1.0) == true);
  TEST_CHECK(std::isnormal(nanostl::numeric_limits<double>::min() / 2.0) ==
             false);
}

static void test_math_exp(void) {

  TEST_CHECK(float_equals(nanostl::exp(0.0f), std::exp(0.0f)));
  TEST_CHECK(float_equals_by_ulps(nanostl::exp(1.0f), std::exp(1.0f), 24));
  TEST_CHECK(float_equals_by_ulps(nanostl::exp(0.1f), std::exp(0.1f), 10));
  TEST_CHECK(float_equals_by_ulps(nanostl::exp(0.01f), std::exp(0.01f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::exp(3.33f), std::exp(3.33f), 86));
  TEST_CHECK(float_equals_by_ulps(nanostl::exp(13.33f), std::exp(13.33f), 29));
}

static void test_math_log(void) {
  //std::cout << "log(0) = " << nanostl::log(0.0f) << std::endl;

  TEST_CHECK(float_equals_by_ulps(nanostl::log(0.0f), std::log(0.0f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::log(1.0f), std::log(1.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log(0.1f), std::log(0.1f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log(0.01f), std::log(0.01f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log(3.33f), std::log(3.33f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::log(13.33f), std::log(13.33f), 0));
}

static void test_math_log10(void) {
  //std::cout << "log10(0) = " << nanostl::log10(0.0f) << std::endl;

  TEST_CHECK(float_equals_by_ulps(nanostl::log10(0.0f), std::log10(0.0f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::log10(1.0f), std::log10(1.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log10(0.1f), std::log10(0.1f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log10(0.01f), std::log10(0.01f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::log10(3.33f), std::log10(3.33f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::log10(13.33f), std::log10(13.33f), 1));
}

static void test_math_cos(void) {
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.0f), std::cos(0.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(1.0f), std::cos(1.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.1f), std::cos(0.1f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.01f), std::cos(0.01f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(3.33f), std::cos(3.33f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(13.33f), std::cos(13.33f), 0));
}

static void test_math_sin(void) {
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(0.0f), std::sin(0.0f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(1.0f), std::sin(1.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(0.1f), std::sin(0.1f), 3));
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(0.01f), std::sin(0.01f), 10));
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(3.33f), std::sin(3.33f), 2));
  TEST_CHECK(float_equals_by_ulps(nanostl::sin(13.33f), std::sin(13.33f), 0));
}

static void test_math_sqrt(void) {

  TEST_CHECK(float_equals_by_ulps(nanostl::sqrt(0.0f), std::sqrt(0.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::sqrt(1.0f), std::sqrt(1.0f), 0));

  // TODO(LTE): Use tighter eps bounds.
  TEST_CHECK(float_equals_by_eps(nanostl::sqrt(0.01f), std::sqrt(0.01f), 0.01f));
  TEST_CHECK(float_equals_by_eps(nanostl::sqrt(0.1f), std::sqrt(0.1f), 0.001f));
  TEST_CHECK(float_equals_by_eps(nanostl::sqrt(0.01f), std::sqrt(0.01f), 0.001f));
  TEST_CHECK(float_equals_by_eps(nanostl::sqrt(3.33f), std::sqrt(3.33f), 0.0001f));
  TEST_CHECK(float_equals_by_eps(nanostl::sqrt(13.33f), std::sqrt(13.33f), 0.0001f));
}

static void test_math_erf(void) {

  TEST_CHECK(float_equals_by_ulps(nanostl::erf(0.0f), std::erf(0.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::erf(1.0f), std::erf(1.0f), 5));

  TEST_CHECK(float_equals_by_ulps(nanostl::erf(3.33f), std::erf(3.33f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::erf(13.33f), std::erf(13.33f), 0));

  // TODO(LTE): Use eps bounds.
  TEST_CHECK(float_equals_by_ulps(nanostl::erf(0.1f), std::erf(0.1f), 43));
  TEST_CHECK(float_equals_by_ulps(nanostl::erf(0.01f), std::erf(0.01f), 1043));
}

static void test_math_erfc(void) {

  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(0.0f), std::erfc(0.0f), 0));

  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(0.1f), std::erfc(0.1f), 5));
  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(0.01f), std::erfc(0.01f), 16));
  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(13.33f), std::erfc(13.33f), 0));

  // TODO(LTE): Use eps bounds.
  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(1.0f), std::erfc(1.0f), 19));
  TEST_CHECK(float_equals_by_ulps(nanostl::erfc(3.33f), std::erfc(3.33f), 341679));
}

static void test_math_fmin(void) {

  float xn = std::numeric_limits<float>::quiet_NaN();
  float yn = std::numeric_limits<float>::signaling_NaN();

  float pinf = std::numeric_limits<float>::infinity();

  float x = 1.0f;
  float y = 2.0f;

  TEST_CHECK(float_equals_by_ulps(nanostl::fmin(x, y), 1.0f, 0));

  // (NaN, inf) -> inf
  TEST_CHECK(nanostl::isinf(nanostl::fmin(xn, pinf)) == std::isinf(std::fmin(xn, pinf)));

  // (2, NaN) -> 2
  TEST_CHECK(!nanostl::isnan(nanostl::fmin(y, xn)));

  // (NaN, NaN) -> NaN
  TEST_CHECK(nanostl::isnan(nanostl::fmin(yn, xn)));
}

// ierf is not present in std::math
//static void test_math_ierf(void) {
//
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(0.0f), std::ierf(0.0f), 0));
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(1.0f), std::ierf(1.0f), 5));
//
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(3.33f), std::ierf(3.33f), 1));
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(13.33f), std::ierf(13.33f), 0));
//
//  // TODO(LTE): Use eps bounds.
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(0.1f), std::ierf(0.1f), 43));
//  TEST_CHECK(float_equals_by_ulps(nanostl::ierf(0.01f), std::ierf(0.01f), 1043));
//}

static void test_float_nan(void) {

  float qnan = nanostl::numeric_limits<float>::quiet_NaN();

  nanostl::IEEE754Float flt; flt.f = qnan;
  TEST_CHECK(flt.bits.exponent == 255);
  TEST_CHECK((flt.bits.mantissa & (1 << 22)));

  float snan = nanostl::numeric_limits<float>::signaling_NaN();
  flt.f = snan;
  TEST_CHECK(flt.bits.exponent == 255);
  TEST_CHECK((flt.bits.mantissa & (1 << 22)) == 0);
  TEST_CHECK(flt.bits.mantissa != 0);

}

static void test_double_nan(void) {

  double qnan = nanostl::numeric_limits<double>::quiet_NaN();

  nanostl::IEEE754Double flt; flt.f = qnan;
  TEST_CHECK(flt.bits.exponent == 2047);
  TEST_CHECK((flt.bits.mantissa & (1ull << 51)) != 0);
  // qnanbit: 0008000000000000

  //printf("%016llx\n", flt.ull);
  //printf("%016llx\n", flt.bits.mantissa);
  //printf("%016llx\n", (1ull << 51));
  //printf("%016llx\n", flt.bits.mantissa & (1ull << 51));

  double snan = nanostl::numeric_limits<double>::signaling_NaN();
  flt.f = snan;

  TEST_CHECK(flt.bits.exponent == 2047);
  TEST_CHECK((flt.bits.mantissa & (1ull << 51)) == 0);
  TEST_CHECK(flt.bits.mantissa != 0);

}

static void test_digits10(void) {
  TEST_CHECK(nanostl::numeric_limits<bool>::digits10 == std::numeric_limits<bool>::digits10);

  TEST_CHECK(nanostl::numeric_limits<char>::digits10 == std::numeric_limits<char>::digits10);
  TEST_CHECK(nanostl::numeric_limits<unsigned char>::digits10 == std::numeric_limits<unsigned char>::digits10);
  TEST_CHECK(nanostl::numeric_limits<short>::digits10 == std::numeric_limits<short>::digits10);
  TEST_CHECK(nanostl::numeric_limits<unsigned short>::digits10 == std::numeric_limits<unsigned short>::digits10);

  TEST_CHECK(nanostl::numeric_limits<int>::digits10 == std::numeric_limits<int>::digits10);
  TEST_CHECK(nanostl::numeric_limits<unsigned int>::digits10 == std::numeric_limits<unsigned int>::digits10);

  TEST_CHECK(nanostl::numeric_limits<long long>::digits10 == std::numeric_limits<long long>::digits10);
  TEST_CHECK(nanostl::numeric_limits<unsigned long long>::digits10 == std::numeric_limits<unsigned long long>::digits10);
}

extern "C" void test_valarray(void);

TEST_LIST = {{"test-vector", test_vector},
             {"test-limits", test_limits},
             {"test-string", test_string},
             {"test-map", test_map},
             {"test-algorithm", test_algorithm},
             {"test-iterator", test_iterator},
             {"test-math-func1", test_math_func1},
             {"test-math-exp", test_math_exp},
             {"test-math-log", test_math_log},
             {"test-math-log10", test_math_log10},
             {"test-math-sin", test_math_sin},
             {"test-math-cos", test_math_cos},
             {"test-math-sqrt", test_math_sqrt},
             {"test-math-erf", test_math_erf},
             {"test-math-erfc", test_math_erfc},
             {"test-math-fmin", test_math_fmin},
             {"test-valarray", test_valarray},
             {"test-float-nan", test_float_nan},
             {"test-double-nan", test_double_nan},
             {"test-digits10", test_digits10},
             {nullptr, nullptr}};

// TEST_MAIN();
