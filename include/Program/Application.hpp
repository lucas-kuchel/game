#pragma once

#include <Renderer/Systems/Device.hpp>
#include <Renderer/Systems/Instance.hpp>
#include <Renderer/Systems/Swapchain.hpp>

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

        Renderer::Systems::Instance mInstance;
    };
}