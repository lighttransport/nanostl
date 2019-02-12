/*
 * NanoSTL v0.1.0
 * Generated: 2019-02-13 02:39:07.058478
 * ----------------------------------------------------------
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
#ifndef NANOSTL_SINGLE_INCLUDE_H_
#define NANOSTL_SINGLE_INCLUDE_H_
#ifndef NANOSTL_H_
#define NANOSTL_H_

// #included from: nanovector.h

#define NANOSTL_VECTOR_H_

// #included from: nanoallocator.h

#define NANOSTL_ALLOCATOR_H_

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

namespace nanostl {

typedef unsigned long long size_type;

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

///
/// allocator class implementaion without libc function
///
template <typename T>
class allocator {
 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;

  allocator() {}

  T* allocate(size_type n, const void* hint = 0) {
    (void)hint;  // Ignore `hint' for a while.
    if (n < 1) {
      return 0;
    }

#ifdef NANOSTL_DEBUG
    std::cerr << "allocator::allocate: n " << n << std::endl;
#endif

    return new T[n];
  }

  void deallocate(T* p, size_type n) {
    (void)n;
    delete[] p;
  }

 private:
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace nanostl

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

namespace nanostl {

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wunused-template")
#pragma clang diagnostic ignored "-Wunused-template"
#endif
#endif

template <class T>
static void __swap(T& x, T& y) {
  T c(x);
  x = y;
  y = c;
}

// TODO(LTE): Support allocator.
template <class T, class Allocator = nanostl::allocator<T> >
class vector {
 public:
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef Allocator allocator_type;

  vector() : elements_(0), capacity_(0), size_(0) {}

  vector(const vector& rhs) {
    clear();
    assign(rhs.begin(), rhs.end());
  }

  ~vector() {
    allocator_type allocator;
    if (elements_) {
      allocator.deallocate(elements_, capacity_);
    }
  }

  reference at(size_type pos) {
    // TODO(LTE): out-of-range check.
    return elements_[pos];
  }

  const_reference at(size_type pos) const {
    // TODO(LTE): out-of-range check.
    return elements_[pos];
  }

  // No initialized value
  void resize(size_type count) {
    if (count < 1) {
      return;
    }

    if (count > capacity()) {
      // TODO(LTE): Use memcpy() or realloc() like functionality to speed up
      // resizing.
      size_type n = (count > recommended_size()) ? count : recommended_size();
#ifdef NANOSTL_DEBUG
      std::cout << "vector::resize: count " << count << ", capacity "
                << capacity() << ", recommended_size " << recommended_size()
                << ", n " << n << std::endl;
#endif
      allocator_type allocator;

      value_type* new_elements = allocator.allocate(n);
      size_type new_capacity = n;

      for (size_type i = 0; i < size(); i++) {
        new_elements[i] = elements_[i];
      }

      // delete old buffer
      allocator.deallocate(elements_, capacity_);

      elements_ = new_elements;
      capacity_ = new_capacity;
    }

    size_ = count;
  }

  void push_back(const value_type& val) {
    resize(size() + 1);
    elements_[size_ - 1] = val;
  }

  // void push_back(value_type &val); // C++11

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  void clear() { size_ = 0; }

  size_type capacity() const { return capacity_; }

  reference operator[](size_type pos) { return elements_[pos]; }

  const_reference operator[](size_type pos) const { return elements_[pos]; }

  pointer data() { return elements_; }

  vector& operator=(const vector& rhs);
  vector& operator+=(const vector& rhs);

  inline iterator begin(void) const { return elements_ + 0; }

  inline iterator end(void) const { return elements_ + size_; }

  inline iterator erase(iterator pos) {
    while ((pos + 1) != end()) {
      (*pos) = *(pos + 1);
      pos++;
    }
    size_--;

    return pos;
  }

  template <class InputIterator>
  void assign(InputIterator first, InputIterator last) {
    clear();
    for (; first != last; ++first) {
      push_back(*first);
    }
  }

  void swap(vector& x) {
    __swap(elements_, x.elements_);
    __swap(capacity_, x.capacity_);
    __swap(size_, x.size_);
  }

 private:
  size_type recommended_size() const {
    // Simply use twice as large.
    size_type s = 2 * capacity();
    return s;
  }

  T* elements_;
  size_type capacity_;
  size_type size_;
};

template <class T, class Allocator>
inline vector<T, Allocator>& vector<T, Allocator>::operator=(
    const vector<T, Allocator>& rhs) {
  if (this != &rhs) {
    assign(rhs.begin(), rhs.end());
  }
  return *this;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace nanostl

// #included from: nanostring.h

#define NANOSTL_STRING_H_

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

//
// Simple alternative implementation of std::string
// Implement `string' as `vector<char>`
// TODO(LTE): Support traits and allocator.
//

namespace nanostl {

template <class charT>
class basic_string {
 public:
  typedef unsigned long long size_type;

  typedef charT value_type;
  typedef charT &reference;
  typedef const charT &const_reference;
  typedef charT *pointer;
  typedef const charT *const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;

  basic_string() {}

  basic_string(const basic_string &s) { data_ = s.data_; }

  basic_string(const charT *s) {
    while (s && (*s) != '\0') {
      data_.push_back(*s);
      s++;
    }
    data_.push_back('\0');
  }

  basic_string(const charT *first, const charT *last) {
    const char *s = first;
    while (s && (s <= last)) {
      data_.push_back(*s);
      s++;
    }
  }

  basic_string(const charT *s, size_type count) {
    for (size_type i = 0; i < count; i++) {
      data_.push_back(s[i]);
    }
  }

  bool empty() const { return data_.size() == 0; }

  size_type size() const { return data_.size(); }
  size_type length() const { return data_.size(); }

  void clear() { data_.clear(); }

  const charT *c_str() const { return &data_.at(0); }

  char &at(size_type pos) { return data_[pos]; }

  const charT &at(size_type pos) const { return data_[pos]; }

  char &operator[](size_type pos) { return data_[pos]; }

  const charT &operator[](size_type pos) const { return data_[pos]; }

  int compare(const basic_string &str) const {
    return compare_(&data_[0], &str[0]);
  }

  int compare(const charT *s) const { return compare_(&data_[0], s); }

  iterator erase(iterator pos) { return data_.erase(pos); }

  basic_string operator+(const basic_string &s) const;
  basic_string &operator+=(const basic_string &s);

  basic_string &operator=(const basic_string &s);

  bool operator==(const basic_string &str) const { return compare(str) == 0; }
  bool operator==(const charT *s) const { return compare(s) == 0; }
  bool operator!=(const basic_string &str) const { return compare(str) != 0; }
  bool operator!=(const charT *s) const { return compare(s) != 0; }
  bool operator<(const basic_string &str) const { return compare(str) < 0; }
  bool operator<(const charT *s) const { return compare(s) < 0; }
  bool operator>(const basic_string &str) const { return compare(str) > 0; }
  bool operator>(const charT *s) const { return compare(s) > 0; }

 private:
  nanostl::vector<charT> data_;

  inline int compare_(const charT *p, const charT *q) const {
    while (*p && (*p == *q)) {
      p++;
      q++;
    }

    return *reinterpret_cast<const unsigned char *>(p) -
           *reinterpret_cast<const unsigned char *>(q);
  }
};

template <class charT>
basic_string<charT> basic_string<charT>::operator+(
    const basic_string<charT> &s) const {
  basic_string<charT> result(*this);
  result += s;
  return result;
}

template <class charT>
basic_string<charT> &basic_string<charT>::operator+=(
    const basic_string<charT> &s) {
  const_iterator first = s.data_.begin();
  const_iterator last = s.data_.end();

  for (; first != last; ++first) {
    data_.push_back(*first);
  }

  return (*this);
}

typedef basic_string<char> string;

}  // namespace nanostl

// #included from: nanomap.h

#define NANOSTL_MAP_H_

// #included from: nanoutility.h

#define NANOSTL_UTILITY_H_

namespace nanostl {

template <class T1, class T2>
struct pair {
  T1 first;
  T2 second;
  pair() {}
  pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template <class T1, class T2>
inline bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y) {
  return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y) {
  return pair<T1, T2>(x, y);
}

}  // namespace nanostl

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

//
// Simple alternative implementation of std::map
//

namespace nanostl {

#ifdef __clang__
#pragma clang diagnostic push
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

typedef unsigned int priority_type;

// https://ja.wikipedia.org/wiki/Xorshift
static inline priority_type priority_rand() {
  static priority_type y = 2463534242;
  y = y ^ (y << 13);
  y = y ^ (y >> 17);
  return y = y ^ (y << 5);
}

// TODO(LTE): Support Comparator and Allocator.
template <class Key, class T>
class map {
 public:
  typedef Key key_type;
  typedef nanostl::pair<const Key, T> value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;

  struct Node {
    value_type val;
    priority_type pri;
    Node* ch[2];  // left, right
    Node(value_type v) : val(v), pri(priority_rand()) { ch[0] = ch[1] = 0; }
    inline Key key() { return val.first; }
    inline T mapped() { return val.second; }
  };

  class iterator {
    map<Key, T>* mp;
    Node* p;

   public:
    iterator(map<Key, T>* _mp = 0, Node* _p = 0) : mp(_mp), p(_p) {}
    iterator& operator++() {
      // O(log n)
      p = mp->__upper_bound(mp->root, p->val.first);
      return *this;
    }
    reference operator*() const { return p->val; }
    pointer operator->() const { return &(p->val); }
    bool operator==(const iterator& rhs) const {
      if (rhs.isEnd() && this->isEnd()) return true;
      return *rhs == this->p->val;
    }
    bool operator!=(const iterator& rhs) const {
      if (rhs.isEnd() && this->isEnd())
        return false;
      else if (rhs.isEnd() || this->isEnd())
        return true;
      return *rhs != this->p->val;
    }
    bool isEnd() const { return p == 0; }
  };

  typedef const iterator const_iterator;

  map() { root = 0; }

  ~map() { __delete(root); }

  // accessors:

  iterator begin() { return iterator(this, root); }
  iterator end() { return iterator(this, 0); }
  const_iterator end() const { return iterator(this, 0); }
  bool empty() const { return !root; }
  T& operator[](const key_type& k) {
    return (*((insert(value_type(k, T()))).first)).second;
  }

  // insert/erase

  typedef pair<iterator, bool> pair_iterator_bool;
  pair_iterator_bool insert(const value_type& x) {
    pair<Node*, pair_iterator_bool> p = __insert(root, x);
    root = p.first;
    return p.second;
  }

  // map operations:

  const_iterator find(const key_type& key) const {
    Node* t = __find(root, key);
    return (!t) ? this->end() : iterator(this, t);
  }

  const_iterator upper_bound(const key_type& key) const {
    Node* t = __upper_bound(root, key);
    return (!t) ? this->end() : iterator(this, t);
  }

  // debug:

  void print() {
#ifdef NANOSTL_DEBUG
    __print(root);
#endif
  }

 private:
  Node* root;

  // b: the direction of rotation
  Node* __rotate(Node* t, int b) {
    Node* s = t->ch[1 - b];
    t->ch[1 - b] = s->ch[b];
    s->ch[b] = t;
    return s;  // return the upper node after the rotation
  }

  // {pointer to the root node of the subtree, {iterator to inserted/found
  // value, inserted or not}}
  pair<Node*, pair_iterator_bool> __insert(Node* t, const value_type& x) {
    if (!t) {
      Node* n = new Node(x);
      return make_pair(n, make_pair(iterator(this, n), true));
    }
    Key key = x.first;
    if (key == t->key()) {
      return make_pair(t, make_pair(iterator(this, t), false));
    }
    int b = key > t->key();
    pair<Node*, pair_iterator_bool> p = __insert(t->ch[b], x);
    t->ch[b] = p.first;
    if (t->pri > t->ch[b]->pri) t = __rotate(t, 1 - b);
    return make_pair(t, p.second);
  }

  Node* __find(Node* t, const key_type& key) const {
    return (!t || key == t->key()) ? t : __find(t->ch[key > t->key()], key);
  }

  Node* __upper_bound(Node* t, const key_type& key) const {
    if (!t) return 0;
    if (key < t->key()) {
      Node* s = __upper_bound(t->ch[0], key);
      return s ? s : t;
    }
    return __upper_bound(t->ch[1], key);
  }

  void __delete(Node* t) {
    if (!t) return;
    __delete(t->ch[0]);
    __delete(t->ch[1]);
    delete t;
  }

#ifdef NANOSTL_DEBUG
  // for debug
  void __print(Node* t) {
    if (!t) {
      std::cout << "[]" << std::endl;
      return;
    }
    // preorder
    std::cout << "[key = " << t->val.first << ", mapped = " << t->val.second
              << ", pri = " << t->pri << "]" << std::endl;
    __print(t->ch[0]);
    __print(t->ch[1]);
  }
#endif
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}  // namespace nanostl

// #included from: nanolimits.h

#define NANOSTL_LIMITS_H_

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

// #included from: nanocommon.h

#define NANOSTL_COMMON_H_

namespace nanostl {

// In BigEndian architecture, must define NANOSTL_BIG_ENDIAN explicitly at the
// moment.

namespace {

union IEEE754Float {
  float f;
  struct {
#if defined(NANOSTL_BIG_ENDIAN)
    unsigned int sign : 1;
    unsigned int exponent : 8;
    unsigned int mantissa : 23;
#else
    unsigned int mantissa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
#endif
  } bits;
};

union IEEE754Double {
  double f;
  struct {
#if defined(NANOSTL_BIG_ENDIAN)
    unsigned long long sign : 1;
    unsigned long long exponent : 11;
    unsigned long long mantissa : 52;
#else
    unsigned long long mantissa : 52;
    unsigned long long exponent : 11;
    unsigned long long sign : 1;
#endif
  } bits;
};

} // namespace

}  // namespace nanostl

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#endif

namespace nanostl {

template <class T>
struct numeric_limits;

template <>
struct numeric_limits<char> {
  static inline char min(void) { return -128; }
  static inline char max(void) { return 127; }
  static inline char epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned char> {
  static inline unsigned char min(void) { return 0; }
  static inline unsigned char max(void) { return 255; }
  static inline unsigned char epsilon(void) { return 0; }
};

template <>
struct numeric_limits<short> {
  static inline short min(void) { return -32768; }
  static inline short max(void) { return 32767; }
  static inline short epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned short> {
  static inline unsigned short min(void) { return 0; }
  static inline unsigned short max(void) { return 65535; }
  static inline unsigned short epsilon(void) { return 0; }
};

template <>
struct numeric_limits<int> {
  static inline int min(void) { return -2147483648; }
  static inline int max(void) { return 2147483647; }
  static inline int epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned int> {
  static inline unsigned int min(void) { return 0; }
  static inline unsigned int max(void) { return 0xffffffffU; }
  static inline unsigned int epsilon(void) { return 0; }
};

template <>
struct numeric_limits<long long> {
  static inline long long min(void) { return (-0x7FFFFFFFFFFFFFFFLL - 1LL); }
  static inline long long max(void) { return 0x7FFFFFFFFFFFFFFFLL; }
  static inline long long epsilon(void) { return 0; }
};

template <>
struct numeric_limits<unsigned long long> {
  static inline unsigned long long min(void) { return 0; }
  static inline unsigned long long max(void) { return 0xFFFFFFFFFFFFFFFFULL; }
  static inline unsigned long long epsilon(void) { return 0; }
};

template <>
struct numeric_limits<float> {
  static inline float min(void) { return (1.17549435E-38f); }  // 0x1.0p-126f
  static inline float max(void) {
    return (3.402823466e+38F);
  }  // 0x1.fffffep127f
  static inline float epsilon(void) { return (1.19209290E-07f); }  // 0x1.0p-23f
  static inline float infinity(void) {
    IEEE754Float flt;
    flt.bits.exponent = 255;
    flt.bits.mantissa = 0;
    flt.bits.sign = 0;

    return flt.f;
  }
};

template <>
struct numeric_limits<double> {
  static inline double min(void) {
    return (2.2250738585072014e-308);
  }  // 0x1.0p-1022
  static inline double max(void) {
    return (1.7976931348623157e+308);
  }  // 0x1.fffffffffffffp102
  static inline double epsilon(void) {
    return (2.2204460492503131e-016);
  }  // 0x1.0p-52

  static inline double infinity(void) {
    IEEE754Double flt;
    flt.bits.exponent = 2047;
    flt.bits.mantissa = 0;
    flt.bits.sign = 0;

    return flt.f;
  }
};

}  // namespace nanostl

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// #included from: nanoalgorithm.h

#define NANOSTL_ALGORITHM_H_

namespace nanostl {

template <class T>
const T& min(const T& a, const T& b) {
  return !(b < a) ? a : b;
}

template <class T>
const T& max(const T& a, const T& b) {
  return !(b > a) ? a : b;
}

}  // namespace nanostl

// #included from: nanomath.h

#define NANOSTL_MATH_H_

//
// Implements some <cmath> functionality.
// WARNING: Implementation is approximation. Not IEEE-754 compatible, it looses
// some precision, don't work well depending on CPU's rounding-mode.
//

namespace nanostl {

template <typename T>
static T fabs(T num) {
  // TODO(LTE): Handle +0 and -0 case correctly.
  // TODO(LTE): Handle +inf and -inf case correctly.

  if (num < static_cast<T>(0)) {
    return -num;
  }

  return num;
}

// https://stackoverflow.com/questions/8377412/ceil-function-how-can-we-implement-it-ourselves
// FIXME(LTE): Won't work for large float value.
template <typename T>
inline T ceil(T num) {
  int inum = int(num);
  T diff = num - static_cast<T>(inum);
  if (fabs(diff) < nanostl::numeric_limits<T>::epsilon()) {
    return inum;
  }

  if (num > static_cast<T>(0)) {
    return inum + 1;
  } else {
    return inum;
  }
}

// https://stackoverflow.com/questions/5122993/floor-int-function-implementaton
template <typename T>
inline T floor(T x) {
  if (x >= static_cast<T>(0)) {
    return int(x);
  } else {
    int y = int(x);
    if (fabs(static_cast<T>(y) - x) < nanostl::numeric_limits<T>::epsilon()) {
      return y;
    } else {
      return y - 1;
    }
  }
}

inline bool isfinite(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 255;
  return ret;
}

inline bool isfinite(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = flt.bits.exponent != 2047;
  return ret;
}

inline bool isinf(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa == 0);
  return ret;
}

inline bool isinf(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa == 0);
  return ret;
}

inline bool isnan(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 255) && (flt.bits.mantissa != 0);
  return ret;
}

inline bool isnan(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent == 2047) && (flt.bits.mantissa != 0);
  return ret;
}

inline bool isnormal(float x) {
  IEEE754Float flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 255);
  return ret;
}

inline bool isnormal(double x) {
  IEEE754Double flt;
  flt.f = x;

  bool ret = (flt.bits.exponent != 0) && (flt.bits.exponent != 2047);
  return ret;
}

}  // namespace nanostl

#endif // NANOSTL_H_
#endif // NANOSTL_SINGLE_INCLUDE_H_

