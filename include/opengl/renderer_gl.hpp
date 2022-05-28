#pragma once

#include <glad/glad.h>

#include "opengl/camera_gl.hpp"
#include "opengl/vao_gl.hpp"

class RendererGL {
public:
    RendererGL();
    ~RendererGL();

    void render_frame();
    void enable_wireframe(bool enabled);
private:
    VAOGL vao;
    GLuint shader_program;
    GLuint vbo;
    GLuint ebo;
    CameraGL cam;

    bool is_wireframed{ false };
};
