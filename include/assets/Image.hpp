/***************************************************************************************************
 * @file  Image.hpp
 * @brief Declaration of the Image class
 **************************************************************************************************/

#pragma once

#include <filesystem>
#include <string>

/**
 * @class Image
 * @brief Loads and holds data of an image.
 */
class Image {
public:
    /**
     * @brief Loads the image at the specified path.
     * @param path The path to the image.
     * @param flip_vertically Whether to flip the image on vertically.
     */
    explicit Image(const std::filesystem::path& path, bool flip_vertically = true);

    /**
     * @brief Frees all the allocated memory.
     */
    ~Image();

    /**
     * @return The data of the image.
     */
    const unsigned char* get_data() const;

    /**
     * @return The width of the image.
     */
    unsigned int get_width() const;

    /**
     * @return The height of the image.
     */
    unsigned int get_height() const;

    /**
     * @return The number of channels of the image.
     */
    unsigned int get_channels_amount() const;

    /**
     * @return The OpenGL internal format corresponding to the image's amount of channels.
     * @param srgb Whether to set the internal format to SRGB.
     */
    unsigned int get_internal_format(bool srgb) const;

private:
    unsigned char* data;          ///< The data of the image.
    unsigned int width;           ///< The width of the image.
    unsigned int height;          ///< The height of the image.
    unsigned int channels_amount; ///< The number of channels of the image.
};
