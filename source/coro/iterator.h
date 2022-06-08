/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_ITERATOR_H
#define CORO_ITERATOR_H

#include "utils.h"
#include <iterator>

namespace coro {
  template <promise_shared promise_>
  struct iterator {
    iterator(iterator const &) = delete;
    iterator &operator=(iterator const &) = delete;
    iterator &operator=(iterator &&) = delete;

    iterator(iterator &&other) noexcept;
    explicit iterator(
        std::coroutine_handle<promise_> handle) noexcept;
    ~iterator() noexcept;

    void               operator++() noexcept;
    [[nodiscard]] auto operator*();
    [[nodiscard]] auto operator!=(std::default_sentinel_t) noexcept;

  private:
    std::coroutine_handle<promise_> m_handle;
  };
}

#include "iterator.impl.h"

#endif
