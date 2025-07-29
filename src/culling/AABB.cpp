/***************************************************************************************************
 * @file  AABB.cpp
 * @brief Implementation of the AABB struct
 **************************************************************************************************/

#include "culling/AABB.hpp"

#include <limits>

#include "maths/geometry.hpp"

AABB::AABB()
    : min_point(vec3(std::numeric_limits<float>::max()), 1.0f),
      max_point(vec3(std::numeric_limits<float>::lowest()), 1.0f) { }

AABB::AABB(const vec3& min_point, const vec3& max_point)
    : min_point(min_point, 1.0f),
      max_point(max_point, 1.0f) { }

bool AABB::is_in_frustum(const Frustum& frustum) const {
    unsigned int planes[6]{ 0, 0, 0, 0, 0, 0 };

    vec4 points[8]{
        frustum.view_projection * vec4(min_point.x, min_point.y, min_point.z, 1.0f),
        frustum.view_projection * vec4(min_point.x, min_point.y, max_point.z, 1.0f),
        frustum.view_projection * vec4(min_point.x, max_point.y, min_point.z, 1.0f),
        frustum.view_projection * vec4(min_point.x, max_point.y, max_point.z, 1.0f),
        frustum.view_projection * vec4(max_point.x, min_point.y, min_point.z, 1.0f),
        frustum.view_projection * vec4(max_point.x, min_point.y, max_point.z, 1.0f),
        frustum.view_projection * vec4(max_point.x, max_point.y, min_point.z, 1.0f),
        frustum.view_projection * vec4(max_point.x, max_point.y, max_point.z, 1.0f)
    };

    for(const vec4& point : points) {
        if(point.x < -point.w) { ++planes[0]; }
        if(point.x > point.w) { ++planes[1]; }
        if(point.y < -point.w) { ++planes[2]; }
        if(point.y > point.w) { ++planes[3]; }
        if(point.z < -point.w) { ++planes[4]; }
        if(point.z > point.w) { ++planes[5]; }
    }

    for(unsigned int points_above_plane : planes) {
        if(points_above_plane == 8) { return false; }
    }

    return true;
}

mat4 AABB::get_global_model_matrix() const {
    vec3 center = 0.5f * (min_point + max_point);

    return mat4(
        max_point.x - center.x, 0.0f, 0.0f, center.x,
        0.0f, max_point.y - center.y, 0.0f, center.y,
        0.0f, 0.0f, max_point.z - center.z, center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

void AABB::set(const vec3& min, const vec3& max) {
    min_point = min;
    max_point = max;
}

void AABB::set(const AABB& aabb, const Transform& transform) {
    const mat4& model = transform.get_global_model_const_reference();

    vec4 corners[8]{
        model * vec4(aabb.min_point.x, aabb.min_point.y, aabb.min_point.z, 1.0f),
        model * vec4(aabb.min_point.x, aabb.min_point.y, aabb.max_point.z, 1.0f),
        model * vec4(aabb.min_point.x, aabb.max_point.y, aabb.min_point.z, 1.0f),
        model * vec4(aabb.min_point.x, aabb.max_point.y, aabb.max_point.z, 1.0f),
        model * vec4(aabb.max_point.x, aabb.min_point.y, aabb.min_point.z, 1.0f),
        model * vec4(aabb.max_point.x, aabb.min_point.y, aabb.max_point.z, 1.0f),
        model * vec4(aabb.max_point.x, aabb.max_point.y, aabb.min_point.z, 1.0f),
        model * vec4(aabb.max_point.x, aabb.max_point.y, aabb.max_point.z, 1.0f)
    };

    min_point.x = min_point.y = min_point.z = std::numeric_limits<float>::max();
    max_point.x = max_point.y = max_point.z = std::numeric_limits<float>::lowest();

    for(const vec4& point : corners) {
        axis_aligned_min(min_point, point);
        axis_aligned_max(max_point, point);
    }
}
