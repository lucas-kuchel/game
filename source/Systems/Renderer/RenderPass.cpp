#include <Systems/Renderer.hpp>

#include <Systems/Renderer/DirectX12.hpp>
#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Resources::RenderPassHandle Renderer::CreateRenderPass(const Resources::RenderPassDescriptor& descriptor)
    {
        Resources::RenderPassHandle handle;

        if (mRenderPasses.FreeList.empty())
        {
            handle.ID = mRenderPasses.Generations.size();
            mRenderPasses.Generations.push_back(0);
        }
        else
        {
            handle.ID = mRenderPasses.FreeList.back();
            handle.Generation = mRenderPasses.Generations[handle.ID];
            mRenderPasses.FreeList.pop_back();
        }

        auto& info = mRenderPasses.Data.Insert(handle.ID, Resources::RenderPassData());

        info.BackendMetadata = nullptr;
        info.ColourAttachments = descriptor.ColourAttachments;
        info.DepthAttachment = descriptor.DepthAttachment;
        info.StencilAttachment = descriptor.StencilAttachment;

        std::visit([&](auto& backend)
                   { backend->CreateRenderPass(info); }, mBackend);

        return handle;
    }

    Resources::RenderPassData Renderer::GetRenderPassData(const Resources::RenderPassHandle& handle)
    {
        if (!mRenderPasses.Data.Contains(handle.ID) || mRenderPasses.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::RenderPassData Systems::Renderer::GetRenderPassData(const Resources::RenderPassHandle&):\n"
                                   "Invalid argument\n"
                                   "provided render pass does not exist");
        }

        return mRenderPasses.Data.Get(handle.ID);
    }

    void Renderer::DeleteRenderPass(const Resources::RenderPassHandle& handle)
    {
        if (!mRenderPasses.Data.Contains(handle.ID) || mRenderPasses.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::DeleteRenderPass(const Resources::RenderPassHandle&):\n"
                                   "Invalid argument\n"
                                   "provided render pass does not exist");
        }

        auto& info = mRenderPasses.Data.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeleteRenderPass(info); }, mBackend);

        mRenderPasses.Generations[handle.ID]++;
        mRenderPasses.FreeList.push_back(handle.ID);
        mRenderPasses.Data.Remove(handle.ID);
    }
}