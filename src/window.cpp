#include "window.hpp"
#include "settings.hpp"
#include "glad/glad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

static void error_callback(int error, const char* description) {
    std::cerr << "Error Code: " << error << ", Description: " << description << '\n';
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout << "Key Pressed: " << key << '\n';
}

Window::Window() {
    #ifndef NDEBUG
    std::cout << glfwGetVersionString() << '\n';
    #endif

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()){   
        throw std::runtime_error{ "GLFW failed to initialize" };
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow(Settings::DefaultWidth, Settings::DefaultHeight, Settings::EngineName, NULL, NULL);
    if(!window){
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("GLAD failed to initialize");
    }

    while(!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        #ifndef NDEBUG
        // Gets the deltatime and prints the FPS
        double deltaTime{ glfwGetTime() };
        glfwSetTime(0.0);
      //  std::cout << 1 / deltaTime << '\n';
        #endif
    }
}

Window::~Window() {
    #ifndef NDEBUG
    std::cout << "Cleaning GLFW...\n";
    #endif

    glfwDestroyWindow(window);
    glfwTerminate();
}
