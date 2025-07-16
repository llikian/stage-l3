/***************************************************************************************************
 * @file  MRMaterial.cpp
 * @brief Implementation of the MRMaterial class
 **************************************************************************************************/

#include "mesh/MRMaterial.hpp"

MRMaterial::MRMaterial()
    : metallic(0.0f), // Dielectric
      roughness(0.5f),
      reflectance(0.5f) // Index of Refraction = 1.5f, 4% reflectance
{ }

bool MRMaterial::has_transparency() const {
    return base_color.w < 1.0f || base_color_map.has_transparency();
}
