#ifndef NANOVECTOR_H_
#define NANOVECTOR_H_

#include "nanoallocator.h"

namespace nanostl {

// TODO(LTE): Support allocator.
template<class T, class Allocator = nanostl::allocator<T> >
class vector
{
 public:
  typedef T value_type;
  typedef Allocator allocator_type;

  T at(size_type pos);
  const T at(size_type pos) const;

  // No initialized value
  void resize(size_type count);

  bool empty() const {
    return size_ == 0;
  }
  
  size_type size() const {
    return size_;
  }

  void clear() {
    size_ = 0;
  }

 private:
  T *data_;
  size_type size_;
  size_type allocated_;

};

}  // nanostl

#endif // NANOVECTOR_H_
