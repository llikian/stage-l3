/***************************************************************************************************
 * @file  Frustum.hpp
 * @brief Declaration of the Frustum struct
 **************************************************************************************************/

#pragma once

#include "Camera.hpp"
#include "Plane.hpp"
#include "mesh/LineMesh.hpp"
#include "mesh/TriangleMesh.hpp"

/**
 * @struct Frustum
 * @brief
 */
struct Frustum {
    Frustum(const Camera& camera, float aspect_ratio);
    Frustum(const Camera& camera, float aspect_ratio, LineMesh& lines, TriangleMesh& faces);

    void update(const Camera& camera, float aspect_ratio);

    Plane near_plane;
    Plane far_plane;
    Plane top_plane;
    Plane bottom_plane;
    Plane left_plane;
    Plane right_plane;
};
