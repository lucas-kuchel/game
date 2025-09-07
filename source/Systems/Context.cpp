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