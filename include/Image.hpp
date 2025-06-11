/***************************************************************************************************
 * @file  Image.hpp
 * @brief Declaration of the Image class
 **************************************************************************************************/

#pragma once

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
     */
    Image(const std::string& path);

    /**
     * @brief Frees all the allocated memory.
     */
    ~Image();

    /**
     * @brief Getter for the data member.
     * @return The data of the image.
     */
    const unsigned char* getData() const;

    /**
     * @brief Getter for the width member.
     * @return The width of the image.
     */
    unsigned int getWidth() const;

    /**
     * @brief Getter for the height member.
     * @return The height of the image.
     */
    unsigned int getHeight() const;

    /**
     * @brief Getter for the nbChannels member.
     * @return The number of channels of the image.
     */
    unsigned int getChannelsNumber() const;

private:
    unsigned char* data;     ///< The data of the image.
    unsigned int width;      ///< The width of the image.
    unsigned int height;     ///< The height of the image.
    unsigned int nbChannels; ///< The number of channels of the image.
};