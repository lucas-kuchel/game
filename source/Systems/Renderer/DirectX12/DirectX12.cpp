#if !defined(PLATFORM_WINDOWS)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/DirectX12.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    struct RendererBackendImplementation<RendererBackend::DirectX12>::Internals
    {
    };

    RendererBackendImplementation<RendererBackend::DirectX12>::RendererBackendImplementation(RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mWin32WindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::Win32Backend>())
    {
    }

    RendererBackendImplementation<RendererBackend::DirectX12>::~RendererBackendImplementation()
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::Clear()
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::Present()
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::AllocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::UploadBufferData(Resources::BufferData&, const Resources::BufferUploadDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::DeallocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::CreatePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::DeletePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::CreateSubmission(Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::DeleteSubmission(Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::CreateRenderQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::SubmitSubmission(Resources::RenderQueueData&, Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::CommitQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::DeleteQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::CreateRenderPass(Resources::RenderPassData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::DeleteRenderPass(Resources::RenderPassData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::DirectX12>::SetVSyncMode(RendererVSyncMode)
    {
    }
}
#endif