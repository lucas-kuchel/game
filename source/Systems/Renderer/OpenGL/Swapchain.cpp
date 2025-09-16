#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <GL/glew.h>

namespace Systems
{
    void RendererBackendImplementation<RendererBackend::OpenGL>::Clear()
    {
        auto& windowSize = mWindow.Get<WindowAttribute::Size>();

        glViewport(0, 0, windowSize.x, windowSize.y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void RendererBackendImplementation<RendererBackend::OpenGL>::Present()
    {
        mOpenGLWindowLayer.SwapBuffers();
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OpenGL>::Set<RendererAttribute::VSyncMode>(const RendererVSyncMode& vsyncMode)
    {
        GLint swapInterval = 0;

        switch (vsyncMode)
        {
            case RendererVSyncMode::Disabled:
            {
                swapInterval = 0;

                break;
            }
            case RendererVSyncMode::Enabled:
            {
                swapInterval = 1;

                break;
            }
            case RendererVSyncMode::AllowLate:
            {
                swapInterval = -1;

                break;
            }
        }

        mOpenGLWindowLayer.SetSwapInterval(swapInterval);
    }

    template <>
    void RendererBackendImplementation<RendererBackend::OpenGL>::Set<RendererAttribute::ClearColour>(const RendererClearColour& clearColour)
    {
        glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
    }
}
