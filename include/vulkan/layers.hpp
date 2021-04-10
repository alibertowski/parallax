#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace parallax_vulkan{
    class layers {
    public:
        layers() = default;
        layers(const VkInstance* p_instance);

        void layers_init();
        void clean_up();
        static std::vector<const char*> get_layers();
        static VkDebugUtilsMessengerCreateInfoEXT populate_debugger_messenger_create_info();
    private:
        VkDebugUtilsMessengerEXT debug_messenger_;
        const VkInstance* instance_;

        static VkBool32 VKAPI_PTR debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
            VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
            void* pUserData);
    };
}