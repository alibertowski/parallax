#include <cstdlib>

#include "lowsystems/engine.hpp"

int main() {
    Engine eng{};
    Engine::PrimaryEngine = &eng;
    eng.run();

    return EXIT_SUCCESS;
}
