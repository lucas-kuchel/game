#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <GL/glew.h>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::CreateRenderQueue(Resources::RenderQueueData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::AppendToQueue(Resources::RenderQueueData& data, Resources::SubmissionData& submissionData)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::SubmitQueue(Resources::RenderQueueData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeleteQueue(Resources::RenderQueueData& data)
    {
    }
}