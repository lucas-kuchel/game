#pragma once

#include <Systems/Context.hpp>
#include <Systems/Window.hpp>

#include <Types/SparseSet.hpp>

#include <Resources/Buffer.hpp>
#include <Resources/Pipelines.hpp>
#include <Resources/RenderPass.hpp>
#include <Resources/RenderQueue.hpp>
#include <Resources/Submissions.hpp>

#include <vulkan/vulkan.h>

namespace Systems
{
    enum class RendererVSyncMode : int;

    template <typename T>
    struct ResourceRegistry;

    template <RendererBackend>
    class RendererBackendImplementation;

    using RendererWindow = Window;

    struct RendererBackendDescriptor;

    template <>
    class RendererBackendImplementation<RendererBackend::Vulkan>
    {
    public:
        RendererBackendImplementation(RendererBackendDescriptor& descriptor);
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

        void CreateRenderQueue(Resources::RenderQueueData& data);
        void SubmitSubmission(Resources::RenderQueueData& data, Resources::SubmissionData& submissionData);
        void CommitQueue(Resources::RenderQueueData& data);
        void DeleteQueue(Resources::RenderQueueData& data);

        void CreateRenderPass(Resources::RenderPassData& data);
        void DeleteRenderPass(Resources::RenderPassData& data);

        void SetVSyncMode(RendererVSyncMode mode);

    private:
        struct Internals
        {
#if !defined(PLATFORM_APPLE)
            VkInstance Instance;
            VkPhysicalDevice PhysicalDevice;
#endif
        };

        Internals mInternals;

        ResourceRegistry<Resources::SubmissionData>& mSubmissions;
        ResourceRegistry<Resources::RasterPipelineData>& mRasterPipelines;
        ResourceRegistry<Resources::BufferData>& mBuffers;

        RendererWindow& mWindow;

        WindowInteractionLayer<WindowInteractive::Vulkan> mVulkanWindowLayer;

        void CreateInstance();
        void SelectPhysicalDevice();
    };
}