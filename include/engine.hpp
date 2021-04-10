#pragma once

#include "window.hpp"
#include "vulkan/renderer.hpp"

class engine{
public:
    void run();
    void clean_up();
private:
    window window_;
    parallax_vulkan::renderer renderer_;
};