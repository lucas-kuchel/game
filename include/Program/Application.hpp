#pragma once

#include <Systems/Registry.hpp>
#include <Systems/Renderer.hpp>
#include <Systems/Window.hpp>

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

        Systems::ContextDescriptor CreateContext();
        Systems::WindowDescriptor CreateWindow();
        Systems::RendererDescriptor CreateRenderer();
    };
}