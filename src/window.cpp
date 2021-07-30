#include "window.hpp"
#include "settings.hpp"
#include "gl_glfw.hpp"
#include "engine.hpp"

#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp> 

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


static void key_callback(GLFWwindow*, int key, int, int action, int) {
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

float lastX{ Settings::DefaultWidth / 2 };
float lastY{ Settings::DefaultHeight / 2} ;
float yaw{ -90.0f };
float pitch{ 0.0f };
static void cursor_pos_callback(GLFWwindow*, double xpos, double ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cam::front = glm::normalize(direction);
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

    window = glfwCreateWindow(Settings::DefaultWidth, Settings::DefaultHeight, Settings::EngineName, nullptr, nullptr);
    if(!window){
        glfwTerminate();
        throw std::runtime_error("GLFW failed to create window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("GLAD failed to initialize");
    }
}

Window::~Window() {
    #ifndef NDEBUG
    std::cout << "Cleaning GLFW" << std::endl;
    #endif

    glfwDestroyWindow(window);
    glfwTerminate();
}
