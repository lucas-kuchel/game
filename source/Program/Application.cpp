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

        auto& events = mWindow.QueryEvents();

        while (!events.empty())
        {
            auto& event = events.front();

            switch (event.Type)
            {
                case Systems::WindowEventType::WindowClose:
                    mRunning = false;
                default:
                    break;
            }

            events.pop();
        }

        mWindow.Update();

        mRenderer.Clear();
        mRenderer.Present();
    }

    bool Application::ShouldUpdate() const
    {
        return mRunning;
    }

    Systems::ContextDescriptor Application::CreateContext()
    {
        return {
            .Renderer = Systems::RendererBackend::Metal,
        };
    }

    Systems::WindowDescriptor Application::CreateWindow()
    {
        return {
            .Context = mContext,
            .Title = "New Window",
            .Size = {800u, 600u},
            .Shown = true,
            .Visibility = Systems::WindowVisibility::Windowed,
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