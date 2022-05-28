#pragma once

#include <glad/glad.h>
#include <string>

class ShaderGL {
public:
    ShaderGL(const std::string &fileName, GLenum shaderType);
    ~ShaderGL();

    inline const GLuint& get_shader() const { return shader; };
private:
    GLuint shader;
};
