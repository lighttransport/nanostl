#include <nanotype_traits.h>

#include <stdio.h>

template<class T>
typename nanostl::enable_if<nanostl::is_floating_point<T>::value>::type
float_func(T x) {
  printf("float type: %f\n", x);
}

int main(int argc, char **argv)
{
  float_func(2.0f);

  printf("is_same<float, float> = %d\n", nanostl::is_same<float, float>::value);
  printf("is_same<float, int> = %d\n", nanostl::is_same<float, int>::value);

  return 0;
}
