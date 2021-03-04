#include <vulkan/vulkan.h>
#include <stdexcept>

#include "pxPhysicalDevices.hpp"

pxPhysicalDevices::pxPhysicalDevices(const VkInstance* pInstance){
    instance = pInstance;

    uint32_t physcDeviceCount{ 0 };
    vkEnumeratePhysicalDevices(*instance, &physcDeviceCount, NULL);
    if(physcDeviceCount == 0){
        std::runtime_error{ "Failed to find a valid GPU with Vulkan support" };
    }

    std::vector<VkPhysicalDevice> devices{ physcDeviceCount };
    vkEnumeratePhysicalDevices(*instance, &physcDeviceCount, devices.data());

    physicalDevices = std::vector<pxPhysicalDevice>{};
    for (size_t i = 0; i < physcDeviceCount; i++)
    {
        physicalDevices.push_back(pxPhysicalDevice{ devices[i] });
    }
}