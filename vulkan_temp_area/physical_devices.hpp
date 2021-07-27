#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace parallax_vulkan{
    class physical_devices{
    public:
        physical_devices() = default;
        physical_devices(const VkInstance* pInstance);
    private:
    };    
}
