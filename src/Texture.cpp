/***************************************************************************************************
 * @file  Texture.cpp
 * @brief Implementation of the Texture class
 **************************************************************************************************/

#include "Texture.hpp"

#include <glad/glad.h>

Texture::Texture() : id(0) { }

Texture::Texture(const std::string& path) : id(0) {
    create(path);
}

Texture::Texture(const Image& image) : id(0) {
    create(image);
}

Texture::Texture(const vec3& color) : id(0) {
    create(color);
}

Texture::Texture(unsigned char r, unsigned char g, unsigned char b) : id(0) {
    create(r, g, b);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::create(const std::string& path) {
    if(id != 0) { glDeleteTextures(1, &id); }

    Image image(path);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::create(const Image& image) {
    if(id != 0) { glDeleteTextures(1, &id); }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::create(const vec3& color) {
    if(id != 0) { glDeleteTextures(1, &id); }

    unsigned char c[3]{
        static_cast<unsigned char>(color.x * 255.0f),
        static_cast<unsigned char>(color.y * 255.0f),
        static_cast<unsigned char>(color.z * 255.0f)
    };

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, c);
}

void Texture::create(unsigned char r, unsigned char g, unsigned char b) {
    if(id != 0) { glDeleteTextures(1, &id); }

    unsigned char c[3]{ r, g, b };

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, c);
}

void Texture::bind(unsigned int texUnit) const {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture::is_default_texture() const {
    return id == 0;
}

unsigned int Texture::get_id() const {
    return id;
}
