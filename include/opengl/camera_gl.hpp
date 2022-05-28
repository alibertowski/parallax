#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraGL {
public:
    void update_camera(GLuint program);
    inline bool get_projection_type() const { return is_orthographic; };
    inline void switch_projection(bool isOrthographic) { is_orthographic = isOrthographic; };
private:
    bool is_orthographic{ true };

    glm::mat4 view{ 
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f)) };
    glm::mat4 projection;
};
