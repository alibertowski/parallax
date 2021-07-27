#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <array>
#include <string>
#include <cstring>

#include "vulkan/layers.hpp"

const std::array<const char*, 1> required_layers { "VK_LAYER_KHRONOS_validation" };

Layers::Layers(const VkInstance* instance){
    instance_ = p_instance;
}

void Layers::layers_init(){
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info { populate_debugger_messenger_create_info() };

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance_, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        if(func(*instance_, &debug_create_info, NULL, &debug_messenger_) != VK_SUCCESS){
            std::runtime_error{ "Failed to set up debug messenger" };
        }
    } else {
        std::runtime_error{ "Failed to set up debug messenger" };
    }
}

void Layers::clean_up(){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance_, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(*instance_, debug_messenger_, NULL);
    }
}

template <std::size_t size>
std::array<const char*, size> Layers::get_layers() {
    return required_layers;
}

VkDebugUtilsMessengerCreateInfoEXT Layers::populate_debugger_messenger_create_info(){
    VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
    debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_create_info.pNext = NULL;
    debug_create_info.flags = 0;
    debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = debug_callback;

    return debug_create_info;
}

VkBool32 VKAPI_PTR Layers::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
    std::cout << "Severity: " << messageSeverity << "\nType: " << messageTypes 
        << "\nMessage: " << pCallbackData->pMessage << "\nUser Data: " << pUserData << "\n\n";

    return VK_FALSE;
}
