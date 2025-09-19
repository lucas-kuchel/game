#if defined(PLATFORM_APPLE)
#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Metal.hpp>

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
    };

    struct MetalShaderInfo
    {
        std::vector<NSUInteger> ConstantBufferBindings;
        std::vector<NSUInteger> StorageBufferBindings;
    };

    struct MetalPipelineData
    {
        id<MTLRenderPipelineState> State = nil;
        id<MTLDepthStencilState> DepthStencilState = nil;

        std::vector<MetalShaderInfo> ShaderInfo;

        std::vector<NSUInteger> VertexBufferBindings;

        MTLTriangleFillMode FillMode;
        MTLCullMode CullMode;
        MTLWinding CullWinding;
        MTLCompareFunction DepthCompare;

        MTLPrimitiveType Primitive;
    };

    struct MetalSubmissionData
    {
        std::size_t IndexCount;
        MTLIndexType IndexType;
    };

    struct MetalRenderQueueData
    {
        id<MTLCommandBuffer> Buffer;
        id<MTLRenderCommandEncoder> Encoder;
    };

    struct MetalRenderPassData
    {
        MTLRenderPassDescriptor* RenderPass;
    };

    struct MetalAttributeTypeInfo
    {
        MTLVertexFormat Format;
        NSUInteger Size;
    };

    struct RendererBackendImplementation<RendererBackend::Metal>::Internals
    {
    public:
        id<MTLDevice> Device = nil;
        id<MTLCommandQueue> CommandQueue = nil;
        id<CAMetalDrawable> CurrentDrawable = nil;

        NSUInteger DrawableWidth;
        NSUInteger DrawableHeight;

        CAMetalLayer* MetalLayer = nil;

        static MetalAttributeTypeInfo GetTypeInfo(Resources::BufferAttribute attribute)
        {
            switch (attribute)
            {
                case Resources::BufferAttribute::F32_R:
                    return {MTLVertexFormatFloat, sizeof(float)};
                case Resources::BufferAttribute::F32_RG:
                    return {MTLVertexFormatFloat2, sizeof(float) * 2};
                case Resources::BufferAttribute::F32_RGB:
                    return {MTLVertexFormatFloat3, sizeof(float) * 3};
                case Resources::BufferAttribute::F32_RGBA:
                    return {MTLVertexFormatFloat4, sizeof(float) * 4};

                case Resources::BufferAttribute::I8_R:
                    return {MTLVertexFormatChar, sizeof(std::int8_t)};
                case Resources::BufferAttribute::I8_RG:
                    return {MTLVertexFormatChar2, sizeof(std::int8_t) * 2};
                case Resources::BufferAttribute::I8_RGB:
                    return {MTLVertexFormatChar3, sizeof(std::int8_t) * 3};
                case Resources::BufferAttribute::I8_RGBA:
                    return {MTLVertexFormatChar4, sizeof(std::int8_t) * 4};

                case Resources::BufferAttribute::I16_R:
                    return {MTLVertexFormatShort, sizeof(std::int16_t)};
                case Resources::BufferAttribute::I16_RG:
                    return {MTLVertexFormatShort2, sizeof(std::int16_t) * 2};
                case Resources::BufferAttribute::I16_RGB:
                    return {MTLVertexFormatShort3, sizeof(std::int16_t) * 3};
                case Resources::BufferAttribute::I16_RGBA:
                    return {MTLVertexFormatShort4, sizeof(std::int16_t) * 4};

                case Resources::BufferAttribute::I32_R:
                    return {MTLVertexFormatInt, sizeof(std::int32_t)};
                case Resources::BufferAttribute::I32_RG:
                    return {MTLVertexFormatInt2, sizeof(std::int32_t) * 2};
                case Resources::BufferAttribute::I32_RGB:
                    return {MTLVertexFormatInt3, sizeof(std::int32_t) * 3};
                case Resources::BufferAttribute::I32_RGBA:
                    return {MTLVertexFormatInt4, sizeof(std::int32_t) * 4};

                case Resources::BufferAttribute::U8_R:
                    return {MTLVertexFormatUChar, sizeof(std::uint8_t)};
                case Resources::BufferAttribute::U8_RG:
                    return {MTLVertexFormatUChar2, sizeof(std::uint8_t) * 2};
                case Resources::BufferAttribute::U8_RGB:
                    return {MTLVertexFormatUChar3, sizeof(std::uint8_t) * 3};
                case Resources::BufferAttribute::U8_RGBA:
                    return {MTLVertexFormatUChar4, sizeof(std::uint8_t) * 4};

                case Resources::BufferAttribute::U16_R:
                    return {MTLVertexFormatUShort, sizeof(std::uint16_t)};
                case Resources::BufferAttribute::U16_RG:
                    return {MTLVertexFormatUShort2, sizeof(std::uint16_t) * 2};
                case Resources::BufferAttribute::U16_RGB:
                    return {MTLVertexFormatUShort3, sizeof(std::uint16_t) * 3};
                case Resources::BufferAttribute::U16_RGBA:
                    return {MTLVertexFormatUShort4, sizeof(std::uint16_t) * 4};

                case Resources::BufferAttribute::U32_R:
                    return {MTLVertexFormatUInt, sizeof(std::uint32_t)};
                case Resources::BufferAttribute::U32_RG:
                    return {MTLVertexFormatUInt2, sizeof(std::uint32_t) * 2};
                case Resources::BufferAttribute::U32_RGB:
                    return {MTLVertexFormatUInt3, sizeof(std::uint32_t) * 3};
                case Resources::BufferAttribute::U32_RGBA:
                    return {MTLVertexFormatUInt4, sizeof(std::uint32_t) * 4};

                case Resources::BufferAttribute::SN8_R:
                    return {MTLVertexFormatCharNormalized, sizeof(std::int8_t)};
                case Resources::BufferAttribute::SN8_RG:
                    return {MTLVertexFormatChar2Normalized, sizeof(std::int8_t) * 2};
                case Resources::BufferAttribute::SN8_RGB:
                    return {MTLVertexFormatChar3Normalized, sizeof(std::int8_t) * 3};
                case Resources::BufferAttribute::SN8_RGBA:
                    return {MTLVertexFormatChar4Normalized, sizeof(std::int8_t) * 4};

                case Resources::BufferAttribute::SN16_R:
                    return {MTLVertexFormatShortNormalized, sizeof(std::int16_t)};
                case Resources::BufferAttribute::SN16_RG:
                    return {MTLVertexFormatShort2Normalized, sizeof(std::int16_t) * 2};
                case Resources::BufferAttribute::SN16_RGB:
                    return {MTLVertexFormatShort3Normalized, sizeof(std::int16_t) * 3};
                case Resources::BufferAttribute::SN16_RGBA:
                    return {MTLVertexFormatShort4Normalized, sizeof(std::int16_t) * 4};

                case Resources::BufferAttribute::UN8_R:
                    return {MTLVertexFormatUCharNormalized, sizeof(std::uint8_t)};
                case Resources::BufferAttribute::UN8_RG:
                    return {MTLVertexFormatUChar2Normalized, sizeof(std::uint8_t) * 2};
                case Resources::BufferAttribute::UN8_RGB:
                    return {MTLVertexFormatUChar3Normalized, sizeof(std::uint8_t) * 3};
                case Resources::BufferAttribute::UN8_RGBA:
                    return {MTLVertexFormatUChar4Normalized, sizeof(std::uint8_t) * 4};

                case Resources::BufferAttribute::UN16_R:
                    return {MTLVertexFormatUShortNormalized, sizeof(std::uint16_t)};
                case Resources::BufferAttribute::UN16_RG:
                    return {MTLVertexFormatUShort2Normalized, sizeof(std::uint16_t) * 2};
                case Resources::BufferAttribute::UN16_RGB:
                    return {MTLVertexFormatUShort3Normalized, sizeof(std::uint16_t) * 3};
                case Resources::BufferAttribute::UN16_RGBA:
                    return {MTLVertexFormatUShort4Normalized, sizeof(std::uint16_t) * 4};

                default:
                    return {MTLVertexFormatInvalid, 0};
            }
        }

        static MTLVertexDescriptor* BuildVertexDescriptor(const Resources::RasterPipelineData& data)
        {
            MTLVertexDescriptor* vertexDescriptor = [[MTLVertexDescriptor alloc] init];

            auto& info = *static_cast<MetalPipelineData*>(data.BackendMetadata);

            NSUInteger attributeIndex = 0;
            NSUInteger bufferIndex = 0;

            for (const auto& buf : data.VertexBufferFormats)
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
                    vertexDescriptor.attributes[attributeIndex].bufferIndex = info.VertexBufferBindings[bufferIndex];

                    offset += typeInfo.Size;

                    attributeIndex++;
                }

                vertexDescriptor.layouts[info.VertexBufferBindings[bufferIndex]].stride = offset;
                vertexDescriptor.layouts[info.VertexBufferBindings[bufferIndex]].stepFunction = MTLVertexStepFunctionPerVertex;
                vertexDescriptor.layouts[info.VertexBufferBindings[bufferIndex]].stepRate = 1;

                bufferIndex++;
            }

            return vertexDescriptor;
        }

        static std::pair<std::string, MetalShaderInfo> ConvertShaderBinary(Resources::RasterPipelineData& pipelineData, Resources::ShaderDescriptor& shaderDescriptor, std::size_t stageIndex)
        {
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

            switch (stageIndex)
            {
                case 0:
                    msl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelVertex);
                    break;
                case 1:
                    msl.set_entry_point(shaderDescriptor.Function, spv::ExecutionModelFragment);
                    break;
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

            MetalShaderInfo shaderInfo;

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

            auto& pipelineInfo = *static_cast<MetalPipelineData*>(pipelineData.BackendMetadata);

            if (stageIndex == 0)
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

                std::size_t attributeCount = 0;

                for (std::size_t i = 0; i < pipelineData.VertexBufferFormats.size(); i++)
                {
                    pipelineInfo.VertexBufferBindings.push_back(vertexBufferOffset + i);
                    attributeCount += pipelineData.VertexBufferFormats[i].Attributes.size();
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

            return {msl.compile(), shaderInfo};
        }
    };

    RendererBackendImplementation<RendererBackend::Metal>::RendererBackendImplementation(RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mCocoaWindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::CocoaBackend>())
    {
        NSWindow* window = static_cast<NSWindow*>(mCocoaWindowLayer.GetNSWindow());

        mInternals = std::make_unique<Internals>();

        mInternals->Device = MTLCreateSystemDefaultDevice();
        mInternals->CommandQueue = [mInternals->Device newCommandQueue];

        mInternals->MetalLayer = [CAMetalLayer layer];

        mInternals->MetalLayer.device = mInternals->Device;
        mInternals->MetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        mInternals->MetalLayer.framebufferOnly = YES;
        mInternals->MetalLayer.maximumDrawableCount = 3;

        NSView* contentView = window.contentView;

        [contentView setWantsLayer:YES];
        [contentView setLayer:mInternals->MetalLayer];

        switch (descriptor.VSyncMode)
        {
            case RendererVSyncMode::Disabled:
                mInternals->MetalLayer.displaySyncEnabled = NO;
                break;
            case RendererVSyncMode::Enabled:
                mInternals->MetalLayer.displaySyncEnabled = YES;
                break;
            case RendererVSyncMode::AllowLate:
                throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::PipelineHandle Systems::Renderer::Renderer(Systems::RendererDescriptor&):\n"
                                                                   "Metal VSync support conflict\n"
                                                                   "Metal does not support 'RendererVSyncMode::AllowLate'\n"
                                                                   "Use another supported VSync mode");
        }
    }

    RendererBackendImplementation<RendererBackend::Metal>::~RendererBackendImplementation()
    {
        for (auto& submission : mSubmissions.Data)
        {
            if (submission.BackendMetadata)
            {
                DeleteSubmission(submission);
            }
        }

        for (auto& pipeline : mRasterPipelines.Data)
        {
            if (pipeline.BackendMetadata)
            {
                DeletePipeline(pipeline);
            }
        }

        for (auto& buffer : mBuffers.Data)
        {
            if (buffer.BackendMetadata)
            {
                DeallocateBuffer(buffer);
            }
        }
    }

    void RendererBackendImplementation<RendererBackend::Metal>::Clear()
    {
        while (!mInternals->CurrentDrawable)
        {
            mInternals->CurrentDrawable = [mInternals->MetalLayer nextDrawable];
        }

        if (mInternals->DrawableWidth != mInternals->MetalLayer.drawableSize.width || mInternals->DrawableHeight != mInternals->MetalLayer.drawableSize.height)
        {
            mInternals->DrawableWidth = mInternals->MetalLayer.drawableSize.width;
            mInternals->DrawableHeight = mInternals->MetalLayer.drawableSize.height;
        }
    }

    void RendererBackendImplementation<RendererBackend::Metal>::Present()
    {
        if (!mInternals->CurrentDrawable)
        {
            return;
        }

        // TODO: present drawable through a dedicated command buffer

        mInternals->CurrentDrawable = nil;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::AllocateBuffer(Resources::BufferData& data)
    {
        data.BackendMetadata = new MetalBufferData();

        auto& info = *static_cast<MetalBufferData*>(data.BackendMetadata);

        info.Handle = [mInternals->Device newBufferWithLength:data.Size options:MTLResourceStorageModeShared];
    }

    void RendererBackendImplementation<RendererBackend::Metal>::UploadBufferData(Resources::BufferData& data, const Resources::BufferUploadDescriptor& upload)
    {
        auto& info = *static_cast<MetalBufferData*>(data.BackendMetadata);

        std::uint8_t* memory = static_cast<std::uint8_t*>([info.Handle contents]);
        std::memcpy(memory + upload.Offset, upload.Data, upload.Stride);
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeallocateBuffer(Resources::BufferData& data)
    {
        auto* info = static_cast<MetalBufferData*>(data.BackendMetadata);

        [info->Handle release];

        delete info;

        data.BackendMetadata = nullptr;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreatePipeline(Resources::RasterPipelineData& data)
    {
        data.BackendMetadata = new MetalPipelineData();

        auto& info = *static_cast<MetalPipelineData*>(data.BackendMetadata);

        auto [vertexShaderSource, vertexShaderInfo] = mInternals->ConvertShaderBinary(data, data.VertexShader, 0);
        auto [pixelShaderSource, pixelShaderInfo] = mInternals->ConvertShaderBinary(data, data.PixelShader, 1);

        NSMutableArray<id<MTLFunction>>* functions = [NSMutableArray array];

        NSError* vertexError = nil;
        NSError* pixelError = nil;

        NSString* vertexMsl = [NSString stringWithUTF8String:vertexShaderSource.c_str()];
        NSString* pixelMsl = [NSString stringWithUTF8String:pixelShaderSource.c_str()];

        id<MTLLibrary> vertexLibrary = [mInternals->Device newLibraryWithSource:vertexMsl options:nil error:&vertexError];
        id<MTLLibrary> pixelLibrary = [mInternals->Device newLibraryWithSource:pixelMsl options:nil error:&pixelError];

        if (!vertexLibrary)
        {
            std::string errorStr = [[vertexError localizedDescription] UTF8String];

            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal shader compilation error\n"
                                                                    "failed to compile vertex shader\n"
                                                                    "Metal compilation error info log:\n\n{}",
                                   errorStr.data());
        }

        if (!pixelLibrary)
        {
            std::string errorStr = [[pixelError localizedDescription] UTF8String];

            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal shader compilation error\n"
                                                                    "failed to pixel shader\n"
                                                                    "Metal compilation error info log:\n\n{}",
                                   errorStr.data());
        }

        id<MTLFunction> vertexFunction = [vertexLibrary newFunctionWithName:[NSString stringWithUTF8String:data.VertexShader.Function.c_str()]];
        id<MTLFunction> pixelFunction = [pixelLibrary newFunctionWithName:[NSString stringWithUTF8String:data.PixelShader.Function.c_str()]];

        if (!vertexFunction)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal shader compilation error\n"
                                                                    "specified vertex shader entry does not exist");
        }

        if (!pixelFunction)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal shader compilation error\n"
                                                                    "specified pixel shader entry does not exist");
        }

        MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

        pipelineDescriptor.vertexFunction = vertexFunction;
        pipelineDescriptor.fragmentFunction = pixelFunction;

        switch (data.Topology)
        {
            case Resources::PolygonTopology::TriangleList:
                info.Primitive = MTLPrimitiveTypeTriangle;
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;
                break;
            case Resources::PolygonTopology::LineList:
                info.Primitive = MTLPrimitiveTypeLine;
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassLine;
                break;
            case Resources::PolygonTopology::Point:
                info.Primitive = MTLPrimitiveTypePoint;
                pipelineDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassPoint;
                break;
        }

        pipelineDescriptor.vertexDescriptor = [mInternals->BuildVertexDescriptor(data) retain];
        pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
        pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
        pipelineDescriptor.rasterizationEnabled = YES;

        switch (data.CullFace)
        {
            case Resources::PolygonCullFace::Front:
                info.CullMode = MTLCullModeFront;
                break;
            case Resources::PolygonCullFace::Back:
                info.CullMode = MTLCullModeBack;
                break;
            case Resources::PolygonCullFace::None:
                info.CullMode = MTLCullModeNone;
                break;
        }

        switch (data.FrontFace)
        {
            case Resources::PolygonFrontFace::Clockwise:
                info.CullWinding = MTLWindingClockwise;
                break;
            case Resources::PolygonFrontFace::Anticlockwise:
                info.CullWinding = MTLWindingCounterClockwise;
                break;
        }

        switch (data.DepthComparison)
        {
            case Resources::ComparisonOperation::Always:
                info.DepthCompare = MTLCompareFunctionAlways;
                break;
            case Resources::ComparisonOperation::Equal:
                info.DepthCompare = MTLCompareFunctionEqual;
                break;
            case Resources::ComparisonOperation::Greater:
                info.DepthCompare = MTLCompareFunctionGreater;
                break;
            case Resources::ComparisonOperation::GreaterEqual:
                info.DepthCompare = MTLCompareFunctionGreaterEqual;
                break;
            case Resources::ComparisonOperation::LessEqual:
                info.DepthCompare = MTLCompareFunctionLessEqual;
                break;
            case Resources::ComparisonOperation::Less:
                info.DepthCompare = MTLCompareFunctionLess;
                break;
            case Resources::ComparisonOperation::NotEqual:
                info.DepthCompare = MTLCompareFunctionNotEqual;
                break;
            case Resources::ComparisonOperation::Never:
                info.DepthCompare = MTLCompareFunctionNever;
                break;
        }

        switch (data.FillMode)
        {
            case Resources::PolygonFillMode::Solid:
                info.FillMode = MTLTriangleFillModeFill;
                break;
            case Resources::PolygonFillMode::Line:
                info.FillMode = MTLTriangleFillModeLines;
                break;
            case Resources::PolygonFillMode::Point:
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                        "Metal pipeline creation error\n"
                                                                        "Metal does not support point fill mode");
        }

        NSError* pipelineError = nil;

        info.State = [mInternals->Device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&pipelineError];

        if (!info.State)
        {
            std::string errorStr = [[pipelineError localizedDescription] UTF8String];

            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreatePipeline(const Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal pipeline creation error\n"
                                                                    "Metal error info log:\n\n{}",
                                   errorStr.data());
        }

        MTLDepthStencilDescriptor* depthStencilDescriptor = [[MTLDepthStencilDescriptor alloc] init];

        depthStencilDescriptor.depthCompareFunction = info.DepthCompare;
        depthStencilDescriptor.depthWriteEnabled = data.DepthWrite;

        info.DepthStencilState = [mInternals->Device newDepthStencilStateWithDescriptor:depthStencilDescriptor];
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeletePipeline(Resources::RasterPipelineData& data)
    {
        auto* info = static_cast<MetalPipelineData*>(data.BackendMetadata);

        [info->State release];

        delete info;

        data.BackendMetadata = nullptr;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreateSubmission(Resources::SubmissionData& data)
    {
        data.BackendMetadata = new MetalSubmissionData();

        auto& info = *static_cast<MetalSubmissionData*>(data.BackendMetadata);
        auto& indexBufferData = mBuffers.Data.Get(data.IndexBuffer.ID);

        switch (data.IndexFormat)
        {
            case Resources::IndexType::U16:
                info.IndexType = MTLIndexTypeUInt16;
                info.IndexCount = indexBufferData.Size / 2;
                break;
            case Resources::IndexType::U32:
                info.IndexType = MTLIndexTypeUInt32;
                info.IndexCount = indexBufferData.Size / 4;
                break;
        }
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeleteSubmission(Resources::SubmissionData& data)
    {
        auto* info = static_cast<MetalSubmissionData*>(data.BackendMetadata);

        delete info;

        data.BackendMetadata = nullptr;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreateRenderQueue(Resources::RenderQueueData& data)
    {
        data.BackendMetadata = new MetalRenderQueueData();

        auto& renderPassData = mBuffers.Data.Get(data.RenderPass.ID);
        auto& info = *static_cast<MetalRenderQueueData*>(data.BackendMetadata);
        auto& renderPassInfo = *static_cast<MetalRenderPassData*>(renderPassData.BackendMetadata);

        info.Buffer = [mInternals->CommandQueue commandBuffer];
        info.Encoder = [info.Buffer renderCommandEncoderWithDescriptor:renderPassInfo.RenderPass];
    }

    void RendererBackendImplementation<RendererBackend::Metal>::SubmitSubmission(Resources::RenderQueueData& data, Resources::SubmissionData& submissionData)
    {
        auto& info = *static_cast<MetalRenderQueueData*>(data.BackendMetadata);
        auto& submissionInfo = *static_cast<MetalSubmissionData*>(submissionData.BackendMetadata);

        auto& pipelineData = mRasterPipelines.Data.Get(submissionData.Pipeline.ID);
        auto& pipelineInfo = *static_cast<MetalPipelineData*>(pipelineData.BackendMetadata);

        [info.Encoder setRenderPipelineState:pipelineInfo.State];

        for (NSUInteger i = 0; i < pipelineInfo.VertexBufferBindings.size(); i++)
        {
            auto& bufferData = mBuffers.Data.Get(submissionData.VertexBuffers[i].ID);
            auto& bufferInfo = *static_cast<MetalBufferData*>(bufferData.BackendMetadata);

            [info.Encoder setVertexBuffer:bufferInfo.Handle offset:0 atIndex:pipelineInfo.VertexBufferBindings[i]];
        }

        auto& indexBufferData = mBuffers.Data.Get(submissionData.IndexBuffer.ID);
        auto& indexBufferInfo = *static_cast<MetalBufferData*>(indexBufferData.BackendMetadata);

        [info.Encoder drawIndexedPrimitives:pipelineInfo.Primitive
                                 indexCount:submissionInfo.IndexCount
                                  indexType:submissionInfo.IndexType
                                indexBuffer:indexBufferInfo.Handle
                          indexBufferOffset:0];
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CommitQueue(Resources::RenderQueueData& data)
    {
        auto& info = *static_cast<MetalRenderQueueData*>(data.BackendMetadata);

        [info.Encoder endEncoding];
        [info.Buffer commit];
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeleteQueue(Resources::RenderQueueData& data)
    {
        auto* info = static_cast<MetalRenderQueueData*>(data.BackendMetadata);

        [info->Encoder endEncoding];
        [info->Encoder release];
        [info->Buffer release];

        delete info;

        data.BackendMetadata = nullptr;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::CreateRenderPass(Resources::RenderPassData& data)
    {
        data.BackendMetadata = new MetalRenderPassData();

        auto& info = *static_cast<MetalRenderPassData*>(data.BackendMetadata);

        info.RenderPass = [MTLRenderPassDescriptor renderPassDescriptor];

        for (std::size_t i = 0; i < data.ColourAttachments.size(); i++)
        {
            auto& colourAttachment = data.ColourAttachments[i];

            MTLTextureDescriptor* textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                                                                         width:mInternals->DrawableWidth
                                                                                                        height:mInternals->DrawableHeight
                                                                                                     mipmapped:NO];
            textureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;

            auto clearColour = MTLClearColor(colourAttachment.Clear.r, colourAttachment.Clear.g, colourAttachment.Clear.b, colourAttachment.Clear.a);

            MTLLoadAction loadAction;
            MTLStoreAction storeAction;

            switch (colourAttachment.Load)
            {
                case Resources::LoadOperation::Load:
                    loadAction = MTLLoadActionLoad;
                    break;
                case Resources::LoadOperation::Clear:
                    loadAction = MTLLoadActionClear;
                    break;
                case Resources::LoadOperation::DontCare:
                    loadAction = MTLLoadActionDontCare;
                    break;
            }

            switch (colourAttachment.Store)
            {
                case Resources::StoreOperation::Store:
                    storeAction = MTLStoreActionStore;
                    break;
                case Resources::StoreOperation::DontCare:
                    storeAction = MTLStoreActionDontCare;
                    break;
            }

            info.RenderPass.colorAttachments[i].clearColor = clearColour;
            info.RenderPass.colorAttachments[i].loadAction = loadAction;
            info.RenderPass.colorAttachments[i].texture = [mInternals->Device newTextureWithDescriptor:textureDescriptor];
        }

        MTLLoadAction depthLoadAction;
        MTLStoreAction depthStoreAction;
        MTLLoadAction stencilLoadAction;
        MTLStoreAction stencilStoreAction;

        switch (data.DepthAttachment.Load)
        {
            case Resources::LoadOperation::Load:
                depthLoadAction = MTLLoadActionLoad;
                break;
            case Resources::LoadOperation::Clear:
                depthLoadAction = MTLLoadActionClear;
                break;
            case Resources::LoadOperation::DontCare:
                depthLoadAction = MTLLoadActionDontCare;
                break;
        }

        switch (data.DepthAttachment.Store)
        {
            case Resources::StoreOperation::Store:
                depthStoreAction = MTLStoreActionStore;
                break;
            case Resources::StoreOperation::DontCare:
                depthStoreAction = MTLStoreActionDontCare;
                break;
        }

        switch (data.StencilAttachment.Load)
        {
            case Resources::LoadOperation::Load:
                stencilLoadAction = MTLLoadActionLoad;
                break;
            case Resources::LoadOperation::Clear:
                stencilLoadAction = MTLLoadActionClear;
                break;
            case Resources::LoadOperation::DontCare:
                stencilLoadAction = MTLLoadActionDontCare;
                break;
        }

        switch (data.StencilAttachment.Store)
        {
            case Resources::StoreOperation::Store:
                stencilStoreAction = MTLStoreActionStore;
                break;
            case Resources::StoreOperation::DontCare:
                stencilStoreAction = MTLStoreActionDontCare;
                break;
        }

        MTLTextureDescriptor* depthStencilDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float_Stencil8
                                                                                                          width:mInternals->DrawableWidth
                                                                                                         height:mInternals->DrawableHeight
                                                                                                      mipmapped:NO];

        depthStencilDescriptor.usage = MTLTextureUsageRenderTarget;
        depthStencilDescriptor.storageMode = MTLStorageModePrivate;

        id<MTLTexture> depthStencilTexture = [mInternals->Device newTextureWithDescriptor:depthStencilDescriptor];

        info.RenderPass.depthAttachment.texture = depthStencilTexture;
        info.RenderPass.depthAttachment.loadAction = depthLoadAction;
        info.RenderPass.depthAttachment.storeAction = depthStoreAction;
        info.RenderPass.depthAttachment.clearDepth = data.DepthAttachment.Clear;

        info.RenderPass.stencilAttachment.texture = depthStencilTexture;
        info.RenderPass.stencilAttachment.loadAction = stencilLoadAction;
        info.RenderPass.stencilAttachment.storeAction = stencilStoreAction;
        info.RenderPass.stencilAttachment.clearStencil = data.StencilAttachment.Clear;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::DeleteRenderPass(Resources::RenderPassData& data)
    {
        auto* info = static_cast<MetalRenderPassData*>(data.BackendMetadata);

        delete info;

        data.BackendMetadata = nullptr;
    }

    void RendererBackendImplementation<RendererBackend::Metal>::SetVSyncMode(RendererVSyncMode vsyncMode)
    {
        switch (vsyncMode)
        {
            case RendererVSyncMode::Disabled:
                mInternals->MetalLayer.displaySyncEnabled = NO;
                break;
            case RendererVSyncMode::Enabled:
                mInternals->MetalLayer.displaySyncEnabled = YES;
                break;
            case RendererVSyncMode::AllowLate:
                throw Debug::Exception(Debug::ErrorCode::CONFLICT, "Resources::PipelineHandle Systems::Renderer::Renderer(Systems::RendererDescriptor&):\n"
                                                                   "Metal VSync support conflict\n"
                                                                   "Metal does not support 'RendererVSyncMode::AllowLate'\n"
                                                                   "Use another supported VSync mode");
        }
    }
}
#endif