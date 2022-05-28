#pragma once
#include <vector>

#include "opengl/sprite_gl.hpp"

namespace SpriteCollection {
    void insert_sprite(SpriteGL& s);
    const std::vector<SpriteGL>& get_sprites();  
}
