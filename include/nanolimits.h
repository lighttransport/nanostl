/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_LIMITS_H_
#define NANOSTL_LIMITS_H_

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

#include "nanocommon.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#endif

namespace nanostl {

template <class T>
struct numeric_limits;

template <>
struct numeric_limits<char> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline char min(void) { return -128; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline char max(void) { return 127; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline char epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned char> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned char min(void) { return 0; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned char max(void) { return 255; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned char epsilon(void) { return 0; }
};

template <>
struct numeric_limits<short> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline short min(void) { return -32768; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline short max(void) { return 32767; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline short epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned short> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned short min(void) { return 0; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned short max(void) { return 65535; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned short epsilon(void) { return 0; }
};

template <>
struct numeric_limits<int> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline int min(void) { return -2147483648; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline int max(void) { return 2147483647; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline int epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned int> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned int min(void) { return 0; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned int max(void) { return 0xffffffffU; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned int epsilon(void) { return 0; }
};

template <>
struct numeric_limits<long long> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline long long min(void) { return (-0x7FFFFFFFFFFFFFFFLL - 1LL); }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline long long max(void) { return 0x7FFFFFFFFFFFFFFFLL; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline long long epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned long long> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned long long min(void) { return 0; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned long long max(void) { return 0xFFFFFFFFFFFFFFFFULL; }
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline unsigned long long epsilon(void) { return 0; }
};

template <>
struct numeric_limits<float> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float min(void) { return (1.17549435E-38f); }  // 0x1.0p-126f
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float max(void) {
    return (3.402823466e+38F);
  }  // 0x1.fffffep127f
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float epsilon(void) { return (1.19209290E-07f); }  // 0x1.0p-23f

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float denorm_min(void) {
    IEEE754Float flt;
    flt.bits.sign = 0;
    flt.bits.exponent = 0;
    flt.bits.mantissa = 1;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float infinity(void) {
    IEEE754Float flt;
    flt.bits.exponent = 255;
    flt.bits.mantissa = 0;
    flt.bits.sign = 0;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float quiet_NaN(void) {
    IEEE754Float flt;
    flt.bits.exponent = 255;
    flt.bits.mantissa = 1 << 22;
    flt.bits.sign = 0;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline float signaling_NaN(void) {
    IEEE754Float flt;
    flt.bits.exponent = 255;
    flt.bits.mantissa = 1;  // Set LSB at the moment
    flt.bits.sign = 0;

    return flt.f;
  }
};

template <>
struct numeric_limits<double> {
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double min(void) {
    return (2.2250738585072014e-308);
  }  // 0x1.0p-1022
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double max(void) {
    return (1.7976931348623157e+308);
  }  // 0x1.fffffffffffffp102
  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double epsilon(void) {
    return (2.2204460492503131e-016);
  }  // 0x1.0p-52

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double denorm_min(void) {
    IEEE754Double flt;
    flt.bits.sign = 0;
    flt.bits.exponent = 0;
    flt.bits.mantissa = 1;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double infinity(void) {
    IEEE754Double flt;
    flt.bits.exponent = 2047;
    flt.bits.mantissa = 0;
    flt.bits.sign = 0;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double quiet_NaN(void) {
    IEEE754Double flt;
    flt.bits.exponent = 2047;
    flt.bits.mantissa = 1ull << 51;
    flt.bits.sign = 0;

    return flt.f;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  static inline double signaling_NaN(void) {
    IEEE754Double flt;
    flt.bits.exponent = 2047;
    flt.bits.mantissa = 1;  // Set LSB at the moment
    flt.bits.sign = 0;

    return flt.f;
  }
};

}  // namespace nanostl

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif  // NANOSTL_LIMITS_H_
