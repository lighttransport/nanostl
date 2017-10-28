# NanoSTL, small subset of C++ STL.

NanoSTL is good for using STL-like feature in your C++-like JIT compiler.

## Supported features

* vector
* string

## Differences compared to (full featured) C++ STL

* RTTI and exception is not supported.
* Returns `NULL` when memory allocation failed(no `bad_alloc`)

## TODO

* [ ] CUDA NVRTC support

## Licenss

MIT license
