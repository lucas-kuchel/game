#pragma once

#include <cstdlib>
#include <limits>
#include <vector>

namespace Resources
{
    enum class BufferAttribute
    {
        F32_R,
        F32_RG,
        F32_RGB,
        F32_RGBA,

        I8_R,
        I8_RG,
        I8_RGB,
        I8_RGBA,
        I16_R,
        I16_RG,
        I16_RGB,
        I16_RGBA,
        I32_R,
        I32_RG,
        I32_RGB,
        I32_RGBA,

        U8_R,
        U8_RG,
        U8_RGB,
        U8_RGBA,
        U16_R,
        U16_RG,
        U16_RGB,
        U16_RGBA,
        U32_R,
        U32_RG,
        U32_RGB,
        U32_RGBA,

        SN8_R,
        SN8_RG,
        SN8_RGB,
        SN8_RGBA,
        SN16_R,
        SN16_RG,
        SN16_RGB,
        SN16_RGBA,

        UN8_R,
        UN8_RG,
        UN8_RGB,
        UN8_RGBA,
        UN16_R,
        UN16_RG,
        UN16_RGB,
        UN16_RGBA,

        F32_2x2,
        F32_3x3,
        F32_4x4,
        F32_2x3,
        F32_3x2,
        F32_2x4,
        F32_4x2,
        F32_3x4,
        F32_4x3,
    };

    struct BufferFormat
    {
        std::vector<BufferAttribute> Attributes;
    };

    struct BufferUploadDescriptor
    {
        void* Data = nullptr;

        std::size_t Stride = 0;
        std::size_t Offset = 0;
    };

    struct BufferData
    {
        std::size_t Size;

        void* BackendMetadata = nullptr;
    };

    struct BufferHandle
    {
        std::size_t ID = std::numeric_limits<std::size_t>::max();
        std::size_t Generation = std::numeric_limits<std::size_t>::max();
    };
}