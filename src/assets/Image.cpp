/***************************************************************************************************
 * @file  Image.cpp
 * @brief Implementation of the Image class
 **************************************************************************************************/

#include "assets/Image.hpp"

#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

Image::Image(const std::filesystem::path& path, bool flip_vertically) {
    stbi_set_flip_vertically_on_load(flip_vertically);

    int w, h, c;
    data = stbi_load(path.string().c_str(), &w, &h, &c, 0);
    if(data == nullptr) { throw std::runtime_error("Couldn't load image '" + path.string() + '\''); }

    width = w;
    height = h;
    channels_amount = c;
}

Image::~Image() {
    stbi_image_free(data);
}

const unsigned char* Image::get_data() const {
    return data;
}

unsigned int Image::get_width() const {
    return width;
}

unsigned int Image::get_height() const {
    return height;
}

unsigned int Image::get_channels_amount() const {
    return channels_amount;
}

unsigned int Image::get_internal_format(bool srgb) const {
    switch(channels_amount) {
        case 3: return srgb ? GL_SRGB : GL_RGB;
        case 4: return srgb ? GL_SRGB_ALPHA : GL_RGBA;
        case 1: return GL_RED;
        case 2: return GL_RG;
        default:
            throw std::runtime_error(std::string("Format error, ") + std::to_string(channels_amount)
                                     + " channels in image.");
    }
}
