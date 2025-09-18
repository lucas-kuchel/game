#pragma once

#include <Resources/Queue.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

namespace Resources
{
    struct RenderPassDescriptor
    {
        // frame attachments will go here
    };

    struct RenderPassData
    {
        std::vector<RenderQueueHandle> RenderQueues;

        void* BackendMetadata = nullptr;
    };

    struct RenderPassHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}