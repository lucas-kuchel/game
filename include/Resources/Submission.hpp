#pragma once

#include <Resources/Buffer.hpp>
#include <Resources/Pipeline.hpp>

#include <cstdlib>
#include <vector>

namespace Resources
{
    struct SubmissionDescriptor
    {
        std::vector<BufferHandle> VertexBuffers;
        BufferHandle IndexBuffer;

        PipelineHandle Pipeline;
    };

    struct SubmissionHandle
    {
        std::size_t ID = 0;
        std::size_t Generation = 0;
    };
}