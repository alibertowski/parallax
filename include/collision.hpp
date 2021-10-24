#pragma once

#include <glm/ext/vector_float3.hpp>

class Collision {
public:
    inline Collision(glm::vec3 newSize) : size{ newSize } { update_min_and_max(); };

    inline const glm::vec3& get_min() const { return min; };
    inline const glm::vec3& get_max() const { return max; };
    void set_position(const glm::vec3& newPosition);
private:
    glm::vec3 size;
    glm::vec3 min{ 0.0f, 0.0f, 0.0f };
    glm::vec3 max{ 0.0f, 0.0f, 0.0f };
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };

    void update_min_and_max();
};
