#include <vulkan/vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstring>

#include "validation.hpp"

validation::validation(const VkInstance* pInstance){
    instance = pInstance;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.pNext = NULL;
    debugCreateInfo.flags = 0;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = DebugCallback;

    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        if(func(*pInstance, &debugCreateInfo, NULL, &debugMessenger) != VK_SUCCESS){
            std::runtime_error{ "Failed to set up debug messenger" };
        }
    } else {
        std::runtime_error{ "Failed to set up debug messenger" };
    }
}

VKAPI_PTR VkBool32 validation::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData){
    std::cout << "Severity: " << messageSeverity << "\nType: " << messageTypes 
        << "\nMessage: " << pCallbackData->pMessage << '\n' << pUserData;

    return VK_FALSE;
}

void validation::CleanUp(){
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(*instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(*instance, debugMessenger, NULL);
    }
}

bool validation::CheckLayerSupport(){
    uint32_t layerCount{ 0 };
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    std::vector<VkLayerProperties> layers{ layerCount };
    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    std::size_t layersFound{ 0 };
    for(const VkLayerProperties& layer : layers){
        for(const auto& otherLayer : layers::layersToEnable){
            if(strcmp(otherLayer, layer.layerName) == 0){
                ++layersFound;
            }
        }
        
        if(layersFound == layers::layersToEnable.size()){
            return true;
        }
    }

    return false;
}