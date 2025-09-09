#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::SubmissionHandle Renderer::CreateSubmission(const Resources::SubmissionDescriptor& descriptor)
    {
        if (!mRasterPipelineData.Contains(descriptor.Pipeline.ID) || mPipelineGenerations[descriptor.Pipeline.ID] != descriptor.Pipeline.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        auto& pipelineData = mRasterPipelineData.Get(descriptor.Pipeline.ID);
        auto& vertexAttributes = pipelineData.VertexBufferFormats;
        auto& dataAttributes = pipelineData.DataBufferFormats;

        if (vertexAttributes.size() != descriptor.VertexBuffers.size())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "mismatch between vertex buffer formats and vertex buffer handles");
        }

        if (dataAttributes.size() != descriptor.DataBuffers.size())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "mismatch between data buffer formats and data buffer handles");
        }

        Resources::SubmissionHandle handle;

        if (mSubmissionFreeList.empty())
        {
            handle.ID = mSubmissionGenerations.size();
            mSubmissionGenerations.push_back(0);
            mSubmissionData.Insert(handle.ID, descriptor);
        }
        else
        {
            handle.ID = mSubmissionFreeList.back();
            handle.Generation = mSubmissionGenerations[handle.ID];
            mSubmissionFreeList.pop_back();
            mSubmissionData.Insert(handle.ID, descriptor);
        }

        std::visit([&](auto& backend)
                   { backend->CreateSubmission(handle, descriptor); }, mBackend);

        return handle;
    }

    Resources::SubmissionDescriptor Renderer::GetSubmissionInfo(const Resources::SubmissionHandle& handle)
    {
        if (!mSubmissionData.Contains(handle.ID) || mSubmissionGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionDescriptor Systems::Renderer::GetSubmissionInfo(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "submission does not exist");
        }

        return mSubmissionData.Get(handle.ID);
    }

    void Renderer::DeleteSubmission(const Resources::SubmissionHandle& handle)
    {
        if (!mSubmissionData.Contains(handle.ID) || mSubmissionGenerations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DeleteSubmission(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "submission does not exist");
        }

        std::visit([&](auto& backend)
                   { backend->DeleteSubmission(handle); }, mBackend);

        mSubmissionGenerations[handle.ID]++;
        mSubmissionFreeList.push_back(handle.ID);
        mSubmissionData.Remove(handle.ID);
    }
}