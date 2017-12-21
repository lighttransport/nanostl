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

#include "nanoutility.h"  // nanostl::pair
#include "nanovector.h"

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

//
// Simple alternative implementation of std::map
//

namespace nanostl {

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

}  // namespace nanostl

#endif  // NANOMAP_H_
