#pragma once

#include <Systems/Context.hpp>
#include <Systems/Window.hpp>

#include <Resources/Buffer.hpp>
#include <Resources/Pipelines.hpp>
#include <Resources/Queue.hpp>
#include <Resources/RenderPass.hpp>
#include <Resources/Submissions.hpp>

#include <Types/SparseSet.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <variant>

namespace Systems
{
    enum class RendererAttribute
    {
        VSyncMode,
        ClearColour,
    };

    enum class RendererVSyncMode
    {
        Enabled,
        AllowLate,
        Disabled,
    };

    using RendererClearColour = glm::fvec4;
    using RendererContext = Context;
    using RendererWindow = Window;

    struct RendererDescriptor
    {
        RendererContext& Context;
        RendererWindow& Window;

        RendererClearColour ClearColour = {0.0f, 0.0f, 0.0f, 1.0f};
        RendererVSyncMode VSyncMode = RendererVSyncMode::Enabled;
    };

    template <RendererAttribute A>
    struct RendererAttributeType;

    template <>
    struct RendererAttributeType<RendererAttribute::VSyncMode>
    {
        using Type = RendererVSyncMode;
    };

    template <>
    struct RendererAttributeType<RendererAttribute::ClearColour>
    {
        using Type = RendererClearColour;
    };

    template <typename T>
    struct ResourceRegistry
    {
        Types::SparseSet<T> Data;

        std::vector<std::size_t> FreeList;
        std::vector<std::size_t> Generations;
    };

    template <RendererBackend>
    class RendererBackendImplementation;

    struct RendererBackendDescriptor
    {
        ResourceRegistry<Resources::SubmissionData>& Submissions;
        ResourceRegistry<Resources::RasterPipelineData>& RasterPipelines;
        ResourceRegistry<Resources::BufferData>& Buffers;

        RendererWindow& Window;

        RendererClearColour ClearColour;
        RendererVSyncMode VSyncMode;
    };

    using RendererBackendVariant = std::variant<
        std::unique_ptr<RendererBackendImplementation<RendererBackend::OpenGL>>,
        std::unique_ptr<RendererBackendImplementation<RendererBackend::Vulkan>>,
        std::unique_ptr<RendererBackendImplementation<RendererBackend::Metal>>>;

    class Renderer
    {
    public:
        Renderer(const RendererDescriptor& descriptor);
        ~Renderer();

        void Clear();
        void Present();

        Resources::BufferHandle AllocateBuffer(std::size_t size);
        Resources::BufferData GetBufferData(const Resources::BufferHandle& handle);

        void UploadBufferData(const Resources::BufferHandle& handle, const Resources::BufferUploadDescriptor& upload);
        void DeallocateBuffer(const Resources::BufferHandle& handle);

        Resources::PipelineHandle CreatePipeline(const Resources::RasterPipelineDescriptor& descriptor);
        Resources::RasterPipelineData GetRasterPipelineData(const Resources::PipelineHandle& handle);

        void DeletePipeline(const Resources::PipelineHandle& handle);

        Resources::SubmissionHandle CreateSubmission(Resources::SubmissionDescriptor& descriptor);
        Resources::SubmissionData GetSubmissionData(const Resources::SubmissionHandle& handle);

        void DeleteSubmission(const Resources::SubmissionHandle& handle);

        Resources::RenderQueueHandle CreateRenderQueue();
        Resources::RenderQueueData GetRenderQueueData(const Resources::RenderQueueHandle& handle);

        void SubmitSubmission(const Resources::RenderQueueHandle& handle, const Resources::SubmissionHandle& submission);
        void DeleteQueue(const Resources::RenderQueueHandle& handle);

        Resources::RenderPassHandle CreateRenderPass(const Resources::RenderPassDescriptor& descriptor);
        Resources::RenderPassData GetRenderPassData(const Resources::RenderPassHandle& handle);

        void SubmitRenderQueue(const Resources::RenderPassHandle& handle, const Resources::RenderQueueHandle& queue);
        void SubmitRenderPass(const Resources::RenderPassHandle& handle);
        void DeleteRenderPass(const Resources::RenderPassHandle& handle);

        template <RendererAttribute A>
        const RendererAttributeType<A>::Type& Get() const;

        template <RendererAttribute A>
        void Set(const RendererAttributeType<A>::Type& value);

    private:
        friend class CommandBuffer;

        RendererContext& mContext;
        RendererWindow& mWindow;

        RendererClearColour mClearColour;
        RendererVSyncMode mVSyncMode;

        ResourceRegistry<Resources::SubmissionData> mSubmissions;
        ResourceRegistry<Resources::RasterPipelineData> mRasterPipelines;
        ResourceRegistry<Resources::BufferData> mBuffers;
        ResourceRegistry<Resources::RenderQueueData> mRenderQueues;
        ResourceRegistry<Resources::RenderPassData> mRenderPasses;

        bool mClearColourDirty = false;
        bool mVSyncModeDirty = false;

        RendererBackendVariant mBackend;

        RendererBackendVariant CreateBackend(const RendererDescriptor& descriptor);
    };
}
