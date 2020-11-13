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

#ifdef NANOSTL_DEBUG
#include <stdio.h>
#endif

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
    //basic_ostream& operator<<(long __n);
    //basic_ostream& operator<<(unsigned long __n);
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
basic_ostream<_CharT, _Traits>::operator<<(bool __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(short __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(unsigned short __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(int __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(unsigned int __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(long long __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(unsigned long long __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(float __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template <class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
basic_ostream<_CharT, _Traits>::operator<<(double __n)
{

  _ss << nanostl::to_string(__n);

  return *this;
}

template<>
basic_ostream<char, nanostl::char_traits<char>>&
basic_ostream<char, nanostl::char_traits<char>>::operator<<(const void *__n)
{
  // FIXME: Assume const char *
  string str = nanostl::string(static_cast<const char *>(__n));
  _ss << str;

#ifdef NANOSTL_DEBUG
  printf("n : %s\n", str.c_str());
  printf("string: %s\n", _ss.str().c_str());
#endif
  return *this;
}


// --------------------------------------

// Need to link with iostream.cc
extern ostream cout;
extern ostream cerr;

} // namespace nanostl

#ifdef NANOSTL_IMPLEMENTATION
#ifndef NANOSTL_IOSTREAM_IMPLEMENTATION
#define NANOSTL_IOSTREAM_IMPLEMENTATION
#endif
#endif

#ifdef NANOSTL_IOSTREAM_IMPLEMENTATION

#ifndef NANOSTL_NO_IO
#include <stdio.h>
#endif

namespace nanostl {

#ifndef NANOSTL_NO_IO

#if __has_feature(cxx_alignas)
#define _ALIGNAS_TYPE(x) alignas(x)
#define _ALIGNAS(x) alignas(x)
#else
#error TODO
#define _ALIGNAS_TYPE(x) __attribute__((__aligned__(_LIBCPP_ALIGNOF(x))))
#define _ALIGNAS(x) __attribute__((__aligned__(x)))
#endif

ios_base::~ios_base() {
  // TODO
}

template<>
ostream& ostream::flush() {
  printf("%s", _ss.str().c_str());

  _ss.clear();

  return (*this);
}


//_ALIGNAS_TYPE (ostream) char cout[sizeof(ostream)];
ostream cout; // fixme

// from libcxx
class DoIOSInit {
 public:
  DoIOSInit();
  ~DoIOSInit();
};

DoIOSInit::DoIOSInit() {
  // force_locale_initialization();

#ifndef _LIBCPP_HAS_NO_STDOUT
  // ostream* cout_ptr = ::new(cout) ostream(::new(__cout)
  // __stdoutbuf<char>(stdout, &mb_cout));
#endif
  // ostream* cerr_ptr = ::new(cerr) ostream(::new(__cerr)
  // __stdoutbuf<char>(stderr, &mb_cerr));
  //                    ::new(clog) ostream(cerr_ptr->rdbuf());

  //_VSTD::unitbuf(*cerr_ptr);
#ifndef _LIBCPP_HAS_NO_STDOUT
  // cerr_ptr->tie(cout_ptr);
#endif
}

DoIOSInit::~DoIOSInit(){
#ifndef _LIBCPP_HAS_NO_STDOUT
// ostream* cout_ptr = reinterpret_cast<ostream*>(cout);
// wostream* wcout_ptr = reinterpret_cast<wostream*>(wcout);
// cout_ptr->flush();
// wcout_ptr->flush();
#endif

    // ostream* clog_ptr = reinterpret_cast<ostream*>(clog);
    // wostream* wclog_ptr = reinterpret_cast<wostream*>(wclog);
    // clog_ptr->flush();
    // wclog_ptr->flush();
}

ios_base::Init::Init() {
  static DoIOSInit init_the_streams;  // gets initialized once
}

ios_base::Init::~Init() {}

// compiler specific way
//__attribute__ ((__visibility__("hidden"))) ios_base::Init __start_std_streams
//__attribute__((init_priority(101)));

// Invoke Init() before main()
ios_base::Init __start_std_streams;

#else
#error TODO
#endif

}
#endif

#endif // NANOSTL_IOSTREAM_H_
