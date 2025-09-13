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

    enum class DepthCompareOperation
    {
        NEVER,
        LESS,
        NOT_EQUAL,
        EQUAL,
        LESS_EQUAL,
        GREATER_EQUAL,
        GREATER,
        ALWAYS,
    };

    struct PipelineDepthState
    {
        bool Read = true;
        bool Write = true;

        DepthCompareOperation Operation = DepthCompareOperation::LESS;
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
        PipelineDepthState DepthState;
    };

    struct PipelineHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = 0;
    };
}