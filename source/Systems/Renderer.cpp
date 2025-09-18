#include <Systems/Renderer.hpp>

#include <Systems/Renderer/DirectX12.hpp>
#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Renderer::Renderer(const RendererDescriptor& descriptor)
        : mContext(descriptor.Context), mWindow(descriptor.Window),
          mVSyncMode(descriptor.VSyncMode), mBackend(CreateBackend(descriptor))
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Clear()
    {
        std::visit([](auto& backend)
                   { backend->Clear(); }, mBackend);
    }

    void Renderer::Present()
    {
        if (mVSyncModeDirty)
        {
            std::visit([this](auto& backend)
                       { backend->SetVSyncMode(mVSyncMode); }, mBackend);

            mVSyncModeDirty = false;
        }

        std::visit([](auto& backend)
                   { backend->Present(); }, mBackend);
    }

    const RendererVSyncMode& Renderer::GetVSyncMode() const
    {
        return mVSyncMode;
    }

    void Renderer::SetVSyncMode(RendererVSyncMode mode)
    {
        mVSyncMode = mode;
        mVSyncModeDirty = true;
    }

    RendererBackendVariant Renderer::CreateBackend(const RendererDescriptor& descriptor)
    {
        RendererBackendDescriptor backendDescriptor = {
            .Submissions = mSubmissions,
            .RasterPipelines = mRasterPipelines,
            .Buffers = mBuffers,
            .Window = descriptor.Window,
            .VSyncMode = descriptor.VSyncMode,
        };

        switch (descriptor.Context.Get<ContextAttribute::Renderer>())
        {
            case RendererBackend::DirectX12:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::DirectX12>>(backendDescriptor);
            }
            case RendererBackend::Vulkan:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::Vulkan>>(backendDescriptor);
            }
            case RendererBackend::Metal:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::Metal>>(backendDescriptor);
            }
        }

        throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(const RendererDescriptor&):\n"
                                                                "general error\n"
                                                                "no valid case for renderer backend found");
    }
}