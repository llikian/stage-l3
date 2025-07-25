/***************************************************************************************************
 * @file  Frustum.hpp
 * @brief Declaration of the Frustum struct
 **************************************************************************************************/

#pragma once

#include "assets/Camera.hpp"
#include "maths/mat4.hpp"

/**
 * @struct Frustum
 * @brief
 */
struct Frustum {
    void update(const Camera& camera);

    mat4 view_projection;
    vec4 points[8];
};
