#include <Systems/Renderer/OpenGL.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DrawCommandBuffer(const CommandBuffer& buffer)
    {
        auto& submissions = buffer.GetContents();

        for (const auto& submission : submissions)
        {
            auto& submissionInfo = mSpecifics->Submissions.Get(submission.ID);
            auto& pipelineInfo = mSpecifics->RasterPipelines.Get(submissionInfo.Descriptor.Pipeline.ID);
            auto& indexBufferInfo = mSpecifics->Buffers.Get(submissionInfo.Descriptor.IndexBuffer.ID);

            auto typeSize = mSpecifics->GetTypeSize(submissionInfo.Descriptor.IndexBufferType);
            auto typeInfo = mSpecifics->GetAttributeFormat(submissionInfo.Descriptor.IndexBufferType);

            auto count = indexBufferInfo.Descriptor.Size / typeSize;

            glUseProgram(pipelineInfo.ID);
            glBindVertexArray(submissionInfo.ID);

            for (std::size_t i = 0; i < submissionInfo.Descriptor.DataBuffers.size(); i++)
            {
                auto& buffer = submissionInfo.Descriptor.DataBuffers[i];
                auto& format = pipelineInfo.Descriptor.DataBufferFormats[i];

                auto& bufferData = mSpecifics->Buffers.Get(buffer.ID);

                GLenum target = GL_INVALID_ENUM;

                switch (format.Type)
                {
                    case Resources::BufferType::CONSTANT:
                    {
                        target = GL_UNIFORM_BUFFER;

                        break;
                    }
                    case Resources::BufferType::STORAGE:
                    {
                        target = GL_SHADER_STORAGE_BUFFER;

                        break;
                    }
                    default:
                    {
                        throw Debug::Exception(Debug::ErrorCode::CONFLICT, "[UNREACHABLE]");
                    }
                }

                glBindBufferBase(target, format.Index, bufferData.ID);
            }

            switch (pipelineInfo.Descriptor.RasterState.FaceCulling)
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

            switch (pipelineInfo.Descriptor.RasterState.FrontFace)
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

            switch (pipelineInfo.Descriptor.RasterState.PolygonMode)
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

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteCommandBuffer(const CommandBuffer&)
    {
    }
}