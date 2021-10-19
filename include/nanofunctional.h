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

#ifndef NANOSTL_FUNCTIONAL_H_
#define NANOSTL_FUNCTIONAL_H_

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

#include "__hashfunc.h"
#include "__nullptr"

namespace nanostl {

// from libc++ =======

template< class Key > struct hash;
//template<> struct hash<int>;
//template<> struct hash<unsigned int>;

template <>
struct hash<bool>
{
    size_t operator()(bool __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<signed char>
{
    size_t operator()(signed char __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<unsigned char>
{
    size_t operator()(unsigned char __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<signed short>
{
    size_t operator()(signed short __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<unsigned short>
{
    size_t operator()(unsigned short __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<int>
{
    size_t operator()(int __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<unsigned int>
{
    size_t operator()(unsigned int __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<long>
{
    size_t operator()(long __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<unsigned long>
{
    size_t operator()(unsigned long __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<long long>
{
    size_t operator()(long long __v) const noexcept {return static_cast<size_t>(__v);}
};

template <>
struct hash<unsigned long long>
{
    size_t operator()(unsigned long long __v) const noexcept {return static_cast<size_t>(__v);}
};

template <class _Tp, size_t = sizeof(_Tp) / sizeof(size_t)>
struct __scalar_hash;

template <class _Tp>
struct __scalar_hash<_Tp, 0>
{
    size_t operator()(_Tp __v) const noexcept
    {
        union
        {
            _Tp    __t;
            size_t __a;
        } __u;
        __u.__a = 0;
        __u.__t = __v;
        return __u.__a;
    }
};

template <class _Tp>
struct __scalar_hash<_Tp, 1>
{
    size_t operator()(_Tp __v) const noexcept
    {
        union
        {
            _Tp    __t;
            size_t __a;
        } __u;
        __u.__a = 0;
        __u.__t = __v;
        return __u.__a;
    }
};

template <>
struct hash<float> : public __scalar_hash<float>
{
    size_t operator()(float __v) const noexcept {
      // -0.0 and 0.0 return same hash
      if (__v == 0.0f) {
        return 0;
      }
      return __scalar_hash<float>::operator()(__v);
    }
};

template <>
struct hash<double> : public __scalar_hash<double>
{
    size_t operator()(double __v) const noexcept {
      // -0.0 and 0.0 return same hash
      if (__v == 0.0f) {
        return 0;
      }
      return __scalar_hash<double>::operator()(__v);
    }
};

template <>
struct hash<nullptr_t>
{
    size_t operator()(nullptr_t) const noexcept {return 662607004ull;}
};

// TODO: long double

// ===================


//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

template <class _Arg, class _Result>
struct _NANOSTL_TEMPLATE_VIS unary_function
{
    typedef _Arg    argument_type;
    typedef _Result result_type;
};

template <class _Arg1, class _Arg2, class _Result>
struct _NANOSTL_TEMPLATE_VIS binary_function
{
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};



template <class _Tp>
struct __has_result_type
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::result_type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

// __weak_result_type

template <class _Tp>
struct __derives_from_unary_function
{
private:
    struct __two {char __lx; char __lxx;};
    static __two __test(...);
    template <class _Ap, class _Rp>
        static unary_function<_Ap, _Rp>
        __test(const volatile unary_function<_Ap, _Rp>*);
public:
    static const bool value = !is_same<decltype(__test((_Tp*)0)), __two>::value;
    typedef decltype(__test((_Tp*)0)) type;
};

template <class _Tp>
struct __derives_from_binary_function
{
private:
    struct __two {char __lx; char __lxx;};
    static __two __test(...);
    template <class _A1, class _A2, class _Rp>
        static binary_function<_A1, _A2, _Rp>
        __test(const volatile binary_function<_A1, _A2, _Rp>*);
public:
    static const bool value = !is_same<decltype(__test((_Tp*)0)), __two>::value;
    typedef decltype(__test((_Tp*)0)) type;
};

template <class _Tp, bool = __derives_from_unary_function<_Tp>::value>
struct __maybe_derive_from_unary_function  // bool is true
    : public __derives_from_unary_function<_Tp>::type
{
};

template <class _Tp>
struct __maybe_derive_from_unary_function<_Tp, false>
{
};

template <class _Tp, bool = __derives_from_binary_function<_Tp>::value>
struct __maybe_derive_from_binary_function  // bool is true
    : public __derives_from_binary_function<_Tp>::type
{
};

template <class _Tp>
struct __maybe_derive_from_binary_function<_Tp, false>
{
};

template <class _Tp, bool = __has_result_type<_Tp>::value>
struct __weak_result_type_imp // bool is true
    : public __maybe_derive_from_unary_function<_Tp>,
      public __maybe_derive_from_binary_function<_Tp>
{
    typedef /*_LIBCPP_NODEBUG*/ typename _Tp::result_type result_type;
};

template <class _Tp>
struct __weak_result_type_imp<_Tp, false>
    : public __maybe_derive_from_unary_function<_Tp>,
      public __maybe_derive_from_binary_function<_Tp>
{
};

template <class _Tp>
struct __weak_result_type
    : public __weak_result_type_imp<_Tp>
{
};

// 0 argument case

template <class _Rp>
struct __weak_result_type<_Rp ()>
{
    typedef /*_LIBCPP_NODEBUG*/ _Rp result_type;
};

template <class _Rp>
struct __weak_result_type<_Rp (&)()>
{
    typedef /*_LIBCPP_NODEBUG*/ _Rp result_type;
};

template <class _Rp>
struct __weak_result_type<_Rp (*)()>
{
    typedef /*_LIBCPP_NODEBUG*/ _Rp result_type;
};

// 1 argument case

template <class _Rp, class _A1>
struct __weak_result_type<_Rp (_A1)>
    : public unary_function<_A1, _Rp>
{
};

template <class _Rp, class _A1>
struct __weak_result_type<_Rp (&)(_A1)>
    : public unary_function<_A1, _Rp>
{
};

template <class _Rp, class _A1>
struct __weak_result_type<_Rp (*)(_A1)>
    : public unary_function<_A1, _Rp>
{
};

template <class _Rp, class _Cp>
struct __weak_result_type<_Rp (_Cp::*)()>
    : public unary_function<_Cp*, _Rp>
{
};

template <class _Rp, class _Cp>
struct __weak_result_type<_Rp (_Cp::*)() const>
    : public unary_function<const _Cp*, _Rp>
{
};

template <class _Rp, class _Cp>
struct __weak_result_type<_Rp (_Cp::*)() volatile>
    : public unary_function<volatile _Cp*, _Rp>
{
};

template <class _Rp, class _Cp>
struct __weak_result_type<_Rp (_Cp::*)() const volatile>
    : public unary_function<const volatile _Cp*, _Rp>
{
};

// 2 argument case

template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp (_A1, _A2)>
    : public binary_function<_A1, _A2, _Rp>
{
};

template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp (*)(_A1, _A2)>
    : public binary_function<_A1, _A2, _Rp>
{
};

template <class _Rp, class _A1, class _A2>
struct __weak_result_type<_Rp (&)(_A1, _A2)>
    : public binary_function<_A1, _A2, _Rp>
{
};

template <class _Rp, class _Cp, class _A1>
struct __weak_result_type<_Rp (_Cp::*)(_A1)>
    : public binary_function<_Cp*, _A1, _Rp>
{
};

template <class _Rp, class _Cp, class _A1>
struct __weak_result_type<_Rp (_Cp::*)(_A1) const>
    : public binary_function<const _Cp*, _A1, _Rp>
{
};

template <class _Rp, class _Cp, class _A1>
struct __weak_result_type<_Rp (_Cp::*)(_A1) volatile>
    : public binary_function<volatile _Cp*, _A1, _Rp>
{
};

template <class _Rp, class _Cp, class _A1>
struct __weak_result_type<_Rp (_Cp::*)(_A1) const volatile>
    : public binary_function<const volatile _Cp*, _A1, _Rp>
{
};

// 3 or more arguments

template <class _Rp, class _A1, class _A2, class _A3, class ..._A4>
struct __weak_result_type<_Rp (_A1, _A2, _A3, _A4...)>
{
    typedef _Rp result_type;
};

template <class _Rp, class _A1, class _A2, class _A3, class ..._A4>
struct __weak_result_type<_Rp (&)(_A1, _A2, _A3, _A4...)>
{
    typedef _Rp result_type;
};

template <class _Rp, class _A1, class _A2, class _A3, class ..._A4>
struct __weak_result_type<_Rp (*)(_A1, _A2, _A3, _A4...)>
{
    typedef _Rp result_type;
};

template <class _Rp, class _Cp, class _A1, class _A2, class ..._A3>
struct __weak_result_type<_Rp (_Cp::*)(_A1, _A2, _A3...)>
{
    typedef _Rp result_type;
};

template <class _Rp, class _Cp, class _A1, class _A2, class ..._A3>
struct __weak_result_type<_Rp (_Cp::*)(_A1, _A2, _A3...) const>
{
    typedef _Rp result_type;
};

template <class _Rp, class _Cp, class _A1, class _A2, class ..._A3>
struct __weak_result_type<_Rp (_Cp::*)(_A1, _A2, _A3...) volatile>
{
    typedef _Rp result_type;
};

template <class _Rp, class _Cp, class _A1, class _A2, class ..._A3>
struct __weak_result_type<_Rp (_Cp::*)(_A1, _A2, _A3...) const volatile>
{
    typedef _Rp result_type;
};

template <class _Tp, class ..._Args>
struct __invoke_return
{
    typedef decltype(__invoke(declval<_Tp>(), declval<_Args>()...)) type;
};





template <class _Tp>
class _NANOSTL_TEMPLATE_VIS reference_wrapper
//#if _LIBCPP_STD_VER <= 17 || !defined(_LIBCPP_ABI_NO_BINDER_BASES)
    : public __weak_result_type<_Tp>
//#endif
{
public:
    // types
    typedef _Tp type;
private:
    type* __f_;

    static void __fun(_Tp&) __NANOSTL_NOEXCEPT;
    static void __fun(_Tp&&) = delete;

};

//===----------------------------------------------------------------------===//


}  // namespace nanostl

#endif  // NANOSTL_FUNCTIONAL_H_
