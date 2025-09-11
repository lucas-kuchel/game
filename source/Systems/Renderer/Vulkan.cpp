#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(const WindowInteractionLayer<WindowInteractive::VULKAN_LAYER>& layer)
            : Layer(layer)
        {
        }

        WindowInteractionLayer<WindowInteractive::VULKAN_LAYER> Layer;
    };

    RendererBackendImplementation<RendererBackend::VULKAN>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
    }

    RendererBackendImplementation<RendererBackend::VULKAN>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::Update()
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::CreateBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::SetBufferData(const Resources::BufferHandle&, const Resources::BufferDescriptor&, const Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::DeleteBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::CreatePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::DeletePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::CreateSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::DeleteSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::CreateCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::DrawCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::VULKAN>::DeleteCommandBuffer(const CommandBuffer&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::VULKAN>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::VULKAN>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type&)
    {
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::VULKAN>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window.CreateInteractionLayer<WindowInteractive::VULKAN_LAYER>());
    }
}
