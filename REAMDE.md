# NanoSTL, small subset of C++ STL.

NanoSTL is good for using STL-like feature in your C++-like JIT compiler.

## Supported features

* vector
* string

## Supported architectures

* 64bit and 32bit machine.

## Types

* char : 8bit
* short : 16bit
* int : 32bit
* long long : 64bit
* float : 32bit IEEE754 floating point.
* double : 64bit IEEE754 floating point.

## Differences compared to (full featured) C++ STL

* RTTI and exception is not supported.
* Returns `NULL` when memory allocation failed(no `bad_alloc`)

## TODO

* [ ] CUDA NVRTC support

## Licenss

MIT license

### Third party licenses

* microtest.h : MIT license.
