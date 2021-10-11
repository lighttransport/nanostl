# NanoSTL, a portable and small subset of C++ STL.

[![Build Status](https://travis-ci.org/lighttransport/nanostl.svg?branch=master)](https://travis-ci.org/lighttransport/nanostl)

NanoSTL is good for using STL-like feature in your C++ application and C++-like JIT compiler.

NanoSTL is portable, it should run well on many environments including Windows, Linux, macOS, x86, ARM, SPARC, etc.

NanoSTL should work well on C++11 compiler or later.
(C++03 or earlier is not supported)

## Status

Eearly testing stage. Not ready for the production use.

## Features

* Experimental CUDA support. You can use NanoSTL on device functions.
  * See `sandbox/cuda/`

## Example

NanoSTL is header-only library. No need to compile/link with `.cc`
Some function(e.g. `std::to_string`) requires the implementation, thus must define `NANOSTL_IMPLEMENTATION` in **single** `*.cc` file.

```
#define NANOSTL_IMPLEMENTATION
#include <nanosring.h>

...
```

## Supported features

* vector
* string
  * [x] `to_string(float)`(using ryu)
  * [x] `to_string(double)`(using ryu)
  * [x] `stof`(string to float. using ryu_parse)
  * [x] `stod`(string to double. using ryu_parse)
* algorithm
* limits
  * [x] `numeric_limits<T>::min`
  * [x] `numeric_limits<T>::max`
  * [x] `numeric_limits<T>::epsilon()`
  * [x] `numeric_limits<T>::digits10`
  * [x] `numeric_limits<float>::denorm_min()`
  * [x] `numeric_limits<double>::denorm_min()`
  * [x] `numeric_limits<float>::inifinity()`
  * [x] `numeric_limits<float>::quiet_NaN()`
  * [x] `numeric_limits<float>::signaling_NaN()`
  * [x] `numeric_limits<double>::inifinity()`
  * [x] `numeric_limits<double>::quiet_NaN()`
  * [x] `numeric_limits<double>::signaling_NaN()`
* map

Be careful! Not all C++ STL functions are supported for each module.

### Limited support

* math : Approximate math functions. Please keep in mind this is basically not be IEEE-754 compliant and does not consier a processor's rounding mode.
* valarray
* cstring
  * [x] memcpy
  * [ ] memmove
  * [ ] strcpy
  * [ ] strncpy
  * [ ] strcat
  * [ ] strncat
  * [ ] memcmp
  * [ ] strcmp
  * [ ] strcoll
  * [ ] strncmp
  * [ ] strxfrm
  * [ ] memchr
  * [ ] strchr
  * [ ] strcspn
  * [ ] strpbrk
  * [ ] strrchr
  * [ ] strspn
  * [ ] strstr
  * [ ] strtok
  * [ ] memset
  * [ ] strerror
  * [ ] strlen
  * [ ] NULL
  * [x] `size_t`
* [ ] iostream
* [x] hash: Basic type
* [ ] hash: string

#### math functions

* [x] isnan
* [x] isinf
* [x] isfinite
* [x] isnormal
* [x] fabs(float)
* [x] copysign(float)
* [x] sqrt(float) Approximated.
* [x] exp(float)
* [x] pow(float)
* [x] log(float)
* [x] log10(float)
* [x] sin(float)
* [x] cos(float)
* [x] tanh(float)
* [x] cbrt(float)
* [x] erf(float)
* [x] erfc(float)
* [x] ierf(float)

## Other list of implementation status

* cassert
  * assert
    * [x] CUDA
    * [ ] Other platform
* To be filled...

## Supported architectures

* 64bit arctecture only.
* Big endian and little endian
  * Some math functions may not run on big endian machine.
* CUDA device.

## Supported compilers

Even though NanoSTL should be compilable with older and various C++ compilers, at least following compilers shold work well.

* gcc 4.4.7+
  * NanoSTL itself can be compilable with gcc 4.2.4(fails to compile Catch unit test code)
* clang 3.4+

## Supported threading library

For `thread` and parallel STL feature, threading library is required.

* pthread(non-Windows)
* Win32 thread(Windows)

## Types

NanoSTL assumes following type definitions.

* char : 8bit(NOTE: unsigned is default for some ARM compiler. Use `signed char` if you want signed 8bit type)
* short : 16bit
* int : 32bit
* long long : 64bit
* float : 32bit IEEE754 floating point.
* double : 64bit IEEE754 floating point.

`long` and `int long` is not recommended to use.  Please use `cstdint` typedefs.

## Compiler macros

* `NANOSTL_BIG_ENDIAN` Set endianness to big endian. Considering to support various compilers, user must explicitly specify endianness to the compiler. Default is little endian.
* `NANOSTL_NO_IO` Disable all I/O operation(e.g. iostream). Useful for embedded devices.
* `NANOSTL_NO_THREAD` Disable `thread`, `atomic` and `mutex` feature.
* `NANOSTL_PSTL` Enable parallel STL feature. Requires C++17 compiler. This also undefine `NANOSTL_NO_THREAD`

### header-only mode

You can define `NANOSTL_IMPLEMENTATION` to define the implementation of some STL functions.
This is useful if you want to use NanoSTL as a header-only library
(No need to compile/add `.cc`)

```
#define NANOSTL_IMPLEMENTATION
#include <nanostl.h>
```


## Differences compared to (full featured) C++ STL

* **No thread safety** Currently NanoSTL is not thread safe
  * Application must care about the thread safety
  * For example, need to use mutex or lock for `nanostl::vector::push_back()` operation if you are accesing `nanostl::vector` object from multiple threads.
* RTTI and exception is not supported.
* Returns `NULL` when memory allocation failed(no `bad_alloc`)
* stof, stod
  * Return (signaling) NaN for invalid input

## TODO

* [ ] iostream(stdout)
* [ ] iostream: Custom output/input sink.
* [ ] fstream(file IO)
* [ ] Math complex type
* [x] CUDA support(experimental)
* [x] isnan/isinf/isfinite support
* [ ] Unit tests
  * [ ] Unit tests on CUDA platform
  * [ ] Write mote unit tests for CPU platform
* [ ] Multithread support
* [ ] Backport of some C++11 features(e.g. `unordered_map`)
* [ ] Replace oiio math functions so that we can have clean MIT licensed code.
* [ ] FLOAT16 and BFLOAT16 support.
* [ ] C++17 parallel STL

## Developer note

### Generate single header file.

```
$ python scripts/generateSingleHeader.py
```

Each `.h` must have wrapped with like this:

```
#ifndef NANOSTL_*_H_`
#define NANOSTL_*_H_`

// comment after `#endif` is required!
#endif // NANOSTL_*_H_
```

to extract codes for single header generation.
(no `#pragma once`)

### Unit tests

Compiling unit test requires C++11 compiler since unit test uses some C++11 math functions for the reference.

```
$ cd tests
$ make
$ ./test
```

### Debugging

Use `NANOSTL_DEBUG` define for debugging.

## Licenss

Unless otherwise expressed, NanoSTL is licensed under MIT license.
Some functions in nanomath is licenced under modified BSD license.

### Third party licenses

* SipHash: cc0 license. https://www.131002.net/siphash/
* variant-lite:  Copyright 2016-2018 by Martin Moene. Boost Software License 1.0. https://github.com/martinmoene/variant-lite
* acutest : MIT license.
* faster math functions: Some math functions implemented in nanomath is grabbed from OpenImageIO fmath.h, which is licensed under modified BSD license. https://github.com/OpenImageIO/oiio/
* ryu(floating point <-> string converter). NanoSTL choose Boost 1.0 license. https://github.com/ulfjack/ryu
* libc++: Apache License v2.0 with LLVM Exceptions
* `mattiasgustavsson/libs`: MIT license. https://github.com/mattiasgustavsson/libs
