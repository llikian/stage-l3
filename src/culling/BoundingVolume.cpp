/***************************************************************************************************
 * @file  BoundingVolume.cpp
 * @brief Implementation of the BoundingVolume struct
 **************************************************************************************************/

#include "culling/BoundingVolume.hpp"

SphereVolume::SphereVolume() : center(0.0f, 0.0f, 0.0f), radius(1.0f) { }

SphereVolume::SphereVolume(const vec3& center, float radius) : center(center), radius(radius) { }

bool SphereVolume::is_in_frustum(const Frustum& frustum, const Transform& model_transform) {
    vec3 global_scale = model_transform.get_global_scale();
    float max_scale = std::max({ global_scale.x, global_scale.y, global_scale.z });
    SphereVolume global_sphere(model_transform.get_global_model() * vec4(center, 1.0f), radius * max_scale * 0.5f);

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
