/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../coro/generator.h"
#include "../../coro/task.h"
#include <catch2/catch.hpp>

namespace coro::test {
  TEST_CASE("generator for", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
    };

    int value = 1;
    for (auto n : range(1, 6))
      REQUIRE(n == value++);
    REQUIRE(value == 6);
  }

  TEST_CASE("generator call", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
    };

    auto gen = range(1, 6);

    REQUIRE(gen.next() == 1);
    REQUIRE(gen.next() == 2);
    REQUIRE(gen.next() == 3);
    REQUIRE(gen.next() == 4);
    REQUIRE(gen.next() == 5);
    REQUIRE(gen.next() == 5);
  }

  TEST_CASE("generator return", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
      co_yield end;
    };

    auto gen = range(1, 6);

    REQUIRE(gen.next() == 1);
    REQUIRE(gen.next() == 2);
    REQUIRE(gen.next() == 3);
    REQUIRE(gen.next() == 4);
    REQUIRE(gen.next() == 5);
    REQUIRE(gen.next() == 6);
    REQUIRE(gen.next() == 6);
  }

  TEST_CASE("generator await 1", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
      co_yield 0;
    };

    auto foo = [&]() -> task<int> {
      auto gen = range(1, 6);
      auto sum = 0;
      while (!gen.is_done())
        sum += co_await gen;
      co_return sum;
    };

    REQUIRE(foo().run() == 15);
  }

  TEST_CASE("generator await 2", "[generator]") {
    auto foo = [](int begin) -> task<int> {
      co_return begin * 3 - 1;
    };

    auto range = [&](int begin) -> generator<int> {
      int end = co_await foo(begin);
      for (int i = begin; i < end; i++)
        co_yield i;
    };

    auto bar = range(2);

    REQUIRE(bar.next() == 2);
    REQUIRE(bar.next() == 3);
    REQUIRE(bar.next() == 4);
  }

  TEST_CASE("generator move", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
      co_yield 0;
    };
    auto foo = range(1, 6);
    auto bar = std::move(foo);
    REQUIRE(bar.next() == 1);
    REQUIRE(bar.next() == 2);
    REQUIRE(bar.next() == 3);
    REQUIRE(bar.next() == 4);
    REQUIRE(bar.next() == 5);
    REQUIRE(bar.next() == 0);
    bar = range(3, 5);
    REQUIRE(bar.next() == 3);
    REQUIRE(bar.next() == 4);
    REQUIRE(bar.next() == 0);
  }

  TEST_CASE("generator copy", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
      co_yield 0;
    };
    auto foo = range(1, 6);
    auto bar = decltype(foo) { foo };
    REQUIRE(foo.next() == 1);
    REQUIRE(bar.next() == 2);
    REQUIRE(foo.next() == 3);
    REQUIRE(bar.next() == 4);
    REQUIRE(foo.next() == 5);
    REQUIRE(bar.next() == 0);
    foo = range(3, 5);
    bar = decltype(foo) { foo };
    REQUIRE(bar.next() == 3);
    REQUIRE(foo.next() == 4);
    REQUIRE(bar.next() == 0);
  }

  TEST_CASE("generator exception", "[generator]") {
    auto foo = []() -> generator<int> {
      throw 2;
      co_yield 1;
    };
    int thrown_value = -1;
    try {
      std::ignore = foo().next();
    } catch (int &value) {
      thrown_value = value;
    }
    REQUIRE(thrown_value == 2);
  }

  TEST_CASE("generator nested", "[generator]") {
    auto range = [](int begin, int end) -> generator<int> {
      for (int i = begin; i < end; i++)
        co_yield i;
      co_yield end;
    };

    auto foo = [&](int first, int second,
                   int third) -> generator<int> {
      for (int i = first; i < second; i++)
        co_yield 1;
      co_yield range(first + (second - first) / 2,
                     second + (third - second) / 2);
      for (int i = second; i < third; i++)
        co_yield 9;
    };

    auto gen = foo(2, 6, 8);

    REQUIRE(gen.next() == 1);
    REQUIRE(gen.next() == 1);
    REQUIRE(gen.next() == 1);
    REQUIRE(gen.next() == 1);

    REQUIRE(gen.next() == 4);
    REQUIRE(gen.next() == 5);
    REQUIRE(gen.next() == 6);
    REQUIRE(gen.next() == 7);

    REQUIRE(gen.next() == 9);
    REQUIRE(gen.next() == 9);
  };
}
