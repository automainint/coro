/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_TASK_H
#define CORO_TASK_H

#include "memory.h"
#include "utils.h"
#include <iterator>

namespace coro {
  template <typename type_ = void> struct task {
    struct promise_type {
      using handle_type = std::coroutine_handle<promise_type>;

      type_ m_value = {};
      int m_ref_count = 0;
      std::exception_ptr m_exception = nullptr;
      bool m_continue = false;
      handle_type m_pending = nullptr;

      auto initial_suspend() noexcept;
      auto final_suspend() noexcept;
      auto get_return_object() noexcept;
      auto unhandled_exception() noexcept;
      void return_value(type_ const &_value) noexcept;
      void return_value(type_ &&_value) noexcept;
      auto yield_value(std::default_sentinel_t) noexcept;

      [[nodiscard]] static auto
      operator new(size_t size, std::pmr::memory_resource *resource =
                                    default_memory_resource())
          -> void *;
      static void operator delete(void *p);
    };

    using handle_type = typename promise_type::handle_type;

    task(task const &) noexcept;
    task(task &&) noexcept;
    auto operator=(task const &) noexcept -> task &;
    auto operator=(task &&) noexcept -> task &;

    explicit task(handle_type _handle) noexcept;
    ~task() noexcept;

    [[nodiscard]] auto await_ready() const noexcept;
    auto await_resume();

    auto await_suspend(std::coroutine_handle<>) noexcept;
    auto await_suspend(handle_type handle) noexcept;

    void resume() noexcept;

    [[nodiscard]] auto run();
    [[nodiscard]] auto get();
    [[nodiscard]] auto is_done() const noexcept;

  private:
    handle_type m_handle;
  };

  template <> struct task<void> {
    struct promise_type {
      using handle_type = std::coroutine_handle<promise_type>;

      int m_ref_count = 0;
      std::exception_ptr m_exception = nullptr;
      bool m_continue = false;
      handle_type m_pending = nullptr;

      auto initial_suspend() noexcept;
      auto final_suspend() noexcept;
      auto get_return_object() noexcept;
      auto unhandled_exception() noexcept;
      void return_void() noexcept;
      auto yield_value(std::default_sentinel_t) noexcept;

      [[nodiscard]] static auto
      operator new(size_t size, std::pmr::memory_resource *resource =
                                    default_memory_resource())
          -> void *;
      static void operator delete(void *p);
    };

    using handle_type = promise_type::handle_type;

    task(task const &) noexcept;
    task(task &&) noexcept;
    auto operator=(task const &) noexcept -> task &;
    auto operator=(task &&) noexcept -> task &;

    explicit task(handle_type _handle) noexcept;
    ~task() noexcept;

    [[nodiscard]] auto await_ready() const noexcept;
    auto await_resume();

    auto await_suspend(std::coroutine_handle<>) noexcept;
    auto await_suspend(handle_type handle) noexcept;

    void resume();

    [[nodiscard]] auto is_done() const noexcept;

  private:
    handle_type m_handle;
  };
}

#include "task.impl.h"

#endif
