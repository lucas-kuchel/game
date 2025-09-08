#pragma once

#include <Systems/Context.hpp>
#include <Systems/Renderer.hpp>
#include <Systems/Window.hpp>

#include <Game/Instance.hpp>

namespace Program
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Update();

        bool ShouldUpdate() const;

    private:
        Systems::Context mContext;
        Systems::Window mWindow;
        Systems::Renderer mRenderer;

        Game::Instance mInstance;

        Systems::ContextDescriptor CreateContext();
        Systems::WindowDescriptor CreateWindow();
        Systems::RendererDescriptor CreateRenderer();
    };
}