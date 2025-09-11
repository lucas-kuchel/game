#pragma once

#include <Resources/Buffer.hpp>

#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

namespace Resources
{
    enum class ShaderStage
    {
        VERTEX,
        PIXEL,
        GEOMETRY,
    };

    struct ShaderDescriptor
    {
        ShaderStage Stage;

        std::vector<BufferFormatDescriptor> ConstantBufferFormats;
        std::vector<BufferFormatDescriptor> StorageBufferFormats;

        std::string Path;
        std::string Function;

        bool operator<(const ShaderDescriptor& other) const
        {
            return Stage < other.Stage;
        }
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

    struct PipelineRasterState
    {
        PipelinePrimitive Primitive;
        PipelineFrontFace FrontFace;
        PipelinePolygonMode PolygonMode;
        PipelineFaceCulling FaceCulling;
    };

    struct RasterPipelineDescriptor
    {
        std::vector<ShaderDescriptor> Shaders;

        std::vector<BufferFormatDescriptor> VertexBufferFormats;

        PipelineRasterState RasterState;
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