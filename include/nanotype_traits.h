/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_TYPE_TRAITS_H_
#define NANOSTL_TYPE_TRAITS_H_

//
// Simple implementation of subset of <type_traits>
//

namespace nanostl {

template <class T, T v>
struct integral_constant;
typedef integral_constant<bool, true> true_type;    // C++11
typedef integral_constant<bool, false> false_type;  // C++11

template <class T>
struct add_rvalue_reference;

}  // namespace nanostl

#endif  // NANOSTL_TYPE_TRAITS_H_
