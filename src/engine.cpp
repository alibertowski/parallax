#include "engine.hpp"
#include "renderer.hpp"
#include "gl_glfw.hpp"
#include "glfw_service.hpp"
#include "component_manager.hpp"
#include "time.hpp"

#include <iostream>
#include <string>

Engine* Engine::PrimaryEngine{ nullptr };

Engine::Engine() {
    cam.set_name(std::string{ "Camera" });
    ComponentManager::add_component(&cam);
}

void Engine::run() {
    Renderer r;
    double lastTimeValue{ 0.0 };
    GLFWwindow* primaryWindow{ GLFW::get_primary_window() };
    auto components{ ComponentManager::get_components() };

    while(!glfwWindowShouldClose(primaryWindow)) {
        glfwPollEvents();

        for(Component* comp : components) {
            comp->update();
        }

        r.render_frame();
        glfwSwapBuffers(primaryWindow);

        double timeValue{ glfwGetTime() };
        Time::set_delta_time(static_cast<float>(timeValue - lastTimeValue));
        lastTimeValue = timeValue;
        std::cout << "FPS: " << (1 / Time::get_delta_time()) << '\n';
    }
}
