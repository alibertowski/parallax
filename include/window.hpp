#pragma once

#include "gl_glfw.hpp"

class Window {
public:
    Window();
    ~Window();

    inline GLFWwindow* get_window() const { return window; };
private:
    GLFWwindow* window;
};
