#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateRenderPass(Resources::RenderPassData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SubmitRenderQueue(Resources::RenderPassData& data, Resources::RenderQueueData& queueData)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SubmitRenderPass(Resources::RenderPassData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteRenderPass(Resources::RenderPassData& data)
    {
    }
}