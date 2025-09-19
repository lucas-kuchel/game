#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::Clear()
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::Present()
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SetVSyncMode(RendererVSyncMode)
    {
    }
}
#endif