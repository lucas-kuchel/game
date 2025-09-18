#include <Systems/Renderer.hpp>

#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/OpenGL.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Resources::RenderQueueHandle Renderer::CreateRenderQueue()
    {
        Resources::RenderQueueHandle handle;

        if (mRenderQueues.FreeList.empty())
        {
            handle.ID = mRenderQueues.Generations.size();
            mRenderQueues.Generations.push_back(0);
        }
        else
        {
            handle.ID = mRenderQueues.FreeList.back();
            handle.Generation = mRenderQueues.Generations[handle.ID];
            mRenderQueues.FreeList.pop_back();
        }

        auto& info = mRenderQueues.Data.Insert(handle.ID, Resources::RenderQueueData{.Submissions = {}, .BackendMetadata = nullptr});

        std::visit([&](auto& backend)
                   { backend->CreateRenderQueue(info); }, mBackend);

        return handle;
    }

    Resources::RenderQueueData Renderer::GetRenderQueueData(const Resources::RenderQueueHandle& handle)
    {
        if (!mRenderQueues.Data.Contains(handle.ID) || mRenderQueues.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "Resources::RenderQueueData Systems::Renderer::GetRenderQueueData(const Resources::RenderQueueHandle&):\n"
                                   "Invalid argument\n"
                                   "provided render queue does not exist");
        }

        return mRenderQueues.Data.Get(handle.ID);
    }

    void Renderer::SubmitSubmission(const Resources::RenderQueueHandle& handle, const Resources::SubmissionHandle& submission)
    {
        if (!mRenderQueues.Data.Contains(handle.ID) || mRenderQueues.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::AppendToQueue(const Resources::RenderQueueHandle&, const Resources::SubmissionHandle&):\n"
                                   "Invalid argument\n"
                                   "provided render queue does not exist");
        }

        if (!mSubmissions.Data.Contains(submission.ID) || mSubmissions.Generations[submission.ID] != submission.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::AppendToQueue(const Resources::RenderQueueHandle&, const Resources::SubmissionHandle&):\n"
                                   "Invalid argument\n"
                                   "provided submission does not exist");
        }

        auto& data = mRenderQueues.Data.Get(handle.ID);
        auto& submissionData = mSubmissions.Data.Get(submission.ID);

        data.Submissions.push_back(submission);

        std::visit([&](auto& backend)
                   { backend->SubmitSubmission(data, submissionData); }, mBackend);
    }

    void Renderer::DeleteQueue(const Resources::RenderQueueHandle& handle)
    {
        if (!mRenderQueues.Data.Contains(handle.ID) || mRenderQueues.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT,
                                   "void Systems::Renderer::DeleteQueue(const Resources::RenderQueueHandle&):\n"
                                   "Invalid argument\n"
                                   "provided render queue does not exist");
        }

        auto& data = mRenderQueues.Data.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeleteQueue(data); }, mBackend);

        mRenderQueues.Generations[handle.ID]++;
        mRenderQueues.FreeList.push_back(handle.ID);
        mRenderQueues.Data.Remove(handle.ID);
    }
}