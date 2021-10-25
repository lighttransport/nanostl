/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021-present Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_EXCEPTION_H_
#define NANOSTL_EXCEPTION_H_

#include "nanocommon.h"

namespace nanostl {

class exception
{
public:
    exception() __NANOSTL_NOEXCEPT;
    exception(const exception&) __NANOSTL_NOEXCEPT;
    exception& operator=(const exception&) __NANOSTL_NOEXCEPT;
    virtual ~exception() __NANOSTL_NOEXCEPT;
    virtual const char* what() const __NANOSTL_NOEXCEPT;
};


/*noreturn*/ void terminate() __NANOSTL_NOEXCEPT;

}  // namespace nanostl

#endif  // NANOSTL_EXCEPTION_H_
