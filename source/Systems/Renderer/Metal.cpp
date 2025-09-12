#if !defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

#include <filesystem>
#include <fstream>

#include <spirv_cross/spirv_msl.hpp>

namespace Systems
{
    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(const WindowInteractionLayer<WindowInteractive::COCOA_LAYER>& layer)
            : Layer(layer)
        {
        }

        WindowInteractionLayer<WindowInteractive::COCOA_LAYER> Layer;

        std::vector<std::string> ConvertShaderBinaries(const Resources::RasterPipelineDescriptor& descriptor)
        {
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

            for (const auto& shaderDescriptor : descriptor.Shaders)
            {
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

                spirv_cross::CompilerMSL msl(std::move(spirv));
                spirv_cross::CompilerMSL::Options options;

                msl.set_msl_options(options);

                auto entryPoints = msl.get_entry_points_and_stages();

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
                        msl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelVertex);

                        break;
                    }
                    case Resources::ShaderStage::PIXEL:
                    {
                        msl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelFragment);

                        break;
                    }
                    case Resources::ShaderStage::GEOMETRY:
                    {
                        msl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelGeometry);

                        break;
                    }
                }

                spirv_cross::ShaderResources resources = msl.get_shader_resources();

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

                        msl.set_decoration(attribute.id, spv::DecorationLocation, static_cast<std::uint32_t>(i));
                    }
                }

                std::string source = msl.compile();

                Debug::Log("{}", source);

                sources.push_back(source);
            }

            return sources;
        }
    };

    RendererBackendImplementation<RendererBackend::METAL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
    }

    RendererBackendImplementation<RendererBackend::METAL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::Update()
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SetBufferData(const Resources::BufferHandle&, const Resources::BufferDescriptor&, const Resources::BufferData&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteBuffer(const Resources::BufferHandle&, const Resources::BufferDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreatePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor& descriptor)
    {
        mSpecifics->ConvertShaderBinaries(descriptor);
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeletePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteCommandBuffer(const CommandBuffer&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type&)
    {
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::METAL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window.CreateInteractionLayer<WindowInteractive::COCOA_LAYER>());
    }
}
#endif