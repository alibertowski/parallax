#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/mat4x4.hpp>

class CoordinateFrame {
public:
    glm::mat4 get_matrix();
    inline const glm::vec3& get_position() const { return position; };
    inline const glm::vec3& get_look_vector() const { return look_vector; };
    inline const glm::vec3& get_right_vector() const { return right_vector; };
    inline const glm::vec3& get_up_vector() const { return up_vector; };
    inline void set_position(const glm::vec3& newPos) { position = newPos; }; 

    void set_look_vector(const glm::vec3& newLookVector);

    static constexpr glm::vec3 UpAxis{ 0.0f, 1.0f, 0.0f };
private:
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 look_vector;
    glm::vec3 right_vector;
    glm::vec3 up_vector;
};
