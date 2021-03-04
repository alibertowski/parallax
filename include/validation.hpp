#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace layers{
    #ifdef NDEBUG
    inline constexpr bool enableLayers = false;
    const std::vector<std::string> layersToEnable;
    #else
    inline constexpr bool enableLayers = true;
    const std::vector<const char*> layersToEnable { "VK_LAYER_KHRONOS_validation" };
    #endif
}

class validation{
public:
    validation() = default;
    validation(const VkInstance* pInstance);

    void CleanUp();
    static bool CheckLayerSupport();
    static VKAPI_PTR VkBool32 DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData);
private:
    VkDebugUtilsMessengerEXT debugMessenger;
    const VkInstance* instance;
};