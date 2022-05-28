#pragma once

#include <string>
#include <glm/ext/vector_float3.hpp>

#include "image.hpp"
#include "opengl/gl_glfw.hpp"

class SpriteGL {
public:
    SpriteGL(Image i, glm::vec3 pos);

    inline const glm::vec3& get_position() const { return position; };
    inline GLuint get_texture() const { return texture; };
private:
    GLuint texture;
    glm::vec3 position;
};
