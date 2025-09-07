#pragma once

#include <Systems/Context.hpp>
#include <Systems/Window.hpp>

#include <Types/SparseSet.hpp>

#include <Resources/Buffer.hpp>
#include <Resources/Pipeline.hpp>
#include <Resources/Submission.hpp>

#include <memory>
#include <variant>

#include <glm/glm.hpp>

namespace Systems
{
    enum class RendererAttribute : std::uint32_t;

    using RendererClearColour = glm::fvec4;
    using RendererContext = Context;
    using RendererWindow = Window;

    template <RendererBackend>
    class RendererBackendImplementation;

    class RendererBackendImplementationSpecifics;

    struct RendererDescriptor;

    class RendererCommandBuffer;

    template <RendererAttribute>
    struct RendererAttributeType;

    template <>
    class RendererBackendImplementation<RendererBackend::OPENGL>
    {
    public:
        RendererBackendImplementation(const RendererDescriptor& descriptor);
        RendererBackendImplementation(RendererBackendImplementation&&) = default;
        ~RendererBackendImplementation();

        void Update();

        void CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor);
        void SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data);
        void DeleteBuffer(const Resources::BufferHandle& handle);

        void CreatePipeline(const Resources::PipelineHandle& handle, const Resources::PipelineDescriptor& descriptor);
        void DeletePipeline(const Resources::PipelineHandle& handle);

        void CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor);
        void DeleteSubmission(const Resources::SubmissionHandle& handle);

        void CreateCommandBuffer(const RendererCommandBuffer& buffer);
        void SubmitToCommandBuffer(const RendererCommandBuffer& buffer, const Resources::SubmissionHandle& submission);
        void DrawCommandBuffer(const RendererCommandBuffer& buffer);
        void DeleteCommandBuffer(const RendererCommandBuffer& buffer);

        template <RendererAttribute A>
        void Set(const RendererAttributeType<A>::Type& value);

    private:
        RendererBackendImplementationSpecifics* mSpecifics;

        RendererBackendImplementationSpecifics* CreateSpecifics(const RendererDescriptor& descriptor);
    };

    template <>
    class RendererBackendImplementation<RendererBackend::VULKAN>
    {
    public:
        RendererBackendImplementation(const RendererDescriptor& descriptor);
        RendererBackendImplementation(RendererBackendImplementation&&) = default;
        ~RendererBackendImplementation();

        void Update();

        void CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor);
        void SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data);
        void DeleteBuffer(const Resources::BufferHandle& handle);

        void CreatePipeline(const Resources::PipelineHandle& handle, const Resources::PipelineDescriptor& descriptor);
        void DeletePipeline(const Resources::PipelineHandle& handle);

        void CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor);
        void DeleteSubmission(const Resources::SubmissionHandle& handle);

        void CreateCommandBuffer(const RendererCommandBuffer& buffer);
        void SubmitToCommandBuffer(const RendererCommandBuffer& buffer, const Resources::SubmissionHandle& submission);
        void DrawCommandBuffer(const RendererCommandBuffer& buffer);
        void DeleteCommandBuffer(const RendererCommandBuffer& buffer);

        template <RendererAttribute A>
        void Set(const RendererAttributeType<A>::Type& value);

    private:
        RendererBackendImplementationSpecifics* mSpecifics;

        RendererBackendImplementationSpecifics* CreateSpecifics(const RendererDescriptor& descriptor);
    };

    using RendererBackendVariant = std::variant<
        std::unique_ptr<RendererBackendImplementation<RendererBackend::OPENGL>>,
        std::unique_ptr<RendererBackendImplementation<RendererBackend::VULKAN>>>;
}