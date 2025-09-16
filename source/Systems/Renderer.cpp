#include <Systems/Renderer.hpp>

#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/OpenGL.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Renderer::Renderer(const RendererDescriptor& descriptor)
        : mContext(descriptor.Context), mWindow(descriptor.Window), mClearColour(descriptor.ClearColour),
          mVSyncMode(descriptor.VSyncMode), mBackend(CreateBackend(descriptor))
    {
        std::visit([this](auto& backend)
                   { backend->template Set<RendererAttribute::ClearColour>(mClearColour); }, mBackend);
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
        if (mClearColourDirty)
        {
            std::visit([this](auto& backend)
                       { backend->template Set<RendererAttribute::ClearColour>(mClearColour); }, mBackend);

            mClearColourDirty = false;
        }

        if (mVSyncModeDirty)
        {
            std::visit([this](auto& backend)
                       { backend->template Set<RendererAttribute::VSyncMode>(mVSyncMode); }, mBackend);

            mVSyncModeDirty = false;
        }

        std::visit([](auto& backend)
                   { backend->Present(); }, mBackend);
    }

    template <>
    const RendererAttributeType<RendererAttribute::ClearColour>::Type& Renderer::Get<RendererAttribute::ClearColour>() const
    {
        return mClearColour;
    }

    template <>
    const RendererAttributeType<RendererAttribute::VSyncMode>::Type& Renderer::Get<RendererAttribute::VSyncMode>() const
    {
        return mVSyncMode;
    }

    template <>
    void Renderer::Set<RendererAttribute::ClearColour>(const RendererClearColour& value)
    {
        mClearColour = value;
        mClearColourDirty = true;
    }

    template <>
    void Renderer::Set<RendererAttribute::VSyncMode>(const RendererVSyncMode& value)
    {
        mVSyncMode = value;
        mVSyncModeDirty = true;
    }

    RendererBackendVariant Renderer::CreateBackend(const RendererDescriptor& descriptor)
    {
        RendererBackendDescriptor backendDescriptor = {
            .Submissions = mSubmissions,
            .RasterPipelines = mRasterPipelines,
            .Buffers = mBuffers,
            .Window = descriptor.Window,
            .ClearColour = descriptor.ClearColour,
            .VSyncMode = descriptor.VSyncMode,
        };

        switch (descriptor.Context.Get<ContextAttribute::Renderer>())
        {
            case RendererBackend::OpenGL:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::OpenGL>>(backendDescriptor);
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