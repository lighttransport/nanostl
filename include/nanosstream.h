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

#ifndef NANOSTL_SSTREAM_H_
#define NANOSTL_SSTREAM_H_

#include "nanoios.h"
#include "nanostreambuf.h"
#include "nanostring.h"
#include "nanocstdint.h"

namespace nanostl {

// Work in progress.
// TODO(LTE): Implement
// TODO(LTE): Inherit ostream
#if 1
class stringstream
{
  typedef unsigned long long size_type;

  public:
    NANOSTL_HOST_AND_DEVICE_QUAL
    stringstream& operator=(const stringstream&) = delete; // disable copy

    NANOSTL_HOST_AND_DEVICE_QUAL
    stringstream& operator=(stringstream&& other) {
      str_ = other.str_;
      other.str_ = string();

      return (*this);
    }

    NANOSTL_HOST_AND_DEVICE_QUAL
    stringstream() {}

    NANOSTL_HOST_AND_DEVICE_QUAL
    ~stringstream() {}

    // FIXME(LTE): Signature is different from STL spec. take the reference. drop const
    NANOSTL_HOST_AND_DEVICE_QUAL
    string &str() {
      return str_;
    }

    // TODO(LTE): Use stringbuf or streambuf
    NANOSTL_HOST_AND_DEVICE_QUAL
    stringstream& operator<<(const nanostl::string &s) {

      str_ += s;
      return (*this);
    }

    // TODO(LTE): Inherit ios
    void clear() {
      str_ = string();
    }

 private:

  NANOSTL_HOST_AND_DEVICE_QUAL
  size_type size() const { return str_.size(); }

 private:
  nanostl::string str_;
};
#endif

}  // namespace nanostl

#endif  // NANOSTL_SSTREAM_H_
