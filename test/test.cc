#include "nanoalgorithm.h"
#include "nanolimits.h"
#include "nanomap.h"
#include "nanomath.h"
#include "nanosstream.h"
#include "nanostring.h"
#include "nanoutility.h"
#include "nanovector.h"

#include <cstdio>
#include <cstdlib>
//#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

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
  std::cout << "x: " << x << ", y: " << y << ", diff_ulps = " << diff << std::endl;

  if (diff <= max_ulp_diffs) {
    return true;
  }

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

  TEST_CHECK(float_equals(nanostl::numeric_limits<double>::min(),
                          std::numeric_limits<double>::min()));
  TEST_CHECK(float_equals(nanostl::numeric_limits<double>::max(),
                          std::numeric_limits<double>::max()));
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
  TEST_CHECK(nanostl::isnan(0.0f / 0.0f) == true);

  const float pos_inf_f = std::numeric_limits<float>::infinity();
  const double pos_inf_d = std::numeric_limits<double>::infinity();
  const float nan_f = 0.0f / 0.0f;
  const double nan_d = 0.0 / 0.0;
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
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.0f), std::cos(0.0f), 1));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(1.0f), std::cos(1.0f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.1f), std::cos(0.1f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(0.01f), std::cos(0.01f), 0));
  TEST_CHECK(float_equals_by_ulps(nanostl::cos(3.33f), std::cos(3.33f), 1));
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
             {nullptr, nullptr}};

// TEST_MAIN();
