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

#ifndef NANOSTL_CSTDDEF_H
#define NANOSTL_CSTDDEF_H

#include "nanocommon.h"

namespace nanostl {

template <class _Tp> struct __libcpp_is_integral                     { enum { value = 0 }; };
template <>          struct __libcpp_is_integral<bool>               { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<char>               { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<signed char>        { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<unsigned char>      { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<wchar_t>            { enum { value = 1 }; };
//#ifndef _LIBCPP_HAS_NO_CHAR8_T
//template <>          struct __libcpp_is_integral<char8_t>            { enum { value = 1 }; };
//#endif
//#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
//template <>          struct __libcpp_is_integral<char16_t>           { enum { value = 1 }; };
//template <>          struct __libcpp_is_integral<char32_t>           { enum { value = 1 }; };
//#endif
template <>          struct __libcpp_is_integral<short>              { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<unsigned short>     { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<int>                { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<unsigned int>       { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<long>               { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<unsigned long>      { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<long long>          { enum { value = 1 }; };
template <>          struct __libcpp_is_integral<unsigned long long> { enum { value = 1 }; };
//#ifndef _LIBCPP_HAS_NO_INT128
//template <>          struct __libcpp_is_integral<__int128_t>         { enum { value = 1 }; };
//template <>          struct __libcpp_is_integral<__uint128_t>        { enum { value = 1 }; };
//#endif


} // namespace nanostl


#endif // NANOSTL_CSTDDEF_H
