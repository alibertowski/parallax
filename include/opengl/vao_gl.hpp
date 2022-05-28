#pragma once

#include <glad/glad.h>

class VAOGL {
public:
    VAOGL();
    ~VAOGL();

    void bind();
private:
    GLuint vao;
};
