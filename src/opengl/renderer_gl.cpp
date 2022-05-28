#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl/renderer_gl.hpp"
#include "opengl/shader_gl.hpp"
#include "lowsystems/engine.hpp"
#include "opengl/sprite_gl.hpp"
#include "sprite_collection.hpp"

#ifndef NDEBUG
static void gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const
    GLchar *message, const void*) {
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}
#endif

RendererGL::RendererGL() {
    #ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    #endif
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    ShaderGL vertexShader{ "shader.vert", GL_VERTEX_SHADER };
    ShaderGL fragShader{ "shader.frag", GL_FRAGMENT_SHADER };
    glReleaseShaderCompiler();

    int success;
    char infoLog[512];

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader.get_shader());
    glAttachShader(shader_program, fragShader.get_shader());
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    float vertices[] = {
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,    // top right
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,    // bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,    // bottom left
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f     // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader_program);
}

RendererGL::~RendererGL() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader_program);
}

void RendererGL::render_frame() {
    glClear(GL_COLOR_BUFFER_BIT);

    cam.update_camera(shader_program);
    const std::vector<SpriteGL>& sprites{ SpriteCollection::get_sprites() };

    GLint modelLoc = glGetUniformLocation(shader_program, "model");
    const size_t kSpriteSize{ sprites.size() };
    for(size_t i{ 0 }; i < kSpriteSize; ++i) {
        glm::mat4 model{ glm::mat4{ 1.0f } };
        model = glm::scale(model, glm::vec3{ 50.0f, 50.0f, 0.0f });
        model = glm::translate(model, sprites[i].get_position());

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindTexture(GL_TEXTURE_2D, sprites[i].get_texture());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void RendererGL::enable_wireframe(bool enabled) {
    if (enabled && !is_wireframed) {
        is_wireframed = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if(!enabled && is_wireframed) {
        is_wireframed = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}