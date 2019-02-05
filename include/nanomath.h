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

#ifndef NANOSTL_MATH_H_
#define NANOSTL_MATH_H_

#include "nanolimits.h"

namespace nanostl {

// https://stackoverflow.com/questions/8377412/ceil-function-how-can-we-implement-it-ourselves
// FIXME(LTE): Won't work for large float value.
template<typename T>
static T ceil(T num)
{
    int inum = int(num);
    T diff = num - static_cast<T>(inum);
    if ((-nanostl::numeric_limits<T>::epsilon() < diff) &&
        (diff < nanostl::numeric_limits<T>::epsilon())) {
        return inum;
    }

    if (inum > 0) {
      return inum + 1;
    } else {
      return inum;
    }
}

}  // namespace nanostl

#endif  // NANOSTL_MATH_H_
