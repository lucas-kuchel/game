#if defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

#include <filesystem>
#include <fstream>

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <spirv_cross/spirv_msl.hpp>

namespace Systems
{
    struct MetalBufferData
    {
        id<MTLBuffer> Handle = nil;

        Resources::BufferDescriptor Descriptor;
    };

    struct MetalPipelineData
    {
        id<MTLRenderPipelineState> State = nil;

        Resources::RasterPipelineDescriptor Descriptor;
    };

    struct MetalSubmissionData
    {
        Resources::SubmissionDescriptor Descriptor;
    };

    struct MetalCommandBufferData
    {
        id<MTLCommandBuffer> Buffer;
    };

    struct MetalAttributeTypeInfo
    {
        MTLVertexFormat Format;
        NSUInteger Size;
    };

    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(RendererWindow& window)
            : Window(window)
        {
        }

        RendererWindow& Window;

        Types::SparseSet<MetalBufferData> Buffers;
        Types::SparseSet<MetalPipelineData> Pipelines;
        Types::SparseSet<MetalSubmissionData> Submissions;
        Types::SparseSet<MetalCommandBufferData> CommandBuffers;

        id<MTLDevice> Device = nil;
        id<MTLCommandQueue> CommandQueue = nil;
        id<MTLCommandBuffer> CommandBuffer = nil;

        CAMetalLayer* MetalLayer = nil;

        glm::fvec4 ClearColour;

        static MetalAttributeTypeInfo GetTypeInfo(Resources::BufferAttributeType type)
        {
            using T = Resources::BufferAttributeType;

            switch (type)
            {
                case T::R32_FLOAT:
                {
                    return {MTLVertexFormatFloat, sizeof(float)};
                }
                case T::R32G32_FLOAT:
                {
                    return {MTLVertexFormatFloat2, sizeof(float) * 2};
                }
                case T::R32G32B32_FLOAT:
                {
                    return {MTLVertexFormatFloat3, sizeof(float) * 3};
                }
                case T::R32G32B32A32_FLOAT:
                {
                    return {MTLVertexFormatFloat4, sizeof(float) * 4};
                }
                case T::R8_INT:
                {
                    return {MTLVertexFormatChar, sizeof(std::int8_t)};
                }
                case T::R8G8_INT:
                {
                    return {MTLVertexFormatChar2, sizeof(std::int8_t) * 2};
                }
                case T::R8G8B8_INT:
                {
                    return {MTLVertexFormatChar3, sizeof(std::int8_t) * 3};
                }
                case T::R8G8B8A8_INT:
                {
                    return {MTLVertexFormatChar4, sizeof(std::int8_t) * 4};
                }
                case T::R16_INT:
                {
                    return {MTLVertexFormatShort, sizeof(std::int16_t)};
                }
                case T::R16G16_INT:
                {
                    return {MTLVertexFormatShort2, sizeof(std::int16_t) * 2};
                }
                case T::R16G16B16_INT:
                {
                    return {MTLVertexFormatShort3, sizeof(std::int16_t) * 3};
                }
                case T::R16G16B16A16_INT:
                {
                    return {MTLVertexFormatShort4, sizeof(std::int16_t) * 4};
                }
                case T::R32_INT:
                {
                    return {MTLVertexFormatInt, sizeof(std::int32_t)};
                }
                case T::R32G32_INT:
                {
                    return {MTLVertexFormatInt2, sizeof(std::int32_t) * 2};
                }
                case T::R32G32B32_INT:
                {
                    return {MTLVertexFormatInt3, sizeof(std::int32_t) * 3};
                }
                case T::R32G32B32A32_INT:
                {
                    return {MTLVertexFormatInt4, sizeof(std::int32_t) * 4};
                }
                case T::R8_UINT:
                {
                    return {MTLVertexFormatUChar, sizeof(std::uint8_t)};
                }
                case T::R8G8_UINT:
                {
                    return {MTLVertexFormatUChar2, sizeof(std::uint8_t) * 2};
                }
                case T::R8G8B8_UINT:
                {
                    return {MTLVertexFormatUChar3, sizeof(std::uint8_t) * 3};
                }
                case T::R8G8B8A8_UINT:
                {
                    return {MTLVertexFormatUChar4, sizeof(std::uint8_t) * 4};
                }
                case T::R16_UINT:
                {
                    return {MTLVertexFormatUShort, sizeof(std::uint16_t)};
                }
                case T::R16G16_UINT:
                {
                    return {MTLVertexFormatUShort2, sizeof(std::uint16_t) * 2};
                }
                case T::R16G16B16_UINT:
                {
                    return {MTLVertexFormatUShort3, sizeof(std::uint16_t) * 3};
                }
                case T::R16G16B16A16_UINT:
                {
                    return {MTLVertexFormatUShort4, sizeof(std::uint16_t) * 4};
                }
                case T::R32_UINT:
                {
                    return {MTLVertexFormatUInt, sizeof(std::uint32_t)};
                }
                case T::R32G32_UINT:
                {
                    return {MTLVertexFormatUInt2, sizeof(std::uint32_t) * 2};
                }
                case T::R32G32B32_UINT:
                {
                    return {MTLVertexFormatUInt3, sizeof(std::uint32_t) * 3};
                }
                case T::R32G32B32A32_UINT:
                {
                    return {MTLVertexFormatUInt4, sizeof(std::uint32_t) * 4};
                }
                case T::R8_SNORM:
                {
                    return {MTLVertexFormatCharNormalized, sizeof(std::int8_t)};
                }
                case T::R8G8_SNORM:
                {
                    return {MTLVertexFormatChar2Normalized, sizeof(std::int8_t) * 2};
                }
                case T::R8G8B8_SNORM:
                {
                    return {MTLVertexFormatChar3Normalized, sizeof(std::int8_t) * 3};
                }
                case T::R8G8B8A8_SNORM:
                {
                    return {MTLVertexFormatChar4Normalized, sizeof(std::int8_t) * 4};
                }
                case T::R16_SNORM:
                {
                    return {MTLVertexFormatShortNormalized, sizeof(std::int16_t)};
                }
                case T::R16G16_SNORM:
                {
                    return {MTLVertexFormatShort2Normalized, sizeof(std::int16_t) * 2};
                }
                case T::R16G16B16_SNORM:
                {
                    return {MTLVertexFormatShort3Normalized, sizeof(std::int16_t) * 3};
                }
                case T::R16G16B16A16_SNORM:
                {
                    return {MTLVertexFormatShort4Normalized, sizeof(std::int16_t) * 4};
                }
                case T::R8_UNORM:
                {
                    return {MTLVertexFormatUCharNormalized, sizeof(std::uint8_t)};
                }
                case T::R8G8_UNORM:
                {
                    return {MTLVertexFormatUChar2Normalized, sizeof(std::uint8_t) * 2};
                }
                case T::R8G8B8_UNORM:
                {
                    return {MTLVertexFormatUChar3Normalized, sizeof(std::uint8_t) * 3};
                }
                case T::R8G8B8A8_UNORM:
                {
                    return {MTLVertexFormatUChar4Normalized, sizeof(std::uint8_t) * 4};
                }
                case T::R16_UNORM:
                {
                    return {MTLVertexFormatUShortNormalized, sizeof(std::uint16_t)};
                }
                case T::R16G16_UNORM:
                {
                    return {MTLVertexFormatUShort2Normalized, sizeof(std::uint16_t) * 2};
                }
                case T::R16G16B16_UNORM:
                {
                    return {MTLVertexFormatUShort3Normalized, sizeof(std::uint16_t) * 3};
                }
                case T::R16G16B16A16_UNORM:
                {
                    return {MTLVertexFormatUShort4Normalized, sizeof(std::uint16_t) * 4};
                }
                default:
                {
                    return {MTLVertexFormatInvalid, 0};
                }
            }
        }

        MTLVertexDescriptor* BuildVertexDescriptor(const Resources::RasterPipelineDescriptor& desc)
        {
            MTLVertexDescriptor* vertexDesc = [[MTLVertexDescriptor alloc] init];

            NSUInteger attributeIndex = 0;
            NSUInteger bufferIndex = 0;

            for (const auto& buf : desc.VertexBufferFormats)
            {
                NSUInteger offset = 0;

                for (auto attr : buf.Attributes)
                {
                    auto typeInfo = GetTypeInfo(attr);

                    if (typeInfo.Format == MTLVertexFormatInvalid)
                    {
                        throw;
                    }

                    vertexDesc.attributes[attributeIndex].format = typeInfo.Format;
                    vertexDesc.attributes[attributeIndex].offset = offset;
                    vertexDesc.attributes[attributeIndex].bufferIndex = bufferIndex;

                    offset += typeInfo.Size;

                    attributeIndex++;
                }

                vertexDesc.layouts[bufferIndex].stride = offset;
                vertexDesc.layouts[bufferIndex].stepFunction = MTLVertexStepFunctionPerVertex;
                vertexDesc.layouts[bufferIndex].stepRate = 1;

                bufferIndex++;
            }

            return vertexDesc;
        }

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

                sources.push_back(source);
            }

            return sources;
        }
    };

    RendererBackendImplementation<RendererBackend::METAL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
        auto layer = mSpecifics->Window.CreateInteractionLayer<WindowInteractive::COCOA_LAYER>();
        NSWindow* window = static_cast<NSWindow*>(layer.GetCocoaWindow());

        mSpecifics->Device = MTLCreateSystemDefaultDevice();
        mSpecifics->CommandQueue = [mSpecifics->Device newCommandQueue];

        mSpecifics->MetalLayer = [CAMetalLayer layer];

        mSpecifics->MetalLayer.device = mSpecifics->Device;
        mSpecifics->MetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        mSpecifics->MetalLayer.framebufferOnly = YES;

        NSView* contentView = window.contentView;

        [contentView setWantsLayer:YES];
        [contentView setLayer:mSpecifics->MetalLayer];
    }

    RendererBackendImplementation<RendererBackend::METAL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::Update()
    {
        id<CAMetalDrawable> drawable = [mSpecifics->MetalLayer nextDrawable];

        if (!drawable)
        {
            return;
        }

        mSpecifics->CommandBuffer = [mSpecifics->CommandQueue commandBuffer];

        MTLRenderPassDescriptor* clearPass = [MTLRenderPassDescriptor renderPassDescriptor];

        clearPass.colorAttachments[0].texture = [drawable texture];
        clearPass.colorAttachments[0].loadAction = MTLLoadActionClear;
        clearPass.colorAttachments[0].storeAction = MTLStoreActionStore;
        clearPass.colorAttachments[0].clearColor = MTLClearColorMake(
            mSpecifics->ClearColour.r,
            mSpecifics->ClearColour.g,
            mSpecifics->ClearColour.b,
            mSpecifics->ClearColour.a);

        id<MTLRenderCommandEncoder> clearEncoder = [mSpecifics->CommandBuffer renderCommandEncoderWithDescriptor:clearPass];

        [clearEncoder endEncoding];

        [mSpecifics->CommandBuffer commit];
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor)
    {
        auto& data = mSpecifics->Buffers.Insert(handle.ID, MetalBufferData());

        data.Handle = [mSpecifics->Device newBufferWithLength:descriptor.Size options:MTLResourceStorageModeShared];

        data.Descriptor = descriptor;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferDescriptor&, const Resources::BufferData& data)
    {
        auto& info = mSpecifics->Buffers.Get(handle.ID);

        std::uint8_t* memory = static_cast<std::uint8_t*>([info.Handle contents]);
        std::memcpy(memory + data.Offset, data.Data, data.Stride);
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor&)
    {
        mSpecifics->Buffers.Remove(handle.ID);
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

    void RendererBackendImplementation<RendererBackend::METAL>::CreateCommandBuffer(const CommandBuffer& commandBuffer)
    {
        auto& info = mSpecifics->CommandBuffers.Insert(commandBuffer.GetID(), MetalCommandBufferData());

        info.Buffer = [mSpecifics->CommandQueue commandBuffer];
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteCommandBuffer(const CommandBuffer& commandBuffer)
    {
        mSpecifics->CommandBuffers.Remove(commandBuffer.GetID());
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type&)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& clearColour)
    {
        mSpecifics->ClearColour = clearColour;
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::METAL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window);
    }
}
#endif