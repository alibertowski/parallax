#include "window.hpp"
#include "settings.hpp"
#include "gl_glfw.hpp"
#include "glfw_service.hpp"

#include <stdexcept>
#include <iostream>

static void glfw_error_callback(int error, const char* description) {
    switch(error) {
        case GLFW_NO_ERROR:
            std::cout << "Error Type: GLFW_NO_ERROR" << std::endl;
            break;
        case GLFW_NOT_INITIALIZED:
            std::cout << "Error Type: GLFW_NOT_INITIALIZED" << std::endl;
            break;
        case GLFW_NO_CURRENT_CONTEXT:
            std::cout << "Error Type: GLFW_NO_CURRENT_CONTEXT" << std::endl;
            break;
        case GLFW_INVALID_ENUM:
            std::cout << "Error Type: GLFW_INVALID_ENUM" << std::endl;
            break;
        case GLFW_INVALID_VALUE:
            std::cout << "Error Type: GLFW_INVALID_VALUE" << std::endl;
            break;
        case GLFW_OUT_OF_MEMORY:
            std::cout << "Error Type: GLFW_OUT_OF_MEMORY" << std::endl;
            break;
        case GLFW_API_UNAVAILABLE:
            std::cout << "Error Type: GLFW_API_UNAVAILABLE" << std::endl;
            break;
        case GLFW_VERSION_UNAVAILABLE:
            std::cout << "Error Type: GLFW_VERSION_UNAVAILABLE" <<  std::endl;
            break;
        case GLFW_PLATFORM_ERROR:
            std::cout << "Error Type: GLFW_PLATFORM_ERROR" << std::endl;
            break;
        case GLFW_FORMAT_UNAVAILABLE:
            std::cout << "Error Type: GLFW_FORMAT_UNAVAILABLE" << std::endl;
            break;
        case GLFW_NO_WINDOW_CONTEXT:
            std::cout << "Error Type: GLFW_NO_WINDOW_CONTEXT" << std::endl;
            break;
    }

    std::cout << "Description: " << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);

    Window* currentWindow{ (Window*)glfwGetWindowUserPointer(w) };
    currentWindow->set_width(width);
    currentWindow->set_height(height);
}

Window::Window() {
    #ifndef NDEBUG
    std::cout << glfwGetVersionString() << std::endl;
    #endif

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()){   
        throw std::runtime_error{ "GLFW failed to initialize" };
    }

    #ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(Settings::DefaultWidth, Settings::DefaultHeight, Settings::EngineName, nullptr, nullptr);
    if(!window){
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("GLAD failed to initialize");
    }

    GLFW::set_primary_window(window);
}

Window::~Window() {
    #ifndef NDEBUG
    std::cout << "Cleaning GLFW" << std::endl;
    #endif

    glfwDestroyWindow(window);
    glfwTerminate();
}
