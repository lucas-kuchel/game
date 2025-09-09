#include <Systems/Renderer.hpp>

namespace Systems
{
    Renderer::Renderer(const RendererDescriptor& descriptor)
        : mContext(descriptor.Context), mWindow(descriptor.Window), mClearColour(descriptor.ClearColour),
          mVSyncMode(descriptor.VSyncMode), mBackend(CreateBackend(descriptor))
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::Update()
    {
        if (mClearColourDirty)
        {
            std::visit([this](auto& backend)
                       { backend->template Set<RendererAttribute::CLEAR_COLOUR>(mClearColour); }, mBackend);

            mClearColourDirty = false;
        }

        if (mVSyncModeDirty)
        {
            std::visit([this](auto& backend)
                       { backend->template Set<RendererAttribute::VSYNC_MODE>(mVSyncMode); }, mBackend);

            mVSyncModeDirty = false;
        }

        std::visit([](auto& backend)
                   { backend->Update(); }, mBackend);
    }

    template <>
    const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& Renderer::Get<RendererAttribute::CLEAR_COLOUR>() const
    {
        return mClearColour;
    }

    template <>
    const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type& Renderer::Get<RendererAttribute::VSYNC_MODE>() const
    {
        return mVSyncMode;
    }

    template <>
    void Renderer::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& value)
    {
        mClearColour = value;
        mClearColourDirty = true;
    }

    template <>
    void Renderer::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type& value)
    {
        mVSyncMode = value;
        mVSyncModeDirty = true;
    }

    RendererBackendVariant Renderer::CreateBackend(const RendererDescriptor& descriptor)
    {
        switch (descriptor.Context.Get<ContextAttribute::RENDERER>())
        {
            case RendererBackend::OPENGL:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::OPENGL>>(descriptor);
            }
            case RendererBackend::VULKAN:
            {
                return std::make_unique<RendererBackendImplementation<RendererBackend::VULKAN>>(descriptor);
            }
        }

        throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(const RendererDescriptor&):\n"
                                                                "general error\n"
                                                                "no valid case for renderer backend found");
    }
}