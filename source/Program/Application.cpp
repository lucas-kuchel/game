#include <Program/Application.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Program
{
    Application::Application()
        : mContext(CreateContext()), mWindow(CreateWindow()), mRenderer(CreateRenderer())
    {
    }

    Application::~Application()
    {
    }

    void Application::Update()
    {
        mContext.Update();
        mWindow.ManageEvents();
        mRenderer.Clear();
        mRenderer.Present();
    }

    bool Application::ShouldUpdate() const
    {
        return mWindow.Get<Systems::WindowAttribute::Status>() == Systems::WindowStatus::Active;
    }

    Systems::ContextDescriptor Application::CreateContext()
    {
        return {
            .Renderer = Systems::RendererBackend::OpenGL,
        };
    }

    Systems::WindowDescriptor Application::CreateWindow()
    {
        return {
            .Context = mContext,
            .Title = "New Window",
            .Size = {800u, 600u},
        };
    }

    Systems::RendererDescriptor Application::CreateRenderer()
    {
        return {
            .Context = mContext,
            .Window = mWindow,
            .ClearColour = {0.0f, 0.0f, 0.0f, 1.0f},
            .VSyncMode = Systems::RendererVSyncMode::Enabled,
        };
    }
}