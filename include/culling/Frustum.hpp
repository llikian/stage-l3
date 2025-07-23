/***************************************************************************************************
 * @file  Frustum.hpp
 * @brief Declaration of the Frustum struct
 **************************************************************************************************/

#pragma once

#include "maths/mat4.hpp"

/**
 * @struct Frustum
 * @brief
 */
struct Frustum {
    mat4 view_projection;
};
