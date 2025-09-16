#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <vector>

#include <GL/glew.h>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::CreateSubmission(Resources::SubmissionData& data)
    {
        data.BackendMetadata = new OpenGLSubmissionData();

        auto& info = *static_cast<OpenGLSubmissionData*>(data.BackendMetadata);

        auto& pipelineInfo = mRasterPipelines.Data.Get(data.Pipeline.ID);
        auto& indexBufferInfo = mBuffers.Data.Get(data.IndexBuffer.ID);

        std::size_t indexTypeSize = 0;

        switch (data.IndexFormat)
        {
            case Resources::IndexType::U16:
                indexTypeSize = 2;
                break;
            case Resources::IndexType::U32:
                indexTypeSize = 4;
                break;
        }

        info.IndexCount = indexBufferInfo.Size / indexTypeSize;

        glCreateVertexArrays(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create OpenGL VAO");
        }

        std::uint32_t bufferIndex = 0;

        for (const auto& vertexBuffer : data.VertexBuffers)
        {
            const auto& bufferInfo = mBuffers.Data.Get(vertexBuffer.ID);
            const auto& format = pipelineInfo.VertexBufferFormats[bufferIndex];

            std::size_t stride = 0;

            for (const auto& attribute : format.Attributes)
            {
                stride += GetAttributeFormat(attribute).Size;
            }

            auto& bufferMetadata = *static_cast<OpenGLBufferData*>(bufferInfo.BackendMetadata);

            glVertexArrayVertexBuffer(info.ID, bufferIndex, bufferMetadata.ID, 0, stride);

            std::size_t offset = 0;
            std::size_t index = 0;

            for (const auto& attribute : format.Attributes)
            {
                auto typeInfo = GetAttributeFormat(attribute);

                glEnableVertexArrayAttrib(info.ID, index);

                glVertexArrayAttribFormat(
                    info.ID,
                    index,
                    typeInfo.Components,
                    typeInfo.Type,
                    typeInfo.Normalized ? GL_TRUE : GL_FALSE,
                    offset);

                glVertexArrayAttribBinding(info.ID, index, bufferIndex);

                offset += typeInfo.Size;
                index++;
            }

            bufferIndex++;
        }

        auto& indexBufferMetadata = *static_cast<OpenGLBufferData*>(indexBufferInfo.BackendMetadata);

        glVertexArrayElementBuffer(info.ID, indexBufferMetadata.ID);

        std::vector<GLuint> constantBufferHandles;
        std::vector<GLuint> storageBufferHandles;

        for (auto& buffer : data.VertexShader.ConstantBuffers)
        {
            constantBufferHandles.push_back(static_cast<OpenGLBufferData*>(mBuffers.Data.Get(buffer.ID).BackendMetadata)->ID);
        }

        for (auto& buffer : data.VertexShader.StorageBuffers)
        {
            storageBufferHandles.push_back(static_cast<OpenGLBufferData*>(mBuffers.Data.Get(buffer.ID).BackendMetadata)->ID);
        }

        for (auto& buffer : data.PixelShader.ConstantBuffers)
        {
            constantBufferHandles.push_back(static_cast<OpenGLBufferData*>(mBuffers.Data.Get(buffer.ID).BackendMetadata)->ID);
        }

        for (auto& buffer : data.PixelShader.StorageBuffers)
        {
            storageBufferHandles.push_back(static_cast<OpenGLBufferData*>(mBuffers.Data.Get(buffer.ID).BackendMetadata)->ID);
        }

        info.ConstantBufferHandles.insert(info.ConstantBufferHandles.end(), constantBufferHandles.begin(), constantBufferHandles.end());
        info.StorageBufferHandles.insert(info.StorageBufferHandles.end(), storageBufferHandles.begin(), storageBufferHandles.end());

        DeduplicatePreserveOrder(info.ConstantBufferHandles);
        DeduplicatePreserveOrder(info.StorageBufferHandles);
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeleteSubmission(Resources::SubmissionData& data)
    {
        auto* info = static_cast<OpenGLSubmissionData*>(data.BackendMetadata);

        glDeleteVertexArrays(1, &info->ID);

        delete info;

        data.BackendMetadata = nullptr;
    }
}
