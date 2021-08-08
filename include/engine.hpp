#pragma once

#include "window.hpp"
#include "camera.hpp"
#include "mouse.hpp"

#include <memory>

class Engine {
public:
    Engine();

    void run();
    inline Mouse* get_mouse() const { return mouse.get(); };
    inline Window* get_window() const { return window.get(); };

    static Engine* PrimaryEngine;
private:
    Camera cam;
    std::unique_ptr<Window> window{ Window::create_window() };
    std::unique_ptr<Mouse> mouse{ Mouse::create_mouse() };
};
