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

bool AABB::is_in_frustum(const Frustum& frustum, const mat4& model_matrix) const {
    unsigned int planes[6]{ 0, 0, 0, 0, 0, 0 };

    mat4 mvp_matrix = frustum.view_projection * model_matrix;

    vec4 points[8]{
        mvp_matrix * vec4(min_point),
        mvp_matrix * vec4(min_point.x, min_point.y, max_point.z, 1.0f),
        mvp_matrix * vec4(min_point.x, max_point.y, min_point.z, 1.0f),
        mvp_matrix * vec4(min_point.x, max_point.y, max_point.z, 1.0f),
        mvp_matrix * vec4(max_point.x, min_point.y, min_point.z, 1.0f),
        mvp_matrix * vec4(max_point.x, min_point.y, max_point.z, 1.0f),
        mvp_matrix * vec4(max_point.x, max_point.y, min_point.z, 1.0f),
        mvp_matrix * vec4(max_point)
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

mat4 AABB::get_global_model_matrix(const Transform& transform) const {
    mat4 global_model = transform.get_global_model();
    vec4 min = global_model * min_point;
    vec4 max = global_model * max_point;

    vec3 center(0.5f * (min.x + max.x), 0.5f * (min.y + max.y), 0.5f * (min.z + max.z));

    return mat4(
        max.x - center.x, 0.0f, 0.0f, center.x,
        0.0f, max.y - center.y, 0.0f, center.y,
        0.0f, 0.0f, max.z - center.z, center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

void AABB::update(const vec3& min, const vec3& max) {
    min_point.x = std::min(min_point.x, min.x);
    min_point.y = std::min(min_point.y, min.y);
    min_point.z = std::min(min_point.z, min.z);

    max_point.x = std::max(max_point.x, max.x);
    max_point.y = std::max(max_point.y, max.y);
    max_point.z = std::max(max_point.z, max.z);
}


void AABB::update(const AABB& aabb, const Transform& transform) {
    const mat4 model = transform.get_global_model_const_reference();
    vec4 min = model * aabb.min_point;
    vec4 max = model * aabb.max_point;
    update(vec3(min) / min.w, vec3(max) / max.w);
}

void AABB::set(const vec3& min, const vec3& max) {
    min_point.x = min.x;
    min_point.y = min.y;
    min_point.z = min.z;

    max_point.x = max.x;
    max_point.y = max.y;
    max_point.z = max.z;
}
