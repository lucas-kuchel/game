#pragma once

#include <GL/glew.h>

#include <Systems/Renderer.hpp>

namespace Systems
{
    struct OpenGLBufferData
    {
        GLuint ID = 0;
        GLuint Usage = 0;
        GLuint Target = 0;

        Resources::BufferDescriptor Descriptor;
    };

    struct OpenGLPipelineData
    {
        GLuint ID = 0;

        GLenum Primitive = GL_INVALID_ENUM;

        Resources::PipelineDescriptor Descriptor;
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

        WindowInteractionLayer<WindowInteractive::OPENGL_LAYER> Layer;
        std::reference_wrapper<RendererWindow> Window;

        Types::SparseSet<OpenGLBufferData> BufferData;
        Types::SparseSet<OpenGLPipelineData> PipelineData;
        Types::SparseSet<OpenGLSubmissionData> SubmissionData;

        int GetSwapInterval(RendererVSyncMode mode);
        OpenGLAttributeFormat GetGLAttributeFormat(Resources::BufferAttributeType type);
        std::size_t GetTypeSize(Resources::BufferAttributeType type);
    };
}