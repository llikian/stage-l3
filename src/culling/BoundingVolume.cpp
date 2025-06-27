/***************************************************************************************************
 * @file  BoundingVolume.cpp
 * @brief Implementation of the BoundingVolume struct
 **************************************************************************************************/

#include "culling/BoundingVolume.hpp"

#include "maths/geometry.hpp"

SphereVolume::SphereVolume() : center(0.0f, 0.0f, 0.0f), radius(1.0f) { }

SphereVolume::SphereVolume(const vec3& center, float radius) : center(center), radius(radius) { }

bool SphereVolume::is_in_frustum(const Frustum& frustum, const Transform& transform) {
    const vec3 global_scale = transform.get_global_scale();
    const float max_scale = std::max({ global_scale.x, global_scale.y, global_scale.z });
    const SphereVolume global_sphere(transform.get_global_model() * vec4(center, 1.0f), radius * max_scale * 0.5f);

    return global_sphere.is_in_or_above_plane(frustum.left_plane) &&
           global_sphere.is_in_or_above_plane(frustum.right_plane) &&
           global_sphere.is_in_or_above_plane(frustum.top_plane) &&
           global_sphere.is_in_or_above_plane(frustum.bottom_plane) &&
           global_sphere.is_in_or_above_plane(frustum.near_plane) &&
           global_sphere.is_in_or_above_plane(frustum.far_plane);
}

bool SphereVolume::is_in_or_above_plane(const Plane& plane) const {
    return plane.get_signed_distance(center) > -radius;
}

AABB::AABB() : center(0.0f, 0.0f, 0.0f), extents(0.0f, 0.0f, 0.0f) { }

AABB::AABB(const vec3& min, const vec3& max)
    : center((max + min) / 2.0f),
      extents(max.x - center.x, max.y - center.y, max.z - center.z) { }

AABB::AABB(const vec3& center, float extent_x, float extent_y, float extent_z)
    : center(center),
      extents(extent_x, extent_y, extent_z) { }

bool AABB::is_in_frustum(const Frustum& frustum, const Transform& transform) {
    static const vec3 X_AXIS(1.0f, 0.0f, 0.0f);
    static const vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
    static const vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

    const vec3 front = extents.z * transform.get_front_vector();
    const vec3 right = extents.x * transform.get_right_vector();
    const vec3 up = extents.y * transform.get_up_vector();

    const AABB global_AABB(transform.get_global_model() * vec4(center, 1.0f),
                           std::abs(dot(X_AXIS, front) + std::abs(dot(X_AXIS, right)) + std::abs(dot(X_AXIS, up))),
                           std::abs(dot(Y_AXIS, front) + std::abs(dot(Y_AXIS, right)) + std::abs(dot(Y_AXIS, up))),
                           std::abs(dot(Z_AXIS, front) + std::abs(dot(Z_AXIS, right)) + std::abs(dot(Z_AXIS, up))));

    return global_AABB.is_in_or_above_plane(frustum.left_plane) &&
           global_AABB.is_in_or_above_plane(frustum.right_plane) &&
           global_AABB.is_in_or_above_plane(frustum.top_plane) &&
           global_AABB.is_in_or_above_plane(frustum.bottom_plane) &&
           global_AABB.is_in_or_above_plane(frustum.near_plane) &&
           global_AABB.is_in_or_above_plane(frustum.far_plane);
}

bool AABB::is_in_or_above_plane(const Plane& plane) const {
    float r = extents.x * std::abs(plane.normal.x)
              + extents.y * std::abs(plane.normal.y)
              + extents.z * std::abs(plane.normal.z);
    return plane.get_signed_distance(center) > -r;
}
