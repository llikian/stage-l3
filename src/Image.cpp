/***************************************************************************************************
 * @file  Image.cpp
 * @brief Implementation of the Image class
 **************************************************************************************************/

#include "Image.hpp"

#include <iostream>

#include "stb_image.h"

Image::Image(const std::string& path) {
    stbi_set_flip_vertically_on_load(true);

    int W, H, nbC;
    data = stbi_load(path.c_str(), &W, &H, &nbC, 3); // TODO : Maybe change 3 to 4 for transparency
    if(data == nullptr) { throw std::runtime_error("Couldn't load image '" + path + '\''); }

    width = W;
    height = H;
    nbChannels = nbC;
}

Image::~Image() {
    stbi_image_free(data);
}

const unsigned char* Image::getData() const {
    return data;
}

unsigned int Image::getWidth() const {
    return width;
}

unsigned int Image::getHeight() const {
    return height;
}

unsigned int Image::getChannelsNumber() const {
    return nbChannels;
}
