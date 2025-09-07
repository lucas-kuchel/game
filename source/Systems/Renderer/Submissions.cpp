#include <Systems/Renderer.hpp>

namespace Systems
{
    Resources::SubmissionHandle Renderer::CreateSubmission(const Resources::SubmissionDescriptor& descriptor)
    {
        std::vector<Resources::BufferAttributeDescriptor> meshAttributes;

        for (const auto& buffer : descriptor.VertexBuffers)
        {
            if (!mBufferData.Contains(buffer.ID) || mBufferGenerations[buffer.ID] != buffer.Generation)
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "provided vertex buffer does not exist");
            }

            auto& bufferData = mBufferData.Get(buffer.ID);
            auto& attributes = bufferData.Attributes;

            meshAttributes.insert(meshAttributes.end(), attributes.begin(), attributes.end());
        }

        if (!mPipelineData.Contains(descriptor.Pipeline.ID) || mPipelineGenerations[descriptor.Pipeline.ID] != descriptor.Pipeline.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        auto& pipelineData = mPipelineData.Get(descriptor.Pipeline.ID);
        auto& pipelineAttributes = pipelineData.VertexInput;

        std::sort(pipelineAttributes.begin(), pipelineAttributes.end());
        std::sort(meshAttributes.begin(), meshAttributes.end());

        if (pipelineAttributes.size() != meshAttributes.size() || pipelineAttributes != meshAttributes)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "mismatch between pipeline attribute inputs and provided vertex attributes");
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