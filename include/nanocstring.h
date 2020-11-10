/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_CSTRING_H_
#define NANOSTL_CSTRING_H_

#include "nanocommon.h"

namespace nanostl {

// Naiive implementation of memcpy
NANOSTL_HOST_AND_DEVICE_QUAL
inline void *memcpy(void *dest, const void *src, unsigned long long num)
{
  unsigned char *d_ptr = reinterpret_cast<unsigned char *>(dest);
  const unsigned char *s_ptr = reinterpret_cast<const unsigned char *>(src);

  for (unsigned long long i = 0; i < num; i++) {
    d_ptr[i] = s_ptr[i];
  }

  return dest;
}

}  // namespace nanostl

#endif  // NANOSTL_CSTRING_H_
