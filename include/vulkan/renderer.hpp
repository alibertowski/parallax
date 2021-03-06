#pragma once

#include <vulkan/vulkan.h>
#include "window.hpp"
#include "pxPhysicalDevices.hpp"
#include "layers.hpp"

namespace plxVulkan{
    class renderer{
    public:
        renderer(window* pWindow);

        void renderer_init();
        void cleanup();
    private:
        window* windowInstance;
        VkInstance instance;
        pxPhysicalDevices physicalDevices;
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;
        layers layer;

        void createInstance();
        void initVulkan();
        void mainLoop();
        void CreateSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void SetupVulkanDebugger();
        void CreateSwapchain();
    };
}