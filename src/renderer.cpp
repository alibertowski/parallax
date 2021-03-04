#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "renderer.hpp"
#include "window.hpp"
#include "settings.hpp"
#include "pxPhysicalDevices.hpp"
#include "validation.hpp"

renderer::renderer(window* pWindow){
    windowInstance = pWindow;
}

void renderer::renderer_init(){
    initVulkan();
    mainLoop();
}

void renderer::initVulkan(){
    createInstance();
    SetupVulkanDebugger();
    pickPhysicalDevice();
    createLogicalDevice();
}

void renderer::cleanup(){
    if(layers::enableLayers){
        validator.CleanUp();
    }

    vkDestroyDevice(device, NULL);
    vkDestroyInstance(instance, NULL);
}

void renderer::SetupVulkanDebugger(){
    if (!layers::enableLayers) return;
    
    validator = validation{ &instance };
}

void renderer::createInstance(){
    if(layers::enableLayers && !validation::CheckLayerSupport()){
        throw std::runtime_error{ "Layers not supported" };
    }

    uint32_t extensionCount{ 0 };
    auto glfwExtensions { glfwGetRequiredInstanceExtensions(&extensionCount) };

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

    if(layers::enableLayers){
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.pNext = NULL;
        debugCreateInfo.flags = 0;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        debugCreateInfo.pfnUserCallback = validation::DebugCallback;

        createInfo.enabledLayerCount = static_cast<uint32_t>(layers::layersToEnable.size());
        createInfo.ppEnabledLayerNames = layers::layersToEnable.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
        createInfo.pNext = NULL;
    }

    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if(!vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS){
        std::runtime_error{ "Failed to create Vulkan instance" };
    }
}

void renderer::mainLoop(){
    std::cout << "Loop started\n";

    while (!glfwWindowShouldClose(windowInstance->GetWindow())) {
        glfwPollEvents();
    }
}

void renderer::pickPhysicalDevice(){
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

void renderer::createLogicalDevice(){
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