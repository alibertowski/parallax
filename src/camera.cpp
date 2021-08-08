#include "camera.hpp"
#include "glfw_service.hpp"
#include "engine.hpp"
#include "mouse.hpp"
#include "window.hpp"
#include "time.hpp"

#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>

glm::mat4 Camera::calculate_view() {
    glm::vec3 position{ cframe.get_position() };
    glm::vec3 lookVector{ cframe.get_look_vector() };
    glm::vec3 rightVector{ cframe.get_right_vector() };
    glm::vec3 upVector{ cframe.get_up_vector() };

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = rightVector.x;
    rotation[1][0] = rightVector.y;
    rotation[2][0] = rightVector.z;
    rotation[0][1] = upVector.x;
    rotation[1][1] = upVector.y;
    rotation[2][1] = upVector.z;
    rotation[0][2] = lookVector.x;
    rotation[1][2] = lookVector.y;
    rotation[2][2] = lookVector.z;

    return rotation * translation;
}

glm::mat4 Camera::get_projection() {
    Window* window{ Engine::PrimaryEngine->get_window() };
    return glm::perspective(glm::radians(75.0f), (static_cast<float>(window->get_width()) / static_cast<float>(window->get_height())), 0.1f, 1000.0f);
}

void Camera::update() {
    Mouse* mouse{ Engine::PrimaryEngine->get_mouse() };
    int currentX{ mouse->get_x_pos() };
    int currentY{ mouse->get_y_pos() };

    float xoffset{ currentX - lastX };
    float yoffset{ lastY - currentY }; 
    lastX = currentX;
    lastY = currentY;

    const float sensitivity{ 10.0f * Time::get_delta_time()};
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cframe.set_look_vector(-direction);

    const float speed{ 10.0f * Time::get_delta_time() };
    if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 pos{ -cframe.get_look_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    } else if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 pos{ cframe.get_look_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    }
    
    if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 pos{ -cframe.get_right_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    } else if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 pos{ cframe.get_right_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    }

    if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_Q) == GLFW_PRESS) {
        glm::vec3 pos{ -cframe.get_up_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    } else if (glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_E) == GLFW_PRESS) {
        glm::vec3 pos{ cframe.get_up_vector() * speed };
        cframe.set_position(cframe.get_position() + pos);
    }

    // TODO: Make this changable through a graphic API wrapper instead of directly
    int fKeyAction{ glfwGetKey(GLFW::get_primary_window(), GLFW_KEY_F) };
    if (fKeyAction == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (fKeyAction == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
