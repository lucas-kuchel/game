#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <fstream>
#include <vector>

#include <spirv_cross/spirv_glsl.hpp>

#include <GL/glew.h>

namespace Systems
{
    OpenGLAttributeFormat RendererBackendImplementation<RendererBackend::OpenGL>::GetAttributeFormat(Resources::BufferAttribute attribute)
    {
        switch (attribute)
        {
            case Resources::BufferAttribute::F32_R:
                return {GL_FLOAT, 1, 4, GL_FALSE};
            case Resources::BufferAttribute::F32_RG:
                return {GL_FLOAT, 2, 8, GL_FALSE};
            case Resources::BufferAttribute::F32_RGB:
                return {GL_FLOAT, 3, 12, GL_FALSE};
            case Resources::BufferAttribute::F32_RGBA:
                return {GL_FLOAT, 4, 16, GL_FALSE};

            case Resources::BufferAttribute::I8_R:
                return {GL_BYTE, 1, 1, GL_FALSE};
            case Resources::BufferAttribute::I8_RG:
                return {GL_BYTE, 2, 2, GL_FALSE};
            case Resources::BufferAttribute::I8_RGB:
                return {GL_BYTE, 3, 3, GL_FALSE};
            case Resources::BufferAttribute::I8_RGBA:
                return {GL_BYTE, 4, 4, GL_FALSE};

            case Resources::BufferAttribute::I16_R:
                return {GL_SHORT, 1, 2, GL_FALSE};
            case Resources::BufferAttribute::I16_RG:
                return {GL_SHORT, 2, 4, GL_FALSE};
            case Resources::BufferAttribute::I16_RGB:
                return {GL_SHORT, 3, 6, GL_FALSE};
            case Resources::BufferAttribute::I16_RGBA:
                return {GL_SHORT, 4, 8, GL_FALSE};

            case Resources::BufferAttribute::I32_R:
                return {GL_INT, 1, 4, GL_FALSE};
            case Resources::BufferAttribute::I32_RG:
                return {GL_INT, 2, 8, GL_FALSE};
            case Resources::BufferAttribute::I32_RGB:
                return {GL_INT, 3, 12, GL_FALSE};
            case Resources::BufferAttribute::I32_RGBA:
                return {GL_INT, 4, 16, GL_FALSE};

            case Resources::BufferAttribute::U8_R:
                return {GL_UNSIGNED_BYTE, 1, 1, GL_FALSE};
            case Resources::BufferAttribute::U8_RG:
                return {GL_UNSIGNED_BYTE, 2, 2, GL_FALSE};
            case Resources::BufferAttribute::U8_RGB:
                return {GL_UNSIGNED_BYTE, 3, 3, GL_FALSE};
            case Resources::BufferAttribute::U8_RGBA:
                return {GL_UNSIGNED_BYTE, 4, 4, GL_FALSE};

            case Resources::BufferAttribute::U16_R:
                return {GL_UNSIGNED_SHORT, 1, 2, GL_FALSE};
            case Resources::BufferAttribute::U16_RG:
                return {GL_UNSIGNED_SHORT, 2, 4, GL_FALSE};
            case Resources::BufferAttribute::U16_RGB:
                return {GL_UNSIGNED_SHORT, 3, 6, GL_FALSE};
            case Resources::BufferAttribute::U16_RGBA:
                return {GL_UNSIGNED_SHORT, 4, 8, GL_FALSE};

            case Resources::BufferAttribute::U32_R:
                return {GL_UNSIGNED_INT, 1, 4, GL_FALSE};
            case Resources::BufferAttribute::U32_RG:
                return {GL_UNSIGNED_INT, 2, 8, GL_FALSE};
            case Resources::BufferAttribute::U32_RGB:
                return {GL_UNSIGNED_INT, 3, 12, GL_FALSE};
            case Resources::BufferAttribute::U32_RGBA:
                return {GL_UNSIGNED_INT, 4, 16, GL_FALSE};

            case Resources::BufferAttribute::SN8_R:
                return {GL_BYTE, 1, 1, GL_TRUE};
            case Resources::BufferAttribute::SN8_RG:
                return {GL_BYTE, 2, 2, GL_TRUE};
            case Resources::BufferAttribute::SN8_RGB:
                return {GL_BYTE, 3, 3, GL_TRUE};
            case Resources::BufferAttribute::SN8_RGBA:
                return {GL_BYTE, 4, 4, GL_TRUE};

            case Resources::BufferAttribute::SN16_R:
                return {GL_SHORT, 1, 2, GL_TRUE};
            case Resources::BufferAttribute::SN16_RG:
                return {GL_SHORT, 2, 4, GL_TRUE};
            case Resources::BufferAttribute::SN16_RGB:
                return {GL_SHORT, 3, 6, GL_TRUE};
            case Resources::BufferAttribute::SN16_RGBA:
                return {GL_SHORT, 4, 8, GL_TRUE};

            case Resources::BufferAttribute::UN8_R:
                return {GL_UNSIGNED_BYTE, 1, 1, GL_TRUE};
            case Resources::BufferAttribute::UN8_RG:
                return {GL_UNSIGNED_BYTE, 2, 2, GL_TRUE};
            case Resources::BufferAttribute::UN8_RGB:
                return {GL_UNSIGNED_BYTE, 3, 3, GL_TRUE};
            case Resources::BufferAttribute::UN8_RGBA:
                return {GL_UNSIGNED_BYTE, 4, 4, GL_TRUE};

            case Resources::BufferAttribute::UN16_R:
                return {GL_UNSIGNED_SHORT, 1, 2, GL_TRUE};
            case Resources::BufferAttribute::UN16_RG:
                return {GL_UNSIGNED_SHORT, 2, 4, GL_TRUE};
            case Resources::BufferAttribute::UN16_RGB:
                return {GL_UNSIGNED_SHORT, 3, 6, GL_TRUE};
            case Resources::BufferAttribute::UN16_RGBA:
                return {GL_UNSIGNED_SHORT, 4, 8, GL_TRUE};

            case Resources::BufferAttribute::F32_2x2:
                return {GL_FLOAT, 4, 16, GL_FALSE};
            case Resources::BufferAttribute::F32_2x3:
                return {GL_FLOAT, 6, 24, GL_FALSE};
            case Resources::BufferAttribute::F32_2x4:
                return {GL_FLOAT, 8, 32, GL_FALSE};
            case Resources::BufferAttribute::F32_3x2:
                return {GL_FLOAT, 6, 24, GL_FALSE};
            case Resources::BufferAttribute::F32_3x3:
                return {GL_FLOAT, 9, 36, GL_FALSE};
            case Resources::BufferAttribute::F32_3x4:
                return {GL_FLOAT, 12, 48, GL_FALSE};
            case Resources::BufferAttribute::F32_4x2:
                return {GL_FLOAT, 8, 32, GL_FALSE};
            case Resources::BufferAttribute::F32_4x3:
                return {GL_FLOAT, 12, 48, GL_FALSE};
            case Resources::BufferAttribute::F32_4x4:
                return {GL_FLOAT, 16, 64, GL_FALSE};
        }

        throw;
    }

    std::string RendererBackendImplementation<RendererBackend::OpenGL>::ConvertShaderBinary(Resources::RasterPipelineData& pipelineData, Resources::ShaderDescriptor& shaderDescriptor, std::size_t stageIndex)
    {
        auto& data = *static_cast<OpenGLRasterPipelineData*>(pipelineData.BackendMetadata);

        std::ifstream spirvFile(shaderDescriptor.Path, std::ios::binary | std::ios::ate);

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

        switch (stageIndex)
        {
            case 0: // Vertex
            {
                glsl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelVertex);

                break;
            }
            case 1: // Pixel
            {
                glsl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelFragment);

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

        if (stageIndex == 0) // vertex
        {
            std::size_t attributeCount = 0;

            for (const auto& vertexinput : pipelineData.VertexBufferFormats)
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

        DeduplicatePreserveOrder(data.ConstantBufferBindings);
        DeduplicatePreserveOrder(data.StorageBufferBindings);

        return glsl.compile();
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::DeduplicatePreserveOrder(std::vector<GLuint>& vector)
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
}