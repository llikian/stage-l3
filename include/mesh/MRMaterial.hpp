/***************************************************************************************************
 * @file  MRMaterial.hpp
 * @brief Declaration of the MRMaterial class
 **************************************************************************************************/

#pragma once
#include "maths/vec4.hpp"
#include "Texture.hpp"

/**
 * @class MRMaterial
 * @brief Metallic-Roughness material.
 */
struct MRMaterial {
    MRMaterial();

    vec4 base_color; ///< Diffuse albedo for dielectrics / Specular color for metals.
    Texture base_color_map; ///< Diffuse albedo for dielectrics / Specular color for metals.
    float metallic; ///< Whether a surface appears to be dielectric (0.0) or metallic (1.0). Usually a binary value.
    float roughness; ///< Perceived smoothness (1.0) or roughness (0.0).
    Texture metallic_roughness_map; ///< The green channel is a roughness map / The blue channel is a metallic map.
    float fresnel0; ///< Fresnel coefficient at angle 0 (When the light direction is the same as the normal).
};
