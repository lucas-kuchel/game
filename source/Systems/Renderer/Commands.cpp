#include <Systems/Renderer.hpp>

namespace Systems
{
    CommandBuffer::CommandBuffer(RendererBackendVariant& backend, Renderer& renderer, std::size_t id)
        : mBackend(backend), mRenderer(renderer), mID(id)
    {
        std::visit([this](auto& backend)
                   { backend->CreateCommandBuffer(*this); }, mBackend.get());
    }

    CommandBuffer::~CommandBuffer()
    {
        std::visit([this](auto& backend)
                   { backend->DeleteCommandBuffer(*this); }, mBackend.get());

        auto& renderer = mRenderer.get();

        renderer.mCommandBufferFreeList.push_back(mID);
        renderer.mCommandBufferCount--;
    }

    void CommandBuffer::Submit(const Resources::SubmissionHandle& submission)
    {
        if (mBuffer.Contains(submission.ID))
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::CommandBuffer::Submit(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "provided submission has already been submitted into this command buffer");
        }

        mBuffer.Insert(submission.ID, submission);

        std::visit([&, this](auto& backend)
                   { backend->SubmitToCommandBuffer(*this, submission); }, mBackend.get());
    }

    void CommandBuffer::Flush()
    {
        mBuffer.Clear();
        mBuffer.ShrinkToFit();
    }

    CommandBuffer Renderer::CreateCommandBuffer()
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

        return CommandBuffer(mBackend, *this, id);
    }

    void Renderer::DrawCommandBuffer(const CommandBuffer& buffer)
    {
        if (buffer.mBuffer.Empty())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DrawCommandBuffer(const CommandBuffer&):\n"
                                                                       "invalid argument\n"
                                                                       "provided command buffer is empty");
        }

        std::visit([&](auto& backend)
                   { backend->DrawCommandBuffer(buffer); }, mBackend);
    }

    const Types::SparseSet<Resources::SubmissionHandle>& CommandBuffer::GetContents() const
    {
        return mBuffer;
    }
}
