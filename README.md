# NanoSTL, a portable and small subset of C++ STL.

[![Build Status](https://travis-ci.org/lighttransport/nanostl.svg?branch=master)](https://travis-ci.org/lighttransport/nanostl)

NanoSTL is good for using STL-like feature in your C++ application and C++-like JIT compiler.

NanoSTL is portable, it should run well on many environments including Windows, Linux, macOS, x86, ARM, SPARC, etc.

## Status

Eearly testing stage. Not ready for the production use.

## Supported features

Be careful! Not all C++ STL functions are supported for each module.

* vector
* string
* algorithm
* limits
* map

### Limited support

* math : Approximate math functions. Please keep in mind this is basically not be IEEE-754 compliant and does not consier a processor's rounding mode.

#### math functions

* [x] isnan
* [x] isinf
* [x] isfinite
* [x] isnormal
* [x] exp(float)
* [x] log(float)
* [ ] sin(float)
* [ ] cos(float)
* [ ] erf(float)

## Supported architectures

* 64bit and 32bit machine.

## Supported compilers

Even though NanoSTL should be compilable with older and various C++ compilers, at least following compilers works well.

* gcc 4.4.7+
  * NanoSTL itself can be compilable with gcc 4.2.4(fails to compile Catch unit test code)
* clang 3.4+

## Types

NanoSTL assumes following type definitions.

* char : 8bit(NOTE: unsigned is default for some ARM compiler. Use `signed char` if you want signed 8bit type)
* short : 16bit
* int : 32bit
* long long : 64bit
* float : 32bit IEEE754 floating point.
* double : 64bit IEEE754 floating point.

## Compiler macros

* `NANOSTL_BIG_ENDIAN` Set endianness to big endian. Considering to support various compilers, user must explicitly specify endianness to the compiler. Default is little endian.

## Differences compared to (full featured) C++ STL

* RTTI and exception is not supported.
* Returns `NULL` when memory allocation failed(no `bad_alloc`)

## TODO

* [ ] CUDA NVRTC support
* [x] isnan/isinf/isfinite support
* [x] Unit tests
* [ ] Multithread support
* [ ] Backport of some C++11 features(e.g. `unordered_map`)
* [ ] Replace oiio math functions so that we can have clean MIT licensed code.
* [ ] BFLOAT16 support.

## Developer note

### Generate single header file.

```
$ cd scripts
$ python generateSingleHeader.py
```

### Debugging

Use `NANOSTL_DEBUG` define for debugging.

## Licenss

MIT license. Some functions in nanomath is licenced under modified BSD license.

### Third party licenses

* acutest : MIT license.
* faster math functions: Some math functions implemented in nanomath is grabbed from OpenImageIO fmath.h, which is licensed under modified BSD license. https://github.com/OpenImageIO/oiio/
