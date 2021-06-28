#pragma once

#include "window.hpp"
#include "vulkan/renderer.hpp"

class Engine {
public:
    void run();
    void clean_up();
private:
    Window window_;
    parallax_vulkan::renderer renderer_;
};
