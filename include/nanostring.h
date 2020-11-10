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

#ifndef NANOSTL_STRING_H_
#define NANOSTL_STRING_H_

#include "__nanostrutil.h"
#include "nanolimits.h"
#include "nanovector.h"
#include "nanoutility.h"

#ifdef NANOSTL_DEBUG
#if !defined(__CUDACC__)
#include <iostream>
#endif
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
  typedef charT &reference;
  typedef const charT &const_reference;
  typedef charT *pointer;
  typedef const charT *const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string() {}

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string(const basic_string &s) { data_ = s.data_; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string(const charT *s) {
    while (s && (*s) != '\0') {
      data_.push_back(*s);
      s++;
    }
    data_.push_back('\0');
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string(const charT *first, const charT *last) {
    const char *s = first;
    while (s && (s <= last)) {
      data_.push_back(*s);
      s++;
    }
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string(const charT *s, size_type count) {
    for (size_type i = 0; i < count; i++) {
      data_.push_back(s[i]);
    }
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool empty() const { return data_.size() == 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type size() const {
    // -1 for '\0'
    return data_.size() - 1;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type length() const {
    // -1 for '\0'
    return data_.size() - 1;
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  void clear() { data_.clear(); }

  NANOSTL_HOST_AND_DEVICE_QUAL
  const charT *c_str() const { return &data_.at(0); }

  NANOSTL_HOST_AND_DEVICE_QUAL
  char &at(size_type pos) { return data_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  const charT &at(size_type pos) const { return data_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  char &operator[](size_type pos) { return data_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  const charT &operator[](size_type pos) const { return data_[pos]; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  int compare(const basic_string &str) const {
    return compare_(&data_[0], &str[0]);
  }

  NANOSTL_HOST_AND_DEVICE_QUAL
  int compare(const charT *s) const { return compare_(&data_[0], s); }

  NANOSTL_HOST_AND_DEVICE_QUAL
  iterator erase(iterator pos) { return data_.erase(pos); }

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string operator+(const basic_string &s) const;

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string &operator+=(const basic_string &s);

  NANOSTL_HOST_AND_DEVICE_QUAL
  basic_string &operator=(const basic_string &s);

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator==(const basic_string &str) const { return compare(str) == 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator==(const charT *s) const { return compare(s) == 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator!=(const basic_string &str) const { return compare(str) != 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator!=(const charT *s) const { return compare(s) != 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator<(const basic_string &str) const { return compare(str) < 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator<(const charT *s) const { return compare(s) < 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator>(const basic_string &str) const { return compare(str) > 0; }

  NANOSTL_HOST_AND_DEVICE_QUAL
  bool operator>(const charT *s) const { return compare(s) > 0; }

 private:
  nanostl::vector<charT> data_;

  inline int compare_(const charT *p, const charT *q) const {
    while (*p && (*p == *q)) {
      p++;
      q++;
    }

    return *reinterpret_cast<const unsigned char *>(p) -
           *reinterpret_cast<const unsigned char *>(q);
  }
};

template <class charT>
basic_string<charT> basic_string<charT>::operator+(
    const basic_string<charT> &s) const {
  basic_string<charT> result(*this);
  result += s;
  return result;
}

template <class charT>
basic_string<charT> &basic_string<charT>::operator+=(
    const basic_string<charT> &s) {
  const_iterator first = s.data_.begin();
  const_iterator last = s.data_.end();

  for (; first != last; ++first) {
    data_.push_back(*first);
  }

  return (*this);
}

typedef basic_string<char> string;

NANOSTL_HOST_AND_DEVICE_QUAL
static string to_string(int value) {
  // naiive implementation of base-10 int to ascii
  // based on https://www.techiedelight.com/implement-itoa-function-in-c/

  auto myreverse = [](char *buf, int i, int j) {
    while (i < j) {
      swap(buf[i++], buf[j--]);
    }
  };

  char buffer[numeric_limits<int>::digits10 + 2];  // +2 for sign and null

  int n = (value < 0) ? -value : value;  // abs
  int i = 0;
  int base = 10;
  while (n) {
    int r = n % base;

    if (r >= 10) {
      buffer[i++] = 'A' + (r - 10);  //
    } else {
      buffer[i++] = '0' + r;
    }

    n = n / base;
  }

  if (i == 0) {
    buffer[i++] = '0';
  }

  if ((value < 0) && (base == 10)) {
    buffer[i++] = '-';
  }

  buffer[i] = '\0';

  myreverse(buffer, 0, i - 1);

  return string(buffer);
}

NANOSTL_HOST_AND_DEVICE_QUAL
static string to_string(int64_t value) {
  // naiive implementation of base-10 int to ascii
  // based on https://www.techiedelight.com/implement-itoa-function-in-c/

  auto myreverse = [](char *buf, int i, int j) {
    while (i < j) {
      swap(buf[i++], buf[j--]);
    }
  };

  char buffer[numeric_limits<int64_t>::digits10 + 2];  // +2 for sign and null

  int n = (value < 0) ? -value : value;  // abs
  int i = 0;
  int base = 10;
  while (n) {
    int r = n % base;

    if (r >= 10) {
      buffer[i++] = 'A' + (r - 10);  //
    } else {
      buffer[i++] = '0' + r;
    }

    n = n / base;
  }

  if (i == 0) {
    buffer[i++] = '0';
  }

  if ((value < 0) && (base == 10)) {
    buffer[i++] = '-';
  }

  buffer[i] = '\0';

  myreverse(buffer, 0, i - 1);

  return string(buffer);
}

// TODO: Move implementation to .cc and remove `static`
NANOSTL_HOST_AND_DEVICE_QUAL
static string to_string(float value) {
  char buf[16];
  ryu::f2s_buffered(value, buf);

  return string(buf);
}

NANOSTL_HOST_AND_DEVICE_QUAL
static string to_string(double value) {
  char buf[25];
  ryu::d2s_buffered(value, buf);

  return string(buf);
}

NANOSTL_HOST_AND_DEVICE_QUAL
static float stof(const nanostl::string &str, nanostl::size_t *idx = nullptr) {
  (void)idx;  // TODO(LTE):
  float value;
  ryu::RyuStatus ret = ryu::s2f_n(str.c_str(), str.size(), &value);

  if (ret != ryu::RYU_SUCCESS) {
    // TODO(LTE): Report an error
    return nanostl::numeric_limits<float>::signaling_NaN();
  }

  return value;
}

NANOSTL_HOST_AND_DEVICE_QUAL
static float stod(const nanostl::string &str, nanostl::size_t *idx = nullptr) {
  (void)idx;  // TODO(LTE):
  double value;
  ryu::RyuStatus ret = ryu::s2d_n(str.c_str(), str.size(), &value);

  if (ret != ryu::RYU_SUCCESS) {
    // TODO(LTE): Report an error
    return nanostl::numeric_limits<double>::signaling_NaN();
  }

  return value;
}

}  // namespace nanostl

#endif  // NANOSTL_STRING_H_
