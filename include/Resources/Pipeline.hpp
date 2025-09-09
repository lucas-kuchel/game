#pragma once

#include <Resources/Buffer.hpp>

#include <cstdlib>
#include <limits>
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

    enum class PipelineFrontFace
    {
        CLOCKWISE,
        ANTICLOCKWISE,
    };

    enum class PipelinePolygonMode
    {
        SOLID,
        LINE,
        POINT,
    };

    enum class PipelineFaceCulling
    {
        BACKFACE,
        FRONTFACE,
        DISABLED,
    };

    struct RasterPipelineDescriptor
    {
        std::vector<ShaderDescriptor> Shaders;

        std::vector<BufferFormatDescriptor> DataBufferFormats;
        std::vector<BufferFormatDescriptor> VertexBufferFormats;

        PipelinePrimitive Primitive;
        PipelineFrontFace FrontFace;
        PipelinePolygonMode PolygonMode;
        PipelineFaceCulling FaceCulling;
    };

    struct PipelineHandle
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