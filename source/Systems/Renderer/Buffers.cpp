#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::BufferHandle Renderer::CreateBuffer(const Resources::BufferDescriptor& descriptor)
    {
        Resources::BufferHandle handle;

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

    Resources::BufferDescriptor Renderer::GetBufferData(const Resources::BufferHandle& handle)
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

        if (data.Stride + data.Offset > info.Size)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::SetBufferData(const Resources::BufferHandle&, const Resources::BufferData&):\n"
                                   "Invalid argument\n"
                                   "provided stride and offset for buffer data insertion exceeded buffer size");
        }

        std::visit([&](auto& backend)
                   { backend->SetBufferData(handle, info, data); }, mBackend);
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

        auto& info = mBufferData.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeleteBuffer(handle, info); }, mBackend);

        mBufferGenerations[handle.ID]++;
        mBufferFreeList.push_back(handle.ID);
        mBufferData.Remove(handle.ID);
    }
}