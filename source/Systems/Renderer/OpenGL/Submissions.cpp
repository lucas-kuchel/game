#include <Systems/Renderer/OpenGL.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor)
    {
        auto& info = mSpecifics->SubmissionData.Insert(handle.ID, OpenGLSubmissionData());

        glCreateVertexArrays(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create OpenGL VAO");
        }

        info.Descriptor = descriptor;

        const auto& pipeline = mSpecifics->RasterPipelineData.Get(descriptor.Pipeline.ID);

        std::uint32_t bufferIndex = 0;

        for (const auto& vertexBuffer : descriptor.VertexBuffers)
        {
            const auto& bufferInfo = mSpecifics->BufferData.Get(vertexBuffer.ID);
            const auto& format = pipeline.Descriptor.VertexBufferFormats[bufferIndex];

            std::size_t stride = 0;

            for (const auto& attribute : format.Attributes)
            {
                stride += mSpecifics->GetTypeSize(attribute);
            }

            glVertexArrayVertexBuffer(info.ID, bufferIndex, bufferInfo.ID, 0, stride);

            std::size_t offset = 0;
            std::size_t index = 0;

            for (const auto& attribute : format.Attributes)
            {
                auto typeInfo = mSpecifics->GetGLAttributeFormat(attribute);

                glEnableVertexArrayAttrib(info.ID, index);

                glVertexArrayAttribFormat(
                    info.ID,
                    index,
                    typeInfo.Components,
                    typeInfo.GLType,
                    typeInfo.Normalized ? GL_TRUE : GL_FALSE,
                    offset);

                glVertexArrayAttribBinding(info.ID, index, bufferIndex);

                offset += mSpecifics->GetTypeSize(attribute);
                index++;
            }

            bufferIndex++;
        }

        const auto& indexBufferInfo = mSpecifics->BufferData.Get(descriptor.IndexBuffer.ID);

        glVertexArrayElementBuffer(info.ID, indexBufferInfo.ID);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteSubmission(const Resources::SubmissionHandle& handle)
    {
        auto& info = mSpecifics->SubmissionData.Get(handle.ID);

        if (info.ID != 0)
        {
            glDeleteVertexArrays(1, &info.ID);

            info.ID = 0;
        }

        mSpecifics->SubmissionData.Remove(handle.ID);
    }
}