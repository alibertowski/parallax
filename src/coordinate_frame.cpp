#include "coordinate_frame.hpp"

#include <glm/mat4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>

glm::mat4 CoordinateFrame::get_matrix() {
    glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    translation[3][0] = position.x; // Third column, first row
    translation[3][1] = position.y;
    translation[3][2] = position.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = right_vector.x; // First column, first row
    rotation[1][0] = right_vector.y;
    rotation[2][0] = right_vector.z;
    rotation[0][1] = up_vector.x; // First column, second row
    rotation[1][1] = up_vector.y;
    rotation[2][1] = up_vector.z;
    rotation[0][2] = look_vector.x; // First column, third row
    rotation[1][2] = look_vector.y;
    rotation[2][2] = look_vector.z;

    return rotation * translation;
}

void CoordinateFrame::set_look_vector(const glm::vec3& newPos) {
    look_vector = newPos;
    right_vector = glm::normalize(glm::cross(CoordinateFrame::UpAxis, look_vector));
    up_vector = glm::cross(look_vector, right_vector);
}