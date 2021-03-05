#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "vulkan/renderer.hpp"
#include "window.hpp"
#include "settings.hpp"
#include "vulkan/pxPhysicalDevices.hpp"
#include "vulkan/layers.hpp"
#include "vulkan/extensions.hpp"

plxVulkan::renderer::renderer(window* pWindow){
    windowInstance = pWindow;
}

void plxVulkan::renderer::renderer_init(){
    initVulkan();
    mainLoop();
}

void plxVulkan::renderer::initVulkan(){
    createInstance();
    SetupVulkanDebugger();
    pickPhysicalDevice();
    createLogicalDevice();
}

void plxVulkan::renderer::cleanup(){
    if(enableDebugger){
        layer.CleanUp();
    }

    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);
}

void plxVulkan::renderer::SetupVulkanDebugger(){
    if (!enableDebugger) return;
    
    layer = layers{ &instance };
    layer.Init();
}

void plxVulkan::renderer::createInstance(){
    auto requiredLayers{ layers::GetLayers() };
    auto requiredExtensions{ extensions::GetRequiredInstanceExtensions() };

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = NULL;
    appInfo.pApplicationName = settings::name;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = settings::name;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;

    if(enableDebugger){
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{ layers::PopulateDebuggerMessengerCreateInfo() };
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
        createInfo.pNext = NULL;
    }

    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if(!vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS){
        std::runtime_error{ "Failed to create Vulkan instance" };
    }
}

void plxVulkan::renderer::mainLoop(){
    std::cout << "Loop started\n";

    while (!glfwWindowShouldClose(windowInstance->GetWindow())) {
        glfwPollEvents();
    }
}

void plxVulkan::renderer::pickPhysicalDevice(){
    physicalDevices = pxPhysicalDevices{ &instance };
    const std::vector<pxPhysicalDevice> devices { physicalDevices.GetDevices() };

    for(const pxPhysicalDevice& device : devices){ 
        uint32_t queueFamilycount { 0 };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, NULL);
        
        std::vector<VkQueueFamilyProperties2> queueFamilyProperties { queueFamilycount };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, queueFamilyProperties.data());
        for (const auto& queueFamily : queueFamilyProperties){
            if (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                physicalDevices.SetPrimaryDevice(device);
            }
        }
    }

   if (physicalDevices.GetPrimaryDevice().GetPhysicalDevice() == VK_NULL_HANDLE){
       throw std::runtime_error{ "Could not find a suitable GPU" };
   }
}

void plxVulkan::renderer::createLogicalDevice(){
    float queuePriority{ 1.0F };
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.pNext = NULL;
    queueCreateInfo.queueFamilyIndex = 0; // TODO
    queueCreateInfo.queueCount = 0; // TODO
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures physicalDeviceFeatures{}; // Possibly TODO: Just enable stuff once engine is done
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = 0; // TODO
    deviceCreateInfo.ppEnabledExtensionNames = 0; // TODO
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if(vkCreateDevice(physicalDevices.GetPrimaryDevice().GetPhysicalDevice(), &deviceCreateInfo, NULL, &device) != VK_SUCCESS){
        std::runtime_error{ "Logical device was unable to be created" };
    }

    VkDeviceQueueInfo2 queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    queueInfo.pNext = NULL;
    queueInfo.flags = 0; // TODO
    queueInfo.queueFamilyIndex = 0; // TODO
    queueInfo.queueIndex = 0; // TODO.

    vkGetDeviceQueue2(device, &queueInfo, &queue);
}