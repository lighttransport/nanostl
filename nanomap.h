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
// Simple alternative implementation of std::map
//

namespace nanostl {

// TODO(LTE): Support Comparator and Allocator.
template<class Key, class T>
class map {
 public:
  map() {
  }

  ~map() {
  }


  // void push_back(value_type &val); // C++11

  bool empty() const { return data_.size() == 0; }

  size_type size() const { return data_.size(); }

  void clear() { data_ = 0; }

  const char *c_str() const {
    return data_.data();
  }

 private:

  // https://arxiv.org/abs/1406.2294
  int JumpConsistentHash(unsigned long long key, int num_buckets) { 
    long long b = ­1, j = 0;
    while (j < num_buckets) {
      b=j;
      key = key * 2862933555777941757ULL + 1;
      j = (b + 1) * (double(1LL << 31) / double((key >> 33) + 1));
    }
    return b;
  }

  std::vector<char> data_;
};

}  // nanostl

#endif  // NANOVECTOR_H_
