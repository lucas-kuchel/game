#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

#include <spirv_cross/spirv_glsl.hpp>

#include <GL/glew.h>

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
        GLenum FaceCulling = GL_INVALID_ENUM;
        GLenum FrontFace = GL_INVALID_ENUM;
        GLenum PolygonMode = GL_INVALID_ENUM;

        std::vector<GLuint> ConstantBufferBindings;
        std::vector<GLuint> StorageBufferBindings;

        bool CullBackface = false;

        Resources::RasterPipelineDescriptor Descriptor;
    };

    struct OpenGLSubmissionData
    {
        GLuint ID = 0;

        std::vector<GLuint> ConstantBufferHandles;
        std::vector<GLuint> StorageBufferHandles;

        std::size_t IndexCount;

        Resources::SubmissionDescriptor Descriptor;
    };

    struct OpenGLAttributeFormat
    {
        GLenum Type = GL_INVALID_ENUM;
        GLint Components = 0;
        GLboolean Normalized = GL_FALSE;
    };

    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(RendererWindow& window)
            : Window(window), WindowOpenGLLayer(window.CreateInteractionLayer<WindowInteractive::OPENGL_LAYER>())
        {
        }

        RendererWindow& Window;

        WindowInteractionLayer<WindowInteractive::OPENGL_LAYER> WindowOpenGLLayer;

        Types::SparseSet<OpenGLBufferData> Buffers;
        Types::SparseSet<OpenGLRasterPipelineData> RasterPipelines;
        Types::SparseSet<OpenGLSubmissionData> Submissions;

        OpenGLAttributeFormat GetAttributeFormat(Resources::BufferAttributeType type)
        {
            switch (type)
            {
                case Resources::BufferAttributeType::R32_FLOAT:
                {
                    return {GL_FLOAT, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32_FLOAT:
                {
                    return {GL_FLOAT, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32_FLOAT:
                {
                    return {GL_FLOAT, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32A32_FLOAT:
                {
                    return {GL_FLOAT, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8_INT:
                {
                    return {GL_BYTE, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8_INT:
                {
                    return {GL_BYTE, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8B8_INT:
                {
                    return {GL_BYTE, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8B8A8_INT:
                {
                    return {GL_BYTE, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16_INT:
                {
                    return {GL_SHORT, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16_INT:
                {
                    return {GL_SHORT, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16B16_INT:
                {
                    return {GL_SHORT, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16B16A16_INT:
                {
                    return {GL_SHORT, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32_INT:
                {
                    return {GL_INT, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32_INT:
                {
                    return {GL_INT, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32_INT:
                {
                    return {GL_INT, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32A32_INT:
                {
                    return {GL_INT, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8_UINT:
                {
                    return {GL_UNSIGNED_BYTE, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8_UINT:
                {
                    return {GL_UNSIGNED_BYTE, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8B8_UINT:
                {
                    return {GL_UNSIGNED_BYTE, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8G8B8A8_UINT:
                {
                    return {GL_UNSIGNED_BYTE, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16_UINT:
                {
                    return {GL_UNSIGNED_SHORT, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16_UINT:
                {
                    return {GL_UNSIGNED_SHORT, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16B16_UINT:
                {
                    return {GL_UNSIGNED_SHORT, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R16G16B16A16_UINT:
                {
                    return {GL_UNSIGNED_SHORT, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32_UINT:
                {
                    return {GL_UNSIGNED_INT, 1, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32_UINT:
                {
                    return {GL_UNSIGNED_INT, 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32_UINT:
                {
                    return {GL_UNSIGNED_INT, 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::R32G32B32A32_UINT:
                {
                    return {GL_UNSIGNED_INT, 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::R8_SNORM:
                {
                    return {GL_BYTE, 1, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8_SNORM:
                {
                    return {GL_BYTE, 2, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8B8_SNORM:
                {
                    return {GL_BYTE, 3, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8B8A8_SNORM:
                {
                    return {GL_BYTE, 4, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16_SNORM:
                {
                    return {GL_SHORT, 1, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16_SNORM:
                {
                    return {GL_SHORT, 2, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16B16_SNORM:
                {
                    return {GL_SHORT, 3, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16B16A16_SNORM:
                {
                    return {GL_SHORT, 4, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8_UNORM:
                {
                    return {GL_UNSIGNED_BYTE, 1, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8_UNORM:
                {
                    return {GL_UNSIGNED_BYTE, 2, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8B8_UNORM:
                {
                    return {GL_UNSIGNED_BYTE, 3, GL_TRUE};
                }
                case Resources::BufferAttributeType::R8G8B8A8_UNORM:
                {
                    return {GL_UNSIGNED_BYTE, 4, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16_UNORM:
                {
                    return {GL_UNSIGNED_SHORT, 1, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16_UNORM:
                {
                    return {GL_UNSIGNED_SHORT, 2, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16B16_UNORM:
                {
                    return {GL_UNSIGNED_SHORT, 3, GL_TRUE};
                }
                case Resources::BufferAttributeType::R16G16B16A16_UNORM:
                {
                    return {GL_UNSIGNED_SHORT, 4, GL_TRUE};
                }
                case Resources::BufferAttributeType::F32_2x2:
                {
                    return {GL_FLOAT, 2 * 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_2x3:
                {
                    return {GL_FLOAT, 2 * 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_2x4:
                {
                    return {GL_FLOAT, 2 * 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_3x2:
                {
                    return {GL_FLOAT, 3 * 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_3x3:
                {
                    return {GL_FLOAT, 3 * 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_3x4:
                {
                    return {GL_FLOAT, 3 * 4, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_4x2:
                {
                    return {GL_FLOAT, 4 * 2, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_4x3:
                {
                    return {GL_FLOAT, 4 * 3, GL_FALSE};
                }
                case Resources::BufferAttributeType::F32_4x4:
                {
                    return {GL_FLOAT, 4 * 4, GL_FALSE};
                }
            }

            throw Debug::Exception(Debug::ErrorCode::NOT_IMPLEMENTED, "[UNREACHABLE]");
        }

        std::size_t GetTypeSize(Resources::BufferAttributeType type)
        {
            switch (type)
            {
                case Resources::BufferAttributeType::R8_SNORM:
                case Resources::BufferAttributeType::R8_UNORM:
                case Resources::BufferAttributeType::R8_INT:
                case Resources::BufferAttributeType::R8_UINT:
                {
                    return 1;
                }
                case Resources::BufferAttributeType::R16_SNORM:
                case Resources::BufferAttributeType::R16_UNORM:
                case Resources::BufferAttributeType::R16_INT:
                case Resources::BufferAttributeType::R16_UINT:
                case Resources::BufferAttributeType::R8G8_SNORM:
                case Resources::BufferAttributeType::R8G8_UNORM:
                case Resources::BufferAttributeType::R8G8_INT:
                case Resources::BufferAttributeType::R8G8_UINT:
                {
                    return 2;
                }
                case Resources::BufferAttributeType::R8G8B8_SNORM:
                case Resources::BufferAttributeType::R8G8B8_UNORM:
                case Resources::BufferAttributeType::R8G8B8_INT:
                case Resources::BufferAttributeType::R8G8B8_UINT:
                {
                    return 3;
                }
                case Resources::BufferAttributeType::R32_FLOAT:
                case Resources::BufferAttributeType::R32_INT:
                case Resources::BufferAttributeType::R32_UINT:
                case Resources::BufferAttributeType::R16G16_SNORM:
                case Resources::BufferAttributeType::R16G16_UNORM:
                case Resources::BufferAttributeType::R16G16_INT:
                case Resources::BufferAttributeType::R16G16_UINT:
                case Resources::BufferAttributeType::R8G8B8A8_SNORM:
                case Resources::BufferAttributeType::R8G8B8A8_UNORM:
                case Resources::BufferAttributeType::R8G8B8A8_INT:
                case Resources::BufferAttributeType::R8G8B8A8_UINT:
                {
                    return 4;
                }
                case Resources::BufferAttributeType::R16G16B16_SNORM:
                case Resources::BufferAttributeType::R16G16B16_UNORM:
                case Resources::BufferAttributeType::R16G16B16_INT:
                case Resources::BufferAttributeType::R16G16B16_UINT:
                {
                    return 6;
                }
                case Resources::BufferAttributeType::R32G32_INT:
                case Resources::BufferAttributeType::R32G32_UINT:
                case Resources::BufferAttributeType::R32G32_FLOAT:
                case Resources::BufferAttributeType::R16G16B16A16_SNORM:
                case Resources::BufferAttributeType::R16G16B16A16_UNORM:
                case Resources::BufferAttributeType::R16G16B16A16_INT:
                case Resources::BufferAttributeType::R16G16B16A16_UINT:
                {
                    return 8;
                }
                case Resources::BufferAttributeType::R32G32B32_FLOAT:
                case Resources::BufferAttributeType::R32G32B32_INT:
                case Resources::BufferAttributeType::R32G32B32_UINT:
                {
                    return 12;
                }
                case Resources::BufferAttributeType::F32_2x2:
                case Resources::BufferAttributeType::R32G32B32A32_FLOAT:
                case Resources::BufferAttributeType::R32G32B32A32_INT:
                case Resources::BufferAttributeType::R32G32B32A32_UINT:
                {
                    return 16;
                }
                case Resources::BufferAttributeType::F32_2x4:
                case Resources::BufferAttributeType::F32_4x2:
                {
                    return 32;
                }
                case Resources::BufferAttributeType::F32_2x3:
                case Resources::BufferAttributeType::F32_3x2:
                {
                    return 24;
                }
                case Resources::BufferAttributeType::F32_3x4:
                case Resources::BufferAttributeType::F32_4x3:
                {
                    return 48;
                }
                case Resources::BufferAttributeType::F32_3x3:
                {
                    return 36;
                }
                case Resources::BufferAttributeType::F32_4x4:
                {
                    return 64;
                }
            }

            throw Debug::Exception(Debug::ErrorCode::NOT_IMPLEMENTED, "[UNREACHABLE]");
        }

        std::vector<std::string> ConvertShaderBinaries(OpenGLRasterPipelineData& data)
        {
            auto& descriptor = data.Descriptor;

            for (std::size_t i = 0; i < descriptor.Shaders.size(); i++)
            {
                for (std::size_t j = i + 1; j < descriptor.Shaders.size(); j++)
                {
                    auto& one = descriptor.Shaders[i];
                    auto& two = descriptor.Shaders[j];

                    if (one.Path == two.Path)
                    {
                        throw Debug::Exception(Debug::ErrorCode::FILE_NOT_FOUND, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                                 "SPIR-V shader loading error\n"
                                                                                 "SPIR-V does not allow for multiple shader stages in a single binary\n"
                                                                                 "all shader paths must be unique (path = {})",
                                               one.Path);
                    }
                }
            }

            std::vector<std::string> sources;

            sources.reserve(descriptor.Shaders.size());

            for (std::size_t i = 0; i < descriptor.Shaders.size(); i++)
            {
                const auto& shaderDescriptor = descriptor.Shaders[i];

                std::filesystem::path spirvPath = shaderDescriptor.Path;

                std::ifstream spirvFile(spirvPath, std::ios::binary | std::ios::ate);

                if (!spirvFile)
                {
                    throw Debug::Exception(Debug::ErrorCode::FILE_NOT_FOUND, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                             "SPIR-V shader loading error\n"
                                                                             "provided SPIR-V binary file does not exist or permissions do not allow reading (path = {})",
                                           shaderDescriptor.Path);
                }

                std::size_t size = spirvFile.tellg();

                spirvFile.seekg(0);

                if (size % 4 != 0)
                {
                    throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                            "SPIR-V shader loading error\n"
                                                                            "error while reading SPIR-V binary file (path = {})",
                                           shaderDescriptor.Path);
                }

                std::vector<std::uint32_t> spirv(size / sizeof(std::uint32_t));

                if (!spirvFile.read(reinterpret_cast<char*>(spirv.data()), size))
                {
                    throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                            "SPIR-V shader loading error\n"
                                                                            "error while reading SPIR-V binary file (path = {})",
                                           shaderDescriptor.Path);
                }

                if (spirvFile.gcount() != static_cast<std::streamsize>(size))
                {
                    throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                            "SPIR-V shader loading error\n"
                                                                            "error while reading SPIR-V binary file (path = {})",
                                           shaderDescriptor.Path);
                }

                spirv_cross::CompilerGLSL glsl(std::move(spirv));
                spirv_cross::CompilerGLSL::Options options;

                options.version = 460;
                options.es = false;
                options.vulkan_semantics = false;

                glsl.set_common_options(options);

                auto entryPoints = glsl.get_entry_points_and_stages();

                bool found = false;

                for (auto& entryPoint : entryPoints)
                {
                    if (entryPoint.name == shaderDescriptor.Function)
                    {
                        found = true;

                        break;
                    }
                }

                if (!found)
                {
                    throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                       "SPIR-V shader loading error\n"
                                                                       "shader does not contain specified entry point (path = {})",
                                           shaderDescriptor.Path);
                }

                switch (shaderDescriptor.Stage)
                {
                    case Resources::ShaderStage::VERTEX:
                    {
                        glsl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelVertex);

                        break;
                    }
                    case Resources::ShaderStage::PIXEL:
                    {
                        glsl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelFragment);

                        break;
                    }
                    case Resources::ShaderStage::GEOMETRY:
                    {
                        glsl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelGeometry);

                        break;
                    }
                }

                spirv_cross::ShaderResources resources = glsl.get_shader_resources();

                if (resources.uniform_buffers.size() > shaderDescriptor.ConstantBufferFormats.size())
                {
                    throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                       "SPIR-V shader loading error\n"
                                                                       "shader has incorrect constant buffer count for pipeline requirements (path = {})",
                                           shaderDescriptor.Path);
                }

                if (resources.storage_buffers.size() > shaderDescriptor.StorageBufferFormats.size())
                {
                    throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                       "SPIR-V shader loading error\n"
                                                                       "shader has incorrect storage buffer count for pipeline requirements (path = {})",
                                           shaderDescriptor.Path);
                }

                for (auto& constantBuffer : resources.uniform_buffers)
                {
                    GLuint binding = glsl.get_decoration(constantBuffer.id, spv::DecorationBinding);

                    data.ConstantBufferBindings.push_back(binding);
                }

                for (auto& storageBuffer : resources.storage_buffers)
                {
                    GLuint binding = glsl.get_decoration(storageBuffer.id, spv::DecorationBinding);

                    data.StorageBufferBindings.push_back(binding);
                }

                if (shaderDescriptor.Stage == Resources::ShaderStage::VERTEX)
                {
                    std::size_t attributeCount = 0;

                    for (const auto& vertexinput : descriptor.VertexBufferFormats)
                    {
                        attributeCount += vertexinput.Attributes.size();
                    }

                    if (resources.stage_inputs.size() != attributeCount)
                    {
                        throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::RasterPipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                           "SPIR-V vertex shader loading error\n"
                                                                           "shader has incorrect vertex attribute count for pipeline requirements (path = {})",
                                               shaderDescriptor.Path);
                    }

                    for (std::size_t i = 0; i < resources.stage_inputs.size(); i++)
                    {
                        auto& attribute = resources.stage_inputs[i];

                        glsl.set_decoration(attribute.id, spv::DecorationLocation, static_cast<std::uint32_t>(i));
                    }
                }

                std::string source = glsl.compile();

                sources.push_back(source);
            }

            DeduplicatePreserveOrder(data.ConstantBufferBindings);
            DeduplicatePreserveOrder(data.StorageBufferBindings);

            return sources;
        }

        static void DeduplicatePreserveOrder(std::vector<GLuint>& vector)
        {
            std::unordered_set<GLuint> seen;

            auto it = vector.begin();

            for (auto iterator = vector.begin(); iterator != vector.end(); iterator++)
            {
                if (seen.insert(*iterator).second)
                {
                    *it++ = std::move(*iterator);
                }
            }

            vector.erase(it, vector.end());
        }
    };

    RendererBackendImplementation<RendererBackend::OPENGL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
        auto& windowSize = mSpecifics->Window.Get<WindowAttribute::SIZE>();

        GLint swapInterval = 0;

        switch (descriptor.VSyncMode)
        {
            case RendererVSyncMode::DISABLED:
            {
                swapInterval = 0;

                break;
            }
            case RendererVSyncMode::STRICT:
            {
                swapInterval = 1;

                break;
            }
            case RendererVSyncMode::RELAXED:
            {
                swapInterval = -1;

                break;
            }
        }

        mSpecifics->WindowOpenGLLayer.MakeContextCurrent();
        mSpecifics->WindowOpenGLLayer.SetSwapInterval(swapInterval);

        if (glewInit() != GLEW_OK)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(const RendererDescriptor&):\n"
                                                                    "OpenGL extension loading error\n"
                                                                    "failed to load OpenGL extensions");
        }

        glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(descriptor.ClearColour.r, descriptor.ClearColour.g, descriptor.ClearColour.b, descriptor.ClearColour.a);
        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    RendererBackendImplementation<RendererBackend::OPENGL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::BeginFrame()
    {
        auto& windowSize = mSpecifics->Window.Get<WindowAttribute::SIZE>();

        glViewport(0, 0, windowSize.x, windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::EndFrame()
    {
        mSpecifics->WindowOpenGLLayer.SwapBuffers();
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor)
    {
        OpenGLBufferData& info = mSpecifics->Buffers.Insert(handle.ID, OpenGLBufferData());

        info.Descriptor = descriptor;

        glCreateBuffers(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::BufferHandle Systems::Renderer::CreateBuffer(const Resources::BufferDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create buffer (Size = {})",
                                   descriptor.Size);
        }

        glNamedBufferData(info.ID, descriptor.Size, nullptr, GL_DYNAMIC_DRAW);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferDescriptor&, const Resources::BufferData& data)
    {
        OpenGLBufferData& info = mSpecifics->Buffers.Get(handle.ID);

        glNamedBufferSubData(info.ID, data.Offset, data.Stride, data.Data);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor&)
    {
        OpenGLBufferData& info = mSpecifics->Buffers.Get(handle.ID);

        glDeleteBuffers(1, &info.ID);

        mSpecifics->Buffers.Remove(handle.ID);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::CreatePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor& descriptor)
    {
        auto& info = mSpecifics->RasterPipelines.Insert(handle.ID, OpenGLRasterPipelineData());

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

        switch (descriptor.RasterState.FaceCulling)
        {
            case Resources::PipelineFaceCulling::BACKFACE:
            {
                info.CullBackface = true;
                info.FaceCulling = GL_BACK;

                break;
            }
            case Resources::PipelineFaceCulling::FRONTFACE:
            {
                info.CullBackface = true;
                info.FaceCulling = GL_FRONT;

                break;
            }
            case Resources::PipelineFaceCulling::DISABLED:
            {
                info.CullBackface = false;

                break;
            }
        }

        switch (descriptor.RasterState.FrontFace)
        {
            case Resources::PipelineFrontFace::CLOCKWISE:
            {
                info.FrontFace = GL_CW;

                break;
            }
            case Resources::PipelineFrontFace::ANTICLOCKWISE:
            {
                info.FrontFace = GL_CCW;

                break;
            }
        }

        switch (descriptor.RasterState.PolygonMode)
        {
            case Resources::PipelinePolygonMode::SOLID:
            {
                info.PolygonMode = GL_FILL;

                break;
            }
            case Resources::PipelinePolygonMode::LINE:
            {
                info.PolygonMode = GL_LINE;

                break;
            }
            case Resources::PipelinePolygonMode::POINT:
            {
                info.PolygonMode = GL_POINT;

                break;
            }
        }

        info.ID = glCreateProgram();
        info.Descriptor = descriptor;

        auto shaderSources = mSpecifics->ConvertShaderBinaries(info);

        std::vector<GLuint> shaders;

        shaders.reserve(descriptor.Shaders.size());

        for (std::size_t i = 0; i < descriptor.Shaders.size(); i++)
        {
            auto& source = shaderSources[i];
            auto& shaderDescriptor = descriptor.Shaders[i];

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

            GLuint shader = glCreateShader(type);

            shaders.push_back(shader);

            std::int32_t compiled = 0;

            const char* buffer = source.c_str();

            glShaderSource(shader, 1, &buffer, nullptr);
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

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeletePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor&)
    {
        auto& info = mSpecifics->RasterPipelines.Get(handle.ID);

        if (info.ID != 0)
        {
            glDeleteProgram(info.ID);

            info.ID = 0;
        }

        mSpecifics->RasterPipelines.Remove(handle.ID);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor)
    {
        auto& info = mSpecifics->Submissions.Insert(handle.ID, OpenGLSubmissionData());
        auto& pipelineInfo = mSpecifics->RasterPipelines.Get(descriptor.Pipeline.ID);
        auto& indexBufferInfo = mSpecifics->Buffers.Get(descriptor.IndexBuffer.ID);

        auto typeSize = mSpecifics->GetTypeSize(descriptor.IndexBufferType);

        info.Descriptor = descriptor;
        info.IndexCount = indexBufferInfo.Descriptor.Size / typeSize;

        glCreateVertexArrays(1, &info.ID);

        if (info.ID == 0)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR,
                                   "Resources::SubmissionHandle Systems::Renderer::CreateSubmission(const Resources::SubmissionDescriptor&):\n"
                                   "OpenGL resource creation error\n"
                                   "failed to create OpenGL VAO");
        }

        std::uint32_t bufferIndex = 0;

        for (const auto& vertexBuffer : descriptor.VertexBuffers)
        {
            const auto& bufferInfo = mSpecifics->Buffers.Get(vertexBuffer.ID);
            const auto& format = pipelineInfo.Descriptor.VertexBufferFormats[bufferIndex];

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
                auto typeInfo = mSpecifics->GetAttributeFormat(attribute);

                glEnableVertexArrayAttrib(info.ID, index);

                glVertexArrayAttribFormat(
                    info.ID,
                    index,
                    typeInfo.Components,
                    typeInfo.Type,
                    typeInfo.Normalized ? GL_TRUE : GL_FALSE,
                    offset);

                glVertexArrayAttribBinding(info.ID, index, bufferIndex);

                offset += mSpecifics->GetTypeSize(attribute);
                index++;
            }

            bufferIndex++;
        }

        glVertexArrayElementBuffer(info.ID, indexBufferInfo.ID);

        static constexpr Resources::ShaderStage stageOrder[] = {
            Resources::ShaderStage::VERTEX,
            Resources::ShaderStage::GEOMETRY,
            Resources::ShaderStage::PIXEL,
        };

        for (auto stage : stageOrder)
        {
            for (const auto& shader : descriptor.ShaderStages)
            {
                if (shader.Stage != stage)
                {
                    continue;
                }

                std::vector<GLuint> constantBufferHandles;
                std::vector<GLuint> storageBufferHandles;

                for (auto& buffer : shader.ConstantBuffers)
                {
                    constantBufferHandles.push_back(mSpecifics->Buffers.Get(buffer.ID).ID);
                }

                for (auto& buffer : shader.StorageBuffers)
                {
                    storageBufferHandles.push_back(mSpecifics->Buffers.Get(buffer.ID).ID);
                }

                info.ConstantBufferHandles.insert(info.ConstantBufferHandles.end(), constantBufferHandles.begin(), constantBufferHandles.end());
                info.StorageBufferHandles.insert(info.StorageBufferHandles.end(), storageBufferHandles.begin(), storageBufferHandles.end());
            }
        }

        mSpecifics->DeduplicatePreserveOrder(info.ConstantBufferHandles);
        mSpecifics->DeduplicatePreserveOrder(info.StorageBufferHandles);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor&)
    {
        auto& info = mSpecifics->Submissions.Get(handle.ID);

        if (info.ID != 0)
        {
            glDeleteVertexArrays(1, &info.ID);

            info.ID = 0;
        }

        mSpecifics->Submissions.Remove(handle.ID);
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::CreateCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DrawCommandBuffer(const CommandBuffer& buffer)
    {
        auto& submissions = buffer.GetContents();

        for (const auto& submission : submissions)
        {
            auto& submissionInfo = mSpecifics->Submissions.Get(submission.ID);
            auto& pipelineInfo = mSpecifics->RasterPipelines.Get(submissionInfo.Descriptor.Pipeline.ID);

            auto typeInfo = mSpecifics->GetAttributeFormat(submissionInfo.Descriptor.IndexBufferType);

            glUseProgram(pipelineInfo.ID);
            glBindVertexArray(submissionInfo.ID);

            for (size_t i = 0; i < pipelineInfo.ConstantBufferBindings.size(); i++)
            {
                auto& bufferID = submissionInfo.ConstantBufferHandles[i];

                glBindBufferBase(GL_UNIFORM_BUFFER, pipelineInfo.ConstantBufferBindings[i], bufferID);
            }

            for (size_t i = 0; i < pipelineInfo.StorageBufferBindings.size(); i++)
            {
                auto& bufferID = submissionInfo.StorageBufferHandles[i];

                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, pipelineInfo.StorageBufferBindings[i], bufferID);
            }

            if (pipelineInfo.CullBackface)
            {
                glEnable(GL_CULL_FACE);

                glCullFace(pipelineInfo.FaceCulling);
                glFrontFace(pipelineInfo.FrontFace);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }

            glPolygonMode(GL_FRONT_AND_BACK, pipelineInfo.PolygonMode);
            glDrawElements(pipelineInfo.Primitive, submissionInfo.IndexCount, typeInfo.Type, nullptr);
        }
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::DeleteCommandBuffer(const CommandBuffer&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OPENGL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type& vsyncMode)
    {
        GLint swapInterval = 0;

        switch (vsyncMode)
        {
            case RendererVSyncMode::DISABLED:
            {
                swapInterval = 0;

                break;
            }
            case RendererVSyncMode::STRICT:
            {
                swapInterval = 1;

                break;
            }
            case RendererVSyncMode::RELAXED:
            {
                swapInterval = -1;

                break;
            }
        }

        mSpecifics->WindowOpenGLLayer.SetSwapInterval(swapInterval);
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OPENGL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& clearColour)
    {
        glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::OPENGL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window);
    }
}
