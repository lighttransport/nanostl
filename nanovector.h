#ifndef NANOVECTOR_H_
#define NANOVECTOR_H_

#include "nanoallocator.h"

#ifdef NANOSTL_DEBUG
#include <iostream>
#endif

namespace nanostl {

// TODO(LTE): Support allocator.
template <class T, class Allocator = nanostl::allocator<T> >
class vector {
 public:
  vector() : active_index_(0), size_(0) {
    data_[0] = 0;
    data_[1] = 0;
    capacity_[0] = 0;
    capacity_[1] = 0;
  }

  ~vector() {
    if (data_[0]) {
      allocator_.deallocate(data_[0], capacity_[0]);
    }
    if (data_[1]) {
      allocator_.deallocate(data_[1], capacity_[1]);
    }
  }

  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef Allocator allocator_type;

  reference at(size_type pos) {
    // TODO(LTE): out-of-range check.
    return data_[active_index_][pos];
  }

  const_reference at(size_type pos) const {
    // TODO(LTE): out-of-range check.
    return data_[active_index_][pos];
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
      size_type next_index = active_index_ ? 0 : 1;
      if (data_[next_index]) {
        allocator_.deallocate(data_[next_index], capacity_[next_index]);
      }

      data_[next_index] = allocator_.allocate(n);
      capacity_[next_index] = n;

      for (size_type i = 0; i < size(); i++) {
        data_[next_index][i] = data_[active_index_][i];
      }

      // delete old buffer
      allocator_.deallocate(data_[active_index_], capacity_[active_index_]);
      data_[active_index_] = 0;

      active_index_ = next_index;
    }

    size_ = count;
  }

  void push_back(const value_type& val) {
    resize(size() + 1);
    data_[active_index_][size_ - 1] = val;
  }

  // void push_back(value_type &val); // C++11

  bool empty() const { return size_ == 0; }

  size_type size() const { return size_; }

  void clear() { size_ = 0; }

  size_type capacity() const { return capacity_[active_index_]; }

  reference operator[](size_type pos) { return data_[active_index_][pos]; }

  const_reference operator[](size_type pos) const {
    return data_[active_index_][pos];
  }

 private:
  size_type recommended_size() const {
    // Simply use twice as large.
    size_type s = 2 * capacity();
    return s;
  }

  // Simple double buffering.
  // TODO(LTE): Implement another buffer algorithm to save memory.
  T* data_[2];
  size_type capacity_[2];
  size_type active_index_;

  size_type size_;

  allocator_type allocator_;

  char __pad1_[7];
};

}  // nanostl

#endif  // NANOVECTOR_H_
