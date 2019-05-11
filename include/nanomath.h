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
static inline T fabs(T num) {
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
static inline T ceil(T num) {
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
static inline T floor(T x) {
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

static inline bool isfinite(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 255;
  return ret;
}

static inline bool isfinite(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 2047;
  return ret;
}

static inline bool isinf(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa == 0);
  return ret;
}

static inline bool isinf(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa == 0);
  return ret;
}

static inline bool isnan(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa != 0);
  return ret;
}

static inline bool isnan(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa != 0);
  return ret;
}

static inline bool isnormal(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 255);
  return ret;
}

static inline bool isnormal(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 2047);
  return ret;
}

template <typename T>
static inline T clamp(const T& a, const T& low_val, const T& high_val) {
  return nanostl::max(low_val, nanostl::min(high_val, a));
}

static inline float UintToFloat(const unsigned int ui) {
  IEEE754Float flt;
  flt.ui = ui;
  return flt.f;
}

static inline unsigned int FloatToUint(const float f) {
  IEEE754Float flt;
  flt.f = f;
  return flt.ui;
}

// Following faster approximated math functions are based on OIIO fmath.h

//  Copyright 2008-2014 Larry Gritz and the other authors and contributors.
//  All Rights Reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the software's owners nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  (This is the Modified BSD License)
//
//  A few bits here are based upon code from NVIDIA that was also released
//  under the same modified BSD license, and marked as:
//     Copyright 2004 NVIDIA Corporation. All Rights Reserved.
//
//  Some parts of this file were first open-sourced in Open Shading Language,
//  then later moved here. The original copyright notice was:
//     Copyright (c) 2009-2014 Sony Pictures Imageworks Inc., et al.
//
//  Many of the math functions were copied from or inspired by other
//  public domain sources or open source packages with compatible licenses.
//  The individual functions give references were applicable.

#define kM_LN2 (0.69314718055994530941723212145817656)

/// Fused multiply and add: (a*b + c)
static inline float madd(float a, float b, float c) { return a * b + c; }

// TODO(LTE): Generate our own approx function using sollya
static inline float fast_exp2(const float& xval) {
  // clamp to safe range for final addition
  float x = clamp(xval, -126.0f, 126.0f);
  // range reduction
  int m = int(x);
  x -= m;
  x = 1.0f - (1.0f - x);  // crush denormals (does not affect max ulps!)
  // 5th degree polynomial generated with sollya
  // Examined 2247622658 values of exp2 on [-126,126]: 2.75764912 avg ulp diff,
  // 232 max ulp ulp histogram:
  //  0  = 87.81%
  //  1  =  4.18%
  float r = 1.33336498402e-3f;
  r = madd(x, r, 9.810352697968e-3f);
  r = madd(x, r, 5.551834031939e-2f);
  r = madd(x, r, 0.2401793301105f);
  r = madd(x, r, 0.693144857883f);
  r = madd(x, r, 1.0f);
  // multiply by 2 ^ m by adding in the exponent
  // NOTE: left-shift of negative number is undefined behavior
  unsigned int ri = FloatToUint(r);
  float _f = UintToFloat(ri + (static_cast<unsigned int>(m) << 23));
  return _f;
}

template <typename T>
static inline T exp(T x);

static inline float exp(float x) {
  // Examined 2237485550 values of exp on [-87.3300018,87.3300018]: 2.6666452
  // avg ulp diff, 230 max ulp

  return fast_exp2(x * static_cast<float>(1 / kM_LN2));
}

static inline float fast_log2(const float& xval) {
  // NOTE: clamp to avoid special cases and make result "safe" from large
  // negative values/nans
  float x = clamp(xval, nanostl::numeric_limits<float>::min(),
                  nanostl::numeric_limits<float>::max());
  // based on https://github.com/LiraNuna/glsl-sse2/blob/master/source/vec4.h
  unsigned bits = FloatToUint(x);
  int exponent = int(bits >> 23) - 127;
  float f = UintToFloat((bits & 0x007FFFFF) | 0x3f800000) - 1.0f;
  // Examined 2130706432 values of log2 on [1.17549435e-38,3.40282347e+38]:
  // 0.0797524457 avg ulp diff, 3713596 max ulp, 7.62939e-06 max error ulp
  // histogram:
  //  0  = 97.46%
  //  1  =  2.29%
  //  2  =  0.11%
  float f2 = f * f;
  float f4 = f2 * f2;
  float hi = madd(f, -0.00931049621349f, 0.05206469089414f);
  float lo = madd(f, 0.47868480909345f, -0.72116591947498f);
  hi = madd(f, hi, -0.13753123777116f);
  hi = madd(f, hi, 0.24187369696082f);
  hi = madd(f, hi, -0.34730547155299f);
  lo = madd(f, lo, 1.442689881667200f);
  return ((f4 * hi) + (f * lo)) + exponent;
}

static inline float log(const float& x) {
  IEEE754Float flt;
  flt.f = x;
  if ((flt.bits.mantissa == 0) && (flt.bits.exponent == 0)) {
    // +0, -0
    return -nanostl::numeric_limits<float>::infinity();
  }

  if (flt.ui == 0x3f800000) { // 1
    return 0.0f; // +0
  }

  if (x < 0.0f) {
    return nanostl::numeric_limits<float>::infinity();
  }

  // Examined 2130706432 values of logf on [1.17549435e-38,3.40282347e+38]:
  // 0.313865375 avg ulp diff, 5148137 max ulp, 7.62939e-06 max error
  return fast_log2(x) * static_cast<float>(kM_LN2);
}

}  // namespace nanostl

#endif  // NANOSTL_MATH_H_
