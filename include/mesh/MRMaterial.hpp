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

    Texture albedo_map;
    Texture metallic_roughness_map;
    vec4 albedo;
    float metallic;
    float roughness;
    float fresnel0;
};
