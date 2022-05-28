#include <iostream>
#include <string>

#include "lowsystems/engine.hpp"
#include "opengl/renderer_gl.hpp"
#include "opengl/gl_glfw.hpp"
#include "lowsystems/glfw_service.hpp"
#include "time.hpp"
#include "controls.hpp"
#include "opengl/sprite_gl.hpp"
#include "systems/physics.hpp"

Engine* Engine::PrimaryEngine{ nullptr };

void update_systems() {
    Physics::update();
}

void Engine::run() {
    RendererGL r;
    double lastTimeValue{ 0.0 };
    GLFWwindow* primaryWindow{ GLFW::get_primary_window() };

    SpriteGL{ Image{ "dirt.jpg" }, glm::vec3{ 0.0f, 0.0f, 0.0f } };
    SpriteGL{ Image{ "dirt.jpg" }, glm::vec3{ 10.0f, 0.0f, 0.0f } };
    SpriteGL{ Image{ "dirt.jpg" }, glm::vec3{ 0.0f, 10.0f, 0.0f } };
    SpriteGL{ Image{ "dirt.jpg" }, glm::vec3{ 10.0f, 10.0f, 0.0f } };

    while(!glfwWindowShouldClose(primaryWindow)) {
        update_systems();
        r.render_frame();

        glfwSwapBuffers(primaryWindow);
        glfwPollEvents();

        if (Input::IsControlPressed(Input::Control::Wireframe)) {
            r.enable_wireframe(true);
        } else {
            r.enable_wireframe(false);
        }

        double timeValue{ glfwGetTime() };
        Time::set_delta_time(static_cast<float>(timeValue - lastTimeValue));
        lastTimeValue = timeValue;
    }
}
