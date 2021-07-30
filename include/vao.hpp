#pragma once

#include <glad/glad.h>

class VAO {
public:
    VAO();
    ~VAO();

    void bind();
private:
    GLuint vao;
};
