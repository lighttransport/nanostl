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

//
// Based on libcxx: Apache-2.0 WITH LLVM-exception
//
#ifndef NANOSTL_TYPE_TRAITS_H_
#define NANOSTL_TYPE_TRAITS_H_
#pragma once

#include "nanocommon.h"


namespace nanostl {

// helper class:
template <class T, T v>
struct integral_constant;
typedef integral_constant<bool, true> true_type;    // C++11
typedef integral_constant<bool, false> false_type;  // C++11

template <bool B>                                  // C++14
using bool_constant = integral_constant<bool, B>;  // C++14
typedef bool_constant<true> true_type;             // C++14
typedef bool_constant<false> false_type;           // C++14

// helper traits
template <bool, class T = void>
struct enable_if;
// template <bool, class T, class F> struct conditional;

// Primary classification traits:
template <class T>
struct is_void;
// template <class T> struct is_null_pointer;  // C++14
template <class T>
struct is_integral;
template <class T>
struct is_floating_point;
template <class T>
struct is_array;
template <class T>
struct is_pointer;
// template <class T> struct is_lvalue_reference;
// template <class T> struct is_rvalue_reference;
// template <class T> struct is_member_object_pointer;
// template <class T> struct is_member_function_pointer;
// template <class T> struct is_enum;
// template <class T> struct is_union;
// template <class T> struct is_class;
// template <class T> struct is_function;

    template <class T> struct remove_const;
    template <class T> struct remove_volatile;
 template <class T> struct remove_cv;


template <class T, class U> struct is_same;


template <bool b, class T = void>
using enable_if_t = typename enable_if<b, T>::type;  // C++14


template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_const            {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_const<const _Tp> {typedef _Tp type;};
#if _LIBCPP_STD_VER > 11
//template <class _Tp> using remove_const_t = typename remove_const<_Tp>::type;
#endif

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_volatile               {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_volatile<volatile _Tp> {typedef _Tp type;};
#if _LIBCPP_STD_VER > 11
//template <class _Tp> using remove_volatile_t = typename remove_volatile<_Tp>::type;
#endif


template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_cv
{typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;};
#if _LIBCPP_STD_VER > 11
//template <class _Tp> using remove_cv_t = typename remove_cv<_Tp>::type;
#endif


template <bool, class T> struct _NANOSTL_TEMPLATE_VIS enable_if {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS enable_if<true, _Tp> {typedef _Tp type;};


typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;


template <class _Tp, _Tp __v>
struct _NANOSTL_TEMPLATE_VIS integral_constant
{
  static constexpr const _Tp      value = __v;
  typedef _Tp               value_type;
  typedef integral_constant type;
  constexpr operator value_type() const noexcept {return value;}
#if _LIBCPP_STD_VER > 11
  //_LIBCPP_INLINE_VISIBILITY
  //constexpr value_type operator ()() const _NOEXCEPT {return value;}
#endif
};

template <class _Tp, _Tp __v>
constexpr const _Tp integral_constant<_Tp, __v>::value;


template <class _Tp, class _Up> struct _NANOSTL_TEMPLATE_VIS is_same           : public false_type {};
template <class _Tp>            struct _NANOSTL_TEMPLATE_VIS is_same<_Tp, _Tp> : public true_type {};


template <class _Tp> struct __libcpp_is_floating_point              : public false_type {};
template <>          struct __libcpp_is_floating_point<float>       : public true_type {};
template <>          struct __libcpp_is_floating_point<double>      : public true_type {};
template <>          struct __libcpp_is_floating_point<long double> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_floating_point
    : public __libcpp_is_floating_point<typename remove_cv<_Tp>::type> {};


}  // namespace nanostl

#endif
