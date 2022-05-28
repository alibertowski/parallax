#include <vector>

#include "sprite_collection.hpp"
#include "opengl/sprite_gl.hpp"

std::vector<SpriteGL> sprites;

void SpriteCollection::insert_sprite(SpriteGL& s) {
    sprites.push_back(s);
}

const std::vector<SpriteGL>& SpriteCollection::get_sprites() {
    return sprites;
}
