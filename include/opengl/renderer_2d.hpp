#pragma once

#include "opengl/camera.hpp"
#include "vao.hpp"

#include <glad/glad.h>

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    void render_frame();
private:
    VAO vao;
    GLuint shader_program;
    GLuint vbo;
    GLuint ebo;
    Camera2 cam;
};
