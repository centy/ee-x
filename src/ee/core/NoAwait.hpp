#ifndef EE_X_NO_AWAIT_HPP
#define EE_X_NO_AWAIT_HPP

#ifdef __cplusplus

#include <experimental/coroutine>
#include <type_traits>

#include "ee/CoreFwd.hpp"

namespace ee {
namespace core {
/// https://devblogs.microsoft.com/oldnewthing/20190322-00/?p=102354
struct CompleteAsynchronously {
    struct promise_type {
        auto get_return_object() const noexcept {
            return CompleteAsynchronously();
        }

        void return_void() const noexcept {}

        auto initial_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        auto final_suspend() const noexcept { //
            return std::experimental::suspend_never();
        }

        void unhandled_exception() noexcept { //
            std::terminate();
        }
    };
};

template <class T, class = std::enable_if_t<std::is_invocable_v<T>>>
CompleteAsynchronously
noAwait(T lambda) /** Must copy lambda, don't use T&& */ {
    co_await lambda();
}
} // namespace core

using core::noAwait;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_NO_AWAIT_HPP
