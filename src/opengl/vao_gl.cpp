#include <glad/glad.h>

#include "opengl/vao_gl.hpp"

VAOGL::VAOGL() {
    glGenVertexArrays(1, &vao);
}

VAOGL::~VAOGL() {
    glDeleteVertexArrays(1, &vao);
}

void VAOGL::bind() {
    glBindVertexArray(vao);
}
