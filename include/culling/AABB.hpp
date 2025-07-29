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

    bool is_in_frustum(const Frustum& frustum) const;
    mat4 get_global_model_matrix() const;

    template <typename T1, typename T2>
    static void axis_aligned_min(T1& A, const T2& B) {
        A.x = std::min(A.x, B.x);
        A.y = std::min(A.y, B.y);
        A.z = std::min(A.z, B.z);
    }

    template <typename T1, typename T2>
    static void axis_aligned_max(T1& A, const T2& B) {
        A.x = std::max(A.x, B.x);
        A.y = std::max(A.y, B.y);
        A.z = std::max(A.z, B.z);
    }

    void set(const vec3& min, const vec3& max);
    void set(const AABB& aabb, const Transform& transform);

    vec4 min_point;
    vec4 max_point;
};
