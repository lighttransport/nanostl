/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Light Transport Entertainment, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef NANOSTL_RATIO_H_
#define NANOSTL_RATIO_H_

#include "nanocommon.h"

namespace nanostl {

namespace {

// compile-time gcd and lcm
// TODO: Use C++17 gcd interface?

template<intmax_t a, intmax_t b>
struct nanogcd
{
  static const intmax_t value = nanogcd<b, a % b>::value;
};

template<intmax_t a>
struct nanogcd<a, 0>
{
  static const intmax_t value = a;
};

template<>
struct nanogcd<0, 0>
{
  static const intmax_t value = 1;
};


template<intmax_t a, intmax_t b>
struct nanolcm
{
  static const intmax_t value = a / nanogcd<a, b>::value * b;
};


template<intmax_t val>
struct nanoabs
{
  static const intmax_t value = val < 0 ? -val : val;
};

template<intmax_t val>
struct nanosign
{
  static const intmax_t value = val == 0 ? 0 : (val < 0 ? -1 : 1);
};


} // namespace

template<intmax_t N, intmax_t D = 1>
class ratio {
 private:
  static_assert(D != 0, "Division by zero");

  static constexpr const intmax_t _Nabs = nanoabs<N>::value;
  static constexpr const intmax_t _Dabs = nanoabs<D>::value;
  static constexpr const intmax_t _ss = nanosign<N>::value * nanosign<D>::value;
  static constexpr const intmax_t _gcd = nanogcd<_Nabs, _Dabs>::value;

 public:
  static constexpr intmax_t num = _ss * _Nabs / _gcd;
  static constexpr intmax_t den = _Dabs / _gcd;

  typedef ratio<num, den> type;

};


typedef ratio<1LL, 1000000000000000000LL> atto;
typedef ratio<1LL,    1000000000000000LL> femto;
typedef ratio<1LL,       1000000000000LL> pico;
typedef ratio<1LL,          1000000000LL> nano;
typedef ratio<1LL,             1000000LL> micro;
typedef ratio<1LL,                1000LL> milli;
typedef ratio<1LL,                 100LL> centi;
typedef ratio<1LL,                  10LL> deci;
typedef ratio<                 10LL, 1LL> deca;
typedef ratio<                100LL, 1LL> hecto;
typedef ratio<               1000LL, 1LL> kilo;
typedef ratio<            1000000LL, 1LL> mega;
typedef ratio<         1000000000LL, 1LL> giga;
typedef ratio<      1000000000000LL, 1LL> tera;
typedef ratio<   1000000000000000LL, 1LL> peta;
typedef ratio<1000000000000000000LL, 1LL> exa;


} // namespace nanostl

#endif // NANOSTL_CHRONO_H_
