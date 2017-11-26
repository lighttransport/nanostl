/*
The MIT License (MIT)

Copyright (c) 2017 Light Transport Entertainment, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef NANOMAP_H_
#define NANOMAP_H_

#include "nanovector.h"
#include "nanoutility.h"    // nanostl::pair

#define NANOSTL_DEBUG

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

//
// Simple alternative implementation of std::map
//

namespace nanostl {

typedef unsigned int priority_type;

// https://ja.wikipedia.org/wiki/Xorshift
priority_type priority_rand() {
  static priority_type y = 2463534242;
  y = y ^ (y << 13); y = y ^ (y >> 17);
  return y = y ^ (y << 5);
}

// TODO(LTE): Support Comparator and Allocator.
template<class Key, class T>
class map {
 public:
  typedef Key key_type;
  typedef nanostl::pair<const Key, T> value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef pointer iterator; // nanostl::pair<const Key, T>*
  typedef const_pointer const_iterator;
  typedef T mapped_value;
  // typedef Compare key_compare;
  // typedef Allocator allocator_type;

  struct Node {
    value_type val; // {key, mapped}
    priority_type pri;
    Node* ch[2];  // left, right
    Node(value_type v) : val(v), pri(priority_rand()) {
      ch[0] = ch[1] = 0;
    }
    inline Key key() { return val.first; }
    inline T mapped() { return val.second; }
  };

  map() {
    root = 0;
  }

  ~map() {
    // TODO(yurahuna): implement deconstructor
  }

// accessors:

  iterator begin() { return root; }
  const_iterator begin() const { return root; }
  iterator end() { return 0; }              // TODO(yurahuna): where is end?
  const_iterator end() const { return 0; }  // TODO(yurahuna): where is end?
  bool empty() const { return !root; }
  // size_type size() const { return t.size(); }
  // size_type max_size() const { return t.max_size(); }
  // Allocator<T>::reference operator[](const key_type& k) {
  //   return (*((insert(value_type(k, T()))).first)).second;
  // }
  // void swap(map<Key, T, Compare>& x) { t.swap(x.t); }

// insert/erase

  void insert(const value_type& x) {
    root = __insert(root, x);
  }

// map operations:

  iterator find(const key_type& key) {
    Node *t = __find(root, key);
    return !t ? this->end() : &(t->val);
  }

  iterator upper_bound(const key_type& key) {
    Node *t = __upper_bound(root, key);
    return !t ? this->end() : &(t->val);
  }

// debug:

  void print() {
    __print(root);
  }

 private:
  Node *root;

  // b: the direction of rotation
  Node *__rotate(Node *t, int b) {
    Node *s = t->ch[1 - b];
    t->ch[1 - b] = s->ch[b];
    s->ch[b] = t;
    return s;   // return the upper node after the rotation
  }

  Node *__insert(Node *t, const value_type& x) {
    if (!t) return new Node(x);
    int key = x.first;
    if (key == t->key()) {
      // If there exists the node has the same key, return the node
      return t;
    }
    int b = key > t->key();
    t->ch[b] = __insert(t->ch[b], x);
    if (t->pri > t->ch[b]->pri) t = __rotate(t, 1-b);
    return t;
  }

  Node *__find(Node *t, const key_type& key) {
    return (!t || key == t->key()) ? t : __find(t->ch[key > t->key()], key);
  }

  Node *__upper_bound(Node *t, const key_type& key) {
    if (!t) return 0;
    if (key < t->key()) {
      Node *s = __upper_bound(t->ch[0], key);
      return s ? s : t;
    }
    return __upper_bound(t->ch[1], key);
  }

  // for debug
  void __print(Node *t) {
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
};

}  // nanostl

#endif  // NANOMAP_H_
