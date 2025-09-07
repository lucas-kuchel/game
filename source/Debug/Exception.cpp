#include <Debug/Exception.hpp>

namespace Debug
{
    std::string_view Exception::GetMessage() const noexcept
    {
#if defined(DEBUG_BUILD_TYPE)
        return mMessage;
#else
        return "";
#endif
    }

    std::uint32_t Exception::GetCode() const noexcept
    {
#if defined(DEBUG_BUILD_TYPE)
        return std::uint32_t(mCode);
#else
        return 1;
#endif
    }
}