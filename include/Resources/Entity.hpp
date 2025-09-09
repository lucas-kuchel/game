#pragma once

#include <cstdlib>
#include <limits>

namespace Resources
{
    struct EntityHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = 0;

        bool Valid() const
        {
            return ID != std::numeric_limits<std::size_t>::max();
        }

        bool Invalid() const
        {
            return ID == std::numeric_limits<std::size_t>::max();
        }
    };
}