/*
The MIT License (MIT)

Copyright (c) 2017 Light Transport Entertainment, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef NANOSTRING_H_
#define NANOSTRING_H_

#include "nanovector.h"

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

//
// Simple alternative implementation of std::string
// Implement `string' as `vector<char>`
// TODO(LTE): Support traits and allocator.
//

namespace nanostl {

template <class charT>
class basic_string {
 public:
  typedef unsigned long long size_type;

  typedef charT value_type;
  typedef charT& reference;
  typedef const charT& const_reference;
  typedef charT* pointer;
  typedef const charT* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;

  basic_string() {
  }

  basic_string(const basic_string &s) {
    data_ = s.data_;
  }

  basic_string(const charT *s) {
    while (s && (*s) != '\0') {
      data_.push_back(*s);
      s++;
    }
  }

  basic_string(const charT *first, const charT *last) {
    const char *s = first;
    while (s && (s <= last)) {
      data_.push_back(*s);
      s++;
    }
  }

  basic_string(const charT *s, size_type count) {
    for (size_type i = 0; i < count; i++) {
      data_.push_back(s[i]);
    }
  }

  bool empty() const { return data_.size() == 0; }

  size_type size() const { return data_.size(); }
  size_type length() const { return data_.size(); }

  void clear() { data_.clear(); }

  const charT *c_str() const {
    return &data_.at(0);
  }

  char &at(size_type pos) {
    return data_[pos];
  }

  const charT &at(size_type pos) const {
    return data_[pos];
  }

  char &operator[](size_type pos) {
    return data_[pos];
  }

  const charT &operator[](size_type pos) const {
    return data_[pos];
  }

  int compare(const basic_string &str) const {
    return compare_(&data_[0], &str[0]);
  }

  int compare(const charT *s) const {
    return compare_(&data_[0], s);
  }




  iterator erase(iterator pos) {
    return data_.erase(pos);
  }

  basic_string operator+(const basic_string &s) const;
  basic_string& operator+=(const basic_string &s);

  basic_string& operator=(const basic_string &s);

  bool operator==(const basic_string &str) const    { return compare(str) == 0; }
  bool operator==(const charT *s) const             { return compare(s) == 0; }
  bool operator!=(const basic_string &str) const    { return compare(str) != 0; }
  bool operator!=(const charT *s) const             { return compare(s) != 0; }
  bool operator<(const basic_string &str) const     { return compare(str) < 0; }
  bool operator<(const charT *s) const              { return compare(s) < 0; }
  bool operator>(const basic_string &str) const     { return compare(str) > 0; }
  bool operator>(const charT *s) const              { return compare(s) > 0; }

 private:
  nanostl::vector<charT> data_;

  inline int compare_(const charT *p, const charT *q) const {
    while (*p && (*p == *q)) {
      p++;
      q++;
    }

    return *reinterpret_cast<const unsigned char*>(p) - *reinterpret_cast<const unsigned char*>(q);
  }

};

template <class charT>
basic_string<charT> basic_string<charT>::operator+(const basic_string<charT> &s) const {
  basic_string<charT> result(*this);
  result += s;
  return result;
}

template <class charT>
basic_string<charT>& basic_string<charT>::operator+=(const basic_string<charT> &s) {
  const_iterator first = s.data_.begin();
  const_iterator last = s.data_.end();

  for (; first != last; ++first) {
    data_.push_back(*first);
  }

  return (*this);
}

typedef basic_string<char> string;

}  // nanostl

#endif  // NANOVECTOR_H_
