#pragma once

#include "vao.hpp"

#include <glad/glad.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    inline GLuint get_shader_program() const { return shader_program; };
private:
    VAO vao;
    GLuint shader_program;
};
