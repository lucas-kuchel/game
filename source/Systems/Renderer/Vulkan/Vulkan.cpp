#if defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    RendererBackendImplementation<RendererBackend::Vulkan>::RendererBackendImplementation(RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mVulkanWindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::Vulkan>())
    {
    }

    RendererBackendImplementation<RendererBackend::Vulkan>::~RendererBackendImplementation()
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::Clear()
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::Present()
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::AllocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::UploadBufferData(Resources::BufferData&, const Resources::BufferUploadDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeallocateBuffer(Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CreatePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeletePipeline(Resources::RasterPipelineData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateSubmission(Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteSubmission(Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateRenderQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SubmitSubmission(Resources::RenderQueueData&, Resources::SubmissionData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CommitQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteQueue(Resources::RenderQueueData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateRenderPass(Resources::RenderPassData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteRenderPass(Resources::RenderPassData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SetVSyncMode(RendererVSyncMode)
    {
    }
}
#endif