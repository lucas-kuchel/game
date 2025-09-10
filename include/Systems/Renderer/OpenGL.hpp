#pragma once

#include <GL/glew.h>

#include <Systems/Renderer.hpp>

namespace Systems
{
    struct OpenGLBufferData
    {
        GLuint ID = 0;

        Resources::BufferDescriptor Descriptor;
    };

    struct OpenGLRasterPipelineData
    {
        GLuint ID = 0;

        GLenum Primitive = GL_INVALID_ENUM;

        Resources::RasterPipelineDescriptor Descriptor;
    };

    struct OpenGLSubmissionData
    {
        GLuint ID = 0;

        Resources::SubmissionDescriptor Descriptor;
    };

    struct OpenGLAttributeFormat
    {
        GLenum GLType = GL_INVALID_ENUM;
        GLint Components = 0;
        GLboolean Normalized = GL_FALSE;
    };

    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(const WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>& layer, RendererWindow& window);

        RendererWindow& Window;

        WindowInteractionLayer<WindowInteractive::OPENGL_LAYER> OpenGLWindowLayer;

        Types::SparseSet<OpenGLBufferData> Buffers;
        Types::SparseSet<OpenGLRasterPipelineData> RasterPipelines;
        Types::SparseSet<OpenGLSubmissionData> Submissions;

        int GetSwapInterval(RendererVSyncMode mode);
        OpenGLAttributeFormat GetAttributeFormat(Resources::BufferAttributeType type);
        std::size_t GetTypeSize(Resources::BufferAttributeType type);
    };
}