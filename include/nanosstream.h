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

namespace nanostl {

#if 0
class stringstream
{
  typedef unsigned long long pos_type;
  typedef long long off_type;

  public:
    stringstream() : binary_(0), length_(0) {}
    ~stringstream() {}

    bool seek_set(const pos_type offset) {
      if (offset > length_) {
        return false;
      }

      idx_ = offset;
      return true;
    }

    bool seek_from_currect(const off_type offset) {
      if ((int64_t(idx_) + offset) < 0) {
        return false;
      }

      if (size_t((off_type(idx_) + offset)) > length_) {
        return false;
      }

      idx_ = size_t(off_type(idx_) + offset);
      return true;
    }

    size_t read(const size_t n, const uint64_t dst_len, unsigned char* dst) {
      size_t len = n;
      if ((idx_ + len) > length_) {
        len = length_ - idx_;
      }

      if (len > 0) {
        if (dst_len < len) {
          // dst does not have enough space. return 0 for a while.
          return 0;
        }

        // TODO(LTE): Use memcpy if available.
        for (unsigned long long i = 0; i < len; i++) {
          dst[i] = binary_[idx_ + i];
        }
        return len;

      } else {
        return 0;
      }
    }

    stringstream &seekg(pos_type pos);

    string str() const {
      return string(reinterpret_cast<const char *>(binary_), length_);
    }
  

 private:

  size_t tell() const { return idx_; }

  //const uint8_t* data() const { return binary_; }

  size_t size() const { return length_; }

 private:
  const unsigned char* binary_;
  const size_t length_;
  char pad_[7];
  unsigned long long idx_;
};
#endif

}  // namespace nanostl

#endif  // NANOSTL_SSTREAM_H_
