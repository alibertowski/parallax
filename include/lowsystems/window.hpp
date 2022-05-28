#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "settings.hpp"

class Window {
public:
    virtual ~Window() = default;
    
    virtual int get_width() const = 0;
    virtual int get_height() const = 0;

    static Window* create_window();
};

class GLFWWindow : public Window {
public:
    GLFWWindow();
    virtual ~GLFWWindow() override;

    virtual inline int get_width() const override { return width; };
    virtual inline int get_height() const override { return height; };
    inline void set_width(int newWidth) { width = newWidth; };
    inline void set_height(int newHeight) { height = newHeight; };
private:
    int width{ Settings::DefaultWidth };
    int height{ Settings::DefaultHeight };
};
