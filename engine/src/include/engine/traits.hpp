#pragma once

namespace engine {
    template <typename>
    inline constexpr bool IsFloatingType = false;

    template <>
    inline constexpr bool IsFloatingType<float> = true;

    template <>
    inline constexpr bool IsFloatingType<double> = true;

    template <typename, typename>
    inline constexpr bool SameTypes = false;

    template <typename T>
    inline constexpr bool SameTypes<T, T> = true;

    template <typename T, T Min, T Max, T N>
    inline constexpr bool IsInRange = Min <= N && N <= Max;
}