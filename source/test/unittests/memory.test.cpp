/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../coro/generator.h"
#include "../../coro/task.h"
#include <catch2/catch.hpp>

namespace coro::test {
  using std::pmr::memory_resource;

  TEST_CASE("allocate and deallocate") {
    auto *p = allocate(default_memory_resource(), sizeof(int));
    REQUIRE(p != nullptr);
    deallocate(p);
  }

  TEST_CASE("allocate 100 ints") {
    auto const size = 100;
    auto *p = reinterpret_cast<int *>(
        allocate(default_memory_resource(), sizeof(int) * size));
    REQUIRE(p != nullptr);
    for (int i = 0; i < size; i++)
      p[i] = i;
    deallocate(p);
  }

  auto _foo_int(memory_resource *) -> task<int> { co_return 42; }

  TEST_CASE("int task with memory resource") {
    auto foo = _foo_int(default_memory_resource());
    REQUIRE(foo.run() == 42);
  }

  auto _foo_void(memory_resource *) -> task<> { co_return; }

  TEST_CASE("void task with memory resource") {
    auto foo = _foo_void(default_memory_resource());
    foo.resume();
    REQUIRE(foo.is_done());
  }

  auto _bar_int(memory_resource *) -> generator<int> {
    co_yield 42;
    co_return;
  }

  TEST_CASE("int generator with memory resource") {
    auto bar = _bar_int(default_memory_resource());
    REQUIRE(bar.next() == 42);
  }
}
