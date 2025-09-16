#pragma once

#include <Resources/Buffers.hpp>
#include <Resources/Pipelines.hpp>

#include <cstdlib>
#include <vector>

namespace Resources
{
    enum class IndexType
    {
        U16,
        U32,
    };

    struct ShaderStageSubmissionDescriptor
    {
        std::vector<BufferHandle> ConstantBuffers;
        std::vector<BufferHandle> StorageBuffers;
    };

    struct SubmissionDescriptor
    {
        ShaderStageSubmissionDescriptor VertexShader;
        ShaderStageSubmissionDescriptor PixelShader;

        std::vector<BufferHandle> VertexBuffers;

        IndexType IndexFormat;
        BufferHandle IndexBuffer;
        PipelineHandle Pipeline;
    };

    struct SubmissionData
    {
        ShaderStageSubmissionDescriptor VertexShader;
        ShaderStageSubmissionDescriptor PixelShader;

        std::vector<BufferHandle> VertexBuffers;

        IndexType IndexFormat;
        BufferHandle IndexBuffer;
        PipelineHandle Pipeline;

        void* BackendMetadata = nullptr;
    };

    struct SubmissionHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}