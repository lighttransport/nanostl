/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Light Transport Entertainment, Inc.
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

#ifndef NANOSTL_TUPLE_H_
#define NANOSTL_TUPLE_H_

#include "nanotype_traits.h"
#include "nanoutility.h"
#include "nanocommon.h"

// Use tao::tuple as the base implementation
#include "tao/tuple/tuple.hpp"
#include "tao/seq/integer_sequence.hpp"
#include "tao/seq/make_integer_sequence.hpp"

namespace nanostl {

// Import tao::tuple as our tuple implementation
using tao::tuple;

// Import sequence utilities
using tao::seq::index_sequence;
using tao::seq::make_index_sequence;

// tuple traits
template<class T>
struct tuple_size;

template<class... Types>
struct tuple_size<tuple<Types...>> : integral_constant<size_t, sizeof...(Types)> {};

template<class T>
struct tuple_size<const T> : tuple_size<T> {};

template<class T>
struct tuple_size<volatile T> : tuple_size<T> {};

template<class T>
struct tuple_size<const volatile T> : tuple_size<T> {};

template<class T>
struct tuple_size<T&> : tuple_size<T> {};

template<class T>
struct tuple_size<T&&> : tuple_size<T> {};

// tuple_element
template<size_t I, class T>
struct tuple_element;

template<size_t I, class... Types>
struct tuple_element<I, tuple<Types...>> {
    using type = typename tao::seq::at_index_t<I, Types...>;
};

template<size_t I, class T>
struct tuple_element<I, const T> {
    using type = const typename tuple_element<I, T>::type;
};

template<size_t I, class T>
struct tuple_element<I, volatile T> {
    using type = volatile typename tuple_element<I, T>::type;
};

template<size_t I, class T>
struct tuple_element<I, const volatile T> {
    using type = const volatile typename tuple_element<I, T>::type;
};

template<size_t I, class T>
using tuple_element_t = typename tuple_element<I, T>::type;

// Import get functions from tao
using tao::get;

// For now, omit type-based get as it's C++14 and complex to implement correctly
// Users can use index-based get instead

// Use tao's make_tuple to avoid conflicts
using tao::make_tuple;

// forward_as_tuple
template<class... Types>
constexpr tuple<Types&&...> forward_as_tuple(Types&&... args) __NANOSTL_NOEXCEPT {
    return tuple<Types&&...>(nanostl::forward<Types>(args)...);
}

// tie
template<class... Types>
constexpr tuple<Types&...> tie(Types&... args) __NANOSTL_NOEXCEPT {
    return tuple<Types&...>(args...);
}

// ignore - for use with tie
namespace {
struct __ignore_t {
    template<class T>
    const __ignore_t& operator=(const T&) const { return *this; }
};
}
static const __ignore_t ignore{};

// tao::tuple already provides comparison operators, so we don't need to redefine them

// Simplified tuple_cat - only support two tuples for now
template<class... T1, class... T2>
tuple<T1..., T2...> tuple_cat(const tuple<T1...>& t1, const tuple<T2...>& t2) {
    return __tuple_cat_two(t1, t2, 
                          make_index_sequence<sizeof...(T1)>{},
                          make_index_sequence<sizeof...(T2)>{});
}

template<class... T1, class... T2, size_t... I1, size_t... I2>
tuple<T1..., T2...> __tuple_cat_two(const tuple<T1...>& t1, const tuple<T2...>& t2,
                                    index_sequence<I1...>, index_sequence<I2...>) {
    return make_tuple(get<I1>(t1)..., get<I2>(t2)...);
}

// tao::tuple already provides swap functionality

} // namespace nanostl

#endif // NANOSTL_TUPLE_H_
