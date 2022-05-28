#include <unordered_map>

#include "sprite_collection.hpp"
#include "opengl/sprite_gl.hpp"
#include "image.hpp"
#include "opengl/gl_glfw.hpp"

SpriteGL::SpriteGL(Image i, glm::vec3 pos) {
    position = pos;
    SpriteCollection::insert_sprite(*this);
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i.get_width(), i.get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, i.get_image_data());
    glGenerateMipmap(GL_TEXTURE_2D);
}
