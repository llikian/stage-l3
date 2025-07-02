/***************************************************************************************************
 * @file  BoundingVolume.cpp
 * @brief Implementation of the BoundingVolume struct
 **************************************************************************************************/

#include "culling/BoundingVolume.hpp"

#include "maths/geometry.hpp"

bool BoundingVolume::is_in_frustum(const Frustum& frustum) const {
    return is_in_or_above_plane(frustum.left_plane) &&
           is_in_or_above_plane(frustum.right_plane) &&
           is_in_or_above_plane(frustum.top_plane) &&
           is_in_or_above_plane(frustum.bottom_plane) &&
           is_in_or_above_plane(frustum.near_plane) &&
           is_in_or_above_plane(frustum.far_plane);
}

SphereVolume::SphereVolume() : center(0.0f, 0.0f, 0.0f), radius(1.0f) { }

SphereVolume::SphereVolume(const vec3& center, float radius) : center(center), radius(radius) { }

SphereVolume SphereVolume::get_global_volumue(const Transform& transform) const {
    const vec3 global_scale = transform.get_global_scale();
    const float max_scale = std::max({ global_scale.x, global_scale.y, global_scale.z });
    return SphereVolume(transform.get_global_model() * vec4(center, 1.0f), radius * max_scale * 0.5f);
}

bool SphereVolume::is_in_frustum(const Frustum& frustum, const Transform& transform) const {
    return get_global_volumue(transform).BoundingVolume::is_in_frustum(frustum);
}

bool SphereVolume::is_in_or_above_plane(const Plane& plane) const {
    return plane.get_signed_distance(center) >= -radius * 2.0f;
}

mat4 SphereVolume::get_global_model_matrix(const Transform& transform) const {
    SphereVolume global_volume = get_global_volumue(transform);
    return mat4(
        global_volume.radius, 0.0f, 0.0f, global_volume.center.x,
        0.0f, global_volume.radius, 0.0f, global_volume.center.y,
        0.0f, 0.0f, global_volume.radius, global_volume.center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

AABB::AABB() : center(0.0f, 0.0f, 0.0f), extents(0.0f, 0.0f, 0.0f) { }

AABB::AABB(const vec3& min, const vec3& max)
    : center((max + min) / 2.0f),
      extents(max.x - center.x, max.y - center.y, max.z - center.z) { }

AABB::AABB(const vec3& center, float extent_x, float extent_y, float extent_z)
    : center(center),
      extents(extent_x, extent_y, extent_z) { }

AABB AABB::get_global_volumue(const Transform& transform) const {
    const vec3 front = extents.z * transform.get_front_vector();
    const vec3 right = extents.x * transform.get_right_vector();
    const vec3 up = extents.y * transform.get_up_vector();

    return AABB(transform.get_global_model() * vec4(center, 1.0f),
                std::abs(front.x) + std::abs(right.x) + std::abs(up.x),
                std::abs(front.y) + std::abs(right.y) + std::abs(up.y),
                std::abs(front.z) + std::abs(right.z) + std::abs(up.z));
}

bool AABB::is_in_frustum(const Frustum& frustum, const Transform& transform) const {
    return get_global_volumue(transform).BoundingVolume::is_in_frustum(frustum);
}

bool AABB::is_in_or_above_plane(const Plane& plane) const {
    return plane.get_signed_distance(center) >= -std::max({
               extents.x * std::abs(plane.normal.x),
               extents.y * std::abs(plane.normal.y),
               extents.z * std::abs(plane.normal.z)
           }) * 2.0f;
}

mat4 AABB::get_global_model_matrix(const Transform& transform) const {
    AABB global_volume = get_global_volumue(transform);
    return mat4(
        global_volume.extents.x, 0.0f, 0.0f, global_volume.center.x,
        0.0f, global_volume.extents.y, 0.0f, global_volume.center.y,
        0.0f, 0.0f, global_volume.extents.z, global_volume.center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}
