#include <cstdlib>
#include "engine.hpp"

int main() {
    Engine eng;
    eng.run();
    eng.clean_up();

    return EXIT_SUCCESS;
}
