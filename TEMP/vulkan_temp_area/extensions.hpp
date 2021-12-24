#pragma once

#include<vector>

class Extensions{
public:
    static std::vector<const char*> get_required_instance_extensions();
    static std::vector<const char*> get_required_device_extensions();
};
