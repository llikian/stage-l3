/***************************************************************************************************
 * @file  Framebuffer.cpp
 * @brief Implementation of the Framebuffer class
 **************************************************************************************************/

#include "Framebuffer.hpp"

Framebuffer::Framebuffer(unsigned int width, unsigned int height) : FBO(0), RBO(0), width(width), height(height) {
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    texture.create(width, height, nullptr, GL_RGBA, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.get_id(), 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Couldn't create framebuffer");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Framebuffer::~Framebuffer() {
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

unsigned int Framebuffer::get_texture_id() const {
    return texture.get_id();
}

void Framebuffer::bind_texture(unsigned int texture_unit) const {
    texture.bind(texture_unit);
}

vec2 Framebuffer::get_resolution() const {
    return vec2(width, height);
}

void Framebuffer::bind_default() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
