/***************************************************************************************************
 * @file  AABB.hpp
 * @brief Declaration of the AABB struct
 **************************************************************************************************/

#pragma once

#include "maths/Transform.hpp"

struct AABB {
    AABB();
    AABB(const vec3& min_point, const vec3& max_point);

    bool is_in_frustum(const mat4& mvp_matrix) const;
    mat4 get_global_model_matrix(const Transform& transform) const;

    vec4 points[8];
};
