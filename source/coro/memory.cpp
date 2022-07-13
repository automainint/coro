/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "memory.h"

#include <cstring>
#include <memory_resource>

namespace coro {
  using std::pmr::memory_resource,
      std::pmr::synchronized_pool_resource;

  struct block_info {
    memory_resource *resource;
    size_t size;
  };

  [[nodiscard]] auto default_memory_resource()
      -> std::pmr::memory_resource * {
    static synchronized_pool_resource resource;
    return &resource;
  }

  auto allocate(memory_resource *resource, size_t size) -> void * {
    if (resource == nullptr)
      return nullptr;
    auto *p = static_cast<std::byte *>(
        resource->allocate(sizeof(block_info) + size));
    if (p == nullptr)
      return nullptr;
    auto block = block_info { .resource = resource, .size = size };
    std::memcpy(p, &block, sizeof(block_info));
    return p + sizeof(block_info);
  }

  void deallocate(void *p) {
    if (p == nullptr)
      return;
    auto *q = reinterpret_cast<std::byte *>(p) - sizeof(block_info);
    auto block = block_info {};
    std::memcpy(&block, q, sizeof(block_info));
    block.resource->deallocate(q, sizeof(block_info) + block.size);
  }
}
