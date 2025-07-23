/***************************************************************************************************
 * @file  Framebuffer.hpp
 * @brief Declaration of the Framebuffer class
 **************************************************************************************************/

#pragma once

#include "assets/Texture.hpp"
#include "glad/glad.h"
#include "maths/vec2.hpp"

/**
 * @class Framebuffer
 * @brief
 */
class Framebuffer {
public:
    Framebuffer(unsigned int width, unsigned int height);

    ~Framebuffer();

    void bind() const;
    static void bind_default();

    unsigned int get_texture_id() const;
    void bind_texture(unsigned int texture_unit) const;

    vec2 get_resolution() const;

private:
    unsigned int FBO; ///< Frame Buffer Object.
    unsigned int RBO; ///< Rendering Buffer Object.
    Texture texture;  ///< The texture the framebuffer will render on.

    unsigned int width; ///< The width of the framebuffer's texture.
    unsigned int height; ///< The height of the framebuffer's texture.
};
