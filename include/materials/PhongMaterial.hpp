/***************************************************************************************************
 * @file  PhongMaterial.hpp
 * @brief Declaration of the PhongMaterial struct
 **************************************************************************************************/

#pragma once

#include "assets/Shader.hpp"
#include "assets/Texture.hpp"
#include "Material.hpp"

/**
 * @class PhongMaterial
 * @brief Holds information about a material. Allows to specify how an object is rendered.
 */
struct PhongMaterial : Material {
    /**
     * @brief Sets default values for the material's data.
     * @param name The material's name.
     */
    explicit PhongMaterial(const std::string& name);

    /**
     * @brief Updates a shader's uniforms' values with the material's data.
     * @param shader The shader whose uniforms need to be updated.
     */
    void update_shader_uniforms(const Shader* shader) const override;

    /**
     * @return Whether any of the material's textures has transparency.
     */
    bool has_transparency() const override;

    /**
     * @brief Add this material to the object editor.
     */
    void add_to_object_editor() override;

    vec3 ambient;            ///< Ambient color.
    vec3 diffuse;            ///< Diffuse color.
    vec3 specular;           ///< Specular color
    float specular_exponent; ///< Specular Exponent.

    Texture diffuse_map; ///< Diffuse map.
};
