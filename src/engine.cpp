#include "engine.hpp"
#include "renderer.hpp"
#include "gl_glfw.hpp"

void Engine::run() {
    Renderer r;
    while(!glfwWindowShouldClose(window.get_window())) {
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window.get_window());
        glfwPollEvents();
    }
}
