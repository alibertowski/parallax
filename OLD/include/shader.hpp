#pragma once

#include <glad/glad.h>
#include <string>

class Shader {
public:
    Shader(const std::string &fileName, GLenum shaderType);
    ~Shader();

    inline const GLuint& get_shader() const { return shader; };
private:
    GLuint shader;
};
