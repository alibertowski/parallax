#pragma once

#include "window.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp> 

namespace cam {
    inline glm::vec3 front{ 0.0f, 0.0f,  1.0f };
}

class Engine {
public:
    void run();
private:
    Window window;
};
