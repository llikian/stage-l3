/***************************************************************************************************
 * @file  primitives.hpp
 * @brief Declaration of functions aimed at creating basic meshes such as cubes or spheres
 **************************************************************************************************/

#pragma once

#include "TriangleMesh.hpp"

/**
 * @brief Creates a mesh representing a sphere of radius 1.
 * @param mesh The mesh to create the sphere in.
 * @param horizontal_slices The amount of horizontal slices.
 * @param vertical_slices The amount of vertical slices.
 */
void create_sphere_mesh(TriangleMesh& mesh, unsigned int horizontal_slices, unsigned int vertical_slices);

/**
 * @brief Creates a mesh representing a quad as follow:
 * \verbatim
 * A---D
 * | / |
 * B---C
 * \endverbatim
 * To be clear this is just for visualisation's sake, the four points don't need to form a square,
 * but they do need to be coplanar which is why D is not passed as a parameter but calculated (since
 * three points are always coplanar in 3D space).
 * @param mesh The mesh to create the quad in.
 * @param A The upper left point of the quad.
 * @param B The lower left point of the quad.
 * @param C The lower right point of the quad.
 */
void create_quad_mesh(TriangleMesh& mesh, const vec3& A, const vec3& B, const vec3& C);
