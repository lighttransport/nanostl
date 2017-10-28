#ifndef NANOALGORITHM_H_
#define NANOALGORITHM_H_

namespace nanostl {

template <class T> const T& min(const T& a, const T& b) {
  return !(b<a)?a:b;
}

template <class T> const T& max(const T& a, const T& b) {
  return !(b>a)?a:b;
}

}  // namespace nanostl

#endif  // NANOALGORITHM_H_
