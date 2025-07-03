/***************************************************************************************************
 * @file  Frustum.hpp
 * @brief Declaration of the Frustum struct
 **************************************************************************************************/

#pragma once

#include "Camera.hpp"
#include "mesh/LineMesh.hpp"
#include "mesh/TriangleMesh.hpp"
#include "Plane.hpp"

/**
 * @struct Frustum
 * @brief
 */
struct Frustum {
    explicit Frustum(const Camera& camera);
    Frustum(const Camera& camera, LineMesh& lines, TriangleMesh& faces);

    void update(const Camera& camera);

    Plane near_plane;
    Plane far_plane;
    Plane top_plane;
    Plane bottom_plane;
    Plane left_plane;
    Plane right_plane;
};
