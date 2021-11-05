#include "mouse.hpp"
#include "gl_glfw.hpp"
#include "engine.hpp"
#include "glfw_service.hpp"

Mouse* Mouse::create_mouse() {
    return new GLFWMouse{};
}

static void cursor_pos_callback(GLFWwindow*, double newX, double newY) {
    GLFWMouse* m{ static_cast<GLFWMouse*>(Engine::PrimaryEngine->get_mouse()) };
    m->set_x_pos(static_cast<int>(newX));
    m->set_y_pos(static_cast<int>(newY));
}

GLFWMouse::GLFWMouse() {
    glfwSetCursorPosCallback(GLFW::get_primary_window(), cursor_pos_callback);
}
