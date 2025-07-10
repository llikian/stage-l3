/***************************************************************************************************
 * @file  MRMaterial.cpp
 * @brief Implementation of the MRMaterial class
 **************************************************************************************************/

#include "mesh/MRMaterial.hpp"

MRMaterial::MRMaterial()
    : metallic(0.0f), // Dielectric
      roughness(0.5f),
      fresnel0(0.04f) // Index of Refraction = 1.5f
{ }
