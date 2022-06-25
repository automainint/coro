/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../coro/memory.h"
#include <catch2/catch.hpp>

namespace coro {
  TEST_CASE("allocate and deallocate") {
    auto *p = allocate(sizeof(int));
    REQUIRE(p != nullptr);
    deallocate(p);
  }

  TEST_CASE("allocate 100 ints") {
    auto const size = 100;
    auto *p = reinterpret_cast<int *>(allocate(sizeof(int) * size));
    REQUIRE(p != nullptr);
    for (int i = 0; i < size; i++)
      p[i] = i;
    deallocate(p);
  }
}
