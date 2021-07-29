#include "shader.hpp"

#include <fstream>
#include <string>
#include <iterator>
#include <stdexcept>
#include <iostream>

Shader::Shader(const std::string &fileName, const GLenum &shaderType) {
    constexpr char ShaderFileDirectory[] { "../res/shaders/opengl/" };
    std::string filePath{ ShaderFileDirectory + fileName };
    std::ifstream shaderFile{ filePath };
    if(!shaderFile.good()) {
        throw std::runtime_error{ "Failed to load shader file: " + filePath };
    }

    std::string source_code { std::istreambuf_iterator<char>{ shaderFile }, std::istreambuf_iterator<char>{} };
    shaderFile.close();

    auto sourceCodeCStr { source_code.c_str() };
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation error: " << infoLog << '\n';
    }
}

Shader::~Shader() {
    glDeleteShader(shader);
}