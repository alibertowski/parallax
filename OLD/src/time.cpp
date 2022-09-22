#include "time.hpp"

static float delta_time{ 0.0f };

float Time::get_delta_time() {
    return delta_time;
}

void Time::set_delta_time(const float newDeltaTime) {
    delta_time = newDeltaTime;
}
