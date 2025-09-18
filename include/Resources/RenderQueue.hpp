#pragma once

#include <Resources/RenderPass.hpp>
#include <Resources/Submissions.hpp>

#include <cstdlib>
#include <limits>
#include <vector>

namespace Resources
{
    struct RenderQueueDescriptor
    {
        RenderPassHandle RenderPass;
    };

    struct RenderQueueData
    {
        std::vector<SubmissionHandle> Submissions;

        RenderPassHandle RenderPass;

        void* BackendMetadata = nullptr;
    };

    struct RenderQueueHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}