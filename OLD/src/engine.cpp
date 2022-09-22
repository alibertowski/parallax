#include "engine.hpp"
#include "opengl/renderer_2d.hpp"
#include "gl_glfw.hpp"
#include "glfw_service.hpp"
#include "component_manager.hpp"
#include "time.hpp"
#include "collision.hpp"
#include "controls.hpp"

#include <iostream>
#include <string>

Engine* Engine::PrimaryEngine{ nullptr };

Engine::Engine() {
    cam.set_name(std::string{ "Camera" });
    ComponentManager::add_component(&cam);
}

void Engine::run() {
    Renderer2D r;
    double lastTimeValue{ 0.0 };
    GLFWwindow* primaryWindow{ GLFW::get_primary_window() };

    while(!glfwWindowShouldClose(primaryWindow)) {
        r.render_frame();
        glfwSwapBuffers(primaryWindow);
        glfwPollEvents();

        if (Input::IsControlPressed(Input::Control::Wireframe)) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        double timeValue{ glfwGetTime() };
        Time::set_delta_time(static_cast<float>(timeValue - lastTimeValue));
        lastTimeValue = timeValue;
    }
}
