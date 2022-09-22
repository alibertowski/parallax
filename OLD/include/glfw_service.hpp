#pragma once

#include "gl_glfw.hpp"

namespace GLFW {
    GLFWwindow* get_primary_window();
    void set_primary_window(GLFWwindow* w);
}
