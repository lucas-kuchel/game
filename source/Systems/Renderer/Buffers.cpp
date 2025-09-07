#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::BufferHandle Renderer::CreateBuffer(const Resources::BufferDescriptor& descriptor)
    {
        Resources::BufferHandle handle;

        if (descriptor.Attributes.empty())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "any buffer requires at least 1 attribute");
        }
        else if (descriptor.Type == Resources::BufferType::INDEX)
        {
            if (descriptor.Attributes.size() != 1)
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "an index buffer requires exactly 1 attribute");
            }
            if (descriptor.Attributes[0].Type != Resources::BufferAttributeType::R16_UINT && descriptor.Attributes[0].Type != Resources::BufferAttributeType::R32_UINT)
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "an index buffer requires an attribute type of either Resources::BufferAttributeType::R16_UINT or Resources::BufferAttributeType::R32_UINT");
            }
        }
        else
        {
            for (const auto& attribute : descriptor.Attributes)
            {
                switch (attribute.Type)
                {
                    case Resources::BufferAttributeType::F32_2x2:
                    case Resources::BufferAttributeType::F32_2x3:
                    case Resources::BufferAttributeType::F32_2x4:
                    case Resources::BufferAttributeType::F32_3x2:
                    case Resources::BufferAttributeType::F32_3x3:
                    case Resources::BufferAttributeType::F32_3x4:
                    case Resources::BufferAttributeType::F32_4x2:
                    case Resources::BufferAttributeType::F32_4x3:
                    case Resources::BufferAttributeType::F32_4x4:
                    {
                        if (descriptor.Type == Resources::BufferType::VERTEX)
                        {
                            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                                       "invalid argument\n"
                                                                                       "vertex buffers do not support matrix types");
                        }

                        break;
                    }
                    case Resources::BufferAttributeType::R8_SNORM:
                    case Resources::BufferAttributeType::R8_UNORM:
                    case Resources::BufferAttributeType::R8G8_SNORM:
                    case Resources::BufferAttributeType::R8G8_UNORM:
                    case Resources::BufferAttributeType::R8G8B8_SNORM:
                    case Resources::BufferAttributeType::R8G8B8_UNORM:
                    case Resources::BufferAttributeType::R8G8B8A8_SNORM:
                    case Resources::BufferAttributeType::R8G8B8A8_UNORM:
                    case Resources::BufferAttributeType::R16_SNORM:
                    case Resources::BufferAttributeType::R16_UNORM:
                    case Resources::BufferAttributeType::R16G16_SNORM:
                    case Resources::BufferAttributeType::R16G16_UNORM:
                    case Resources::BufferAttributeType::R16G16B16_SNORM:
                    case Resources::BufferAttributeType::R16G16B16_UNORM:
                    case Resources::BufferAttributeType::R16G16B16A16_SNORM:
                    case Resources::BufferAttributeType::R16G16B16A16_UNORM:
                    {
                        if (descriptor.Type != Resources::BufferType::VERTEX)
                        {
                            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                                       "invalid argument\n"
                                                                                       "constant and storage buffers do not support normalised types");
                        }

                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }

        if (descriptor.Type == Resources::BufferType::VERTEX)
        {
            for (std::size_t i = 0; i < descriptor.Attributes.size(); i++)
            {
                for (std::size_t j = i; j < descriptor.Attributes.size(); j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    auto& one = descriptor.Attributes[i];
                    auto& two = descriptor.Attributes[j];

                    if (one.Slot == two.Slot)
                    {
                        throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                                                                   "invalid argument\n"
                                                                                   "two or more attributes are sharing slot {}",
                                               one.Slot);
                    }
                }
            }
        }

        if (mBufferFreeList.empty())
        {
            handle.ID = mBufferGenerations.size();

            mBufferGenerations.push_back(0);

            mBufferData.Insert(handle.ID, descriptor);
        }
        else
        {
            handle.ID = mBufferFreeList.back();
            handle.Generation = mBufferGenerations[handle.ID];

            mBufferFreeList.pop_back();

            mBufferData.Insert(handle.ID, descriptor);
        }

        std::visit([&](auto& backend)
                   { backend->CreateBuffer(handle, descriptor); }, mBackend);

        return handle;
    }

    Resources::BufferDescriptor Renderer::GetBufferInfo(const Resources::BufferHandle& handle)
    {
        if (!mBufferData.Contains(handle.ID) || mBufferGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::BufferDescriptor Systems::Renderer::GetBufferInfo(const Resources::BufferHandle&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        return mBufferData.Get(handle.ID);
    }

    void Renderer::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data)
    {
        if (!mBufferData.Contains(handle.ID) || mBufferGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::SetBufferData(const Resources::BufferHandle&, const Resources::BufferData&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        auto& info = mBufferData.Get(handle.ID);

        if (data.Offset + data.Size > info.Size)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::SetBufferData(const Resources::BufferHandle&, const Resources::BufferData&):\n"
                                   "Invalid argument\n"
                                   "provided size and offset for buffer data insertion exceeded buffer size");
        }

        std::visit([&](auto& backend)
                   { backend->SetBufferData(handle, data); }, mBackend);
    }

    void Renderer::DeleteBuffer(const Resources::BufferHandle& handle)
    {
        if (!mBufferData.Contains(handle.ID) || mBufferGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::DeleteBuffer(const Resources::BufferHandle&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        std::visit([&](auto& backend)
                   { backend->DeleteBuffer(handle); }, mBackend);

        mBufferGenerations[handle.ID]++;
        mBufferFreeList.push_back(handle.ID);
        mBufferData.Remove(handle.ID);
    }
}