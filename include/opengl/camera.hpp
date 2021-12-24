#pragma once

#include "vao.hpp"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera2 {
public:
    void update_camera(GLuint program);
    inline bool get_projection_type() const { return is_orthographic; };
    inline void switch_projection(bool isOrthographic) { is_orthographic = isOrthographic; };
private:
    bool is_orthographic{ true };

    //glm::mat4 model{ 1.0f };
    glm::mat4 model{ glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 0.2f }) }; // TODO: Remove since this is used for objects individually
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f };
};
 