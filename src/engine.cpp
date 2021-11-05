#include "engine.hpp"
#include "renderer.hpp"
#include "gl_glfw.hpp"
#include "glfw_service.hpp"
#include "component_manager.hpp"
#include "time.hpp"
#include "collision.hpp"

#include <iostream>
#include <string>

Engine* Engine::PrimaryEngine{ nullptr };

Engine::Engine() {
    cam.set_name(std::string{ "Camera" });
    ComponentManager::add_component(&cam);
}

static bool Colliding(const Collision& a, const Collision& b) {
    glm::vec3 minVec{a.get_min()};
    glm::vec3 maxVec{a.get_max()};
    glm::vec3 min2Vec{b.get_min()};
    glm::vec3 max2Vec{b.get_max()};

    return maxVec.x >= min2Vec.x &&
            max2Vec.x >= minVec.x &&
            maxVec.y >= min2Vec.y &&
            max2Vec.y >= minVec.y &&
            maxVec.z >= min2Vec.z &&
            max2Vec.z >= minVec.z; // TODO: fix
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

//        cam.get_coordinate_frame().set_position(cam.get_coordinate_frame().get_position() - glm::vec3{0.0f, 2.0f * Time::get_delta_time(), 0.0f});
        std::cout << Colliding(Collision{ glm::vec3{16.0f, 1.0f, 10.0f} }, cam.get_collision()) << '\n';

        r.render_frame();
        glfwSwapBuffers(primaryWindow);

        double timeValue{ glfwGetTime() };
        Time::set_delta_time(static_cast<float>(timeValue - lastTimeValue));
        lastTimeValue = timeValue;
        std::cout << "FPS: " << (1 / Time::get_delta_time()) << '\n';
    }
}
