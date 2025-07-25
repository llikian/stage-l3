/***************************************************************************************************
 * @file  AABB.hpp
 * @brief Declaration of the AABB struct
 **************************************************************************************************/

#pragma once

#include "Frustum.hpp"
#include "maths/Transform.hpp"

struct AABB {
    AABB();
    AABB(const vec3& min_point, const vec3& max_point);

    bool is_in_frustum(const Frustum& frustum, const mat4& model_matrix) const;
    mat4 get_global_model_matrix(const Transform& transform) const;

    void update(const vec3& min, const vec3& max);
    void update(const AABB& aabb, const Transform& transform);

    void set(const vec3& min, const vec3& max);

    vec4 min_point;
    vec4 max_point;
};
