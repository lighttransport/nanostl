// SPDX-License-Identifier: MIT
// Copyright 2024 - Present, Light Transport Entertainment Inc.

#ifndef NANOSTL_NUMERIC
#define NANOSTL_NUMERIC

#include "nanoutility.h" // nanostl::move

namespace nanostl {

template <class InputIt, class T>
T accumulate(InputIt first, InputIt last, T init) {
  T ret = init;

  while (first != last) {
    ret = ret + *(first);

    first++;
  } 

  return ret;
}

template <class InputIt, class T, class BinaryOp>
T accumulate(InputIt first, InputIt last, T init, BinaryOp bin_op) {

  T ret = init;

  while (first != last) {
    T rhs = (*first);
    ret = bin_op(nanostl::move(ret), rhs);
    first++;
  } 

  return ret;
}

} // namespace nanostl


#endif // NANOSTL_NUMERIC
