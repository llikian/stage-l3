/***************************************************************************************************
 * @file  Cubemap.cpp
 * @brief Implementation of the Cubemap class
 **************************************************************************************************/

#include "assets/Cubemap.hpp"

#include "assets/Image.hpp"
#include "glad/glad.h"

Cubemap::Cubemap(const std::initializer_list<std::filesystem::path>& paths) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    unsigned int i = 0;
    for(const std::filesystem::path& path : paths) {
        Image image(path, false);

        unsigned int width = image.get_width();
        unsigned int height = image.get_height();
        unsigned int format = image.get_internal_format(false);
        const unsigned char* data = image.get_data();
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        ++i;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &id);
}

void Cubemap::bind(unsigned int texUnit) const {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

unsigned int Cubemap::get_id() const {
    return id;
}
