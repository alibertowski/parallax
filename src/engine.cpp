#include "engine.hpp"
#include "renderer.hpp"
#include "gl_glfw.hpp"
#include "coordinate_frame.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Engine::run() {
    Renderer r;
    double lastTimeValue{ 0.0 };
    while(!glfwWindowShouldClose(window.get_window())) {
        double timeValue{ glfwGetTime() };
        glfwPollEvents();

        // Update Before Render
        cam.update();
        // End Update

        // RENDER UPDATE
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection{ glm::perspective(glm::radians(75.0f), (static_cast<float>(window.get_width()) / static_cast<float>(window.get_height())), 0.1f, 1000.0f) };
        glUniformMatrix4fv(glGetUniformLocation(r.get_shader_program(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for(int i{ 0 }; i < 16; ++i) {
            for(int j{ 0 }; j < 16; ++j) {
                for(int f{ 0 }; f < 100; ++f) {
                    glm::mat4 model{ glm::mat4{ 1.0f } }; // make sure to initialize matrix to identity matrix first
                    model = glm::translate(model, glm::vec3{ static_cast<float>(i) * 5.0f, static_cast<float>(f) * 5.0f, static_cast<float>(j) * 5.0f });
                    model = glm::scale(model, glm::vec3{ 5.0f, 5.0f, 5.0f });

                    GLint modelLoc{ glGetUniformLocation(r.get_shader_program(), "model") };
                    GLint viewLoc{ glGetUniformLocation(r.get_shader_program(), "view") };
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.calculate_view()));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        glfwSwapBuffers(window.get_window());
        // END RENDER UPDATE

     //   std::cout << "FPS: " << (1 / (timeValue - lastTimeValue)) << std::endl;
        lastTimeValue = timeValue;
    }
}
