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
}
