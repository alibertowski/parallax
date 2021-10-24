#include "renderer.hpp"
#include "shader.hpp"
#include "image.hpp"
#include "component_manager.hpp"
#include "camera.hpp"

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

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

static std::vector<float> get_cube_vertices() {
    constexpr int CubeVertexCount{ 30 };
    constexpr float minusXSide[CubeVertexCount] = {
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f
    };

    constexpr float plusXSide[CubeVertexCount] = {
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 1.0f
    };

    constexpr float minusYSide[CubeVertexCount] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f
    };

    constexpr float plusYSide[CubeVertexCount] = {
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    constexpr float minusZSide[CubeVertexCount] = {
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f
    };

    constexpr float plusZSide[CubeVertexCount] = {
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f
    };

    std::vector<float> updatedVertices{};
    constexpr float SideOffset{ 1.0f };
    constexpr int XCount{ 16 };
    constexpr int YCount{ 1 };
    constexpr int ZCount{ 10 };
    for (int x{ 0 }; x < XCount; ++x)
    {
        for (int y{ 0 }; y < YCount; ++y)
        {
            for (int z{ 0 }; z < ZCount; ++z)
            {
                if (x == 0) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = minusXSide[i];
                    }

                    newSide[1] += y * SideOffset; // Y
                    newSide[2] += z * SideOffset; // Z
                    newSide[6] += y * SideOffset; // Y
                    newSide[7] += z * SideOffset; // Z
                    newSide[11] += y * SideOffset; // Y
                    newSide[12] += z * SideOffset; // Z
                    newSide[16] += y * SideOffset; // Y
                    newSide[17] += z * SideOffset; // Z
                    newSide[21] += y * SideOffset; // Y
                    newSide[22] += z * SideOffset; // Z
                    newSide[26] += y * SideOffset; // Y
                    newSide[27] += z * SideOffset; // Z

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                } 
                
                if (x == XCount - 1) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = plusXSide[i];
                    }

                    newSide[0] += x * SideOffset; // X
                    newSide[1] += y * SideOffset; // Y
                    newSide[2] += z * SideOffset; // Z
                    newSide[5] += x * SideOffset; // X
                    newSide[6] += y * SideOffset; // Y
                    newSide[7] += z * SideOffset; // Z
                    newSide[10] += x * SideOffset; // X
                    newSide[11] += y * SideOffset; // Y
                    newSide[12] += z * SideOffset; // Z
                    newSide[15] += x * SideOffset; // X
                    newSide[16] += y * SideOffset; // Y
                    newSide[17] += z * SideOffset; // Z
                    newSide[20] += x * SideOffset; // X
                    newSide[21] += y * SideOffset; // Y
                    newSide[22] += z * SideOffset; // Z
                    newSide[25] += x * SideOffset; // X
                    newSide[26] += y * SideOffset; // Y
                    newSide[27] += z * SideOffset; // Z

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                }

                if (y == 0) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = minusYSide[i];
                    }

                    newSide[0] += x * SideOffset; // X
                    newSide[2] += z * SideOffset; // Z
                    newSide[5] += x * SideOffset; // X
                    newSide[7] += z * SideOffset; // Z
                    newSide[10] += x * SideOffset; // X
                    newSide[12] += z * SideOffset; // Z
                    newSide[15] += x * SideOffset; // X
                    newSide[17] += z * SideOffset; // Z
                    newSide[20] += x * SideOffset; // X
                    newSide[22] += z * SideOffset; // Z
                    newSide[25] += x * SideOffset; // X 
                    newSide[27] += z * SideOffset; // Z

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                } 
                
                if (y == YCount - 1) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = plusYSide[i];
                    }

                    newSide[0] += x * SideOffset; // X
                    newSide[1] += y * SideOffset; // Y
                    newSide[2] += z * SideOffset; // Z
                    newSide[5] += x * SideOffset; // X
                    newSide[6] += y * SideOffset; // Y
                    newSide[7] += z * SideOffset; // Z
                    newSide[10] += x * SideOffset; // X
                    newSide[11] += y * SideOffset; // Y
                    newSide[12] += z * SideOffset; // Z
                    newSide[15] += x * SideOffset; // X
                    newSide[16] += y * SideOffset; // Y
                    newSide[17] += z * SideOffset; // Z
                    newSide[20] += x * SideOffset; // X
                    newSide[21] += y * SideOffset; // Y
                    newSide[22] += z * SideOffset; // Z
                    newSide[25] += x * SideOffset; // X
                    newSide[26] += y * SideOffset; // Y
                    newSide[27] += z * SideOffset; // Z

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                }

                if (z == 0) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = minusZSide[i];
                    }

                    newSide[0] += x * SideOffset; // X
                    newSide[1] += y * SideOffset; // Y
                    newSide[5] += x * SideOffset; // X
                    newSide[6] += y * SideOffset; // Y
                    newSide[10] += x * SideOffset; // X
                    newSide[11] += y * SideOffset; // Y
                    newSide[15] += x * SideOffset; // X
                    newSide[16] += y * SideOffset; // Y
                    newSide[20] += x * SideOffset; // X
                    newSide[21] += y * SideOffset; // Y
                    newSide[25] += x * SideOffset; // X 
                    newSide[26] += y * SideOffset; // Y

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                } 
                
                if (z == ZCount - 1) {
                    float newSide[CubeVertexCount];
                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        newSide[i] = plusZSide[i];
                    }

                    newSide[0] += x * SideOffset; // X
                    newSide[1] += y * SideOffset; // Y
                    newSide[2] += z * SideOffset; // Z
                    newSide[5] += x * SideOffset; // X
                    newSide[6] += y * SideOffset; // Y
                    newSide[7] += z * SideOffset; // Z
                    newSide[10] += x * SideOffset; // X
                    newSide[11] += y * SideOffset; // Y
                    newSide[12] += z * SideOffset; // Z
                    newSide[15] += x * SideOffset; // X
                    newSide[16] += y * SideOffset; // Y
                    newSide[17] += z * SideOffset; // Z
                    newSide[20] += x * SideOffset; // X
                    newSide[21] += y * SideOffset; // Y
                    newSide[22] += z * SideOffset; // Z
                    newSide[25] += x * SideOffset; // X
                    newSide[26] += y * SideOffset; // Y
                    newSide[27] += z * SideOffset; // Z

                    for(int i{ 0 }; i < CubeVertexCount; ++i) {
                        updatedVertices.push_back(newSide[i]);
                    }
                }
            }
        }
    }

    return updatedVertices;
}

void Renderer::render_frame() {
    Camera* cam{ static_cast<Camera*>(ComponentManager::find_component("Camera")) };
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(cam->get_projection()));

    GLint viewLoc{ glGetUniformLocation(shader_program, "view") };
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam->calculate_view()));

    glm::mat4 model{ glm::mat4{ 1.0f } };
 //   model = glm::scale(model, glm::vec3{ 10.0f, 10.0f, 10.0f });
    GLint modelLoc{ glGetUniformLocation(shader_program, "model") };
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 207360);
    // for(int i{ 0 }; i < 16; ++i) {
    //     for(int j{ 0 }; j < 16; ++j) {
    //         for(int f{ 0 }; f < 100; ++f) {
    //             glm::mat4 model{ glm::mat4{ 1.0f } };
    //             model = glm::translate(model, glm::vec3{ static_cast<float>(i) * 5.0f, static_cast<float>(f) * 5.0f, static_cast<float>(j) * 5.0f });
    //             model = glm::scale(model, glm::vec3{ 5.0f, 5.0f, 5.0f });

    //             GLint modelLoc{ glGetUniformLocation(shader_program, "model") };
    //             glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    //             glDrawArrays(GL_TRIANGLES, 0, 36);
    //         }
    //     }
    // }
}

Renderer::Renderer() {
    #ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    #endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Shader vertexShader{ "shader.vert", GL_VERTEX_SHADER };
    Shader fragShader{ "shader.frag", GL_FRAGMENT_SHADER };
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

    std::vector<float> vertices{ get_cube_vertices() };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    //glGenBuffers(1, &ebo);

    vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  //  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(shader_program);
    glClearColor(0.301f, 0.294f, 0.384f, 1.0f);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps

    Image wallImage{ "dirt.jpg" };
    if (wallImage.get_image_data())
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallImage.get_width(), wallImage.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, wallImage.get_image_data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    //glDeleteBuffers(1, &vbo);
}

Renderer::~Renderer() {
    glDeleteProgram(shader_program);
}
