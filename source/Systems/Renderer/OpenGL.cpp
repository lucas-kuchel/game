#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

namespace Systems
{
    RendererBackendImplementationSpecifics::RendererBackendImplementationSpecifics(const WindowInteractionLayer<WindowInteractive::OPENGL_LAYER>& layer, RendererWindow& window)
        : Window(window), Layer(layer)
    {
    }

    int RendererBackendImplementationSpecifics::GetSwapInterval(RendererVSyncMode mode)
    {
        switch (mode)
        {
            case RendererVSyncMode::DISABLED:
            {
                return 0;
            }
            case RendererVSyncMode::RELAXED:
            {
                return -1;
            }
            case RendererVSyncMode::STRICT:
            {
                return 1;
            }
        }

        throw Debug::Exception(Debug::ErrorCode::NOT_IMPLEMENTED, "[UNREACHABLE]");
    }

    OpenGLAttributeFormat RendererBackendImplementationSpecifics::GetGLAttributeFormat(Resources::BufferAttributeType type)
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

    std::size_t RendererBackendImplementationSpecifics::GetTypeSize(Resources::BufferAttributeType type)
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

    RendererBackendImplementation<RendererBackend::OPENGL>::RendererBackendImplementation(const RendererDescriptor& descriptor)
        : mSpecifics(CreateSpecifics(descriptor))
    {
        auto& layer = mSpecifics->Layer;
        auto& windowSize = mSpecifics->Window.Get<WindowAttribute::SIZE>();
        auto swapInterval = mSpecifics->GetSwapInterval(descriptor.VSyncMode);

        layer.MakeContextCurrent();
        layer.SetSwapInterval(swapInterval);

        if (glewInit() != GLEW_OK)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(const RendererDescriptor&):\n"
                                                                    "OpenGL extension loading error\n"
                                                                    "failed to load OpenGL extensions");
        }

        glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(descriptor.ClearColour[0], descriptor.ClearColour[1], descriptor.ClearColour[2], descriptor.ClearColour[3]);
    }

    RendererBackendImplementation<RendererBackend::OPENGL>::~RendererBackendImplementation()
    {
        delete mSpecifics;
    }

    void RendererBackendImplementation<RendererBackend::OPENGL>::Update()
    {
        auto& layer = mSpecifics->Layer;
        auto& windowSize = mSpecifics->Window.Get<WindowAttribute::SIZE>();

        layer.SwapBuffers();

        glViewport(0, 0, windowSize[0], windowSize[1]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OPENGL>::Set<RendererAttribute::VSYNC_MODE>(const RendererAttributeType<RendererAttribute::VSYNC_MODE>::Type& value)
    {
        auto& layer = mSpecifics->Layer;

        layer.SetSwapInterval(mSpecifics->GetSwapInterval(value));
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OPENGL>::Set<RendererAttribute::CLEAR_COLOUR>(const RendererAttributeType<RendererAttribute::CLEAR_COLOUR>::Type& value)
    {
        glClearColor(value[0], value[1], value[2], value[3]);
    }

    RendererBackendImplementationSpecifics* RendererBackendImplementation<RendererBackend::OPENGL>::CreateSpecifics(const RendererDescriptor& descriptor)
    {
        auto& window = descriptor.Window;

        return new RendererBackendImplementationSpecifics(window.CreateInteractionLayer<WindowInteractive::OPENGL_LAYER>(), window);
    }
}