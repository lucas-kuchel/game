#include <Systems/Renderer/OpenGL.hpp>

#include <fstream>
#include <string>

namespace Systems
{
    std::string GetGLSLShaderPath(const std::string_view& hlslPath, Resources::ShaderStage stage)
    {
        namespace fs = std::filesystem;

        fs::path baseOutDir = fs::path(hlslPath).replace_extension();

        std::string stageStr;
        switch (stage)
        {
            case Resources::ShaderStage::VERTEX:
                stageStr = "vertex";
                break;
            case Resources::ShaderStage::PIXEL:
                stageStr = "pixel";
                break;
            case Resources::ShaderStage::GEOMETRY:
                stageStr = "geometry";
                break;
            default:
                stageStr = "unknown";
                break;
        }

        fs::path glslDir = baseOutDir / "glsl";

        std::string basename = fs::path(hlslPath).stem().string();
        fs::path glslFile = glslDir / (basename + "." + stageStr + ".glsl");

        return glslFile.string();
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::CreatePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor& descriptor)
    {
        auto& info = mSpecifics->RasterPipelines.Insert(handle.ID, OpenGLRasterPipelineData());

        info.ID = glCreateProgram();
        info.Descriptor = descriptor;

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
                                   "general OpenGL error\n"
                                   "failed to create shader program");
        }

        switch (descriptor.RasterState.Primitive)
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
            GLenum type = GL_INVALID_ENUM;

            switch (shaderDescriptor.Stage)
            {
                case Resources::ShaderStage::VERTEX:
                {
                    type = GL_VERTEX_SHADER;

                    break;
                }
                case Resources::ShaderStage::PIXEL:
                {
                    type = GL_FRAGMENT_SHADER;

                    break;
                }
                case Resources::ShaderStage::GEOMETRY:
                {
                    type = GL_GEOMETRY_SHADER;

                    break;
                }
            }

            std::string shaderPath = GetGLSLShaderPath(shaderDescriptor.Path, shaderDescriptor.Stage);

            std::uint32_t shader = glCreateShader(type);
            std::ifstream file(shaderPath, std::ios::ate);

            if (!file)
            {
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                       "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
                                       "OpenGL shader loading error\n"
                                       "failed to load GLSL shader from file (path = {})\n"
                                       "file doesn't exist or permissions do not allow for reading",
                                       shaderPath);
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::string buffer(size, '\0');

            if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
            {
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                       "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
                                       "OpenGL shader loading error\n"
                                       "failed to load GLSL shader from file (path = {})",
                                       "permissions do not allow for reading or external process terminated stream",
                                       shaderPath);
            }

            shaders.push_back(shader);

            std::int32_t compiled = 0;

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
                                       "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
                                       "OpenGL shader compilation error\n"
                                       "failed to compile shader\n"
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
                                   "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
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
        auto& info = mSpecifics->RasterPipelines.Get(handle.ID);

        if (info.ID != 0)
        {
            glDeleteProgram(info.ID);

            info.ID = 0;
        }

        mSpecifics->RasterPipelines.Remove(handle.ID);
    }
}