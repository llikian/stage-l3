/***************************************************************************************************
 * @file  AABB.cpp
 * @brief Implementation of the AABB struct
 **************************************************************************************************/

#include "culling/AABB.hpp"

#include "maths/geometry.hpp"

AABB::AABB()
    : points(
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f, 0.0f, 0.0f, 1.0f)
    ) { }

AABB::AABB(const vec3& min_point, const vec3& max_point)
    : points(
        vec4(min_point, 1.0f),
        vec4(min_point.x, min_point.y, max_point.z, 1.0f),
        vec4(min_point.x, max_point.y, min_point.z, 1.0f),
        vec4(min_point.x, max_point.y, max_point.z, 1.0f),
        vec4(max_point.x, min_point.y, min_point.z, 1.0f),
        vec4(max_point.x, min_point.y, max_point.z, 1.0f),
        vec4(max_point.x, max_point.y, min_point.z, 1.0f),
        vec4(max_point, 1.0f)
    ) { }

bool AABB::is_in_frustum(const mat4& mvp_matrix) const {
    unsigned int planes[6]{ 0, 0, 0, 0, 0, 0 };

    for(const vec4& point : points) {
        vec4 p = mvp_matrix * point;
        if(p.x < -p.w) { ++planes[0]; }
        if(p.x > p.w) { ++planes[1]; }
        if(p.y < -p.w) { ++planes[2]; }
        if(p.y > p.w) { ++planes[3]; }
        if(p.z < -p.w) { ++planes[4]; }
        if(p.z > p.w) { ++planes[5]; }
    }

    for(unsigned int points_on_or_above_plane : planes) {
        if(points_on_or_above_plane == 8) { return false; }
    }

    return true;
}

mat4 AABB::get_global_model_matrix(const Transform& transform) const {
    mat4 global_model = transform.get_global_model();
    vec4 min = global_model * points[0];
    vec4 max = global_model * points[7];

    vec3 center(0.5f * (min.x + max.x), 0.5f * (min.y + max.y), 0.5f * (min.z + max.z));

    return mat4(
        max.x - center.x, 0.0f, 0.0f, center.x,
        0.0f, max.y - center.y, 0.0f, center.y,
        0.0f, 0.0f, max.z - center.z, center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
