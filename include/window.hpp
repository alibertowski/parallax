#pragma once

#include <GLFW/glfw3.h>

class window{
public:
    void window_init();
    void cleanup();
    inline GLFWwindow* GetWindow() { return window; }

private:
    GLFWwindow* window;
};