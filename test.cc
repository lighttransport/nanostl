#include "nanovector.h"
#include "nanoalgorithm.h"
#include "nanolimits.h"
#include "nanostring.h"
#include "nanomap.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wundef"
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wdeprecated"
#pragma clang diagnostic ignored "-Wextra-semi"
#if __has_warning("-Wcomma")
#pragma clang diagnostic ignored "-Wcomma"
#endif
#if __has_warning("-Wundefined-func-template")
#pragma clang diagnostic ignored "-Wundefined-func-template"
#endif
#endif

#include "microtest.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// For testing.
#include <limits>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

#define FLOAT_EQ(a, b) { \
  float val0 = (a); \
  float val1 = (b); \
  ASSERT_EQ(*(reinterpret_cast<unsigned int *>(&val0)), *(reinterpret_cast<unsigned int *>(&val1))); \
}

#define DOUBLE_EQ(a, b) { \
  double val0 = (a); \
  double val1 = (b); \
  ASSERT_EQ(*(reinterpret_cast<unsigned long long *>(&val0)), *(reinterpret_cast<unsigned long long *>(&val1))); \
}

TEST(VectorTest) {

  nanostl::vector<int> v;

  ASSERT_TRUE(v.empty());

  v.push_back(1);
  ASSERT_FALSE(v.empty());

  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);


  ASSERT_EQ(v.at(0), 1);
  ASSERT_EQ(v[1], 2);
  ASSERT_EQ(v[2], 3);
  ASSERT_EQ(v[3], 4);
  ASSERT_EQ(v[4], 5);

  v.resize(3);
  ASSERT_EQ(v.at(0), 1);
  ASSERT_EQ(v[1], 2);
  ASSERT_EQ(v[2], 3);

  v.resize(6);
  ASSERT_EQ(v.at(0), 1);
  ASSERT_EQ(v[1], 2);
  ASSERT_EQ(v[2], 3);
}

TEST(AlgorithmTest) {

  ASSERT_EQ(nanostl::min(1, 2), 1);
  ASSERT_EQ(nanostl::max(1, 2), 2);

}

TEST(StringTest) {

  nanostl::string s("a");

  ASSERT_FALSE(s.empty());

}

TEST(LimitsTest) {

  ASSERT_EQ(nanostl::numeric_limits<char>::min(),  std::numeric_limits<char>::min());
  ASSERT_EQ(nanostl::numeric_limits<char>::max(),  std::numeric_limits<char>::max());

  ASSERT_EQ(nanostl::numeric_limits<short>::min(),  std::numeric_limits<short>::min());
  ASSERT_EQ(nanostl::numeric_limits<short>::max(),  std::numeric_limits<short>::max());

  ASSERT_EQ(nanostl::numeric_limits<int>::min(), std::numeric_limits<int>::min());
  ASSERT_EQ(nanostl::numeric_limits<int>::max(), std::numeric_limits<int>::max());

  FLOAT_EQ(nanostl::numeric_limits<float>::min(),  std::numeric_limits<float>::min());
  FLOAT_EQ(nanostl::numeric_limits<float>::max(),  std::numeric_limits<float>::max());

  DOUBLE_EQ(nanostl::numeric_limits<double>::min(),  std::numeric_limits<double>::min());
  DOUBLE_EQ(nanostl::numeric_limits<double>::max(),  std::numeric_limits<double>::max());

}

TEST_MAIN();
