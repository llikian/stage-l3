/***************************************************************************************************
 * @file  Texture.cpp
 * @brief Implementation of the Texture class
 **************************************************************************************************/

#include "Texture.hpp"

#include <glad/glad.h>

#ifdef DEBUG
#include "debug.hpp"
#endif

Texture::Texture() : id(0), b_has_transparency(false) { }

Texture::Texture(const std::string& path)
    : id(0), b_has_transparency(false) {
    create(path);
}

Texture::Texture(const Image& image)
    : id(0), b_has_transparency(false) {
    create(image);
}

Texture::Texture(const vec3& color)
    : id(0), b_has_transparency(false) {
    create(color);
}

Texture::Texture(unsigned char r, unsigned char g, unsigned char b)
    : id(0), b_has_transparency(false) {
    create(r, g, b);
}

Texture::Texture(const Texture& texture) : id(texture.id), b_has_transparency(texture.has_transparency()) { }

Texture& Texture::operator=(const Texture& texture) {
    id = texture.id;
    b_has_transparency = texture.b_has_transparency;
    return *this;
}

void Texture::init() {
    glGenTextures(1, &id);
#ifdef DEBUG_LOG_TEXTURE_LIFETIME
    std::cout << "Created texture " << id << ".\n";
#endif
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::free() {
    if(id == 0) { return; }
    glDeleteTextures(1, &id);
#ifdef DEBUG_LOG_TEXTURE_LIFETIME
    std::cout << "Freed texture " << id << ".\n";
#endif
    id = 0;
}

void Texture::create(const std::string& path) {
    create(Image(path));
}

void Texture::create(const Image& image) {
    init();

    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    unsigned int format = image.get_color_format();
    const unsigned char* data = image.get_data();
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    b_has_transparency = false;
    if(image.get_channels_amount() == 4) {
        for(unsigned int j = 0 ; j < height ; ++j) {
            for(unsigned int i = 0 ; i < width ; ++i) {
                if(data[4 * (j * width + i) + 3] < 255) {
                    b_has_transparency = true;
                    return;
                }
            }
        }
    }
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
    b_has_transparency = false;
}

void Texture::create(const cgltf_texture_view& texture_view) {
    init();

    const cgltf_texture* texture = texture_view.texture;
    const cgltf_sampler* sampler = texture->sampler;

    switch(sampler->wrap_s) {
        case cgltf_wrap_mode_clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        case cgltf_wrap_mode_mirrored_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        case cgltf_wrap_mode_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
        default: break;
    }

    switch(sampler->wrap_t) {
        case cgltf_wrap_mode_clamp_to_edge:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        case cgltf_wrap_mode_mirrored_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case cgltf_wrap_mode_repeat:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        default: break;
    }

    switch(sampler->min_filter) {
        case cgltf_filter_type_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        case cgltf_filter_type_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        case cgltf_filter_type_nearest_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case cgltf_filter_type_linear_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case cgltf_filter_type_nearest_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case cgltf_filter_type_linear_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default: break;
    }

    switch(sampler->mag_filter) {
        case cgltf_filter_type_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        case cgltf_filter_type_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case cgltf_filter_type_nearest_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        case cgltf_filter_type_linear_mipmap_nearest:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        case cgltf_filter_type_nearest_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        case cgltf_filter_type_linear_mipmap_linear:
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        default: break;
    }

    switch(texture->image->buffer_view->type) {
        case cgltf_buffer_view_type_invalid:
            std::cout << "cgltf_buffer_view_type_invalid" << '\n';
            break;
        case cgltf_buffer_view_type_indices:
            std::cout << "cgltf_buffer_view_type_indices" << '\n';
            break;
        case cgltf_buffer_view_type_vertices:
            std::cout << "cgltf_buffer_view_type_vertices" << '\n';
            break;
        case cgltf_buffer_view_type_max_enum:
            std::cout << "cgltf_buffer_view_type_max_enum" << '\n';
            break;
    }

    // TODO : Finish writing this
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

bool Texture::has_transparency() const {
    return b_has_transparency;
}
