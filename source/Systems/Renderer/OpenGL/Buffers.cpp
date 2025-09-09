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
                                   "failed to create buffer (Size = {})",
                                   descriptor.Size);
        }

        info.Descriptor = descriptor;

        glNamedBufferData(info.ID, descriptor.Size, nullptr, GL_DYNAMIC_DRAW);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data)
    {
        OpenGLBufferData& info = mSpecifics->BufferData.Get(handle.ID);

        glNamedBufferSubData(info.ID, data.Offset, data.Stride, data.Data);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteBuffer(const Resources::BufferHandle& handle)
    {
        OpenGLBufferData& info = mSpecifics->BufferData.Get(handle.ID);

        glDeleteBuffers(1, &info.ID);

        mSpecifics->BufferData.Remove(handle.ID);
    }
}