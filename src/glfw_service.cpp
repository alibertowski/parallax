#include "glfw_service.hpp"
#include "gl_glfw.hpp"

static GLFWwindow* window;

GLFWwindow* GLFW::get_primary_window() {
    return window;
}

void GLFW::set_primary_window(GLFWwindow* w) {
    window = w;
}