#pragma once

#include "window.hpp"
#include "camera.hpp"

class Engine {
public:
    void run();
private:
    Window window;
    Camera cam;
};
// TODO: Possible have constructor create all the necessarry things in the services
