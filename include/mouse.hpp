#pragma once

#include "gl_glfw.hpp"

class Mouse {
public:
    virtual int get_x_pos() const = 0;
    virtual int get_y_pos() const = 0;

    static Mouse* create_mouse();
};

namespace {
    class GLFWMouse : public Mouse { // TODO: For 3rd party things like this, try creating a class that gets all the dependencies for that library to decouple
    public:
        GLFWMouse();

        virtual int get_x_pos() const override;
        virtual int get_y_pos() const override;
    private:
        GLFWwindow* window;
    };
}
