#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

#include <filesystem>

#if defined(PLATFORM_APPLE)
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

namespace Systems
{
    static MTLVertexFormat ToMTLVertexFormat(Resources::BufferAttributeType type)
    {
        using T = Resources::BufferAttributeType;
        switch (type)
        {
            case T::R32_FLOAT:
                return MTLVertexFormatFloat;
            case T::R32G32_FLOAT:
                return MTLVertexFormatFloat2;
            case T::R32G32B32_FLOAT:
                return MTLVertexFormatFloat3;
            case T::R32G32B32A32_FLOAT:
                return MTLVertexFormatFloat4;

            case T::R8_INT:
                return MTLVertexFormatChar;
            case T::R8G8_INT:
                return MTLVertexFormatChar2;
            case T::R8G8B8_INT:
                return MTLVertexFormatInvalid; // Metal has no packed 3-byte, must pad to 4
            case T::R8G8B8A8_INT:
                return MTLVertexFormatChar4;

            case T::R16_INT:
                return MTLVertexFormatShort;
            case T::R16G16_INT:
                return MTLVertexFormatShort2;
            case T::R16G16B16_INT:
                return MTLVertexFormatInvalid; // same issue
            case T::R16G16B16A16_INT:
                return MTLVertexFormatShort4;

            case T::R32_INT:
                return MTLVertexFormatInt;
            case T::R32G32_INT:
                return MTLVertexFormatInt2;
            case T::R32G32B32_INT:
                return MTLVertexFormatInt3;
            case T::R32G32B32A32_INT:
                return MTLVertexFormatInt4;

            case T::R8_UINT:
                return MTLVertexFormatUChar;
            case T::R8G8_UINT:
                return MTLVertexFormatUChar2;
            case T::R8G8B8_UINT:
                return MTLVertexFormatInvalid;
            case T::R8G8B8A8_UINT:
                return MTLVertexFormatUChar4;

            case T::R16_UINT:
                return MTLVertexFormatUShort;
            case T::R16G16_UINT:
                return MTLVertexFormatUShort2;
            case T::R16G16B16_UINT:
                return MTLVertexFormatInvalid;
            case T::R16G16B16A16_UINT:
                return MTLVertexFormatUShort4;

            case T::R32_UINT:
                return MTLVertexFormatUInt;
            case T::R32G32_UINT:
                return MTLVertexFormatUInt2;
            case T::R32G32B32_UINT:
                return MTLVertexFormatUInt3;
            case T::R32G32B32A32_UINT:
                return MTLVertexFormatUInt4;

            // Normalized formats
            case T::R8_SNORM:
                return MTLVertexFormatCharNormalized;
            case T::R8G8_SNORM:
                return MTLVertexFormatChar2Normalized;
            case T::R8G8B8_SNORM:
                return MTLVertexFormatInvalid;
            case T::R8G8B8A8_SNORM:
                return MTLVertexFormatChar4Normalized;

            case T::R16_SNORM:
                return MTLVertexFormatShortNormalized;
            case T::R16G16_SNORM:
                return MTLVertexFormatShort2Normalized;
            case T::R16G16B16_SNORM:
                return MTLVertexFormatInvalid;
            case T::R16G16B16A16_SNORM:
                return MTLVertexFormatShort4Normalized;

            case T::R8_UNORM:
                return MTLVertexFormatUCharNormalized;
            case T::R8G8_UNORM:
                return MTLVertexFormatUChar2Normalized;
            case T::R8G8B8_UNORM:
                return MTLVertexFormatInvalid;
            case T::R8G8B8A8_UNORM:
                return MTLVertexFormatUChar4Normalized;

            case T::R16_UNORM:
                return MTLVertexFormatUShortNormalized;
            case T::R16G16_UNORM:
                return MTLVertexFormatUShort2Normalized;
            case T::R16G16B16_UNORM:
                return MTLVertexFormatInvalid;
            case T::R16G16B16A16_UNORM:
                return MTLVertexFormatUShort4Normalized;

            default:
                return MTLVertexFormatInvalid;
        }
    }

    MTLVertexDescriptor* BuildVertexDescriptor(const Resources::RasterPipelineDescriptor& desc)
    {
        MTLVertexDescriptor* vertexDesc = [[MTLVertexDescriptor alloc] init];

        NSUInteger attributeIndex = 0;

        for (const auto& buf : desc.VertexBufferFormats)
        {
            NSUInteger bufferIndex = buf.Index;
            NSUInteger offset = 0;

            for (auto attr : buf.Attributes)
            {
                MTLVertexFormat fmt = ToMTLVertexFormat(attr);
                if (fmt == MTLVertexFormatInvalid)
                {
                    NSLog(@"[Metal] Unsupported vertex format in buffer %lu", (unsigned long)bufferIndex);
                    continue;
                }

                vertexDesc.attributes[attributeIndex].format = fmt;
                vertexDesc.attributes[attributeIndex].offset = offset;
                vertexDesc.attributes[attributeIndex].bufferIndex = bufferIndex;

                // advance offset
                switch (fmt)
                {
                    case MTLVertexFormatFloat:
                    case MTLVertexFormatInt:
                        offset += 4;
                        break;
                    case MTLVertexFormatFloat2:
                    case MTLVertexFormatInt2:
                        offset += 8;
                        break;
                    case MTLVertexFormatFloat3:
                    case MTLVertexFormatInt3:
                        offset += 12;
                        break;
                    case MTLVertexFormatFloat4:
                    case MTLVertexFormatInt4:
                        offset += 16;
                        break;
                    default:
                        break;
                }

                attributeIndex++;
            }

            vertexDesc.layouts[bufferIndex].stride = offset;
            vertexDesc.layouts[bufferIndex].stepFunction = MTLVertexStepFunctionPerVertex;
            vertexDesc.layouts[bufferIndex].stepRate = 1;
        }

        return vertexDesc;
    }

    std::string GetMetalLibraryPath(const std::string_view& hlslPath)
    {
        std::filesystem::path baseOutDir = std::filesystem::path(hlslPath).replace_extension();
        std::filesystem::path metallib = baseOutDir / "msl" / "shaders.metallib";
        return metallib.string();
    }

    struct MetalBufferData
    {
#if defined(PLATFORM_APPLE)
        id<MTLBuffer> Handle = nil;
#endif
        Resources::BufferDescriptor Descriptor;
    };

    struct MetalPipelineData
    {
#if defined(PLATFORM_APPLE)
        id<MTLRenderPipelineState> State = nil;
        NSMutableArray* Libraries = nil;
#endif

        Resources::RasterPipelineDescriptor Descriptor;
    };

    struct MetalSubmissionData
    {
        Resources::SubmissionDescriptor Descriptor;
    };

    class RendererBackendImplementationSpecifics
    {
    public:
        RendererBackendImplementationSpecifics(const WindowInteractionLayer<WindowInteractive::COCOA_LAYER>& layer)
            : Layer(layer)
        {
        }

        WindowInteractionLayer<WindowInteractive::COCOA_LAYER> Layer;

        Types::SparseSet<MetalBufferData> Buffers;
        Types::SparseSet<MetalPipelineData> Pipelines;
        Types::SparseSet<MetalSubmissionData> Submissions;

        glm::fvec4 ClearColour;

#if defined(PLATFORM_APPLE)
        id<MTLDevice> Device = nil;
        id<MTLCommandQueue> CommandQueue = nil;
        CAMetalLayer* MetalLayer = nil;
#endif
    };

    RendererBackendImplementation<RendererBackend::METAL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
#if defined(PLATFORM_APPLE)
        NSWindow* window = static_cast<NSWindow*>(mSpecifics->Layer.GetCocoaWindow());

        mSpecifics->Device = MTLCreateSystemDefaultDevice();
        mSpecifics->CommandQueue = [mSpecifics->Device newCommandQueue];

        mSpecifics->MetalLayer = [CAMetalLayer layer];

        mSpecifics->MetalLayer.device = mSpecifics->Device;
        mSpecifics->MetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        mSpecifics->MetalLayer.framebufferOnly = YES;

        NSView* contentView = window.contentView;

        [contentView setWantsLayer:YES];
        [contentView setLayer:mSpecifics->MetalLayer];
#endif
    }

    RendererBackendImplementation<RendererBackend::METAL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::METAL>::Update()
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateBuffer(const Resources::BufferHandle& handle, const Resources::BufferDescriptor& descriptor)
    {
#if defined(PLATFORM_APPLE)
        auto& data = mSpecifics->Buffers.Insert(handle.ID, MetalBufferData());

        data.Handle = [mSpecifics->Device newBufferWithLength:descriptor.Size
                                                      options:MTLResourceStorageModeShared];

        data.Descriptor = descriptor;
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SetBufferData(const Resources::BufferHandle& handle, const Resources::BufferData& data)
    {
#if defined(PLATFORM_APPLE)
        auto& info = mSpecifics->Buffers.Get(handle.ID);

        std::uint8_t* memory = static_cast<std::uint8_t*>([info.Handle contents]);
        std::memcpy(memory + data.Offset, data.Data, data.Stride);
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteBuffer(const Resources::BufferHandle& handle)
    {
#if defined(PLATFORM_APPLE)
        mSpecifics->Buffers.Remove(handle.ID);
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreatePipeline(const Resources::PipelineHandle& handle, const Resources::RasterPipelineDescriptor& descriptor)
    {
#if defined(PLATFORM_APPLE)
        auto& info = mSpecifics->Pipelines.Insert(handle.ID, MetalPipelineData());

        info.Libraries = [[NSMutableArray alloc] init];
        info.Descriptor = descriptor;

        MTLRenderPipelineDescriptor* pipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];

        for (const auto& shaderDescriptor : descriptor.Shaders)
        {
            std::string metallibPath = GetMetalLibraryPath(shaderDescriptor.Path);

            NSError* error = nil;
            NSString* pathStr = [NSString stringWithUTF8String:metallibPath.c_str()];
            NSURL* url = [NSURL fileURLWithPath:pathStr];

            id<MTLLibrary> library = [mSpecifics->Device newLibraryWithURL:url error:&error];

            if (!library || error)
            {
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreateRasterPipeline(counst Resources::RasterPipelineDescriptor&):\n"
                                                                        "Metal library loading error\n"
                                                                        "failed to load Metal shader library (path = {})",
                                       shaderDescriptor.Path);
            }

            [info.Libraries addObject:library];

            NSString* entryName = [NSString stringWithUTF8String:shaderDescriptor.Function.data()];

            id<MTLFunction> function = [library newFunctionWithName:entryName];

            if (!function)
            {
                throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreateRasterPipeline(counst Resources::RasterPipelineDescriptor&):\n"
                                                                        "Metal function loading error\n"
                                                                        "failed to load Metal shader function (path = {}, function = {})",
                                       shaderDescriptor.Path, shaderDescriptor.Function);
            }

            switch (shaderDescriptor.Stage)
            {
                case Resources::ShaderStage::VERTEX:
                    pipelineDesc.vertexFunction = function;
                    break;
                case Resources::ShaderStage::PIXEL:
                    pipelineDesc.fragmentFunction = function;
                    break;
                case Resources::ShaderStage::GEOMETRY:
                    throw Debug::Exception(Debug::ErrorCode::NOT_IMPLEMENTED, "Resources::PipelineHandle Systems::Renderer::CreateRasterPipeline(counst Resources::RasterPipelineDescriptor&):\n"
                                                                              "Metal pipeline construction error\n"
                                                                              "Metal does not support geometry shaders (path = {})",
                                           shaderDescriptor.Path);
            }
        }

        pipelineDesc.vertexDescriptor = BuildVertexDescriptor(descriptor);

        pipelineDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

        NSError* pipelineError = nil;

        info.State = [mSpecifics->Device newRenderPipelineStateWithDescriptor:pipelineDesc error:&pipelineError];

        if (!info.State || pipelineError)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Resources::PipelineHandle Systems::Renderer::CreateRasterPipeline(counst Resources::RasterPipelineDescriptor&):\n"
                                                                    "Metal pipeline construction error\n"
                                                                    "Error creating pipeline state");
        }

        [info.Libraries removeAllObjects];
        info.Libraries = nil;
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeletePipeline(const Resources::PipelineHandle& handle)
    {
#if defined(PLATFORM_APPLE)
        mSpecifics->Pipelines.Remove(handle.ID);
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateSubmission(const Resources::SubmissionHandle& handle, const Resources::SubmissionDescriptor& descriptor)
    {
#if defined(PLATFORM_APPLE)
        auto& info = mSpecifics->Submissions.Insert(handle.ID, MetalSubmissionData());

        info.Descriptor = descriptor;
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteSubmission(const Resources::SubmissionHandle& handle)
    {
#if defined(PLATFORM_APPLE)
        mSpecifics->Submissions.Remove(handle.ID);
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateCommandBuffer(const CommandBuffer&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::SubmitToCommandBuffer(const CommandBuffer&, const Resources::SubmissionHandle&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer& buffer)
    {
#if defined(PLATFORM_APPLE)
        for (auto& submissionHandle : buffer.GetContents())
        {
            auto& submissionInfo = mSpecifics->Submissions.Get(submissionHandle.ID);
            auto& pipelineInfo = mSpecifics->Pipelines.Get(submissionInfo.Descriptor.Pipeline.ID);

            id<CAMetalDrawable> drawable = [mSpecifics->MetalLayer nextDrawable];

            if (!drawable)
            {
                return;
            }

            auto& colour = mSpecifics->ClearColour;

            id<MTLCommandBuffer> cmd = [mSpecifics->CommandQueue commandBuffer];
            MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];

            passDescriptor.colorAttachments[0].texture = drawable.texture;
            passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
            passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(colour.r, colour.g, colour.b, colour.a);
            passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

            id<MTLRenderCommandEncoder> encoder = [cmd renderCommandEncoderWithDescriptor:passDescriptor];

            [encoder setRenderPipelineState:pipelineInfo.State];

            for (std::size_t i = 0; i < submissionInfo.Descriptor.VertexBuffers.size(); i++)
            {
                auto handle = submissionInfo.Descriptor.VertexBuffers[i];
                auto& bufferInfo = mSpecifics->Buffers.Get(handle.ID);
                auto& format = pipelineInfo.Descriptor.VertexBufferFormats[i];

                [encoder setVertexBuffer:bufferInfo.Handle offset:0 atIndex:format.Index];
            }

            for (std::size_t i = 0; i < submissionInfo.Descriptor.DataBuffers.size(); i++)
            {
                auto handle = submissionInfo.Descriptor.DataBuffers[i];
                auto& bufferInfo = mSpecifics->Buffers.Get(handle.ID);
                auto& format = pipelineInfo.Descriptor.DataBufferFormats[i];

                [encoder setVertexBuffer:bufferInfo.Handle offset:0 atIndex:format.Index];
                [encoder setFragmentBuffer:bufferInfo.Handle offset:0 atIndex:format.Index];
            }

            auto& indexBufferInfo = mSpecifics->Buffers.Get(submissionInfo.Descriptor.IndexBuffer.ID);

            MTLIndexType indexType;
            std::size_t typeSize = 0;

            switch (submissionInfo.Descriptor.IndexBufferType)
            {
                case Resources::BufferAttributeType::R16_UINT:
                {
                    indexType = MTLIndexTypeUInt16;
                    typeSize = 2;

                    break;
                }
                case Resources::BufferAttributeType::R32_UINT:
                {
                    indexType = MTLIndexTypeUInt32;
                    typeSize = 4;

                    break;
                }
                default:
                {
                    break;
                }
            }

            NSUInteger indexCount = indexBufferInfo.Descriptor.Size / typeSize;
            NSUInteger indexOffset = 0;
            MTLPrimitiveType primitive;

            switch (pipelineInfo.Descriptor.RasterState.Primitive)
            {
                case Resources::PipelinePrimitive::TRIANGLE_LIST:
                {
                    primitive = MTLPrimitiveTypeTriangle;

                    break;
                }
                case Resources::PipelinePrimitive::LINE_LIST:
                {
                    primitive = MTLPrimitiveTypeLine;

                    break;
                }
                case Resources::PipelinePrimitive::POINT:
                {
                    primitive = MTLPrimitiveTypePoint;

                    break;
                }
            }

            if (indexCount > 0)
            {
                [encoder drawIndexedPrimitives:primitive
                                    indexCount:indexCount
                                     indexType:indexType
                                   indexBuffer:indexBufferInfo.Handle
                             indexBufferOffset:indexOffset];
            }

            [encoder endEncoding];
            [cmd presentDrawable:drawable];
            [cmd commit];
        }

#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteCommandBuffer(const CommandBuffer&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    template <>
    void RendererBackendImplementation<RendererBackend::METAL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& colour)
    {
#if defined(PLATFORM_APPLE)
        mSpecifics->ClearColour = colour;
#endif
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::METAL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window.CreateInteractionLayer<WindowInteractive::COCOA_LAYER>());
    }
}
