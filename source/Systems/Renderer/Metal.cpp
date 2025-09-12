#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics()
        {
        }
    };

    RendererBackendImplementation<RendererBackend::METAL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
    }

    RendererBackendImplementation<RendererBackend::METAL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::Update()
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SetBufferData(const Resources::BufferHandle&, const Resources::BufferDescriptor&, const Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreatePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeletePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteCommandBuffer(const CommandBuffer&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type&)
    {
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::METAL>::CreateSpecifics(const RendererDescriptor&)
    {
        return new RendererBackendImplementationSpecifics();
    }
}
#endif