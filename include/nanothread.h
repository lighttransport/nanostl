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

#ifndef NANOSTL_THREAD_H_
#define NANOSTL_THREAD_H_

#if !defined(NANOSTL_NO_THREAD)

#include "nanocommon.h"
#include "nanochrono.h"
#include "nanotuple.h"
#include "nanomemory.h"

// forward decl(from libs_thread)
typedef void* thread_tls_t;
extern void* thread_tls_get( thread_tls_t tls );
extern thread_tls_t thread_tls_create( void );
extern void thread_tls_set(thread_tls_t tls, void *value);

namespace nanostl {

namespace {

//===--------------------------- thread -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

template <class _Tp> class __thread_specific_ptr;
class /*_LIBCPP_TYPE_VIS*/ __thread_struct;
class /*_LIBCPP_HIDDEN*/ __thread_struct_imp;
class __assoc_sub_state;

//TODO: func_vis
/*_LIBCPP_FUNC_VIS*/ __thread_specific_ptr<__thread_struct>& __thread_local_data();


class /*_LIBCPP_TYPE_VIS*/ __thread_struct
{
    __thread_struct_imp* __p_;

public:
    __thread_struct();
    ~__thread_struct();
    __thread_struct(const __thread_struct&) = default;
    __thread_struct& operator=(const __thread_struct&) = default;

    // TODO
    //void notify_all_at_thread_exit(condition_variable*, mutex*);
    void __make_ready_at_thread_exit(__assoc_sub_state*);
};

template <class _Tp>
class __thread_specific_ptr
{
    thread_tls_t __key_;

     // Only __thread_local_data() may construct a __thread_specific_ptr
     // and only with _Tp == __thread_struct.
    static_assert((is_same<_Tp, __thread_struct>::value), "");
    __thread_specific_ptr();
    friend /*_LIBCPP_FUNC_VIS*/ __thread_specific_ptr<__thread_struct>& __thread_local_data();

    __thread_specific_ptr(const __thread_specific_ptr&);
    __thread_specific_ptr& operator=(const __thread_specific_ptr&);

    /*_LIBCPP_HIDDEN*/ static void /*_LIBCPP_TLS_DESTRUCTOR_CC*/ __at_thread_exit(void*);

public:
    typedef _Tp* pointer;

    ~__thread_specific_ptr();

    _LIBCPP_INLINE_VISIBILITY
    pointer get() const {return static_cast<_Tp*>(thread_tls_get(__key_));}
    _LIBCPP_INLINE_VISIBILITY
    pointer operator*() const {return *get();}
    _LIBCPP_INLINE_VISIBILITY
    pointer operator->() const {return get();}
    void set_pointer(pointer __p);
};


template <class _Tp>
void /*_LIBCPP_TLS_DESTRUCTOR_CC*/
__thread_specific_ptr<_Tp>::__at_thread_exit(void* __p)
{
    delete static_cast<pointer>(__p);
}

template <class _Tp>
__thread_specific_ptr<_Tp>::__thread_specific_ptr()
{
  //int __ec =
  //    __libcpp_tls_create(&__key_, &__thread_specific_ptr::__at_thread_exit);
  //if (__ec)
  //  __throw_system_error(__ec, "__thread_specific_ptr construction failed");

  // TODO: at_thread_exit handler
  __key_ = thread_tls_create();
  if (__key_ == nullptr) {
    // TODO: throw
  }
}

template <class _Tp>
__thread_specific_ptr<_Tp>::~__thread_specific_ptr()
{
    // __thread_specific_ptr is only created with a static storage duration
    // so this destructor is only invoked during program termination. Invoking
    // pthread_key_delete(__key_) may prevent other threads from deleting their
    // thread local data. For this reason we leak the key.
}

template <class _Tp>
void
__thread_specific_ptr<_Tp>::set_pointer(pointer __p)
{
    _LIBCPP_ASSERT(get() == nullptr,
                   "Attempting to overwrite thread local data");
    thread_tls_set(__key_, __p);
}


template <class _Fp>
/*_LIBCPP_INLINE_VISIBILITY*/
void* __thread_proxy(void* __vp)
{
    // _Fp = tuple< unique_ptr<__thread_struct>, Functor, Args...>
    unique_ptr<_Fp> __p(static_cast<_Fp*>(__vp));
    __thread_local_data().set_pointer(tao::get<0>(*__p.get()).release());
    //typedef typename __make_tuple_indices<tao::tuple_size<_Fp>::value, 2>::type _Index;
    //typedef typename tao::seq::make_integer_sequence<typename tao::tuple_size<_Fp>::value, 2>::type _Index;
    //_VSTD::__thread_execute(*__p.get(), _Index());
    //_VSTD::__thread_execute(*__p.get(), tao::seq::make_integer_sequence<tao::tuple_size<_Fp>::value, 2>());
    return nullptr;
}


//===----------------------------------------------------------------------===//

} // namespace

class thread {

 public:

  class id {
   public:
    id() __NANOSTL_NOEXCEPT;

    //bool operator==(thread::id x, thread::id y) noexcept;
    //bool operator!=(thread::id x, thread::id y) noexcept;
    //bool operator<(thread::id x, thread::id y) noexcept;
    //bool operator<=(thread::id x, thread::id y) noexcept;
    //bool operator>(thread::id x, thread::id y) noexcept;
    //bool operator>=(thread::id x, thread::id y) noexcept;

    //template<class CharT, class Traits>
    //std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, thread::id id);

    template <class T> struct hash;
    //template <> struct hash<thread::id>; // TODO

  };

  thread() __NANOSTL_NOEXCEPT;

  template <class _Fp, class ..._Args>
  explicit thread(_Fp&& __f, _Args&&... __args) {
    typedef unique_ptr<__thread_struct> _TSPtr;
    _TSPtr __tsp(new __thread_struct);
    typedef tuple<_TSPtr, typename decay<_Fp>::type, typename decay<_Args>::type...> _Gp;

    unique_ptr<_Gp> __p(
            new _Gp(nanostl::move(__tsp),
                    nanostl::__decay_copy(nanostl::forward<_Fp>(__f)),
                    nanostl::__decay_copy(nanostl::forward<_Args>(__args))...));

  }


  ~thread();

  thread(const thread&) = delete;
  thread(thread&& t) __NANOSTL_NOEXCEPT;

  thread& operator=(const thread&) = delete;
  thread& operator=(thread&& t) __NANOSTL_NOEXCEPT;

  void swap(thread& t) __NANOSTL_NOEXCEPT;

  bool joinable() const __NANOSTL_NOEXCEPT;
  void join();
  void detach();

  id get_id() const __NANOSTL_NOEXCEPT;

  // posix: pthread_t
  // windows: HANDLE
  void *native_handle() {
    return thread_handle_;
  }

  static unsigned hardware_concurrency() __NANOSTL_NOEXCEPT;

 private:
  // opeque pointer
  void *thread_handle_{nullptr};
};

namespace this_thread
{

void sleep_for(const chrono::nanoseconds &ns);

} // namespace this_thread

} // namespace nanostl

#endif // NANOSTL_NO_THREAD

#endif // NANOSTL_THREAD_H_
