#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstring>

#include "vulkan/layers.hpp"

const std::vector<const char*> requiredLayers { "VK_LAYER_KHRONOS_validation" };

plxVulkan::layers::layers(const VkInstance* pInstance){
    instance = pInstance;
}

VKAPI_PTR VkBool32 plxVulkan::layers::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
    std::cout << "Severity: " << messageSeverity << "\nType: " << messageTypes 
        << "\nMessage: " << pCallbackData->pMessage << "\nUser Data: " << pUserData << "\n\n";

    return VK_FALSE;
}

void plxVulkan::layers::CleanUp(){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(*instance, debugMessenger, NULL);
    }
}

void plxVulkan::layers::Init(){
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo { PopulateDebuggerMessengerCreateInfo() };

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        if(func(*instance, &debugCreateInfo, NULL, &debugMessenger) != VK_SUCCESS){
            std::runtime_error{ "Failed to set up debug messenger" };
        }
    } else {
        std::runtime_error{ "Failed to set up debug messenger" };
    }
}

std::vector<const char*> plxVulkan::layers::GetLayers(){
    return requiredLayers;
}

VkDebugUtilsMessengerCreateInfoEXT plxVulkan::layers::PopulateDebuggerMessengerCreateInfo(){
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.pNext = NULL;
    debugCreateInfo.flags = 0;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = DebugCallback;

    return debugCreateInfo;
}