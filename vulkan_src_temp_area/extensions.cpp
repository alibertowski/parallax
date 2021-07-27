#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include "vulkan/extensions.hpp"

#ifndef NDEBUG
const std::vector<const char*> required_debug_extensions{ "VK_EXT_debug_utils" };
#endif

const std::vector<const char*> required_device_extensions{ "VK_KHR_swapchain"};

std::vector<const char*> Extensions::get_required_instance_extensions(){
    uint32_t extension_count{ 0 };
    auto glfw_extensions { glfwGetRequiredInstanceExtensions(&extension_count) };
    std::vector<const char*> final_extensions{ glfw_extensions, glfw_extensions + extension_count };

    #ifndef NDEBUG
    for(const auto& extension : required_debug_extensions){
        final_extensions.push_back(extension);
    }
    #endif

    return final_extensions;
}

std::vector<const char*> Extensions::get_required_device_extensions(){
    return required_device_extensions;
}
