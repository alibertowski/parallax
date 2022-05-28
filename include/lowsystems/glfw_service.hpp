#pragma once

#define GLFW_INCLUDE_NONE // TODO: Might need to be redone
#include <GLFW/glfw3.h>

namespace GLFW {
    GLFWwindow* get_primary_window();
    void set_primary_window(GLFWwindow* w);
}
