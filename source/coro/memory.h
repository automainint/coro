/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_MEMORY_H
#define CORO_MEMORY_H

#include <cstddef>
#include <memory_resource>

namespace coro {
  [[nodiscard]] auto default_memory_resource()
      -> std::pmr::memory_resource *;
  [[nodiscard]] auto allocate(std::pmr::memory_resource *resource,
                              size_t size) -> void *;
  void deallocate(void *p);
}

#endif
