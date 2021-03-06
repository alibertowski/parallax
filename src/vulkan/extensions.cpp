#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include "vulkan/extensions.hpp"

#ifndef NDEBUG
const std::vector<const char*> requiredDebugExtensions{ "VK_EXT_debug_utils" };
#endif

const std::vector<const char*> requiredDeviceExtensions{ "VK_KHR_swapchain"};

std::vector<const char*> plxVulkan::extensions::GetRequiredInstanceExtensions(){
    uint32_t extensionCount{ 0 };
    auto glfwExtensions { glfwGetRequiredInstanceExtensions(&extensionCount) };

    std::vector<const char*> finalExtensions{ glfwExtensions, glfwExtensions + extensionCount };
    #ifndef NDEBUG
    for(const auto& extension : requiredDebugExtensions){
        finalExtensions.push_back(extension);
    }
    #endif

    return finalExtensions;
}

std::vector<const char*> plxVulkan::extensions::GetRequiredDeviceExtensions(){
    return requiredDeviceExtensions;
}