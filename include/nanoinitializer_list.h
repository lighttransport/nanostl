/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_INITIALIZER_LIST_H_
#define NANOSTL_INITIALIZER_LIST_H_

#include "nanocommon.h"


namespace nanostl {

//
// Based on libcxx: Apache-2.0 WITH LLVM-exception
//
//===----------------------- initializer_list -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//


template<class _Ep>
class _NANOSTL_TEMPLATE_VIS initializer_list
{
    const _Ep* __begin_;
    size_t    __size_;

    /*_LIBCPP_INLINE_VISIBILITY*/
    /*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
    initializer_list(const _Ep* __b, size_t __s) __NANOSTL_NOEXCEPT
        : __begin_(__b),
          __size_(__s)
        {}
public:
    typedef _Ep        value_type;
    typedef const _Ep& reference;
    typedef const _Ep& const_reference;
    typedef size_t    size_type;

    typedef const _Ep* iterator;
    typedef const _Ep* const_iterator;

    /*_LIBCPP_INLINE_VISIBILITY*/
    /*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
    initializer_list() __NANOSTL_NOEXCEPT : __begin_(nullptr), __size_(0) {}

    /*_LIBCPP_INLINE_VISIBILITY*/
    /*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
    size_t    size()  const __NANOSTL_NOEXCEPT {return __size_;}

    /*_LIBCPP_INLINE_VISIBILITY*/
    /*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
    const _Ep* begin() const __NANOSTL_NOEXCEPT {return __begin_;}

    /*_LIBCPP_INLINE_VISIBILITY*/
    /*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
    const _Ep* end()   const __NANOSTL_NOEXCEPT {return __begin_ + __size_;}
};

template<class _Ep>
inline /*_LIBCPP_INLINE_VISIBILITY*/
/*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
const _Ep*
begin(initializer_list<_Ep> __il) __NANOSTL_NOEXCEPT
{
    return __il.begin();
}

template<class _Ep>
inline /*_LIBCPP_INLINE_VISIBILITY*/
/*_LIBCPP_CONSTEXPR_AFTER_CXX11*/
const _Ep*
end(initializer_list<_Ep> __il) __NANOSTL_NOEXCEPT
{
    return __il.end();
}

//===----------------------------------------------------------------------===//


}  // namespace nanostl

#endif // NANOSTL_INITIALIZER_LIST_H_
