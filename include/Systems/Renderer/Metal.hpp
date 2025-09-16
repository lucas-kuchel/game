#pragma once

#include <Systems/Context.hpp>
#include <Systems/Window.hpp>

#include <Types/SparseSet.hpp>

#include <Resources/Buffers.hpp>
#include <Resources/Pipelines.hpp>
#include <Resources/Submissions.hpp>

#include <memory>

namespace Systems
{
    enum class RendererAttribute : int;

    template <typename T>
    struct ResourceRegistry;

    template <RendererBackend>
    class RendererBackendImplementation;

    template <RendererAttribute>
    class RendererAttributeType;

    using RendererWindow = Window;

    struct RendererBackendDescriptor;

    template <>
    class RendererBackendImplementation<RendererBackend::Metal>
    {
    public:
        RendererBackendImplementation(const RendererBackendDescriptor& descriptor);
        ~RendererBackendImplementation();

        void Clear();
        void Present();

        void AllocateBuffer(Resources::BufferData& data);
        void UploadBufferData(Resources::BufferData& data, const Resources::BufferUploadDescriptor& upload);
        void DeallocateBuffer(Resources::BufferData& data);

        void CreatePipeline(Resources::RasterPipelineData& data);
        void DeletePipeline(Resources::RasterPipelineData& data);

        void CreateSubmission(Resources::SubmissionData& data);
        void DeleteSubmission(Resources::SubmissionData& data);

        template <RendererAttribute A>
        void Set(const RendererAttributeType<A>::Type& value);

    private:
        struct Internals;

        std::unique_ptr<Internals> mInternals;

        ResourceRegistry<Resources::SubmissionData>& mSubmissions;
        ResourceRegistry<Resources::RasterPipelineData>& mRasterPipelines;
        ResourceRegistry<Resources::BufferData>& mBuffers;

        RendererWindow& mWindow;

        WindowInteractionLayer<WindowInteractive::CocoaBackend> mCocoaWindowLayer;
    };
}