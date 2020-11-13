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

#include "nanoalgorithm.h"
#include "nanocommon.h"
#include "nanolimits.h"

//
// Implements some <cmath> functionality.
// WARNING: Implementation is approximation. Not IEEE-754 compatible, it looses
// some precision, don't work well depending on CPU's rounding-mode.
//

// we need to import the definition from math.h
//#define FP_NAN (0)
//#define FP_INFINITE (1)
//#define FP_ZERO (2)
//#define FP_SUBNORMAL (3)
//#define FP_NORMAL (4)
//#define NANOSTL_FP_UNKNOWN  (5)


namespace nanostl {

static inline bool __is_zero(float arg) {
  IEEE754Float flt;
  flt.f = arg;

  if ((flt.bits.mantissa == 0) && (flt.bits.exponent == 0)) {
    return true;
  }

  return false;
}

static inline bool __is_zero(double arg) {
  IEEE754Double flt;
  flt.f = arg;

  if ((flt.bits.mantissa == 0) && (flt.bits.exponent == 0)) {
    return true;
  }

  return false;
}

static inline bool __is_subnormal(float arg) {
  IEEE754Float flt;
  flt.f = arg;

  if ((flt.bits.mantissa != 0) && (flt.bits.exponent == 0)) {
    return true;
  }

  return false;
}

static inline bool __is_subnormal(double arg) {
  IEEE754Double flt;
  flt.f = arg;

  if ((flt.bits.mantissa != 0) && (flt.bits.exponent == 0)) {
    return true;
  }

  return false;
}

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
static inline T fmin(T x, T y) {
  // https://en.cppreference.com/w/cpp/numeric/math/fmin
  // If one of the two arguments is NaN, the value of the other argument is returned
  // Only if both arguments are NaN, NaN is returned

  if (isnan(x) && isnan(y)) {
    // TODO(LTE): Consider signaling_NaN and quiet_NaN?
    return x;
  }

  if (isnan(x)) {
    return y;
  }

  if (isnan(y)) {
    return x;
  }

  return (x < y) ? x : y;
}

template <typename T>
static inline T fmax(T x, T y) {
  // https://en.cppreference.com/w/cpp/numeric/math/fmin
  // If one of the two arguments is NaN, the value of the other argument is returned
  // Only if both arguments are NaN, NaN is returned

  if (isnan(x) && isnan(y)) {
    // TODO(LTE): Consider signaling_NaN and quiet_NaN?
    return x;
  }

  if (isnan(x)) {
    return y;
  }

  if (isnan(y)) {
    return x;
  }

  return (x > y) ? x : y;
}


#if 0
template <typename T>
static inline int fpclassify(T arg) {
  if (isnan(arg)) {
    return FP_NAN;
  } else if (isinf(arg)) {
    return FP_INFINITE;
  } else if (__is_zero(arg)) {
    return FP_ZERO;
  } else if (__is_subnormal(arg)) {
    return FP_SUBNORMAL;
  } else if (isnormal(arg)) {
    return FP_NORMAL;
  } else {
    return NANOSTL_FP_UNKNOWN;
  }
}
#endif

template <typename T>
static inline T clamp(const T& a, const T& low_val, const T& high_val) {
  return nanostl::max(low_val, nanostl::min(high_val, a));
}

static inline float UintToFloat(const unsigned int ui) {
  IEEE754Float flt;
  flt.ui = ui;
  return flt.f;
}

static inline float IntToFloat(const int i) {
  IEEE754Float flt;
  flt.i = i;
  return flt.f;
}

static inline unsigned int FloatToUint(const float f) {
  IEEE754Float flt;
  flt.f = f;
  return flt.ui;
}

static inline int FloatToInt(const float f) {
  IEEE754Float flt;
  flt.f = f;
  return flt.i;
}

static inline float sqrt(const float x) {
  IEEE754Float flt;
  flt.f = x;
  if ((flt.bits.mantissa == 0) && (flt.bits.exponent == 0)) {
    // +0, -0
    return x;
  }

  if (flt.ui == 0x3f800000) {  // 1
    return 1.0f;               // +0
  }

  // -inf
  if ((x < 0.0f) && nanostl::isinf(x)) {
    return x;
  }

  if (x < 0.0f) {
    return nanostl::numeric_limits<float>::quiet_NaN();
  }

  // Faster sqrt using Sqrt2 descrived in:
  // https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi

  // TODO(LTE): Consider Big endian machine.

  const float xhalf = 0.5f * x;

  union  // get bits for floating value
  {
    float x;
    int i;
  } u;
  u.x = x;
  u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0
  return x * u.x *
         (1.5f -
          xhalf * u.x * u.x);  // Newton step, repeating increases accuracy
}

static inline float copysign(const float x, const float y) {
  IEEE754Float flt_x;
  flt_x.f = x;

  IEEE754Float flt_y;
  flt_y.f = y;

  unsigned int x_abs = flt_x.ui & 0x7fffffff;
  unsigned int y_sign = flt_y.ui & 0x80000000;

  IEEE754Float flt;
  flt.ui = y_sign | x_abs;

  return flt.f;
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

#define kM_1_PI (0.318309886183790671537767526745028724)
#define kM_PI_2 (1.57079632679489661923132169163975144)
#define kM_PI_4 (0.785398163397448309615660845819875721)
#define kM_LN2 (0.69314718055994530941723212145817656)
#define kM_LN10 (2.30258509299404568401799145468436421)

/// Fused multiply and add: (a*b + c)
static inline float madd(float a, float b, float c) { return a * b + c; }

/// Round to nearest integer, returning as an int.
static inline int fast_rint(float x) {
  // emulate rounding by adding/substracting 0.5
  return static_cast<int>(x + copysign(0.5f, x));
}

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

  if (flt.ui == 0x3f800000) {  // 1
    return 0.0f;               // +0
  }

  if (x < 0.0f) {
    return nanostl::numeric_limits<float>::infinity();
  }

  // Examined 2130706432 values of logf on [1.17549435e-38,3.40282347e+38]:
  // 0.313865375 avg ulp diff, 5148137 max ulp, 7.62939e-06 max error
  return fast_log2(x) * static_cast<float>(kM_LN2);
}

static inline float log10(const float& x) {
  IEEE754Float flt;
  flt.f = x;

  if ((flt.bits.mantissa == 0) && (flt.bits.exponent == 0)) {
    // +0, -0
    return -nanostl::numeric_limits<float>::infinity();
  }

  if (flt.ui == 0x3f800000) {  // 1
    return 0.0f;               // +0
  }

  if (x < 0.0f) {
    return nanostl::numeric_limits<float>::infinity();
  }

  // Examined 2130706432 values of log10f on [1.17549435e-38,3.40282347e+38]:
  // 0.631237033 avg ulp diff, 4471615 max ulp, 3.8147e-06 max error
  return fast_log2(x) * static_cast<float>(kM_LN2 / kM_LN10);
}

// fast_safe_pow
static inline float pow(float x, float y) {
  if (fabs(y) < nanostl::numeric_limits<float>::epsilon())
    return 1.0f;  // x^0=1
  if (fabs(x) < nanostl::numeric_limits<float>::epsilon())
    return 0.0f;  // 0^y=0
  // be cheap & exact for special case of squaring and identity
  if (y == 1.0f) return x;
  if (y == 2.0f) {
    return nanostl::min(x * x, nanostl::numeric_limits<float>::max());
  }
  float sign = 1.0f;
  if (x < 0) {
    // if x is negative, only deal with integer powers
    // powf returns NaN for non-integers, we will return 0 instead
    int ybits = FloatToInt(y) & 0x7fffffff;
    if (ybits >= 0x4b800000) {
      // always even int, keep positive
    } else if (ybits >= 0x3f800000) {
      // bigger than 1, check
      int k = (ybits >> 23) - 127;   // get exponent
      int j = ybits >> (23 - k);     // shift out possible fractional bits
      if ((j << (23 - k)) == ybits)  // rebuild number and check for a match
        sign = IntToFloat(0x3f800000 | (j << 31));  // +1 for even, -1 for odd
      else
        return 0.0f;  // not integer
    } else {
      return 0.0f;  // not integer
    }
  }
  return sign * fast_exp2(y * fast_log2(fabs(x)));
}

static inline float sin(float x) {
  // very accurate argument reduction from SLEEF
  // starts failing around x=262000
  // Results on: [-2pi,2pi]
  // Examined 2173837240 values of sin: 0.00662760244 avg ulp diff, 2 max
  // ulp, 1.19209e-07 max error
  int q = fast_rint(x * float(kM_1_PI));
  float qf = q;
  x = madd(qf, -0.78515625f * 4, x);
  x = madd(qf, -0.00024187564849853515625f * 4, x);
  x = madd(qf, -3.7747668102383613586e-08f * 4, x);
  x = madd(qf, -1.2816720341285448015e-12f * 4, x);
  x = float(kM_PI_2) - (float(kM_PI_2) - x);  // crush denormals
  float s = x * x;
  if ((q & 1) != 0) x = -x;
  // this polynomial approximation has very low error on [-pi/2,+pi/2]
  // 1.19209e-07 max error in total over [-2pi,+2pi]
  float u = 2.6083159809786593541503e-06f;
  u = madd(u, s, -0.0001981069071916863322258f);
  u = madd(u, s, +0.00833307858556509017944336f);
  u = madd(u, s, -0.166666597127914428710938f);
  u = madd(s, u * x, x);
  // For large x, the argument reduction can fail and the polynomial can be
  // evaluated with arguments outside the valid internal. Just clamp the bad
  // values away (setting to 0.0f means no branches need to be generated).
  if (fabs(u) > 1.0f) u = 0.0f;
  return u;
}

static inline float cos(float x) {
  // same argument reduction as fast_sin
  int q = fast_rint(x * float(kM_1_PI));
  float qf = q;
  x = madd(qf, -0.78515625f * 4, x);
  x = madd(qf, -0.00024187564849853515625f * 4, x);
  x = madd(qf, -3.7747668102383613586e-08f * 4, x);
  x = madd(qf, -1.2816720341285448015e-12f * 4, x);
  x = float(kM_PI_2) - (float(kM_PI_2) - x);  // crush denormals
  float s = x * x;
  // polynomial from SLEEF's sincosf, max error is
  // 4.33127e-07 over [-2pi,2pi] (98% of values are "exact")
  float u = -2.71811842367242206819355e-07f;
  u = madd(u, s, +2.47990446951007470488548e-05f);
  u = madd(u, s, -0.00138888787478208541870117f);
  u = madd(u, s, +0.0416666641831398010253906f);
  u = madd(u, s, -0.5f);
  u = madd(u, s, +1.0f);
  if ((q & 1) != 0) u = -u;
  if (fabs(u) > 1.0f) u = 0.0f;
  return u;
}

// NOTE: this approximation is only valid on [-8192.0,+8192.0], it starts
// becoming really poor outside of this range because the reciprocal amplifies
// errors
static inline float tan(float x) {
  // derived from SLEEF implementation
  // note that we cannot apply the "denormal crush" trick everywhere because
  // we sometimes need to take the reciprocal of the polynomial
  int q = fast_rint(x * float(2 * kM_1_PI));
  float qf = q;
  x = madd(qf, -0.78515625f * 2, x);
  x = madd(qf, -0.00024187564849853515625f * 2, x);
  x = madd(qf, -3.7747668102383613586e-08f * 2, x);
  x = madd(qf, -1.2816720341285448015e-12f * 2, x);
  if ((q & 1) == 0)
    x = float(kM_PI_4) -
        (float(kM_PI_4) -
         x);  // crush denormals (only if we aren't inverting the result later)
  float s = x * x;
  float u = 0.00927245803177356719970703f;
  u = madd(u, s, 0.00331984995864331722259521f);
  u = madd(u, s, 0.0242998078465461730957031f);
  u = madd(u, s, 0.0534495301544666290283203f);
  u = madd(u, s, 0.133383005857467651367188f);
  u = madd(u, s, 0.333331853151321411132812f);
  u = madd(s, u * x, x);
  if ((q & 1) != 0) u = -1.0f / u;
  return u;
}

static inline float sinh(float x) {
  float a = fabs(x);
  if (a > 1.0f) {
    // Examined 53389559 values of sinh on [1,87.3300018]: 33.6886442 avg ulp
    // diff, 178 max ulp
    float e = exp(a);
    return copysign(0.5f * e - 0.5f / e, x);
  } else {
    a = 1.0f - (1.0f - a);  // crush denorms
    float a2 = a * a;
    // degree 7 polynomial generated with sollya
    // Examined 2130706434 values of sinh on [-1,1]: 1.19209e-07 max error
    float r = 2.03945513931e-4f;
    r = madd(r, a2, 8.32990277558e-3f);
    r = madd(r, a2, 0.1666673421859f);
    r = madd(r * a, a2, a);
    return copysign(r, x);
  }
}

static inline float cosh(float x) {
  // Examined 2237485550 values of cosh on [-87.3300018,87.3300018]: 1.78256726
  // avg ulp diff, 178 max ulp
  float e = exp(fabs(x));
  return 0.5f * e + 0.5f / e;
}

static inline float tanh(float x) {
  // Examined 4278190080 values of tanh on
  // [-3.40282347e+38,3.40282347e+38]: 3.12924e-06 max error NOTE: ulp error is
  // high because of sub-optimal handling around the origin
  float e = exp(2.0f * fabs(x));
  return copysign(1 - 2 / (1 + e), x);
}

static inline float erf(float x) {
  // Examined 1082130433 values of erff on [0,4]: 1.93715e-06 max error
  // Abramowitz and Stegun, 7.1.28
  const float a1 = 0.0705230784f;
  const float a2 = 0.0422820123f;
  const float a3 = 0.0092705272f;
  const float a4 = 0.0001520143f;
  const float a5 = 0.0002765672f;
  const float a6 = 0.0000430638f;
  const float a = fabs(x);
  const float b = 1.0f - (1.0f - a);  // crush denormals
  const float r =
      madd(madd(madd(madd(madd(madd(a6, b, a5), b, a4), b, a3), b, a2), b, a1),
           b, 1.0f);
  const float s = r * r;  // ^2
  const float t = s * s;  // ^4
  const float u = t * t;  // ^8
  const float v = u * u;  // ^16
  return copysign(1.0f - 1.0f / v, x);
}

// Fast cube root (performs better that using fast_pow's above with y=1/3)
static inline float cbrt(float x) {
  float x0 = fabs(x);
  // from hacker's delight
  float a = IntToFloat(0x2a5137a0 + FloatToInt(x0) / 3);  // Initial guess.
  // Examined 14272478 values of cbrt on
  // [-9.99999935e-39,9.99999935e-39]: 8.14687e-14 max error Examined 2131958802
  // values of cbrt on [9.99999935e-39,3.40282347e+38]: 2.46930719 avg ulp diff,
  // 12 max ulp
  a = 0.333333333f * (2.0f * a + x0 / (a * a));  // Newton step.
  a = 0.333333333f * (2.0f * a + x0 / (a * a));  // Newton step again.
  a = (fabs(x0) < nanostl::numeric_limits<float>::epsilon()) ? 0
                                                             : a;  // Fix 0 case
  return copysign(a, x);
}

static inline float erfc(float x) {
  // Examined 2164260866 values of erfcf on [-4,4]: 1.90735e-06 max error
  // ulp histogram:
  //   0  = 80.30%
  return 1.0f - erf(x);
}

static inline float ierf(float x) {
  // from: Approximating the erfinv function by Mike Giles
  // to avoid trouble at the limit, clamp input to 1-eps
  float a = fabs(x);
  if (a > 0.99999994f) a = 0.99999994f;
  float w = -log((1.0f - a) * (1.0f + a)), p;
  if (w < 5.0f) {
    w = w - 2.5f;
    p = 2.81022636e-08f;
    p = madd(p, w, 3.43273939e-07f);
    p = madd(p, w, -3.5233877e-06f);
    p = madd(p, w, -4.39150654e-06f);
    p = madd(p, w, 0.00021858087f);
    p = madd(p, w, -0.00125372503f);
    p = madd(p, w, -0.00417768164f);
    p = madd(p, w, 0.246640727f);
    p = madd(p, w, 1.50140941f);
  } else {
    w = sqrt(w) - 3.0f;
    p = -0.000200214257f;
    p = madd(p, w, 0.000100950558f);
    p = madd(p, w, 0.00134934322f);
    p = madd(p, w, -0.00367342844f);
    p = madd(p, w, 0.00573950773f);
    p = madd(p, w, -0.0076224613f);
    p = madd(p, w, 0.00943887047f);
    p = madd(p, w, 1.00167406f);
    p = madd(p, w, 2.83297682f);
  }
  return p * x;
}

// -- End OIIO fmath.h
// ----------------------------------------------------------------------------

}  // namespace nanostl

#endif  // NANOSTL_MATH_H_
