#pragma once
#include <glm/ext/vector_float3.hpp>
#include <vector>

struct PhysicsComponent {
public:
    std::vector<glm::vec3> velocity;
};
