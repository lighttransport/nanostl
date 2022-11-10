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

//
// Based on libcxx: Apache-2.0 WITH LLVM-exception
//
#ifndef NANOSTL_TYPE_TRAITS_H_
#define NANOSTL_TYPE_TRAITS_H_

#include "nanocommon.h"
#include "nanocstddef.h"
//#include "nanoutility.h" // swap
#include "__nullptr"

namespace nanostl {

// helper class:
template <class T, T v>
struct integral_constant;
typedef integral_constant<bool, true> true_type;    // C++11
typedef integral_constant<bool, false> false_type;  // C++11

template <bool B>                                  // C++14
using bool_constant = integral_constant<bool, B>;  // C++14
typedef bool_constant<true> true_type;             // C++14
typedef bool_constant<false> false_type;           // C++14

// helper traits
template <bool, class T = void>
struct enable_if;
// template <bool, class T, class F> struct conditional;

template <bool _Bp, class _If, class _Then>
    struct _NANOSTL_TEMPLATE_VIS conditional {typedef _If type;};
template <class _If, class _Then>
    struct _NANOSTL_TEMPLATE_VIS conditional<false, _If, _Then> {typedef _Then type;};

// Primary classification traits:
//template <class T>
//struct is_void;
// template <class T> struct is_null_pointer;  // C++14
//template <class T>
//struct is_integral;
//template <class T>
//struct is_floating_point;

//template <class T>
//struct is_pointer;
// template <class T> struct is_lvalue_reference;
// template <class T> struct is_rvalue_reference;
// template <class T> struct is_member_object_pointer;
// template <class T> struct is_member_function_pointer;
// template <class T> struct is_enum;
// template <class T> struct is_union;
template <class T> struct is_class;
// template <class T> struct is_function;

    template <class T> struct remove_const;
    template <class T> struct remove_volatile;
 template <class T> struct remove_cv;


template <class T, class U> struct is_same;


template <bool b, class T = void>
using enable_if_t = typename enable_if<b, T>::type;  // C++14


template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_const            {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_const<const _Tp> {typedef _Tp type;};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_volatile               {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_volatile<volatile _Tp> {typedef _Tp type;};


template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_cv
{typedef typename remove_volatile<typename remove_const<_Tp>::type>::type type;};


template <bool, class T> struct _NANOSTL_TEMPLATE_VIS enable_if {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS enable_if<true, _Tp> {typedef _Tp type;};


typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;


template <class _Tp, _Tp __v>
struct _NANOSTL_TEMPLATE_VIS integral_constant
{
  static constexpr const _Tp      value = __v;
  typedef _Tp               value_type;
  typedef integral_constant type;
  constexpr operator value_type() const noexcept {return value;}
};

template <class _Tp, _Tp __v>
constexpr const _Tp integral_constant<_Tp, __v>::value;


template <class _Tp, class _Up> struct _NANOSTL_TEMPLATE_VIS is_same           : public false_type {};
template <class _Tp>            struct _NANOSTL_TEMPLATE_VIS is_same<_Tp, _Tp> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_void
    : public is_same<typename remove_cv<_Tp>::type, void> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_void_v = is_void<_Tp>::value;
//#endif


// is_array

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_array
    : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_array<_Tp[]>
    : public true_type {};
template <class _Tp, size_t _Np> struct _NANOSTL_TEMPLATE_VIS is_array<_Tp[_Np]>
    : public true_type {};


// remove_extent

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_extent
    {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_extent<_Tp[]>
    {typedef _Tp type;};
template <class _Tp, size_t _Np> struct _NANOSTL_TEMPLATE_VIS remove_extent<_Tp[_Np]>
    {typedef _Tp type;};


template <bool _Val>
using _BoolConstant = integral_constant<bool, _Val>;

// is_integral

// __libcpp_is_integral defined in nanocstddef.h
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_integral
    : public _BoolConstant<__libcpp_is_integral<typename remove_cv<_Tp>::type>::value> {};



template <class _Tp> struct __libcpp_is_floating_point              : public false_type {};
template <>          struct __libcpp_is_floating_point<float>       : public true_type {};
template <>          struct __libcpp_is_floating_point<double>      : public true_type {};
template <>          struct __libcpp_is_floating_point<long double> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_floating_point
    : public __libcpp_is_floating_point<typename remove_cv<_Tp>::type> {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_arithmetic
    : public integral_constant<bool, is_integral<_Tp>::value      ||
                                     is_floating_point<_Tp>::value> {};


#if _NANOSTL_STD_VER > 14
template <bool __b>
using bool_constant = integral_constant<bool, __b>;
#define _NANOSTL_BOOL_CONSTANT(__b) bool_constant<(__b)>
#else
#define _NANOSTL_BOOL_CONSTANT(__b) integral_constant<bool,(__b)>
#endif

template <class _Tp, bool = is_integral<_Tp>::value>
struct __libcpp_is_signed_impl : public _NANOSTL_BOOL_CONSTANT(_Tp(-1) < _Tp(0)) {};

template <class _Tp>
struct __libcpp_is_signed_impl<_Tp, false> : public true_type {};  // floating point

template <class _Tp, bool = is_arithmetic<_Tp>::value>
struct __libcpp_is_signed : public __libcpp_is_signed_impl<_Tp> {};

template <class _Tp> struct __libcpp_is_signed<_Tp, false> : public false_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_signed : public __libcpp_is_signed<_Tp> {};


template <class _Tp, bool = is_integral<_Tp>::value>
struct __libcpp_is_unsigned_impl : public _NANOSTL_BOOL_CONSTANT(_Tp(0) < _Tp(-1)) {};

template <class _Tp>
struct __libcpp_is_unsigned_impl<_Tp, false> : public false_type {};  // floating point

template <class _Tp, bool = is_arithmetic<_Tp>::value>
struct __libcpp_is_unsigned : public __libcpp_is_unsigned_impl<_Tp> {};

template <class _Tp> struct __libcpp_is_unsigned<_Tp, false> : public false_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_unsigned : public __libcpp_is_unsigned<_Tp> {};


// since libcxx uses compiler's intrinsic function __is_abstract, use boost's one instead.
//
//  (C) Copyright John Maddock 2015.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

typedef char yes_type;
struct no_type {
  char padding[8];
};

template<class T>
struct is_abstract_imp2
{
   // Deduction fails if T is void, function type,
   // reference type (14.8.2/2)or an abstract class type
   // according to review status issue #337
   //
   template<class U>
   static no_type check_sig(U (*)[1]);
   template<class U>
   static yes_type check_sig(...);
   //
   // T must be a complete type, further if T is a template then
   // it must be instantiated in order for us to get the right answer:
   //
   static_assert(sizeof(T) != 0, "T must be a complete type");

   // GCC2 won't even parse this template if we embed the computation
   // of s1 in the computation of value.
#ifdef __GNUC__
   static const size_t s1 = sizeof(is_abstract_imp2<T>::template check_sig<T>(0));
#else
//#if BOOST_WORKAROUND(BOOST_MSVC_FULL_VER, >= 140050000)
//#pragma warning(push)
//#pragma warning(disable:6334)
//#endif
   static const size_t s1 = sizeof(check_sig<T>(0));
//#if BOOST_WORKAROUND(BOOST_MSVC_FULL_VER, >= 140050000)
//#pragma warning(pop)
//#endif
#endif

   static const bool value =
      (s1 == sizeof(yes_type));
};

template <bool v>
struct is_abstract_select
{
   template <class T>
   struct rebind
   {
      typedef is_abstract_imp2<T> type;
   };
};
template <>
struct is_abstract_select<false>
{
   template <class T>
   struct rebind
   {
      typedef false_type type;
   };
};

template <class T>
struct is_abstract_imp
{
   typedef is_abstract_select< is_class<T>::value> selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type type;

   static const bool value = type::value;
};

// -----------------------------------


// is_abstract

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_abstract
    : public integral_constant<bool, __is_abstract(_Tp)> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_abstract_v = is_abstract<_Tp>::value;
//#endif

// is_final

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS __libcpp_is_final
    : public integral_constant<bool, __is_final(_Tp)> {};

//#if _LIBCPP_STD_VER > 11
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS
is_final : public integral_constant<bool, __is_final(_Tp)> {};
//#endif
//
//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_final_v = is_final<_Tp>::value;
//#endif


// remove_reference

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

// is_reference

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_lvalue_reference       : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_lvalue_reference<_Tp&> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_rvalue_reference        : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_rvalue_reference<_Tp&&> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_reference        : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_reference<_Tp&>  : public true_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_reference<_Tp&&> : public true_type {};

// __uncvref

template <class _Tp>
struct __uncvref  {
    typedef typename remove_cv<typename remove_reference<_Tp>::type>::type type;
};

template <class _Tp>
struct __unconstref {
    typedef typename remove_const<typename remove_reference<_Tp>::type>::type type;
};

//#ifndef _LIBCPP_CXX03_LANG
template <class _Tp>
using __uncvref_t = typename __uncvref<_Tp>::type;
//#endif



// is_const

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_const            : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_const<_Tp const> : public true_type {};


template <class _Tp, class _Up>
using _IsSame = _BoolConstant<
    is_same<_Tp, _Up>::value
>;

template <class _Tp, class _Up>
using _IsNotSame = _BoolConstant<
    !is_same<_Tp, _Up>::value
>;

struct __two {char __lx[2];};

// __is_referenceable  [defns.referenceable]

struct __is_referenceable_impl {
    template <class _Tp> static _Tp& __test(int);
    template <class _Tp> static __two __test(...);
};

template <class _Tp>
struct __is_referenceable : integral_constant<bool,
    _IsNotSame<decltype(__is_referenceable_impl::__test<_Tp>(0)), __two>::value> {};

// add_pointer

template <class _Tp,
        bool = __is_referenceable<_Tp>::value ||
                is_same<typename remove_cv<_Tp>::type, void>::value>
struct __add_pointer_impl
    {typedef  typename remove_reference<_Tp>::type* type;};
template <class _Tp> struct __add_pointer_impl<_Tp, false>
    {typedef  _Tp type;};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS add_pointer
    {typedef  typename __add_pointer_impl<_Tp>::type type;};

//#if _LIBCPP_STD_VER > 11
template <class _Tp> using add_pointer_t = typename add_pointer<_Tp>::type;
//#endif


// is_function

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_function
    : public _BoolConstant<
 !(is_reference<_Tp>::value || is_const<const _Tp>::value)
    > {};


// decay

template <class _Up, bool>
struct __decay {
    typedef typename remove_cv<_Up>::type type;
};

template <class _Up>
struct __decay<_Up, true> {
public:
    typedef typename conditional
                     <
                         is_array<_Up>::value,
                         typename remove_extent<_Up>::type*,
                         typename conditional
                         <
                              is_function<_Up>::value,
                              typename add_pointer<_Up>::type,
                              typename remove_cv<_Up>::type
                         >::type
                     >::type type;
};

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS decay
{
private:
    typedef typename remove_reference<_Tp>::type _Up;
public:
    typedef typename __decay<_Up, __is_referenceable<_Up>::value>::type type;
};

// Suppress deprecation notice for volatile-qualified return type resulting
// from volatile-qualified types _Tp.
//_LIBCPP_SUPPRESS_DEPRECATED_PUSH
template <class _Tp> _Tp&& __declval(int);
template <class _Tp> _Tp   __declval(long);
//_LIBCPP_SUPPRESS_DEPRECATED_POP

template <class _Tp>
decltype(__declval<_Tp>(0))
declval() __NANOSTL_NOEXCEPT;



template <class>
struct __void_t { typedef void type; };

template <class _Tp>
struct __identity { typedef _Tp type; };

// common_type

template <class _Tp, class _Up, class = void>
struct __common_type2_imp {};

// sub-bullet 3 - "if decay_t<decltype(false ? declval<D1>() : declval<D2>())> ..."
template <class _Tp, class _Up>
struct __common_type2_imp<_Tp, _Up,
                          typename __void_t<decltype(
                                            true ? declval<_Tp>() : declval<_Up>()
                                            )>::type>
{
  typedef typename decay<decltype(
                         true ? declval<_Tp>() : declval<_Up>()
                         )>::type type;
};

template <class, class = void>
struct __common_type_impl {};


# define _LIBCPP_OPTIONAL_PACK(...) , __VA_ARGS__

template <class... Tp>
struct __common_types;
template <class... _Tp>
struct _NANOSTL_TEMPLATE_VIS common_type;


template <class _Tp, class _Up>
struct __common_type_impl<
    __common_types<_Tp, _Up>,
    typename __void_t<typename common_type<_Tp, _Up>::type>::type>
{
  typedef typename common_type<_Tp, _Up>::type type;
};

template <class _Tp, class _Up, class _Vp _LIBCPP_OPTIONAL_PACK(class... _Rest)>
struct __common_type_impl<
    __common_types<_Tp, _Up, _Vp _LIBCPP_OPTIONAL_PACK(_Rest...)>,
    typename __void_t<typename common_type<_Tp, _Up>::type>::type>
    : __common_type_impl<__common_types<typename common_type<_Tp, _Up>::type,
                                        _Vp _LIBCPP_OPTIONAL_PACK(_Rest...)> > {
};


// bullet 1 - sizeof...(Tp) == 0

template <>
struct _NANOSTL_TEMPLATE_VIS common_type<> {};

// bullet 2 - sizeof...(Tp) == 1

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS common_type<_Tp>
    : public common_type<_Tp, _Tp> {};

// bullet 3 - sizeof...(Tp) == 2

// sub-bullet 1 - "If is_same_v<T1, D1> is false or ..."
template <class _Tp, class _Up>
struct _NANOSTL_TEMPLATE_VIS common_type<_Tp, _Up>
    : conditional<
        _IsSame<_Tp, typename decay<_Tp>::type>::value && _IsSame<_Up, typename decay<_Up>::type>::value,
        __common_type2_imp<_Tp, _Up>,
        common_type<typename decay<_Tp>::type, typename decay<_Up>::type>
    >::type
{};

// bullet 4 - sizeof...(Tp) > 2

template <class _Tp, class _Up, class _Vp _LIBCPP_OPTIONAL_PACK(class... _Rest)>
struct _NANOSTL_TEMPLATE_VIS
    common_type<_Tp, _Up, _Vp _LIBCPP_OPTIONAL_PACK(_Rest...)>
    : __common_type_impl<
          __common_types<_Tp, _Up, _Vp _LIBCPP_OPTIONAL_PACK(_Rest...)> > {};


#undef _LIBCPP_OPTIONAL_PACK

// is_convertible

namespace __is_convertible_imp
{
template <class _Tp> void  __test_convert(_Tp);

template <class _From, class _To, class = void>
struct __is_convertible_test : public false_type {};

template <class _From, class _To>
struct __is_convertible_test<_From, _To,
    decltype(__is_convertible_imp::__test_convert<_To>(declval<_From>()))> : public true_type
{};

template <class _Tp, bool _IsArray =    is_array<_Tp>::value,
                     bool _IsFunction = is_function<_Tp>::value,
                     bool _IsVoid =     is_void<_Tp>::value>
                     struct __is_array_function_or_void                          {enum {value = 0};};
template <class _Tp> struct __is_array_function_or_void<_Tp, true, false, false> {enum {value = 1};};
template <class _Tp> struct __is_array_function_or_void<_Tp, false, true, false> {enum {value = 2};};
template <class _Tp> struct __is_array_function_or_void<_Tp, false, false, true> {enum {value = 3};};
}

template <class _Tp,
    unsigned = __is_convertible_imp::__is_array_function_or_void<typename remove_reference<_Tp>::type>::value>
struct __is_convertible_check
{
    static const size_t __v = 0;
};

template <class _Tp>
struct __is_convertible_check<_Tp, 0>
{
    static const size_t __v = sizeof(_Tp);
};

template <class _T1, class _T2,
    unsigned _T1_is_array_function_or_void = __is_convertible_imp::__is_array_function_or_void<_T1>::value,
    unsigned _T2_is_array_function_or_void = __is_convertible_imp::__is_array_function_or_void<_T2>::value>
struct __nanostl_is_convertible
    : public integral_constant<bool,
        __is_convertible_imp::__is_convertible_test<_T1, _T2>::value
    >
{};

template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 0, 1> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 1, 1> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 2, 1> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 3, 1> : public false_type {};

template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 0, 2> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 1, 2> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 2, 2> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 3, 2> : public false_type {};

template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 0, 3> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 1, 3> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 2, 3> : public false_type {};
template <class _T1, class _T2> struct __nanostl_is_convertible<_T1, _T2, 3, 3> : public true_type {};

template <class _T1, class _T2> struct _NANOSTL_TEMPLATE_VIS is_convertible
    : public __nanostl_is_convertible<_T1, _T2>
{
    static const size_t __complete_check1 = __is_convertible_check<_T1>::__v;
    static const size_t __complete_check2 = __is_convertible_check<_T2>::__v;
};

// remove_all_extents

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_all_extents
    {typedef _Tp type;};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS remove_all_extents<_Tp[]>
    {typedef typename remove_all_extents<_Tp>::type type;};
template <class _Tp, size_t _Np> struct _NANOSTL_TEMPLATE_VIS remove_all_extents<_Tp[_Np]>
    {typedef typename remove_all_extents<_Tp>::type type;};

//#if _LIBCPP_STD_VER > 11
template <class _Tp> using remove_all_extents_t = typename remove_all_extents<_Tp>::type;
//#endif


// is_destructible

//  if it's a reference, return true
//  if it's a function, return false
//  if it's   void,     return false
//  if it's an array of unknown bound, return false
//  Otherwise, return "declval<_Up&>().~_Up()" is well-formed
//    where _Up is remove_all_extents<_Tp>::type

template <class>
struct __is_destructible_apply { typedef int type; };

template <typename _Tp>
struct __is_destructor_wellformed {
    template <typename _Tp1>
    static char  __test (
        typename __is_destructible_apply<decltype(declval<_Tp1&>().~_Tp1())>::type
    );

    template <typename _Tp1>
    static __two __test (...);

    static const bool value = sizeof(__test<_Tp>(12)) == sizeof(char);
};

template <class _Tp, bool>
struct __destructible_imp;

template <class _Tp>
struct __destructible_imp<_Tp, false>
   : public integral_constant<bool,
        __is_destructor_wellformed<typename remove_all_extents<_Tp>::type>::value> {};

template <class _Tp>
struct __destructible_imp<_Tp, true>
    : public true_type {};

template <class _Tp, bool>
struct __destructible_false;

template <class _Tp>
struct __destructible_false<_Tp, false> : public __destructible_imp<_Tp, is_reference<_Tp>::value> {};

template <class _Tp>
struct __destructible_false<_Tp, true> : public false_type {};

template <class _Tp>
struct is_destructible
    : public __destructible_false<_Tp, is_function<_Tp>::value> {};

template <class _Tp>
struct is_destructible<_Tp[]>
    : public false_type {};

template <>
struct is_destructible<void>
    : public false_type {};


template <class T, class... Args> struct is_constructible;
template <class T>                struct is_default_constructible;

// is_default_constructible

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS is_default_constructible
    : public is_constructible<_Tp>
    {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_default_constructible_v = is_default_constructible<_Tp>::value;
//#endif

#ifndef _LIBCPP_CXX03_LANG
// First of all, we can't implement this check in C++03 mode because the {}
// default initialization syntax isn't valid.
// Second, we implement the trait in a funny manner with two defaulted template
// arguments to workaround Clang's PR43454.
template <class _Tp>
void __test_implicit_default_constructible(_Tp);

template <class _Tp, class = void, class = typename is_default_constructible<_Tp>::type>
struct __is_implicitly_default_constructible
    : false_type
{ };

template <class _Tp>
struct __is_implicitly_default_constructible<_Tp, decltype(__test_implicit_default_constructible<_Tp const&>({})), true_type>
    : true_type
{ };

template <class _Tp>
struct __is_implicitly_default_constructible<_Tp, decltype(__test_implicit_default_constructible<_Tp const&>({})), false_type>
    : false_type
{ };
#endif // !C++03

// is_constructible
//
// since libcxx uses compiler's intrinsic function __is_constructible, use boost's one instead.
//
//  (C) Copyright John Maddock 2015.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

   namespace detail{

      struct is_constructible_imp
      {
         template<typename T, typename ...TheArgs, typename = decltype(T(declval<TheArgs>()...))>
         static yes_type test(int);
         template<typename, typename...>
         static no_type test(...);

         template<typename T, typename Arg, typename = decltype(::new T(declval<Arg>()))>
         static yes_type test1(int);
         template<typename, typename>
         static no_type test1(...);

         template <typename T>
         static yes_type ref_test(T);
         template <typename T>
         static no_type ref_test(...);
      };

   }

   template <class T, class ...Args> struct is_constructible : public integral_constant<bool, sizeof(detail::is_constructible_imp::test<T, Args...>(0)) == sizeof(yes_type)>{};
   template <class T, class Arg> struct is_constructible<T, Arg> : public integral_constant<bool, is_destructible<T>::value && sizeof(detail::is_constructible_imp::test1<T, Arg>(0)) == sizeof(yes_type)>{};
   template <class Ref, class Arg> struct is_constructible<Ref&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&>(declval<Arg>())) == sizeof(yes_type)>{};
   template <class Ref, class Arg> struct is_constructible<Ref&&, Arg> : public integral_constant<bool, sizeof(detail::is_constructible_imp::ref_test<Ref&&>(declval<Arg>())) == sizeof(yes_type)>{};

   template <> struct is_constructible<void> : public false_type{};
   template <> struct is_constructible<void const> : public false_type{};
   template <> struct is_constructible<void const volatile> : public false_type{};
   template <> struct is_constructible<void volatile> : public false_type{};

   //template <class T> struct is_constructible<T> : public is_default_constructible<T>{};

// ----------------------------------------------------------------------------------------------------------



// is_union

template <class _Tp> struct __libcpp_union : public false_type {};
template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_union
    : public __libcpp_union<typename remove_cv<_Tp>::type> {};

// is_class

namespace __is_class_imp
{
template <class _Tp> char  __test(int _Tp::*);
template <class _Tp> __two __test(...);
}

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_class
    : public integral_constant<bool, sizeof(__is_class_imp::__test<_Tp>(0)) == 1 && !is_union<_Tp>::value> {};



// is_empty

template <class _Tp>
struct __is_empty1
    : public _Tp
{
    double __lx;
};

struct __is_empty2
{
    double __lx;
};

template <class _Tp, bool = is_class<_Tp>::value>
struct __libcpp_empty : public integral_constant<bool, sizeof(__is_empty1<_Tp>) == sizeof(__is_empty2)> {};

template <class _Tp> struct __libcpp_empty<_Tp, false> : public false_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_empty : public __libcpp_empty<_Tp> {};


// is_assignable

template<typename, typename _Tp> struct __select_2nd { typedef /*_LIBCPP_NODEBUG*/ _Tp type; };

template <class _Tp, class _Arg>
typename __select_2nd<decltype((declval<_Tp>() = declval<_Arg>())), true_type>::type
__is_assignable_test(int);

template <class, class>
false_type __is_assignable_test(...);


template <class _Tp, class _Arg, bool = is_void<_Tp>::value || is_void<_Arg>::value>
struct __is_assignable_imp
    : public decltype((__is_assignable_test<_Tp, _Arg>(0))) {};

template <class _Tp, class _Arg>
struct __is_assignable_imp<_Tp, _Arg, true>
    : public false_type
{
};

template <class _Tp, class _Arg>
struct is_assignable
    : public __is_assignable_imp<_Tp, _Arg> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp, class _Arg>
//inline constexpr bool is_assignable_v = is_assignable<_Tp, _Arg>::value;
//#endif


// is_nothrow_constructible

template <bool, bool, class _Tp, class... _Args> struct __libcpp_is_nothrow_constructible;

template <class _Tp, class... _Args>
struct __libcpp_is_nothrow_constructible</*is constructible*/true, /*is reference*/false, _Tp, _Args...>
    : public integral_constant<bool, noexcept(_Tp(declval<_Args>()...))>
{
};

template <class _Tp>
void __nanostl_implicit_conversion_to(_Tp) noexcept { }

template <class _Tp, class _Arg>
struct __libcpp_is_nothrow_constructible</*is constructible*/true, /*is reference*/true, _Tp, _Arg>
    : public integral_constant<bool, noexcept(__nanostl_implicit_conversion_to<_Tp>(declval<_Arg>()))>
{
};

template <class _Tp, bool _IsReference, class... _Args>
struct __libcpp_is_nothrow_constructible</*is constructible*/false, _IsReference, _Tp, _Args...>
    : public false_type
{
};

template <class _Tp, class... _Args>
struct _NANOSTL_TEMPLATE_VIS is_nothrow_constructible
    : __libcpp_is_nothrow_constructible<is_constructible<_Tp, _Args...>::value, is_reference<_Tp>::value, _Tp, _Args...>
{
};

template <class _Tp, size_t _Ns>
struct _NANOSTL_TEMPLATE_VIS is_nothrow_constructible<_Tp[_Ns]>
    : __libcpp_is_nothrow_constructible<is_constructible<_Tp>::value, is_reference<_Tp>::value, _Tp>
{
};

// is_nothrow_default_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_nothrow_default_constructible
    : public is_nothrow_constructible<_Tp>
    {};


// is_nothrow_assignable

template <bool, class _Tp, class _Arg> struct __libcpp_is_nothrow_assignable;

template <class _Tp, class _Arg>
struct __libcpp_is_nothrow_assignable<false, _Tp, _Arg>
    : public false_type
{
};

template <class _Tp, class _Arg>
struct __libcpp_is_nothrow_assignable<true, _Tp, _Arg>
    : public integral_constant<bool, noexcept(declval<_Tp>() = declval<_Arg>()) >
{
};

template <class _Tp, class _Arg>
struct _NANOSTL_TEMPLATE_VIS is_nothrow_assignable
    : public __libcpp_is_nothrow_assignable<is_assignable<_Tp, _Arg>::value, _Tp, _Arg>
{
};


// add_lvalue_reference

template <class _Tp, bool = __is_referenceable<_Tp>::value> struct __add_lvalue_reference_impl            { typedef /*_LIBCPP_NODEBUG*/ _Tp  type; };
template <class _Tp                                       > struct __add_lvalue_reference_impl<_Tp, true> { typedef /*_LIBCPP_NODEBUG*/ _Tp& type; };

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS add_lvalue_reference
{typedef /*_LIBCPP_NODEBUG*/ typename  __add_lvalue_reference_impl<_Tp>::type type;};

//#if _LIBCPP_STD_VER > 11
//template <class _Tp> using add_lvalue_reference_t = typename add_lvalue_reference<_Tp>::type;
//#endif

// add_rvalue_reference

template <class _Tp, bool = __is_referenceable<_Tp>::value> struct __add_rvalue_reference_impl            { typedef /*_LIBCPP_NODEBUG*/ _Tp   type; };
template <class _Tp                                       > struct __add_rvalue_reference_impl<_Tp, true> { typedef /*_LIBCPP_NODEBUG*/ _Tp&& type; };

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS add_rvalue_reference
{typedef /*_LIBCPP_NODEBUG*/ typename __add_rvalue_reference_impl<_Tp>::type type;};

//#if _LIBCPP_STD_VER > 11
//template <class _Tp> using add_rvalue_reference_t = typename add_rvalue_reference<_Tp>::type;
//#endif

// add_const

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS add_const {
  typedef /*_LIBCPP_NODEBUG*/ const _Tp type;
};

// is_copy_constructible

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS is_copy_constructible
    : public is_constructible<_Tp,
                  typename add_lvalue_reference<typename add_const<_Tp>::type>::type> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_copy_constructible_v = is_copy_constructible<_Tp>::value;
//#endif

// is_move_constructible

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS is_move_constructible
    : public is_constructible<_Tp, typename add_rvalue_reference<_Tp>::type>
    {};


// is_copy_assignable

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_copy_assignable
    : public is_assignable<typename add_lvalue_reference<_Tp>::type,
                  typename add_lvalue_reference<typename add_const<_Tp>::type>::type> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_copy_assignable_v = is_copy_assignable<_Tp>::value;
//#endif


// is_move_assignable

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_move_assignable
    : public is_assignable<typename add_lvalue_reference<_Tp>::type,
                           typename add_rvalue_reference<_Tp>::type> {};


// is_nothrow_copy_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_nothrow_copy_constructible
    : public is_nothrow_constructible<_Tp,
                  typename add_lvalue_reference<typename add_const<_Tp>::type>::type> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<_Tp>::value;
//#endif

// is_nothrow_move_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_nothrow_move_constructible
    : public is_nothrow_constructible<_Tp, typename add_rvalue_reference<_Tp>::type>
    {};

// is_nothrow_copy_assignable

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_nothrow_copy_assignable
    : public is_nothrow_assignable<typename add_lvalue_reference<_Tp>::type,
                  typename add_lvalue_reference<typename add_const<_Tp>::type>::type> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<_Tp>::value;
//#endif

// is_nothrow_move_assignable

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_nothrow_move_assignable
    : public is_nothrow_assignable<typename add_lvalue_reference<_Tp>::type,
                                     typename add_rvalue_reference<_Tp>::type>
    {};

// aligned_storage

template <class _Hp, class _Tp>
struct __type_list
{
    typedef _Hp _Head;
    typedef _Tp _Tail;
};


struct __nat
{
//#ifndef _LIBCPP_CXX03_LANG
    __nat() = delete;
    __nat(const __nat&) = delete;
    __nat& operator=(const __nat&) = delete;
    ~__nat() = delete;
//#endif
};

template <class _Tp>
struct __align_type
{
    static const size_t value = alignof(_Tp); /*_LIBCPP_PREFERRED_ALIGNOF(_Tp);*/
    typedef _Tp type;
};

struct __struct_double {long double __lx;};
struct __struct_double4 {double __lx[4];};

typedef
    __type_list<__align_type<unsigned char>,
    __type_list<__align_type<unsigned short>,
    __type_list<__align_type<unsigned int>,
    __type_list<__align_type<unsigned long>,
    __type_list<__align_type<unsigned long long>,
    __type_list<__align_type<double>,
    __type_list<__align_type<long double>,
    __type_list<__align_type<__struct_double>,
    __type_list<__align_type<__struct_double4>,
    __type_list<__align_type<int*>,
    __nat
    > > > > > > > > > > __all_types;

template <size_t _Align>
struct alignas(_Align) __fallback_overaligned {};

template <class _TL, size_t _Align> struct __find_pod;

template <class _Hp, size_t _Align>
struct __find_pod<__type_list<_Hp, __nat>, _Align>
{
    typedef typename conditional<
                             _Align == _Hp::value,
                             typename _Hp::type,
                             __fallback_overaligned<_Align>
                         >::type type;
};

template <class _Hp, class _Tp, size_t _Align>
struct __find_pod<__type_list<_Hp, _Tp>, _Align>
{
    typedef typename conditional<
                             _Align == _Hp::value,
                             typename _Hp::type,
                             typename __find_pod<_Tp, _Align>::type
                         >::type type;
};

template <class _TL, size_t _Len> struct __find_max_align;

template <class _Hp, size_t _Len>
struct __find_max_align<__type_list<_Hp, __nat>, _Len> : public integral_constant<size_t, _Hp::value> {};

template <size_t _Len, size_t _A1, size_t _A2>
struct __select_align
{
private:
    static const size_t __min = _A2 < _A1 ? _A2 : _A1;
    static const size_t __max = _A1 < _A2 ? _A2 : _A1;
public:
    static const size_t value = _Len < __max ? __min : __max;
};

template <class _Hp, class _Tp, size_t _Len>
struct __find_max_align<__type_list<_Hp, _Tp>, _Len>
    : public integral_constant<size_t, __select_align<_Len, _Hp::value, __find_max_align<_Tp, _Len>::value>::value> {};

template <size_t _Len, size_t _Align = __find_max_align<__all_types, _Len>::value>
struct _NANOSTL_TEMPLATE_VIS aligned_storage
{
    typedef typename __find_pod<__all_types, _Align>::type _Aligner;
    union type
    {
        _Aligner __align;
        unsigned char __data[(_Len + _Align - 1)/_Align * _Align];
    };
};

#if _LIBCPP_STD_VER > 11
template <size_t _Len, size_t _Align = __find_max_align<__all_types, _Len>::value>
    using aligned_storage_t = typename aligned_storage<_Len, _Align>::type;
#endif

#define _CREATE_ALIGNED_STORAGE_SPECIALIZATION(n) \
template <size_t _Len>\
struct _NANOSTL_TEMPLATE_VIS aligned_storage<_Len, n>\
{\
    struct alignas(n) type\
    {\
        unsigned char __lx[(_Len + n - 1)/n * n];\
    };\
}

_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x1);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x2);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x4);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x8);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x10);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x20);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x40);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x80);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x100);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x200);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x400);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x800);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x1000);
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x2000);
// PE/COFF does not support alignment beyond 8192 (=0x2000)
#if !defined(_LIBCPP_OBJECT_FORMAT_COFF)
_CREATE_ALIGNED_STORAGE_SPECIALIZATION(0x4000);
#endif // !defined(_LIBCPP_OBJECT_FORMAT_COFF)

#undef _CREATE_ALIGNED_STORAGE_SPECIALIZATION




// __swappable

template <class _Tp> struct __is_swappable;
template <class _Tp> struct __is_nothrow_swappable;

template <class _Tp>
using __swap_result_t = typename enable_if<is_move_constructible<_Tp>::value && is_move_assignable<_Tp>::value>::type;

template <class _Tp>
inline /*_LIBCPP_INLINE_VISIBILITY*/
/*_LIBCPP_CONSTEXPR_AFTER_CXX17*/ __swap_result_t<_Tp>
swap(_Tp& __x, _Tp& __y) __NANOSTL_NOEXCEPT_(is_nothrow_move_constructible<_Tp>::value &&
                                    is_nothrow_move_assignable<_Tp>::value);

template<class _Tp, size_t _Np>
inline /*_LIBCPP_INLINE_VISIBILITY*/ /*_LIBCPP_CONSTEXPR_AFTER_CXX17*/
typename enable_if<
    __is_swappable<_Tp>::value
>::type
swap(_Tp (&__a)[_Np], _Tp (&__b)[_Np]) __NANOSTL_NOEXCEPT_(__is_nothrow_swappable<_Tp>::value);


namespace __detail
{
// ALL generic swap overloads MUST already have a declaration available at this point.

template <class _Tp, class _Up = _Tp,
          bool _NotVoid = !is_void<_Tp>::value && !is_void<_Up>::value>
struct __swappable_with
{
    template <class _LHS, class _RHS>
    static decltype(swap(declval<_LHS>(), declval<_RHS>()))
    __test_swap(int);
    template <class, class>
    static __nat __test_swap(long);

    // Extra parens are needed for the C++03 definition of decltype.
    typedef decltype((__test_swap<_Tp, _Up>(0))) __swap1;
    typedef decltype((__test_swap<_Up, _Tp>(0))) __swap2;

    static const bool value = _IsNotSame<__swap1, __nat>::value
                           && _IsNotSame<__swap2, __nat>::value;
};

template <class _Tp, class _Up>
struct __swappable_with<_Tp, _Up,  false> : false_type {};

template <class _Tp, class _Up = _Tp, bool _Swappable = __swappable_with<_Tp, _Up>::value>
struct __nothrow_swappable_with {
  static const bool value =
#ifndef _LIBCPP_HAS_NO_NOEXCEPT
      noexcept(swap(declval<_Tp>(), declval<_Up>()))
  &&  noexcept(swap(declval<_Up>(), declval<_Tp>()));
#else
      false;
#endif
};

template <class _Tp, class _Up>
struct __nothrow_swappable_with<_Tp, _Up, false> : false_type {};

}  // __detail

template <class _Tp>
struct __is_swappable
    : public integral_constant<bool, __detail::__swappable_with<_Tp&>::value>
{
};

template <class _Tp>
struct __is_nothrow_swappable
    : public integral_constant<bool, __detail::__nothrow_swappable_with<_Tp&>::value>
{
};

// is_trivially_constructible

template <class _Tp, class... _Args>
struct _NANOSTL_TEMPLATE_VIS is_trivially_constructible
    : integral_constant<bool, __is_trivially_constructible(_Tp, _Args...)>
{
};

// is_trivially_default_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_trivially_default_constructible
    : public is_trivially_constructible<_Tp>
    {};

// is_trivially_copy_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_trivially_copy_constructible
    : public is_trivially_constructible<_Tp, typename add_lvalue_reference<const _Tp>::type>
    {};

// is_trivially_move_constructible

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_trivially_move_constructible
    : public is_trivially_constructible<_Tp, typename add_rvalue_reference<_Tp>::type>
    {};

// __is_nullptr_t

template <class _Tp> struct __is_nullptr_t_impl       : public false_type {};
template <>          struct __is_nullptr_t_impl<nullptr_t> : public true_type {};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS __is_nullptr_t
    : public __is_nullptr_t_impl<typename remove_cv<_Tp>::type> {};

//#if _LIBCPP_STD_VER > 11
//template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_null_pointer
//    : public __is_nullptr_t_impl<typename remove_cv<_Tp>::type> {};

//#if _LIBCPP_STD_VER > 14
//template <class _Tp>
//inline constexpr bool is_null_pointer_v = is_null_pointer<_Tp>::value;
//#endif

//#endif // _LIBCPP_STD_VER > 11


// is_member_function_pointer

template <class _Tp> struct __libcpp_is_member_pointer {
  enum {
    __is_member = false,
    __is_func = false,
    __is_obj = false
  };
};
template <class _Tp, class _Up> struct __libcpp_is_member_pointer<_Tp _Up::*> {
  enum {
    __is_member = true,
    __is_func = is_function<_Tp>::value,
    __is_obj = !__is_func,
  };
};

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_member_function_pointer
    : public _BoolConstant< __libcpp_is_member_pointer<typename remove_cv<_Tp>::type>::__is_func > {};


// is_member_pointer

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_member_pointer
 : public _BoolConstant< __libcpp_is_member_pointer<typename remove_cv<_Tp>::type>::__is_member > {};

// is_pointer

template <class _Tp> struct __libcpp_is_pointer       : public false_type {};
template <class _Tp> struct __libcpp_is_pointer<_Tp*> : public true_type {};

template <class _Tp> struct __libcpp_remove_objc_qualifiers { typedef _Tp type; };
//#if defined(_LIBCPP_HAS_OBJC_ARC)
//template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __strong> { typedef _Tp type; };
//template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __weak> { typedef _Tp type; };
//template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __autoreleasing> { typedef _Tp type; };
//template <class _Tp> struct __libcpp_remove_objc_qualifiers<_Tp __unsafe_unretained> { typedef _Tp type; };
//#endif

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_pointer
    : public __libcpp_is_pointer<typename __libcpp_remove_objc_qualifiers<typename remove_cv<_Tp>::type>::type> {};



// is_enum

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_enum
    : public integral_constant<bool, !is_void<_Tp>::value             &&
                                     !is_integral<_Tp>::value         &&
                                     !is_floating_point<_Tp>::value   &&
                                     !is_array<_Tp>::value            &&
                                     !is_pointer<_Tp>::value          &&
                                     !is_reference<_Tp>::value        &&
                                     !is_member_pointer<_Tp>::value   &&
                                     !is_union<_Tp>::value            &&
                                     !is_class<_Tp>::value            &&
                                     !is_function<_Tp>::value         > {};


// is_scalar

template <class _Tp> struct __is_block : false_type {};
#if defined(_LIBCPP_HAS_EXTENSION_BLOCKS)
template <class _Rp, class ..._Args> struct __is_block<_Rp (^)(_Args...)> : true_type {};
#endif


template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_scalar
    : public integral_constant<bool, is_arithmetic<_Tp>::value     ||
                                     is_member_pointer<_Tp>::value ||
                                     is_pointer<_Tp>::value        ||
                                     __is_nullptr_t<_Tp>::value    ||
                                     __is_block<_Tp>::value        ||
                                     is_enum<_Tp>::value           > {};

template <> struct _NANOSTL_TEMPLATE_VIS is_scalar<nullptr_t> : public true_type {};


// is_object

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS is_object
    : public integral_constant<bool, is_scalar<_Tp>::value ||
                                     is_array<_Tp>::value  ||
                                     is_union<_Tp>::value  ||
                                     is_class<_Tp>::value  > {};



}  // namespace nanostl

#endif // NANOSTL_TYPE_TRAITS_H_
