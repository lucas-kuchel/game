#pragma once

#include <spark/types/traits.hpp>

namespace spark {
    template <typename T>
    constexpr T&& forward(remove_reference<T>& value) noexcept {
        return static_cast<T&&>(value);
    }

    template <typename T>
    constexpr T&& forward(remove_reference<T>&& value) noexcept {
        return static_cast<T&&>(value);
    }

    template <typename T>
    constexpr remove_reference<T>&& move(T&& value) noexcept {
        return static_cast<remove_reference<T>&&>(value);
    }

    template <typename T>
    constexpr const T& min(const T& a, const T& b) noexcept {
        return (b < a) ? b : a;
    }

    template <typename T>
    constexpr const T& max(const T& a, const T& b) noexcept {
        return (a < b) ? b : a;
    }

    template <typename T>
    constexpr T abs(T value) noexcept {
        return (value < T(0)) ? -value : value;
    }
}