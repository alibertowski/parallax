#include <cstdlib>
#include "engine.hpp"

int main() {
    engine eng;
    eng.run();
    eng.clean_up();
    
    return EXIT_SUCCESS;
}
