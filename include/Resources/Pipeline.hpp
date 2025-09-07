#pragma once

#include <Resources/Buffer.hpp>

#include <cstdlib>
#include <string>
#include <vector>

namespace Resources
{
    enum class ShaderType
    {
        VERTEX,
        PIXEL,
        GEOMETRY,
    };

    struct ShaderDescriptor
    {
        ShaderType Type;

        std::string Path;
    };

    enum class PipelinePrimitive
    {
        TRIANGLE_LIST,
        LINE_LIST,
        POINT,
    };

    struct PipelineDescriptor
    {
        std::vector<ShaderDescriptor> Shaders;
        std::vector<BufferAttributeDescriptor> VertexInput;
        std::vector<BufferHandle> InputBuffers;

        PipelinePrimitive Primitive;
    };

    struct PipelineHandle
    {
        std::size_t ID = 0;
        std::size_t Generation = 0;
    };
}