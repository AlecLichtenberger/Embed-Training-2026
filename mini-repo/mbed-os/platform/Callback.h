#pragma once
#include <functional>
#include <type_traits>

namespace mbed {

template <typename Signature>
class Callback;

template <typename R, typename... Args>
class Callback<R(Args...)> {
public:
    Callback() = default;
    Callback(std::nullptr_t) {}

    template <typename F>
    Callback(F f) : _fn(f) {}

    R call(Args... args) const {
        if constexpr (std::is_void_v<R>) {
            if (_fn) _fn(args...);
        } else {
            if (_fn) return _fn(args...);
            return R();
        }
    }

    R operator()(Args... args) const { return call(args...); }

    explicit operator bool() const { return static_cast<bool>(_fn); }

    

private:
    std::function<R(Args...)> _fn;
};


template <typename T, typename R, typename... Args>
Callback<R(Args...)> callback(T *obj, R (T::*method)(Args...)) {
    return Callback<R(Args...)>([obj, method](Args... args) -> R {
        return (obj->*method)(args...);
    });
}

template <typename T, typename R, typename... Args>
Callback<R(Args...)> callback(T *obj, R (T::*method)(Args...) const) {
    return Callback<R(Args...)>([obj, method](Args... args) -> R {
        return (obj->*method)(args...);
    });
}

} // namespace mbed

using mbed::Callback;