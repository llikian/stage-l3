/***************************************************************************************************
 * @file  Texture.cpp
 * @brief Implementation of the Texture class
 **************************************************************************************************/

#include "Texture.hpp"

#include <glad/glad.h>

#include "AssetManager.hpp"

#ifdef DEBUG
#include "debug.hpp"
#endif

Texture::Texture() : id(0), b_has_transparency(false) { }

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
}

void Texture::free() {
    if(id == 0) { return; }
    glDeleteTextures(1, &id);
#ifdef DEBUG_LOG_TEXTURE_LIFETIME
    std::cout << "Freed texture " << id << ".\n";
#endif
    id = 0;
}

void Texture::create(unsigned int width,
                     unsigned int height,
                     const unsigned char* data,
                     unsigned int format,
                     bool srgb) {
    init();
    bind();

    int internal_format;
    unsigned int channels_amount;
    switch(format) {
        case GL_RGB:
            internal_format = srgb ? GL_SRGB8 : GL_RGB8;
            channels_amount = 3;
            break;
        case GL_RGBA:
            internal_format = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
            channels_amount = 4;
            break;
        case GL_RED:
            internal_format = GL_R8;
            channels_amount = 1;
            break;
        case GL_RG:
            internal_format = GL_RG8;
            channels_amount = 2;
            break;
        default:
            std::cout << "[WARNING] Texture wasn't created: Format unknown.\n";
            return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    b_has_transparency = false;
    if(data != nullptr && channels_amount == 4) {
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

void Texture::create(const std::string& path, bool flip_vertically, bool srgb) {
    Image image(path, flip_vertically);
    create(image.get_width(), image.get_height(), image.get_data(), image.get_color_format(), srgb);
}

void Texture::create(const Image& image, bool srgb) {
    create(image.get_width(), image.get_height(), image.get_data(), image.get_color_format(), srgb);
}

void Texture::create(const vec3& color) {
    unsigned char c[3]{
        static_cast<unsigned char>(color.x * 255.0f),
        static_cast<unsigned char>(color.y * 255.0f),
        static_cast<unsigned char>(color.z * 255.0f)
    };
    create(1, 1, c, GL_RGB, false);
}

void Texture::create(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char color[3]{ r, g, b };
    create(1, 1, color, GL_RGB, false);
}

void Texture::create(const std::filesystem::path& parent_path, const cgltf_texture_view& texture_view, bool srgb) {
    init();
    bind();

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

    if(texture->image->buffer_view != nullptr) {
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
    }

    if(texture->image->uri != nullptr) {
        *this = AssetManager::add_texture(parent_path / texture->image->uri, false, srgb);
    } else {
        // TODO implement loading textures without URIs.
        throw std::runtime_error("Didn't implement textures without URIs yet :p");
    }
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
