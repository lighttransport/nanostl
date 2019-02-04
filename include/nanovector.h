/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017-2018 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_VECTOR_H_
#define NANOSTL_VECTOR_H_

#include "nanoallocator.h"

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

template <class T>
static void __swap(T& x, T& y) {
  T c(x);
  x = y;
  y = c;
}

// TODO(LTE): Support allocator.
template <class T, class Allocator = nanostl::allocator<T> >
class vector {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef Allocator allocator_type;

  vector() : elements_(0), capacity_(0), size_(0) {}

  vector(const vector& rhs) {
    clear();
    assign(rhs.begin(), rhs.end());
  }

  ~vector() {
    allocator_type allocator;
    if (elements_) {
      allocator.deallocate(elements_, capacity_);
    }
  }

  reference at(size_type pos) {
    // TODO(LTE): out-of-range check.
    return elements_[pos];
  }

  const_reference at(size_type pos) const {
    // TODO(LTE): out-of-range check.
    return elements_[pos];
  }

  // No initialized value
  void resize(size_type count) {
    if (count < 1) {
      return;
    }

    if (count > capacity()) {
      // TODO(LTE): Use memcpy() or realloc() like functionality to speed up
      // resizing.
      size_type n = (count > recommended_size()) ? count : recommended_size();
#ifdef NANOSTL_DEBUG
      std::cout << "vector::resize: count " << count << ", capacity "
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

  void push_back(const value_type& val) {
    resize(size() + 1);
    elements_[size_ - 1] = val;
  }

  // void push_back(value_type &val); // C++11

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  void clear() { size_ = 0; }

  size_type capacity() const { return capacity_; }

  reference operator[](size_type pos) { return elements_[pos]; }

  const_reference operator[](size_type pos) const { return elements_[pos]; }

  pointer data() { return elements_; }

  vector& operator=(const vector& rhs);
  vector& operator+=(const vector& rhs);

  inline iterator begin(void) const { return elements_ + 0; }

  inline iterator end(void) const { return elements_ + size_; }

  inline iterator erase(iterator pos) {
    while ((pos + 1) != end()) {
      (*pos) = *(pos + 1);
      pos++;
    }
    size_--;

    return pos;
  }

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  void swap(vector& x) {
    __swap(elements_, x.elements_);
    __swap(capacity_, x.capacity_);
    __swap(size_, x.size_);
  }

 private:
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
inline vector<T, Allocator>& vector<T, Allocator>::operator=(
    const vector<T, Allocator>& rhs) {
  if (this != &rhs) {
    assign(rhs.begin(), rhs.end());
  }
  return *this;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace nanostl

#endif  // NANOSTL_VECTOR_H_
