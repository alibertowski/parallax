#include "engine.hpp"
#include "renderer.hpp"
#include "gl_glfw.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp> 
#include <cmath>
#include <iostream>

void Engine::run() {
    Renderer r;

    double lastTimeValue{ 0.0 };
    glm::vec3 camera { 0.0f, 0.0f, -3.0f };
    constexpr float speed{ 0.5f };
    while(!glfwWindowShouldClose(window.get_window())) {
        double timeValue{ glfwGetTime() };

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window.get_window(), GLFW_KEY_W) == GLFW_PRESS) {
            camera += speed * cam::front;
        } else if (glfwGetKey(window.get_window(), GLFW_KEY_S) == GLFW_PRESS) {
            camera += speed * -cam::front;
        } else if (glfwGetKey(window.get_window(), GLFW_KEY_A) == GLFW_PRESS) {
            camera += speed * glm::vec3(1.0f, 0.0f,  0.0f);
        } else if (glfwGetKey(window.get_window(), GLFW_KEY_D) == GLFW_PRESS) {
            camera += speed * glm::vec3(-1.0f, 0.0f,  0.0f);
        }

        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(75.0f), (static_cast<float>(window.get_width()) / static_cast<float>(window.get_height())), 0.1f, 1000.0f);
        glUniformMatrix4fv(glGetUniformLocation(r.get_shader_program(), "projection"), 1, GL_FALSE, &projection[0][0]);

        for(int i = 0; i < 16; ++i) {
            for(int j = 0; j < 16; ++j) {
                for(int f = 0; f < 100; ++f) {
                    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                    glm::mat4 view          = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(static_cast<float>(i) * 5.0f, static_cast<float>(f) * 5.0f, static_cast<float>(j) * 5.0f));
                    model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
                    view  = glm::lookAt(camera, camera + cam::front, glm::vec3(0.0f, 1.0f, 0.0f));

                    unsigned int modelLoc = glGetUniformLocation(r.get_shader_program(), "model");
                    unsigned int viewLoc  = glGetUniformLocation(r.get_shader_program(), "view");
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        glfwSwapBuffers(window.get_window());
        glfwPollEvents();

        std::cout << "FPS: " << (1 / (timeValue - lastTimeValue)) << std::endl;
        lastTimeValue = timeValue;
    }
}
