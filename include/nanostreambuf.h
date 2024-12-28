/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_STREAMBUF_H_
#define NANOSTL_STREAMBUF_H_

#include "nanostring.h"
#include "nanotype_traits.h"
#include "nanoiosfwd.h"

namespace nanostl {

template <class _CharT, class _Traits>
class basic_streambuf {
 public:
  // types:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  static_assert((is_same<_CharT, typename traits_type::char_type>::value),
                "traits_type::char_type must be the same type as CharT");

  virtual ~basic_streambuf();

 protected:
  basic_streambuf();
  basic_streambuf(const basic_streambuf& __rhs);
  basic_streambuf& operator=(const basic_streambuf& __rhs);
  void swap(basic_streambuf& __rhs);
};

//extern template class basic_streambuf<char>;

//typedef basic_streambuf<char> streambuf;

}  // namespace nanostl

#endif  // NANOSTL_STREAMBUF_H_
