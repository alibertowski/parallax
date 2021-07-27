#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>

#include "vulkan/physical_devices.hpp"

parallax_vulkan::physical_devices::physical_devices(const VkInstance* pInstance){
    std::cout << "Instance: " << pInstance << '\n';
}
