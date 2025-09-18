#include <Systems/Renderer.hpp>

#include <Systems/Renderer/DirectX12.hpp>
#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Resources::BufferHandle Renderer::AllocateBuffer(std::size_t size)
    {
        Resources::BufferHandle handle;

        if (mBuffers.FreeList.empty())
        {
            handle.ID = mBuffers.Generations.size();
            mBuffers.Generations.push_back(0);
        }
        else
        {
            handle.ID = mBuffers.FreeList.back();
            handle.Generation = mBuffers.Generations[handle.ID];
            mBuffers.FreeList.pop_back();
        }

        auto& info = mBuffers.Data.Insert(handle.ID, Resources::BufferData{.Size = size, .BackendMetadata = nullptr});

        std::visit([&](auto& backend)
                   { backend->AllocateBuffer(info); }, mBackend);

        return handle;
    }

    Resources::BufferData Renderer::GetBufferData(const Resources::BufferHandle& handle)
    {
        if (!mBuffers.Data.Contains(handle.ID) || mBuffers.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::BufferDescriptor Systems::Renderer::GetBufferInfo(const Resources::BufferHandle&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        return mBuffers.Data.Get(handle.ID);
    }

    void Renderer::UploadBufferData(const Resources::BufferHandle& handle, const Resources::BufferUploadDescriptor& upload)
    {
        if (!mBuffers.Data.Contains(handle.ID) || mBuffers.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::SetBufferData(const Resources::BufferHandle&, const Resources::BufferData&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        auto& info = mBuffers.Data.Get(handle.ID);

        if (upload.Stride + upload.Offset > info.Size)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::SetBufferData(const Resources::BufferHandle&, const Resources::BufferData&):\n"
                                   "Invalid argument\n"
                                   "provided stride and offset for buffer data insertion exceeded buffer size");
        }

        std::visit([&](auto& backend)
                   { backend->UploadBufferData(info, upload); }, mBackend);
    }

    void Renderer::DeallocateBuffer(const Resources::BufferHandle& handle)
    {
        if (!mBuffers.Data.Contains(handle.ID) || mBuffers.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::DeleteBuffer(const Resources::BufferHandle&):\n"
                                   "Invalid argument\n"
                                   "provided buffer does not exist");
        }

        auto& info = mBuffers.Data.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeallocateBuffer(info); }, mBackend);

        mBuffers.Generations[handle.ID]++;
        mBuffers.FreeList.push_back(handle.ID);
        mBuffers.Data.Remove(handle.ID);
    }
}