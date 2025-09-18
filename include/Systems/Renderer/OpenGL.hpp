#pragma once

#include <Systems/Context.hpp>
#include <Systems/Window.hpp>

#include <Types/SparseSet.hpp>

#include <Resources/Buffer.hpp>
#include <Resources/Pipelines.hpp>
#include <Resources/Queue.hpp>
#include <Resources/RenderPass.hpp>
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

    struct OpenGLBufferData
    {
        std::uint32_t ID = 0;
    };

    struct OpenGLRasterPipelineData
    {
        std::uint32_t ID = 0;

        std::uint32_t Primitive = 0;
        std::uint32_t FaceCulling = 0;
        std::uint32_t FrontFace = 0;
        std::uint32_t PolygonMode = 0;
        std::uint32_t DepthOperation = 0;
        std::uint32_t DepthWrite = 0;

        std::vector<std::uint32_t> ConstantBufferBindings;
        std::vector<std::uint32_t> StorageBufferBindings;

        bool CullBackface = false;
    };

    struct OpenGLSubmissionData
    {
        std::uint32_t ID = 0;

        std::vector<std::uint32_t> ConstantBufferHandles;
        std::vector<std::uint32_t> StorageBufferHandles;

        std::uint32_t IndexCount;
    };

    struct OpenGLAttributeFormat
    {
        std::uint32_t Type = 0;
        std::uint32_t Components = 0;
        std::uint32_t Size = 0;

        bool Normalized = false;
    };

    template <>
    class RendererBackendImplementation<RendererBackend::OpenGL>
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

        void CreateRenderQueue(Resources::RenderQueueData& data);
        void SubmitSubmission(Resources::RenderQueueData& data, Resources::SubmissionData& submissionData);
        void DeleteQueue(Resources::RenderQueueData& data);

        void CreateRenderPass(Resources::RenderPassData& data);
        void SubmitRenderQueue(Resources::RenderPassData& data, Resources::RenderQueueData& queueData);
        void SubmitRenderPass(Resources::RenderPassData& data);
        void DeleteRenderPass(Resources::RenderPassData& data);

        template <RendererAttribute A>
        void Set(const RendererAttributeType<A>::Type& value);

    private:
        struct Internals
        {
        };

        std::unique_ptr<Internals> mInternals;

        ResourceRegistry<Resources::SubmissionData>& mSubmissions;
        ResourceRegistry<Resources::RasterPipelineData>& mRasterPipelines;
        ResourceRegistry<Resources::BufferData>& mBuffers;

        RendererWindow& mWindow;

        WindowInteractionLayer<WindowInteractive::OpenGL> mOpenGLWindowLayer;

        static OpenGLAttributeFormat GetAttributeFormat(Resources::BufferAttribute attribute);
        static std::string ConvertShaderBinary(Resources::RasterPipelineData& pipelineData, Resources::ShaderDescriptor& shaderDescriptor, std::size_t stageIndex);
        static void DeduplicatePreserveOrder(std::vector<std::uint32_t>& vector);
    };
}