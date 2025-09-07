#include <Systems/Renderer/OpenGL.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateCommandBuffer(const RendererCommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SubmitToCommandBuffer(const RendererCommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DrawCommandBuffer(const RendererCommandBuffer& buffer)
    {
        auto& submissions = buffer.GetContents();

        for (const auto& submission : submissions)
        {
            auto& submissionInfo = mSpecifics->SubmissionData.Get(submission.ID);
            auto& pipelineInfo = mSpecifics->PipelineData.Get(submissionInfo.Descriptor.Pipeline.ID);
            auto& bufferInfo = mSpecifics->BufferData.Get(submissionInfo.Descriptor.IndexBuffer.ID);

            auto typeSize = mSpecifics->GetTypeSize(bufferInfo.Descriptor.Attributes[0].Type);
            auto typeInfo = mSpecifics->GetGLAttributeFormat(bufferInfo.Descriptor.Attributes[0].Type);
            auto count = bufferInfo.Descriptor.Size / typeSize;

            glUseProgram(pipelineInfo.ID);
            glBindVertexArray(submissionInfo.ID);

            for (const auto& buffer : pipelineInfo.Descriptor.InputBuffers)
            {
                auto& bufferData = mSpecifics->BufferData.Get(buffer.ID);

                glBindBufferBase(bufferData.Target, bufferData.Descriptor.Slot, bufferData.ID);
            }

            glDrawElements(pipelineInfo.Primitive, count, typeInfo.GLType, nullptr);
        }
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteCommandBuffer(const RendererCommandBuffer&)
    {
    }
}