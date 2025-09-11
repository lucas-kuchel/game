#pragma once

#include <Systems/Renderer/Backends.hpp>
#include <Systems/Renderer/Commands.hpp>

#include <glm/glm.hpp>

namespace Systems
{
    enum class RendererAttribute : std::uint32_t
    {
        VSYNC_MODE,
        CLEAR_COLOUR,
    };

    enum class RendererVSyncMode : std::uint32_t
    {
        STRICT,
        RELAXED,
        DISABLED,
    };

    using RendererClearColour = glm::fvec4;
    using RendererContext = Context;
    using RendererWindow = Window;

    struct RendererDescriptor
    {
        RendererContext& Context;
        RendererWindow& Window;

        RendererClearColour ClearColour = {0.0f, 0.0f, 0.0f, 1.0f};
        RendererVSyncMode VSyncMode = RendererVSyncMode::STRICT;
    };

    template <RendererAttribute A>
    struct RendererAttributeType;

    template <>
    struct RendererAttributeType<RendererAttribute::VSYNC_MODE>
    {
        using Type = RendererVSyncMode;
    };

    template <>
    struct RendererAttributeType<RendererAttribute::CLEAR_COLOUR>
    {
        using Type = RendererClearColour;
    };

    class Renderer
    {
    public:
        Renderer(const RendererDescriptor& descriptor);
        ~Renderer();

        void Update();

        Resources::BufferHandle CreateBuffer(const Resources::BufferDescriptor& descriptor);
        Resources::BufferDescriptor GetBufferData(const Resources::BufferHandle& handle);
        void SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data);
        void DeleteBuffer(const Resources::BufferHandle& handle);

        Resources::PipelineHandle CreatePipeline(const Resources::RasterPipelineDescriptor& descriptor);
        Resources::RasterPipelineDescriptor GetRasterPipelineInfo(const Resources::PipelineHandle& handle);
        void DeletePipeline(const Resources::PipelineHandle& handle);

        Resources::SubmissionHandle CreateSubmission(Resources::SubmissionDescriptor& descriptor);
        Resources::SubmissionDescriptor GetSubmissionInfo(const Resources::SubmissionHandle& handle);
        void DeleteSubmission(const Resources::SubmissionHandle& handle);

        CommandBuffer CreateCommandBuffer();
        void DrawCommandBuffer(const CommandBuffer& buffer);

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

        Types::SparseSet<Resources::BufferDescriptor> mBufferData;
        Types::SparseSet<Resources::RasterPipelineDescriptor> mRasterPipelineData;
        Types::SparseSet<Resources::SubmissionDescriptor> mSubmissionData;

        std::vector<std::size_t> mBufferGenerations;
        std::vector<std::size_t> mPipelineGenerations;
        std::vector<std::size_t> mSubmissionGenerations;

        std::vector<std::size_t> mBufferFreeList;
        std::vector<std::size_t> mPipelineFreeList;
        std::vector<std::size_t> mSubmissionFreeList;

        std::size_t mCommandBufferCount = 0;
        std::vector<std::size_t> mCommandBufferFreeList;

        bool mClearColourDirty = false;
        bool mVSyncModeDirty = false;

        RendererBackendVariant mBackend;

        RendererBackendVariant CreateBackend(const RendererDescriptor& descriptor);
    };
}
