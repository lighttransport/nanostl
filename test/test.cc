#include "nanovector.h"
#include "nanoalgorithm.h"
#include "nanolimits.h"
#include "nanostring.h"
#include "nanomap.h"
#include "nanoutility.h"
#include "nanosstream.h"

#include <cstdlib>
#include <cstdio>
//#include <cstdint>
#include <cassert>
#include <iostream>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"
#endif
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

//#include "microtest.h"
#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_NO_CPP11
#include "catch.hpp"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// For testing.
#include <limits>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

#if 0
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
#endif

#ifdef __clang__
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

TEST_CASE("VectorTest", "[vector]") {

  nanostl::vector<int> v;

  REQUIRE(v.empty() == true);

  v.push_back(1);
  REQUIRE(v.empty() == false);

  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);


  REQUIRE(v.at(0) == 1);
  REQUIRE(v[1] == 2);
  REQUIRE(v[2] == 3);
  REQUIRE(v[3] == 4);
  REQUIRE(v[4] == 5);

  v.resize(3);
  REQUIRE(v.at(0) == 1);
  REQUIRE(v[1] == 2);
  REQUIRE(v[2] == 3);

  v.resize(6);
  REQUIRE(v.at(0) == 1);
  REQUIRE(v[1] == 2);
  REQUIRE(v[2] == 3);

  v.erase(v.begin());
  REQUIRE(v.at(0) == 2);

  nanostl::vector<int> y;
  y.swap(v);

  REQUIRE(y.at(0) == 2);
  REQUIRE(v.size() == 0);
}

TEST_CASE("AlgorithmTest", "[algorithm]") {

  REQUIRE(nanostl::min(1, 2) == 1);
  REQUIRE(nanostl::max(1, 2) == 2);

}

TEST_CASE("StringTest", "[string]") {

  nanostl::string s("a");

  REQUIRE(s.empty() == false);

  REQUIRE(s[0] == 'a');
  REQUIRE(s.at(0) == 'a');

}

TEST_CASE("MapTest", "[map]") {

  nanostl::map<nanostl::string, int> m;

  REQUIRE(m.empty());

  m["a"] = 1;
  m["b"] = 2;

  REQUIRE(m["a"] == 1);
  REQUIRE(m["b"] == 2);

}

TEST_CASE("LimitsTest", "[limits]") {

  REQUIRE(nanostl::numeric_limits<char>::min() == std::numeric_limits<char>::min());
  REQUIRE(nanostl::numeric_limits<char>::max() == std::numeric_limits<char>::max());

  REQUIRE(nanostl::numeric_limits<short>::min() ==  std::numeric_limits<short>::min());
  REQUIRE(nanostl::numeric_limits<short>::max() ==  std::numeric_limits<short>::max());

  REQUIRE(nanostl::numeric_limits<int>::min() == std::numeric_limits<int>::min());
  REQUIRE(nanostl::numeric_limits<int>::max() == std::numeric_limits<int>::max());

  REQUIRE(double(nanostl::numeric_limits<float>::min()) == Approx(double(std::numeric_limits<float>::min())) );
  REQUIRE(double(nanostl::numeric_limits<float>::max()) == Approx(double(std::numeric_limits<float>::max())) );

  REQUIRE(nanostl::numeric_limits<double>::min() == Approx(std::numeric_limits<double>::min()) );
  REQUIRE(nanostl::numeric_limits<double>::max() == Approx(std::numeric_limits<double>::max()) );

}

//TEST_MAIN();
