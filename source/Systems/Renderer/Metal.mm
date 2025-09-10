#include <Systems/Renderer.hpp>

#include <Debug/Logging.hpp>

#if defined(PLATFORM_APPLE)
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

namespace Systems
{
    struct MetalBufferData
    {
#if defined(PLATFORM_APPLE)
        id<MTLBuffer> Handle = nil;
#endif
    };

    struct MetalPipelineData
    {
    };

    struct MetalSubmissionData
    {
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

    void RendererBackendImplementation<RendererBackend::METAL>::CreatePipeline(const Resources::PipelineHandle&, const Resources::RasterPipelineDescriptor&)
    {
#if defined(PLATFORM_APPLE)

#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeletePipeline(const Resources::PipelineHandle&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::CreateSubmission(const Resources::SubmissionHandle&, const Resources::SubmissionDescriptor&)
    {
#if defined(PLATFORM_APPLE)
#endif
    }

    void RendererBackendImplementation<RendererBackend::METAL>::DeleteSubmission(const Resources::SubmissionHandle&)
    {
#if defined(PLATFORM_APPLE)
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

    void RendererBackendImplementation<RendererBackend::METAL>::DrawCommandBuffer(const CommandBuffer&)
    {
#if defined(PLATFORM_APPLE)
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
        id<CAMetalDrawable> drawable = [mSpecifics->MetalLayer nextDrawable];

        if (!drawable)
        {
            return;
        }

        MTLRenderPassDescriptor* passDesc = [MTLRenderPassDescriptor renderPassDescriptor];

        passDesc.colorAttachments[0].texture = drawable.texture;
        passDesc.colorAttachments[0].loadAction = MTLLoadActionClear;
        passDesc.colorAttachments[0].clearColor = MTLClearColorMake(colour.r, colour.g, colour.b, colour.a);
        passDesc.colorAttachments[0].storeAction = MTLStoreActionStore;

        id<MTLCommandBuffer> cmd = [mSpecifics->CommandQueue commandBuffer];
        id<MTLRenderCommandEncoder> encoder = [cmd renderCommandEncoderWithDescriptor:passDesc];

        [encoder endEncoding];
        [cmd presentDrawable:drawable];
        [cmd commit];
#endif
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::METAL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        return new RendererBackendImplementationSpecifics(descriptor.Window.CreateInteractionLayer<WindowInteractive::COCOA_LAYER>());
    }
}