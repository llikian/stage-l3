/***************************************************************************************************
 * @file  Texture.hpp
 * @brief Declaration of the Texture class
 **************************************************************************************************/

#pragma once

#include <filesystem>

#include "cgltf.h"
#include "Image.hpp"
#include "maths/vec3.hpp"

/**
 * @brief Gets the format, channels amount and type corresponding to an OpenGL internal format enum.
 * @param internal_format The OpenGL internal format.
 * @param format The base format corresponding to the internal format.
 * @param channels_amount The amount of chanels corresponding to the internal format.
 * @param type The type corresponding to the internal format.
 */
void get_internal_format_parameters(int internal_format,
                                    unsigned int& format,
                                    unsigned int& channels_amount,
                                    unsigned int& type);

/**
 * @class Texture
 * @brief Creates a texture and assigns an image's data to it. Can then be bound.
 */
class Texture {
public:
    /**
     * @brief Default constructor. Initializes the texture id to 0, the default texture.
     */
    Texture();

    /**
     * @brief Copy constructor.
     * @warning The responsibility of freeing the texture goes to the user, so if multiple copies of
     * the same texture exist, be sure that all copies are no longer in use before freeing.
     * @param texture The texture to copy.
     */
    Texture(const Texture& texture);

    /**
     * @brief Copy operator.
     * @warning The responsibility of freeing the texture goes to the user, so if multiple copies of
     * the same texture exist, be sure that all copies are no longer in use before freeing.
     * Additionally, if this instance of the Texture class already had a texture (id != 0) and you
     * no longer wish to use that texture, be sure to call the free methodbeforehand.
     * @param texture The texture to copy.
     */
    Texture& operator=(const Texture& texture);

    /**
     * @brief Creates a texture.
     */
    void init();

    /**
     * @brief Deletes the texture.
     */
    void free();

    /**
     * @brief Creates a texture with the specified, width, height, data and format.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param width The texture's width.
     * @param height The texture's height.
     * @param data The texture's image data.
     * @param internal_format The image data's internal format.
     */
    void create(unsigned int width, unsigned int height, const void* data, int internal_format);

    /**
     * @brief Creates a texture by loading an image and assigning its data to a new texture.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param path The image's path.
     * @param flip_vertically Whether to flip the image on vertically.
     * @param srgb Whether to set the internal format to SRGB.
     */
    void create(const std::string& path, bool flip_vertically, bool srgb);

    /**
     * @brief Creates a texture by assigning an image's data to a new texture.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param image The image.
     * @param srgb Whether to set the internal format to SRGB.
     */
    void create(const Image& image, bool srgb);

    /**
     * @brief Creates a 1*1px texture with a specific color.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param color The color of the texture.
     */
    void create(const vec3& color);

    /**
     * @brief Creates a 1*1px texture with a specific color.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param r The r component of the texture's color.
     * @param g The g component of the texture's color.
     * @param b The b component of the texture's color.
     */
    void create(unsigned char r, unsigned char g, unsigned char b);

    /**
     * @brief Creates a texture using a GLTF texture view.
     * @warning The responsibility of freeing the texture goes to the user, so if this instance of
     * the Texture class already had an active texture (id != 0) and you no longer wish to use that
     * texture, be sure to call the free method beforehand.
     * @param parent_path The path to the folder where the GLTF file containing the texture is in.
     * @param texture_view The GLTF texture view that contains all the information regarding the
     * texture.
     * @param srgb Whether to set the internal format to SRGB.
     */
    void create(const std::filesystem::path& parent_path, const cgltf_texture_view& texture_view, bool srgb);

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

    /**
     * @return Whether the texture has transparency.
     */
    bool has_transparency() const;

private:
    unsigned int id;         ///< Texture id.
    bool b_has_transparency; ///< Whether the texture has transparency.
};
