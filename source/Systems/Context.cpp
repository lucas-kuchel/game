#include <Systems/Context.hpp>

#include <Debug/Exception.hpp>

#include <GLFW/glfw3.h>

namespace Systems
{
    Context::Context(const ContextDescriptor& descriptor)
        : mRenderer(descriptor.Renderer)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Context::Context():\n"
                                                                    "system initialisation error\n"
                                                                    "failed to initialise window API");
        }

        switch (mRenderer)
        {
            case RendererBackend::Metal:
#if !defined(PLATFORM_APPLE)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "Metal graphics API is only available on macOS\n"
                                                                           "change Systems::ContextDescriptor::Renderer to a supported API for this platform");
#endif
                break;
            case RendererBackend::Vulkan:
#if defined(PLATFORM_APPLE)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "Vulkan graphics API is unavailable on macOS\n"
                                                                           "change Systems::ContextDescriptor::Renderer to Metal for macOS");
#endif
                break;
            case RendererBackend::DirectX12:
#if defined(PLATFORM_UNIX)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "Direct3D graphics API is unavailable on Unix-like operating systems\n"
                                                                           "change Systems::ContextDescriptor::Renderer to Metal for macOS or Vulkan for Linux");
#endif
                break;
        }
    }

    Context::~Context()
    {
        glfwTerminate();
    }

    void Context::Update()
    {
        glfwPollEvents();
    }

    template <>
    const RendererBackend& Context::Get<ContextAttribute::Renderer>() const
    {
        return mRenderer;
    }
}