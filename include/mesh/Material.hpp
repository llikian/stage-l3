/***************************************************************************************************
 * @file  Material.hpp
 * @brief Declaration of the Material struct
 **************************************************************************************************/

#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

// TODO: Write and use different shaders depending on which material attributes are active

/**
 * @class Material
 * @brief
 */
struct Material {
    void update_shader_uniforms(const Shader& shader) const;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specular_exponent;

    Texture ambient_map;
    Texture diffuse_map;
};
