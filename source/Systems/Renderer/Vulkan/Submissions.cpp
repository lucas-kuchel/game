#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateSubmission(Resources::SubmissionData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeleteSubmission(Resources::SubmissionData& data)
    {
    }
}
