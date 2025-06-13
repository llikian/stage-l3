/***************************************************************************************************
 * @file  Image.cpp
 * @brief Implementation of the Image class
 **************************************************************************************************/

#include "Image.hpp"

#include <iostream>

#include "stb_image.h"

Image::Image(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    int w, h, c;
    data = stbi_load(path.c_str(), &w, &h, &c, 0);
    if(data == nullptr) { throw std::runtime_error("Couldn't load image '" + path + '\''); }

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
