#include "window.hpp"
#include "settings.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>

static void error_callback(int error, const char* description){
    std::cerr << "Error Code: " << error << ", Description: " << description << '\n';
}

void window::window_init(){
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()){
        throw std::runtime_error{ "GLFW failed to initialize" };
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(640, 480, settings::name, NULL, NULL);
    if(!window){
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }
}

void window::cleanup(){
    std::cout << "Closing...\n";

    glfwDestroyWindow(window);
    glfwTerminate();
}