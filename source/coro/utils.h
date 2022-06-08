/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_UTILS_H
#define CORO_UTILS_H

#include "traits.h"
#include <coroutine>
#include <exception>

namespace coro {
  template <promise_not_shared promise_>
  void _acquire_handle(std::coroutine_handle<promise_>) noexcept;

  template <promise_shared promise_>
  void
  _acquire_handle(std::coroutine_handle<promise_> handle) noexcept;

  template <promise_not_shared promise_>
  void _release_handle(std::coroutine_handle<promise_>) noexcept;

  template <promise_shared promise_>
    requires(!promise_nested<promise_>)
  void _release_handle(
      std::coroutine_handle<promise_> handle) noexcept;

  template <promise_nested promise_>
  void
  _release_handle(std::coroutine_handle<promise_> handle) noexcept;

  template <promise_returning promise_>
    requires(!promise_nested<promise_>)
  auto _resume(std::coroutine_handle<promise_> handle) noexcept
      -> bool;

  template <promise_void promise_>
    requires(!promise_nested<promise_>)
  auto _resume(std::coroutine_handle<promise_> handle) -> bool;

  template <promise_nested promise_>
  auto _resume(std::coroutine_handle<promise_> handle) -> bool;

  template <promise_basic promise_>
  [[nodiscard]] auto
  _is_done(std::coroutine_handle<promise_> handle) noexcept;

  template <promise_value promise_>
    requires(!promise_nested<promise_>) [
        [nodiscard]] auto _get_value(std::coroutine_handle<promise_>
                                         handle);

  template <promise_value promise_>
    requires(promise_nested<promise_>) [
        [nodiscard]] auto _get_value(std::coroutine_handle<promise_>
                                         handle);

  template <promise_void promise_>
  [[nodiscard]] auto
  _get_value(std::coroutine_handle<promise_> handle);

  template <promise_nested upper_, typename under_>
  void _nest(std::coroutine_handle<upper_> upper,
             std::coroutine_handle<under_> under);

  template <typename upper_, typename under_>
    requires(!promise_nested<upper_>)
  void _nest(std::coroutine_handle<upper_>,
             std::coroutine_handle<under_>);
}

#include "utils.impl.h"

#endif
