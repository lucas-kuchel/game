#include <Systems/Renderer.hpp>

#include <Systems/Renderer/Metal.hpp>
#include <Systems/Renderer/OpenGL.hpp>
#include <Systems/Renderer/Vulkan.hpp>

namespace Systems
{
    Resources::SubmissionHandle Renderer::CreateSubmission(Resources::SubmissionDescriptor& descriptor)
    {
        if (!mRasterPipelines.Data.Contains(descriptor.Pipeline.ID) || mRasterPipelines.Generations[descriptor.Pipeline.ID] != descriptor.Pipeline.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "provided pipeline does not exist");
        }

        auto& pipelineData = mRasterPipelines.Data.Get(descriptor.Pipeline.ID);
        auto& vertexAttributes = pipelineData.VertexBufferFormats;

        if (vertexAttributes.size() != descriptor.VertexBuffers.size())
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(Resources::SubmissionDescriptor&):\n"
                                                                       "invalid argument\n"
                                                                       "mismatch between vertex buffer formats and vertex buffer handles");
        }

        auto validateShader = [&](Resources::ShaderStageSubmissionDescriptor& submission, Resources::ShaderDescriptor& shader)
        {
            if (shader.ConstantBufferFormats.size() != submission.ConstantBuffers.size())
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(Resources::SubmissionDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "mismatch between storage buffer formats and storage buffer handles");
            }

            if (shader.StorageBufferFormats.size() != submission.StorageBuffers.size())
            {
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(Resources::SubmissionDescriptor&):\n"
                                                                           "invalid argument\n"
                                                                           "mismatch between storage buffer formats and storage buffer handles");
            }
        };

        validateShader(descriptor.VertexShader, pipelineData.VertexShader);
        validateShader(descriptor.PixelShader, pipelineData.PixelShader);

        Resources::SubmissionHandle handle;

        if (mSubmissions.FreeList.empty())
        {
            handle.ID = mSubmissions.Generations.size();
            mSubmissions.Generations.push_back(0);
        }
        else
        {
            handle.ID = mSubmissions.FreeList.back();
            handle.Generation = mSubmissions.Generations[handle.ID];
            mSubmissions.FreeList.pop_back();
        }

        auto& info = mSubmissions.Data.Insert(handle.ID, Resources::SubmissionData());

        info.BackendMetadata = nullptr;
        info.IndexBuffer = descriptor.IndexBuffer;
        info.IndexFormat = descriptor.IndexFormat;
        info.VertexBuffers = descriptor.VertexBuffers;
        info.VertexShader = descriptor.VertexShader;
        info.PixelShader = descriptor.PixelShader;
        info.Pipeline = descriptor.Pipeline;

        std::visit([&](auto& backend)
                   { backend->CreateSubmission(info); }, mBackend);

        return handle;
    }

    Resources::SubmissionData Renderer::GetSubmissionData(const Resources::SubmissionHandle& handle)
    {
        if (!mSubmissions.Data.Contains(handle.ID) || mSubmissions.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Resources::SubmissionDescriptor Systems::Renderer::GetSubmissionInfo(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "submission does not exist");
        }

        return mSubmissions.Data.Get(handle.ID);
    }

    void Renderer::DeleteSubmission(const Resources::SubmissionHandle& handle)
    {
        if (!mSubmissions.Data.Contains(handle.ID) || mSubmissions.Generations[handle.ID] != handle.Generation)
        {
            throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "void Systems::Renderer::DeleteSubmission(const Resources::SubmissionHandle&):\n"
                                                                       "invalid argument\n"
                                                                       "submission does not exist");
        }

        auto& info = mSubmissions.Data.Get(handle.ID);

        std::visit([&](auto& backend)
                   { backend->DeleteSubmission(info); }, mBackend);

        mSubmissions.Generations[handle.ID]++;
        mSubmissions.FreeList.push_back(handle.ID);
        mSubmissions.Data.Remove(handle.ID);
    }
}