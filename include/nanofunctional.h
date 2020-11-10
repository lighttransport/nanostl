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

}  // namespace nanostl

#endif  // NANOSTL_FUNCTIONAL_H_
