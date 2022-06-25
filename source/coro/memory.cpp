/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "memory.h"

#include <memory_resource>

namespace coro {
  using std::pmr::unsynchronized_pool_resource;

  static unsynchronized_pool_resource g_resource;

  auto allocate(size_t size) -> void * {
    auto *p = static_cast<std::byte *>(
        g_resource.allocate(sizeof(size_t) + size));
    *reinterpret_cast<size_t *>(p) = size;
    return p + sizeof(size_t);
  }
  void deallocate(void *p) {
    auto *q = reinterpret_cast<std::byte *>(p) - sizeof(size_t);
    g_resource.deallocate(q, *reinterpret_cast<size_t *>(q));
  }
}
