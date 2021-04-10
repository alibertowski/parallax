#pragma once

#include <GLFW/glfw3.h>

class window {
public:
    void window_init();
    void clean_up();
    inline GLFWwindow* get_window() { return window; }

private:
    GLFWwindow* window;
};