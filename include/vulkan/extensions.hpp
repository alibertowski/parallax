#pragma once

#include<vector>

namespace parallax_vulkan{
    class extensions{
    public:
        static std::vector<const char*> get_required_instance_extensions();
        static std::vector<const char*> get_required_device_extensions();
    };
}
