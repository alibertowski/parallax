#pragma once

#include <memory>

#include "lowsystems/window.hpp"
#include "lowsystems/mouse.hpp"

/* TODO:
    1. Think of a way to integrate components and its object (Collision easily updated when the object gets moved; Physics modifying object position)
    2. Folders
    3. Possibly have the components (collisions, physics) have a constructor that adds themselves to the right static service?
*/

class Engine {
public:
    void run();
    inline Mouse* get_mouse() const { return mouse.get(); };
    inline Window* get_window() const { return window.get(); };

    static Engine* PrimaryEngine;
private:
    std::unique_ptr<Window> window{ Window::create_window() };
    std::unique_ptr<Mouse> mouse{ Mouse::create_mouse() };
};
