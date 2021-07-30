#include "vao.hpp"

#include <glad/glad.h>

VAO::VAO() {
    glGenVertexArrays(1, &vao);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao);
}

void VAO::bind() {
    glBindVertexArray(vao);
}
