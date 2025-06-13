/***************************************************************************************************
 * @file  Texture.hpp
 * @brief Declaration of the Texture class
 **************************************************************************************************/

#pragma once

#include "Image.hpp"
#include "maths/vec3.hpp"

/**
 * @class Texture
 * @brief Creates a texture and assigns an image's data to it. Can then be bound.
 */
class Texture {
public:
    Texture();

    /**
     * @brief Constructs a texture by loading an image and assigning its data to a new texture.
     * @param path The image's path.
     */
    Texture(const std::string& path);

    /**
     * @brief Constructs a texture by assigning an image's data to a new texture.
     * @param image The image.
     */
    Texture(const Image& image);

    /**
     * @brief Constructs a 1*1px texture with a specific color.
     * @param color The color of the texture.
     */
    Texture(const vec3& color);

    /**
     * @brief Constructs a 1*1px texture with a specific color.
     * @param r The r component of the texture's color.
     * @param g The g component of the texture's color.
     * @param b The b component of the texture's color.
     */
    Texture(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Deletes the texture.
     */
    ~Texture();

    /**
     * @brief Creates a texture by loading an image and assigning its data to a new texture.
     * @param path The image's path.
     */
    void create(const std::string& path);

    /**
     * @brief Creates a texture by assigning an image's data to a new texture.
     * @param image The image.
     */
    void create(const Image& image);

    /**
     * @brief Creates a 1*1px texture with a specific color.
     * @param color The color of the texture.
     */
    void create(const vec3& color);

    /**
     * @brief Creates a 1*1px texture with a specific color.
     * @param r The r component of the texture's color.
     * @param g The g component of the texture's color.
     * @param b The b component of the texture's color.
     */
    void create(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Binds the texture to a specifc texture unit.
     * @param texUnit The opengl texture unit ID.
     */
    void bind(unsigned int texUnit = 0) const;

    /**
     * @return Whether the texture is the default texture (id == 0).
     */
    bool is_default_texture() const;

    /**
     * @return The texture's id.
     */
    unsigned int get_id() const;

private:
    unsigned int id; ///< Texture id.
};
