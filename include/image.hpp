#pragma once

#include <stb_image.h>
#include <string>

class Image {
public:
    Image(std::string fileName);
    ~Image();

    inline const stbi_uc* get_image_data() const { return image_data; };
    inline int get_width() const { return width; };
    inline int get_height() const { return height; };
private:
    stbi_uc* image_data;
    int width;
    int height;
};
