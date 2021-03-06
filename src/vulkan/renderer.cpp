#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <set>

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
    CreateSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    CreateSwapchain();
}

void plxVulkan::renderer::cleanup(){
    vkDestroyDevice(device, NULL);
    if(enableDebugger){
        layer.CleanUp();
    }

    vkDestroySurfaceKHR(instance, surface, NULL);
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

void plxVulkan::renderer::CreateSurface(){
    if(glfwCreateWindowSurface(instance, windowInstance->GetWindow(), NULL, &surface) != VK_SUCCESS){
        std::runtime_error{ "Failed to create surface" };
    }
}

void plxVulkan::renderer::pickPhysicalDevice(){
    physicalDevices = pxPhysicalDevices{ &instance };
    std::vector<pxPhysicalDevice>& devices { physicalDevices.GetDevices() };

    for(pxPhysicalDevice& device : devices){ 
        uint32_t queueFamilycount { 0 };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, NULL);
        
        std::vector<VkQueueFamilyProperties2> queueFamilyProperties { queueFamilycount };
        for(auto& queue: queueFamilyProperties){
            queue.sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2;
        }

        uint32_t index{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties2(device.GetPhysicalDevice(), &queueFamilycount, queueFamilyProperties.data());
        for (const auto& queueFamily : queueFamilyProperties){
            if (queueFamily.queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT){
                device.GetQueueFamilyIndices().graphicFamilyIndex = index;
            }

            VkBool32 presentationSupport{ VK_FALSE };
            vkGetPhysicalDeviceSurfaceSupportKHR(device.GetPhysicalDevice(), index, surface, &presentationSupport);
            if(presentationSupport == VK_TRUE){
                device.GetQueueFamilyIndices().presentFamilyIndex = index;
            }

            if(device.IsValid()){
                physicalDevices.SetPrimaryDevice(device);
                break;
            }
            ++index;
        }

        if(device.IsValid()) break;
    }

    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    if (!primaryDevice.IsValid()) {
        throw std::runtime_error{ "Could not find a suitable GPU" };
    }
}

void plxVulkan::renderer::createLogicalDevice(){
    float queuePriority{ 1.0F };
    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    auto deviceExtensions{ extensions::GetRequiredDeviceExtensions() };
    std::vector<VkDeviceQueueCreateInfo> queueFamilies;
    std::set<uint32_t> uniqueFamilies{ primaryDevice.GetQueueFamilyIndices().graphicFamilyIndex.value(), primaryDevice.GetQueueFamilyIndices().presentFamilyIndex.value() };
    for(uint32_t queueIndex : uniqueFamilies){
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = NULL;
        queueCreateInfo.queueFamilyIndex = queueIndex;
        queueCreateInfo.queueCount = 1; // Possiby TODO
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueFamilies.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures{}; // Possibly TODO: Just enable stuff once engine is done
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.flags = 0;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueFamilies.size());
    deviceCreateInfo.pQueueCreateInfos = queueFamilies.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if(vkCreateDevice(physicalDevices.GetPrimaryDevice().GetPhysicalDevice(), &deviceCreateInfo, NULL, &device) != VK_SUCCESS){
        std::runtime_error{ "Logical device was unable to be created" };
    }

    VkDeviceQueueInfo2 graphicQueueInfo{};
    graphicQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    graphicQueueInfo.pNext = NULL;
    graphicQueueInfo.queueFamilyIndex = primaryDevice.GetQueueFamilyIndices().graphicFamilyIndex.value();
    graphicQueueInfo.queueIndex = 0; // Possibly TODO.

    VkDeviceQueueInfo2 presentQueueInfo{};
    presentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2;
    presentQueueInfo.pNext = NULL;
    presentQueueInfo.queueFamilyIndex = primaryDevice.GetQueueFamilyIndices().presentFamilyIndex.value();
    presentQueueInfo.queueIndex = 0; // Possibly TODO.

    vkGetDeviceQueue2(device, &graphicQueueInfo, &graphicsQueue);
    vkGetDeviceQueue2(device, &presentQueueInfo, &presentQueue);
}

void plxVulkan::renderer::CreateSwapchain(){
    auto primaryDevice{ physicalDevices.GetPrimaryDevice() };
    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(primaryDevice.GetPhysicalDevice(), surface, &capabilities);

    VkSurfaceFormatKHR format;
    uint32_t surfaceFormatCount{ 0 };
    vkGetPhysicalDeviceSurfaceFormatsKHR(primaryDevice.GetPhysicalDevice(), surface, &surfaceFormatCount, NULL);
    std::vector<VkSurfaceFormatKHR> surfaceFormats{ surfaceFormatCount };
    vkGetPhysicalDeviceSurfaceFormatsKHR(primaryDevice.GetPhysicalDevice(), surface, &surfaceFormatCount, surfaceFormats.data());
    bool selected{ false };
    for (const auto& availableFormat : surfaceFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            format = availableFormat;
            selected = true;
        }
    }
    if(!selected){
        format = surfaceFormats[0];
    }

    VkPresentModeKHR presentMode;
    uint32_t presentModeCount{ 0 };
    vkGetPhysicalDeviceSurfacePresentModesKHR(primaryDevice.GetPhysicalDevice(), surface, &presentModeCount, NULL);
    std::vector<VkPresentModeKHR> presentModes{ presentModeCount };
    vkGetPhysicalDeviceSurfacePresentModesKHR(primaryDevice.GetPhysicalDevice(), surface, &presentModeCount, presentModes.data());
    for(const auto& mode : presentModes){
        if(mode == VK_PRESENT_MODE_IMMEDIATE_KHR){
            presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
        }

        presentMode = VK_PRESENT_MODE_FIFO_KHR;
    }

    VkSwapchainCreateInfoKHR swapCreateInfo{};
    swapCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapCreateInfo.pNext = NULL;
    swapCreateInfo.flags = 0;
    swapCreateInfo.surface = surface;
    swapCreateInfo.minImageCount = capabilities.minImageCount; // TODO: Temporary solutioni
    swapCreateInfo.imageFormat = format.format;
    swapCreateInfo.imageColorSpace = format.colorSpace;
   // swapCreateInfo.imageExtent = 0;
    swapCreateInfo.presentMode = presentMode;

    if(vkCreateSwapchainKHR(device,  &swapCreateInfo, NULL, &swapchain) != VK_SUCCESS){
        throw std::runtime_error{ "Swapchain failed to be created" };
    }
}