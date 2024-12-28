# The Art of C++ / Tuple

[![Release](https://img.shields.io/github/release/taocpp/tuple.svg)](https://github.com/taocpp/tuple/releases/latest)
[![Download](https://api.bintray.com/packages/conan/conan-center/taocpp-tuple%3A_/images/download.svg)](https://bintray.com/conan/conan-center/taocpp-tuple%3A_/_latestVersion)
[![TravisCI](https://travis-ci.org/taocpp/tuple.svg)](https://travis-ci.org/taocpp/tuple)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/github/taocpp/tuple?svg=true)](https://ci.appveyor.com/project/taocpp/tuple)

[The Art of C++](https://taocpp.github.io/) / Tuple is a C++11 header-only proof-of-concept implementation of
[`std::tuple`](http://en.cppreference.com/w/cpp/utility/tuple).

## Rationale

Why another implementation of `std::tuple`? To provide a proof-of-concept that,
when avoiding recursion, code can benefit in significant ways. I prepared a talk
about it, including some benchmarks.

[Download presentation](https://github.com/taocpp/tuple/blob/main/Variadic%20Templates.pdf)

TL;DR: GCC 5 on Linux with libstdc++'s `std::tuple` requires 19.6s and an instantiation
depth of at least 3.719 to compile an
[example](https://github.com/taocpp/tuple/blob/main/src/test/tuple/tuple_benchmark.cpp)
generating a tuple with 265 elements via `std::tuple_cat`.
`tao::tuple` requires 1.2s and an instantiation depth of 26 on the same system.

Apple LLVM 7.0 (~Clang 3.7) with libc++'s `std::tuple` requires 70s and an instantiation
depth of 514 to compile the example. `tao::tuple` requires 1.7s and an instantiation depth
of 15 on the same system. This case does *not* change `std::integer_sequence` or
`std::make_integer_sequence`, all improvements are coming from all the *other* improvements
from avoiding recursion.

## CUDA

`tao::tuple` supports [CUDA](http://www.nvidia.com/object/cuda_home.html).

## Compatibility

* Requires C++11 or newer.
* Tested with GCC 4.8+, Clang 3.4+, and Visual Studio 2017.

## Package Managers

You can download and install [The Art of C++](https://taocpp.github.io/) / Tuple using the [Conan](https://github.com/conan-io/conan) package manager:

    conan install taocpp-tuple/1.0.0@

The taocpp-tuple package in conan is kept up to date by Conan team members and community contributors.
If the version is out-of-date, please [create an issue or pull request](https://github.com/conan-io/conan-center-index) on the Conan Center Index repository.

## Changelog

### 1.0.0

Released 2018-08-01

* Initial release.

## License

The Art of C++ is certified [Open Source](http://www.opensource.org/docs/definition.html) software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license](http://www.opensource.org/licenses/mit-license.html) reproduced here.

> Copyright (c) 2015-2020 Daniel Frey
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
