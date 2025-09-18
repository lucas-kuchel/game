#include <Systems/Renderer.hpp>
#include <Systems/Renderer/Vulkan.hpp>

#include <Debug/Logging.hpp>

namespace Systems
{
    RendererBackendImplementation<RendererBackend::Vulkan>::RendererBackendImplementation(RendererBackendDescriptor& descriptor)
        : mSubmissions(descriptor.Submissions), mRasterPipelines(descriptor.RasterPipelines), mBuffers(descriptor.Buffers),
          mWindow(descriptor.Window), mVulkanWindowLayer(mWindow.CreateInteractionLayer<WindowInteractive::Vulkan>())
    {
        CreateInstance();
        SelectPhysicalDevice();
    }

    RendererBackendImplementation<RendererBackend::Vulkan>::~RendererBackendImplementation()
    {
        vkDestroyInstance(mInternals.Instance, nullptr);
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::CreateInstance()
    {
        std::uint32_t instanceExtensionCount = 0;

        const char** instanceExtensions = mVulkanWindowLayer.GetRequiredInstanceExtensions(&instanceExtensionCount);

        std::uint32_t vulkanVersion = 0;

        if (vkEnumerateInstanceVersion(&vulkanVersion) != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(RendererDescriptor&):\n"
                                                                    "Vulkan version query error\n"
                                                                    "failed to retrieve Vulkan API version");
        }

        VkApplicationInfo applicationInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "generic",
            .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
            .pEngineName = "generic",
            .engineVersion = VK_MAKE_VERSION(0, 0, 1),
            .apiVersion = vulkanVersion,
        };

        VkInstanceCreateInfo instanceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = instanceExtensionCount,
            .ppEnabledExtensionNames = instanceExtensions,
        };

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInternals.Instance) != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(RendererDescriptor&):\n"
                                                                    "Vulkan resource creation error\n"
                                                                    "failed to create Vulkan instance");
        }
    }

    void RendererBackendImplementation<RendererBackend::Vulkan>::SelectPhysicalDevice()
    {
        std::uint32_t physicalDeviceCount = 0;

        if (vkEnumeratePhysicalDevices(mInternals.Instance, &physicalDeviceCount, nullptr) != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(RendererDescriptor&):\n"
                                                                    "Vulkan physical device enumeration error\n"
                                                                    "failed to enumerate physical devices");
        }

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

        if (vkEnumeratePhysicalDevices(mInternals.Instance, &physicalDeviceCount, physicalDevices.data()) != VK_SUCCESS)
        {
            throw Debug::Exception(Debug::ErrorCode::GENERAL_ERROR, "Systems::Renderer::Renderer(RendererDescriptor&):\n"
                                                                    "Vulkan physical device enumeration error\n"
                                                                    "failed to enumerate physical devices");
        }

        auto scorePhysicalDevice = [](const VkPhysicalDeviceProperties& physicalDeviceProperties) -> std::uint32_t
        {
            int score = 0;

            if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                score += 1000;
            }
            else if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                score += 500;
            }
            else
            {
                score += 100;
            }

            score += physicalDeviceProperties.limits.maxImageDimension2D / 1024;

            uint32_t major = VK_VERSION_MAJOR(physicalDeviceProperties.apiVersion);
            uint32_t minor = VK_VERSION_MINOR(physicalDeviceProperties.apiVersion);

            score += major * 100 + minor * 10;

            return score;
        };

        for (VkPhysicalDevice& physicalDevice : physicalDevices)
        {

            if (!mInternals.PhysicalDevice)
            {
                mInternals.PhysicalDevice = physicalDevice;

                continue;
            }

            VkPhysicalDeviceProperties physicalDeviceProperties;
            VkPhysicalDeviceProperties currentPhysicalDeviceProperties;

            vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
            vkGetPhysicalDeviceProperties(mInternals.PhysicalDevice, &currentPhysicalDeviceProperties);

            if (scorePhysicalDevice(physicalDeviceProperties) > scorePhysicalDevice(currentPhysicalDeviceProperties))
            {
                mInternals.PhysicalDevice = physicalDevice;
            }
        }

        if (!mInternals.PhysicalDevice)
        {
            mInternals.PhysicalDevice = physicalDevices.front();
        }
    }
}
