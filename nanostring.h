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

class string {
 public:
  string() {
  }

  string(const string &s) {
    data_ = s.data_;
  }

  string(const char *s) {
    while (s && (*s) != '\0') {
      data_.push_back(*s);
      s++;
    }
  }

  // void push_back(value_type &val); // C++11

  bool empty() const { return data_.size() == 0; }

  size_type size() const { return data_.size(); }
  size_type length() const { return data_.size(); }

  void clear() { data_.clear(); }

  const char *c_str() const {
    return &data_.at(0);
  }

  string operator+(const string &s) const;
  string& operator+=(const string &s);

  string& operator=(const string &s);

 private:
  nanostl::vector<char> data_;
};

string string::operator+(const string &s) const {
  string result(*this);
  result += s;
  return result;
}

string& string::operator+=(const string &s) {
  vector<char>::const_iterator first = s.data_.begin();
  vector<char>::const_iterator last = s.data_.end();

  for (; first != last; ++first) {
    data_.push_back(*first);
  }

  return (*this);
}

}  // nanostl

#endif  // NANOVECTOR_H_
