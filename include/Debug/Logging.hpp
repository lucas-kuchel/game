#pragma once

#include <format>

#if defined(DEBUG_BUILD_TYPE)
#include <iostream>
#endif

namespace Debug
{
#if defined(DEBUG_BUILD_TYPE)
    template <typename... Ts>
    inline void Log(std::format_string<Ts...> message, Ts&&... args) noexcept
    {

        std::string formatted = std::format(message, std::forward<Ts>(args)...);

        std::cout << "[DEBUG] LOG: " << formatted << std::endl;
    }

    template <typename... Ts>
    inline void LogWarning(std::format_string<Ts...> message, Ts&&... args) noexcept
    {

        std::string formatted = std::format(message, std::forward<Ts>(args)...);

        std::cout << "[DEBUG] WARNING: " << formatted << std::endl;
    }

    template <typename... Ts>
    inline void LogError(std::format_string<Ts...> message, Ts&&... args) noexcept
    {

        std::string formatted = std::format(message, std::forward<Ts>(args)...);

        std::cout << "[DEBUG] ERROR: " << formatted << std::endl;
    }
#else
    template <typename... Ts>
    inline void Log(std::format_string<Ts...>, Ts&&...) noexcept
    {
    }

    template <typename... Ts>
    inline void LogWarning(std::format_string<Ts...>, Ts&&...) noexcept
    {
    }

    template <typename... Ts>
    inline void LogError(std::format_string<Ts...>, Ts&&...) noexcept
    {
    }
#endif
}