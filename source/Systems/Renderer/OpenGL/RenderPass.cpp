#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::CreateRenderPass(Resources::RenderPassData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::SubmitRenderQueue(Resources::RenderPassData& data, Resources::RenderQueueData& queueData)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::SubmitRenderPass(Resources::RenderPassData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeleteRenderPass(Resources::RenderPassData& data)
    {
    }
}