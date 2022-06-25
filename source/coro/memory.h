/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_MEMORY_H
#define CORO_MEMORY_H

#include <cstddef>

namespace coro {
  [[nodiscard]] auto allocate(size_t size) -> void *;
  void deallocate(void *p);
}

#endif
