/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 - 2019 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_ALGORITHM_H_
#define NANOSTL_ALGORITHM_H_

#include "nanocommon.h"

namespace nanostl {

template <class T>
NANOSTL_HOST_AND_DEVICE_QUAL const T& min(const T& a, const T& b) {
  return !(b < a) ? a : b;
}

template <class T>
NANOSTL_HOST_AND_DEVICE_QUAL const T& max(const T& a, const T& b) {
  return !(b > a) ? a : b;
}

template <class ForwardIt>
NANOSTL_HOST_AND_DEVICE_QUAL ForwardIt max_element(ForwardIt first,
                                                   ForwardIt last) {
  if (first == last) return last;

  ForwardIt largest = first;
  ++first;
  for (; first != last; ++first) {
    if (*largest < *first) {
      largest = first;
    }
  }
  return largest;
}

template <class ForwardIt>
NANOSTL_HOST_AND_DEVICE_QUAL ForwardIt min_element(ForwardIt first,
                                                   ForwardIt last) {
  if (first == last) return last;

  ForwardIt smallest = first;
  ++first;
  for (; first != last; ++first) {
    if (*smallest > *first) {
      smallest = first;
    }
  }
  return smallest;
}

}  // namespace nanostl

#endif  // NANOSTL_ALGORITHM_H_
