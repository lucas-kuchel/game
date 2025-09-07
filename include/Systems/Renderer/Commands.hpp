#pragma once

#include <Systems/Renderer/Backends.hpp>

namespace Systems
{
    class Renderer;

    class RendererCommandBuffer
    {
    public:
        ~RendererCommandBuffer();

        void Submit(const Resources::SubmissionHandle& submission);
        void Flush();

        const Types::SparseSet<Resources::SubmissionHandle>& GetContents() const;

    private:
        friend class Renderer;

        RendererCommandBuffer(RendererBackendVariant& backend, Renderer& renderer, std::size_t id);

        Types::SparseSet<Resources::SubmissionHandle> mBuffer;

        std::reference_wrapper<RendererBackendVariant> mBackend;
        std::reference_wrapper<Renderer> mRenderer;
        std::size_t mID;
    };
}