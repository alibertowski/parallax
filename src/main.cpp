#include "window.hpp"
#include "renderer.hpp"

int main() {
    window w{};
    renderer r{ &w };
    w.window_init();
    r.renderer_init();

    r.cleanup();
    w.cleanup();
    return 0;
}