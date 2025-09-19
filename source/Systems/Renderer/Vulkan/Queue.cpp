#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateRenderQueue(Resources::RenderQueueData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SubmitSubmission(Resources::RenderQueueData& data, Resources::SubmissionData& submissionData)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CommitQueue(Resources::RenderQueueData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteQueue(Resources::RenderQueueData& data)
    {
    }
}
#endif