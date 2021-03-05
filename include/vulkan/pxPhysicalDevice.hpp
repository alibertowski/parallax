#pragma once

#include <vulkan/vulkan.h>

namespace plxVulkan{
    class pxPhysicalDevice{
    public:
        pxPhysicalDevice() = default;
        pxPhysicalDevice(VkPhysicalDevice pPhysicalDevice);

        const inline VkPhysicalDevice& GetPhysicalDevice() const { return physicalDevice; }
    private:
        VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
    };
}