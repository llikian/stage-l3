/***************************************************************************************************
 * @file  MRMaterial.hpp
 * @brief Declaration of the MRMaterial struct
 **************************************************************************************************/

#pragma once

#include "assets/Texture.hpp"
#include "Material.hpp"
#include "maths/vec4.hpp"

/**
 * @struct MRMaterial
 * @brief Metallic-Roughness material.
 */
struct MRMaterial : Material {
    explicit MRMaterial(const std::string& name);

    /**
     * @brief Updates a shader's uniforms' values with the material's data.
     * @param shader The shader whose uniforms need to be updated.
     */
    void update_shader_uniforms(const Shader* shader) const override;

    /**
     * @return Whether the material's base color or base color map have transparency.
     */
    bool has_transparency() const override;

    /**
     * @brief Add this material to the object editor.
     */
    void add_to_object_editor() override;

    vec4 base_color; ///< Diffuse albedo for dielectrics / Specular color for metals.
    Texture base_color_map; ///< Diffuse albedo for dielectrics / Specular color for metals.
    float metallic; ///< Whether a surface appears to be dielectric (0.0) or metallic (1.0). Usually a binary value.
    float roughness; ///< Perceived smoothness (1.0) or roughness (0.0).
    Texture metallic_roughness_map; ///< The green channel is a roughness map / The blue channel is a metallic map.
    float reflectance; ///< Fresnel reflectance at normal incidence angle (When view direction == normal).
    Texture normal_map;
};
