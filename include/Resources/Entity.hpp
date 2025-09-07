#pragma once

#include <cstdlib>

namespace Resources
{
    struct EntityHandle
    {
        std::size_t ID;
        std::size_t Generation;
    };
}