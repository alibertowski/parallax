#include "lowsystems/glfw_service.hpp"
#include "opengl/gl_glfw.hpp"

static GLFWwindow* window{ nullptr };

GLFWwindow* GLFW::get_primary_window() {
    return window;
}

void GLFW::set_primary_window(GLFWwindow* w) {
    window = w;
}