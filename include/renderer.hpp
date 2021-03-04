#pragma once

#include <vulkan/vulkan.h>
#include "window.hpp"
#include "pxPhysicalDevices.hpp"
#include "validation.hpp"

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
    VkQueue queue;
    validation validator;

    void createInstance();
    void initVulkan();
    void mainLoop();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void SetupVulkanDebugger();
};