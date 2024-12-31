#include "nanoallocator.h"
#include "nanocommon.h"
#include "__nullptr"

#include <stddef.h> // size_t

extern "C" {

extern void free(void *);
extern void *malloc(size_t n);
}

// NanoSTL mod: Do not throw bad_alloc
_NANOSTL_WEAK void *operator new(size_t size) noexcept
{
  void *p = nullptr;

  if (size == 0) {
    return p;
  }

  p = malloc(size);
  return p;
}

_NANOSTL_WEAK void *operator new[](size_t size) noexcept
{
  return operator new(size);
}

_NANOSTL_WEAK void operator delete(void *ptr) noexcept
{
  free(ptr);
}

_NANOSTL_WEAK void operator delete[](void *ptr) noexcept
{
  operator delete(ptr);
}

