#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

private:
    GLFWwindow* window;
};
