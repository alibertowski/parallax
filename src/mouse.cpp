#include "mouse.hpp"
#include "gl_glfw.hpp"
#include "glfw_service.hpp"

Mouse* Mouse::create_mouse() {
    return new GLFWMouse();
}

GLFWMouse::GLFWMouse() : window{ GLFW::get_primary_window() } {
}

int GLFWMouse::get_x_pos() const {
    double x;
    glfwGetCursorPos(window, &x, nullptr);
    return static_cast<int>(x);
}

int GLFWMouse::get_y_pos() const {
    double y;
    glfwGetCursorPos(window, nullptr, &y);
    return static_cast<int>(y);
}
