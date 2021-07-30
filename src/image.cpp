#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION // Required in one source file for stb_image.h to work
#include <stb_image.h>
#include <iostream>

Image::Image(std::string fileName) {
    constexpr char ImageDirectory[]{ "../res/textures/" };
    std::string filePath{ ImageDirectory + fileName };

    int nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    image_data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    #ifndef NDEBUG
    if(!image_data) {
        std::cout << "Failed to load image: " << fileName << std::endl;
    }
    #endif
}

Image::~Image() {
    stbi_image_free(image_data);
}
