#include <Systems/Renderer/OpenGL.hpp>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor)
    {
        OpenGLBufferData& info = mSpecifics->BufferData.Insert(handle.ID, OpenGLBufferData());

        glCreateBuffers(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create OpenGL buffer");
        }

        info.Descriptor = descriptor;
        info.Usage = GL_DYNAMIC_DRAW;

        switch (descriptor.Type)
        {
            case Resources::BufferType::VERTEX:
            {
                info.Target = GL_ARRAY_BUFFER;

                break;
            }
            case Resources::BufferType::INDEX:
            {
                info.Target = GL_ELEMENT_ARRAY_BUFFER;

                break;
            }
            case Resources::BufferType::CONSTANT:
            {
                info.Target = GL_UNIFORM_BUFFER;

                break;
            }
            case Resources::BufferType::STORAGE:
            {
                info.Target = GL_SHADER_STORAGE_BUFFER;

                break;
            }
        }

        glNamedBufferData(info.ID, descriptor.Size, nullptr, info.Usage);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data)
    {
        OpenGLBufferData& info = mSpecifics->BufferData.Get(handle.ID);

        glNamedBufferSubData(info.ID, data.Offset, data.Size, data.Data);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteBuffer(const Resources::BufferHandle& handle)
    {
        OpenGLBufferData& info = mSpecifics->BufferData.Get(handle.ID);

        glDeleteBuffers(1, &info.ID);

        mSpecifics->BufferData.Remove(handle.ID);
    }
}