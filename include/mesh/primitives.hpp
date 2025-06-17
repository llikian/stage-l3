/***************************************************************************************************
 * @file  primitives.hpp
 * @brief Declaration of functions aimed at creating basic meshes such as cubes or spheres
 **************************************************************************************************/

#pragma once

#include "TriangleMesh.hpp"

/**
 * @brief Creates a mesh for a sphere.
 * @param mesh The mesh to create the sphere in.
 * @param horizontal_slices The amount of horizontal slices.
 * @param vertical_slices The amount of vertical slices.
 */
void create_sphere_mesh(TriangleMesh& mesh, unsigned int horizontal_slices, unsigned int vertical_slices);
