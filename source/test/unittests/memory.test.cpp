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
}
