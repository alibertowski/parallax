#include "controls.hpp"
#include "engine.hpp"
#include "glfw_service.hpp"

bool Input::IsControlPressed(Input::Control c) {
    auto window{ GLFW::get_primary_window() };

    switch (c) {
        case Input::Control::Up:
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                return true;
            }

            return false;
        case Input::Control::Down:
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                return true;
            }

            return false;
        case Input::Control::Left:
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                return true;
            }

            return false;
        case Input::Control::Right:
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                return true;
            }

            return false;
        case Input::Control::Wireframe:
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                return true;
            }

            return false;
    }

    return false;
}