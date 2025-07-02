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
 * @brief Holds information about a material. Allows to specify how an object is rendered.
 */
struct Material {
    /**
     * @brief Sets default values for the material's data.
     * @param name The material's name.
     */
    explicit Material(const std::string& name);

    /**
     * @brief Updates a shader's values with the material's data.
     * @param shader The shader whose uniforms need to be updated.
     */
    void update_shader_uniforms(const Shader& shader);

    /**
     * @return Whether any of the material's textures has transparency.
     */
    bool has_transparency() const;

    std::string name; ///< The material's name.

    vec3 ambient; ///< Ambient color.
    vec3 diffuse; ///< Diffuse color.
    vec3 specular; ///< Specular color
    float specular_exponent; ///< Specular Exponent.

    Texture diffuse_map; ///< Diffuse map.
};
