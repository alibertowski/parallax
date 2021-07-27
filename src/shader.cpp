#include "shader.hpp"

#include <fstream>
#include <string>
#include <iterator>
#include <stdexcept>

Shader::Shader(std::string fileName) {
    constexpr char ShaderFileDirectory[] { "../res/shaders/opengl/" };
    std::string filePath{ ShaderFileDirectory + fileName };
    std::ifstream shaderFile{ filePath };
    if(!shaderFile.good()) {
        throw std::runtime_error{ "Failed to load shader file: " + filePath };
    }

    source_code = std::string{ std::istreambuf_iterator<char>{ shaderFile }, std::istreambuf_iterator<char>{} };
    shaderFile.close();
}
