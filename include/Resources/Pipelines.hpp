#pragma once

#include <Resources/Buffer.hpp>

#include <cstdlib>
#include <limits>
#include <string>
#include <vector>

namespace Resources
{
    struct ShaderDescriptor
    {
        std::vector<BufferFormat> ConstantBufferFormats;
        std::vector<BufferFormat> StorageBufferFormats;

        std::string Path;
        std::string Function;
    };

    enum class PolygonTopology
    {
        TriangleList,
        LineList,
        Point,
    };

    enum class PolygonFrontFace
    {
        Clockwise,
        Anticlockwise,
    };

    enum class PolygonFillMode
    {
        Solid,
        Line,
        Point,
    };

    enum class PolygonCullFace
    {
        Back,
        Front,
        None,
    };

    enum class ComparisonOperation
    {
        Never,
        Less,
        Equal,
        NotEqual,
        LessEqual,
        GreaterEqual,
        Greater,
        Always,
    };

    struct RasterPipelineDescriptor
    {
        ShaderDescriptor VertexShader;
        ShaderDescriptor PixelShader;

        std::vector<BufferFormat> VertexBufferFormats;

        PolygonFrontFace FrontFace = PolygonFrontFace::Clockwise;
        PolygonTopology Topology = PolygonTopology::LineList;
        PolygonFillMode FillMode = PolygonFillMode::Solid;
        PolygonCullFace CullFace = PolygonCullFace::Back;

        ComparisonOperation DepthComparison = ComparisonOperation::Less;

        bool DepthTest = true;
        bool DepthWrite = true;
    };

    struct RasterPipelineData
    {
        ShaderDescriptor VertexShader;
        ShaderDescriptor PixelShader;

        std::vector<BufferFormat> VertexBufferFormats;

        PolygonFrontFace FrontFace = PolygonFrontFace::Clockwise;
        PolygonTopology Topology = PolygonTopology::LineList;
        PolygonFillMode FillMode = PolygonFillMode::Solid;
        PolygonCullFace CullFace = PolygonCullFace::Back;

        ComparisonOperation DepthComparison = ComparisonOperation::Less;

        bool DepthTest = true;
        bool DepthWrite = true;

        void* BackendMetadata = nullptr;
    };

    struct PipelineHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}