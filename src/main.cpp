#include "window.hpp"
#include "vulkan/renderer.hpp"

int main() {
    window w{};
    plxVulkan::renderer r{ &w };
    w.window_init();
    r.renderer_init();

    r.cleanup();
    w.cleanup();
    return 0;
}