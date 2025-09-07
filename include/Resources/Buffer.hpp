#pragma once

#include <cstdlib>
#include <vector>

namespace Resources
{
    enum class BufferType
    {
        VERTEX,
        INDEX,
        STORAGE,
        CONSTANT,
    };

    enum class BufferAttributeType
    {
        R32_FLOAT,
        R32G32_FLOAT,
        R32G32B32_FLOAT,
        R32G32B32A32_FLOAT,

        R8_INT,
        R8G8_INT,
        R8G8B8_INT,
        R8G8B8A8_INT,

        R16_INT,
        R16G16_INT,
        R16G16B16_INT,
        R16G16B16A16_INT,

        R32_INT,
        R32G32_INT,
        R32G32B32_INT,
        R32G32B32A32_INT,

        R8_UINT,
        R8G8_UINT,
        R8G8B8_UINT,
        R8G8B8A8_UINT,

        R16_UINT,
        R16G16_UINT,
        R16G16B16_UINT,
        R16G16B16A16_UINT,

        R32_UINT,
        R32G32_UINT,
        R32G32B32_UINT,
        R32G32B32A32_UINT,

        R8_SNORM,
        R8G8_SNORM,
        R8G8B8_SNORM,
        R8G8B8A8_SNORM,

        R16_SNORM,
        R16G16_SNORM,
        R16G16B16_SNORM,
        R16G16B16A16_SNORM,

        R8_UNORM,
        R8G8_UNORM,
        R8G8B8_UNORM,
        R8G8B8A8_UNORM,

        R16_UNORM,
        R16G16_UNORM,
        R16G16B16_UNORM,
        R16G16B16A16_UNORM,

        F32_2x2,
        F32_3x3,
        F32_4x4,
        F32_2x3,
        F32_3x2,
        F32_2x4,
        F32_4x2,
        F32_3x4,
        F32_4x3
    };

    struct BufferAttributeDescriptor
    {
        BufferAttributeType Type;

        std::size_t Slot = 0;

        bool operator<(const BufferAttributeDescriptor& other) const
        {
            return Slot < other.Slot && Type < other.Type;
        }

        bool operator==(const BufferAttributeDescriptor& other) const
        {
            return Slot == other.Slot && Type == other.Type;
        }
    };

    struct BufferDescriptor
    {
        BufferType Type = BufferType::STORAGE;

        std::vector<BufferAttributeDescriptor> Attributes;

        std::size_t Slot = 0;
        std::size_t Size = 0;
    };

    struct BufferData
    {
        void* Data = nullptr;

        std::size_t Size = 0;
        std::size_t Offset = 0;
    };

    struct BufferHandle
    {
        std::size_t ID = 0;
        std::size_t Generation = 0;
    };
}