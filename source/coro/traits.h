/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef CORO_TRAITS_H
#define CORO_TRAITS_H

#include <concepts>
#include <utility>

namespace coro {
  template <typename promise_>
  concept promise_basic = requires(promise_ promise) {
                            promise.initial_suspend();
                            promise.final_suspend();
                            promise.get_return_object();
                            promise.unhandled_exception();
                          };

  template <typename promise_>
  concept promise_value =
      promise_basic<promise_> &&
      requires(promise_ promise) { std::move(promise.m_value); };

  template <typename promise_>
  concept promise_returning =
      promise_basic<promise_> &&
      requires(promise_ promise) {
        promise.return_value(decltype(promise.m_value) {});
      };

  template <typename promise_>
  concept promise_void = promise_basic<promise_> && !
  promise_value<promise_>;

  template <typename promise_>
  concept promise_shared =
      promise_basic<promise_> && requires(promise_ promise) {
                                   ++promise.m_ref_count;
                                   --promise.m_ref_count == 0;
                                 };

  template <typename promise_>
  concept promise_not_shared = promise_basic<promise_> && !
  promise_shared<promise_>;

  template <typename promise_>
  concept promise_nested =
      promise_shared<promise_> && requires(promise_ promise) {
                                    promise.m_continue;
                                    promise.m_pending;
                                  };
}

#endif
