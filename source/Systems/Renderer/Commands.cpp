#include <Systems/Renderer.hpp>

namespace Systems
{
    RendererCommandBuffer::RendererCommandBuffer(RendererBackendVariant& backend, Renderer& renderer, std::size_t id)
        : mBackend(backend), mRenderer(renderer), mID(id)
    {
        std::visit([this](auto& backend)
                   { backend->CreateCommandBuffer(*this); }, mBackend.get());
    }

    RendererCommandBuffer::~RendererCommandBuffer()
    {
        std::visit([this](auto& backend)
                   { backend->DeleteCommandBuffer(*this); }, mBackend.get());

        auto& renderer = mRenderer.get();

        renderer.mCommandBufferFreeList.push_back(mID);
        renderer.mCommandBufferCount--;
    }

    void RendererCommandBuffer::Submit(const Resources::SubmissionHandle& submission)
    {
        if (mBuffer.Contains(submission.ID))
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::RendererCommandBuffer::Submit(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided submission has already been submitted into this command buffer");
        }

        mBuffer.Insert(submission.ID, submission);

        std::visit([&, this](auto& backend)
                   { backend->SubmitToCommandBuffer(*this, submission); }, mBackend.get());
    }

    void RendererCommandBuffer::Flush()
    {
        mBuffer.Clear();
        mBuffer.ShrinkToFit();
    }

    RendererCommandBuffer Renderer::CreateCommandBuffer()
    {
        std::size_t id = 0;

        if (mCommandBufferFreeList.empty())
        {
            id = mCommandBufferCount;

            mCommandBufferCount++;
        }
        else
        {
            id = mCommandBufferFreeList.back();
            mCommandBufferFreeList.pop_back();
        }

        return RendererCommandBuffer(mBackend, *this, id);
    }

    void Renderer::DrawCommandBuffer(const RendererCommandBuffer& buffer)
    {
        if (buffer.mBuffer.Empty())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DrawCommandBuffer(const RendererCommandBuffer&):\n"
                                                                       "invalid argument\n"
                                                                       "provided command buffer is empty");
        }

        std::visit([&](auto& backend)
                   { backend->DrawCommandBuffer(buffer); }, mBackend);
    }

    const Types::SparseSet<Resources::SubmissionHandle>& RendererCommandBuffer::GetContents() const
    {
        return mBuffer;
    }
}