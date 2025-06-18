/***************************************************************************************************
 * @file  Texture.cpp
 * @brief Implementation of the Texture class
 **************************************************************************************************/

#include "Texture.hpp"

#include <glad/glad.h>

Texture::Texture() : id(0) { }

Texture::Texture(const std::string& path)
    : id(0) {
    create(path);
}

Texture::Texture(const Image& image)
    : id(0) {
    create(image);
}

Texture::Texture(const vec3& color)
    : id(0) {
    create(color);
}

Texture::Texture(unsigned char r, unsigned char g, unsigned char b)
    : id(0) {
    create(r, g, b);
}

Texture::Texture(const Texture& texture) {
    id = texture.id;
}

Texture& Texture::operator=(const Texture& texture) {
    id = texture.id;
    return *this;
}

void Texture::init() {
    glGenTextures(1, &id);
    std::cout << "Created texture " << id << ".\n";
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::free() {
    if(id == 0) { return; }
    glDeleteTextures(1, &id);
    std::cout << "Freed texture " << id << ".\n";
    id = 0;
}

void Texture::create(const std::string& path) {
    create(Image(path));
}

void Texture::create(const Image& image) {
    init();

    unsigned int format = image.get_color_format();
    glTexImage2D(GL_TEXTURE_2D, 0, format,
                 image.get_width(), image.get_height(), 0,
                 format, GL_UNSIGNED_BYTE,
                 image.get_data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::create(const vec3& color) {
    create(static_cast<unsigned char>(color.x * 255.0f),
           static_cast<unsigned char>(color.y * 255.0f),
           static_cast<unsigned char>(color.z * 255.0f));
}

void Texture::create(unsigned char r, unsigned char g, unsigned char b) {
    init();
    unsigned char c[3]{ r, g, b };
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
