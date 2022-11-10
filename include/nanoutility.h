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

#ifndef NANOSTL_UTILITY_H_
#define NANOSTL_UTILITY_H_

#include "nanocommon.h"
#include "nanotype_traits.h"

namespace nanostl {

template <class T1, class T2>
struct pair {
  T1 first;
  T2 second;
  pair() {}
  pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
  return pair<T1, T2>(x, y);
}

// swap is defined in nanotype_traits
#if 0
template <class T>
inline void swap(T &a, T &b) __NANOSTL_NOEXCEPT
{
  T tmp;
  tmp = a;
  a = b;
  b = tmp;
}
#endif

#if 0 // moved to nanotype_traits
//#if __has_keyword(__remove_reference)
//
//template<class _Tp>
//struct _LIBCPP_TEMPLATE_VIS remove_reference { typedef __remove_reference(_Tp) type; };
//
//#else // __has_keyword(__remove_reference)

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_reference        {typedef /*_LIBCPP_NODEBUG*/ _Tp type;}  ;
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_reference<_Tp&>  {typedef /*_LIBCPP_NODEBUG*/ _Tp type;}  ;
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_reference<_Tp&&> {typedef /*_LIBCPP_NODEBUG*/ _Tp type;}  ;

//#if _LIBCPP_STD_VER > 11
template <class _Tp> using remove_reference_t = typename remove_reference<_Tp>::type;
//#endif


template <class _Tp>
/*_LIBCPP_NODISCARD_EXT*/ inline /*_LIBCPP_INLINE_VISIBILITY*/ constexpr typename remove_reference<_Tp>::type&&
move(_Tp&& __t) __NANOSTL_NOEXCEPT {
  typedef /*_LIBCPP_NODEBUG*/ typename remove_reference<_Tp>::type _Up;
  return static_cast<_Up&&>(__t);
}
#endif

#ifndef _LIBCPP_CXX03_LANG
template <class _Tp>
using __move_if_noexcept_result_t =
    typename conditional<!is_nothrow_move_constructible<_Tp>::value && is_copy_constructible<_Tp>::value, const _Tp&,
                         _Tp&&>::type;
#else // _LIBCPP_CXX03_LANG
template <class _Tp>
using __move_if_noexcept_result_t = const _Tp&;
#endif

template <class _Tp>
/*_LIBCPP_NODISCARD_EXT*/ inline /*_LIBCPP_INLINE_VISIBILITY*/ constexpr __move_if_noexcept_result_t<_Tp>
move_if_noexcept(_Tp& __x) __NANOSTL_NOEXCEPT {
  return move(__x);
}


//template <class T> typename add_rvalue_reference<T>::type declval() noexcept;

template <class _Tp>
/*_LIBCPP_NODISCARD_EXT*/ inline /*_LIBCPP_INLINE_VISIBILITY*/ constexpr _Tp&&
forward(typename remove_reference<_Tp>::type& __t) __NANOSTL_NOEXCEPT {
  return static_cast<_Tp&&>(__t);
}

template <class _Tp>
/*_LIBCPP_NODISCARD_EXT*/ inline /*_LIBCPP_INLINE_VISIBILITY*/ constexpr _Tp&&
forward(typename remove_reference<_Tp>::type&& __t) __NANOSTL_NOEXCEPT {
  static_assert(!is_lvalue_reference<_Tp>::value, "cannot forward an rvalue as an lvalue");
  return static_cast<_Tp&&>(__t);
}

template <class _Tp>
inline /*_LIBCPP_INLINE_VISIBILITY*/ typename decay<_Tp>::type __decay_copy(_Tp&& __t)
//#if _LIBCPP_STD_VER > 17
//    noexcept(is_nothrow_convertible_v<_Tp, remove_reference_t<_Tp> >)
//#endif
{
  return nanostl::forward<_Tp>(__t);
}


}  // namespace nanostl

#endif  // NANOSTL_UTILITY_H_
