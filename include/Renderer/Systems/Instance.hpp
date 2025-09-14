#pragma once

#include <vector>

namespace Renderer::Systems
{
    struct PhysicalDevice
    {
    };

    class InstanceBackend
    {
    };

    class Instance
    {
    public:
        Instance();
        ~Instance();

        std::vector<PhysicalDevice> QueryPhysicalDevices();
    };
}