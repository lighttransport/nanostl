/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2020 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_IOSTREAM_H_
#define NANOSTL_IOSTREAM_H_

#include "nanoiosfwd.h"
#include "nanoios.h"
#include "nanosstream.h"
#include "__nullptr"

namespace nanostl {

// Based on libcxx ----------------------

template <class _CharT, class _Traits>
class basic_ostream
    : virtual public basic_ios<_CharT, _Traits>
{
public:
    // types (inherited from basic_ios (27.5.4)):
    typedef _CharT                         char_type;
    typedef _Traits                        traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;


    basic_ostream& operator<<(bool __n);
    basic_ostream& operator<<(short __n);
    basic_ostream& operator<<(unsigned short __n);
    basic_ostream& operator<<(int __n);
    basic_ostream& operator<<(unsigned int __n);
    basic_ostream& operator<<(long __n);
    basic_ostream& operator<<(unsigned long __n);
    basic_ostream& operator<<(long long __n);
    basic_ostream& operator<<(unsigned long long __n);
    basic_ostream& operator<<(float __f);
    basic_ostream& operator<<(double __f);
    basic_ostream& operator<<(long double __f);
    basic_ostream& operator<<(const void* __p);

    basic_ostream& operator<<(basic_streambuf<char_type, traits_type>* __sb);

    inline
    basic_ostream& operator<<(nullptr_t)
    { return *this << "nullptr"; }

    // 27.7.2.7 Unformatted output:
    basic_ostream& put(char_type __c);
    basic_ostream& write(const char_type* __s, streamsize __n);
    basic_ostream& flush();

protected:
    // TODO: Uncomment
    //basic_ostream() {}

private:
    // FIXME
    stringstream _ss;

};

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(int __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}



// --------------------------------------

// Need to link with iostream.cc
extern ostream cout;
extern ostream cerr;

} // namespace nanostl

#endif // NANOSTL_IOSTREAM_H_
