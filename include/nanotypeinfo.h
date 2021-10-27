#ifndef NANOSTL_TYPEINFO_H
#define NANOSTL_TYPEINFO_H

#include "nanocommon.h"

#ifndef _LIBCPP_INLINE_VISIBILITY
#define _LIBCPP_INLINE_VISIBILITY 
#endif

#ifndef _LIBCPP_ALWAYS_INLINE
#define _LIBCPP_ALWAYS_INLINE inline
#endif

#ifndef _LIBCPP_CONSTEXPR
#define _LIBCPP_CONSTEXPR constexpr
#endif

#ifndef _NOEXCEPT
#define _NOEXCEPT __NANOSTL_NOEXCEPT
#endif

// TODO(LTE): Implement
#ifndef _LIBCPP_EXCEPTION_ABI 
#define _LIBCPP_EXCEPTION_ABI 
#endif

// TODO(LTE): Implement
#ifndef _LIBCPP_AVAILABILITY_TYPEINFO_VTABLE
#define _LIBCPP_AVAILABILITY_TYPEINFO_VTABLE
#endif

namespace nanostl {

//===-------------------------- typeinfo ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//


// ========================================================================== //
//                           Implementations
// ========================================================================== //
// ------------------------------------------------------------------------- //
//                               Unique
//               (_LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION = 1)
// ------------------------------------------------------------------------- //
// This implementation of type_info assumes a unique copy of the RTTI for a
// given type inside a program. This is a valid assumption when abiding to
// Itanium ABI (http://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable-components).
// Under this assumption, we can always compare the addresses of the type names
// to implement equality-comparison of type_infos instead of having to perform
// a deep string comparison.
// -------------------------------------------------------------------------- //
//                             NonUnique
//               (_LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION = 2)
// -------------------------------------------------------------------------- //
// This implementation of type_info does not assume there is always a unique
// copy of the RTTI for a given type inside a program. For various reasons
// the linker may have failed to merge every copy of a types RTTI
// (For example: -Bsymbolic or llvm.org/PR37398). Under this assumption, two
// type_infos are equal if their addresses are equal or if a deep string
// comparison is equal.
// -------------------------------------------------------------------------- //
//                          NonUniqueARMRTTIBit
//               (_LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION = 3)
// -------------------------------------------------------------------------- //
// This implementation of type_info does not assume always a unique copy of
// the RTTI for a given type inside a program. It packs the pointer to the
// type name into a uintptr_t and reserves the high bit of that pointer (which
// is assumed to be free for use under the ABI in use) to represent whether
// that specific copy of the RTTI can be assumed unique inside the program.
// To implement equality-comparison of type_infos, we check whether BOTH
// type_infos are guaranteed unique, and if so, we simply compare the addresses
// of their type names instead of doing a deep string comparison, which is
// faster. If at least one of the type_infos can't guarantee uniqueness, we
// have no choice but to fall back to a deep string comparison.
//
// This implementation is specific to ARM64 on Apple platforms.
//
// Note that the compiler is the one setting (or unsetting) the high bit of
// the pointer when it constructs the type_info, depending on whether it can
// guarantee uniqueness for that specific type_info.

// This value can be overriden in the __config_site. When it's not overriden,
// we pick a default implementation based on the platform here.
#ifndef _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION

  // Windows binaries can't merge typeinfos, so use the NonUnique implementation.
# ifdef _LIBCPP_OBJECT_FORMAT_COFF
#   define _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION 2

  // On arm64 on Apple platforms, use the special NonUniqueARMRTTIBit implementation.
# elif defined(__APPLE__) && defined(__LP64__) && !defined(__x86_64__)
#   define _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION 3

  // On all other platforms, assume the Itanium C++ ABI and use the Unique implementation.
# else
#   define _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION 1
# endif
#endif


struct __type_info_implementations {
  struct __string_impl_base {
    typedef const char* __type_name_t;
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    _LIBCPP_CONSTEXPR static const char* __type_name_to_string(__type_name_t __v) _NOEXCEPT {
      return __v;
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    _LIBCPP_CONSTEXPR static __type_name_t __string_to_type_name(const char* __v) _NOEXCEPT {
      return __v;
    }
  };

  struct __unique_impl : __string_impl_base {
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static size_t __hash(__type_name_t __v) _NOEXCEPT {
      return reinterpret_cast<size_t>(__v);
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __eq(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      return __lhs == __rhs;
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __lt(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      return __lhs < __rhs;
    }
  };


  struct __non_unique_impl : __string_impl_base {
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static size_t __hash(__type_name_t __ptr) _NOEXCEPT {
      size_t __hash = 5381;
      while (unsigned char __c = static_cast<unsigned char>(*__ptr++))
        __hash = (__hash * 33) ^ __c;
      return __hash;
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __eq(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      return __lhs == __rhs || __builtin_strcmp(__lhs, __rhs) == 0;
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __lt(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      return __builtin_strcmp(__lhs, __rhs) < 0;
    }
  };

  struct __non_unique_arm_rtti_bit_impl {
    typedef uintptr_t __type_name_t;

    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static const char* __type_name_to_string(__type_name_t __v) _NOEXCEPT {
      return reinterpret_cast<const char*>(__v &
          ~__non_unique_rtti_bit::value);
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static __type_name_t __string_to_type_name(const char* __v) _NOEXCEPT {
      return reinterpret_cast<__type_name_t>(__v);
    }

    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static size_t __hash(__type_name_t __v) _NOEXCEPT {
      if (__is_type_name_unique(__v))
        //return reinterpret_cast<size_t>(__v);
        return static_cast<size_t>(__v); // FIXME(LTE):
      return __non_unique_impl::__hash(__type_name_to_string(__v));
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __eq(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      if (__lhs == __rhs)
        return true;
      if (__is_type_name_unique(__lhs, __rhs))
        return false;
      return __builtin_strcmp(__type_name_to_string(__lhs), __type_name_to_string(__rhs)) == 0;
    }
    _LIBCPP_INLINE_VISIBILITY _LIBCPP_ALWAYS_INLINE
    static bool __lt(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      if (__is_type_name_unique(__lhs, __rhs))
        return __lhs < __rhs;
      return __builtin_strcmp(__type_name_to_string(__lhs), __type_name_to_string(__rhs)) < 0;
    }

   private:
    // The unique bit is the top bit. It is expected that __type_name_t is 64 bits when
    // this implementation is actually used.
    typedef integral_constant<__type_name_t,
      (1ULL << ((__CHAR_BIT__ * sizeof(__type_name_t)) - 1))> __non_unique_rtti_bit;

    _LIBCPP_INLINE_VISIBILITY
    static bool __is_type_name_unique(__type_name_t __lhs) _NOEXCEPT {
      return !(__lhs & __non_unique_rtti_bit::value);
    }
    _LIBCPP_INLINE_VISIBILITY
    static bool __is_type_name_unique(__type_name_t __lhs, __type_name_t __rhs) _NOEXCEPT {
      return !((__lhs & __rhs) & __non_unique_rtti_bit::value);
    }
  };

  typedef
#if _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION == 1
    __unique_impl
#elif _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION == 2
    __non_unique_impl
#elif _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION == 3
    __non_unique_arm_rtti_bit_impl
#else
#   error invalid configuration for _LIBCPP_TYPEINFO_COMPARISON_IMPLEMENTATION
#endif
     __impl;
};

class _LIBCPP_EXCEPTION_ABI type_info
{
  type_info& operator=(const type_info&);
  type_info(const type_info&);

 protected:
    typedef __type_info_implementations::__impl __impl;

    __impl::__type_name_t __type_name;

    _LIBCPP_INLINE_VISIBILITY
    explicit type_info(const char* __n)
      : __type_name(__impl::__string_to_type_name(__n)) {}

public:
    _LIBCPP_AVAILABILITY_TYPEINFO_VTABLE
    virtual ~type_info();

    _LIBCPP_INLINE_VISIBILITY
    const char* name() const _NOEXCEPT
    {
      return __impl::__type_name_to_string(__type_name);
    }

    _LIBCPP_INLINE_VISIBILITY
    bool before(const type_info& __arg) const _NOEXCEPT
    {
      return __impl::__lt(__type_name, __arg.__type_name);
    }

    _LIBCPP_INLINE_VISIBILITY
    size_t hash_code() const _NOEXCEPT
    {
      return __impl::__hash(__type_name);
    }

    _LIBCPP_INLINE_VISIBILITY
    bool operator==(const type_info& __arg) const _NOEXCEPT
    {
      return __impl::__eq(__type_name, __arg.__type_name);
    }

    _LIBCPP_INLINE_VISIBILITY
    bool operator!=(const type_info& __arg) const _NOEXCEPT
    { return !operator==(__arg); }
};

class _LIBCPP_EXCEPTION_ABI bad_cast
    //: public exception
{
 public:
  bad_cast() _NOEXCEPT;
  bad_cast(const bad_cast&) _NOEXCEPT = default;
  virtual ~bad_cast() _NOEXCEPT;
  virtual const char* what() const _NOEXCEPT;
};

class _LIBCPP_EXCEPTION_ABI bad_typeid
    //: public exception
{
 public:
  bad_typeid() _NOEXCEPT;
  virtual ~bad_typeid() _NOEXCEPT;
  virtual const char* what() const _NOEXCEPT;
};







} // namespace nanostl

#endif // NANOSTL_TYPEINFO_H
