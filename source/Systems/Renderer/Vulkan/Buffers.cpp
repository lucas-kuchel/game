#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::Vulkan>::AllocateBuffer(Resources::BufferData& data)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::UploadBufferData(Resources::BufferData& data, const Resources::BufferUploadDescriptor& upload)
    {
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::DeallocateBuffer(Resources::BufferData& data)
    {
    }
}
#endif