#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <GL/glew.h>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::AllocateBuffer(Resources::BufferData& data)
    {
        data.BackendMetadata = new OpenGLBufferData();

        auto& info = *static_cast<OpenGLBufferData*>(data.BackendMetadata);

        glCreateBuffers(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create buffer (Size = {})",
                                   data.Size);
        }

        glNamedBufferData(info.ID, data.Size, nullptr, GL_DYNAMIC_DRAW);
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::UploadBufferData(Resources::BufferData& data, const Resources::BufferUploadDescriptor& upload)
    {
        OpenGLBufferData& info = *static_cast<OpenGLBufferData*>(data.BackendMetadata);

        glNamedBufferSubData(info.ID, upload.Offset, upload.Stride, upload.Data);
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeallocateBuffer(Resources::BufferData& data)
    {
        OpenGLBufferData* info = static_cast<OpenGLBufferData*>(data.BackendMetadata);

        glDeleteBuffers(1, &info->ID);

        delete info;

        data.BackendMetadata = nullptr;
    }
}
