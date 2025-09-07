#include <Systems/Renderer/OpenGL.hpp>

#include <fstream>
#include <string>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OPENGL>::CreatePipeline(const Resources::PipelineHandle& handle, const Resources::PipelineDescriptor& descriptor)
    {
        auto& info = mSpecifics->PipelineData.Insert(handle.ID, OpenGLPipelineData());

        info.ID = glCreateProgram();
        info.Descriptor = descriptor;

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const PipelineDescriptor&):\n"
                                   "general OpenGL error\n"
                                   "failed to create shader program");
        }

        switch (descriptor.Primitive)
        {
            case Resources::PipelinePrimitive::TRIANGLE_LIST:
            {
                info.Primitive = GL_TRIANGLES;

                break;
            }
            case Resources::PipelinePrimitive::LINE_LIST:
            {
                info.Primitive = GL_LINES;

                break;
            }
            case Resources::PipelinePrimitive::POINT:
            {
                info.Primitive = GL_POINT;

                break;
            }
        }

        std::vector<GLuint> shaders;

        for (const auto& shaderDescriptor : descriptor.Shaders)
        {
            GLenum type;

            switch (shaderDescriptor.Type)
            {
                case Resources::ShaderType::VERTEX:
                {
                    type = GL_VERTEX_SHADER;

                    break;
                }
                case Resources::ShaderType::PIXEL:
                {
                    type = GL_FRAGMENT_SHADER;

                    break;
                }
                case Resources::ShaderType::GEOMETRY:
                {
                    type = GL_GEOMETRY_SHADER;

                    break;
                }
            }

            std::uint32_t shader = glCreateShader(type);
            std::ifstream file(shaderDescriptor.Path);
            std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::int32_t compiled = 0;

            shaders.push_back(shader);

            const char* source = buffer.c_str();

            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

            if (compiled == GL_FALSE)
            {
                GLint logLength = 0;
                std::vector<char> log;

                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

                if (logLength > 0)
                {
                    log.resize(logLength);

                    glGetShaderInfoLog(shader, logLength, nullptr, log.data());
                }

                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                       "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const PipelineDescriptor&):\n"
                                       "OpenGL shader compilation error\n"
                                       "failed to compile OpenGL shader\n"
                                       "OpenGL compilation error info log:\n\n{}",
                                       log.data());
            }

            glAttachShader(info.ID, shader);
        }

        glLinkProgram(info.ID);

        GLint linked = 0;

        glGetProgramiv(info.ID, GL_LINK_STATUS, &linked);

        if (linked == GL_FALSE)
        {
            GLint logLength = 0;

            glGetProgramiv(info.ID, GL_INFO_LOG_LENGTH, &logLength);

            std::vector<GLchar> log(logLength);

            if (logLength > 0)
            {
                glGetProgramInfoLog(info.ID, logLength, nullptr, log.data());
            }

            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const PipelineDescriptor&):\n"
                                   "OpenGL pipeline linking error\n"
                                   "failed to link OpenGL shader program\n"
                                   "OpenGL link error info log:\n\n{}",
                                   log.data());
        }

        for (auto shader : shaders)
        {
            glDeleteShader(shader);
        }
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeletePipeline(const Resources::PipelineHandle& handle)
    {
        auto& info = mSpecifics->PipelineData.Get(handle.ID);

        if (info.ID != 0)
        {
            glDeleteProgram(info.ID);

            info.ID = 0;
        }

        mSpecifics->PipelineData.Remove(handle.ID);
    }
}