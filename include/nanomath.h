/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_MATH_H_
#define NANOSTL_MATH_H_

#include "nanocommon.h"

//
// Implements some <cmath> functionality.
// WARNING: Implementation is approximation. Not IEEE-754 compatible, it looses
// some precision, don't work well depending on CPU's rounding-mode.
//

namespace nanostl {

template <typename T>
static T fabs(T num) {
  // TODO(LTE): Handle +0 and -0 case correctly.
  // TODO(LTE): Handle +inf and -inf case correctly.

  if (num < static_cast<T>(0)) {
    return -num;
  }

  return num;
}

// https://stackoverflow.com/questions/8377412/ceil-function-how-can-we-implement-it-ourselves
// FIXME(LTE): Won't work for large float value.
template <typename T>
inline T ceil(T num) {
  int inum = int(num);
  T diff = num - static_cast<T>(inum);
  if (fabs(diff) < nanostl::numeric_limits<T>::epsilon()) {
    return inum;
  }

  if (num > static_cast<T>(0)) {
    return inum + 1;
  } else {
    return inum;
  }
}

// https://stackoverflow.com/questions/5122993/floor-int-function-implementaton
template <typename T>
inline T floor(T x) {
  if (x >= static_cast<T>(0)) {
    return int(x);
  } else {
    int y = int(x);
    if (fabs(static_cast<T>(y) - x) < nanostl::numeric_limits<T>::epsilon()) {
      return y;
    } else {
      return y - 1;
    }
  }
}

inline bool isfinite(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 255;
  return ret;
}

inline bool isfinite(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 2047;
  return ret;
}

inline bool isinf(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa == 0);
  return ret;
}

inline bool isinf(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa == 0);
  return ret;
}

inline bool isnan(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa != 0);
  return ret;
}

inline bool isnan(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa != 0);
  return ret;
}

inline bool isnormal(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 255);
  return ret;
}

inline bool isnormal(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 2047);
  return ret;
}




}  // namespace nanostl

#endif  // NANOSTL_MATH_H_
