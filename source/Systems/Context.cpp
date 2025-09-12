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
            case RendererBackend::METAL:
            {
#if !defined(PLATFORM_APPLE)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "Metal graphics API is only available on macOS\n"
                                                                           "change Systems::ContextDescriptor::Renderer to a supported API for this platform");
#endif
                break;
            }
            case RendererBackend::VULKAN:
            {
#if defined(PLATFORM_APPLE)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "Vulkan graphics API is unavailable on macOS\n"
                                                                           "change Systems::ContextDescriptor::Renderer to Metal for macOS");
#endif
                break;
            }
            case RendererBackend::OPENGL:
            {
#if defined(PLATFORM_APPLE)
                throw Debug::Exception(Debug::ErrorCode::INVALID_ARGUMENT, "Systems::Context::Context():\n"
                                                                           "\n"
                                                                           "OpenGL graphics API is unavailable on macOS\n"
                                                                           "change Systems::ContextDescriptor::Renderer to Metal for macOS");
#endif
                break;
            }
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
    const RendererBackend& Context::Get<ContextAttribute::RENDERER>() const
    {
        return mRenderer;
    }
}