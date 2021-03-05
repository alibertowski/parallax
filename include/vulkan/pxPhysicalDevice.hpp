#pragma once

#include <vulkan/vulkan.h>
#include <optional>

namespace plxVulkan{
    struct queueFamilyIndices{
        std::optional<uint32_t> graphicFamilyIndex;
        std::optional<uint32_t> presentFamilyIndex;
    };

    class pxPhysicalDevice{
    public:
        pxPhysicalDevice() = default;
        pxPhysicalDevice(VkPhysicalDevice pPhysicalDevice);

        inline queueFamilyIndices& GetQueueFamilyIndices() { return queueFamily; }
        const inline VkPhysicalDevice& GetPhysicalDevice() const { return physicalDevice; }
        inline bool IsValid() const {return queueFamily.graphicFamilyIndex.has_value() && queueFamily.presentFamilyIndex.has_value(); }
    private:
        VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
        queueFamilyIndices queueFamily;
    };
}