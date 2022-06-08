/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../coro/task.h"
#include <catch2/catch.hpp>

namespace coro::test {
  TEST_CASE("task return", "[task]") {
    auto foo = []() -> task<int> { co_return 42; };
    REQUIRE(foo().run() == 42);
  }

  TEST_CASE("task void", "[task]") {
    bool run = false;

    auto foo = [&]() -> task<> {
      run = true;
      co_return;
    };

    auto bar = foo();
    REQUIRE(!run);
    bar.resume();
    REQUIRE(run);
  }

  TEST_CASE("task await", "[task]") {
    bool run = false;

    auto foo = [&]() -> task<> {
      run = true;
      co_return;
    };

    auto bar = [&]() -> task<> { co_await foo(); };

    auto bus = bar();
    REQUIRE(!run);
    bus.resume();
    REQUIRE(run);
  }

  TEST_CASE("task nested", "[task]") {
    auto foo = [&]() -> task<int> { co_return 42; };
    auto bar = [&]() -> task<int> { co_return co_await foo(); };

    REQUIRE(bar().run() == 42);
  }

  TEST_CASE("task int await sentinel", "[task]") {
    auto foo = [&]() -> task<int> {
      co_await std::suspend_always {};
      co_return 5;
    };
    auto bar = foo();
    bar.resume();
    REQUIRE(!bar.is_done());
    bar.resume();
    REQUIRE(bar.is_done());
    REQUIRE(bar.get() == 5);
  }

  TEST_CASE("task void yield sentinel", "[task]") {
    bool run = false;
    auto foo = [&]() -> task<> {
      co_yield std::default_sentinel;
      run = true;
    };
    auto bar = foo();
    bar.resume();
    REQUIRE(!run);
    bar.resume();
    REQUIRE(run);
  }

  TEST_CASE("task move copy 1", "[task]") {
    auto foo = []() -> task<int> { co_return 42; };

    auto a = foo();
    auto b = std::move(a);
    auto c = decltype(b) { b };
    REQUIRE(c.run() == 42);
    REQUIRE(b.is_done());
    a = foo();
    b = std::move(a);
    c = decltype(b) { b };
    REQUIRE(c.run() == 42);
    REQUIRE(b.is_done());
  }

  TEST_CASE("task move copy 2", "[task]") {
    auto foo = []() -> task<> { co_return; };

    auto a = foo();
    auto b = std::move(a);
    auto c = decltype(b) { b };
    c.resume();
    REQUIRE(b.is_done());
    a = foo();
    b = std::move(a);
    c = decltype(b) { b };
    c.resume();
    REQUIRE(b.is_done());
  }

  TEST_CASE("task exception 1", "[task]") {
    auto foo = []() -> task<int> {
      throw 3;
      co_return 1;
    };
    int thrown_value = -1;
    try {
      std::ignore = foo().run();
    } catch (int &value) {
      thrown_value = value;
    }
    REQUIRE(thrown_value == 3);
  }

  TEST_CASE("task exception 2", "[task]") {
    auto foo = []() -> task<> {
      throw 4;
      co_return;
    };
    int thrown_value = -1;
    try {
      foo().resume();
    } catch (int &value) {
      thrown_value = value;
    }
    REQUIRE(thrown_value == 4);
  }

  TEST_CASE("task void nested await 1", "[task]") {
    bool foo_flag = false;
    bool bar_flag = false;

    auto foo = [&]() -> task<> {
      foo_flag = true;
      co_return;
    };

    auto bar = [&]() -> task<> {
      co_await foo();
      bar_flag = true;
    };

    auto bus = bar();
    bus.resume();
    REQUIRE(foo_flag);
    REQUIRE(bar_flag);
  }

  TEST_CASE("task void nested await 2", "[task]") {
    bool foo_flag = false;
    bool bar_flag = false;

    auto foo = [&]() -> task<> {
      co_yield std::default_sentinel;
      foo_flag = true;
    };

    auto bar = [&]() -> task<> {
      co_await foo();
      bar_flag = true;
    };

    auto bus = bar();
    bus.resume();
    REQUIRE(!foo_flag);
    REQUIRE(!bar_flag);
    bus.resume();
    REQUIRE(foo_flag);
    REQUIRE(bar_flag);
  }

  TEST_CASE("task void nested await 3", "[task]") {
    bool foo_1 = false;
    bool foo_2 = false;
    bool bar_1 = false;
    bool bar_2 = false;

    auto foo = [&]() -> task<> {
      foo_1 = true;
      co_yield std::default_sentinel;
      foo_2 = true;
    };

    auto bar = [&]() -> task<> {
      bar_1 = true;
      co_yield std::default_sentinel;
      co_await foo();
      co_yield std::default_sentinel;
      bar_2 = true;
    };

    auto bus = bar();
    REQUIRE(!bar_1);
    bus.resume();
    REQUIRE(bar_1);
    REQUIRE(!foo_1);
    bus.resume();
    REQUIRE(foo_1);
    REQUIRE(!foo_2);
    bus.resume();
    REQUIRE(foo_2);
    REQUIRE(!bar_2);
    bus.resume();
    REQUIRE(bar_2);
  }

  TEST_CASE("task int nested await", "[task]") {
    bool foo_1 = false;
    bool foo_2 = false;
    bool bar_1 = false;
    bool bar_2 = false;

    auto foo = [&]() -> task<int> {
      foo_1 = true;
      co_yield std::default_sentinel;
      foo_2 = true;
      co_return 7;
    };

    auto bar = [&]() -> task<int> {
      bar_1 = true;
      co_yield std::default_sentinel;
      std::ignore = co_await foo();
      co_yield std::default_sentinel;
      bar_2 = true;
      co_return 7;
    };

    auto bus = bar();
    REQUIRE(!bar_1);
    bus.resume();
    REQUIRE(bar_1);
    REQUIRE(!foo_1);
    bus.resume();
    REQUIRE(foo_1);
    REQUIRE(!foo_2);
    bus.resume();
    REQUIRE(foo_2);
    REQUIRE(!bar_2);
    bus.resume();
    REQUIRE(bar_2);
    REQUIRE(bus.get() == 7);
  }
}
