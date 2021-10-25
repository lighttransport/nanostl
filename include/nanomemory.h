#pragma once

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "nanotype_traits.h"
#include "nanocommon.h"
#include "nanofunctional.h"

#include "__nullptr"

#ifndef _LIBCPP_INLINE_VISIBILITY
#define _LIBCPP_INLINE_VISIBILITY
#endif

#ifndef _LIBCPP_CONSTEXPR
#define _LIBCPP_CONSTEXPR constexpr
#endif

// TODO
#ifndef _LIBCPP_CONSTEXPR_AFTER_CXX14
#define _LIBCPP_CONSTEXPR_AFTER_CXX14
#endif

#ifndef _VSTD
#define _VSTD nanostl
#endif

#ifndef _LIBCPP_NODEBUG
#define _LIBCPP_NODEBUG
#endif

namespace nanostl {

template <class _Tp>
struct __has_allocator_type
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::allocator_type* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Tp, class _Alloc, bool = __has_allocator_type<_Tp>::value>
struct __uses_allocator
    : public integral_constant<bool,
        is_convertible<_Alloc, typename _Tp::allocator_type>::value>
{
};

template <class _Tp, class _Alloc>
struct __uses_allocator<_Tp, _Alloc, false>
    : public false_type
{
};

template <class _Tp, class _Alloc>
struct _NANOSTL_TEMPLATE_VIS uses_allocator
    : public __uses_allocator<_Tp, _Alloc>
{
};


struct _NANOSTL_TEMPLATE_VIS allocator_arg_t { explicit allocator_arg_t() = default; };

// TODO(LTE): Building library version
//#if defined(_LIBCPP_CXX03_LANG) || defined(NANOSTL_BUILDING_LIBRARY)
//extern _LIBCPP_EXPORTED_FROM_ABI const allocator_arg_t allocator_arg;
//#else
/* inline */ constexpr allocator_arg_t allocator_arg = allocator_arg_t();
//#endif


// allocator construction

template <class _Tp, class _Alloc, class ..._Args>
struct __uses_alloc_ctor_imp
{
    typedef typename __uncvref<_Alloc>::type _RawAlloc;
    static const bool __ua = uses_allocator<_Tp, _RawAlloc>::value;
    static const bool __ic =
        is_constructible<_Tp, allocator_arg_t, _Alloc, _Args...>::value;
    static const int value = __ua ? 2 - __ic : 0;
};

template <class _Tp, class _Alloc, class ..._Args>
struct __uses_alloc_ctor
    : integral_constant<int, __uses_alloc_ctor_imp<_Tp, _Alloc, _Args...>::value>
    {};

template <class _Tp, class _Allocator, class... _Args>
inline /*_NANOSTL_INLINE_VISIBILITY*/
void __user_alloc_construct_impl (integral_constant<int, 0>, _Tp *__storage, const _Allocator &, _Args &&... __args )
{
    new (__storage) _Tp (nanostl::forward<_Args>(__args)...);
}

// FIXME: This should have a version which takes a non-const alloc.
template <class _Tp, class _Allocator, class... _Args>
inline /*_NANOSTL_INLINE_VISIBILITY */
void __user_alloc_construct_impl (integral_constant<int, 1>, _Tp *__storage, const _Allocator &__a, _Args &&... __args )
{
    new (__storage) _Tp (allocator_arg, __a, nanostl::forward<_Args>(__args)...);
}

// FIXME: This should have a version which takes a non-const alloc.
template <class _Tp, class _Allocator, class... _Args>
inline /*_NANOSTL_INLINE_VISIBILITY*/
void __user_alloc_construct_impl (integral_constant<int, 2>, _Tp *__storage, const _Allocator &__a, _Args &&... __args )
{
    new (__storage) _Tp (nanostl::forward<_Args>(__args)..., __a);
}

#define _LIBCPP_ALLOCATOR_TRAITS_HAS_XXX(NAME, PROPERTY)                \
    template <class _Tp, class = void> struct NAME : false_type { };    \
    template <class _Tp>               struct NAME<_Tp, typename __void_t<typename _Tp:: PROPERTY >::type> : true_type { }

// __pointer
_LIBCPP_ALLOCATOR_TRAITS_HAS_XXX(__has_pointer, pointer);
template <class _Tp, class _Alloc,
          class _RawAlloc = typename remove_reference<_Alloc>::type,
          bool = __has_pointer<_RawAlloc>::value>
struct __pointer {
    using type /*_LIBCPP_NODEBUG*/ = typename _RawAlloc::pointer;
};
template <class _Tp, class _Alloc, class _RawAlloc>
struct __pointer<_Tp, _Alloc, _RawAlloc, false> {
    using type /*_LIBCPP_NODEBUG*/ = _Tp*;
};


// unique_ptr

template <class _Tp, bool>
struct _NANOSTL_TEMPLATE_VIS __dependent_type : public _Tp {};


template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS default_delete {
    static_assert(!is_function<_Tp>::value,
                  "default_delete cannot be instantiated for function types");
  /*_LIBCPP_INLINE_VISIBILITY*/ constexpr default_delete() __NANOSTL_NOEXCEPT = default;
  template <class _Up>
  /*_LIBCPP_INLINE_VISIBILITY*/
  default_delete(const default_delete<_Up>&,
                 typename enable_if<is_convertible<_Up*, _Tp*>::value>::type* =
                     0) __NANOSTL_NOEXCEPT {}

  /*_LIBCPP_INLINE_VISIBILITY*/ void operator()(_Tp* __ptr) const __NANOSTL_NOEXCEPT {
    static_assert(sizeof(_Tp) > 0,
                  "default_delete can not delete incomplete type");
    static_assert(!is_void<_Tp>::value,
                  "default_delete can not delete incomplete type");
    delete __ptr;
  }
};

template <class _Tp>
struct _NANOSTL_TEMPLATE_VIS default_delete<_Tp[]> {
private:
  template <class _Up>
  struct _EnableIfConvertible
      : enable_if<is_convertible<_Up(*)[], _Tp(*)[]>::value> {};

public:
  /*_LIBCPP_INLINE_VISIBILITY*/ constexpr default_delete() __NANOSTL_NOEXCEPT = default;

  template <class _Up>
  /*_LIBCPP_INLINE_VISIBILITY*/
  default_delete(const default_delete<_Up[]>&,
                 typename _EnableIfConvertible<_Up>::type* = 0) __NANOSTL_NOEXCEPT {}

  template <class _Up>
  /*_LIBCPP_INLINE_VISIBILITY*/
  typename _EnableIfConvertible<_Up>::type
  operator()(_Up* __ptr) const __NANOSTL_NOEXCEPT {
    static_assert(sizeof(_Tp) > 0,
                  "default_delete can not delete incomplete type");
    static_assert(!is_void<_Tp>::value,
                  "default_delete can not delete void type");
    delete[] __ptr;
  }
};

template <class _Deleter>
struct __unique_ptr_deleter_sfinae {
  static_assert(!is_reference<_Deleter>::value, "incorrect specialization");
  typedef const _Deleter& __lval_ref_type;
  typedef _Deleter&& __good_rval_ref_type;
  typedef true_type __enable_rval_overload;
};

template <class _Deleter>
struct __unique_ptr_deleter_sfinae<_Deleter const&> {
  typedef const _Deleter& __lval_ref_type;
  typedef const _Deleter&& __bad_rval_ref_type;
  typedef false_type __enable_rval_overload;
};

template <class _Deleter>
struct __unique_ptr_deleter_sfinae<_Deleter&> {
  typedef _Deleter& __lval_ref_type;
  typedef _Deleter&& __bad_rval_ref_type;
  typedef false_type __enable_rval_overload;
};

// Tag used to default initialize one or both of the pair's elements.
struct __default_init_tag {};
struct __value_init_tag {};

template <class _Tp, int _Idx,
          bool _CanBeEmptyBase =
              is_empty<_Tp>::value && !__libcpp_is_final<_Tp>::value>
struct __compressed_pair_elem {
  typedef _Tp _ParamT;
  typedef _Tp& reference;
  typedef const _Tp& const_reference;

  /*_LIBCPP_INLINE_VISIBILITY*/ constexpr
  __compressed_pair_elem(__default_init_tag) {}
  /*_LIBCPP_INLINE_VISIBILITY*/ constexpr
  __compressed_pair_elem(__value_init_tag) : __value_() {}

  template <class _Up, class = typename enable_if<
      !is_same<__compressed_pair_elem, typename decay<_Up>::type>::value
  >::type>
  /*_LIBCPP_INLINE_VISIBILITY*/
  constexpr explicit
  __compressed_pair_elem(_Up&& __u)
      : __value_(nanostl::forward<_Up>(__u))
    {
    }


//#ifndef _LIBCPP_CXX03_LANG
//  template <class... _Args, size_t... _Indexes>
//  /*_LIBCPP_INLINE_VISIBILITY*/ /*_LIBCPP_CONSTEXPR_AFTER_CXX14*/
//  __compressed_pair_elem(piecewise_construct_t, tuple<_Args...> __args,
//                         __tuple_indices<_Indexes...>)
//      : __value_(nanostl::forward<_Args>(nanostl::get<_Indexes>(__args))...) {}
//#endif

  _LIBCPP_INLINE_VISIBILITY reference __get() __NANOSTL_NOEXCEPT { return __value_; }
  _LIBCPP_INLINE_VISIBILITY
  const_reference __get() const __NANOSTL_NOEXCEPT { return __value_; }

private:
  _Tp __value_;
};

template <class _Tp, int _Idx>
struct __compressed_pair_elem<_Tp, _Idx, true> : private _Tp {
  typedef _Tp _ParamT;
  typedef _Tp& reference;
  typedef const _Tp& const_reference;
  typedef _Tp __value_type;

  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR __compressed_pair_elem() = default;
  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
  __compressed_pair_elem(__default_init_tag) {}
  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
  __compressed_pair_elem(__value_init_tag) : __value_type() {}

  template <class _Up, class = typename enable_if<
        !is_same<__compressed_pair_elem, typename decay<_Up>::type>::value
  >::type>
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR explicit
  __compressed_pair_elem(_Up&& __u)
      : __value_type(nanostl::forward<_Up>(__u))
  {}

//#ifndef _LIBCPP_CXX03_LANG
//  template <class... _Args, size_t... _Indexes>
//  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR_AFTER_CXX14
//  __compressed_pair_elem(piecewise_construct_t, tuple<_Args...> __args,
//                         __tuple_indices<_Indexes...>)
//      : __value_type(_VSTD::forward<_Args>(_VSTD::get<_Indexes>(__args))...) {}
//#endif

  _LIBCPP_INLINE_VISIBILITY reference __get() __NANOSTL_NOEXCEPT { return *this; }
  _LIBCPP_INLINE_VISIBILITY
  const_reference __get() const __NANOSTL_NOEXCEPT { return *this; }
};


template <class _T1, class _T2>
class __compressed_pair : private __compressed_pair_elem<_T1, 0>,
                          private __compressed_pair_elem<_T2, 1> {
public:
  // NOTE: This static assert should never fire because __compressed_pair
  // is *almost never* used in a scenario where it's possible for T1 == T2.
  // (The exception is std::function where it is possible that the function
  //  object and the allocator have the same type).
  static_assert((!is_same<_T1, _T2>::value),
    "__compressed_pair cannot be instantiated when T1 and T2 are the same type; "
    "The current implementation is NOT ABI-compatible with the previous "
    "implementation for this configuration");

    typedef _LIBCPP_NODEBUG __compressed_pair_elem<_T1, 0> _Base1;
    typedef _LIBCPP_NODEBUG __compressed_pair_elem<_T2, 1> _Base2;

    template <bool _Dummy = true,
      class = typename enable_if<
          __dependent_type<is_default_constructible<_T1>, _Dummy>::value &&
          __dependent_type<is_default_constructible<_T2>, _Dummy>::value
      >::type
  >
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR __compressed_pair() : _Base1(__value_init_tag()), _Base2(__value_init_tag()) {}

  template <class _U1, class _U2>
  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
  __compressed_pair(_U1&& __t1, _U2&& __t2)
      : _Base1(_VSTD::forward<_U1>(__t1)), _Base2(_VSTD::forward<_U2>(__t2)) {}

//#ifndef _LIBCPP_CXX03_LANG
//  template <class... _Args1, class... _Args2>
//  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR_AFTER_CXX14
//  __compressed_pair(piecewise_construct_t __pc, tuple<_Args1...> __first_args,
//                    tuple<_Args2...> __second_args)
//      : _Base1(__pc, _VSTD::move(__first_args),
//               typename __make_tuple_indices<sizeof...(_Args1)>::type()),
//        _Base2(__pc, _VSTD::move(__second_args),
//               typename __make_tuple_indices<sizeof...(_Args2)>::type()) {}
//#endif


  _LIBCPP_INLINE_VISIBILITY
  typename _Base1::reference first() __NANOSTL_NOEXCEPT {
    return static_cast<_Base1&>(*this).__get();
  }

  _LIBCPP_INLINE_VISIBILITY
  typename _Base1::const_reference first() const __NANOSTL_NOEXCEPT {
    return static_cast<_Base1 const&>(*this).__get();
  }

  _LIBCPP_INLINE_VISIBILITY
  typename _Base2::reference second() __NANOSTL_NOEXCEPT {
    return static_cast<_Base2&>(*this).__get();
  }

  _LIBCPP_INLINE_VISIBILITY
  typename _Base2::const_reference second() const __NANOSTL_NOEXCEPT {
    return static_cast<_Base2 const&>(*this).__get();
  }

  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
  static _Base1* __get_first_base(__compressed_pair* __pair) __NANOSTL_NOEXCEPT {
    return static_cast<_Base1*>(__pair);
  }
  _LIBCPP_INLINE_VISIBILITY _LIBCPP_CONSTEXPR
  static _Base2* __get_second_base(__compressed_pair* __pair) __NANOSTL_NOEXCEPT {
    return static_cast<_Base2*>(__pair);
  }

  _LIBCPP_INLINE_VISIBILITY
  void swap(__compressed_pair& __x)
    __NANOSTL_NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
               __is_nothrow_swappable<_T2>::value)
  {
    using _VSTD::swap;
    swap(first(), __x.first());
    swap(second(), __x.second());
  }
};

template <class _T1, class _T2>
inline _LIBCPP_INLINE_VISIBILITY
void swap(__compressed_pair<_T1, _T2>& __x, __compressed_pair<_T1, _T2>& __y)
    __NANOSTL_NOEXCEPT_(__is_nothrow_swappable<_T1>::value &&
               __is_nothrow_swappable<_T2>::value) {
  __x.swap(__y);
}




#if defined(_LIBCPP_ABI_ENABLE_UNIQUE_PTR_TRIVIAL_ABI)
#  define _LIBCPP_UNIQUE_PTR_TRIVIAL_ABI __attribute__((trivial_abi))
#else
#  define _LIBCPP_UNIQUE_PTR_TRIVIAL_ABI
#endif

template <class _Tp, class _Dp = default_delete<_Tp> >
class _LIBCPP_UNIQUE_PTR_TRIVIAL_ABI _NANOSTL_TEMPLATE_VIS unique_ptr {
public:
  typedef _Tp element_type;
  typedef _Dp deleter_type;
  typedef /*_LIBCPP_NODEBUG*/ typename __pointer<_Tp, deleter_type>::type pointer;

  static_assert(!is_rvalue_reference<deleter_type>::value,
                "the specified deleter type cannot be an rvalue reference");

private:
  __compressed_pair<pointer, deleter_type> __ptr_;

  struct __nat { int __for_bool_; };

  typedef /*_LIBCPP_NODEBUG*/ __unique_ptr_deleter_sfinae<_Dp> _DeleterSFINAE;

  template <bool _Dummy>
  using _LValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__lval_ref_type;

  template <bool _Dummy>
  using _GoodRValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__good_rval_ref_type;

  template <bool _Dummy>
  using _BadRValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__bad_rval_ref_type;

  template <bool _Dummy, class _Deleter = typename __dependent_type<
                             __identity<deleter_type>, _Dummy>::type>
  using _EnableIfDeleterDefaultConstructible /*_LIBCPP_NODEBUG*/ =
      typename enable_if<is_default_constructible<_Deleter>::value &&
                         !is_pointer<_Deleter>::value>::type;

  template <class _ArgType>
  using _EnableIfDeleterConstructible /*_LIBCPP_NODEBUG*/ =
      typename enable_if<is_constructible<deleter_type, _ArgType>::value>::type;

  template <class _UPtr, class _Up>
  using _EnableIfMoveConvertible /*_LIBCPP_NODEBUG*/ = typename enable_if<
      is_convertible<typename _UPtr::pointer, pointer>::value &&
      !is_array<_Up>::value
  >::type;

  template <class _UDel>
  using _EnableIfDeleterConvertible /*_LIBCPP_NODEBUG*/ = typename enable_if<
      (is_reference<_Dp>::value && is_same<_Dp, _UDel>::value) ||
      (!is_reference<_Dp>::value && is_convertible<_UDel, _Dp>::value)
    >::type;

  template <class _UDel>
  using _EnableIfDeleterAssignable = typename enable_if<
      is_assignable<_Dp&, _UDel&&>::value
    >::type;

public:
  template <bool _Dummy = true,
            class = _EnableIfDeleterDefaultConstructible<_Dummy> >
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR unique_ptr() __NANOSTL_NOEXCEPT : __ptr_(pointer(), __default_init_tag()) {}

  template <bool _Dummy = true,
            class = _EnableIfDeleterDefaultConstructible<_Dummy> >
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR unique_ptr(nullptr_t) __NANOSTL_NOEXCEPT : __ptr_(pointer(), __default_init_tag()) {}

  // FIXME:
  //template <bool _Dummy = true,
  //          class = _EnableIfDeleterDefaultConstructible<_Dummy> >
  //_LIBCPP_INLINE_VISIBILITY
  //explicit unique_ptr(pointer __p) __NANOSTL_NOEXCEPT : __ptr_(__p, __default_init_tag()) {}

  _LIBCPP_INLINE_VISIBILITY
  explicit unique_ptr(pointer __p) __NANOSTL_NOEXCEPT : __ptr_(__p, __default_init_tag()) {}

  template <bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_LValRefType<_Dummy> > >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(pointer __p, _LValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(__p, __d) {}

  template <bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_GoodRValRefType<_Dummy> > >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(pointer __p, _GoodRValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(__p, _VSTD::move(__d)) {
    static_assert(!is_reference<deleter_type>::value,
                  "rvalue deleter bound to reference");
  }

  template <bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_BadRValRefType<_Dummy> > >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(pointer __p, _BadRValRefType<_Dummy> __d) = delete;

  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(unique_ptr&& __u) __NANOSTL_NOEXCEPT
      : __ptr_(__u.release(), _VSTD::forward<deleter_type>(__u.get_deleter())) {
  }

  template <class _Up, class _Ep,
      class = _EnableIfMoveConvertible<unique_ptr<_Up, _Ep>, _Up>,
      class = _EnableIfDeleterConvertible<_Ep>
  >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(unique_ptr<_Up, _Ep>&& __u) __NANOSTL_NOEXCEPT
      : __ptr_(__u.release(), _VSTD::forward<_Ep>(__u.get_deleter())) {}

//#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
//  template <class _Up>
//  _LIBCPP_INLINE_VISIBILITY
//  unique_ptr(auto_ptr<_Up>&& __p,
//             typename enable_if<is_convertible<_Up*, _Tp*>::value &&
//                                    is_same<_Dp, default_delete<_Tp> >::value,
//                                __nat>::type = __nat()) __NANOSTL_NOEXCEPT
//      : __ptr_(__p.release(), __default_init_tag()) {}
//#endif


  _LIBCPP_INLINE_VISIBILITY
  unique_ptr& operator=(unique_ptr&& __u) __NANOSTL_NOEXCEPT {
    reset(__u.release());
    __ptr_.second() = _VSTD::forward<deleter_type>(__u.get_deleter());
    return *this;
  }

  template <class _Up, class _Ep,
      class = _EnableIfMoveConvertible<unique_ptr<_Up, _Ep>, _Up>,
      class = _EnableIfDeleterAssignable<_Ep>
  >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr& operator=(unique_ptr<_Up, _Ep>&& __u) __NANOSTL_NOEXCEPT {
    reset(__u.release());
    __ptr_.second() = _VSTD::forward<_Ep>(__u.get_deleter());
    return *this;
  }

//#if _LIBCPP_STD_VER <= 14 || defined(_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR)
//  template <class _Up>
//  _LIBCPP_INLINE_VISIBILITY
//      typename enable_if<is_convertible<_Up*, _Tp*>::value &&
//                             is_same<_Dp, default_delete<_Tp> >::value,
//                         unique_ptr&>::type
//      operator=(auto_ptr<_Up> __p) {
//    reset(__p.release());
//    return *this;
//  }
//#endif
//
//#ifdef _LIBCPP_CXX03_LANG
//  unique_ptr(unique_ptr const&) = delete;
//  unique_ptr& operator=(unique_ptr const&) = delete;
//#endif


  _LIBCPP_INLINE_VISIBILITY
  ~unique_ptr() { reset(); }

  _LIBCPP_INLINE_VISIBILITY
  unique_ptr& operator=(nullptr_t) __NANOSTL_NOEXCEPT {
    reset();
    return *this;
  }

  _LIBCPP_INLINE_VISIBILITY
  typename add_lvalue_reference<_Tp>::type
  operator*() const {
    return *__ptr_.first();
  }
  _LIBCPP_INLINE_VISIBILITY
  pointer operator->() const __NANOSTL_NOEXCEPT {
    return __ptr_.first();
  }
  _LIBCPP_INLINE_VISIBILITY
  pointer get() const __NANOSTL_NOEXCEPT {
    return __ptr_.first();
  }
  _LIBCPP_INLINE_VISIBILITY
  deleter_type& get_deleter() __NANOSTL_NOEXCEPT {
    return __ptr_.second();
  }
  _LIBCPP_INLINE_VISIBILITY
  const deleter_type& get_deleter() const __NANOSTL_NOEXCEPT {
    return __ptr_.second();
  }
  _LIBCPP_INLINE_VISIBILITY
  explicit operator bool() const __NANOSTL_NOEXCEPT {
    return __ptr_.first() != nullptr;
  }

  _LIBCPP_INLINE_VISIBILITY
  pointer release() __NANOSTL_NOEXCEPT {
    pointer __t = __ptr_.first();
    __ptr_.first() = pointer();
    return __t;
  }

  _LIBCPP_INLINE_VISIBILITY
  void reset(pointer __p = pointer()) __NANOSTL_NOEXCEPT {
    pointer __tmp = __ptr_.first();
    __ptr_.first() = __p;
    if (__tmp)
      __ptr_.second()(__tmp);
  }

  _LIBCPP_INLINE_VISIBILITY
  void swap(unique_ptr& __u) __NANOSTL_NOEXCEPT {
    __ptr_.swap(__u.__ptr_);
  }
};

template <class _Tp, class _Dp>
class _LIBCPP_UNIQUE_PTR_TRIVIAL_ABI _NANOSTL_TEMPLATE_VIS unique_ptr<_Tp[], _Dp> {
public:
  typedef _Tp element_type;
  typedef _Dp deleter_type;
  typedef typename __pointer<_Tp, deleter_type>::type pointer;

private:
  __compressed_pair<pointer, deleter_type> __ptr_;

  template <class _From>
  struct _CheckArrayPointerConversion : is_same<_From, pointer> {};

  template <class _FromElem>
  struct _CheckArrayPointerConversion<_FromElem*>
      : integral_constant<bool,
          is_same<_FromElem*, pointer>::value ||
            (is_same<pointer, element_type*>::value &&
             is_convertible<_FromElem(*)[], element_type(*)[]>::value)
      >
  {};

  typedef __unique_ptr_deleter_sfinae<_Dp> _DeleterSFINAE;

  template <bool _Dummy>
  using _LValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__lval_ref_type;

  template <bool _Dummy>
  using _GoodRValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__good_rval_ref_type;

  template <bool _Dummy>
  using _BadRValRefType /*_LIBCPP_NODEBUG*/ =
      typename __dependent_type<_DeleterSFINAE, _Dummy>::__bad_rval_ref_type;

  template <bool _Dummy, class _Deleter = typename __dependent_type<
                             __identity<deleter_type>, _Dummy>::type>
  using _EnableIfDeleterDefaultConstructible /*_LIBCPP_NODEBUG*/ =
      typename enable_if<is_default_constructible<_Deleter>::value &&
                         !is_pointer<_Deleter>::value>::type;

  template <class _ArgType>
  using _EnableIfDeleterConstructible /*_LIBCPP_NODEBUG*/ =
      typename enable_if<is_constructible<deleter_type, _ArgType>::value>::type;

  template <class _Pp>
  using _EnableIfPointerConvertible /*_LIBCPP_NODEBUG*/ = typename enable_if<
      _CheckArrayPointerConversion<_Pp>::value
  >::type;

 template <class _UPtr, class _Up,
        class _ElemT = typename _UPtr::element_type>
  using _EnableIfMoveConvertible /*_LIBCPP_NODEBUG*/ = typename enable_if<
      is_array<_Up>::value &&
      is_same<pointer, element_type*>::value &&
      is_same<typename _UPtr::pointer, _ElemT*>::value &&
      is_convertible<_ElemT(*)[], element_type(*)[]>::value
    >::type;

  template <class _UDel>
  using _EnableIfDeleterConvertible /*_LIBCPP_NODEBUG*/ = typename enable_if<
      (is_reference<_Dp>::value && is_same<_Dp, _UDel>::value) ||
      (!is_reference<_Dp>::value && is_convertible<_UDel, _Dp>::value)
    >::type;

  template <class _UDel>
  using _EnableIfDeleterAssignable /*_LIBCPP_NODEBUG*/ = typename enable_if<
      is_assignable<_Dp&, _UDel&&>::value
    >::type;

public:
  template <bool _Dummy = true,
            class = _EnableIfDeleterDefaultConstructible<_Dummy> >
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR unique_ptr() __NANOSTL_NOEXCEPT : __ptr_(pointer(), __default_init_tag()) {}

  template <bool _Dummy = true,
            class = _EnableIfDeleterDefaultConstructible<_Dummy> >
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR unique_ptr(nullptr_t) __NANOSTL_NOEXCEPT : __ptr_(pointer(), __default_init_tag()) {}

  template <class _Pp, bool _Dummy = true,
            class = _EnableIfDeleterDefaultConstructible<_Dummy>,
            class = _EnableIfPointerConvertible<_Pp> >
  _LIBCPP_INLINE_VISIBILITY
  explicit unique_ptr(_Pp __p) __NANOSTL_NOEXCEPT
      : __ptr_(__p, __default_init_tag()) {}

  template <class _Pp, bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_LValRefType<_Dummy> >,
            class = _EnableIfPointerConvertible<_Pp> >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(_Pp __p, _LValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(__p, __d) {}

  template <bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_LValRefType<_Dummy> > >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(nullptr_t, _LValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(nullptr, __d) {}

  template <class _Pp, bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_GoodRValRefType<_Dummy> >,
            class = _EnableIfPointerConvertible<_Pp> >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(_Pp __p, _GoodRValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(__p, _VSTD::move(__d)) {
    static_assert(!is_reference<deleter_type>::value,
                  "rvalue deleter bound to reference");
  }

  template <bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_GoodRValRefType<_Dummy> > >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(nullptr_t, _GoodRValRefType<_Dummy> __d) __NANOSTL_NOEXCEPT
      : __ptr_(nullptr, _VSTD::move(__d)) {
    static_assert(!is_reference<deleter_type>::value,
                  "rvalue deleter bound to reference");
  }


  template <class _Pp, bool _Dummy = true,
            class = _EnableIfDeleterConstructible<_BadRValRefType<_Dummy> >,
            class = _EnableIfPointerConvertible<_Pp> >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(_Pp __p, _BadRValRefType<_Dummy> __d) = delete;

  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(unique_ptr&& __u) __NANOSTL_NOEXCEPT
      : __ptr_(__u.release(), _VSTD::forward<deleter_type>(__u.get_deleter())) {
  }

  _LIBCPP_INLINE_VISIBILITY
  unique_ptr& operator=(unique_ptr&& __u) __NANOSTL_NOEXCEPT {
    reset(__u.release());
    __ptr_.second() = _VSTD::forward<deleter_type>(__u.get_deleter());
    return *this;
  }

  template <class _Up, class _Ep,
      class = _EnableIfMoveConvertible<unique_ptr<_Up, _Ep>, _Up>,
      class = _EnableIfDeleterConvertible<_Ep>
  >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr(unique_ptr<_Up, _Ep>&& __u) __NANOSTL_NOEXCEPT
      : __ptr_(__u.release(), _VSTD::forward<_Ep>(__u.get_deleter())) {
  }

  template <class _Up, class _Ep,
      class = _EnableIfMoveConvertible<unique_ptr<_Up, _Ep>, _Up>,
      class = _EnableIfDeleterAssignable<_Ep>
  >
  _LIBCPP_INLINE_VISIBILITY
  unique_ptr&
  operator=(unique_ptr<_Up, _Ep>&& __u) __NANOSTL_NOEXCEPT {
    reset(__u.release());
    __ptr_.second() = _VSTD::forward<_Ep>(__u.get_deleter());
    return *this;
  }

//#ifdef _LIBCPP_CXX03_LANG
//  unique_ptr(unique_ptr const&) = delete;
//  unique_ptr& operator=(unique_ptr const&) = delete;
//#endif

public:
  _LIBCPP_INLINE_VISIBILITY
  ~unique_ptr() { reset(); }

  _LIBCPP_INLINE_VISIBILITY
  unique_ptr& operator=(nullptr_t) __NANOSTL_NOEXCEPT {
    reset();
    return *this;
  }

  _LIBCPP_INLINE_VISIBILITY
  typename add_lvalue_reference<_Tp>::type
  operator[](size_t __i) const {
    return __ptr_.first()[__i];
  }
  _LIBCPP_INLINE_VISIBILITY
  pointer get() const __NANOSTL_NOEXCEPT {
    return __ptr_.first();
  }

  _LIBCPP_INLINE_VISIBILITY
  deleter_type& get_deleter() __NANOSTL_NOEXCEPT {
    return __ptr_.second();
  }

  _LIBCPP_INLINE_VISIBILITY
  const deleter_type& get_deleter() const __NANOSTL_NOEXCEPT {
    return __ptr_.second();
  }
  _LIBCPP_INLINE_VISIBILITY
  explicit operator bool() const __NANOSTL_NOEXCEPT {
    return __ptr_.first() != nullptr;
  }

  _LIBCPP_INLINE_VISIBILITY
  pointer release() __NANOSTL_NOEXCEPT {
    pointer __t = __ptr_.first();
    __ptr_.first() = pointer();
    return __t;
  }

  template <class _Pp>
  _LIBCPP_INLINE_VISIBILITY
  typename enable_if<
      _CheckArrayPointerConversion<_Pp>::value
  >::type
  reset(_Pp __p) __NANOSTL_NOEXCEPT {
    pointer __tmp = __ptr_.first();
    __ptr_.first() = __p;
    if (__tmp)
      __ptr_.second()(__tmp);
  }

  _LIBCPP_INLINE_VISIBILITY
  void reset(nullptr_t = nullptr) __NANOSTL_NOEXCEPT {
    pointer __tmp = __ptr_.first();
    __ptr_.first() = nullptr;
    if (__tmp)
      __ptr_.second()(__tmp);
  }

  _LIBCPP_INLINE_VISIBILITY
  void swap(unique_ptr& __u) __NANOSTL_NOEXCEPT {
    __ptr_.swap(__u.__ptr_);
  }

};

template <class _Tp, class _Dp>
inline _LIBCPP_INLINE_VISIBILITY
typename enable_if<
    __is_swappable<_Dp>::value,
    void
>::type
swap(unique_ptr<_Tp, _Dp>& __x, unique_ptr<_Tp, _Dp>& __y) __NANOSTL_NOEXCEPT {__x.swap(__y);}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return __x.get() == __y.get();}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__x == __y);}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator< (const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    typedef typename unique_ptr<_T2, _D2>::pointer _P2;
    typedef typename common_type<_P1, _P2>::type _Vp;
    return less<_Vp>()(__x.get(), __y.get());
}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator> (const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return __y < __x;}

template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__y < __x);}


template <class _T1, class _D1, class _T2, class _D2>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const unique_ptr<_T1, _D1>& __x, const unique_ptr<_T2, _D2>& __y) {return !(__x < __y);}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(const unique_ptr<_T1, _D1>& __x, nullptr_t) __NANOSTL_NOEXCEPT
{
    return !__x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator==(nullptr_t, const unique_ptr<_T1, _D1>& __x) __NANOSTL_NOEXCEPT
{
    return !__x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(const unique_ptr<_T1, _D1>& __x, nullptr_t) __NANOSTL_NOEXCEPT
{
    return static_cast<bool>(__x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator!=(nullptr_t, const unique_ptr<_T1, _D1>& __x) __NANOSTL_NOEXCEPT
{
    return static_cast<bool>(__x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    return less<_P1>()(__x.get(), nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    typedef typename unique_ptr<_T1, _D1>::pointer _P1;
    return less<_P1>()(nullptr, __x.get());
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return nullptr < __x;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return __x < nullptr;
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return !(nullptr < __x);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator<=(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return !(__x < nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(const unique_ptr<_T1, _D1>& __x, nullptr_t)
{
    return !(__x < nullptr);
}

template <class _T1, class _D1>
inline _LIBCPP_INLINE_VISIBILITY
bool
operator>=(nullptr_t, const unique_ptr<_T1, _D1>& __x)
{
    return !(nullptr < __x);
}

template <class _Tp> struct _NANOSTL_TEMPLATE_VIS hash;

template <class _Type, class ...>
using __enable_hash_helper /*_LIBCPP_NODEBUG*/ = _Type;

template <class _Tp, class _Dp>
//#ifdef _LIBCPP_CXX03_LANG
//struct _NANOSTL_TEMPLATE_VIS hash<unique_ptr<_Tp, _Dp> >
//#else
struct _NANOSTL_TEMPLATE_VIS hash<__enable_hash_helper<
    unique_ptr<_Tp, _Dp>, typename unique_ptr<_Tp, _Dp>::pointer> >
//#endif
{
//#if _LIBCPP_STD_VER <= 17 || defined(_LIBCPP_ENABLE_CXX20_REMOVED_BINDER_TYPEDEFS)
//    _LIBCPP_DEPRECATED_IN_CXX17 typedef unique_ptr<_Tp, _Dp> argument_type;
//    _LIBCPP_DEPRECATED_IN_CXX17 typedef size_t               result_type;
//#endif

    _LIBCPP_INLINE_VISIBILITY
    size_t operator()(const unique_ptr<_Tp, _Dp>& __ptr) const
    {
        typedef typename unique_ptr<_Tp, _Dp>::pointer pointer;
        return hash<pointer>()(__ptr.get());
    }
};











} // namespace nanostl
