#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "pxPhysicalDevice.hpp"

namespace parallax_vulkan{
    class pxPhysicalDevices{
    public:
        pxPhysicalDevices() = default;
        pxPhysicalDevices(const VkInstance* pInstance);

        inline std::vector<pxPhysicalDevice>& GetDevices() { return physicalDevices; }
        inline pxPhysicalDevice& GetPrimaryDevice() { return primaryDevice; }
        inline void SetPrimaryDevice(const pxPhysicalDevice& device) { primaryDevice = device; } 

    private:
        const VkInstance* instance;

        std::vector<pxPhysicalDevice> physicalDevices;
        pxPhysicalDevice primaryDevice;
    };    
}