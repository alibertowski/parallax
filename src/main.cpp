#include "engine.hpp"

#include <cstdlib>

int main() {
    Engine eng{};
    Engine::PrimaryEngine = &eng;
    eng.run();

    return EXIT_SUCCESS;
}
