#ifndef __FMO_NOT_ON_HEAP
#define __FMO_NOT_ON_HEAP

#include <stdexcept>
#include <string>

struct not_on_heap
  {
  static void* operator new(std::size_t) { throw std::bad_alloc{}; }

  static void* operator new[](std::size_t) { throw std::bad_alloc{}; }

  static void operator delete(void *) noexcept { }

  static void operator delete[](void *) noexcept { }
  };

#endif
