#pragma once

#include "gl_glfw.hpp"
#include "settings.hpp"

class Window {
public:
    Window();
    ~Window();

    inline GLFWwindow* get_window() const { return window; };
    inline int get_width() const { return width; };
    inline int get_height() const { return height; };

    inline void set_width(int w) { width = w; };
    inline void set_height(int h) { height = h; }; 
private:
    GLFWwindow* window;
    int width{ Settings::DefaultWidth };
    int height{ Settings::DefaultHeight };
};
