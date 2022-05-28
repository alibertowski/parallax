#pragma once

#include "opengl/gl_glfw.hpp"

class Mouse {
public:
    virtual int get_x_pos() const = 0;
    virtual int get_y_pos() const = 0;

    static Mouse* create_mouse();
};

namespace {
    class GLFWMouse : public Mouse {
    public:
        GLFWMouse();

        inline virtual int get_x_pos() const override { return x; };
        inline virtual int get_y_pos() const override { return y; };

        inline void set_x_pos(const int newX) { x = newX; };
        inline void set_y_pos(const int newY) { y = newY; };
    private:
        int x{ 0 };
        int y{ 0 };
    };
}
