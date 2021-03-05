#pragma once

#include<vector>

namespace plxVulkan{
    class extensions{
    public:
        static std::vector<const char*> GetRequiredInstanceExtensions();
        static std::vector<const char*> GetRequiredDeviceExtensions();
    };
}