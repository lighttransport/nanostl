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

static void test_algorithm(void) {
  TEST_CHECK(nanostl::min(1, 2) == 1);
  TEST_CHECK(nanostl::max(1, 2) == 2);
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

static void test_math(void) {

  TEST_CHECK(float_equals(nanostl::ceil(0.0f), std::ceil(0.0f)));

  TEST_CHECK(float_equals(nanostl::ceil(1.0f / 2.0f), std::ceil(1.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(2.0f / 2.0f), std::ceil(2.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(3.0f / 2.0f), std::ceil(3.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(4.0f / 2.0f), std::ceil(4.0f / 2.0f)));

  TEST_CHECK(float_equals(nanostl::ceil(0.0f / 2.0f), std::ceil(0.0f / 2.0f)));

  TEST_CHECK(float_equals(nanostl::ceil(-1.0f / 2.0f), std::ceil(-1.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(-2.0f / 2.0f), std::ceil(-2.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(-3.0f / 2.0f), std::ceil(-3.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(-4.0f / 2.0f), std::ceil(-4.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::ceil(-5.0f / 2.0f), std::ceil(-5.0f / 2.0f)));

  TEST_CHECK(float_equals(nanostl::floor(0.0f / 2.0f), std::floor(0.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(1.0f / 2.0f), std::floor(1.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(2.0f / 2.0f), std::floor(2.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(3.0f / 2.0f), std::floor(3.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(4.0f / 2.0f), std::floor(4.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(5.0f / 2.0f), std::floor(5.0f / 2.0f)));

  TEST_CHECK(float_equals(nanostl::floor(-0.0f / 2.0f), std::floor(-0.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(-1.0f / 2.0f), std::floor(-1.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(-2.0f / 2.0f), std::floor(-2.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(-3.0f / 2.0f), std::floor(-3.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(-4.0f / 2.0f), std::floor(-4.0f / 2.0f)));
  TEST_CHECK(float_equals(nanostl::floor(-5.0f / 2.0f), std::floor(-5.0f / 2.0f)));
}

TEST_LIST = {{"test-vector", test_vector},
             {"test-limits", test_limits},
             {"test-string", test_string},
             {"test-map", test_map},
             {"test-algorithm", test_algorithm},
             {"test-math", test_math},
             {NULL, NULL}};

// TEST_MAIN();
