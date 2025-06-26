/***************************************************************************************************
 * @file  Plane.hpp
 * @brief Declaration of the Plane struct
 **************************************************************************************************/

#pragma once

#include "maths/vec3.hpp"

/**
 * @struct Plane
 * @brief
 */
struct Plane {
    Plane();
    Plane(const vec3& normal, float distance);

    float get_signed_distance(const vec3& point) const;

    vec3 normal;    ///< The plane's normal.
    float distance; ///< Distance from the origin to the nearest point on the plane.
};
