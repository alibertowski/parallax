#include "collision.hpp"

void Collision::set_position(const glm::vec3& newPosition) {
    position = newPosition;
    update_min_and_max();
}

void Collision::update_min_and_max() {
    min.x = (size.x / 2) - position.x;
    min.y = (size.y / 2) - position.y;
    min.z = (size.z / 2) - position.z;

    max.x = (size.x / 2) + position.x;
    max.y = (size.y / 2) + position.y;
    max.z = (size.z / 2) + position.z;
}
