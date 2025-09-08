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

            switch (pipelineInfo.Descriptor.FaceCulling)
            {
                case Resources::PipelineFaceCulling::BACKFACE:
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);

                    break;
                }
                case Resources::PipelineFaceCulling::FRONTFACE:
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT);

                    break;
                }
                case Resources::PipelineFaceCulling::DISABLED:
                {
                    glDisable(GL_CULL_FACE);

                    break;
                }
            }

            switch (pipelineInfo.Descriptor.FrontFace)
            {
                case Resources::PipelineFrontFace::CLOCKWISE:
                {
                    glFrontFace(GL_CW);

                    break;
                }
                case Resources::PipelineFrontFace::ANTICLOCKWISE:
                {
                    glFrontFace(GL_CCW);

                    break;
                }
            }

            switch (pipelineInfo.Descriptor.PolygonMode)
            {
                case Resources::PipelinePolygonMode::SOLID:
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    break;
                }
                case Resources::PipelinePolygonMode::LINE:
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

                    break;
                }
                case Resources::PipelinePolygonMode::POINT:
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

                    break;
                }
            }

            glDrawElements(pipelineInfo.Primitive, count, typeInfo.GLType, nullptr);
        }
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteCommandBuffer(const RendererCommandBuffer&)
    {
    }
}