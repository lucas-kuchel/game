#pragma once

#include <Resources/Buffer.hpp>
#include <Resources/Pipeline.hpp>

#include <cstdlib>
#include <vector>

namespace Resources
{
    struct ShaderStageSubmissionDescriptor
    {
        std::vector<BufferHandle> ConstantBuffers;
        std::vector<BufferHandle> StorageBuffers;

        ShaderStage Stage;

        bool operator<(const ShaderStageSubmissionDescriptor& other) const
        {
            return Stage < other.Stage;
        }
    };

    struct SubmissionDescriptor
    {
        std::vector<ShaderStageSubmissionDescriptor> ShaderStages;
        std::vector<BufferHandle> VertexBuffers;

        BufferHandle IndexBuffer;
        BufferAttributeType IndexBufferType;

        PipelineHandle Pipeline;
    };

    struct SubmissionHandle
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