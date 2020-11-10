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

#ifndef NANOSTL_IOS_H_
#define NANOSTL_IOS_H_

#include "nanotype_traits.h"

namespace nanostl {

class ios_base {
 public:
  typedef unsigned int fmtflags;
  static const fmtflags boolalpha = 0x0001;
  static const fmtflags dec = 0x0002;
  static const fmtflags fixed = 0x0004;
  static const fmtflags hex = 0x0008;
  static const fmtflags internal = 0x0010;
  static const fmtflags left = 0x0020;
  static const fmtflags oct = 0x0040;
  static const fmtflags right = 0x0080;
  static const fmtflags scientific = 0x0100;
  static const fmtflags showbase = 0x0200;
  static const fmtflags showpoint = 0x0400;
  static const fmtflags showpos = 0x0800;
  static const fmtflags skipws = 0x1000;
  static const fmtflags unitbuf = 0x2000;
  static const fmtflags uppercase = 0x4000;
  static const fmtflags adjustfield = left | right | internal;
  static const fmtflags basefield = dec | oct | hex;
  static const fmtflags floatfield = scientific | fixed;

  typedef unsigned int iostate;
  static const iostate badbit = 0x1;
  static const iostate eofbit = 0x2;
  static const iostate failbit = 0x4;
  static const iostate goodbit = 0x0;

  typedef unsigned int openmode;
  static const openmode app = 0x01;
  static const openmode ate = 0x02;
  static const openmode binary = 0x04;
  static const openmode in = 0x08;
  static const openmode out = 0x10;
  static const openmode trunc = 0x20;

  enum seekdir { beg, cur, end };

  // destructor
  virtual ~ios_base();


  inline bool good() const;
  inline bool eof() const;
  inline bool fail() const;
  inline bool bad() const;



private:
    ios_base(const ios_base&) = delete;
    ios_base& operator=(const ios_base&) = delete;

protected:
    ios_base() {
               }

};

// Based on libcxx ----------------------------

template <class _CharT, class _Traits>
class basic_ios
    : public ios_base
{
public:
    // types:
    typedef _CharT char_type;
    typedef _Traits traits_type;

    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    static_assert((is_same<_CharT, typename traits_type::char_type>::value),
                  "traits_type::char_type must be the same type as CharT");


    explicit operator bool() const {return !fail();}

    inline bool operator!() const    {return  fail();}
    //inline iostate rdstate() const   {return ios_base::rdstate();}
    //inline void clear(iostate __state = goodbit) {ios_base::clear(__state);}
    //inline void setstate(iostate __state) {ios_base::setstate(__state);}
    inline bool good() const {return ios_base::good();}
    inline bool eof() const  {return ios_base::eof();}
    inline bool fail() const {return ios_base::fail();}
    inline bool bad() const  {return ios_base::bad();}

};

}  // namespace nanostl

#endif  // NANOSTL_IOS_H_
