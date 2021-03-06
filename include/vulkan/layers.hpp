#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace plxVulkan{
    #ifdef NDEBUG
    inline constexpr bool enableDebugger{ false };
    #else
    inline constexpr bool enableDebugger{ true };
    #endif

    class layers{
    public:
        layers() = default;
        layers(const VkInstance* pInstance);

        void Init();
        void CleanUp();
        static std::vector<const char*> GetLayers();
        static VkDebugUtilsMessengerCreateInfoEXT PopulateDebuggerMessengerCreateInfo();
    private:
        VkDebugUtilsMessengerEXT debugMessenger;
        const VkInstance* instance;

        static VkBool32 VKAPI_PTR DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
            VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
            void* pUserData);
    };
}