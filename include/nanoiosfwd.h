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

#ifndef NANOSTL_IOSFWD_H_
#define NANOSTL_IOSFWD_H_

namespace nanostl {

template <class CharT>
struct char_traits;

template <>
struct char_traits<char>;
// template<> struct char_traits<char8_t>;
template <>
struct char_traits<char16_t>;
template <>
struct char_traits<char32_t>;
template <>
struct char_traits<wchar_t>;

template <class charT, class traits = char_traits<charT> >
class basic_streambuf;
template <class _CharT, class _Traits = char_traits<_CharT> >
class basic_ostream;

typedef basic_streambuf<char> streambuf;
typedef basic_ostream<char> ostream;

template <class _State>             class fpos;

// TODO: Use mbstate_t
typedef fpos<long long>    streampos;

}  // namespace nanostl

#endif  // NANOSTL_IOSFWD_H_