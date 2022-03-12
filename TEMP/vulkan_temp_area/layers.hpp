#pragma once

#include <string>
#include <array>
#include <vulkan/vulkan.h>

class Layers {
public:
    Layers(const VkInstance* p_instance);

    void clean_up();
    template <std::size_t size>
    static std::array<const char*, size> get_layers();
    static VkDebugUtilsMessengerCreateInfoEXT populate_debugger_messenger_create_info();
private:
    VkDebugUtilsMessengerEXT m_debugMessenger;
    const VkInstance* m_instance;

    static VkBool32 VKAPI_PTR debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData);
};
