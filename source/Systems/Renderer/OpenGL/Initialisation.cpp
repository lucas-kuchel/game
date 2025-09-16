#include <Systems/Renderer.hpp>
#include <Systems/Renderer/OpenGL.hpp>

#include <Debug/Logging.hpp>

#include <GL/glew.h>

namespace Systems
{
    RendererBackendImplementation<RendererBackend::OpenGL>::RendererBackendImplementation(const RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mOpenGLWindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::OpenGL>())
    {
        auto& windowSize = mWindow.Get<WindowAttribute::Size>();

        GLint swapInterval = 0;

        switch (descriptor.VSyncMode)
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

        mOpenGLWindowLayer.MakeContextCurrent();
        mOpenGLWindowLayer.SetSwapInterval(swapInterval);

        if (glewInit() != GLEW_OK)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(const RendererDescriptor&):\n"
                                                                    "OpenGL extension loading error\n"
                                                                    "failed to load OpenGL extensions");
        }

        glViewport(0, 0, windowSize[0], windowSize[1]);
        glClearColor(descriptor.ClearColour.r, descriptor.ClearColour.g, descriptor.ClearColour.b, descriptor.ClearColour.a);

        glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    }

    RendererBackendImplementation<RendererBackend::OpenGL>::~RendererBackendImplementation()
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
}
