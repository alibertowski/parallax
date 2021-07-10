#include "engine.hpp"
#include "window.hpp"
#include "vulkan/renderer.hpp"
#include <iostream>

void Engine::run() {
    renderer_ = &window_;
    window_.window_init();
    renderer_.renderer_init();
}

void Engine::clean_up(){
    renderer_.clean_up();
    window_.clean_up();
}

Engine::~Engine() {
    std::cout << "Destructor" << std::endl;
}