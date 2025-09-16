#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <vector>

#include <GL/glew.h>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::CreatePipeline(Resources::RasterPipelineData& data)
    {
        data.BackendMetadata = new OpenGLRasterPipelineData();

        auto& info = *static_cast<OpenGLRasterPipelineData*>(data.BackendMetadata);

        switch (data.Topology)
        {
            case Resources::PolygonTopology::TriangleList:
                info.Primitive = GL_TRIANGLES;
                break;
            case Resources::PolygonTopology::LineList:
                info.Primitive = GL_LINES;
                break;
            case Resources::PolygonTopology::Point:
                info.Primitive = GL_POINT;
                break;
        }

        switch (data.CullFace)
        {
            case Resources::PolygonCullFace::Back:
                info.CullBackface = true;
                info.FaceCulling = GL_BACK;
                break;
            case Resources::PolygonCullFace::Front:
                info.CullBackface = true;
                info.FaceCulling = GL_FRONT;
                break;
            case Resources::PolygonCullFace::None:
                info.CullBackface = false;
                break;
        }

        switch (data.FrontFace)
        {
            case Resources::PolygonFrontFace::Clockwise:
                info.FrontFace = GL_CW;
                break;
            case Resources::PolygonFrontFace::Anticlockwise:
                info.FrontFace = GL_CCW;
                break;
        }

        switch (data.FillMode)
        {
            case Resources::PolygonFillMode::Solid:
                info.PolygonMode = GL_FILL;
                break;
            case Resources::PolygonFillMode::Line:
                info.PolygonMode = GL_LINE;
                break;
            case Resources::PolygonFillMode::Point:
                info.PolygonMode = GL_POINT;
                break;
        }

        switch (data.DepthComparison)
        {
            case Resources::ComparisonOperation::Always:
                info.DepthOperation = GL_ALWAYS;
                break;
            case Resources::ComparisonOperation::Equal:
                info.DepthOperation = GL_EQUAL;
                break;
            case Resources::ComparisonOperation::Greater:
                info.DepthOperation = GL_GREATER;
                break;
            case Resources::ComparisonOperation::GreaterEqual:
                info.DepthOperation = GL_GEQUAL;
                break;
            case Resources::ComparisonOperation::LessEqual:
                info.DepthOperation = GL_LEQUAL;
                break;
            case Resources::ComparisonOperation::Less:
                info.DepthOperation = GL_LESS;
                break;
            case Resources::ComparisonOperation::NotEqual:
                info.DepthOperation = GL_NOTEQUAL;
                break;
            case Resources::ComparisonOperation::Never:
                info.DepthOperation = GL_NEVER;
                break;
        }

        if (data.DepthWrite)
        {
            info.DepthWrite = GL_TRUE;
        }
        else
        {
            info.DepthWrite = GL_FALSE;
        }

        info.ID = glCreateProgram();

        auto compileShader = [&](GLuint& handle, const std::string& source, std::size_t stageIndex)
        {
            GLenum type = GL_INVALID_ENUM;

            switch (stageIndex)
            {
                case 0:
                {
                    type = GL_VERTEX_SHADER;

                    break;
                }
                case 1:
                {
                    type = GL_FRAGMENT_SHADER;

                    break;
                }
            }

            handle = glCreateShader(type);

            std::int32_t compiled = 0;

            const char* buffer = source.c_str();

            glShaderSource(handle, 1, &buffer, nullptr);
            glCompileShader(handle);
            glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);

            if (compiled == GL_FALSE)
            {
                GLint logLength = 0;
                std::vector<char> log;

                glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

                if (logLength > 0)
                {
                    log.resize(logLength);

                    glGetShaderInfoLog(handle, logLength, nullptr, log.data());
                }

                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                       "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const RasterPipelineDescriptor&):\n"
                                       "OpenGL shader compilation error\n"
                                       "failed to compile shader\n"
                                       "OpenGL compilation error info log:\n\n{}",
                                       log.data());
            }

            glAttachShader(info.ID, handle);
        };

        std::string vertexSource = ConvertShaderBinary(data, data.VertexShader, 0);
        std::string pixelSource = ConvertShaderBinary(data, data.PixelShader, 1);

        GLuint vertexShader = 0;
        GLuint pixelShader = 0;

        compileShader(vertexShader, vertexSource, 0);
        compileShader(pixelShader, pixelSource, 1);

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

        glDeleteShader(vertexShader);
        glDeleteShader(pixelShader);
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeletePipeline(Resources::RasterPipelineData& data)
    {
        auto* info = static_cast<OpenGLRasterPipelineData*>(data.BackendMetadata);

        glDeleteProgram(info->ID);

        delete info;

        data.BackendMetadata = nullptr;
    }
}
