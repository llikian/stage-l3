/***************************************************************************************************
 * @file  Material.hpp
 * @brief Declaration of the Material struct
 **************************************************************************************************/

#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

/**
 * @class Material
 * @brief
 */
struct Material {
    void update_shader_uniforms(const Shader& shader) const;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    Texture ambient_map;
    Texture diffuse_map;
};