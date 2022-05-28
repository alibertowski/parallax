#include "controls.hpp"
#include "lowsystems/engine.hpp"
#include "lowsystems/glfw_service.hpp"

bool Input::IsControlPressed(Input::Control c) {
    auto window{ GLFW::get_primary_window() };

    switch (c) {
        case Input::Control::Up:
            return glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        case Input::Control::Down:
            return glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
        case Input::Control::Left:
            return glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
        case Input::Control::Right:
            return glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
        case Input::Control::Wireframe:
            return glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
        default:
        return false;
    }
}