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

namespace nanostl {

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
    template <> struct hash<thread::id>;

  };

  thread() __NANOSTL_NOEXCEPT;
  template <class F, class ...Args> explicit thread(F&& f, Args&&... args);
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

} // namespace nanostl

#endif // NANOSTL_NO_THREAD

#endif // NANOSTL_THREAD_H_
