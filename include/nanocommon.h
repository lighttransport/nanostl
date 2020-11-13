/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2020 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_COMMON_H_
#define NANOSTL_COMMON_H_

#ifdef __CUDACC__
#define NANOSTL_DEVICE_QUAL __device__
#define NANOSTL_HOST_QUAL __host__
#define NANOSTL_HOST_AND_DEVICE_QUAL __host__ __device__
#define NANOSTL_GLOBAL_QUAL __global__
#define NANOSTL_CONSTANT_QUAL __constant__
#else
#define NANOSTL_DEVICE_QUAL
#define NANOSTL_HOST_QUAL
#define NANOSTL_HOST_AND_DEVICE_QUAL
#define NANOSTL_GLOBAL_QUAL
#define NANOSTL_CONSTANT_QUAL
#endif

// TODO(LTE): Implement
#ifndef _NANOSTL_TEMPLATE_VIS
#define _NANOSTL_TEMPLATE_VIS
#endif

namespace nanostl {

// In BigEndian architecture, must define NANOSTL_BIG_ENDIAN explicitly at the
// moment.

namespace {

union IEEE754Float {
  float f;
  unsigned int ui;
  int i;
  struct {
#if defined(NANOSTL_BIG_ENDIAN)
    unsigned int sign : 1;
    unsigned int exponent : 8;
    unsigned int mantissa : 23;
#else
    unsigned int mantissa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
#endif
  } bits;
};

union IEEE754Double {
  double f;
  unsigned long long ull;
  long long ll;
  struct {
#if defined(NANOSTL_BIG_ENDIAN)
    unsigned long long sign : 1;
    unsigned long long exponent : 11;
    unsigned long long mantissa : 52;
#else
    unsigned long long mantissa : 52;
    unsigned long long exponent : 11;
    unsigned long long sign : 1;
#endif
  } bits;
};

using size_t = unsigned long long;

} // namespace

}  // namespace nanostl

#endif  // NANOSTL_COMMON_H_
