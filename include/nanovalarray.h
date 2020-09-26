/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_VALARRAY_H_
#define NANOSTL_VALARRAY_H_

#include "nanoallocator.h"
#include "nanomath.h"

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

namespace nanostl {

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wunused-template")
#pragma clang diagnostic ignored "-Wunused-template"
#endif
#endif

// TODO(LTE): Support allocator.
template <class T, class Allocator = nanostl::allocator<T> >
class valarray {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef Allocator allocator_type;

  NANOSTL_HOST_AND_DEVICE_QUAL
  valarray() : elements_(0), capacity_(0), size_(0) {}

  NANOSTL_HOST_AND_DEVICE_QUAL
  valarray(const valarray& rhs) {
    __initialize();
    assign(rhs.begin(), rhs.end());
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  valarray(const size_type n) {
    __initialize();
    resize(n);
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  ~valarray() {
    allocator_type allocator;
    if (elements_) {
      allocator.deallocate(elements_, capacity_);
    }
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  void resize(size_type count) {
    if (count < 1) {
      return;
    }

    if (count > capacity()) {
      // TODO(LTE): Use memcpy() or realloc() like functionality to speed up
      // resizing.
      size_type n = (count > recommended_size()) ? count : recommended_size();
#ifdef NANOSTL_DEBUG
      std::cout << "valarray::resize: count " << count << ", capacity "
                << capacity() << ", recommended_size " << recommended_size()
                << ", n " << n << std::endl;
#endif
      allocator_type allocator;

      value_type* new_elements = allocator.allocate(n);
      size_type new_capacity = n;

      for (size_type i = 0; i < size(); i++) {
        new_elements[i] = elements_[i];
      }

      // delete old buffer
      allocator.deallocate(elements_, capacity_);

      elements_ = new_elements;
      capacity_ = new_capacity;
    }

    size_ = count;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool empty() const { return size_ == 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type size() const { return size_; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  void clear() { size_ = 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type capacity() const { return capacity_; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  reference operator[](size_type pos) { return elements_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  const_reference operator[](size_type pos) const { return elements_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  pointer data() { return elements_; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  valarray& operator=(const valarray& rhs);

  NANOSTL_HOST_AND_DEVICE_QUAL
  valarray& operator+=(const valarray& rhs);

  NANOSTL_HOST_AND_DEVICE_QUAL
  inline iterator begin(void) const { return elements_ + 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  inline iterator end(void) const { return elements_ + size_; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  inline iterator erase(iterator pos) {
    while ((pos + 1) != end()) {
      (*pos) = *(pos + 1);
      pos++;
    }
    size_--;

    return pos;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    clear();
    for (; first != last; ++first) {
      __push_back(*first);
    }
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  void swap(valarray& x) {
    __swap(elements_, x.elements_);
    __swap(capacity_, x.capacity_);
    __swap(size_, x.size_);
  }

 private:

  NANOSTL_HOST_AND_DEVICE_QUAL
  void __initialize() {
    elements_ = 0;
    capacity_ = 0;
    size_ = 0;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  void __push_back(const value_type& val) {
    resize(size() + 1);
    elements_[size_ - 1] = val;
  }


  NANOSTL_HOST_AND_DEVICE_QUAL
  template<class Ty>
  inline void __swap(Ty& x, Ty& y) {
    Ty c(x);
    x = y;
    y = c;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type recommended_size() const {
    // Simply use twice as large.
    size_type s = 2 * capacity();
    return s;
  }

  T* elements_;
  size_type capacity_;
  size_type size_;
};

template <class T, class Allocator>
inline valarray<T, Allocator>& valarray<T, Allocator>::operator=(
    const valarray<T, Allocator>& rhs) {
  if (this != &rhs) {
    assign(rhs.begin(), rhs.end());
  }
  return *this;
}

// math functions.

template <class T>
valarray<T> sin(const valarray<T>& va) {
  valarray<T> result;
  result.resize(va.size());
  for (size_type i = 0; i < va.size(); i++) {
    result[i] = nanostl::sin(va[i]);
  }

  return result;
}


#ifdef __clang__
#pragma clang diagnostic pop
#endif


}  // namespace nanostl

#endif  // NANOSTL_VALARRAY_H_
