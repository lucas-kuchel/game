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

    template <>
    void RendererBackendImplementation<RendererBackend::Vulkan>::Set<RendererAttribute::ClearColour>(const RendererClearColour& clearColour)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::Vulkan>::Set<RendererAttribute::VSyncMode>(const RendererVSyncMode& vsyncMode)
    {
    }
}
