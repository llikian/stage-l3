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

void get_internal_format_parameters(int internal_format, unsigned int& format, unsigned int& channels_amount,  unsigned int& type) {
    switch(internal_format) {
        case GL_RGB:
        case GL_RGB8:
        case GL_RGB16:
        case GL_RGB8_SNORM:
        case GL_RGB16_SNORM:
        case GL_RGB8UI:
        case GL_RGB16UI:
        case GL_RGB32UI:
        case GL_RGB8I:
        case GL_RGB16I:
        case GL_RGB32I:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_SRGB:
        case GL_SRGB8:
            format = GL_RGB;
            channels_amount = 3;
            break;
        case GL_RGBA:
        case GL_RGBA8:
        case GL_RGBA16:
        case GL_RGBA8_SNORM:
        case GL_RGBA16_SNORM:
        case GL_RGBA8UI:
        case GL_RGBA16UI:
        case GL_RGBA32UI:
        case GL_RGBA8I:
        case GL_RGBA16I:
        case GL_RGBA32I:
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_SRGB_ALPHA:
        case GL_SRGB8_ALPHA8:
            format = GL_RGBA;
            channels_amount = 4;
            break;
        case GL_RG:
        case GL_RG8:
        case GL_RG16:
        case GL_RG8_SNORM:
        case GL_RG16_SNORM:
        case GL_RG8UI:
        case GL_RG16UI:
        case GL_RG32UI:
        case GL_RG8I:
        case GL_RG16I:
        case GL_RG32I:
        case GL_RG16F:
        case GL_RG32F:
            format = GL_RG;
            channels_amount = 2;
            break;
        case GL_RED:
        case GL_R8:
        case GL_R16:
        case GL_R8_SNORM:
        case GL_R16_SNORM:
        case GL_R8UI:
        case GL_R16UI:
        case GL_R32UI:
        case GL_R8I:
        case GL_R16I:
        case GL_R32I:
        case GL_R16F:
        case GL_R32F:
            format = GL_RED;
            channels_amount = 1;
            break;
        default:
            std::cout << "[WARNING] Texture wasn't created: Format unknown.\n";
            return;
    }

    switch(internal_format) {
        case GL_RGBA:
        case GL_RGBA8:
        case GL_RGBA16:
        case GL_RGB:
        case GL_RGB8:
        case GL_RGB16:
        case GL_RG:
        case GL_RG8:
        case GL_RG16:
        case GL_RED:
        case GL_R8:
        case GL_R16:
        // TODO : Check if assuming unsigned char data for srgb is wrong
        case GL_SRGB:
        case GL_SRGB8:
        case GL_SRGB_ALPHA:
        case GL_SRGB8_ALPHA8:
            type = GL_UNSIGNED_BYTE;
            break;
        case GL_RGBA8_SNORM:
        case GL_RGBA16_SNORM:
        case GL_RGB8_SNORM:
        case GL_RGB16_SNORM:
        case GL_RG8_SNORM:
        case GL_RG16_SNORM:
        case GL_R8_SNORM:
        case GL_R16_SNORM:
            type = GL_SIGNED_NORMALIZED;
            break;
        case GL_RGBA8UI:
        case GL_RGBA16UI:
        case GL_RGBA32UI:
        case GL_RGB8UI:
        case GL_RGB16UI:
        case GL_RGB32UI:
        case GL_RG8UI:
        case GL_RG16UI:
        case GL_RG32UI:
        case GL_R8UI:
        case GL_R16UI:
        case GL_R32UI:
            type = GL_UNSIGNED_INT;
            break;
        case GL_RGBA8I:
        case GL_RGBA16I:
        case GL_RGBA32I:
        case GL_RGB8I:
        case GL_RGB16I:
        case GL_RGB32I:
        case GL_RG8I:
        case GL_RG16I:
        case GL_RG32I:
        case GL_R8I:
        case GL_R16I:
        case GL_R32I:
            type = GL_INT;
            break;
        case GL_RGBA16F:
        case GL_RGBA32F:
        case GL_RGB16F:
        case GL_RGB32F:
        case GL_RG16F:
        case GL_RG32F:
        case GL_R16F:
        case GL_R32F:
            type = GL_FLOAT;
            break;
        default:
            std::cout << "[WARNING] Texture wasn't created: Format unknown.\n";
            break;
    }
}

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
                     const void* data,
                     int internal_format) {
    init();
    bind();

    unsigned int format;
    unsigned int channels_amount;
    unsigned int type;
    get_internal_format_parameters(internal_format, format, channels_amount, type);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    b_has_transparency = false;
    if(data != nullptr && channels_amount == 4 && type == GL_UNSIGNED_BYTE) {
        const unsigned char* typed_data = static_cast<const unsigned char*>(data);
        for(unsigned int j = 0 ; j < height ; ++j) {
            for(unsigned int i = 0 ; i < width ; ++i) {
                if(typed_data[4 * (j * width + i) + 3] < 255) {
                    b_has_transparency = true;
                    return;
                }
            }
        }
    }
}

void Texture::create(const std::string& path, bool flip_vertically, bool srgb) {
    create(Image(path, flip_vertically), srgb);
}

void Texture::create(const Image& image, bool srgb) {
    create(image.get_width(), image.get_height(), image.get_data(), image.get_internal_format(srgb));
}

void Texture::create(const vec3& color) {
    unsigned char c[3]{
        static_cast<unsigned char>(color.x * 255.0f),
        static_cast<unsigned char>(color.y * 255.0f),
        static_cast<unsigned char>(color.z * 255.0f)
    };
    create(1, 1, c, GL_RGB);
}

void Texture::create(unsigned char r, unsigned char g, unsigned char b) {
    unsigned char color[3]{ r, g, b };
    create(1, 1, color, GL_RGB);
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
