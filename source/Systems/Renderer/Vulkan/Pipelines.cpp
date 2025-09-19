#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::CreatePipeline(Resources::RasterPipelineData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeletePipeline(Resources::RasterPipelineData& data)
    {
    }
}
#endif