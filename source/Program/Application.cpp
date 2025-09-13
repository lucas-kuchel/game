#include <Program/Application.hpp>

#include <Debug/Exception.hpp>
#include <Debug/Logging.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Program
{
    Application::Application()
        : mContext(CreateContext()), mWindow(CreateWindow()), mRenderer(CreateRenderer()), mInstance(mContext, mRenderer, mWindow)
    {
    }

    Application::~Application()
    {
    }

    void Application::Update()
    {
        mContext.Update();
        mWindow.Update();
        mRenderer.BeginFrame();
        mInstance.Update();
        mRenderer.EndFrame();
    }

    bool Application::ShouldUpdate() const
    {
        return mWindow.Get<Systems::WindowAttribute::STATUS>() == Systems::WindowStatus::ACTIVE;
    }

    Systems::ContextDescriptor Application::CreateContext()
    {
        return {
            .Renderer = Systems::RendererBackend::METAL,
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
            .VSyncMode = Systems::RendererVSyncMode::STRICT,
        };
    }
}