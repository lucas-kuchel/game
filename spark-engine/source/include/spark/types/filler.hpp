#pragma once

#include <spark/types/core.hpp>

namespace spark {
    // @brief placeholder memory
    template <uint64 S, uint64 A>
    struct filler {
        alignas(A) uint8 data[S];
    };

    // @brief constructs a filler for the provided type
    template <typename T>
    using filler_of = filler<sizeof(T), alignof(T)>;
}