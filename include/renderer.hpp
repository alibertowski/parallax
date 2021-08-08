#pragma once

#include "vao.hpp"

#include <glad/glad.h>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void render_frame();
private:
    VAO vao;
    GLuint shader_program;
};
