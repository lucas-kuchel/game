#pragma once

namespace Debug
{
    enum class ErrorCode
    {
        GENERAL_ERROR = 1,
        OUT_OF_RANGE = 2,
        INVALID_ARGUMENT = 3,
        FILE_NOT_FOUND = 4,
        PERMISSION_DENIED = 5,
        INPUT_OUTPUT_ERROR = 6,
        NOT_IMPLEMENTED = 7,
        NULL_POINTER = 8,
        TIMEOUT = 9,
        CONFLICT = 10,
    };
}
