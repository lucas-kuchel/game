#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Metal.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    struct RendererBackendImplementation<RendererBackend::Metal>::Internals
    {
    };

    RendererBackendImplementation<RendererBackend::Metal>::RendererBackendImplementation(const RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mCocoaWindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::CocoaBackend>())
    {
    }

    RendererBackendImplementation<RendererBackend::Metal>::~RendererBackendImplementation()
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::Clear()
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::Present()
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::AllocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::UploadBufferData(Resources::BufferData&, const Resources::BufferUploadDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeallocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreatePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeletePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreateSubmission(Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeleteSubmission(Resources::SubmissionData&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::Metal>::Set<RendererAttribute::VSyncMode>(const RendererVSyncMode&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::Metal>::Set<RendererAttribute::ClearColour>(const RendererClearColour&)
    {
    }
}
#endif