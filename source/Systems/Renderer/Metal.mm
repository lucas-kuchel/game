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

    struct MetalShaderInfo
    {
        std::vector<NSUInteger> ConstantBufferBindings;
        std::vector<NSUInteger> StorageBufferBindings;
    };

    struct MetalPipelineData
    {
        id<MTLRenderPipelineState> State = nil;

        std::vector<MetalShaderInfo> ShaderInfo;

        Resources::RasterPipelineDescriptor Descriptor;

        std::vector<NSUInteger> VertexBufferBindings;

        MTLTriangleFillMode FillMode;
        MTLCullMode CullMode;
        MTLWinding CullWinding;

        MTLPrimitiveType Primitive;
    };

    struct MetalSubmissionData
    {
        Resources::SubmissionDescriptor Descriptor;
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

        id<MTLDevice> Device = nil;
        id<MTLCommandQueue> CommandQueue = nil;
        id<MTLCommandBuffer> RasterCommandBuffer = nil;
        id<CAMetalDrawable> CurrentDrawable = nil;

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

        MTLVertexDescriptor* BuildVertexDescriptor(const MetalPipelineData& data)
        {
            MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor alloc] init];
            auto& descriptor = data.Descriptor;

            NSUInteger attributeIndex = 0;
            NSUInteger bufferIndex = 0;

            for (const auto& buf : descriptor.VertexBufferFormats)
            {
                NSUInteger offset = 0;

                for (auto attr : buf.Attributes)
                {
                    auto typeInfo = GetTypeInfo(attr);

                    if (typeInfo.Format == MTLVertexFormatInvalid)
                    {
                        throw;
                    }

                    vertexDescriptor.attributes[attributeIndex].format = typeInfo.Format;
                    vertexDescriptor.attributes[attributeIndex].offset = offset;
                    vertexDescriptor.attributes[attributeIndex].bufferIndex = data.VertexBufferBindings[bufferIndex];

                    offset += typeInfo.Size;

                    attributeIndex++;
                }

                vertexDescriptor.layouts[data.VertexBufferBindings[bufferIndex]].stride = offset;
                vertexDescriptor.layouts[data.VertexBufferBindings[bufferIndex]].stepFunction = MTLVertexStepFunctionPerVertex;
                vertexDescriptor.layouts[data.VertexBufferBindings[bufferIndex]].stepRate = 1;

                bufferIndex++;
            }

            return vertexDescriptor;
        }

        std::vector<std::string> ConvertShaderBinaries(MetalPipelineData& pipelineData)
        {
            auto& descriptor = pipelineData.Descriptor;

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
            pipelineData.ShaderInfo.reserve(descriptor.Shaders.size());

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

                pipelineData.ShaderInfo.emplace_back();

                auto& shaderInfo = pipelineData.ShaderInfo.back();

                struct ShaderBindingInfo
                {
                    uint32_t originalBinding;
                    spv::StorageClass storageClass;
                    uint32_t finalSlot;
                    spirv_cross::Resource& original;
                };

                std::vector<ShaderBindingInfo> allBuffers;

                for (auto& constantBuffer : resources.uniform_buffers)
                {
                    allBuffers.push_back({msl.get_decoration(constantBuffer.id, spv::DecorationBinding), spv::StorageClassUniform, 0, constantBuffer});
                }

                for (auto& storageBuffer : resources.storage_buffers)
                {
                    allBuffers.push_back({msl.get_decoration(storageBuffer.id, spv::DecorationBinding), spv::StorageClassStorageBuffer, 0, storageBuffer});
                }

                uint32_t nextSlot = 0;

                for (auto& buffer : allBuffers)
                {
                    buffer.finalSlot = nextSlot++;

                    msl.set_decoration(buffer.original.id, spv::DecorationBinding, buffer.finalSlot);
                }

                for (std::size_t i = 0; i < allBuffers.size(); i++)
                {
                    if (allBuffers[i].storageClass == spv::StorageClassUniform)
                    {
                        shaderInfo.ConstantBufferBindings.push_back(allBuffers[i].finalSlot);
                    }
                    else
                    {
                        shaderInfo.StorageBufferBindings.push_back(allBuffers[i].finalSlot);
                    }
                }

                if (shaderDescriptor.Stage == Resources::ShaderStage::VERTEX)
                {
                    NSUInteger vertexBufferOffset = 0;
                    if (!shaderInfo.ConstantBufferBindings.empty())
                    {
                        vertexBufferOffset = *std::max_element(shaderInfo.ConstantBufferBindings.begin(), shaderInfo.ConstantBufferBindings.end()) + 1;
                    }
                    if (!shaderInfo.StorageBufferBindings.empty())
                    {
                        vertexBufferOffset = std::max(vertexBufferOffset, *std::max_element(shaderInfo.StorageBufferBindings.begin(), shaderInfo.StorageBufferBindings.end()) + 1);
                    }

                    for (std::size_t i = 0; i < descriptor.VertexBufferFormats.size(); i++)
                    {
                        pipelineData.VertexBufferBindings.push_back(vertexBufferOffset + i);
                    }
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
        mSpecifics->MetalLayer.maximumDrawableCount = 3;

        NSView* contentView = window.contentView;

        [contentView setWantsLayer:YES];
        [contentView setLayer:mSpecifics->MetalLayer];

        switch (descriptor.VSyncMode)
        {
            case RendererVSyncMode::DISABLED:
            {
                mSpecifics->MetalLayer.displaySyncEnabled = NO;

                break;
            }
            case RendererVSyncMode::STRICT:
            case RendererVSyncMode::RELAXED:
            {
                mSpecifics->MetalLayer.displaySyncEnabled = YES;

                break;
            }
        }
    }

    RendererBackendImplementation<RendererBackend::METAL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::BeginFrame()
    {
        mSpecifics->CurrentDrawable = [mSpecifics->MetalLayer nextDrawable];

        if (!mSpecifics->CurrentDrawable)
        {
            return;
        }

        mSpecifics->RasterCommandBuffer = [mSpecifics->CommandQueue commandBuffer];

        MTLRenderPassDescriptor* clearPass = [MTLRenderPassDescriptor renderPassDescriptor];

        clearPass.colorAttachments[0].texture = [mSpecifics->CurrentDrawable texture];
        clearPass.colorAttachments[0].loadAction = MTLLoadActionClear;
        clearPass.colorAttachments[0].storeAction = MTLStoreActionStore;
        clearPass.colorAttachments[0].clearColor = MTLClearColorMake(
            mSpecifics->ClearColour.r,
            mSpecifics->ClearColour.g,
            mSpecifics->ClearColour.b,
            mSpecifics->ClearColour.a);

        id<MTLRenderCommandEncoder> clearEncoder = [mSpecifics->RasterCommandBuffer renderCommandEncoderWithDescriptor:clearPass];

        [clearEncoder endEncoding];
    }

    void RendererBackendImplementation<RendererBackend::METAL>::EndFrame()
    {
        if (!mSpecifics->CurrentDrawable)
        {
            return;
        }

        [mSpecifics->RasterCommandBuffer presentDrawable:mSpecifics->CurrentDrawable];
        [mSpecifics->RasterCommandBuffer commit];

        mSpecifics->RasterCommandBuffer = nil;
        mSpecifics->CurrentDrawable = nil;
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

    void RendererBackendImplementation<RendererBackend::METAL>::CreatePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor& descriptor)
    {
        auto& info = mSpecifics->Pipelines.Insert(handle.ID, MetalPipelineData());

        info.Descriptor = descriptor;

        auto sources = mSpecifics->ConvertShaderBinaries(info);

        NSMutableArray<id<MTLFunction>>* functions = [NSMutableArray array];

        NSError* error = nil;

        for (const auto& source : sources)
        {
            NSString* mslSource = [NSString stringWithUTF8String:source.c_str()];

            id<MTLLibrary> library = [mSpecifics->Device newLibraryWithSource:mslSource
                                                                      options:nil
                                                                        error:&error];

            if (!library)
            {
                std::string errorStr = [[error localizedDescription] UTF8String];

                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                        "Metal shader compilation error\n"
                                                                        "failed to compile shaders\n"
                                                                        "Metal compilation error info log:\n\n{}",
                                       errorStr.data());
            }

            for (const auto& shader : descriptor.Shaders)
            {
                id<MTLFunction> function = [library newFunctionWithName:[NSString stringWithUTF8String:shader.Function.c_str()]];

                if (function)
                {
                    [functions addObject:function];
                }
            }
        }

        MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

        for (id<MTLFunction> function in functions)
        {
            NSString* functionName = function.name;

            for (const auto& shader : descriptor.Shaders)
            {
                if (shader.Function == [functionName UTF8String])
                {
                    switch (shader.Stage)
                    {
                        case Resources::ShaderStage::VERTEX:
                        {
                            pipelineDescriptor.vertexFunction = function;
                            break;
                        }
                        case Resources::ShaderStage::PIXEL:
                        {
                            pipelineDescriptor.fragmentFunction = function;
                            break;
                        }
                        default:
                        {
                            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                                    "Metal pipeline creation error\n"
                                                                                    "Metal does not support geometry shaders");
                        }
                    }
                }
            }
        }

        switch (info.Descriptor.RasterState.Primitive)
        {
            case Resources::PipelinePrimitive::TRIANGLE_LIST:
            {
                info.Primitive = MTLPrimitiveTypeTriangle;

                break;
            }
            case Resources::PipelinePrimitive::LINE_LIST:
            {
                info.Primitive = MTLPrimitiveTypeLine;

                break;
            }
            case Resources::PipelinePrimitive::POINT:
            {
                info.Primitive = MTLPrimitiveTypePoint;

                break;
            }
        }

        pipelineDescriptor.vertexDescriptor = [mSpecifics->BuildVertexDescriptor(info) retain];
        pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

        pipelineDescriptor.rasterizationEnabled = YES;

        switch (descriptor.RasterState.Primitive)
        {
            case Resources::PipelinePrimitive::TRIANGLE_LIST:
            {
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;

                break;
            }
            case Resources::PipelinePrimitive::LINE_LIST:
            {
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassLine;

                break;
            }
            case Resources::PipelinePrimitive::POINT:
            {
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassPoint;

                break;
            }
        }

        switch (descriptor.RasterState.FaceCulling)
        {
            case Resources::PipelineFaceCulling::FRONTFACE:
            {
                info.CullMode = MTLCullModeFront;

                break;
            }
            case Resources::PipelineFaceCulling::BACKFACE:
            {
                info.CullMode = MTLCullModeBack;

                break;
            }
            case Resources::PipelineFaceCulling::DISABLED:
            {
                info.CullMode = MTLCullModeNone;

                break;
            }
        }

        switch (descriptor.RasterState.FrontFace)
        {
            case Resources::PipelineFrontFace::CLOCKWISE:
            {
                info.CullWinding = MTLWindingClockwise;

                break;
            }
            case Resources::PipelineFrontFace::ANTICLOCKWISE:
            {
                info.CullWinding = MTLWindingCounterClockwise;

                break;
            }
        }

        switch (descriptor.RasterState.PolygonMode)
        {
            case Resources::PipelinePolygonMode::SOLID:
            {
                info.FillMode = MTLTriangleFillModeFill;

                break;
            }
            case Resources::PipelinePolygonMode::LINE:
            {
                info.FillMode = MTLTriangleFillModeLines;

                break;
            }
            case Resources::PipelinePolygonMode::POINT:
            {
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                        "Metal pipeline creation error\n"
                                                                        "Metal does not support point fill mode");
            }
        }

        NSError* pipelineError = nil;

        info.State = [mSpecifics->Device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&pipelineError];

        if (!info.State)
        {
            std::string errorStr = [[pipelineError localizedDescription] UTF8String];
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal pipeline creation error\n"
                                                                    "Metal error info log:\n\n{}",
                                   errorStr.data());
        }
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeletePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor&)
    {
        mSpecifics->Pipelines.Remove(handle.ID);
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor)
    {
        auto& info = mSpecifics->Submissions.Insert(handle.ID, MetalSubmissionData());

        info.Descriptor = descriptor;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor&)
    {
        mSpecifics->Submissions.Remove(handle.ID);
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateCommandBuffer(const CommandBuffer& commandBuffer)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer& buffer)
    {
        auto& submissions = buffer.GetContents();

        if (!mSpecifics->RasterCommandBuffer || !mSpecifics->CurrentDrawable)
        {
            return;
        }

        for (auto& submission : submissions)
        {
            auto& submissionInfo = mSpecifics->Submissions.Get(submission.ID);
            auto& pipelineInfo = mSpecifics->Pipelines.Get(submissionInfo.Descriptor.Pipeline.ID);
            auto& indexBufferInfo = mSpecifics->Buffers.Get(submissionInfo.Descriptor.IndexBuffer.ID);

            MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];

            passDescriptor.colorAttachments[0].texture = [mSpecifics->CurrentDrawable texture];
            passDescriptor.colorAttachments[0].loadAction = MTLLoadActionLoad;
            passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

            id<MTLRenderCommandEncoder> encoder = [mSpecifics->RasterCommandBuffer renderCommandEncoderWithDescriptor:passDescriptor];

            [encoder setRenderPipelineState:pipelineInfo.State];

            for (std::size_t stageIndex = 0; stageIndex < pipelineInfo.ShaderInfo.size(); stageIndex++)
            {
                auto& shaderInfo = pipelineInfo.ShaderInfo[stageIndex];
                auto& stageDescriptor = submissionInfo.Descriptor.ShaderStages[stageIndex];

                for (std::size_t i = 0; i < stageDescriptor.ConstantBuffers.size(); i++)
                {
                    auto& bufferHandle = stageDescriptor.ConstantBuffers[i];
                    auto& bufferData = mSpecifics->Buffers.Get(bufferHandle.ID);

                    if (stageDescriptor.Stage == Resources::ShaderStage::VERTEX)
                    {
                        [encoder setVertexBuffer:bufferData.Handle offset:0 atIndex:shaderInfo.ConstantBufferBindings[i]];
                    }
                    else if (stageDescriptor.Stage == Resources::ShaderStage::PIXEL)
                    {
                        [encoder setFragmentBuffer:bufferData.Handle offset:0 atIndex:shaderInfo.ConstantBufferBindings[i]];
                    }
                }

                for (std::size_t i = 0; i < stageDescriptor.StorageBuffers.size(); i++)
                {
                    auto& bufferHandle = stageDescriptor.StorageBuffers[i];
                    auto& bufferData = mSpecifics->Buffers.Get(bufferHandle.ID);

                    if (stageDescriptor.Stage == Resources::ShaderStage::VERTEX)
                    {
                        [encoder setVertexBuffer:bufferData.Handle offset:0 atIndex:shaderInfo.StorageBufferBindings[i]];
                    }
                    else if (stageDescriptor.Stage == Resources::ShaderStage::PIXEL)
                    {
                        [encoder setFragmentBuffer:bufferData.Handle offset:0 atIndex:shaderInfo.StorageBufferBindings[i]];
                    }
                }

                if (stageDescriptor.Stage == Resources::ShaderStage::VERTEX)
                {
                    for (std::size_t i = 0; i < submissionInfo.Descriptor.VertexBuffers.size(); i++)
                    {
                        auto& vertexBufferHandle = submissionInfo.Descriptor.VertexBuffers[i];
                        auto& vertexBufferData = mSpecifics->Buffers.Get(vertexBufferHandle.ID);

                        [encoder setVertexBuffer:vertexBufferData.Handle offset:0 atIndex:pipelineInfo.VertexBufferBindings[i]];
                    }
                }
            }

            auto& indexBufferData = mSpecifics->Buffers.Get(submissionInfo.Descriptor.IndexBuffer.ID);
            MTLIndexType indexType;

            switch (submissionInfo.Descriptor.IndexBufferType)
            {
                case Resources::BufferAttributeType::R16_UINT:
                {
                    indexType = MTLIndexTypeUInt16;

                    break;
                }
                case Resources::BufferAttributeType::R32_UINT:
                {
                    indexType = MTLIndexTypeUInt32;

                    break;
                }
                default:
                {
                    return;
                }
            }

            auto typeInfo = mSpecifics->GetTypeInfo(submissionInfo.Descriptor.IndexBufferType);

            std::size_t indexCount = indexBufferData.Descriptor.Size / typeInfo.Size;

            [encoder setTriangleFillMode:pipelineInfo.FillMode];
            [encoder setCullMode:pipelineInfo.CullMode];
            [encoder setFrontFacingWinding:pipelineInfo.CullWinding];
            [encoder drawIndexedPrimitives:pipelineInfo.Primitive
                                indexCount:indexCount
                                 indexType:indexType
                               indexBuffer:indexBufferData.Handle
                         indexBufferOffset:0];
            [encoder endEncoding];
        }
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteCommandBuffer(const CommandBuffer& commandBuffer)
    {
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type& vsyncMode)
    {
        switch (vsyncMode)
        {
            case RendererVSyncMode::DISABLED:
            {
                mSpecifics->MetalLayer.displaySyncEnabled = NO;

                break;
            }
            case RendererVSyncMode::STRICT:
            case RendererVSyncMode::RELAXED:
            {
                mSpecifics->MetalLayer.displaySyncEnabled = YES;

                break;
            }
        }
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