#pragma once

#include <Debug/Codes.hpp>

#include <cstdint>
#include <format>
#include <string_view>

#if defined(DEBUG_BUILD_TYPE)
#include <string>
#endif

namespace Debug
{
    class Exception
    {
    public:
#if defined(DEBUG_BUILD_TYPE)
        template <typename... Ts>
        inline Exception(ErrorCode code, std::format_string<Ts...> message, Ts&&... args)
            : mMessage(std::format(message, std::forward<Ts>(args)...)), mCode(code)
        {
        }
#else
        template <typename... Ts>
        inline Exception(std::int32_t, std::format_string<Ts...>, Ts&&...) noexcept
        {
        }
#endif

        std::string_view GetMessage() const noexcept;
        std::uint32_t GetCode() const noexcept;

    private:
#if defined(DEBUG_BUILD_TYPE)
        std::string mMessage;
        ErrorCode mCode;
#endif
    };
}
